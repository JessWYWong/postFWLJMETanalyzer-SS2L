#!/usr/bin/python

import os,sys,time,math,pickle
from ROOT import *
from weights import *
from tdrStyle import *


gROOT.SetBatch(1)
start_time = time.time()


#templateDir='/user_data/rsyarif/kinematics_80x_condor_BLA'

DEBUG=False

#mainDir='/user_data/rsyarif/'
mainDir='./'#/mnt/data/users/wwong/'
templateDir=mainDir

templateDir+='kinematics_LJMet94x_3lepTT_2017datasets_2018_11_7_rizki_NoSYS_2016SFs_FRv3'


if len(sys.argv)>1: templateDir = sys.argv[1] 


lumi=59.74 #for plots

lumiInTemplates='59p74'

# sig='ttm800' # choose the 1st signal to plot
# sigleg='TT(0.8 TeV)'

sig='TTM1000'#'X53X53RHM1100'#'X53X53LHM1100'#'TTM1000' # choose the 1st signal to plot
sigleg='TTM1000'#'X_{5/3}#bar{X_{5/3}} (1.1TeV) - RH'#'#'X_{5/3}#bar{X_{5/3}} (1.1TeV) - LH'#'T#bar{T}(1 TeV)'

sigM1500='TTM1500'#'X53X53RHM1500'#'TTM1500' 
siglegM1500='TTM1500'#'X_{5/3}#bar{X_{5/3}} (1.5TeV) - RH'#'T#bar{T}(1.5 TeV)'

scaleSignals = True
dontShowSignalScaling = False

# scaleFact1 = 300
scaleFact1 = 1./0.1
scaleFact2 = 1./0.1
if not '0p1pb' in templateDir: 
	scaleFact1 = 1.0
	scaleFact2 = 1.0
#if 'Final' in templateDir: scaleFact1 = 40

#systematicList = ['pileup','prefire','btag','mistag','pdfNew','muRFcorrdNew','elPR','elFR','muPR','muFR','jec','jer','TrigEffWeight', 'elIdSys']
systematicList = ['pileup','pdfNew20172018','muRFcorrdNew','jec2018','jer2018','Trig18','elIdSF2018','NPWeightEl2018','NPWeightMu2018','NPWeightElPR2018','NPWeightMuPR2018']

#doAllSys = False
doAllSys = True

isRebinned='_Combine'#_rebinned_stat1p1'#post fix for file names if the name changed b/c of rebinning or some other process
doNormByBinWidth=False # not tested, may not work out of the box
doOneBand = False
if not doAllSys: doOneBand = True # Don't change this!
blind = False
yLog = True

if len(sys.argv)>2: blind = bool( int(sys.argv[2]) )  
if len(sys.argv)>3: yLog  = bool( int(sys.argv[3]) )
print 'bool(sys.argv[2]) =', bool(int(sys.argv[2])),  
print 'Blind = ', blind
print 'bool(sys.argv[3]) =', bool(int(sys.argv[3])),  
print 'yLog = ', yLog

#doRealPull = False
doRealPull = True
if doRealPull: doOneBand=False

totBkgTemp1 = {}
totBkgTemp2 = {}
totBkgTemp3 = {}

titlePretty = {
        'cleanAK4HT':'H_{T}^{lep} (GeV)',
        'Lep1Pt':'Lep1 p_{T} (GeV)',
        'Lep1Eta':'Lep1 #eta',
        'Lep2Pt':'Lep2 p_{T} (GeV)',
        'Lep2Eta':'Lep2 #eta',
        'cleanAK4Jet1Pt':'Jet1 p_{T} (GeV)',
        'cleanAK4Jet1Eta':'Jet1 #eta',
        'cleanAK4Jet2Pt':'Jet2 p_{T} (GeV)',
        'cleanAK4Jet2Eta':'Jet2 #eta',
        'nConst':'N_{Const}',
}

xsec ={}
xsec['TTM1000']  = 0.0440 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo
xsec['TTM1100']  = 0.0224 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo
xsec['TTM1200'] = 0.0118 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo
xsec['TTM1300']  = 0.00639 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo
xsec['TTM1400'] = 0.00354 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo
xsec['TTM1500']  = 0.00200 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo
xsec['TTM1600'] = 0.001148 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo
xsec['TTM1700']  = 0.000666 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo
xsec['TTM1800'] = 0.000391 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo
xsec['BBM900']   = 0.0903 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo
xsec['BBM1000']  = 0.0440 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo
xsec['BBM1100']  = 0.0224 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo
xsec['BBM1200'] = 0.0118 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo
xsec['BBM1300']  = 0.00639 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo
xsec['BBM1400'] = 0.00354 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo
xsec['BBM1500']  = 0.00200 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo
xsec['BBM1600'] = 0.001148 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo
xsec['BBM1700']  = 0.000666 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo
xsec['BBM1800'] = 0.000391 # from https://twiki.cern.ch/twiki/bin/view/CMS/B2GMonteCarlo

def formatUpperHist_v2(histogram,th1hist,discriminant):
	try: xTitle = titlePretty[discriminant]
	except: xTitle = ''
	histogram.GetXaxis().SetLabelSize(0)
	lowside = th1hist.GetBinLowEdge(1)
	highside = th1hist.GetBinLowEdge(th1hist.GetNbinsX()+1)
	histogram.GetXaxis().SetRangeUser(lowside,highside)
	print 'Name being formatted:',histogram.GetName()
	if blind == True:
		histogram.GetXaxis().SetLabelSize(0.05)
		histogram.GetXaxis().SetTitleSize(0.06)
		histogram.GetXaxis().SetTitle(xTitle)
		#histogram.GetXaxis().SetTitle('H_{T}^{lep}')
		histogram.GetYaxis().SetLabelSize(0.05)
		histogram.GetYaxis().SetTitleSize(0.06)
		histogram.GetYaxis().SetTitleOffset(1.2)
		histogram.GetXaxis().SetNdivisions(506)
	else:
		histogram.GetYaxis().SetLabelSize(0.05)
		histogram.GetYaxis().SetTitleSize(0.06)
		histogram.GetYaxis().SetTitleOffset(.78)

	if 'JetPt' in histogram.GetName() or 'JetEta' in histogram.GetName() or 'JetPhi' in histogram.GetName() or 'Pruned' in histogram.GetName() or 'Tau' in histogram.GetName(): histogram.GetYaxis().SetTitle("Jets")
# 	histogram.GetYaxis().CenterTitle()
	histogram.SetMinimum(2.01e-4)
# 	histogram.SetMinimum(0.001)
	if not yLog: 
		histogram.SetMinimum(0.25)
        if(DEBUG): print 'formatting for yLog'
	if yLog:
		uPad.SetLogy()
		histogram.SetMinimum(2.01e-3)#0.101)
		#histogram.SetMinimum(0.0401) #for EEE STrebinnedv2 bin=3 has yield ~0.04
		if not doNormByBinWidth: histogram.SetMaximum(200*histogram.GetMaximum())
		else: histogram.SetMaximum(50*histogram.GetMaximum())
	if(DEBUG): print 'complete formatUpperHist_v2'

