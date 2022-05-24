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
blind = True
if len(sys.argv) >4 and int(sys.argv[4])==0 : blind = False

#name = limitdir.replace('limits_templatesCR_June2020','').replace('limits_templatesSRCR_June2020','')
path = limitdir+'/'+BR+'/cmb/'+mass

isCR = False
if ("CR1_" in limitdir): isCR = True

filename = 'workspace.root'

options = ''

options = ' --rMin -5 --rMax 5'# --setParameters '+masks
if (not isCR) or blind: options = ' --bypassFrequentistFit --expectSignal 0 '+options

if (not isCR) or blind: options += ' -n _ImpExpSig0'
else: options += ' -n _ImpUnblind'

outJSON = 'impacts.json'
if not blind: outJSON = 'impactsUnblind.json'

os.chdir(path)

print "Running Impacts initial fit"
print 'Command = combineTool.py -M Impacts -d '+filename+' -m '+str(mass)+' --doInitialFit --robustFit 1'+options
os.system('combineTool.py -M Impacts -d '+filename+' -m '+str(mass)+' --doInitialFit --robustFit 1'+options)

print "Running over each nuisance"
print 'Command = combineTool.py -M Impacts -d '+filename+' -m '+str(mass)+' --robustFit 1 --doFits'+options
os.system('combineTool.py -M Impacts -d '+filename+' -m '+str(mass)+' --robustFit 1 --doFits'+options)

print "Making json file"
print 'Command = combineTool.py -M Impacts -d '+filename+' -m '+str(mass)+' -o '+outJSON+options
os.system('combineTool.py -M Impacts -d '+filename+' -m '+str(mass)+' -o '+outJSON+options)
