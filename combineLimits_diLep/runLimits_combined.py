import os,sys
from ROOT import TFile, TObject, RooArgSet

## Arguments: limit directory name; mass point; signal amount to inject; number of toys

## Make a datacard first with datacard.py!

limitdir = sys.argv[1]
BR = sys.argv[2]
path = limitdir+'/'+BR+'/'
os.chdir(path)

if 'bW' in BR:
    sigMass = [1000,1100,1200,1300,1400,1500,1600,1700,1800]
else:
    sigMass = [900,1000,1100,1200,1300,1400,1500,1600,1700,1800]

for mass in sigMass:
    print(mass)
    if os.path.exists('cmb/'+str(mass)) and not os.path.exists('cmb/'+str(mass)+'/morphedWorkspace.root'):
	masks = 'mask_TT_elel2017BF_0_2017=1,mask_TT_elmu2017BF_0_2017=1,mask_TT_mumu2017BF_0_2017=1,mask_TT_elel2018AD_0_2018=1,mask_TT_elmu2018AD_0_2018=1,mask_TT_mumu2018AD_0_2018=1'
        if 'tW' in BR: masks = masks.replace('TT','BB')

        os.chdir('cmb/'+str(mass))

        print "Running Fit Diagnostics for initial workspace with SR channels masked"
        print 'Command = combine -M FitDiagnostics -d workspace.root --saveWorkspace -n Masked --setParameters '+masks
        os.system('combine -M FitDiagnostics -d workspace.root --saveWorkspace -n Masked --setParameters '+masks)

        print "Creating initialFit snapshot file: morphedWorkspace.root"
        w_f = TFile.Open('higgsCombineMasked.FitDiagnostics.mH120.root')
        w = w_f.Get('w')
        fr_f = TFile.Open('fitDiagnosticsMasked.root')
        fr = fr_f.Get('fit_b')
        myargs = RooArgSet(fr.floatParsFinal())
        w.saveSnapshot('initialFit',myargs,True)
        fout = TFile('morphedWorkspace.root', "recreate")
        fout.WriteTObject(w,'w')
        fout.Close()
        w_f.Close()
        fr_f.Close()

        os.chdir('../../')


masks = 'mask_TT_elel2017BF_0_2017=0,mask_TT_elmu2017BF_0_2017=0,mask_TT_mumu2017BF_0_2017=0,mask_TT_elel2018AD_0_2018=0,mask_TT_elmu2018AD_0_2018=0,mask_TT_mumu2018AD_0_2018=0'
if 'tW' in BR: masks = masks.replace('TT','BB')


print 'Running Asymptotic CLs limits for all masses'
print 'Command = combineTool.py -M AsymptoticLimits -d cmb/*/morphedWorkspace.root --snapshotName initialFit --there -n .limit --run=blind --setParameters '+masks
os.system('combineTool.py -M AsymptoticLimits -d cmb/*/morphedWorkspace.root --snapshotName initialFit --there -n .limit --run=blind --setParameters '+masks)

print 'Making a JSON file'
print 'Command = combineTool.py -M CollectLimits cmb/*/*.limit.* --use-dirs -o limits.json'
os.system('combineTool.py -M CollectLimits cmb/*/*.limit.* --use-dirs -o limits.json')

print 'Done!'
