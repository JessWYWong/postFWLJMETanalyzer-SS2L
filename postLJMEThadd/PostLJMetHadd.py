#!/usr/bin/python

import os
from eos_utils.EOSSafeUtils import *

def collect_rootfile_paths(basedir,folder,samples):

	'''Collect the paths of the root files of a dataset saved in eos'''

	root_files = {}
	for sample in samples:
	
		sample_dir = basedir+folder+samples[sample]
		dirs = EOSlistdir(sample_dir)
		paths = []

		for dir in dirs:
			files=EOSlist_root_files(sample_dir+dir)
			for file in files:
				paths.append(sample_dir+dir+'/'+file)
				
		root_files[sample] = paths

	return root_files


def create_condor_files_from_template(rel_base,cmssw,locdir,eosdir,samples,rootfile_paths,submit=False):

	'''Create condor jdl and condor bash files based on template files'''

	for sample in samples:
	
		print '\n'+'Creating condor files for:',sample
		print 'rootfiles:\n', 
		rootfiles_to_hadd = ''
		for file in rootfile_paths[sample]: 
			print '\t',file
			rootfiles_to_hadd+=' root://cmseos.fnal.gov/'+file
			
		# Condor file
		condor_templ_file = open(rel_base+"/src/postFWLJMETanalyzer/postLJMEThadd/PostLJMetHadd_condor.templ")
		localcondor = locdir+'/'+sample+".condor"
		condor_file = open(localcondor,"w")
		for line in condor_templ_file:
			line=line.replace('DIRECTORY',locdir)
			line=line.replace('SAMPLENAME',sample)
			condor_file.write(line)
		condor_file.close()

		# Condor executable
		csh_templ_file    = open(rel_base+"/src/postFWLJMETanalyzer/postLJMEThadd/PostLJMetHadd_csh.templ")
		localcsh=locdir+'/'+sample+".csh"
		csh_file = open(localcsh,"w")
		for line in csh_templ_file:
			line=line.replace('CMSSWVERSION',cmssw)
			line=line.replace('HADD_CMD', 'hadd -f '+sample+'_tmp.root '+rootfiles_to_hadd)
			line=line.replace('XRDCP_CMD', 'xrdcp -f '+sample+'_tmp.root root://cmseos.fnal.gov/'+eosdir+'/'+sample+'.root')
			csh_file.write(line)
		csh_file.close()

		print 'condor file is: '+localcondor

		os.system('chmod u+x '+localcsh)

		if (os.path.exists('%s.log'  % localcondor)):
			os.system('rm %s.log' % localcondor)

		#if(submit):os.system('echo condor_submit %s' % localcondor)
		if(submit):os.system('condor_submit %s' % localcondor)


	condor_templ_file.close()
	csh_templ_file.close()


