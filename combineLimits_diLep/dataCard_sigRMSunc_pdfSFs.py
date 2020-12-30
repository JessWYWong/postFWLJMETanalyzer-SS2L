#!/usr/bin/env python

import os,sys,time,math,datetime,itertools
from ROOT import gROOT,TFile,TH1F

if 'CMSSW_10_2_10' in os.environ['CMSSW_BASE']:
        print "Go CMSENV inside CMSSW_10_2_13!"
        exit(1)

parent = os.path.dirname(os.getcwd())
thisdir= os.path.dirname(os.getcwd()+'/')
from utils import *
import CombineHarvester.CombineTools.ch as ch

gROOT.SetBatch(1)

tag = '_FRv5_PRv2_prefiring' ##Tag and saveKey are used for output directory names

fileDir = '/mnt/data/users/wwong/limits_ss2l/'
limitType = 'optimization_082020'
cutStr = 'Lep1Pt40_Lep2Pt30_HT1200_nConst4'
BRconfStr= 'bW0p0_tZ0p5_tH0p5'
subDir = 'Templates_rootfiles_Combine'
if len(sys.argv)>1 : limitType = sys.argv[1]
if len(sys.argv)>2 : cutStr = sys.argv[2]
if len(sys.argv)>3 : BRconfStr = sys.argv[3]
if len(sys.argv)>4 : subDir = sys.argv[4]
if len(sys.argv)>5 : tag = sys.argv[5]

saveKey = tag

fileDir += limitType+'/'+cutStr+'/'+subDir+'/'

#template = 'templates'+sys.argv[2]+'_' ##Change template to template directory. e.g.: templatesSR_......
template = 'templates'+'_'#+'SR'/'CR'

whichsignal = 'TT'
if 'tW' in BRconfStr: whichsignal = 'BB'
year = '2017'
if '2016' in limitType: year = '2016'
elif '2018' in limitType: year = '2018'

print'Tag = ',tag,', BR string = ',BRconfStr

def add_processes_and_observations(cb, prefix=whichsignal):
        print '------------------------------------------------------------------------'
	print '>> Creating processes and observations...'
	for chn in chns:
                print '>>>> \t Creating proc/obs for channel:',chn
		cats_chn = cats[chn]
		cb.AddObservations(  ['*'],  [prefix], [era], [chn],                 cats_chn      )
		cb.AddProcesses(     ['*'],  [prefix], [era], [chn], bkg_procs[chn], cats_chn, False  )
		cb.AddProcesses(     masses, [prefix], [era], [chn], sig_procs,      cats_chn, True   )


def add_shapes(cb, prefix=whichsignal):
        print '------------------------------------------------------------------------'
	print '>> Extracting histograms from input root files...'
	for chn in chns:
                print '>>>> \t Extracting histos for channel:',chn
		#CRbkg_pattern = 'HTNtag_'+lumiStr+'_%s$BIN__$PROCESS' % chn
		#SRbkg_pattern = discrim+'_'+lumiStr+'_%s$BIN__$PROCESS' % chn
		#CRsig_pattern = 'HTNtag_'+lumiStr+'_%s$BIN__$PROCESS$MASS' % chn
		#SRsig_pattern = discrim+'_'+lumiStr+'_%s$BIN__$PROCESS$MASS' % chn
		bkg_pattern = '%s$BIN__$PROCESS' % chn
		sig_pattern = '%s$BIN__$PROCESS$MASS' % chn
		print("pattern", bkg_pattern, sig_pattern)
		#if prefix=='BB': SRbkg_pattern = SRbkg_pattern.replace('Tprime','Bprime')
		#if prefix=='BB': SRsig_pattern = SRsig_pattern.replace('TTM','BBM').replace('Tprime','Bprime')
		if prefix=='BB': bkg_pattern = bkg_pattern.replace('Tprime','Bprime')
		if prefix=='BB': sig_pattern = sig_pattern.replace('TTM','BBM').replace('Tprime','Bprime')

		#if 'isCR' in chn: 
		#	cb.cp().channel([chn]).era([era]).backgrounds().ExtractShapes(rfile, CRbkg_pattern, CRbkg_pattern + '__$SYSTEMATIC')
		#	cb.cp().channel([chn]).era([era]).signals().ExtractShapes(rfile, CRsig_pattern, CRsig_pattern + '__$SYSTEMATIC')
		#elif 'isSR' in chn:
		#	cb.cp().channel([chn]).era([era]).backgrounds().ExtractShapes(rfile, SRbkg_pattern, SRbkg_pattern + '__$SYSTEMATIC')
		#	cb.cp().channel([chn]).era([era]).signals().ExtractShapes(rfile, SRsig_pattern, SRsig_pattern + '__$SYSTEMATIC')
		#else:
		cb.cp().channel([chn]).era([era]).backgrounds().ExtractShapes(rfile, bkg_pattern, bkg_pattern + '__$SYSTEMATIC')
		cb.cp().channel([chn]).era([era]).signals().ExtractShapes(rfile, sig_pattern, sig_pattern + '__$SYSTEMATIC')


