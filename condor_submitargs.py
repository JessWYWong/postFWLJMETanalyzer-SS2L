import os,sys
execfile("/uscms/home/wywong/nobackup/EOSSafeUtils.py") #("/uscms_data/d3/rsyarif/EOSSafeUtils.py")

print 'Getting proxy'
proxyPath=os.popen('voms-proxy-info -path')
proxyPath=proxyPath.readline().strip()

tprimelist = [
#    #'TprimeTprime_M-800',
    #'TprimeTprime_M-900',
    #'TprimeTprime_M-1000',
    #'TprimeTprime_M-1100',
    #'TprimeTprime_M-1200',
    #'TprimeTprime_M-1300',
    #'TprimeTprime_M-1400',
    #'TprimeTprime_M-1500',
    #'TprimeTprime_M-1600',
    #'TprimeTprime_M-1700',
    #'TprimeTprime_M-1800',
]

tprimedecays = ['BWBW','TZTZ','THTH','THBW','TZBW','TZTH']
#tprimedecays = ['BWBW']

bprimelist = [
#    #'BprimeBprime_M-800',
    #'BprimeBprime_M-900',
    #'BprimeBprime_M-1000',
    #'BprimeBprime_M-1100',
    #'BprimeBprime_M-1200',
    #'BprimeBprime_M-1300',
    #'BprimeBprime_M-1400',
    #'BprimeBprime_M-1500',
    #'BprimeBprime_M-1600',
    #'BprimeBprime_M-1700',
    #'BprimeBprime_M-1800',
]

bprimedecays = ['TWTW','BZBZ','BHBH','BHTW','BZTW','BZBH']

# eras = ['2016B-D','2016E-H']
#eras = ['2017B-F' ]
#eras = ['2017C-F']
eras = ['2018A-D']

bkgdatalist = [
    #'TTTTsig0',
    #'TTTTsig1',
    #'TTTTsig2',
    #'TTTTsig3',
    #'TTTTsig4',
    #'TTTTsig5',

    #'X53X53LH_M-1100',
    #'X53X53LH_M-1200',
    #'X53X53LH_M-1400',
    #'X53X53LH_M-1500',
    #'X53X53LH_M-1700',

    #'X53X53RH_M-900',
    #'X53X53RH_M-1000',
    #'X53X53RH_M-1100',
    #'X53X53RH_M-1200',
    #'X53X53RH_M-1300',
    #'X53X53RH_M-1400',
    #'X53X53RH_M-1600',
    #'X53X53RH_M-1500',

    'TTH',
    'TTW',
    'TTZ',
    'TTTT',
    'WZ',
    'ZZ',
#    'WWW',
    'WWZ',
#    'WZZ',
#    'ZZZ',
    'WpWp',


    'DataElEl2018A',
    'DataElMu2018A',
    'DataMuMu2018A',
    'DataElEl2018B',
    'DataElMu2018B',
    'DataMuMu2018B',
    'DataElEl2018C',
    'DataElMu2018C',
    'DataMuMu2018C',
    'DataElEl2018D',
    'DataElMu2018D',
    'DataMuMu2018D',

    #'ChargeMisIDElEl2018A',
    #'ChargeMisIDElMu2018A',
    #'ChargeMisIDElEl2018B',
    #'ChargeMisIDElMu2018B',
    #'ChargeMisIDElEl2018C',
    #'ChargeMisIDElMu2018C',
    #'ChargeMisIDElEl2018D',
    #'ChargeMisIDElMu2018D',

    #'NonPromptDataElEl2018A',
    #'NonPromptDataElMu2018A',
    #'NonPromptDataMuMu2018A',
    #'NonPromptDataElEl2018B',
    #'NonPromptDataElMu2018B',
    #'NonPromptDataMuMu2018B',
    #'NonPromptDataElEl2018C',
    #'NonPromptDataElMu2018C',
    #'NonPromptDataMuMu2018C',
    #'NonPromptDataElEl2018D',
    #'NonPromptDataElMu2018D',
    #'NonPromptDataMuMu2018D',
]

relBase = os.environ['CMSSW_BASE']
print 'Relbase:',relBase


