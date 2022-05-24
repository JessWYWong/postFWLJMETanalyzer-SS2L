import os,sys
from ROOT import TFile, TObject, RooArgSet

## Arguments: limit directory name; mass point; number of toys; (optional) use minos;

## Make a datacard first with datacard.py!

limitdir = sys.argv[1]
mass = sys.argv[2]
nToys = int(sys.argv[3])
blind = True
if len(sys.argv)>4 and int(sys.argv[4]) == 0 : blind = False
useMinos = False
minosOpt = ''
if len(sys.argv)>5: 
    useMinos = True
    minosOpt = sys.argv[5]

useGenerateOnly = False

isCR = False
#if ("CR_" in limitdir): isCR = True

filename = "workspace.root"
if (isCR) : filename = 'initialFitWorkspace.root'
name = "Nuisance"
if not blind : name += "Unblind"
if (useGenerateOnly) : name+="GenerateOnly"
rInj = 0

BR = 'bW0p5_tZ0p25_tH0p25'
if 'BB' in mass: BR = 'tW0p5_bZ0p25_bH0p25'
mass = mass.replace('BB','')

path = limitdir+'/'+BR+'/cmb/'+mass

os.chdir(path)

minosOptStr = ''
if useMinos: minosOptStr = " --minos "+minosOpt
OptStr = ''
if blind:
    OptStr = " --bypassFrequentistFit -t "+str(nToys)

if (useGenerateOnly):
    print "Generating background-only Asimov toy"
    print 'Command = combine -M GenerateOnly -d '+filename+' -t '+str(nToys)+' --saveToys --expectSignal 0 -n '+name
    os.system('combine -M GenerateOnly -d '+filename+' -t '+str(nToys)+' --saveToys --expectSignal 0 -n '+name)

    print 'Command = combine -M FitDiagnostics -d '+filename+' --robustFit=1 -t '+str(nToys)+' --toysFile higgsCombine'+name+'.GenerateOnly.mH120.123456.root --expectSignal 1 --rMin '+str(rInj-10)+' --rMax '+str(rInj+10)+' -n '+name
    os.system('combine -M FitDiagnostics -d '+filename+' --robustFit=1 -t '+str(nToys)+' --toysFile higgsCombine'+name+'.GenerateOnly.mH120.123456.root --expectSignal 1 --rMin '+str(rInj-10)+' --rMax '+str(rInj+10)+' -n '+name)
elif isCR: # set to False above! dummy 
    print "Running Fit Diagnostics for initial workspace"
    print 'Command = combine -M FitDiagnostics -d workspace.root --saveWorkspace --saveShapes --plots'
    os.system('combine -M FitDiagnostics -d workspace.root --saveWorkspace --saveShapes --plots')
    
    print "Creating initialFit snapshot file: initialFitWorkspace.root"
    w_f = TFile.Open('higgsCombineTest.FitDiagnostics.mH120.root')
    w = w_f.Get('w')
    fr_f = TFile.Open('fitDiagnostics.root')
    fr = fr_f.Get('fit_b')
    myargs = RooArgSet(fr.floatParsFinal())
    w.saveSnapshot('initialFit',myargs,True)
    fout = TFile('initialFitWorkspace.root',"recreate")
    fout.WriteTObject(w,'w')
    fout.Close()

    print 'Command = combine -M GenerateOnly -d '+filename+' --snapshotName initialFit --toysFrequentist --bypassFrequentistFit -t '+str(nToys)+' --saveToys --expectSignal '+str(rInj)+' -n '+name
    os.system('combine -M GenerateOnly -d '+filename+' --snapshotName initialFit --toysFrequentist --bypassFrequentistFit -t '+str(nToys)+' --saveToys --expectSignal '+str(rInj)+' -n '+name)

    ## Toy fits again should be ok, since it's just fitting the previously-made toys
    print "Fitting toys...."
    print 'Command = combine -M FitDiagnostics -d '+filename+' --snapshotName initialFit --robustFit=1 --skipBOnlyFit --toysFrequentist --bypassFrequentistFit -t '+str(nToys)+' --toysFile higgsCombine'+name+'.GenerateOnly.mH120.123456.root --rMin '+str(rInj-10)+' --rMax '+str(rInj+10)+' -n '+name
    os.system('combine -M FitDiagnostics -d '+filename+' --snapshotName initialFit --robustFit=1 --skipBOnlyFit --toysFrequentist --bypassFrequentistFit -t '+str(nToys)+' --toysFile higgsCombine'+name+'.GenerateOnly.mH120.123456.root --rMin '+str(rInj-10)+' --rMax '+str(rInj+10)+' -n '+name)

else:
    print "Running Fit Diagnostics for background-only Asimov toy and fits it"
    #print 'Command = combine -M FitDiagnostics -d workspace.root --bypassFrequentistFit -t '+str(nToys)+' --expectSignal '+str(rInj)+' -n '+name+minosOptStr
    #os.system('combine -M FitDiagnostics -d workspace.root --bypassFrequentistFit -t '+str(nToys)+' --expectSignal '+str(rInj)+' -n '+name+minosOptStr)
    #print 'Command = combine -M FitDiagnostics -d workspace.root --bypassFrequentistFit -t '+str(nToys)+' -n '+name+minosOptStr
    #os.system('combine -M FitDiagnostics -d workspace.root --bypassFrequentistFit -t '+str(nToys)+' -n '+name+minosOptStr)
    print 'Command = combine -M FitDiagnostics -d workspace.root'+OptStr+' -n '+name+minosOptStr
    os.system('combine -M FitDiagnostics -d workspace.root'+OptStr+' -n '+name+minosOptStr)


print "Done!"
