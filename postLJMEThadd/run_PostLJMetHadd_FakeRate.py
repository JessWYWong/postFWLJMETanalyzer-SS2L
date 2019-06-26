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

	folder	= 'FWLJMET102X_2lepFakeRate2017_062419'

	locdir = 'hadd_jobs_'+folder
	basedir = '/store/group/lpcljm/'
	eosdir = basedir+folder+'_hadds/'

	#################################################
	### Set sample locations inside the folder
	#################################################

	samples = {} 
	samples['DoubleMuon_FR_Run2017B'] = '/DoubleMuon/diLepFakeRate2017//'
	samples['DoubleMuon_FR_Run2017C'] = '/DoubleMuon/diLepFakeRate2017//'
	samples['DoubleMuon_FR_Run2017D'] = '/DoubleMuon/diLepFakeRate2017//'
	samples['DoubleMuon_FR_Run2017E'] = '/DoubleMuon/diLepFakeRate2017//'
	samples['DoubleMuon_FR_Run2017F'] = '/DoubleMuon/diLepFakeRate2017//'
	
	samples['DoubleEG_FR_Run2017B'] = '/DoubleEG/diLepFakeRate2017//'
	samples['DoubleEG_FR_Run2017C'] = '/DoubleEG/diLepFakeRate2017//'
	samples['DoubleEG_FR_Run2017D'] = '/DoubleEG/diLepFakeRate2017//'
	samples['DoubleEG_FR_Run2017E'] = '/DoubleEG/diLepFakeRate2017//'
	samples['DoubleEG_FR_Run2017F'] = '/DoubleEG/diLepFakeRate2017//'	
	
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
	
	
