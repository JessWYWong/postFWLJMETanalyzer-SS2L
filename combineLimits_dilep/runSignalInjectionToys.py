import os,sys
from ROOT import TFile, TObject, RooArgSet

## Arguments: limit directory name; mass point; signal amount to inject; number of toys

## Make a datacard first with datacard.py!

blind = True
if(len(sys.argv) > 5):
    if(int(sys.argv[5])==0): blind = False
    else: blind = True
cwd = os.getcwd()

limitdir = sys.argv[1]
mass = sys.argv[2]
rInj = float(sys.argv[3])
nToys = int(sys.argv[4])

BR = 'bW0p5_tZ0p25_tH0p25'
if 'BB' in limitdir: BR = 'tW0p5_bZ0p25_bH0p25'
mass = mass.replace('BB','')

name = limitdir.replace('limits_templates_','')+'InjR'+str(rInj).replace(".","p")
if not blind : name = name+"Unblind"
path = limitdir+'/'+BR+'/cmb/'+mass

os.chdir(path)

filename = "workspace.root"

OptStr = ""
if blind : ' --bypassFrequentistFit -t '+str(nToys)

## Toy generation is SR-safe, it throws toys off fit_b, which will be CR-data-only from masking. --bypassFrequentistFit should be bypassing any re-fitting

## Toy fits again should be ok, since it's just fitting the previously-made toys
print "Fitting toys...."

print 'Command = combine -M FitDiagnostics -d '+filename+OptStr+' --expectSignal '+str(rInj)+' --rMin '+str(rInj-10)+' --rMax '+str(rInj+10)+' -n '+name
os.system('combine -M FitDiagnostics -d '+filename+OptStr+' --expectSignal '+str(rInj)+' --rMin '+str(rInj-10)+' --rMax '+str(rInj+10)+' -n '+name)

print "Done!"
