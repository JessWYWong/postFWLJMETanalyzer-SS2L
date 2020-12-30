#!/usr/bin/python

import os
from eos_utils.EOSSafeUtils import *

def collect_rootfile_paths(basedir,folder,samples):

	'''Collect the paths of the root files of a dataset saved in eos'''

	root_files = {}
	for sample in samples:
	
		sample_dir = basedir+folder+samples[sample]
		dirs = []
		paths = []

		files= EOSlistdir(sample_dir)
		for file in files :
			if '.root' in file and os.path.getsize('/eos/uscms'+sample_dir+'/'+file)>1000:
				paths.append(sample_dir+'/'+file)
		dirs = EOSlistdir(sample_dir)
		if len(paths)>0: dirs = []

		for Dir in dirs:
                        print 'dir is ', Dir
			files=EOSlist_root_files(sample_dir+Dir)
			for file in files:
				if '.root' in file and os.path.getsize('/eos/uscms'+sample_dir+Dir+'/'+file)>1000:
					 paths.append(sample_dir+Dir+'/'+file)
					
			subdirs = [] #EOSlistdir(sample_dir+Dir) # EOSlistdir list all files and do not check if it is a directory
                        while len(subdirs) == 1  and ".root" not in subdirs[0]:
 				print subdirs[0]
                        	Dir =Dir+"/"+subdirs[0]
				subdirs = EOSlistdir(sample_dir+Dir)
                        if len(EOSlistSubdirs(sample_dir+Dir))>1: #EOSlistSubdirs only list directories, unlike EOSlistdir
                                print EOSlistSubdirs(sample_dir+Dir)
				print "Multiple sub directories found --> specify which to use in sample list in run_PostLJMetHadd.py"
			if len(subdirs)!=0 : 
				print 'Full path of dir is ', Dir
                        	files=EOSlist_root_files(sample_dir+Dir)
                        #for subdir in subdirs:
                        #        if '.root' in subdir:
                        #                 continue    
                        #        print 'subdir is ', subdir                   
			#	files=EOSlist_root_files(sample_dir+Dir+'/'+subdir)
				for file in files:
					if '.root' in file and os.path.getsize('/eos/uscms'+sample_dir+Dir+'/'+file)>0: 
						paths.append(sample_dir+Dir+'/'+file)
				
		root_files[sample] = paths

	return root_files


def create_condor_files_from_template(rel_base,cmssw,locdir,eosdir,samples,rootfile_paths,submit=False):

	'''Create condor jdl and condor bash files based on template files'''

	for sample in samples:
	
		print '\n'+'Creating condor files for:',sample
		print 'rootfiles:\n', 
		rootfiles_to_hadd = ''
		#if os.path.getsize('/eos/uscms'+rootfile_paths[sample][0])<os.path.getsize('/eos/uscms'+rootfile_paths[sample][-1]) and os.path.getsize('/eos/uscms'+rootfile_paths[sample][0])< 1000000:
		#	rootfile_paths[sample] = reversed(rootfile_paths[sample])
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