# saveDir = 'LJMet94x_2lepTT_2017datasets_2018_11_18_rizki_hadds_Analyzer'
# saveDir = 'LJMet94x_2lepTT_2017datasets_2018_11_18_rizki_hadds_Analyzer_v2_elID2017'
# saveDir = 'LJMet94x_2lepTT_2017datasets_2019_1_13_rizki_hadds_Analyzer_elID2017_IsoTrig' #CHECK isoTrig in X53Analyzer.cc and compile!!
# saveDir = 'LJMet94x_2lepTT_2017datasets_2019_1_13_rizki_hadds_Analyzer_elID2017_NonIsoTrig' #CHECK isoTrig in X53Analyzer.cc and compile!!
# saveDir = 'LJMet94x_2lepTT_2017datasets_2019_2_18_rizki_hadds_Analyzer_elID2017_nonIsoHTtrig' #CHECK nonIso_HTtrig and inputFolder in X53Analyzer.cc and compile!!
#saveDir = 'LJMet94x_2lepTT_2017datasets_2019_3_15_rizki_hadds_Analyzer_elIDv2_nonIsoHTtrig' #CHECK nonIso_HTtrig and inputFolder in X53Analyzer.cc and compile!!
#saveDir = 'FWLJMET102X_2lep2017_wywong_081319_hadds_postFWLJMETanalyzer_elIDv2_IsoTrig'
#saveDir = 'FWLJMET102X_2lep2018_wywong_082020_hadds_postFWLJMETanalyzer_IsoTrig'
#saveDir = 'FWLJMET102X_2lep2018_wywong_082020_hadds_postFWLJMETanalyzer_IsoTrig_HEMveto_ANv8'
#saveDir = 'FWLJMET102X_2lep2018_wywong_082020_hadds_postFWLJMETanalyzer_nonIsoTrig_HEMveto_Aug21'
saveDir = 'FWLJMET102X_2lep2018_wywong_082020_hadds_postFWLJMETanalyzer_nonIsoTrig_HEMveto_Aug21_njet3_3LFR_dFRMu2El8_uPR'#_bTag' #_uPr
#saveDir = 'FWLJMET102X_2lep2018_wywong_082020_hadds_postFWLJMETanalyzer_nonIsoTrig_HEMveto_Aug21_njet3_3LFRminDR_dFRMu8El10'

##### added by Jess ######
PRUNCMode = 0
#if (PRUNCMode==1) : saveDir += '_ElPRUnity'
#elif (PRUNCMode==2) : saveDir += '_MuPRUnity'
##########################
print('dirctory is : ',saveDir)

#condorDir = '/uscms_data/d3/rsyarif/Brown2018/TT_BB_SSDL_Analyzer_2017data/'+saveDir+'_logs/'
condorDir = '/uscms_data/d3/wywong/VLQ/'+saveDir+'_logs/'
if not os.path.exists(condorDir): os.system('mkdir -p '+condorDir)
outputDir = '/store/user/wywong/'+saveDir
if not EOSpathExists(outputDir): os.system('eos root://cmseos.fnal.gov/ mkdir -p '+outputDir)

thisDir = relBase+'/src/postFWLJMETanalyzer_2018'
tarfile = thisDir+'.tar'
print 'Making tar:'

os.system('rm -vi '+relBase+'/src/postFWLJMETanalyzer_2018.tar')
if os.path.exists(tarfile):
    print 'tar already exists! Will not re-tar!'
else:
    os.chdir(thisDir)
    os.chdir('../')
    print 'tar --exclude="postFWLJMETanalyzer_2018/postLJMEThadd/*" --exclude="postFWLJMETanalyzer_2018/.git" --exclude="postFWLJMETanalyzer_2018/test/PromptRate*" --exclude="postFWLJMETanalyzer_2018/test/ChargeMisID*" --exclude="postFWLJMETanalyzer_2018/test/FakeRate*" --exclude="postFWLJMETanalyzer_2018/test/plot*" --exclude="postFWLJMETanalyzer_2018/Limits*" --exclude="postFWLJMETanalyzer_2018/test/csc2015*" --exclude="postFWLJMETanalyzer_2018/test/*.root" --exclude="postFWLJMETanalyzer_2018/combineLimits_diLep/*" --exclude="postFWLJMETanalyzer_2018/scripts/Systematics/*" --exclude="postFWLJMETanalyzer/combineLimits_diLep/*" -zvcf '+tarfile+' '+thisDir.split('/')[-1]+'/'
    os.system('tar --exclude="postFWLJMETanalyzer_2018/postLJMEThadd/*" --exclude="postFWLJMETanalyzer_2018/.git" --exclude="postFWLJMETanalyzer_2018/test/PromptRate*" --exclude="postFWLJMETanalyzer_2018/test/ChargeMisID*" --exclude="postFWLJMETanalyzer_2018/test/FakeRate*" --exclude="postFWLJMETanalyzer_2018/test/plot*" --exclude="postFWLJMETanalyzer_2018/Limits*" --exclude="postFWLJMETanalyzer_2018/test/csc2015*" --exclude="postFWLJMETanalyzer_2018/test/*.root" --exclude="postFWLJMETanalyzer_2018/combineLimits_diLep/*" --exclude="postFWLJMETanalyzer_2018/scripts/Systematics/*" --exclude="postFWLJMETanalyzer/combineLimits_diLep/*" -zvcf '+tarfile+' '+thisDir.split('/')[-1])