def add_bbb(cb):
        ## This function is not used! autoMCstats in the card instead
	print '>> Merging bin errors and generating bbb uncertainties...'
	bbb = ch.BinByBinFactory()
	bbb.SetAddThreshold(0.1).SetMergeThreshold(0.5).SetFixNorm(False)
	
	for chn in chns:
		cb_chn = cb.cp().channel([chn])
		if 'CR' in chn:
			bbb.MergeAndAdd(cb_chn.cp().era([era]).bin_id([0,1,2,3]).process(bkg_procs[chn]), cb)
			bbb.MergeAndAdd(cb_chn.cp().era([era]).bin_id([0,1,2,3]).process(sig_procs), cb)
		else:
			bbb.MergeAndAdd(cb_chn.cp().era([era]).bin_id([0]).process(bkg_procs[chn]), cb)
			bbb.MergeAndAdd(cb_chn.cp().era([era]).bin_id([0]).process(sig_procs), cb)


def rename_and_write(cb):
        print '------------------------------------------------------------------------'
	print '>> Setting standardised bin names...'
	ch.SetStandardBinNames(cb)
	
	writer = ch.CardWriter('limits_'+template+saveKey+'/'+BRconfStr+'/$TAG/$MASS/$ANALYSIS_$CHANNEL_$BINID_$ERA.txt',
						   'limits_'+template+saveKey+'/'+BRconfStr+'/$TAG/common/$ANALYSIS_$CHANNEL.input.root')
	writer.SetVerbosity(1)
	writer.WriteCards('cmb', cb)
	for chn in chns:
                print '>>>> \t WriteCards for channel:',chn
		writer.WriteCards(chn, cb.cp().channel([chn]))
	print '>> Done writing cards!'


def print_cb(cb):
	for s in ['Obs', 'Procs', 'Systs', 'Params']:
		print '* %s *' % s
		getattr(cb, 'Print%s' % s)()
		print


