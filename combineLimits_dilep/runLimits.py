import os,sys
from ROOT import TFile, TObject, RooArgSet

## Arguments: limit directory name; mass point; signal amount to inject; number of toys

## Make a datacard first with datacard.py!

limitdir = sys.argv[1]
BR = sys.argv[2]
blind = False
path = limitdir+'/'+BR+'/'
os.chdir(path)


print 'Running Asymptotic CLs limits for all masses'
if blind:
    print 'Command = combineTool.py -M AsymptoticLimits -d cmb/*/workspace.root --there -n .limit --run=blind'# --setParameters '+masks
    os.system('combineTool.py -M AsymptoticLimits -d cmb/*/workspace.root --there -n .limit --run=blind')# --setParameters '+masks)
else:
    print 'Command = combineTool.py -M AsymptoticLimits -d cmb/*/workspace.root --there -n .limit'
    os.system('combineTool.py -M AsymptoticLimits -d cmb/*/workspace.root --there -n .limit')

print 'Making a JSON file'
print 'Command = combineTool.py -M CollectLimits cmb/*/*.limit.* --use-dirs -o limits.json'
os.system('combineTool.py -M CollectLimits cmb/*/*.limit.* --use-dirs -o limits.json')

print 'Done!'
