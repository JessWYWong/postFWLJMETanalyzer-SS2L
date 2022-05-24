import os,sys

## Arguments: SR limit directory name; CR limit directory name; mass point; number of toys

## Make a datacard first with datacard.py!

limitdir = sys.argv[1] ### !!! directory for SR !!!! --> for toy generation
limitdirCR = sys.argv[2] ### !!! non-SR !!! --> for nuisance pulls when blinded
mass = sys.argv[3]
nToys = int(sys.argv[4])

morphing = False

BR = 'bW0p5_tZ0p25_tH0p25'
if 'BB' in mass: BR = 'tW0p5_bZ0p25_bH0p25'
mass = mass.replace('BB','')

name = limitdir.replace('limits_templates_','')
cwd = os.getcwd()
path = limitdir+'/'+BR+'/cmb/'+mass
pathCR = limitdirCR+'/'+BR+'/cmb/'+mass

filename = "workspace.root"
name = "GoF"
if (morphing): name += "_SRCR"
rInj = 0


if (morphing):
    print 'copy workspace from combining CR and SR datacards and name it morphedWorkspace.root'
    if not os.path.exists(path+'/workspace.root'):
        print('Please run comineCards.py first and argument 1 should be the directory name for combined CR+SR.')
        exit(1)
    if not os.path.exists(pathCR+'/morphedWorkspace.root'):
        os.system('cp '+path+'/workspace.root '+pathCR+'/morphedWorkspace.root')

    os.chdir(pathCR)
    print 'Running fitDiagonostics in CR for nuisance pulls with data'
    print 'Command = combine -M FitDiagnostics -d '+filename+' -n CR'
    os.system('combine -M FitDiagnostics -d '+filename+' -n CR')

    #os.chdir(cwd)
    print 'update morphedWorkspace with post-fit from CR fitDiagnostics'
    #os.system('cp '+path+'/'+filename+' '+path+'/morphedWorkspace.root')
    os.system('python importPars.py '+pathCR+'/morphedWorkspace.root '+pathCR+'/fitDiagnosticsCR.root')

    #os.chdir(path)
    #print 'Generating toys in SR, using fitDiagnostics from CR'
    #print 'Command = combine -M GenerateOnly -d morphedWorkspace.root --toysFrequentist --bypassFrequentistFit -t '+str(nToys)+' --saveToys -n '+name
    #os.system('combine -M GenerateOnly -d workspace.root --toysFrequentist --bypassFrequentistFit -t '+str(nToys)+' --saveToys -n '+name)
    print 'Running GOF with background-only toys, using SR+CR morphedWorkspace'
    print 'Command = combine -M GoodnessOfFit -d morphedWorkspace.root --algo=saturated --toysFrequentist --bypassFrequentistFit -t '+str(nToys)+' --fixedSignalStrength=0  -n '+name+'SRtoys'
    os.system('combine -M GoodnessOfFit -d morphedWorkspace.root --algo=saturated --toysFrequentist --bypassFrequentistFit -t '+str(nToys)+' --fixedSignalStrength=0  -n '+name+'SRtoys')

    #os.chdir(cwd)
    #print 'Copy toy files to CR directory for the remaining procedures'
    #os.system('cp '+path+'/higgsCombine'+name+'SRtoys.GoodnessOfFit.mH120.123456.root '+pathCR+'/')

    #os.chdir(pathCR)
    print 'Running GOF with data in CR'
    print 'Command = combine -M GoodnessOfFit workspace.root --algo=saturated -n '+name+'data'
    os.system('combine -M GoodnessOfFit workspace.root --algo=saturated -n '+name+'data')

else:
    os.chdir(pathCR)
    print 'Running background-only GOF with data'
    print 'Command = combine -M GoodnessOfFit workspace.root --algo=saturated --fixedSignalStrength=0'
    os.system('combine -M GoodnessOfFit workspace.root --algo=saturated')# --fixedSignalStrength=0')

    print 'Running background-only GOF with toys, after frequentist fit'
    print 'Command = combine -M GoodnessOfFit workspace.root --algo=saturated -t '+str(nToys)+' --toysFrequentist --fixedSignalStrength=0'
    os.system('combine -M GoodnessOfFit -d workspace.root --algo=saturated -t '+str(nToys)+' --toysFrequentist')# --fixedSignalStrength=0')

print 'Done!'
