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

	folder	= 'FWLJMET102X_2lepFakeRate2017_062719'

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
	samples['DoubleMuon_FR_Run2017B'] = '/DoubleMuon/diLepFakeRate2017/190627_213729/'
	samples['DoubleMuon_FR_Run2017C'] = '/DoubleMuon/diLepFakeRate2017/190627_213622/'
	samples['DoubleMuon_FR_Run2017D'] = '/DoubleMuon/diLepFakeRate2017/190627_213516/'
	samples['DoubleMuon_FR_Run2017E'] = '/DoubleMuon/diLepFakeRate2017/190627_213405/'
	samples['DoubleMuon_FR_Run2017F'] = '/DoubleMuon/diLepFakeRate2017/190627_213247/'
	
	samples['DoubleEG_FR_Run2017B'] = '/DoubleEG/diLepFakeRate2017/190627_214158/'
	samples['DoubleEG_FR_Run2017C'] = '/DoubleEG/diLepFakeRate2017/190627_214305/'
	samples['DoubleEG_FR_Run2017D'] = '/DoubleEG/diLepFakeRate2017/190627_213843/'
	samples['DoubleEG_FR_Run2017E'] = '/DoubleEG/diLepFakeRate2017/190627_213947/'
	samples['DoubleEG_FR_Run2017F'] = '/DoubleEG/diLepFakeRate2017/190627_214053/'	
	
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
	
	
