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

	folder	= 'FWLJMET102X_2lepFakeRate2017_wywong_082020'

	locdir = 'hadd_jobs_'+folder
	basedir = '/store/group/lpcljm/'
	eosdir = basedir+folder+'_hadds/'

	#################################################
	### Set sample locations inside the folder
	#################################################

	'''
	FWLJMET102X_2lepFakeRate2017_062719

	Task name:			190627_213729:rsyarif_crab_diLepFakeRate2017_DoubleMuonRun2017B
	Jobs status:                    finished     		100.0% (2/2)

	Task name:			190627_213622:rsyarif_crab_diLepFakeRate2017_DoubleMuonRun2017C
	Jobs status:                    finished     		100.0% (10/10)

	Task name:			190627_213516:rsyarif_crab_diLepFakeRate2017_DoubleMuonRun2017D
	Jobs status:                    finished     		100.0% (5/5)

	Task name:			190627_213405:rsyarif_crab_diLepFakeRate2017_DoubleMuonRun2017E
	Jobs status:                    finished     		100.0% (24/24)

	Task name:			190627_213247:rsyarif_crab_diLepFakeRate2017_DoubleMuonRun2017F
	Jobs status:                    finished     		100.0% (12/12)



	Task name:			190627_214158:rsyarif_crab_diLepFakeRate2017_DoubleEGRun2017B
	Jobs status:                    finished     		100.0% (4/4)

	Task name:			190627_214305:rsyarif_crab_diLepFakeRate2017_DoubleEGRun2017C
	Jobs status:                    finished     		100.0% (4/4)

	Task name:			190627_213843:rsyarif_crab_diLepFakeRate2017_DoubleEGRun2017D
	Jobs status:                    finished     		100.0% (3/3)

	Task name:			190627_213947:rsyarif_crab_diLepFakeRate2017_DoubleEGRun2017E
	Jobs status:                    finished     		100.0% (4/4)

	Task name:			190627_214053:rsyarif_crab_diLepFakeRate2017_DoubleEGRun2017F
	Jobs status:                    finished     		100.0% (12/12)


	'''

	samples = {} 
	#samples['DoubleMuon_FR_Run2017B'] = '/DoubleMuon/diLepFakeRate2017/200917_200735/'#200831_093149/'#191029_211359/'
	#samples['DoubleMuon_FR_Run2017C'] = '/DoubleMuon/diLepFakeRate2017/200917_200836/'#200831_093313/'#191101_174700/'
	#samples['DoubleMuon_FR_Run2017D'] = '/DoubleMuon/diLepFakeRate2017/200917_200946/'#200831_134723/'#191030_215158/'
	#samples['DoubleMuon_FR_Run2017E'] = '/DoubleMuon/diLepFakeRate2017/200917_201104/'#200831_094021/'#191030_221223/'
	#samples['DoubleMuon_FR_Run2017F'] = '/DoubleMuon/diLepFakeRate2017/200917_201700/'#200831_094148/'#191030_221615/'
	
	#samples['DoubleEG_FR_Run2017B'] = '/DoubleEG/diLepFakeRate2017/200917_195204/'#200831_084824/'#191029_210330/'
	#samples['DoubleEG_FR_Run2017C'] = '/DoubleEG/diLepFakeRate2017/200917_195813/'#200831_085551/'#191029_210547/'
	#samples['DoubleEG_FR_Run2017D'] = '/DoubleEG/diLepFakeRate2017/200917_195944/'#200831_085753/'#191029_210727/'
	#samples['DoubleEG_FR_Run2017E'] = '/DoubleEG/diLepFakeRate2017/200917_200114/'#200831_090253/'#191029_210909/'
	#samples['DoubleEG_FR_Run2017F'] = '/DoubleEG/diLepFakeRate2017/200917_200233/'#200831_090554/'#191029_211138/'	
	
	#samples['SingleElectronRun2017B'] = '/SingleElectron/diLepFakeRate2017/200923_190019/'
	#samples['SingleElectronRun2017C'] = '/SingleElectron/diLepFakeRate2017/200923_190304/'
	#samples['SingleElectronRun2017D'] = '/SingleElectron/diLepFakeRate2017/200923_192021/'
	#samples['SingleElectronRun2017E'] = '/SingleElectron/diLepFakeRate2017/200923_192221/'
	#samples['SingleElectronRun2017F'] = '/SingleElectron/diLepFakeRate2017/200923_193042/'

	#samples['SingleMuonRun2017B'] = '/SingleMuon/diLepFakeRate2017/200923_193712/'
	#samples['SingleMuonRun2017C'] = '/SingleMuon/diLepFakeRate2017/200923_193910/'
	#samples['SingleMuonRun2017D'] = '/SingleMuon/diLepFakeRate2017/200923_194037/'
	#samples['SingleMuonRun2017E'] = '/SingleMuon/diLepFakeRate2017/200923_194659/'
	#samples['SingleMuonRun2017F'] = '/SingleMuon/diLepFakeRate2017/200923_194800/'

        samples['TTTo2L2Nu'] = '/TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8/diLepFakeRate2017/201125_072328/'
        samples['TTToSemiLeptonic'] = '/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8/diLepFakeRate2017/201125_072442/'
        samples['TTToHadronic'] = '/TTToHadronic_TuneCP5_PSweights_13TeV-powheg-pythia8/diLepFakeRate2017/201125_072631/'


	#################################################
	### Main driver
	#################################################
	
	#make local directory
	os.system('mkdir -p  %s' %locdir)

	#make eos directory
	os.system('eos root://cmseos.fnal.gov mkdir -p  %s' %eosdir)
	
	#Collect paths of the rootfiles
	rootfile_paths = PostLJMetHadd.collect_rootfile_paths(basedir,folder,samples)
	
	#Create condor files
	PostLJMetHadd.create_condor_files_from_template(rel_base,cmssw,locdir,eosdir,samples,rootfile_paths,option.submit)
	
	