def formatUpperHist(histogram,discriminant):
	try: xTitle = titlePretty[discriminant]
	except: xTitle = ''
	histogram.GetXaxis().SetLabelSize(0)

	if blind == True:
		histogram.GetXaxis().SetLabelSize(0.05)
		histogram.GetXaxis().SetTitleSize(0.06)
		histogram.GetXaxis().SetTitle(xTitle)
		#histogram.GetXaxis().SetTitle('H_{T}^{lep}')
		histogram.GetYaxis().SetLabelSize(0.05)
		histogram.GetYaxis().SetTitleSize(0.06)
		histogram.GetYaxis().SetTitleOffset(0.78)
		histogram.GetXaxis().SetNdivisions(506)
	else:
		histogram.GetYaxis().SetLabelSize(0.05)
		histogram.GetYaxis().SetTitleSize(0.06)
		histogram.GetYaxis().SetTitleOffset(.78)

	if 'JetPt' in histogram.GetName() or 'JetEta' in histogram.GetName() or 'JetPhi' in histogram.GetName() or 'Pruned' in histogram.GetName() or 'Tau' in histogram.GetName(): histogram.GetYaxis().SetTitle("Jets")
	histogram.GetYaxis().CenterTitle()
	histogram.SetMinimum(2.01e-4)
# 	histogram.SetMinimum(0.001)
	if not yLog: 
		histogram.SetMinimum(0.25)
	if yLog:
		uPad.SetLogy()
		histogram.SetMinimum(0.1)
		if not doNormByBinWidth: histogram.SetMaximum(200*histogram.GetMaximum())
		else: histogram.SetMaximum(50*histogram.GetMaximum())
		
def formatLowerHist(histogram,discriminant):
	try: xTitle = titlePretty[discriminant]
	except: xTitle = ''
	if blind == True:
		histogram.GetXaxis().SetLabelSize(.15)
		histogram.GetXaxis().SetTitleSize(0.18)
		histogram.GetXaxis().SetTitleOffset(0.95)
		histogram.GetXaxis().SetNdivisions(506)
		histogram.GetXaxis().SetTitle(xTitle)
		#histogram.GetXaxis().SetTitle("S_{T} (GeV)")
		#histogram.GetXaxis().SetTitle('H_{T}^{lep}')
		histogram.GetYaxis().SetLabelSize(0.15)
		histogram.GetYaxis().SetTitleSize(0.145)
		histogram.GetYaxis().SetTitleOffset(.3)
	else:
                histogram.GetXaxis().SetTitle(xTitle)
		histogram.GetXaxis().SetLabelSize(.15)
		histogram.GetXaxis().SetTitleSize(0.18)
		histogram.GetXaxis().SetTitleOffset(0.95)
		histogram.GetYaxis().SetLabelSize(0.15)
		histogram.GetYaxis().SetTitleSize(0.145)
		histogram.GetYaxis().SetTitleOffset(.3)
	if not doRealPull: histogram.GetYaxis().SetTitle('Data/Bkg')
	else: histogram.GetYaxis().SetTitle('#frac{(data-bkg)}{std. dev.}')
	histogram.GetYaxis().SetNdivisions(5)
# 	if doRealPull: histogram.GetYaxis().SetRangeUser(min(-2.99,0.8*histogram.GetBinContent(histogram.GetMaximumBin())),max(2.99,1.2*histogram.GetBinContent(histogram.GetMaximumBin())))
	if doRealPull: histogram.GetYaxis().SetRangeUser(-2.49,2.49)#-1.99,1.99)
	else: histogram.GetYaxis().SetRangeUser(0,1.99)
	#else: histogram.GetYaxis().SetRangeUser(0,2.99)
	histogram.GetYaxis().CenterTitle()

def normByBinWidth(result):
	result.SetBinContent(0,0)
	result.SetBinContent(result.GetNbinsX()+1,0)
	result.SetBinError(0,0)
	result.SetBinError(result.GetNbinsX()+1,0)
	
	for bin in range(1,result.GetNbinsX()+1):
		width=result.GetBinWidth(bin)
		content=result.GetBinContent(bin)
		error=result.GetBinError(bin)
		
		result.SetBinContent(bin, content/width)
		result.SetBinError(bin, error/width)

years = ['17','18']
lumiSys = {'17': 0.023, '18': 0.025}
normSystematics = {
        'muIdSys' : {'elel':0.00,'elmu':0.02,'mumu':0.04}, #flat rate of 2% 
        'elIsoSys': {'elel':0.030,'elmu':0.015,'mumu':0.00}, #flat rate of 1.5% for el
        'muIsoSys': {'elel':0.00,'elmu':0.015,'mumu':0.030}, #flat rate of 1.5% for mu
        'elRecoSys':{'elel':0.02,'elmu':0.01,'mumu':0.01}, #flat rate of 1% for el
}
ddbkgSystematics = {
        '17': { 'FR2LSys': 0.0, 'CMIDSys': 0.345 },
        '18': { 'FR2LSys': 0.0, 'CMIDSys': 0.600 }
        #'17': { 'FR2LSys': 0.106, 'CMIDSys': 0.345 },
        #'18': { 'FR2LSys': 0.115, 'CMIDSys': 0.600 }
}

def getNormUnc(hist,ibin,cat,yr):
	contentsquared = hist.GetBinContent(ibin)**2
	corrdSysSq = lumiSys[yr]**2+ normSystematics['muIdSys'][cat]**2 + normSystematics['elIsoSys'][cat]**2 + normSystematics['muIsoSys'][cat]**2 + normSystematics['elRecoSys'][cat]**2
	#corrdSysSq = lumiSys**2 + trigSys**2 + normSystematics['elIdSys'][cat]**2 + normSystematics['muIdSys'][cat]**2 + normSystematics['elIsoSys'][cat]**2 + normSystematics['muIsoSys'][cat]**2 
	error = corrdSysSq * contentsquared  #correlated uncertainties
	#if 'top' in hist.GetName(): error += topXsecSys*topXsecSys*contentsquared # cross section
	#if 'ewk' in hist.GetName(): error += ewkXsecSys*ewkXsecSys*contentsquared # cross section
	#if 'qcd' in hist.GetName(): error += qcdXsecSys*qcdXsecSys*contentsquared # cross section
	return error

def getDDBKGNormUnc(hist,ibin,cat,yr):
	contentsquared = hist.GetBinContent(ibin)**2
	error = (ddbkgSystematics[yr]['FR2LSys'][cat]**2) * contentsquared  #correlated uncertainties
	return error

def getCMIDNormUnc(hist,ibin,cat,yr):
        contentsquared = hist.GetBinContent(ibin)**2
        error = (ddbkgSystematics[yr]['CMIDSys'][cat]**2) * contentsquared  #correlated uncertainties
        return error

def poissonNormByBinWidth(tgae,hist):
	alpha = 1. - 0.6827
	for ibin in range(0,tgae.GetN()):
		width = float(hist.GetBinWidth(ibin+1))
		X = tgae.GetX()[ibin]
		N = tgae.GetY()[ibin]
		L = 0
		if N != 0: L = Math.gamma_quantile(alpha/2.,N,1.)
		U = Math.gamma_quantile_c(alpha/2.,N+1,1)
		tgae.SetPoint(ibin,X,N/width)
		tgae.SetPointEYlow(ibin,(N-L)/width)
		tgae.SetPointEYhigh(ibin,(U-N)/width)

