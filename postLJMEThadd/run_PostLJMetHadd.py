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

	#folder	= 'FWLJMET102X_2lep2017_wywong_081319'
	#folder = 'FWLJMET102X_2lep2017_wywong_012020'
        folder = 'FWLJMET102X_2lep2017_wywong_082020'

	locdir = 'hadd_jobs_'+folder
	grp_basedir = '/store/group/lpcljm/'
        rerun_basedir = '/store/user/wywong/'
        basedir = grp_basedir
	eosdir = basedir+folder+'_hadds/'
        #eosdir = '/store/group/lpcljm/'+folder+'_hadds/'

	#################################################
	### Set sample locations inside the folder
	#################################################

	bkg_samples = {} 	
	#bkg_samples['TTW'] = '/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/diLep2017/200916_052618/'#200113_222221/'
	#bkg_samples['TTZ'] = '/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/diLep2017/200916_052721/'#200113_233604/'
	#bkg_samples['TTH'] = '/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8/diLep2017/200917_202349/'#200916_051240/'#200114_001248/'
	#bkg_samples['TTTT'] = '/TTTT_TuneCP5_13TeV-amcatnlo-pythia8/diLep2017/200917_204644/'#200916_051442/'#200113_234057/'

	#bkg_samples['WWW'] = '/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/diLep2017/200917_220331/'#200916_051828/'#200113_221458/' #190905_063741/'
	#bkg_samples['WWZ'] = '/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/diLep2017/200917_220447/'#200916_051946/'#200113_221905/'#190905_064242/'
	#bkg_samples['WZZ'] = '/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/diLep2017/200917_220621/'#200916_052218/'#200113_222020/'#190905_064405/'
	#bkg_samples['ZZZ'] = '/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/diLep2017/200917_220921/'#200916_053426/'#200113_222123/'#190905_072331/'

	#bkg_samples['WpWp'] = '/WpWpJJ_EWK-QCD_TuneCP5_13TeV-madgraph-pythia8/diLep2017/200916_052329/'#200114_001643/'
	#bkg_samples['WZ'] = '/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/diLep2017/200916_052116/'#200113_221252/'#190905_072756/'
	#bkg_samples['ZZ'] = '/ZZTo4L_13TeV_powheg_pythia8/diLep2017/200916_052821/'#200113_221402/'
        #bkg_samples['WJets'] = '/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/diLep2017/200916_052506/'#200114_002252/'#190905_063523/'       
        #bkg_samples['DYM50'] = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/diLep2017/201125_071529/'
        #bkg_samples['TTTo2L2Nu'] = '/TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8/diLep2017/201125_071800/'
        #bkg_samples['TTToSemiLeptonic'] = '/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8/diLep2017/201125_071637/'
        #bkg_samples['TTToHadronic'] = '/TTToHadronic_TuneCP5_PSweights_13TeV-powheg-pythia8/diLep2017/201125_071932/'

	signal_samples = {}
        signal_samples['TpTp700'] = '/TprimeTprime_M-700_TuneCP5_13TeV-madgraph-pythia8/diLep2017/210101_010147/'#201223_041055/'
	#signal_samples['TpTp1000'] = '/TprimeTprime_M-1000_TuneCP5_13TeV-madgraph-pythia8/diLep2017/200916_053646/'#200212_184401/'#200114_193547/'#190904_030455/'
	#signal_samples['TpTp1100'] = '/TprimeTprime_M-1100_TuneCP5_13TeV-madgraph-pythia8/diLep2017/200916_054050/'#2200114_193647/'#190904_030613/'
	#signal_samples['TpTp1200'] = '/TprimeTprime_M-1200_TuneCP5_13TeV-madgraph-pythia8/diLep2017/200916_054203/'#2200212_184548/'#190904_030730/'
	#signal_samples['TpTp1300'] = '/TprimeTprime_M-1300_TuneCP5_13TeV-madgraph-pythia8/diLep2017/200916_054311/'#2200114_202133/'
	#signal_samples['TpTp1400'] = '/TprimeTprime_M-1400_TuneCP5_13TeV-madgraph-pythia8/diLep2017/200916_054433/'#2200212_184944/'#200114_210310/'#190904_031227/'
	#signal_samples['TpTp1500'] = '/TprimeTprime_M-1500_TuneCP5_13TeV-madgraph-pythia8/diLep2017/200916_053834/'#2200212_185432/'#200114_211136/'#190904_031348/'
	#signal_samples['TpTp1600'] = '/TprimeTprime_M-1600_TuneCP5_13TeV-madgraph-pythia8/diLep2017/200916_054607/'#2200114_220423/'#190904_031559/'
	#signal_samples['TpTp1700'] = '/TprimeTprime_M-1700_TuneCP5_13TeV-madgraph-pythia8/diLep2017/200916_054752/'#2200114_222739/'#190904_031713/'
	#signal_samples['TpTp1800'] = '/TprimeTprime_M-1800_TuneCP5_13TeV-madgraph-pythia8/diLep2017/200916_055135/'#2200212_185604/'#200114_223102/'#190904_031826/'

	#signal_samples['BpBp900']  = '/BprimeBprime_M-900_TuneCP5_13TeV-madgraph-pythia8/diLep2017/201130_234143/'
	#signal_samples['BpBp1000'] = '/BprimeBprime_M-1000_TuneCP5_13TeV-madgraph-pythia8/diLep2017/201203_005622/'
	#signal_samples['BpBp1100'] = '/BprimeBprime_M-1100_TuneCP5_13TeV-madgraph-pythia8/diLep2017/201201_000817/'
	#signal_samples['BpBp1200'] = '/BprimeBprime_M-1200_TuneCP5_13TeV-madgraph-pythia8/diLep2017/201201_000936/'
	#signal_samples['BpBp1300'] = '/BprimeBprime_M-1300_TuneCP5_13TeV-madgraph-pythia8/diLep2017/201203_010032/'
	#signal_samples['BpBp1400'] = '/BprimeBprime_M-1400_TuneCP5_13TeV-madgraph-pythia8/diLep2017/201201_001327/'
	#signal_samples['BpBp1500'] = '/BprimeBprime_M-1500_TuneCP5_13TeV-madgraph-pythia8/diLep2017/201201_001523/'
	#signal_samples['BpBp1600'] = '/BprimeBprime_M-1600_TuneCP5_13TeV-madgraph-pythia8/diLep2017/201201_001628/'
	#signal_samples['BpBp1700'] = '/BprimeBprime_M-1700_TuneCP5_13TeV-madgraph-pythia8/diLep2017/201203_013931/'
	#signal_samples['BpBp1800'] = '/BprimeBprime_M-1800_TuneCP5_13TeV-madgraph-pythia8/diLep2017/201206_034856/'


	data_samples = {}
	#data_samples['DoubleMuonRun2017B'] = '/DoubleMuon/diLep2017/200917_205632/'#200831_135332/'#200811_100637/'#200110_234654/'#190904_033203/'
	#data_samples['DoubleMuonRun2017C'] = '/DoubleMuon/diLep2017/200917_210001/'#200831_135554/'#200811_151918/'#200110_234810/'#190904_033414/'
	#data_samples['DoubleMuonRun2017D'] = '/DoubleMuon/diLep2017/200917_210218/'#200831_135828/'#200811_152011/'#200111_001019/'#190904_033536/'
	#data_samples['DoubleMuonRun2017E'] = '/DoubleMuon/diLep2017/200917_211059/'#200831_143817/'#200811_152106/'#200111_003220/'#190904_033717/'
	#data_samples['DoubleMuonRun2017F'] = '/DoubleMuon/diLep2017/200917_212411/'#200831_145908/'#200811_152203/'#200111_004034/'#190904_033842/'

	#data_samples['DoubleEGRun2017B'] = '/DoubleEG/diLep2017/200917_205020/'#200831_090309/'#200811_061105/'#200107_212547/'#190904_031950/'
	#data_samples['DoubleEGRun2017C'] = '/DoubleEG/diLep2017/200917_205124/'#200831_090741/'#200811_151226/'#200108_212545/'#190904_032250/'
	#data_samples['DoubleEGRun2017D'] = '/DoubleEG/diLep2017/200917_205245/'#200831_093210/'#200811_151408/'#200108_214212/'#190904_032624/'
	#data_samples['DoubleEGRun2017E'] = '/DoubleEG/diLep2017/200917_205346/'#200831_093327/'#200811_151703/'#200108_214339/'#190904_032906/'
	#data_samples['DoubleEGRun2017F'] = '/DoubleEG/diLep2017/200917_205520/'#200831_135008/'#200811_151759/'#200108_214526/'#190904_033027/'

	#data_samples['MuonEGRun2017B'] = '/MuonEG/diLep2017/200917_214522/'#200907_154103/'#200826_112047/'#200111_004146/'#190904_034033/'
	#data_samples['MuonEGRun2017C'] = '/MuonEG/diLep2017/200917_214703/'#200907_154247/'#200826_120828/'#200111_004351/'#190904_034149/'
	#data_samples['MuonEGRun2017D'] = '/MuonEG/diLep2017/200917_215953/'#200907_154421/'#200826_125835/'#200111_005231/'#190904_034300/'
	#data_samples['MuonEGRun2017E'] = '/MuonEG/diLep2017/200917_220112/'#200907_154520/'#200826_130110/'#200111_005338/'#190904_034417/'
	#data_samples['MuonEGRun2017F'] = '/MuonEG/diLep2017/200917_220226/'#200907_154649/'#200826_132415/'#200111_005446/'#190905_034202/'
        
