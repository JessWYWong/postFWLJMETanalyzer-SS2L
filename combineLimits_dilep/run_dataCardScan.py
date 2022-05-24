import os,sys

if len(sys.argv)< 7:
    print("Warning insufficient arguments provided. Run with `python run_dataCardScan.py [sig] [script] [inDir] [cutDir] [tempDir] [outName]`")
    exit(1)

#sig='TT' # or BB
sig = sys.argv[1]
#runScript = "dataCard_byCat_signalScale.py"
runScript = sys.argv[2]
inDir = sys.argv[3]
cutDir = sys.argv[4]
tempDir = sys.argv[5]
outName = sys.argv[6]
is2016 = False
if "2016" in inDir and "2016" in outName: is2016 = True

BRs = {}
#BRs['bW'] = [0.0,0.50,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.2,0.2,0.2,0.2,0.2,0.4,0.4,0.4,0.4,0.6,0.6,0.6,0.8,0.8]
#BRs['tH'] = [0.5,0.25,0.0,0.0,1.0,0.2,0.4,0.6,0.8,0.0,0.2,0.4,0.6,0.8,0.0,0.2,0.4,0.6,0.0,0.2,0.4,0.0,0.2]
#BRs['tZ'] = [0.5,0.25,0.0,1.0,0.0,0.8,0.6,0.4,0.2,0.8,0.6,0.4,0.2,0.0,0.6,0.4,0.2,0.0,0.4,0.2,0.0,0.2,0.0]
BRs['bW'] = [0.0, 0.0, 0.0, 0.0, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.2, 0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.4, 0.4, 0.4, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.6, 0.6, 0.7, 0.7, 0.7, 0.7, 0.8, 0.9, 0.9]#
BRs['tH'] = [0.1, 0.3, 0.7, 0.9, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.1, 0.3, 0.5, 0.7, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.1, 0.3, 0.5, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.1, 0.3, 0.0, 0.1, 0.2, 0.3, 0.1, 0.0, 0.1]#
BRs['tZ'] = [0.9, 0.7, 0.3, 0.1, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.0, 0.7, 0.5, 0.3, 0.1, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.0, 0.5, 0.3, 0.1, 0.5, 0.4, 0.3, 0.2, 0.1, 0.0, 0.3, 0.1, 0.3, 0.2, 0.1, 0.0, 0.1, 0.1, 0.0]#

BRs['tW'] = BRs['bW']
BRs['bZ'] = BRs['tZ']
BRs['bH'] = BRs['tH']

for BRind in range(len(BRs['bW'])):
    TT_BRconfStr = 'bW'+str(BRs['bW'][BRind]).replace('.','p')+'_tZ'+str(BRs['tZ'][BRind]).replace('.','p')+'_tH'+str(BRs['tH'][BRind]).replace('.','p')
    BB_BRconfStr = 'tW'+str(BRs['tW'][BRind]).replace('.','p')+'_bZ'+str(BRs['bZ'][BRind]).replace('.','p')+'_bH'+str(BRs['bH'][BRind]).replace('.','p')

    BRconfStr = TT_BRconfStr
    if sig == 'BB': BRconfStr = BB_BRconfStr

    cmd = "python -u "+runScript+" "+inDir+" "+cutDir+" "+BRconfStr+" "+tempDir+" "+outName
    if is2016:
        cmd = cmd.replace(".py" ,"_"+TT_BRconfStr+"_"+BB_BRconfStr+".py")

    print(cmd)
    os.system(cmd)