def poissonErrors(tgae,discriminant):
	alpha = 1. - 0.6827
	for ibin in range(0,tgae.GetN()):
		N = tgae.GetY()[ibin]
		L = 0
		if N != 0: L = Math.gamma_quantile(alpha/2.,N,1.)
		U = Math.gamma_quantile_c(alpha/2.,N+1,1)
		tgae.SetPointEYlow(ibin,N-L)
		tgae.SetPointEYhigh(ibin,U-N)

		#if (discriminant=="lepPt" or discriminant=="STrebinnedv2") and (ibin==0 or ibin==1): #manually kill unc in first two bins.
		#	tgae.SetPointEYlow(ibin,0)
		#	tgae.SetPointEYhigh(ibin,0)


plotList = [#distribution name as defined in "doHists.py"
	'',
	'nConst',
        'cleanAK4HT',
        'Lep1Pt',
	'Lep1Eta',
	'cleanAK4Jet1Pt',
	'cleanAK4Jet1Eta',
# # 	'ElDxy',
# # 	'ElDz',
# # 
# # 	'MuDxy',
# # 	'MuDz',
# 	
 	#'NPV',
# 
	#'lepPt',
	#'lepPtRebinned',
	#'ElPt',
	#'MuPt',
	#'lep1Pt',
	#'lep2Pt',
	#'lep3Pt',
	#'lepEta',
	#'ElEta',
	#'MuEta',
	#'lep1Eta',
	#'lep2Eta',
	#'lep3Eta',
	#'Nlep',
	#'Nel',
	#'Nmu',

	#'JetEta',
 	#'Jet1Eta',
 	#'Jet2Eta',
	#'JetPt' ,
 	#'Jet1Pt' ,
 	#'Jet2Pt' ,
# 
# 
 	#'HT',
# # 	'ST',
 	#'MET',
 	#'HTrebinned',
# 	'ST',
# # 	'STrebinned',
# 
	#'STrebinnedv2',
# 
# # 	'STrebinnedv3',
 	#'METrebinned',
# 
	#'NJets' ,
 	#'NBJets',
# 	#'NBJetsCorr',
# # 	'mindeltaRlepJets',
# # 	'mindeltaRlep1Jets',
# # 	'mindeltaRlep2Jets',
# # 	'mindeltaRlep3Jets',
# 
# # 	'minMlB',
# # 	'minMlBv2', #
# # 	'minMlllB',
# # 	'minMlllBv2',
# # 	'minMlllBv3', #
# # 	'minMlllBv4', #
# # 
# # 	'minDPhiMETJet',
# # 	'DRlep1Jet1',
# # 	'minDRlep1Jets',
# # 	'minDRlep2Jets',
# # 	'minDRlep3Jets',
# # 	'minDRlepsJets',	
# 	
# # 	'mindeltaR',
# # 	'mindeltaRB',
# # 
# # 	'mindeltaR1',
# # 	'mindeltaR2',
# # 	'mindeltaR3',
# 
# # 	'PtRel1',
# # 	'PtRel2',
# # 	'PtRel3',
# # 	'PtRel',
# 
# # 	'lepCharge',
# # 	'lepIso',
# # 	'ElIso',
# # 	'MuIso',
# # 	'MllsameFlavorOS',
# # 	'MllOSall',
# # 	'MllOSallmin',
# # 	'MllOSallmax',
# # 	'Mlll'
	]

# for testing
#plotList = ['lepPt', 'STrebinnedv2', 'NJets', 'NBJets', 'JetEta','JetPt', 'lep1Eta','METrebinned','HTrebinned']

fit  = False
fit2 = False
fit3 = False
fit4 = False

isEMlist=['elel','elmu','mumu']
TrigCat = {'elel':'ee','elmu':'em','mumu':'mm'}

for discriminant in plotList:

# 	if discriminant != 'ST': continue
	fileTemp='templates_'+sig[:sig.find('M')]+'_bW0p5_tZ0p25_tH0p25'+isRebinned+'.root'
        if 'X53' in sig: fileTemp='templates_'+sig[:sig.find('M')]+'_tW1p0'+isRebinned+'.root'
        if 'TTTT' in sig: fileTemp='templates_'++sig[:sig.find('M')]+'_'+isRebinned+'.root'
        if not discriminant == '': fileTemp=fileTemp.replace('templates_', 'templates_'+discriminant+'_')
	else: discriminant = 'cleanAK4HT'
	print templateDir+'/'+fileTemp
	if not os.path.exists(templateDir+'/'+fileTemp): 
		print 'not found, skipping'
		continue
	RFile = TFile(templateDir+'/'+fileTemp)

	#systHists={}
	for isEM in isEMlist:
		systHists={}
		gStyle.SetOptStat(0)
		gStyle.SetEndErrorSize(0);
		setTDRStyle()

		if 'El' in discriminant and  isEM=='mumu':continue	
		if 'Mu' in discriminant and  isEM=='elel':continue	

		histPrefix=isEM+'2018AD' #iscriminant+'_'+lumiInTemplates+'fb_'+isEM
		
		try: hTOP = RFile.Get(histPrefix+'__top').Clone()
		except: 
			if(DEBUG): print "There is no TOP!!!!!!!!",
			if(DEBUG): print "Skipping TOP....."
			pass
		try: hEWK = RFile.Get(histPrefix+'__ewk').Clone()
		except: 
			if(DEBUG): print "There is no EWK!!!!!!!!",
			if(DEBUG): print "Skipping EWK....."
			pass
		try: hQCD = RFile.Get(histPrefix+'__qcd').Clone()
		except: 
			if(DEBUG): print "There is no QCD!!!!!!!!",
			if(DEBUG): print "Skipping QCD....."
			pass
		try: hDDBKG = RFile.Get(histPrefix+'__FakeRate').Clone()
		except:
			if(DEBUG): print "There is no DDBKG!!!!!!!!",
			if(DEBUG): print "Skipping DDBKG....."
			pass
                try: hCMID = RFile.Get(histPrefix+'__ChargeMisID').Clone()
                except:
                        if(DEBUG): print "There is no CMID!!!!!!!!",
                        if(DEBUG): print "Skipping CMID....."
                        pass
					
		try: print discriminant,isEM, "TOP", hTOP.Integral()
		except: pass
		try: print discriminant,isEM, "EWK", hEWK.Integral()
		except: pass
		try: print discriminant,isEM, "QCD", hQCD.Integral()
		except: pass
		try: print discriminant,isEM, "DDBKG", hDDBKG.Integral()
		except: pass
                try: print discriminant,isEM, "CMID", hCMID.Integral()
                except: pass
		
		hData = RFile.Get(histPrefix+'__data_obs').Clone()
		gaeData = TGraphAsymmErrors(hData.Clone(hData.GetName()+'gaeData'))
		#hsig1 = RFile.Get(histPrefix+'__'+sig+'bwbw').Clone()
		#hsig2 = RFile.Get(histPrefix+'__'+sig+'tztz').Clone()
		#hsig3 = RFile.Get(histPrefix+'__'+sig+'thth').Clone()

		hsig = RFile.Get(histPrefix+'__'+sig).Clone(histPrefix+'__'+sig+'nominal')
		hsigM1500 = RFile.Get(histPrefix+'__'+sigM1500).Clone(histPrefix+'__'+sigM1500+'nominal')
		#decays = ['tztz','thth','tzbw','thbw','tzth']
		#for decay in decays:
		#	htemp = RFile.Get(histPrefix+'__'+sig+decay).Clone()
		#	hsig.Add(htemp)
		#	htemp = RFile.Get(histPrefix+'__'+sigM1500+decay).Clone()
		#	hsigM1500.Add(htemp)

		# original scale = lumi * xsec * BR(50/25/25) / N(33/33/33)
		#hsig1.Scale(1.0/BR['TTBWBW'])
		#hsig2.Scale(1.0/BR['TTTZTZ'])
		#hsig3.Scale(1.0/BR['TTTHTH'])
		if doNormByBinWidth:
			normByBinWidth(hTOP)
			normByBinWidth(hEWK)
			normByBinWidth(hQCD)
			normByBinWidth(hsig)
			normByBinWidth(hsigM1500)
			normByBinWidth(hData)
		else: poissonErrors(gaeData,discriminant)
                #systematicList = ['pileup','btag','mistag','pdfNew','muRFcorrdNew','elPR','elFR','muPR','muFR','jec','jer','TrigEffWeight', 'elIdSys']
		#systematicList = ['pileup','prefire','btag','mistag','pdfNew','muRFcorrdNew','jec','jer','TrigEffWeight', 'elIdSys']
		if doAllSys:
			for sys in systematicList:
				for ud in ['Up','Down']:
					try: systHists['top'+sys+ud] = RFile.Get(histPrefix+'__top__'+sys.replace('muRFcorrdNew','muRFcorrdNewTTV').replace('Trig',TrigCat[isEM]+'Trig')+''+ud).Clone()
					except: 
						if(DEBUG): print "Skipping",sys,"for TOP"
						pass
					try: systHists['ewk'+sys+ud] = RFile.Get(histPrefix+'__ewk__'+sys.replace('muRFcorrdNew','muRFcorrdNewEwk').replace('Trig',TrigCat[isEM]+'Trig')+''+ud).Clone()
					except: 
						if(DEBUG): print "Skipping",sys,"for EWK"
						pass
					try: systHists['qcd'+sys+ud] = RFile.Get(histPrefix+'__qcd__'+sys+''+ud).Clone()
					except: 
						if(DEBUG): print "Skipping",sys,"for QCD"
						pass
					try: systHists['ddbkg'+sys+ud] = RFile.Get(histPrefix+'__FakeRate__'+sys+''+ud).Clone()
					except: 
						if(DEBUG): print "Skipping",sys,"for DDBKG"
						pass
                                        try: systHists['cmid'+sys+ud] = RFile.Get(histPrefix+'__ChargeMisID__'+sys+''+ud).Clone()
                                        except:
                                                if(DEBUG): print "Skipping",sys,"for CMID"
                                                pass

		try: hTOPstatOnly = hTOP.Clone(hTOP.GetName()+'statOnly')
		except: pass
		try: hEWKstatOnly= hEWK.Clone(hEWK.GetName()+'statOnly')
		except: pass
		try: hQCDstatOnly = hQCD.Clone(hQCD.GetName()+'statOnly')
		except: pass
		try: hDDBKGstatOnly = hDDBKG.Clone(hDDBKG.GetName()+'statOnly')
		except: pass
                try: hCMIDstatOnly = hCMID.Clone(hCMID.GetName()+'statOnly')
                except: pass

                hTOPstatYr = {}
                hEWKstatYr = {}
                hDDBKGstatYr = {}
                hCMIDstatYr = {}
                try: hTOPstatYr['17'] = RFile.Get(histPrefix.replace('1718','2017BF')+'__top').Clone(hTOP.GetName()+'statOnly17')
                except: pass
                try: hEWKstatYr['17'] = RFile.Get(histPrefix.replace('1718','2017BF')+'__ewk').Clone(hEWK.GetName()+'statOnly17')
                except: pass
                try: hDDBKGstatYr['17'] = RFile.Get(histPrefix.replace('1718','2017BF')+'__FakeRate').Clone(hDDBKG.GetName()+'statOnly17')
                except: pass
                try: hCMIDstatYr['17'] = RFile.Get(histPrefix.replace('1718','2017BF')+'__ChargeMisID').Clone(hCMID.GetName()+'statOnly17')
                except: pass
                try: hTOPstatYr['18'] = RFile.Get(histPrefix.replace('1718','2018AD')+'__top').Clone(hTOP.GetName()+'statOnly18')
                except: pass
                try: hEWKstatYr['18'] = RFile.Get(histPrefix.replace('1718','2018AD')+'__ewk').Clone(hEWK.GetName()+'statOnly18')
                except: pass
                try: hDDBKGstatYr['18'] = RFile.Get(histPrefix.replace('1718','2018AD')+'__FakeRate').Clone(hDDBKG.GetName()+'statOnly18')
                except: pass
                try: hCMIDstatYr['18'] = RFile.Get(histPrefix.replace('1718','2018AD')+'__ChargeMisID').Clone(hCMID.GetName()+'statOnly18')
                except: pass

		try: bkgHT = hTOP.Clone()
		except: pass
		try: bkgHT.Add(hEWK)
		except: pass
		try: bkgHT.Add(hQCD)
		except: pass
		try: bkgHT.Add(hDDBKG)
		except: pass
                try: bkgHT.Add(hCMID)
                except: pass
		gaeBkgHT = TGraphAsymmErrors(bkgHT.Clone("gaeBkgHT"))

		if doNormByBinWidth: poissonNormByBinWidth(gaeBkgHT,bkgHT)
		else: poissonErrors(gaeBkgHT,discriminant)

		try:
			totBkgTemp1[isEM] = TGraphAsymmErrors(bkgHT.Clone(bkgHT.GetName()+'shapeOnly'))
			totBkgTemp2[isEM] = TGraphAsymmErrors(bkgHT.Clone(bkgHT.GetName()+'shapePlusNorm'))
			totBkgTemp3[isEM] = TGraphAsymmErrors(bkgHT.Clone(bkgHT.GetName()+'All'))
		except: pass
			
		try:
			BINS_temp = hTOP.GetNbinsX()
		except:
			BINS_temp = 0

		#print(systHists)
		for ibin in range(1,BINS_temp+1):
			errorUp = 0.
			errorDn = 0.
			errorSym = 0.
			errorNorm = 0.
			errorStatUp = gaeBkgHT.GetErrorYhigh(ibin-1)**2
			errorStatDn = gaeBkgHT.GetErrorYlow(ibin-1)**2

			errorStatOnly = bkgHT.GetBinError(ibin)**2