def add_systematics(cb):
        print '------------------------------------------------------------------------'
	print '>> Adding systematic uncertainties...'

	signal = cb.cp().signals().process_set()
        print ( signal, allbkgs, chns, chnsMM, era)	

	cb.cp().process(['FakeRate']).channel(chns).AddSyst(cb, 'FakeRateSys', 'lnN', ch.SystMap('era')(['2017'], 1.106)(['2018'], 1.115)) 
	cb.cp().process(['ChargeMisID']).channel(chns).AddSyst(cb, 'ChargeMisIDSys', 'lnN', ch.SystMap('era')(['2017'], 1.314)(['2018'], 1.589)) 

        ## for MCs
        cb.cp().process(signal + allbkgs).channel(chns).AddSyst(cb, 'lumi$ERA', 'lnN', ch.SystMap('era')(['2016'], 1.025)(['2017'], 1.023)(['2018'], 1.025)) 

        ## Taking lepton SFs as uncorrelated, new calculations each year        
	cb.cp().process(signal + allbkgs).channel(chnsEE).AddSyst(cb, 'elIsoSys', 'lnN', ch.SystMap()(1.030))
	cb.cp().process(signal + allbkgs).channel(chnsEM).AddSyst(cb, 'elIsoSys', 'lnN', ch.SystMap()(1.015))
	cb.cp().process(signal + allbkgs).channel(chnsMM).AddSyst(cb, 'muIsoSys', 'lnN', ch.SystMap()(1.030))
	cb.cp().process(signal + allbkgs).channel(chnsEM).AddSyst(cb, 'muIsoSys', 'lnN', ch.SystMap()(1.015))
	cb.cp().process(signal + allbkgs).channel(chnsEE).AddSyst(cb, 'elRecoSys', 'lnN', ch.SystMap()(1.02))
	cb.cp().process(signal + allbkgs).channel(chnsEM).AddSyst(cb, 'elRecoSys', 'lnN', ch.SystMap()(1.01))

        cb.cp().process(['TTZ']).channel(chns).AddSyst(cb,  'pileup', 'lnN', ch.SystMap('era')(['2017'],(0.9909,1.00258))(['2018'],(1.00795,0.99165)))
        cb.cp().process(['TTW']).channel(chns).AddSyst(cb,  'pileup', 'lnN', ch.SystMap('era')(['2017'],(1.00334,0.99686))(['2018'],(0.99237,1.00655)))
        cb.cp().process(['TTH']).channel(chns).AddSyst(cb,  'pileup', 'lnN', ch.SystMap('era')(['2017'],(1.00419,0.99483))(['2018'],(1.00078,1.00029)))
        cb.cp().process(['TTTT']).channel(chns).AddSyst(cb, 'pileup', 'lnN', ch.SystMap('era')(['2017'],(1.00071,0.99872))(['2018'],(1.00121,0.9983)))
        cb.cp().process(['WZ']).channel(chns).AddSyst(cb,   'pileup', 'lnN', ch.SystMap('era')(['2017'],(1.01215,0.99852))(['2018'],(0.9575,1.02098)))
        cb.cp().process(['ZZ']).channel(chns).AddSyst(cb,   'pileup', 'lnN', ch.SystMap('era')(['2017'],(1.00842,1.00501))(['2018'],(0.5976,1.23798)))
        cb.cp().process(['WpWp']).channel(chns).AddSyst(cb, 'pileup', 'lnN', ch.SystMap('era')(['2017'],(1.00059,1.00236))(['2018'],(0.99841,0.99513)))
        cb.cp().process(['WWZ']).channel(chns).AddSyst(cb,  'pileup', 'lnN', ch.SystMap('era')(['2017'],(1.04185,0.98287))(['2018'],(0.94921,1.02958)))
        cb.cp().process(['WZZ']).channel(chns).AddSyst(cb,  'pileup', 'lnN', ch.SystMap('era')(['2017'],(0.94539,1.02134))(['2018'],(0.87232,1.13134)))
        if era == '2017' : cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'pileup', 'lnN', ch.SystMap('mass')(['1000'],(0.99661,1.00366))(['1100'],(1.00128,1.00139))(['1200'],(0.99445,1.00509))(['1300'],(0.99936,1.0018))(['1400'],(0.99183,1.00566))(['1500'],(1.00437,0.99958))(['1600'],(1.00154,0.99978))(['1700'],(1.00683,0.99562))(['1800'],(1.00164,1.00149)))
        if era == '2018' : cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'pileup', 'lnN', ch.SystMap('mass')(['1000'],(0.99986,1.00305))(['1100'],(0.99257,1.00803))(['1200'],(0.99888,0.99898))(['1300'],(1.00949,0.98955))(['1400'],(1.00031,1.00162))(['1500'],(0.99779,1.0002))(['1600'],(1.00078,1.00248))(['1700'],(1.00019,0.99995))(['1800'],(0.99677,0.99869)))
        if era == '2017' : cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'pileup', 'lnN', ch.SystMap('mass')(['900'],(1.003,0.9959))(['1000'],(1.00445,0.99911))(['1100'],(1.00335,1.00095))(['1200'],(0.99575,0.99963))(['1300'],(1.0012,0.99639))(['1400'],(0.98769,1.01279))(['1500'],(0.99807,1.00236))(['1600'],(0.99545,1.00375))(['1700'],(0.99979,1.00169))(['1800'],(0.99735,0.99936)))
        if era == '2018' : cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'pileup', 'lnN', ch.SystMap('mass')(['900'],(0.98848,1.01168))(['1000'],(1.00302,0.99763))(['1100'],(0.99308,1.00406))(['1200'],(1.00046,0.99546))(['1300'],(1.0048,0.99788))(['1400'],(0.99624,1.00219))(['1500'],(0.99828,1.00452))(['1600'],(0.99302,1.00415))(['1700'],(0.99312,1.01298))(['1800'],(0.99954,0.99945)))

        if era == '2017':
                cb.cp().process(['TTZ']).channel(chns).AddSyst(cb,  'prefire', 'lnN', ch.SystMap('era')(['2017'],(1.01037,0.98961)))
                cb.cp().process(['TTW']).channel(chns).AddSyst(cb,  'prefire', 'lnN', ch.SystMap('era')(['2017'],(1.00929,0.99067)))
                cb.cp().process(['TTH']).channel(chns).AddSyst(cb,  'prefire', 'lnN', ch.SystMap('era')(['2017'],(1.009,0.99082)))
                cb.cp().process(['TTTT']).channel(chns).AddSyst(cb, 'prefire', 'lnN', ch.SystMap('era')(['2017'],(1.01298,0.98679)))
                cb.cp().process(['WZ']).channel(chns).AddSyst(cb,   'prefire', 'lnN', ch.SystMap('era')(['2017'],(1.00816,0.99166)))
                cb.cp().process(['ZZ']).channel(chns).AddSyst(cb,   'prefire', 'lnN', ch.SystMap('era')(['2017'],(1.02391,0.97592)))
                cb.cp().process(['WpWp']).channel(chns).AddSyst(cb, 'prefire', 'lnN', ch.SystMap('era')(['2017'],(1.00945,0.9905)))
                cb.cp().process(['WWZ']).channel(chns).AddSyst(cb,  'prefire', 'lnN', ch.SystMap('era')(['2017'],(1.00694,0.99254)))
                cb.cp().process(['WZZ']).channel(chns).AddSyst(cb,  'prefire', 'lnN', ch.SystMap('era')(['2017'],(1.01429,0.98605)))
                if era == '2017' : cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'prefire', 'lnN', ch.SystMap('mass')(['1000'],(1.00808,0.9918))(['1100'],(1.0079,0.99202))(['1200'],(1.00759,0.9923))(['1300'],(1.00749,0.99237))(['1400'],(1.00778,0.99215))(['1500'],(1.00908,0.99088))(['1600'],(1.00722,0.9927))(['1700'],(1.0087,0.99124))(['1800'],(1.00732,0.9926)))
                if era == '2017' : cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'prefire', 'lnN', ch.SystMap('mass')(['900'],(1.00827,0.99168))(['1000'],(1.00818,0.99169))(['1100'],(1.00681,0.99308))(['1200'],(1.00731,0.99257))(['1300'],(1.00701,0.99292))(['1400'],(1.00869,0.9913))(['1500'],(1.0064,0.9935))(['1600'],(1.00645,0.9935))(['1700'],(1.00731,0.99256))(['1800'],(1.00742,0.99248)))

        cb.cp().process(['TTZ']).channel(chns).AddSyst(cb,  'TrigSys', 'lnN', ch.SystMap('era')(['2017'],(1.01918,0.98083))(['2018'],(1.01896,0.98104)))
        cb.cp().process(['TTW']).channel(chns).AddSyst(cb,  'TrigSys', 'lnN', ch.SystMap('era')(['2017'],(1.0199,0.98011))(['2018'],(1.01933,0.98068)))
        cb.cp().process(['TTH']).channel(chns).AddSyst(cb,  'TrigSys', 'lnN', ch.SystMap('era')(['2017'],(1.02011,0.9799))(['2018'],(1.01958,0.98043)))
        cb.cp().process(['TTTT']).channel(chns).AddSyst(cb, 'TrigSys', 'lnN', ch.SystMap('era')(['2017'],(1.0198,0.98021))(['2018'],(1.0192,0.98081)))
        cb.cp().process(['WZ']).channel(chns).AddSyst(cb,   'TrigSys', 'lnN', ch.SystMap('era')(['2017'],(1.01969,0.98032))(['2018'],(1.01887,0.98113)))
        cb.cp().process(['ZZ']).channel(chns).AddSyst(cb,   'TrigSys', 'lnN', ch.SystMap('era')(['2017'],(1.01735,0.98265))(['2018'],(1.02061,0.97939)))
        cb.cp().process(['WpWp']).channel(chns).AddSyst(cb, 'TrigSys', 'lnN', ch.SystMap('era')(['2017'],(1.01991,0.9801))(['2018'],(1.01916,0.98085)))
        cb.cp().process(['WWZ']).channel(chns).AddSyst(cb,  'TrigSys', 'lnN', ch.SystMap('era')(['2017'],(1.02136,0.97864))(['2018'],(1.01807,0.98193)))
        cb.cp().process(['WZZ']).channel(chns).AddSyst(cb,  'TrigSys', 'lnN', ch.SystMap('era')(['2017'],(1.01688,0.98312))(['2018'],(1.01731,0.98277)))
        if era == '2017' : cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'TrigSys', 'lnN', ch.SystMap('mass')(['1000'],(1.01996,0.98005))(['1100'],(1.01995,0.98006))(['1200'],(1.01989,0.98012))(['1300'],(1.02015,0.97985))(['1400'],(1.01999,0.98002))(['1500'],(1.02014,0.97986))(['1600'],(1.0201,0.97991))(['1700'],(1.02007,0.97993))(['1800'],(1.02022,0.97978)))
        if era == '2018' : cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'TrigSys', 'lnN', ch.SystMap('mass')(['1000'],(1.01941,0.9806))(['1100'],(1.01945,0.98056))(['1200'],(1.01935,0.98066))(['1300'],(1.01939,0.98062))(['1400'],(1.01938,0.98062))(['1500'],(1.01941,0.98059))(['1600'],(1.01972,0.98029))(['1700'],(1.01956,0.98045))(['1800'],(1.01966,0.98035)))
        if era == '2017' : cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'TrigSys', 'lnN', ch.SystMap('mass')(['900'],(1.01975,0.98026))(['1000'],(1.02004,0.97997))(['1100'],(1.01982,0.98019))(['1200'],(1.01993,0.98008))(['1300'],(1.01984,0.98016))(['1400'],(1.01998,0.98003))(['1500'],(1.02004,0.97997))(['1600'],(1.02009,0.97992))(['1700'],(1.02002,0.97999))(['1800'],(1.02011,0.9799)))
        if era == '2018' : cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'TrigSys', 'lnN', ch.SystMap('mass')(['900'],(1.01944,0.98057))(['1000'],(1.01937,0.98063))(['1100'],(1.01944,0.98056))(['1200'],(1.01957,0.98044))(['1300'],(1.01929,0.98072))(['1400'],(1.01938,0.98062))(['1500'],(1.01956,0.98045))(['1600'],(1.0194,0.9806))(['1700'],(1.01945,0.98056))(['1800'],(1.01946,0.98055)))

        cb.cp().process(['TTZ']).channel(chns).AddSyst(cb,  'lepIdSys', 'lnN', ch.SystMap('era')(['2017'],(0.94751,1.05411))(['2018'],(0.96435,1.0362)))
        cb.cp().process(['TTW']).channel(chns).AddSyst(cb,  'lepIdSys', 'lnN', ch.SystMap('era')(['2017'],(0.9547,1.04633))(['2018'],(0.96105,1.0397)))
        cb.cp().process(['TTH']).channel(chns).AddSyst(cb,  'lepIdSys', 'lnN', ch.SystMap('era')(['2017'],(0.96102,1.03975))(['2018'],(0.96684,1.03371)))
        cb.cp().process(['TTTT']).channel(chns).AddSyst(cb, 'lepIdSys', 'lnN', ch.SystMap('era')(['2017'],(0.95911,1.04187))(['2018'],(0.96424,1.03647)))
        cb.cp().process(['WZ']).channel(chns).AddSyst(cb,   'lepIdSys', 'lnN', ch.SystMap('era')(['2017'],(0.92404,1.07832))(['2018'],(0.95981,1.04098)))
        cb.cp().process(['ZZ']).channel(chns).AddSyst(cb,   'lepIdSys', 'lnN', ch.SystMap('era')(['2017'],(0.87766,1.12814))(['2018'],(0.96003,1.04078)))
        cb.cp().process(['WpWp']).channel(chns).AddSyst(cb, 'lepIdSys', 'lnN', ch.SystMap('era')(['2017'],(0.95153,1.0496))(['2018'],(0.95962,1.04122)))
        cb.cp().process(['WWZ']).channel(chns).AddSyst(cb,  'lepIdSys', 'lnN', ch.SystMap('era')(['2017'],(0.96565,1.03494))(['2018'],(0.96118,1.03954)))
        cb.cp().process(['WZZ']).channel(chns).AddSyst(cb,  'lepIdSys', 'lnN', ch.SystMap('era')(['2017'],(0.9471,1.05373))(['2018'],(0.96156,1.03925)))
        if era == '2017' : cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'lepIdSys', 'lnN', ch.SystMap('mass')(['1000'],(0.94904,1.05236))(['1100'],(0.95036,1.0509))(['1200'],(0.94673,1.05467))(['1300'],(0.9465,1.05497))(['1400'],(0.94774,1.05366))(['1500'],(0.94616,1.05533))(['1600'],(0.94396,1.05768))(['1700'],(0.94337,1.05829))(['1800'],(0.94227,1.0595)))
        if era == '2018' : cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'lepIdSys', 'lnN', ch.SystMap('mass')(['1000'],(0.96029,1.04052))(['1100'],(0.96056,1.0402))(['1200'],(0.95888,1.04198))(['1300'],(0.95986,1.04094))(['1400'],(0.96012,1.04071))(['1500'],(0.95965,1.04119))(['1600'],(0.95709,1.04383))(['1700'],(0.95352,1.04787))(['1800'],(0.95605,1.04502)))
        if era == '2017' : cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'lepIdSys', 'lnN', ch.SystMap('mass')(['900'],(0.94655,1.05486))(['1000'],(0.94906,1.05223))(['1100'],(0.94541,1.0561))(['1200'],(0.94647,1.05501))(['1300'],(0.94423,1.05736))(['1400'],(0.94198,1.05973))(['1500'],(0.94245,1.05925))(['1600'],(0.94378,1.05787))(['1700'],(0.93909,1.06273))(['1800'],(0.93756,1.06462)))
        if era == '2018' : cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'lepIdSys', 'lnN', ch.SystMap('mass')(['900'],(0.9549,1.04629))(['1000'],(0.96201,1.03873))(['1100'],(0.95999,1.0408))(['1200'],(0.96009,1.04074))(['1300'],(0.96007,1.04073))(['1400'],(0.95583,1.0451))(['1500'],(0.95854,1.04234))(['1600'],(0.95028,1.05149))(['1700'],(0.95484,1.04614))(['1800'],(0.94979,1.05131)))

        cb.cp().process(['TTZ']).channel(chns).AddSyst(cb,  'pdfNew', 'lnN', ch.SystMap('era')(['2017'],(0.987,1.014))(['2018'],(0.98769,1.01335)))
        cb.cp().process(['TTW']).channel(chns).AddSyst(cb,  'pdfNew', 'lnN', ch.SystMap('era')(['2017'],(0.985,1.015))(['2018'],(0.98274,1.01734)))
        cb.cp().process(['TTH']).channel(chns).AddSyst(cb,  'pdfNew', 'lnN', ch.SystMap('era')(['2017'],(0.813,1.188))(['2018'],(0.98772,1.01228)))
        cb.cp().process(['TTTT']).channel(chns).AddSyst(cb, 'pdfNew', 'lnN', ch.SystMap('era')(['2017'],(0.971,1.03))(['2018'],(0.97415,1.0262)))
        cb.cp().process(['WZ']).channel(chns).AddSyst(cb,   'pdfNew', 'lnN', ch.SystMap('era')(['2017'],(0.988,1.019))(['2018'],(0.98924,1.01076)))
        cb.cp().process(['ZZ']).channel(chns).AddSyst(cb,   'pdfNew', 'lnN', ch.SystMap('era')(['2017'],(0.989,1.011))(['2018'],(0.98948,1.01052)))
        cb.cp().process(['WpWp']).channel(chns).AddSyst(cb, 'pdfNew', 'lnN', ch.SystMap('era')(['2017'],(0.293,1.735))(['2018'],(0.27821,1.75126)))
        cb.cp().process(['WWZ']).channel(chns).AddSyst(cb,  'pdfNew', 'lnN', ch.SystMap('era')(['2017'],(0.789,1.211))(['2018'],(0.75726,1.82787)))
        cb.cp().process(['WZZ']).channel(chns).AddSyst(cb,  'pdfNew', 'lnN', ch.SystMap('era')(['2017'],(0.772,1.228))(['2018'],(0.92762,1.11056)))
        if era == '2017' : cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'pdfNew', 'lnN', ch.SystMap('mass')(['1000'],(0.99466,1.0044))(['1100'],(0.99621,1.00316))(['1200'],(0.99574,1.00473))(['1300'],(0.99445,1.0053))(['1400'],(0.99267,1.00634))(['1500'],(0.98988,1.00844))(['1600'],(0.99846,1.00178))(['1700'],(0.99022,1.00995))(['1800'],(0.99037,1.00842)))
        if era == '2018' : cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'pdfNew', 'lnN', ch.SystMap('mass')(['1000'],(0.99648,1.00264))(['1100'],(0.99476,1.00539))(['1200'],(0.99415,1.00588))(['1300'],(0.99523,1.00374))(['1400'],(0.99641,1.00286))(['1500'],(0.99309,1.00561))(['1600'],(0.99727,1.00254))(['1700'],(0.99041,1.00921))(['1800'],(0.99762,1.00375)))
        if era == '2017' : cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'pdfNew', 'lnN', ch.SystMap('mass')(['900'],(0.99666,1.00266))(['1000'],(0.9937,1.00528))(['1100'],(0.99565,1.00453))(['1200'],(0.98615,1.01432))(['1300'],(0.99226,1.00745))(['1400'],(0.99198,1.00782))(['1500'],(0.98143,1.01598))(['1600'],(0.99016,1.00848))(['1700'],(0.99125,1.00875))(['1800'],(1.00344,0.9979)))
        if era == '2018' : cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'pdfNew', 'lnN', ch.SystMap('mass')(['900'],(0.99388,1.00559))(['1000'],(0.99314,1.00572))(['1100'],(0.99363,1.00641))(['1200'],(0.99195,1.00797))(['1300'],(0.98135,1.02189))(['1400'],(0.98961,1.01091))(['1500'],(0.98235,1.01537))(['1600'],(0.98089,1.01786))(['1700'],(0.99131,1.00897))(['1800'],(0.98685,1.01556)))

        cb.cp().process(['TTZ']).channel(chns).AddSyst(cb,  'muRFcorrdNewTop', 'lnN', ch.SystMap('era')(['2017'],(0.891,1.073))(['2018'],(0.88402,1.08492)))
        cb.cp().process(['TTW']).channel(chns).AddSyst(cb,  'muRFcorrdNewTop', 'lnN', ch.SystMap('era')(['2017'],(0.819,1.181))(['2018'],(0.82007,1.18017)))
        cb.cp().process(['TTH']).channel(chns).AddSyst(cb,  'muRFcorrdNewTop', 'lnN', ch.SystMap('era')(['2017'],(0.798,1.258))(['2018'],(0.71414,1.54219)))
        cb.cp().process(['TTTT']).channel(chns).AddSyst(cb, 'muRFcorrdNewTop', 'lnN', ch.SystMap('era')(['2017'],(0.764,1.245))(['2018'],(0.75458,1.27419)))
        cb.cp().process(['WZ']).channel(chns).AddSyst(cb,   'muRFcorrdNewEwk', 'lnN', ch.SystMap('era')(['2017'],(0.85,1.194))(['2018'],(0.84408,1.20681)))
        cb.cp().process(['ZZ']).channel(chns).AddSyst(cb,   'muRFcorrdNewEwk', 'lnN', ch.SystMap('era')(['2017'],(0.956,1.058))(['2018'],(0.80417,1.26659)))
        cb.cp().process(['WpWp']).channel(chns).AddSyst(cb, 'muRFcorrdNewEwk', 'lnN', ch.SystMap('era')(['2017'],(1.0,1.16))(['2018'],(1.0,1.16037)))
        cb.cp().process(['WWZ']).channel(chns).AddSyst(cb,  'muRFcorrdNewEwk', 'lnN', ch.SystMap('era')(['2017'],(0.845,1.2))(['2018'],(0.84976,1.19424)))
        cb.cp().process(['WZZ']).channel(chns).AddSyst(cb,  'muRFcorrdNewEwk', 'lnN', ch.SystMap('era')(['2017'],(0.85,1.193))(['2018'],(0.83301,1.21823)))
        if era == '2017' : cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'muRFcorrdNewSig', 'lnN', ch.SystMap('mass')(['1000'],(0.99406,1.00629))(['1100'],(0.98981,1.00974))(['1200'],(0.9941,1.00568))(['1300'],(0.99141,1.00807))(['1400'],(0.9934,1.00644))(['1500'],(0.98743,1.01327))(['1600'],(0.99053,1.01082))(['1700'],(1.00108,0.99958))(['1800'],(0.96112,1.04283)))
        if era == '2018' : cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'muRFcorrdNewSig', 'lnN', ch.SystMap('mass')(['1000'],(0.99631,1.00396))(['1100'],(1.00034,0.99885))(['1200'],(0.997,1.00231))(['1300'],(0.99366,1.00711))(['1400'],(0.99634,1.00361))(['1500'],(0.98806,1.01241))(['1600'],(0.9885,1.01073))(['1700'],(0.98272,1.01959))(['1800'],(0.96298,1.04031)))
        if era == '2017' : cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'muRFcorrdNewSig', 'lnN', ch.SystMap('mass')(['900'],(0.99724,1.00262))(['1000'],(0.99589,1.00406))(['1100'],(0.9944,1.00605))(['1200'],(0.99524,1.00549))(['1300'],(0.99587,1.00476))(['1400'],(0.99336,1.00711))(['1500'],(0.995,1.00606))(['1600'],(0.98794,1.01341))(['1700'],(0.98522,1.01539))(['1800'],(0.98258,1.01933)))
        if era == '2018' : cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'muRFcorrdNewSig', 'lnN', ch.SystMap('mass')(['900'],(0.99323,1.00735))(['1000'],(0.99637,1.00361))(['1100'],(0.99448,1.00623))(['1200'],(0.99443,1.00651))(['1300'],(0.99047,1.00986))(['1400'],(0.99193,1.00877))(['1500'],(0.99204,1.00874))(['1600'],(0.98419,1.01726))(['1700'],(1.00473,0.99571))(['1800'],(0.92761,1.20214)))

	
        cb.cp().process(['TTZ']).channel(chns).AddSyst(cb,  'jec', 'lnN', ch.SystMap('era')(['2017'],(0.96125,1.03341))(['2018'],(0.94526,1.05126)))
        cb.cp().process(['TTW']).channel(chns).AddSyst(cb,  'jec', 'lnN', ch.SystMap('era')(['2017'],(0.96826,1.02954))(['2018'],(0.9556,1.0456)))
        cb.cp().process(['TTH']).channel(chns).AddSyst(cb,  'jec', 'lnN', ch.SystMap('era')(['2017'],(0.96625,1.03845))(['2018'],(0.95462,1.05895)))
        cb.cp().process(['TTTT']).channel(chns).AddSyst(cb, 'jec', 'lnN', ch.SystMap('era')(['2017'],(0.97464,1.02483))(['2018'],(0.96144,1.04133)))
        cb.cp().process(['WZ']).channel(chns).AddSyst(cb,   'jec', 'lnN', ch.SystMap('era')(['2017'],(0.97382,1.05236))(['2018'],(0.94118,1.05882)))
        cb.cp().process(['ZZ']).channel(chns).AddSyst(cb,   'jec', 'lnN', ch.SystMap('era')(['2017'],(1.0,1.14286))(['2018'],(1.0,1.0)))
        cb.cp().process(['WpWp']).channel(chns).AddSyst(cb, 'jec', 'lnN', ch.SystMap('era')(['2017'],(0.94828,1.04138))(['2018'],(0.96715,1.06569)))
        cb.cp().process(['WWZ']).channel(chns).AddSyst(cb,  'jec', 'lnN', ch.SystMap('era')(['2017'],(1.0,1.0))(['2018'],(1.0,1.1)))
        cb.cp().process(['WZZ']).channel(chns).AddSyst(cb,  'jec', 'lnN', ch.SystMap('era')(['2017'],(0.92857,1.0))(['2018'],(1.0,1.0625)))
        if era == '2017' : cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'jec', 'lnN', ch.SystMap('mass')(['1000'],(0.98737,1.01328))(['1100'],(0.98892,1.00894))(['1200'],(0.99036,1.00609))(['1300'],(0.99166,1.00426))(['1400'],(0.99217,1.00498))(['1500'],(0.99474,1.00401))(['1600'],(0.99411,1.00601))(['1700'],(0.99573,1.00596))(['1800'],(0.9945,1.00574)))
        if era == '2018' : cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'jec', 'lnN', ch.SystMap('mass')(['1000'],(0.98291,1.01553))(['1100'],(0.98692,1.01218))(['1200'],(0.9911,1.01083))(['1300'],(0.993,1.00864))(['1400'],(0.99369,1.00843))(['1500'],(0.99321,1.0068))(['1600'],(0.99134,1.00571))(['1700'],(0.99104,1.00835))(['1800'],(0.99223,1.00602)))
        if era == '2017' : cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'jec', 'lnN', ch.SystMap('mass')(['900'],(0.98333,1.01482))(['1000'],(0.98866,1.01579))(['1100'],(0.98511,1.01577))(['1200'],(0.99136,1.00668))(['1300'],(0.99398,1.00703))(['1400'],(0.9846,1.01175))(['1500'],(0.99396,1.0082))(['1600'],(0.99387,1.00486))(['1700'],(0.99436,1.0053))(['1800'],(0.9922,1.00574)))
        if era == '2018' : cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'jec', 'lnN', ch.SystMap('mass')(['900'],(0.97673,1.02057))(['1000'],(0.98791,1.01526))(['1100'],(0.98553,1.01581))(['1200'],(0.98834,1.01622))(['1300'],(0.986,1.01054))(['1400'],(0.98567,1.00998))(['1500'],(0.99198,1.01043))(['1600'],(0.9907,1.01234))(['1700'],(0.98172,1.01123))(['1800'],(0.99054,1.01034)))

        cb.cp().process(['TTZ']).channel(chns).AddSyst(cb,  'jer', 'lnN', ch.SystMap('era')(['2017'],(0.99065,1.00935))(['2018'],(0.99363,1.0029)))
        cb.cp().process(['TTW']).channel(chns).AddSyst(cb,  'jer', 'lnN', ch.SystMap('era')(['2017'],(0.99327,1.00522))(['2018'],(0.99747,1.00298)))
        cb.cp().process(['TTH']).channel(chns).AddSyst(cb,  'jer', 'lnN', ch.SystMap('era')(['2017'],(0.99487,1.00726))(['2018'],(0.99896,1.00365)))
        cb.cp().process(['TTTT']).channel(chns).AddSyst(cb, 'jer', 'lnN', ch.SystMap('era')(['2017'],(0.99821,1.00393))(['2018'],(0.99913,1.00095)))
        cb.cp().process(['WZ']).channel(chns).AddSyst(cb,   'jer', 'lnN', ch.SystMap('era')(['2017'],(1.00524,1.04188))(['2018'],(1.0,1.0)))
        cb.cp().process(['ZZ']).channel(chns).AddSyst(cb,   'jer', 'lnN', ch.SystMap('era')(['2017'],(0.92857,1.07143))(['2018'],(1.0,1.0)))
        cb.cp().process(['WpWp']).channel(chns).AddSyst(cb, 'jer', 'lnN', ch.SystMap('era')(['2017'],(0.96897,0.99655))(['2018'],(1.00365,1.0146)))
        cb.cp().process(['WWZ']).channel(chns).AddSyst(cb,  'jer', 'lnN', ch.SystMap('era')(['2017'],(1.0,1.0))(['2018'],(1.0,1.0)))
        cb.cp().process(['WZZ']).channel(chns).AddSyst(cb,  'jer', 'lnN', ch.SystMap('era')(['2017'],(1.0,1.0))(['2018'],(1.0,1.0)))
        if era == '2017' : cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'jer', 'lnN', ch.SystMap('mass')(['1000'],(0.99966,1.00317))(['1100'],(0.99682,1.00298))(['1200'],(0.99962,0.9987))(['1300'],(0.99561,0.99987))(['1400'],(0.99812,1.00229))(['1500'],(0.99851,1.00588))(['1600'],(0.99858,1.00145))(['1700'],(0.99957,1.00444))(['1800'],(0.99526,1.00259)))
        if era == '2018' : cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'jer', 'lnN', ch.SystMap('mass')(['1000'],(0.99909,0.99943))(['1100'],(0.99858,1.00307))(['1200'],(0.99879,1.00236))(['1300'],(0.99945,1.00172))(['1400'],(0.99891,1.00147))(['1500'],(1.00125,1.00098))(['1600'],(0.99933,1.00183))(['1700'],(0.99711,1.00194))(['1800'],(0.99903,1.00008)))
        if era == '2017' : cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'jer', 'lnN', ch.SystMap('mass')(['900'],(0.99759,1.00042))(['1000'],(1.00292,1.00168))(['1100'],(0.99559,1.00783))(['1200'],(0.99986,0.99959))(['1300'],(0.99941,1.00243))(['1400'],(1.00123,1.01057))(['1500'],(0.99808,1.00356))(['1600'],(0.99495,1.00421))(['1700'],(0.9975,1.00219))(['1800'],(0.99806,1.00033)))
        if era == '2018' : cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'jer', 'lnN', ch.SystMap('mass')(['900'],(0.9995,0.99904))(['1000'],(0.99616,0.99982))(['1100'],(0.99908,1.00098))(['1200'],(0.99839,1.00368))(['1300'],(0.99546,1.00207))(['1400'],(1.00128,1.00322))(['1500'],(1.00059,1.00541))(['1600'],(0.99888,1.00226))(['1700'],(0.99807,1.00043))(['1800'],(0.99933,0.99955)))