#        rerun_samples={}
#        rerun_samples['WpWp'] = '/WpWpJJ_EWK-QCD_TuneCP5_13TeV-madgraph-pythia8/'#diLep2017//'
#        rerun_samples['TpTp1300'] = '/TprimeTprime_M-1300_TuneCP5_13TeV-madgraph-pythia8/'#diLep2017//'
#        rerun_samples['BpBp1000'] = '/BprimeBprime_M-1000_TuneCP5_13TeV-madgraph-pythia8/diLep2017/190806_185500/'
#        rerun_samples['BpBp1600']='/BprimeBprime_M-1600_TuneCP5_13TeV-madgraph-pythia8/diLep2017/190806_185643/'
#        rerun_samples['TpTp1000']='/TprimeTprime_M-1000_TuneCP5_13TeV-madgraph-pythia8/diLep2017/190806_190121/'
#        rerun_samples['TpTp1100']='/TprimeTprime_M-1100_TuneCP5_13TeV-madgraph-pythia8/diLep2017/190806_190437/'
#        rerun_samples['TpTp1200']='/TprimeTprime_M-1200_TuneCP5_13TeV-madgraph-pythia8/diLep2017/190806_190546/'
#        rerun_samples['TpTp1800']='/TprimeTprime_M-1800_TuneCP5_13TeV-madgraph-pythia8/diLep2017/190806_190701/'

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
	
	


