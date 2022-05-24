import os, sys

fileDir = os.path.dirname(os.getcwd()+'/')+"/"
templateDir = ['limits_templates_2016Final','limits_templates_FRv5_PRv2_prefire_FIXelIdSyspdf4LHCpdfSFs','limits_templates_2018_FRv2_PRv2_elIdSys_TrigEffWeight_pdf4LHC_FIXpdfSFs']
outDir = 'limits_templates_combine161718'
if len(sys.argv)>1 : 
    outDir = sys.argv[-1]
    templateDir = []
    for d in sys.argv[1:-1]: 
        if d == sys.argv[-1] : continue
        templateDir.append(d)

fileDir_years = [fileDir+d for d in templateDir] 

whichSignal = 'TT'
for d in fileDir_years:
   if '_BB_' in d : whichSignal = 'BB'

massList = range(900,1800+100,100)
#if whichSignal=='BB': massList = range(900,1800+100,100)

BRs={}
nBRconf=0
if whichSignal=='TT':
    BRs['BW']=[0.50,0.0,0.0,1.0,0.0] #0.0,0.0,0.0,0.0,0.0,0.2,0.2,0.2,0.2,0.2,0.4,0.4,0.4,0.4,0.6,0.6,0.6,0.8,0.8,1.0,0.0]
    BRs['TH']=[0.25,0.0,0.5,0.0,1.0] #0.2,0.4,0.6,0.8,1.0,0.0,0.2,0.4,0.6,0.8,0.0,0.2,0.4,0.6,0.0,0.2,0.4,0.0,0.2,0.0,0.5]
    BRs['TZ']=[0.25,1.0,0.5,0.0,0.0] #0.8,0.6,0.4,0.2,0.0,0.8,0.6,0.4,0.2,0.0,0.6,0.4,0.2,0.0,0.4,0.2,0.0,0.2,0.0,0.0,0.5]
    nBRconf=len(BRs['BW'])
if whichSignal=='BB':
    BRs['TW']=[0.50,1.0,0.0,0.0,0.0] #0.0,0.0,0.0,0.0,0.0,0.0,0.2,0.2,0.2,0.2,0.2,0.4,0.4,0.4,0.4,0.6,0.6,0.6,0.8,0.8,0.0]
    BRs['BH']=[0.25,0.0,0.5,0.0,1.0] #0.0,0.2,0.4,0.6,0.8,1.0,0.0,0.2,0.4,0.6,0.8,0.0,0.2,0.4,0.6,0.0,0.2,0.4,0.0,0.2,0.5]
    BRs['BZ']=[0.25,0.0,0.5,1.0,0.0] #1.0,0.8,0.6,0.4,0.2,0.0,0.8,0.6,0.4,0.2,0.0,0.6,0.4,0.2,0.0,0.4,0.2,0.0,0.2,0.0,0.5]
    nBRconf=len(BRs['TW'])

for BRind in range(nBRconf):
    BRconfStr = ''
    if whichSignal=='TT': BRconfStr='bW'+str(BRs['BW'][BRind]).replace('.','p')+'_tZ'+str(BRs['TZ'][BRind]).replace('.','p')+'_tH'+str(BRs['TH'][BRind]).replace('.','p')
    if whichSignal=='BB': BRconfStr='tW'+str(BRs['TW'][BRind]).replace('.','p')+'_bZ'+str(BRs['BZ'][BRind]).replace('.','p')+'_bH'+str(BRs['BH'][BRind]).replace('.','p')
    for mass in massList:
        subDir = "/"+BRconfStr+"/cmb/"+str(mass)
        if not os.path.exists(outDir+subDir+"/workspace.root"):
            if not os.path.exists(outDir+subDir): 
                print("mkdir -p "+outDir+subDir)
                os.system("mkdir -p "+outDir+subDir)
            if not len(fileDir_years) > 1:
                print("Error! There is no card or only 1 card to combine. Please check your arguments.")
                exit(1) 
            Command = "combineCards.py"
            for d in fileDir_years: Command += " "+d+subDir+"/combined.txt.cmb"
            Command += " > "+outDir+subDir+"/combined.txt"
            print(Command)
            os.system(Command)
        else:
            print("Error! workspace.root already exists. Please check.")
            exit(1)

    print("combineTool.py -M T2W -i "+outDir+"/"+BRconfStr+"/cmb/*/combined.txt -o workspace.root --parallel 4 --channel-masks")
    os.system("combineTool.py -M T2W -i "+outDir+"/"+BRconfStr+"/cmb/*/combined.txt -o workspace.root --parallel 4 --channel-masks")
