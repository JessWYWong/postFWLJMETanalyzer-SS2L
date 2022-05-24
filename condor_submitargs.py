import os,sys
execfile("/uscms/home/wywong/nobackup/EOSSafeUtils.py") #("/uscms_data/d3/rsyarif/EOSSafeUtils.py")

print 'Getting proxy'
proxyPath=os.popen('voms-proxy-info -path')
proxyPath=proxyPath.readline().strip()

tprimelist = [
#    'TprimeTprime_M-800',
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
#    'BprimeBprime_M-800',
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
eras = ['2017B-F' ]
#eras = ['2017C-F']

bkgdatalist = [
    #'TTTTsig0',
    #'TTTTsig1',
    #'TTTTsig2',
    #'TTTTsig3',
    #'TTTTsig4',

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
    #'X53X53RH_M-1700',

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


    'DataElEl2017B',
    'DataElMu2017B',
    'DataMuMu2017B',
    'DataElEl2017C',
    'DataElMu2017C',
    'DataMuMu2017C',
    'DataElEl2017D',
    'DataElMu2017D',
    'DataMuMu2017D',
    'DataElEl2017E',
    'DataElMu2017E',
    'DataMuMu2017E',
    'DataElEl2017F',
    'DataElMu2017F',
    'DataMuMu2017F',

    
    #'ChargeMisIDElEl2017B',
    #'ChargeMisIDElMu2017B',
    #'ChargeMisIDElEl2017C',
    #'ChargeMisIDElMu2017C',
    #'ChargeMisIDElEl2017D',
    #'ChargeMisIDElMu2017D',
    #'ChargeMisIDElEl2017E',
    #'ChargeMisIDElMu2017E',
    #'ChargeMisIDElEl2017F',
    #'ChargeMisIDElMu2017F',

    #'NonPromptDataElEl2017B',
    #'NonPromptDataElMu2017B',
    #'NonPromptDataMuMu2017B',
    #'NonPromptDataElEl2017C',
    #'NonPromptDataElMu2017C',
    #'NonPromptDataMuMu2017C',
    #'NonPromptDataElEl2017D',
    #'NonPromptDataElMu2017D',
    #'NonPromptDataMuMu2017D',
    #'NonPromptDataElEl2017E',
    #'NonPromptDataElMu2017E',
    #'NonPromptDataMuMu2017E',
    #'NonPromptDataElEl2017F',
    #'NonPromptDataElMu2017F',
    #'NonPromptDataMuMu2017F',
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
#saveDir = 'FWLJMET102X_2lep2017_wywong_082020_hadds_postFWLJMETanalyzer_IsoTrig'
#saveDir = 'FWLJMET102X_2lep2017_wywong_082020_hadds_postFWLJMETanalyzer_IsoTrig_ANv8'
saveDir = 'FWLJMET102X_2lep2017_wywong_082021_hadds_postFWLJMETanalyzer_nonIsoTrig_Aug21_njet3_3LFR_dFRMu2El7_uPR'#HTscan'
#saveDir = 'FWLJMET102X_2lep2017_wywong_082021_hadds_postFWLJMETanalyzer_nonIsoTrig_Aug21_njet3_3LFRminDR_dFRMu3El10'
#saveDir = 'FWLJMET102X_2lep2017_wywong_082021_hadds_postFWLJMETanalyzer_nonIsoTrig_Aug21_njet3_ttFR'

##### added by Jess ######
PRUNCMode = 0
if (PRUNCMode==1) : saveDir += '_ElPRUnity'
elif (PRUNCMode==2) : saveDir += '_MuPRUnity'
##########################
print('dirctory is : ',saveDir)

#condorDir = '/uscms_data/d3/rsyarif/Brown2018/TT_BB_SSDL_Analyzer_2017data/'+saveDir+'_logs/'
condorDir = '/uscms_data/d3/wywong/VLQ/'+saveDir+'_logs/'
if not os.path.exists(condorDir): os.system('mkdir -p '+condorDir)

outputDir = '/store/user/wywong/'+saveDir

if not EOSpathExists(outputDir): os.system('eos root://cmseos.fnal.gov/ mkdir -p '+outputDir)

thisDir = relBase+'/src/postFWLJMETanalyzer'
tarfile = thisDir+'.tar'
print 'Making tar:'

os.system('rm -vi '+relBase+'/src/postFWLJMETanalyzer.tar')
if os.path.exists(tarfile):
    print 'tar already exists! Will not re-tar!'
else:
    os.chdir(thisDir)
    os.chdir('../')
    print 'tar --exclude="postFWLJMETanalyzer/.git" --exclude="postFWLJMETanalyzer/test/PromptRate*" --exclude="postFWLJMETanalyzer/test/ChargeMisID*" --exclude="postFWLJMETanalyzer/test/FakeRate*" --exclude="postFWLJMETanalyzer/test/plot*" --exclude="postFWLJMETanalyzer/Limits*" --exclude="postFWLJMETanalyzer/test/csc2015*"  --exclude="postFWLJMETanalyzer/test/test*" --exclude="postFWLJMETanalyzer/postLJMEThadd/*" --exclude="postFWLJMETanalyzer/test/Systematic*" --exclude="postFWLJMETanalyzer/test/*.root" --exclude="postFWLJMETanalyzer/combineLimits_diLep/*" --exclude="postFWLJMETanalyzer/scripts/Systematics/*" -zvcf '+tarfile+' '+thisDir.split('/')[-1]+'/'
    os.system('tar --exclude="postFWLJMETanalyzer/.git" --exclude="postFWLJMETanalyzer/test/PromptRate*" --exclude="postFWLJMETanalyzer/test/ChargeMisID*" --exclude="postFWLJMETanalyzer/test/FakeRate*" --exclude="postFWLJMETanalyzer/test/plot*" --exclude="postFWLJMETanalyzer/Limits*" --exclude="postFWLJMETanalyzer/test/csc2015*" --exclude="postFWLJMETanalyzer/test/test*" --exclude="postFWLJMETanalyzer/postLJMEThadd/*" --exclude="postFWLJMETanalyzer/test/Systematic*" --exclude="postFWLJMETanalyzer/test/*.root" --exclude="postFWLJMETanalyzer/combineLimits_diLep/*" --exclude="postFWLJMETanalyzer/scripts/Systematics/*" -zvcf '+tarfile+' '+thisDir.split('/')[-1])

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
request_memory = 12000
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