count = 0

for sample in tprimelist:
    for decay in tprimedecays:
        for era in eras:

            count+=1
            dict={'RUNDIR':thisDir, 'SAMPLE':sample, 'CONDORDIR':condorDir, 'PROXY':proxyPath, 'ERA':era, 'DECAY':decay, 'TARFILE':tarfile, 'OUTPUTDIR':outputDir, 'PRUNCMODE':PRUNCMode}
            jdfName=condorDir+'/%(SAMPLE)s_%(DECAY)s_%(ERA)s.job'%dict
            print jdfName
            jdf=open(jdfName,'w')
            jdf.write(
                """x509userproxy = %(PROXY)s
universe = vanilla
Executable = %(RUNDIR)s/runLJMETanalyzer.sh
Should_Transfer_Files = YES
WhenToTransferOutput = ON_EXIT
Transfer_Input_Files = %(TARFILE)s
Output = %(SAMPLE)s_%(DECAY)s_%(ERA)s.out
Error = %(SAMPLE)s_%(DECAY)s_%(ERA)s.err
Log = %(SAMPLE)s_%(DECAY)s_%(ERA)s.log
Notification = Never
Arguments = %(SAMPLE)s %(ERA)s %(DECAY)s %(OUTPUTDIR)s %(PRUNCMODE)s
Queue 1"""%dict)
            jdf.close()
            os.chdir('%s/'%(condorDir))
            os.system('condor_submit %(SAMPLE)s_%(DECAY)s_%(ERA)s.job'%dict)
            os.system('sleep 0.5')
            os.chdir('%s'%(thisDir))
            print count, "jobs submitted!!!"


for sample in bprimelist:
    for decay in bprimedecays:
        for era in eras:

            count+=1
            dict={'RUNDIR':thisDir, 'SAMPLE':sample, 'CONDORDIR':condorDir, 'PROXY':proxyPath, 'ERA':era, 'DECAY':decay, 'TARFILE':tarfile, 'OUTPUTDIR':outputDir, 'PRUNCMODE':PRUNCMode}
            jdfName=condorDir+'/%(SAMPLE)s_%(DECAY)s_%(ERA)s.job'%dict
            print jdfName
            jdf=open(jdfName,'w')
            jdf.write(
                """x509userproxy = %(PROXY)s
universe = vanilla
Executable = %(RUNDIR)s/runLJMETanalyzer.sh
Should_Transfer_Files = YES
WhenToTransferOutput = ON_EXIT
Transfer_Input_Files = %(TARFILE)s
Output = %(SAMPLE)s_%(DECAY)s_%(ERA)s.out
Error = %(SAMPLE)s_%(DECAY)s_%(ERA)s.err
Log = %(SAMPLE)s_%(DECAY)s_%(ERA)s.log
Notification = Never
Arguments = %(SAMPLE)s %(ERA)s %(DECAY)s %(OUTPUTDIR)s %(PRUNCMODE)s
Queue 1"""%dict)
            jdf.close()
            os.chdir('%s/'%(condorDir))
            os.system('condor_submit %(SAMPLE)s_%(DECAY)s_%(ERA)s.job'%dict)
            os.system('sleep 0.5')
            os.chdir('%s'%(thisDir))
            print count, "jobs submitted!!!"

decay = ''
for sample in bkgdatalist:
    for era in eras:

        count+=1
        dict={'RUNDIR':thisDir, 'SAMPLE':sample, 'CONDORDIR':condorDir, 'PROXY':proxyPath, 'ERA':era, 'DECAY':decay, 'TARFILE':tarfile, 'OUTPUTDIR':outputDir, 'PRUNCMODE':PRUNCMode}
        jdfName=condorDir+'/%(SAMPLE)s_%(ERA)s.job'%dict
        print jdfName
        jdf=open(jdfName,'w')
        jdf.write(
            """x509userproxy = %(PROXY)s
universe = vanilla
Executable = %(RUNDIR)s/runLJMETanalyzer.sh
Should_Transfer_Files = YES
request_memory = 50000
WhenToTransferOutput = ON_EXIT
Transfer_Input_Files = %(TARFILE)s
Output = %(SAMPLE)s_%(ERA)s.out
Error = %(SAMPLE)s_%(ERA)s.err
Log = %(SAMPLE)s_%(ERA)s.log
Notification = Never
Arguments = %(SAMPLE)s %(ERA)s DUMMYARG %(OUTPUTDIR)s %(PRUNCMODE)s
Queue 1"""%dict)
        jdf.close()
        os.chdir('%s/'%(condorDir))
        os.system('condor_submit %(SAMPLE)s_%(ERA)s.job'%dict)
        os.system('sleep 0.5')
        os.chdir('%s'%(thisDir))
        print count, "jobs submitted!!!"