def add_autoMCstat(cb):
        print '------------------------------------------------------------------------'
	print '>> Adding autoMCstats...'
	
	thisDir = os.getcwd()
	mass=0
	massList = range(1000,1800+1,100)
	if whichsignal=='BB': massList = range(900,1800+1,100)

	for chn in chns+['cmb']:
                print '>>>> \t Adding autoMCstats for channel:',chn
		for mass in massList:
			chnDir = os.getcwd()+'/limits_'+template+saveKey+'/'+BRconfStr+'/'+chn+'/'+str(mass)+'/'
			print 'chnDir: ',chnDir
			os.chdir(chnDir)
			files = [x for x in os.listdir(chnDir) if '.txt' in x]
			for ifile in files:
				with open(chnDir+ifile, 'a') as chnfile: chnfile.write('* autoMCStats 1.')
			os.chdir(thisDir)

def create_workspace(cb):
        print '------------------------------------------------------------------------'
	print '>> Creating workspace...'

	#for chn in ['cmb']+chns:  ## do I really need individual workspaces? Not sure...
	for chn in ['cmb']:
                print '>>>> \t Creating workspace for channel:',chn
		chnDir = os.getcwd()+'/limits_'+template+saveKey+'/'+BRconfStr+'/'+chn+'/*'
		cmd = 'combineTool.py -M T2W -i '+chnDir+' -o workspace.root --parallel 4 --channel-masks'
		os.system(cmd)


