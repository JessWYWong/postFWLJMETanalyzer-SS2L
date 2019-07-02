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

	folder	= 'FWLJMET102X_2lep2017_062719'

	locdir = 'hadd_jobs_'+folder
	basedir = '/store/group/lpcljm/'
	eosdir = basedir+folder+'_hadds/'

	#################################################
	### Set sample locations inside the folder
	#################################################

	bkg_samples = {} 	
	bkg_samples['TTW'] = ''
	bkg_samples['TTZ'] = ''
	bkg_samples['TTH'] = ''
	bkg_samples['TTTT'] = ''

	bkg_samples['WWW'] = ''
	bkg_samples['WWZ'] = ''
	bkg_samples['WZZ'] = ''
	bkg_samples['ZZZ'] = ''

	bkg_samples['WpWp'] = ''
	bkg_samples['WZ'] = ''
	bkg_samples['ZZ'] = ''

	signal_samples = {}
	signal_samples['TpTp1000'] = ''
	signal_samples['TpTp1100'] = ''
	signal_samples['TpTp1200'] = ''
	signal_samples['TpTp1300'] = ''
	signal_samples['TpTp1400'] = ''
	signal_samples['TpTp1500'] = ''
	signal_samples['TpTp1600'] = ''
	signal_samples['TpTp1700'] = ''
	signal_samples['TpTp1800'] = ''

	signal_samples['BpBp1000'] = ''
	signal_samples['BpBp1100'] = ''
	signal_samples['BpBp1200'] = ''
	signal_samples['BpBp1300'] = ''
	signal_samples['BpBp1400'] = ''
	signal_samples['BpBp1500'] = ''
	signal_samples['BpBp1600'] = ''
	signal_samples['BpBp1700'] = ''
	signal_samples['BpBp1800'] = ''


	'''
	FWLJMET102X_2lep2017_062719:

	Task name:			190627_222631:rsyarif_crab_diLep2017_DoubleMuonRun2017B
	Jobs status:                    finished     		100.0% (2/2)

	Task name:			190627_222522:rsyarif_crab_diLep2017_DoubleMuonRun2017C
	Jobs status:                    finished     		100.0% (10/10)

	Task name:			190627_222411:rsyarif_crab_diLep2017_DoubleMuonRun2017D
	Jobs status:                    finished     		100.0% (7/7)

	Task name:			190627_222255:rsyarif_crab_diLep2017_DoubleMuonRun2017E
	Jobs status:                    finished     		100.0% (24/24)

	Task name:			190627_222144:rsyarif_crab_diLep2017_DoubleMuonRun2017F
	Jobs status:                    finished     		100.0% (25/25)



	Task name:			190627_223137:rsyarif_crab_diLep2017_DoubleEGRun2017B
	Jobs status:                    finished     		100.0% (15/15)

	Task name:			190627_223244:rsyarif_crab_diLep2017_DoubleEGRun2017C
	Jobs status:                    finished     		100.0% (40/40)

	Task name:			190627_222759:rsyarif_crab_diLep2017_DoubleEGRun2017D
	Jobs status:                    finished     		100.0% (5/5)

	Task name:			190627_222905:rsyarif_crab_diLep2017_DoubleEGRun2017E
	Jobs status:                    finished     		100.0% (17/17)

	Task name:			190627_223028:rsyarif_crab_diLep2017_DoubleEGRun2017F
	Jobs status:                    finished     		100.0% (23/23)


	Task name:			190627_223418:rsyarif_crab_diLep2017_MuonEGRun2017B
	Jobs status:                    finished     		100.0% (2/2)

	Task name:			190627_223551:rsyarif_crab_diLep2017_MuonEGRun2017C
	Jobs status:                    finished     		100.0% (7/7)

	Task name:			190627_223708:rsyarif_crab_diLep2017_MuonEGRun2017D
	Jobs status:                    finished     		100.0% (3/3)

	Task name:			190627_223823:rsyarif_crab_diLep2017_MuonEGRun2017E
	Jobs status:                    finished     		100.0% (11/11)

	Task name:			190627_223952:rsyarif_crab_diLep2017_MuonEGRun2017F
	Jobs status:                    finished     		100.0% (17/17)
	'''
	
	data_samples = {}
	data_samples['DoubleMuonRun2017B'] = '/DoubleMuon/diLep2017/190627_222631/'
	data_samples['DoubleMuonRun2017C'] = '/DoubleMuon/diLep2017/190627_222522/'
	data_samples['DoubleMuonRun2017D'] = '/DoubleMuon/diLep2017/190627_222411/'
	data_samples['DoubleMuonRun2017E'] = '/DoubleMuon/diLep2017/190627_222255/'
	data_samples['DoubleMuonRun2017F'] = '/DoubleMuon/diLep2017/190627_222144/'

	data_samples['DoubleEGRun2017B'] = '/DoubleEG/diLep2017/190627_223137/'
	data_samples['DoubleEGRun2017C'] = '/DoubleEG/diLep2017/190627_223244/'
	data_samples['DoubleEGRun2017D'] = '/DoubleEG/diLep2017/190627_222759/'
	data_samples['DoubleEGRun2017E'] = '/DoubleEG/diLep2017/190627_222905/'
	data_samples['DoubleEGRun2017F'] = '/DoubleEG/diLep2017/190627_223028/'

	data_samples['MuonEGRun2017B'] = '/MuonEG/diLep2017/190627_223418/'
	data_samples['MuonEGRun2017C'] = '/MuonEG/diLep2017/190627_223551/'
	data_samples['MuonEGRun2017D'] = '/MuonEG/diLep2017/190627_223708/'
	data_samples['MuonEGRun2017E'] = '/MuonEG/diLep2017/190627_223823/'
	data_samples['MuonEGRun2017F'] = '/MuonEG/diLep2017/190627_223952/'
	

	#################################################
	### Main driver
	#################################################
	
	#samples to include for submission
	samples_to_process = [
		data_samples,
		#signal_samples,
		#bkg_samples,	
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
	
	


