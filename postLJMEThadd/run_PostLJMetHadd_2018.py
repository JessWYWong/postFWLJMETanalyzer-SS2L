#!/usr/bin/python

import PostLJMetHadd 
import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--submit",action="store_true",default=False)
option = parser.parse_args()


if __name__ == '__main__':

	#################################################
	### Set your folders / parameters
	#################################################
	
	rel_base = os.environ['CMSSW_BASE']
	cmssw = 'CMSSW_9_4_12'

	#folder	= 'FWLJMET102X_2lep2018_wywong_091819'
        #folder = 'FWLJMET102X_2lep2018_wywong_052020'
        folder = 'FWLJMET102X_2lep2018_wywong_082020'

	locdir = 'hadd_jobs_'+folder
	grp_basedir = '/store/group/lpcljm/'
        rerun_basedir = '/store/user/wywong/'
        basedir = grp_basedir
	#eosdir = basedir+folder+'_hadds/'
        eosdir = '/store/group/lpcljm/'+folder+'_hadds/'

	#################################################
	### Set sample locations inside the folder
	#################################################

	bkg_samples = {} 	
	#bkg_samples['TTW'] = '/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/diLep2018/200920_223825/'#190905_035043/'
	#bkg_samples['TTZ'] = '/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/diLep2018/200920_223928/'#190905_035204/'
	#bkg_samples['TTH'] = '/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8/diLep2018/200921_211136/'#200916_052202/'#190905_034340/'
	#bkg_samples['TTTT'] = '/TTTT_TuneCP5_13TeV-amcatnlo-pythia8/diLep2018/200916_052321/'#190905_034853/'

	#bkg_samples['WWW'] = '/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/diLep2018/200920_224037/'#190905_063741/'
	#bkg_samples['WWZ'] = '/WWZ_TuneCP5_13TeV-amcatnlo-pythia8/diLep2018/200920_224658/'#190905_064242/'
	#bkg_samples['WZZ'] = '/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/diLep2018/200920_224807/'#190905_064405/'
	#bkg_samples['ZZZ'] = '/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/diLep2018/200920_224914/'#190905_072331/'

	#bkg_samples['WpWp'] = '/WpWpJJ_EWK-QCD_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/200916_054522/'
	#bkg_samples['WZ'] = '/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/diLep2018/'#190905_072756/'
        bkg_samples['WZ'] = '/WZTo3LNu_TuneCP5_13TeV-powheg-pythia8/diLep2018/200921_202938/'
	bkg_samples['ZZ'] = '/ZZTo4L_TuneCP5_13TeV_powheg_pythia8/diLep2018/200920_225041/'#190905_072644/'
        #bkg_samples['WJets'] = '/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/diLep2018/200916_053410/'#190905_063523/'       

	signal_samples = {}
	signal_samples['TpTp900']   = '/TprimeTprime_M-900_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/210101_010418/'#201223_041450/'
	#signal_samples['TpTp1000'] = '/TprimeTprime_M-1000_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/200916_054647/'#190904_030455/'
	#signal_samples['TpTp1100'] = '/TprimeTprime_M-1100_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/200916_054753/'#190904_030613/'
	#signal_samples['TpTp1200'] = '/TprimeTprime_M-1200_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/200916_055230/'#190904_030730/'
	#signal_samples['TpTp1300'] = '/TprimeTprime_M-1300_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/200916_055327/'
	#signal_samples['TpTp1400'] = '/TprimeTprime_M-1400_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/200916_055423/'#190904_031227/'
	#signal_samples['TpTp1500'] = '/TprimeTprime_M-1500_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/200916_055139/'#190904_031348/'
	#signal_samples['TpTp1600'] = '/TprimeTprime_M-1600_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/200916_055617/'#190904_031559/'
	#signal_samples['TpTp1700'] = '/TprimeTprime_M-1700_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/200916_055759/'#190904_031713/'
	#signal_samples['TpTp1800'] = '/TprimeTprime_M-1800_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/200916_055856/'#190904_031826/'

        #signal_samples['BpBp900'] = '/BprimeBprime_M-900_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/201201_002312/'
        #signal_samples['BpBp1000'] = '/BprimeBprime_M-1000_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/201201_002456/'
        #signal_samples['BpBp1100'] = '/BprimeBprime_M-1100_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/201203_014741/'
        #signal_samples['BpBp1200'] = '/BprimeBprime_M-1200_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/201203_021342/'
        #signal_samples['BpBp1300'] = '/BprimeBprime_M-1300_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/201201_003544/'
        #signal_samples['BpBp1400'] = '/BprimeBprime_M-1400_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/201201_014639/'
        #signal_samples['BpBp1500'] = '/BprimeBprime_M-1500_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/201201_015756/'
        #signal_samples['BpBp1600'] = '/BprimeBprime_M-1600_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/201201_020447/'
        #signal_samples['BpBp1700'] = '/BprimeBprime_M-1700_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/201201_022756/'
        #signal_samples['BpBp1800'] = '/BprimeBprime_M-1800_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/201201_022912/'



	data_samples = {}
	#data_samples['DoubleMuonRun2018A'] = '/DoubleMuon/diLep2018/200904_001431/'#200812_043139/0000/'#200506_181706/0000/'
        #data_samples['DoubleMuonRun2018A1'] = '/DoubleMuon/diLep2018//'#200812_043139/0001/'#200506_181706/0001/'
	#data_samples['DoubleMuonRun2018B'] = '/DoubleMuon/diLep2018/200904_001829/'#200812_043549/'#200506_181820/'
	#data_samples['DoubleMuonRun2018C'] = '/DoubleMuon/diLep2018/200904_002437/'#200812_043915/'#200506_181920/'
	#data_samples['DoubleMuonRun2018D'] = '/DoubleMuon/diLep2018/200904_002820/'#200812_044015/0000/'#200506_182952/0000/'
        #data_samples['DoubleMuonRun2018D1'] = '/DoubleMuon/diLep2018//'#200812_044015/0001/'#200506_182952/0001/'
        #data_samples['DoubleMuonRun2018D2'] = '/DoubleMuon/diLep2018//'#200812_044015/0002/'#200506_182952/0002/'

        #data_samples['EGammaRun2018A'] = '/EGamma/diLep2018/200903_232905/'#200812_044545/0000/'
        #data_samples['EGammaRun2018A1'] = '/EGamma/diLep2018//'#200812_044545/0001/'
        #data_samples['EGammaRun2018A2'] = '/EGamma/diLep2018//'#200812_044545/0002/'
        #data_samples['EGammaRun2018A3'] = '/EGamma/diLep2018//'#200812_044545/0003/'
        #data_samples['EGammaRun2018A4'] = '/EGamma/diLep2018//'#200812_044545/0004/'
	#data_samples['EGammaRun2018B'] = '/EGamma/diLep2018/200903_233256/'#200826_140228/'#200812_044645/'#200506_181207/'
	#data_samples['EGammaRun2018C'] = '/EGamma/diLep2018/200903_234304/'#200812_044812/0000/'#200506_181305/'
        #data_samples['EGammaRun2018C1'] = '/EGamma/diLep2018//'#200812_044812/0001/'
        #data_samples['EGammaRun2018C2'] = '/EGamma/diLep2018//'#200812_044812/0002/'
	#data_samples['EGammaRun2018D'] = '/EGamma/diLep2018/200904_000353/'#200812_045024/0000/'#200506_181605/0000/'
        #data_samples['EGammaRun2018D1'] = '/EGamma/diLep2018//'#200812_045024/0001/'#200506_181605/0001/'
        #data_samples['EGammaRun2018D2'] = '/EGamma/diLep2018//'#200812_045024/0002/'#200506_181605/0002/'
        #data_samples['EGammaRun2018D3'] = '/EGamma/diLep2018//'#200812_045024/0003/'#200506_181605/0003/'
        #data_samples['EGammaRun2018D4'] = '/EGamma/diLep2018//'#200812_045024/0004/'#200506_181605/0004/'
        #data_samples['EGammaRun2018D5'] = '/EGamma/diLep2018//'#200812_045024/0005/'#200506_181605/0005/'
        #data_samples['EGammaRun2018D6'] = '/EGamma/diLep2018//'#200812_045024/0006/'#200506_181605/0006/'
        #data_samples['EGammaRun2018D7'] = '/EGamma/diLep2018//'#200812_045024/0007/'#200506_181605/0007/'
        #data_samples['EGammaRun2018D8'] = '/EGamma/diLep2018//'#200812_045024/0008/'#200506_181605/0008/'

	#data_samples['MuonEGRun2018A'] = '/MuonEG/diLep2018/200907_154757/'#190927_154737/'
	#data_samples['MuonEGRun2018B'] = '/MuonEG/diLep2018/200907_155149/'#190927_161125/'#190904_034033/'
	#data_samples['MuonEGRun2018C'] = '/MuonEG/diLep2018/200907_155615/'#190927_193636/'#190904_034149/'
	#data_samples['MuonEGRun2018D'] = '/MuonEG/diLep2018/200907_155806/'#190927_194010/'#190904_034300/'
        
        rerun_samples={}
        rerun_samples['WpWp'] = '/WpWpJJ_EWK-QCD_TuneCP5_PSweights_13TeV-madgraph-pythia8/'#diLep2018//'
        rerun_samples['TpTp1300'] = '/TprimeTprime_M-1300_TuneCP5_PSweights_13TeV-madgraph-pythia8/'#diLep2018//'
