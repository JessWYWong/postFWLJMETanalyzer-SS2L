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

        cb.cp().process(['TTZ']).channel(chns).AddSyst(cb,  'pileup', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTW']).channel(chns).AddSyst(cb,  'pileup', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTH']).channel(chns).AddSyst(cb,  'pileup', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTTT']).channel(chns).AddSyst(cb, 'pileup', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WZ']).channel(chns).AddSyst(cb,   'pileup', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['ZZ']).channel(chns).AddSyst(cb,   'pileup', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WpWp']).channel(chns).AddSyst(cb, 'pileup', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WWZ']).channel(chns).AddSyst(cb,  'pileup', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WZZ']).channel(chns).AddSyst(cb,  'pileup', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'pileup', 'lnN', ch.SystMap('mass')(['1000'], (Up,Dn)))
        cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'pileup', 'lnN', ch.SystMap('mass')(['1000'], (Up,Dn)))

        if era == '2017':
                cb.cp().process(['TTZ']).channel(chns).AddSyst(cb,  'prefire', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn)))
                cb.cp().process(['TTW']).channel(chns).AddSyst(cb,  'prefire', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn)))
                cb.cp().process(['TTH']).channel(chns).AddSyst(cb,  'prefire', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn)))
                cb.cp().process(['TTTT']).channel(chns).AddSyst(cb, 'prefire', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn)))
                cb.cp().process(['WZ']).channel(chns).AddSyst(cb,   'prefire', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn)))
                cb.cp().process(['ZZ']).channel(chns).AddSyst(cb,   'prefire', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn)))
                cb.cp().process(['WpWp']).channel(chns).AddSyst(cb, 'prefire', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn)))
                cb.cp().process(['WWZ']).channel(chns).AddSyst(cb,  'prefire', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn)))
                cb.cp().process(['WZZ']).channel(chns).AddSyst(cb,  'prefire', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn)))
                cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'prefire', 'lnN', ch.SystMap('mass')(['1000'], (Up,Dn)))
                cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'prefire', 'lnN', ch.SystMap('mass')(['1000'], (Up,Dn)))

        cb.cp().process(['TTZ']).channel(chns).AddSyst(cb,  'TrigSys', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTW']).channel(chns).AddSyst(cb,  'TrigSys', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTH']).channel(chns).AddSyst(cb,  'TrigSys', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTTT']).channel(chns).AddSyst(cb, 'TrigSys', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WZ']).channel(chns).AddSyst(cb,   'TrigSys', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['ZZ']).channel(chns).AddSyst(cb,   'TrigSys', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WpWp']).channel(chns).AddSyst(cb, 'TrigSys', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WWZ']).channel(chns).AddSyst(cb,  'TrigSys', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WZZ']).channel(chns).AddSyst(cb,  'TrigSys', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'TrigSys', 'lnN', ch.SystMap('mass')(['1000'], (Up,Dn)))
        cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'TrigSys', 'lnN', ch.SystMap('mass')(['1000'], (Up,Dn)))

        cb.cp().process(['TTZ']).channel(chns).AddSyst(cb,  'lepIdSys', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTW']).channel(chns).AddSyst(cb,  'lepIdSys', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTH']).channel(chns).AddSyst(cb,  'lepIdSys', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTTT']).channel(chns).AddSyst(cb, 'lepIdSys', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WZ']).channel(chns).AddSyst(cb,   'lepIdSys', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['ZZ']).channel(chns).AddSyst(cb,   'lepIdSys', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WpWp']).channel(chns).AddSyst(cb, 'lepIdSys', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WWZ']).channel(chns).AddSyst(cb,  'lepIdSys', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WZZ']).channel(chns).AddSyst(cb,  'lepIdSys', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'lepIdSys', 'lnN', ch.SystMap('mass')(['1000'], (Up,Dn)))
        cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'lepIdSys', 'lnN', ch.SystMap('mass')(['1000'], (Up,Dn)))

        cb.cp().process(['TTZ']).channel(chns).AddSyst(cb,  'pdfNew', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTW']).channel(chns).AddSyst(cb,  'pdfNew', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTH']).channel(chns).AddSyst(cb,  'pdfNew', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTTT']).channel(chns).AddSyst(cb, 'pdfNew', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WZ']).channel(chns).AddSyst(cb,   'pdfNew', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['ZZ']).channel(chns).AddSyst(cb,   'pdfNew', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WpWp']).channel(chns).AddSyst(cb, 'pdfNew', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WWZ']).channel(chns).AddSyst(cb,  'pdfNew', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WZZ']).channel(chns).AddSyst(cb,  'pdfNew', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'pdfNew', 'lnN', ch.SystMap('mass')(['1000'], (Up,Dn)))
        cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'pdfNew', 'lnN', ch.SystMap('mass')(['1000'], (Up,Dn)))

        cb.cp().process(['TTZ']).channel(chns).AddSyst(cb,  'muRFcorrdNewTop', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTW']).channel(chns).AddSyst(cb,  'muRFcorrdNewTop', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTH']).channel(chns).AddSyst(cb,  'muRFcorrdNewTop', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTTT']).channel(chns).AddSyst(cb, 'muRFcorrdNewTop', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WZ']).channel(chns).AddSyst(cb,   'muRFcorrdNewEwk', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['ZZ']).channel(chns).AddSyst(cb,   'muRFcorrdNewEwk', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WpWp']).channel(chns).AddSyst(cb, 'muRFcorrdNewEwk', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WWZ']).channel(chns).AddSyst(cb,  'muRFcorrdNewEwk', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WZZ']).channel(chns).AddSyst(cb,  'muRFcorrdNewEwk', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'muRFcorrdNewSig', 'lnN', ch.SystMap('mass')(['1000'], (Up,Dn)))
        cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'muRFcorrdNewSig', 'lnN', ch.SystMap('mass')(['1000'], (Up,Dn)))

	
        cb.cp().process(['TTZ']).channel(chns).AddSyst(cb,  'jec', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTW']).channel(chns).AddSyst(cb,  'jec', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTH']).channel(chns).AddSyst(cb,  'jec', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTTT']).channel(chns).AddSyst(cb, 'jec', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WZ']).channel(chns).AddSyst(cb,   'jec', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['ZZ']).channel(chns).AddSyst(cb,   'jec', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WpWp']).channel(chns).AddSyst(cb, 'jec', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WWZ']).channel(chns).AddSyst(cb,  'jec', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WZZ']).channel(chns).AddSyst(cb,  'jec', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'jec', 'lnN', ch.SystMap('mass')(['1000'], (Up,Dn)))
        cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'jec', 'lnN', ch.SystMap('mass')(['1000'], (Up,Dn)))

        cb.cp().process(['TTZ']).channel(chns).AddSyst(cb,  'jer', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTW']).channel(chns).AddSyst(cb,  'jer', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTH']).channel(chns).AddSyst(cb,  'jer', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTTT']).channel(chns).AddSyst(cb, 'jer', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WZ']).channel(chns).AddSyst(cb,   'jer', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['ZZ']).channel(chns).AddSyst(cb,   'jer', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WpWp']).channel(chns).AddSyst(cb, 'jer', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WWZ']).channel(chns).AddSyst(cb,  'jer', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['WZZ']).channel(chns).AddSyst(cb,  'jer', 'lnN', ch.SystMap('era')(['2017'], (Up,Dn))(['2018'], (Up,Dn)))
        cb.cp().process(['TTM']).channel(chns).AddSyst(cb,  'jer', 'lnN', ch.SystMap('mass')(['1000'], (Up,Dn)))
        cb.cp().process(['BBM']).channel(chns).AddSyst(cb,  'jer', 'lnN', ch.SystMap('mass')(['1000'], (Up,Dn)))


def add_autoMCstat(cb):
        print '------------------------------------------------------------------------'
	print '>> Adding autoMCstats...'
	
	thisDir = os.getcwd()
	mass=0
	massList = range(1000,1800+1,100)
	if era == "2017": massList = [700]+massList
	elif era == "2018": massList = [900]+massList
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
	if era=="2017": masses = ['700']+masses
	elif era=="2018": masses = ['900']+masses
        if whichsignal=='BB': masses = ch.ValsFromRange('900:1800|100')
	print 'Found this mass list: ',masses

	go(cb)