def go(cb):
	add_processes_and_observations(cb)
	add_systematics(cb)
	add_shapes(cb)
	rename_and_write(cb)
	add_autoMCstat(cb)
	create_workspace(cb)


if __name__ == '__main__':
	cb = ch.CombineHarvester()
	
	era = year
	lumiStrDir = '35p867'
	if year=='2018': lumiStrDir = '59p74' #59p74 ?59p69
	if year=='2017': lumiStrDir = '41p557' #41p557?41p53
        lumiStr = lumiStrDir+'fb'

	if not os.path.exists('./limits_'+template+saveKey+'/'+BRconfStr): os.system('mkdir -p ./limits_'+template+saveKey+'/'+BRconfStr+'/')

        #discrim = 'STrebinnedv2'
	rfile = fileDir+'/templates_'+whichsignal+'_'+BRconfStr+'_Combine.root'

	print'File: ',rfile
	allbkgs = ['TTW','TTZ','TTH','TTTT','WZ','ZZ','WpWp','WZZ','WWZ']
	ddbkg = ['FakeRate','ChargeMisID']

	dataName = 'data_obs'
	tfile = TFile(rfile)
	allHistNames = [k.GetName() for k in tfile.GetListOfKeys() if not (k.GetName().endswith('Up') or k.GetName().endswith('Down'))]
	tfile.Close()
	chns = [hist[:hist.find('__')] for hist in allHistNames if '__'+dataName in hist]
	print(chns)

	chnsEE = [chn for chn in chns if 'elel' in chn]
        chnsEM = [chn for chn in chns if 'elmu' in chn]
        chnsMM = [chn for chn in chns if 'mumu' in chn]

	bkg_procs = {chn:[hist.split('__')[-1] for hist in allHistNames if chn+'__' in hist and not (hist.endswith('Up') or hist.endswith('Down') or hist.endswith(dataName) or 'TTM' in hist or 'BBM' in hist)] for chn in chns}
	for cat in sorted(bkg_procs.keys()):
		print cat,bkg_procs[cat]
	if 'qcd' in bkg_procs[cat]:
		print '		Removing qcd ...'
		bkg_procs[cat]=bkg_procs[cat][:-1]
	
	if whichsignal=='TT': sig_procs = ['TTM']
	elif whichsignal=='BB': sig_procs = ['BBM'] 	

	cats = {}
	for chn in chns: cats[chn] = [(0, '')]
	
	masses = ch.ValsFromRange('1000:1800|100')	
        if whichsignal=='BB': masses = ch.ValsFromRange('900:1800|100')
	print 'Found this mass list: ',masses

	go(cb)

