import os,sys
from ROOT import TFile, TObject, RooArgSet

## Arguments: limit directory name; mass point; signal amount to inject; number of toys

## Make a datacard first with datacard.py!

limitdir = sys.argv[1]
mass = sys.argv[2]

BR = 'bW0p5_tZ0p25_tH0p25'
if 'BB' in mass: BR = 'tW0p5_bZ0p25_bH0p25'
mass = mass.replace('BB','')

if len(sys.argv) >3 : BR = sys.argv[3]

#name = limitdir.replace('limits_templatesCR_June2020','').replace('limits_templatesSRCR_June2020','')
path = limitdir+'/'+BR+'/cmb/'+mass

isSR = True
if 'CR' in limitdir: isSR = False

filename = 'workspace.root'
options = ''
if isSR: 
    masks = 'mask_TT_elel2017BF_0_2017=0,mask_TT_elmu2017BF_0_2017=0,mask_TT_mumu2017BF_0_2017=0'
    if 'tW' in BR: masks = masks.replace('TT','BB') 
    #masks = 'mask_TT_isSR_isE_notV01T1H_DeepAK8_0_2018=0,mask_TT_isSR_isE_notV01T2pH_DeepAK8_0_2018=0,mask_TT_isSR_isE_notV0T0H0Z01W_DeepAK8_0_2018=0,mask_TT_isSR_isE_notV0T0H0Z2pW_DeepAK8_0_2018=0,mask_TT_isSR_isE_notV0T0H1pZ_DeepAK8_0_2018=0,mask_TT_isSR_isE_notV1T0H_DeepAK8_0_2018=0,mask_TT_isSR_isE_notV2pT_DeepAK8_0_2018=0,mask_TT_isSR_isE_notVbW_DeepAK8_0_2018=0,mask_TT_isSR_isE_notVtH_DeepAK8_0_2018=0,mask_TT_isSR_isE_notVtZ_DeepAK8_0_2018=0,mask_TT_isSR_isE_taggedbWbW_DeepAK8_0_2018=0,mask_TT_isSR_isE_taggedtHbW_DeepAK8_0_2018=0,mask_TT_isSR_isE_taggedtZHtZH_DeepAK8_0_2018=0,mask_TT_isSR_isE_taggedtZbW_DeepAK8_0_2018=0,mask_TT_isSR_isM_notV01T1H_DeepAK8_0_2018=0,mask_TT_isSR_isM_notV01T2pH_DeepAK8_0_2018=0,mask_TT_isSR_isM_notV0T0H0Z01W_DeepAK8_0_2018=0,mask_TT_isSR_isM_notV0T0H0Z2pW_DeepAK8_0_2018=0,mask_TT_isSR_isM_notV0T0H1pZ_DeepAK8_0_2018=0,mask_TT_isSR_isM_notV1T0H_DeepAK8_0_2018=0,mask_TT_isSR_isM_notV2pT_DeepAK8_0_2018=0,mask_TT_isSR_isM_notVbW_DeepAK8_0_2018=0,mask_TT_isSR_isM_notVtH_DeepAK8_0_2018=0,mask_TT_isSR_isM_notVtZ_DeepAK8_0_2018=0,mask_TT_isSR_isM_taggedbWbW_DeepAK8_0_2018=0,mask_TT_isSR_isM_taggedtHbW_DeepAK8_0_2018=0,mask_TT_isSR_isM_taggedtZHtZH_DeepAK8_0_2018=0,mask_TT_isSR_isM_taggedtZbW_DeepAK8_0_2018=0'
    #if 'tW' in BR: masks = masks.replace(',mask_TT_isSR_isE_taggedtZHtZH_DeepAK8_0_2018=0','').replace(',mask_TT_isSR_isM_taggedtZHtZH_DeepAK8_0_2018=0','').replace('bW','tW').replace('tZ','bZ').replace('tH','bH').replace('TT','BB')

    filename = 'morphedWorkspace.root'
    options = ' --snapshotName initialFit --bypassFrequentistFit -t -1 --expectSignal 0 --setParameters '+masks


os.chdir(path)

print "Running Impacts initial fit"
print 'Command = combineTool.py -M Impacts -d '+filename+' -m '+str(mass)+' --doInitialFit --robustFit 1'+options
os.system('combineTool.py -M Impacts -d '+filename+' -m '+str(mass)+' --doInitialFit --robustFit 1'+options)

print "Running over each nuisance"
print 'Command = combineTool.py -M Impacts -d '+filename+' -m '+str(mass)+' --robustFit 1 --doFits'+options
os.system('combineTool.py -M Impacts -d '+filename+' -m '+str(mass)+' --robustFit 1 --doFits'+options)

print "Making json file"
print 'Command = combineTool.py -M Impacts -d '+filename+' -m '+str(mass)+' -o impacts.json'+options
os.system('combineTool.py -M Impacts -d '+filename+' -m '+str(mass)+' -o impacts.json'+options)