# 			errorStatOnly = hTOP.GetBinError(ibin)**2 + hEWK.GetBinError(ibin)**2 #since hDDBKG stat error is in its  "sys" error. #MMMM maybe this isn correct.

                        normUncMC = 0.
                        normUncDDBKGsq = 0.
                        normUncCMIDsq = 0.
                        for yr in years:
				try: normUncMC += math.sqrt(getNormUnc(hTOPstatYr[yr],ibin,isEM,yr))
				except: pass
				try: normUncMC += math.sqrt(getNormUnc(hEWKstatYr[yr],ibin,isEM,yr))
				except: pass
				try: normUncMC += math.sqrt(getNormUnc(hQCDstatYr[yr],ibin,isEM,yr))
				except: pass
				try: normUncDDBKGsq += getDDBKGNormUnc(hDDBKGstatYr[yr],ibin,isEM,yr)
				except: pass
                                try: normUncCMIDsq += getCMIDNormUnc(hCMIDstatYr[yr],ibin,isEM,yr)
                                except: pass
                        errorNorm = (normUncMC)**2 + normUncDDBKGsq + normUncCMIDsq

			if doAllSys:
				for sys in systematicList:
					if not ('PR' in sys or 'FR' in sys or 'NP' in sys):
						errorSym += (0.5*abs(systHists['top'+sys+'Up'].GetBinContent(ibin)-systHists['top'+sys+'Down'].GetBinContent(ibin)))**2				
						errorPlus = systHists['top'+sys+'Up'].GetBinContent(ibin)-hTOP.GetBinContent(ibin)
						errorMinus = hTOP.GetBinContent(ibin)-systHists['top'+sys+'Down'].GetBinContent(ibin)
						if errorPlus > 0: errorUp += errorPlus**2
						else: errorDn += errorPlus**2
						if errorMinus > 0: errorDn += errorMinus**2
						else: errorUp += errorMinus**2
						if sys!='toppt':
							try:
								errorSym += (0.5*abs(systHists['ewk'+sys+'Up'].GetBinContent(ibin)-systHists['ewk'+sys+'Down'].GetBinContent(ibin)))**2				
								errorPlus = systHists['ewk'+sys+'Up'].GetBinContent(ibin)-hEWK.GetBinContent(ibin)
								errorMinus = hEWK.GetBinContent(ibin)-systHists['ewk'+sys+'Down'].GetBinContent(ibin)
								if errorPlus > 0: errorUp += errorPlus**2
								else: errorDn += errorPlus**2
								if errorMinus > 0: errorDn += errorMinus**2
								else: errorUp += errorMinus**2
							except: pass
							try:
								errorSym += (0.5*abs(systHists['qcd'+sys+'Up'].GetBinContent(ibin)-systHists['qcd'+sys+'Down'].GetBinContent(ibin)))**2				
								errorPlus = systHists['qcd'+sys+'Up'].GetBinContent(ibin)-hQCD.GetBinContent(ibin)
								errorMinus = hQCD.GetBinContent(ibin)-systHists['qcd'+sys+'Down'].GetBinContent(ibin)
								if errorPlus > 0: errorUp += errorPlus**2
								else: errorDn += errorPlus**2
								if errorMinus > 0: errorDn += errorMinus**2
								else: errorUp += errorMinus**2
							except: pass
					if 'PR' in sys or 'FR' in sys or 'NP' in sys:
						try: errorSym += (0.5*abs(systHists['ddbkg'+sys+'Up'].GetBinContent(ibin)-systHists['ddbkg'+sys+'Down'].GetBinContent(ibin)))**2				
						except: pass
						try: 
							if not sys=="NPWeightMu2018": errorPlus = systHists['ddbkg'+sys+'Up'].GetBinContent(ibin)-hDDBKG.GetBinContent(ibin)
							else: errorPlus = 0 # one-sided NPweights, -0.02 for Mu
						except: pass
						try: 
							if not sys=="NPWeightEl2018": errorMinus = hDDBKG.GetBinContent(ibin)-systHists['ddbkg'+sys+'Down'].GetBinContent(ibin)
							else:errorMinus = 0 # one-sided NPweights, +0.08 for El
                                                except: pass
                                                if errorPlus > 0: errorUp += errorPlus**2
                                                else: errorDn += errorPlus**2
                                                if errorMinus > 0: errorDn += errorMinus**2
                                                else: errorUp += errorMinus**2


                                                try: errorSym += (0.5*abs(systHists['cmid'+sys+'Up'].GetBinContent(ibin)-systHists['cmid'+sys+'Down'].GetBinContent(ibin)))**2
                                                except: pass
                                                try: errorPlus = systHists['cmid'+sys+'Up'].GetBinContent(ibin)-hCMID.GetBinContent(ibin)
                                                except: pass
                                                try: errorMinus = hCMID.GetBinContent(ibin)-systHists['cmid'+sys+'Down'].GetBinContent(ibin)
						except: pass
						if errorPlus > 0: errorUp += errorPlus**2
						else: errorDn += errorPlus**2
						if errorMinus > 0: errorDn += errorMinus**2
						else: errorUp += errorMinus**2
						#if ('El' in sys and 'el' in isEM) or ('Mu' in sys and 'mu' in isEM): print("NPerror", errorPlus, errorMinus, systHists['ddbkg'+sys+'Up'].GetName(), ibin, hDDBKG.GetBinContent(ibin), systHists['ddbkg'+sys+'Up'].GetBinContent(ibin))
							
			totBkgTemp1[isEM].SetPointEYhigh(ibin-1,math.sqrt(errorUp))
			totBkgTemp1[isEM].SetPointEYlow(ibin-1,math.sqrt(errorDn))
			totBkgTemp2[isEM].SetPointEYhigh(ibin-1,math.sqrt(errorUp+errorNorm))
			totBkgTemp2[isEM].SetPointEYlow(ibin-1,math.sqrt(errorDn+errorNorm))
# 			totBkgTemp3[isEM].SetPointEYhigh(ibin-1,math.sqrt(errorUp+errorNorm+errorStatOnly))
# 			totBkgTemp3[isEM].SetPointEYlow(ibin-1,math.sqrt(errorDn+errorNorm+errorStatOnly))
			totBkgTemp3[isEM].SetPointEYhigh(ibin-1,math.sqrt(errorUp+errorNorm+errorStatUp))
			totBkgTemp3[isEM].SetPointEYlow(ibin-1,math.sqrt(errorDn+errorNorm+errorStatDn))
						
		bkgHTgerr = totBkgTemp3[isEM].Clone()