#        rerun_samples['BpBp1000'] = '/BprimeBprime_M-1000_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/190806_185500/'
#        rerun_samples['BpBp1600']='/BprimeBprime_M-1600_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/190806_185643/'
#        rerun_samples['TpTp1000']='/TprimeTprime_M-1000_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/190806_190121/'
#        rerun_samples['TpTp1100']='/TprimeTprime_M-1100_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/190806_190437/'
#        rerun_samples['TpTp1200']='/TprimeTprime_M-1200_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/190806_190546/'
#        rerun_samples['TpTp1800']='/TprimeTprime_M-1800_TuneCP5_PSweights_13TeV-madgraph-pythia8/diLep2018/190806_190701/'

	#################################################
	### Main driver
	#################################################
	
	#samples to include for submission
	samples_to_process = [
		#data_samples,
		signal_samples,
		#bkg_samples,	
                #rerun_samples,
	]

	#make local directory
	os.system('mkdir -p  %s' %locdir)

	#make eos directory
	os.system('eos root://cmseos.fnal.gov mkdir -p  %s' %eosdir)
		
	for samples in samples_to_process: 
                       
		#Collect paths of the rootfiles
		rootfile_paths = PostLJMetHadd.collect_rootfile_paths(basedir,folder,samples)
	
		#Create condor files
		PostLJMetHadd.create_condor_files_from_template(rel_base,cmssw,locdir,eosdir,samples,rootfile_paths,option.submit)
	
	


