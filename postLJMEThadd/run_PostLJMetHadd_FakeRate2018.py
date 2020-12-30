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

	#folder	= 'FWLJMET102X_2lepFR2018_min1Jet_wywong_052020'
	folder = 'FWLJMET102X_2lepFakeRate2018_wywong_082020'

	locdir = 'hadd_jobs_'+folder
	basedir = '/store/group/lpcljm/'
	eosdir = basedir+folder+'_hadds/' #'/store/user/wywong/'+folder+'_hadds/'

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
	#samples['DoubleMuon_FR_Run2018A'] = '/DoubleMuon/diLepFakeRate2018/200903_232008/'#200521_183533/'
	#samples['DoubleMuon_FR_Run2018B'] = '/DoubleMuon/diLepFakeRate2018/200903_232343/'#200521_185154/'
	#samples['DoubleMuon_FR_Run2018C'] = '/DoubleMuon/diLepFakeRate2018/200903_232608/'#200521_192322/'
	#samples['DoubleMuon_FR_Run2018D'] = '/DoubleMuon/diLepFakeRate2018/200903_232739/'#200521_204701/'

	#samples['DoubleEG_FR_Run2018A'] = '/EGamma/diLepFakeRate2018/200903_231049/'#200520_214705/'	
	#samples['DoubleEG_FR_Run2018B'] = '/EGamma/diLepFakeRate2018/200903_231427/'#200521_182610/'
	#samples['DoubleEG_FR_Run2018C'] = '/EGamma/diLepFakeRate2018/200903_231550/'#200521_182937/'
	#samples['DoubleEG_FR_Run2018D'] = '/EGamma/diLepFakeRate2018/200903_231825/'#200521_183323/'

	samples['SingleElectronRun2018A'] = '/EGamma/diLepFakeRate2018/200925_211110/'
	samples['SingleElectronRun2018B'] = '/EGamma/diLepFakeRate2018/200925_211319/'
	samples['SingleElectronRun2018C'] = '/EGamma/diLepFakeRate2018/200925_213143/'
	samples['SingleElectronRun2018D'] = '/EGamma/diLepFakeRate2018/200925_214133/'

	samples['SingleMuonRun2018A'] = '/SingleMuon/diLepFakeRate2018/200925_215707/'
	samples['SingleMuonRun2018B'] = '/SingleMuon/diLepFakeRate2018/200925_215911/'
	samples['SingleMuonRun2018C'] = '/SingleMuon/diLepFakeRate2018/200926_163807/'
	samples['SingleMuonRun2018D'] = '/SingleMuon/diLepFakeRate2018/200926_163956/'
	
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
	
	