# 		if (isEM=='EEE' and discriminant=="STrebinnedv2"): 
# 			for ibin in range(1,hTOP.GetNbinsX()):
# 				print "gaeBkgHT  -- bin: ",ibin," yield: ",gaeBkgHT.GetY()[ibin-1],", + ",gaeBkgHT.GetErrorYhigh(ibin-1)," - ",gaeBkgHT.GetErrorYlow(ibin-1)
# 				print "bkgHTgerr -- bin: ",ibin," yield: ",bkgHTgerr.GetY()[ibin-1],", + ",bkgHTgerr.GetErrorYhigh(ibin-1)," - ",bkgHTgerr.GetErrorYlow(ibin-1)

		'''
		scaleFact1 = int(bkgHT.GetMaximum()/hsig1.GetMaximum()) - int(bkgHT.GetMaximum()/hsig1.GetMaximum()) % 10
		scaleFact1 *= 0.60
		if scaleFact1==0: scaleFact1=int(bkgHT.GetMaximum()/hsig1.GetMaximum())
		if scaleFact1==0: scaleFact1=1
		if not scaleSignals:
			scaleFact1=1
 		#else:
 		#	scaleFact1=25
                '''
		#hsig1.Scale(scaleFact1)
		#hsig2.Scale(scaleFact1)
		#hsig3.Scale(scaleFact1)
		hsig.Scale(xsec[sig]*scaleFact1)
		hsigM1500.Scale(xsec[sigM1500]*scaleFact2)
		print(sig, xsec[sig]*scaleFact1)
		print(sigM1500, xsec[sigM1500]*scaleFact2)
		
		stackbkgHT = THStack("stackbkgHT","")
		try: stackbkgHT.Add(hTOP)
		except: pass
		try: stackbkgHT.Add(hEWK)
		except: pass
		try: 
			if hQCD.Integral()/bkgHT.Integral()>.005: stackbkgHT.Add(hQCD) #don't plot QCD if it is less than 0.5%
		except: pass
		try: stackbkgHT.Add(hCMID)#DDBKG)
		except: pass
                try: stackbkgHT.Add(hDDBKG)#CMID)
                except: pass

		try:
			hTOP.SetLineColor(kCyan)
			hTOP.SetFillColor(kCyan)
			hTOP.SetLineWidth(2)
		except: pass
		try: 
			hEWK.SetLineColor(kMagenta-2)
			hEWK.SetFillColor(kMagenta-2)
			hEWK.SetLineWidth(2)
		except: pass
		try:
			hQCD.SetLineColor(kOrange+5)
			hQCD.SetFillColor(kOrange+5)
			hQCD.SetLineWidth(2)
		except: pass
		try:
			hDDBKG.SetLineColor(15)
			hDDBKG.SetFillColor(15)
			hDDBKG.SetLineWidth(2)
		except: pass
		try:
                        hCMID.SetLineColor(kGreen+3)
                        hCMID.SetFillColor(kGreen+3)
                        hCMID.SetLineWidth(2)
                except: pass

		
		hsig.SetLineColor(kBlack)
		hsig.SetLineStyle(1)
		hsig.SetLineWidth(3)
		hsigM1500.SetLineColor(kBlack)
		hsigM1500.SetLineStyle(2)
		hsigM1500.SetLineWidth(3)
		#hsig1.SetLineColor(kRed)
		#hsig1.SetLineStyle(2)
		#hsig1.SetLineWidth(3)
		#hsig2.SetLineColor(kOrange-2)
		#hsig2.SetLineStyle(5)
		#hsig2.SetLineWidth(3)
		#hsig3.SetLineColor(kGreen+1)
		#hsig3.SetLineStyle(7)
		#hsig3.SetLineWidth(3)
		
# 		hData.SetMarkerStyle(20)
# 		hData.SetMarkerSize(1.2)
# 		hData.SetLineWidth(2)
		gaeData.SetMarkerStyle(20)
		gaeData.SetMarkerSize(1.2)
		gaeData.SetLineWidth(2)
		gaeData.SetMarkerColor(kBlack)
		gaeData.SetLineColor(kBlack)

		bkgHTgerr.SetFillStyle(3004)
		bkgHTgerr.SetFillColor(kBlack)


		c1 = TCanvas("c1","c1",600,500)
		yDiv=0.25
		if blind == True: yDiv=0.1
		# for some reason the markers at 0 don't show with this setting:
		uMargin = 0.00001
		#if blind == True: uMargin = 0.15
		rMargin=.04
		# overlap the pads a little to hide the error bar gap:
		#uPad=TPad("uPad","",0,yDiv-0.009,1,1) #for actual plots
		uPad=TPad("uPad","",0,yDiv,1,1) #for actual plots
		#if blind: uPad=TPad("uPad","",0,0,1,1)
		uPad.SetTopMargin(0.08)
		uPad.SetBottomMargin(uMargin)
		if blind == True: uPad.SetBottomMargin(0.15)
		uPad.SetRightMargin(rMargin)
		uPad.SetLeftMargin(.105)
		uPad.Draw()
		if blind == False:
			lPad=TPad("lPad","",0,0,1,yDiv) #for sigma runner
			lPad.SetTopMargin(0.05)
			lPad.SetBottomMargin(.4)
			lPad.SetRightMargin(rMargin)
			lPad.SetLeftMargin(.105)
			lPad.SetGridy()
			lPad.Draw()

# 		try: 
# 			if not doNormByBinWidth: hData.SetMaximum(1.2*max(hData.GetMaximum(),bkgHT.GetMaximum()))
# 		except: 
# 			pass
# 		hData.SetMinimum(0.015)
# 		hData.SetTitle("")
# 		if doNormByBinWidth: hData.GetYaxis().SetTitle("Events / 1 GeV")
# 		else: hData.GetYaxis().SetTitle("Events")
# 		formatUpperHist(hData)
# 		uPad.cd()
# 		hData.SetTitle("")
# 		if not blind: hData.Draw("E1 X0")

		# this is super important now!! gaeData has badly defined (negative) maximum
		gaeData.SetMaximum(2.5*max(gaeData.GetMaximum(),stackbkgHT.GetMaximum()))
		gaeData.SetMinimum(0.015)
		gaeData.SetTitle("")
		if doNormByBinWidth: gaeData.GetYaxis().SetTitle("< Events / GeV >")
		else: gaeData.GetYaxis().SetTitle("Events / bin")
		formatUpperHist_v2(gaeData,hData,discriminant)
		if 'CR' in templateDir and 'ST' in discriminant:
                        #isRebinned = '_above1400'
			gaeData.GetXaxis().SetLimits(0.,1400.)
			gaeData.GetXaxis().SetRangeUser(0.,1400.)
		if(DEBUG): print 'set dummy gaeData X axis title'
		gaeData.GetXaxis().SetTitle("TEST")
		uPad.cd()
		gaeData.SetTitle("")
		gStyle.SetErrorX(0.);
		if not blind: 
			gaeData.Draw("ap")

		if blind:
			hsig.SetMaximum(1.5*max(hsig.GetMaximum(),stackbkgHT.GetMaximum()))
			hsig.SetMinimum(0.015)
			if doNormByBinWidth: hsig.GetYaxis().SetTitle("Events / 1 GeV")
			else: hsig.GetYaxis().SetTitle("Events")
			formatUpperHist(hsig,discriminant)
			hsig.Draw("HIST")

		stackbkgHT.Draw("SAME HIST")
		hsig.Draw("SAME HIST")
		hsigM1500.Draw("SAME HIST")
