#!/usr/bin/python

import PostLJMetHadd 
import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--submit",action="store_true")
option = parser.parse_args()


if __name__ == '__main__':

	#################################################
	### Set your folders / parameters
	#################################################
	
	rel_base = os.environ['CMSSW_BASE']
	cmssw = 'CMSSW_9_4_12'

	folder	= 'FWLJMET102X_2lep2017_061419'

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
	Task name:			190621_201224:rsyarif_crab_diLep2017_DoubleMuonRun2017B
	Task name:			190621_201119:rsyarif_crab_diLep2017_DoubleMuonRun2017C
	Task name:			190621_201012:rsyarif_crab_diLep2017_DoubleMuonRun2017D
	Task name:			190621_200904:rsyarif_crab_diLep2017_DoubleMuonRun2017E
	Task name:			190621_200758:rsyarif_crab_diLep2017_DoubleMuonRun2017F

	Task name:			190614_194738:rsyarif_crab_diLep2017_DoubleEGRun2017B
	Task name:			190614_194848:rsyarif_crab_diLep2017_DoubleEGRun2017C
	Task name:			190614_194342:rsyarif_crab_diLep2017_DoubleEGRun2017D
	Task name:			190614_194500:rsyarif_crab_diLep2017_DoubleEGRun2017E
	Task name:			190614_194617:rsyarif_crab_diLep2017_DoubleEGRun2017F

	Task name:			190621_201335:rsyarif_crab_diLep2017_MuonEGRun2017B
	Task name:			190621_201439:rsyarif_crab_diLep2017_MuonEGRun2017C
	Task name:			190621_201552:rsyarif_crab_diLep2017_MuonEGRun2017D
	Task name:			190621_201657:rsyarif_crab_diLep2017_MuonEGRun2017E
	Task name:			190621_201803:rsyarif_crab_diLep2017_MuonEGRun2017F
	'''
	
	data_samples = {}
	data_samples['DoubleMuonRun2017B'] = '/DoubleMuon/diLep2017/190621_201224/'
	data_samples['DoubleMuonRun2017C'] = '/DoubleMuon/diLep2017/190621_201119/'
	data_samples['DoubleMuonRun2017D'] = '/DoubleMuon/diLep2017/190621_201012/'
	data_samples['DoubleMuonRun2017E'] = '/DoubleMuon/diLep2017/190621_200904/'
	data_samples['DoubleMuonRun2017F'] = '/DoubleMuon/diLep2017/190621_200758/'

	data_samples['MuonEGRun2017B'] = '/MuonEG/diLep2017/190621_201335/'
	data_samples['MuonEGRun2017C'] = '/MuonEG/diLep2017/190621_201439/'
	data_samples['MuonEGRun2017D'] = '/MuonEG/diLep2017/190621_201552/'
	data_samples['MuonEGRun2017E'] = '/MuonEG/diLep2017/190621_201657/'
	data_samples['MuonEGRun2017F'] = '/MuonEG/diLep2017/190621_201803/'
	
	data_samples['DoubleEGRun2017B'] = '/DoubleEG/diLep2017/190614_194738/'
	data_samples['DoubleEGRun2017C'] = '/DoubleEG/diLep2017/190614_194848/'
	data_samples['DoubleEGRun2017D'] = '/DoubleEG/diLep2017/190614_194342/'
	data_samples['DoubleEGRun2017E'] = '/DoubleEG/diLep2017/190614_194500/'
	data_samples['DoubleEGRun2017F'] = '/DoubleEG/diLep2017/190614_194617/'

	#################################################
	### Main driver
	#################################################
	
	samples_to_process = [
		data_samples,
		signal_samples,
		bkg_samples,	
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
	
	