# 		hsig1.Draw("SAME HIST")
# 		hsig2.Draw("SAME HIST")
# 		hsig3.Draw("SAME HIST")
		if not blind: gaeData.Draw("p") #redraw data so its not hidden
		uPad.RedrawAxis()
		gStyle.SetErrorX(0.5);
		bkgHTgerr.Draw("SAME E2")

		leg = {}
		if 'Tau21' in discriminant:
			leg = TLegend(0.15,0.53,0.45,0.90)
		#elif 'Eta' in discriminant or 'deltaRjet2' in discriminant:
		#	leg = TLegend(0.72,0.43,0.95,0.90)
		elif blind:
			leg = TLegend(0.360,0.7,0.939,0.89)
		else:
			leg = TLegend(0.360,0.622,0.939,0.89)
		leg.SetShadowColor(0)
		leg.SetFillColor(0)
		leg.SetFillStyle(0)
		leg.SetLineColor(0)
		leg.SetLineStyle(0)
		leg.SetBorderSize(0)
		leg.SetNColumns(2)
		leg.SetTextFont(62)
		leg.SetTextSize(0.06)
		if blind: leg.SetTextSize(0.05)
		if not blind: leg.AddEntry(gaeData,"Data","pel")
# 		try: leg.AddEntry(hDDBKG,"DD BKG","f")
		try: leg.AddEntry(hDDBKG,"Nonprompt","f")
		except: pass

		scaleFact1Str = ''
		scaleFact2Str = ''		
		#if scaleFact1 > 1.0 : scaleFact1Str = ' x'+str(scaleFact1)
		#if scaleFact2 > 1.0 : scaleFact2Str = ' x'+str(scaleFact2)
		#if not scaleSignals or dontShowSignalScaling:
		#	scaleFact1Str = ''
		#	scaleFact2Str = ''
		leg.AddEntry(hsig,sigleg+scaleFact1Str,"l")
                try: leg.AddEntry(hCMID,"ChargeMisID","f")
                except: pass
                leg.AddEntry(hsigM1500,siglegM1500+scaleFact2Str,"l")
		try: leg.AddEntry(hEWK,"VV(V)","f")
		except: pass
		leg.AddEntry(None, '','')
		try: leg.AddEntry(hTOP,"t#bar{t}V+t#bar{t}t#bar{t}","f")
		except: pass
		try: 
			if hQCD.Integral()/bkgHT.Integral()>.005: leg.AddEntry(hQCD,"QCD","f") #don't plot QCD if it is less than 0.5%
		except: pass
		leg.AddEntry(bkgHTgerr,"","")
		leg.AddEntry(bkgHTgerr,"Bkg uncert.","f")
		leg.Draw("same")

		prelimTex=TLatex()
		prelimTex.SetNDC()
		prelimTex.SetTextAlign(31) # align right
		prelimTex.SetTextFont(42)
		prelimTex.SetTextSize(0.07)
		if blind: prelimTex.SetTextSize(0.06)
		prelimTex.SetLineWidth(2)
		prelimTex.DrawLatex(0.95,0.94,str(lumi)+" fb^{-1} (13 TeV)")

		prelimTex2=TLatex()
		prelimTex2.SetNDC()
		prelimTex2.SetTextFont(61)
		prelimTex2.SetLineWidth(2)
		prelimTex2.SetTextSize(0.10)
		if blind: prelimTex2.SetTextSize(0.08)
		prelimTex2.DrawLatex(0.16,0.81,"CMS")

		prelimTex3=TLatex()
		prelimTex3.SetNDC()
		prelimTex3.SetTextAlign(13)
		prelimTex3.SetTextFont(52)
		prelimTex3.SetTextSize(0.075)
		prelimTex3.SetLineWidth(2)
# 		if not blind: prelimTex3.DrawLatex(0.24,0.975,"Preliminary")
# 		#if blind: prelimTex3.DrawLatex(0.29175,0.9364,"Preliminary")
# 		if blind: prelimTex3.DrawLatex(0.29175,0.975,"Preliminary")
 		prelimTex3.SetTextSize(0.06) #for Supplementary
# 		if not blind: prelimTex3.DrawLatex(0.14,0.78,"Supplementary")
 		if not blind:
			prelimTex3.SetTextSize(0.05)
			prelimTex3.DrawLatex(0.15,0.78,"Work in progress") 
			#prelimTex3.DrawLatex(0.14,0.78,"Preliminary")

		chLatex = TLatex()
		chLatex.SetNDC()
		chLatex.SetTextSize(0.08)
		if blind: chLatex.SetTextSize(0.06)
		chLatex.SetTextAlign(31) # align right
		chString = ''
		if isEM=='All': chString+='All channels'
		if isEM=='elel': chString+='ee'
		if isEM=='elmu': chString+='e#mu'
		if isEM=='mumu': chString+='#mu#mu'
		if not blind: chLatex.DrawLatex(0.92, 0.56, chString)
		else: chLatex.DrawLatex(0.92, 0.65, chString)

		flat = TF1("flat","pol1",30,250);

		line = TF1("line","pol1",250,1500);
		line2 = TF1("line2","pol1",30,1500);
		line3 = TF1("line3","pol1",30,1500);
		line4 = TF1("line4","pol1",30,1500);

		line.SetLineWidth(2);

		para = TF1("para","pol2",30,1500); para.SetLineColor(kBlue);
		para2 = TF1("para2","pol2",30,1500); para2.SetLineColor(kBlue);
		para3 = TF1("para3","pol2",30,1500); para3.SetLineColor(kBlue);

		cube = TF1("cube","pol3",30,1500); cube.SetLineColor(kGreen);
		cube2 = TF1("cube2","pol3",30,1500); cube2.SetLineColor(kGreen);
		cube3 = TF1("cube3","pol3",30,1500); cube3.SetLineColor(kGreen);

		if blind == False and not doRealPull:
			lPad.cd()
			pull=hData.Clone("pull")
#			pull.Scale(1.0/pull.Integral())
#			pullDenom = bkgHT.Clone("pullDenom")
#			pullDenom.Scale(1.0/pullDenom.Integral())
#			pull.Divide(pullDenom)
			try: pull.Divide(hData, bkgHT)
			except: pass
	
			try:
				for binNo in range(0,hData.GetNbinsX()+2):
					if bkgHT.GetBinContent(binNo)!=0:
						pull.SetBinError(binNo,hData.GetBinError(binNo)/bkgHT.GetBinContent(binNo))
			except: pass

			pullData=TGraphAsymmErrors(pull.Clone("pullData"))
			for binNo in range(0,hData.GetNbinsX()+2):
				if bkgHT.GetBinContent(binNo)!=0:
					pullData.SetPointEYhigh(binNo-1,gaeData.GetErrorYhigh(binNo-1)/bkgHT.GetBinContent(binNo))
					pullData.SetPointEYlow(binNo-1,gaeData.GetErrorYlow(binNo-1)/bkgHT.GetBinContent(binNo))
			
			pull.SetMaximum(2)
			pull.SetMinimum(0)
			pull.SetFillColor(kGray+2)#1)
			pull.SetLineColor(kGray+2)#1)
			formatLowerHist(pull,discriminant)
			pull.Draw("E0")
			pull.Draw("p")#.Draw("ap")

			if 'Bins' in discriminant:
				print '******************************'
				print 'Data/MC for',discriminant
				for binNo in range(0,pull.GetNbinsX()+2):
					print 'Bin',binNo,': content =',pull.GetBinContent(binNo),'; error =',pull.GetBinError(binNo),';'
		
			BkgOverBkg = pull.Clone("bkgOverbkg")
			try: BkgOverBkg.Divide(bkgHT, bkgHT)
			except: pass
			pullUncBandTot=TGraphAsymmErrors(BkgOverBkg.Clone("pulluncTot"))
			
			try:
				for binNo in range(0,hData.GetNbinsX()+2):
					if bkgHT.GetBinContent(binNo)!=0:
						pullUncBandTot.SetPointEYhigh(binNo-1,totBkgTemp3[isEM].GetErrorYhigh(binNo-1)/bkgHT.GetBinContent(binNo))
						pullUncBandTot.SetPointEYlow(binNo-1,totBkgTemp3[isEM].GetErrorYlow(binNo-1)/bkgHT.GetBinContent(binNo))			
			except: pass
						
			if not doOneBand: pullUncBandTot.SetFillStyle(3001)
			else: pullUncBandTot.SetFillStyle(3344)
			pullUncBandTot.SetFillColor(1)
			pullUncBandTot.SetLineColor(1)
			pullUncBandTot.SetMarkerSize(0)
			gStyle.SetHatchesLineWidth(1)
			#pullUncBandTot.Draw("SAME E2")
				
			pullUncBandNorm=TGraphAsymmErrors(BkgOverBkg.Clone("pulluncNorm"))

			try:
				for binNo in range(0,hData.GetNbinsX()+2):
					if bkgHT.GetBinContent(binNo)!=0:
						pullUncBandNorm.SetPointEYhigh(binNo-1,totBkgTemp2[isEM].GetErrorYhigh(binNo-1)/bkgHT.GetBinContent(binNo))
						pullUncBandNorm.SetPointEYlow(binNo-1,totBkgTemp2[isEM].GetErrorYlow(binNo-1)/bkgHT.GetBinContent(binNo))
			except: pass			

			pullUncBandNorm.SetFillStyle(3001)
			pullUncBandNorm.SetFillColor(2)
			pullUncBandNorm.SetLineColor(2)
			pullUncBandNorm.SetMarkerSize(0)
			gStyle.SetHatchesLineWidth(1)
			#if not doOneBand: pullUncBandNorm.Draw("SAME E2")
			
			pullUncBandStat=TGraphAsymmErrors(BkgOverBkg.Clone("pulluncStat"))

			try:
				for binNo in range(0,hData.GetNbinsX()+2):
					if bkgHT.GetBinContent(binNo)!=0:
						pullUncBandStat.SetPointEYhigh(binNo-1,totBkgTemp1[isEM].GetErrorYhigh(binNo-1)/bkgHT.GetBinContent(binNo))
						pullUncBandStat.SetPointEYlow(binNo-1,totBkgTemp1[isEM].GetErrorYlow(binNo-1)/bkgHT.GetBinContent(binNo))			
			except: pass
			
			pullUncBandStat.SetFillStyle(3001)
			pullUncBandStat.SetFillColor(3)
			pullUncBandStat.SetLineColor(3)
			pullUncBandStat.SetMarkerSize(0)
			gStyle.SetHatchesLineWidth(1)
			#if not doOneBand: pullUncBandStat.Draw("SAME E2")

			pullLegend=TLegend(0.14,0.87,0.85,0.96)
			SetOwnership( pullLegend, 0 )   # 0 = release (not keep), 1 = keep
			pullLegend.SetShadowColor(0)
			pullLegend.SetNColumns(3)
			pullLegend.SetFillColor(0)
			pullLegend.SetFillStyle(0)
			pullLegend.SetLineColor(0)
			pullLegend.SetLineStyle(0)
			pullLegend.SetBorderSize(0)
			pullLegend.SetTextFont(42)
			if not doOneBand: pullLegend.AddEntry(pullUncBandStat , "Bkg shape syst." , "f")
			if not doOneBand: pullLegend.AddEntry(pullUncBandNorm , "Bkg shape #oplus norm. syst." , "f")
			if not doOneBand: pullLegend.AddEntry(pullUncBandTot , "Bkg stat. #oplus all syst." , "f")
#			else: pullLegend.AddEntry(pullUncBandTot , "Bkg stat. #oplus syst." , "f")
			else: 
				pullLegend.AddEntry(pullUncBandTot , "Bkg stat." , "f")
			#	pullLegend.AddEntry(jsf, "Fit","l")
			#	pullLegend.AddEntry(jsfup, "#pm 1#sigma","l")
			#pullLegend.Draw("SAME")
			pull.Draw("SAME")
			lPad.RedrawAxis()

		if blind == False and doRealPull:
			lPad.cd()
			pull=hData.Clone(hData.GetName()+'pull')
			for binNo in range(1,hData.GetNbinsX()+1):
				# case for data < MC:
				dataerror = gaeData.GetErrorYhigh(binNo-1)
				MCerror = totBkgTemp3[isEM].GetErrorYlow(binNo-1)
				# case for data > MC: 
				if(hData.GetBinContent(binNo) > bkgHT.GetBinContent(binNo)):
					dataerror = gaeData.GetErrorYlow(binNo-1)
					MCerror = totBkgTemp3[isEM].GetErrorYhigh(binNo-1)
				if(dataerror+MCerror!=0):pull.SetBinContent(binNo,(hData.GetBinContent(binNo)-bkgHT.GetBinContent(binNo))/math.sqrt(MCerror**2+dataerror**2))
			pull.SetMaximum(3)
			pull.SetMinimum(-3)
			pull.SetFillColor(kGray+2)
			pull.SetLineColor(kGray+2)
			formatLowerHist(pull,discriminant)
			if 'CR' in templateDir and 'ST' in discriminant:
                        #         isRebinned = '_above1400'
                                #pull.GetXaxis().SetLimits(hData.GetBinLowEdge(1),1400)
                                pull.SetAxisRange(hData.GetBinLowEdge(1),1399.9,"X")
			pull.Draw("HIST")

		savePrefix = templateDir+'/plots/'
		if not os.path.exists(savePrefix): os.system('mkdir '+savePrefix)
		savePrefix+=histPrefix+"_"+discriminant+isRebinned#+isRebinned#+"_"+discriminant+isRebinned
		if doRealPull: savePrefix+='_pull'
		if yLog: savePrefix+='_logy'
		if blind: savePrefix+='_blinded'

		if doOneBand:
			c1.SaveAs(savePrefix+"_totBand.pdf")
			c1.SaveAs(savePrefix+"_totBand.png")
			c1.SaveAs(savePrefix+"_totBand.root")
			c1.SaveAs(savePrefix+"totBand.C")
		else:
			c1.SaveAs(savePrefix+".pdf")
			c1.SaveAs(savePrefix+".png")
			c1.SaveAs(savePrefix+".root")
			c1.SaveAs(savePrefix+".C")
			
	RFile.Close()

print("--- %s minutes ---" % (round(time.time() - start_time, 2)/60))
