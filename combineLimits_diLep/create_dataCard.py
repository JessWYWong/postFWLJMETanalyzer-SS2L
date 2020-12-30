#!/usr/bin/env python

import os,sys
import importlib
import glob

tag = "v1"
templateFile = "dataCard_template.py"
if len(sys.argv)>1: tag = sys.argv[1]
if len(sys.argv)>2: templateFile = sys.argv[2]

Tmass = range(1000,1800+100,100)
Bmass = range(900,1800+100,100)

outDir = "."
if not os.path.exists(os.getcwd()+'/'+outDir): os.system('mkdir -v '+outDir)

######## import corresponding uncertainties files ########
era = ['2017','2018']
systName_Alias = { 
	'jec':'jec',
	'jer':'jer',
	'pdfNew':'pdf', 
	'muRFcorrdNew':'scale',
	'lepIdSys':'idSF',
	'TrigSys':'trigSF',
	'prefire':'prefire',
	'pileup':'pu'
}

allSyst = {}
for year in era:
	ssdl = os.path.dirname(os.getcwd()+'/ssdluncerts'+year+'/')
	sys.path.append(ssdl)
	if not year in allSyst.keys(): allSyst[year] = {}

	MCBkgFiles = glob.glob(ssdl+"/*_MCBkg.py")
	for f in MCBkgFiles:
		wanted = [key for key in f.split("/")[-1].split("_") if key in systName_Alias.keys()]
		if not len(wanted)>0: continue
		MCunc = importlib.import_module(f.split("/")[-1][:-3])
		tempDict = MCunc.uncDict
		for i in wanted:
			for shift in ['Up','Dn']:
				if not i+shift in allSyst[year].keys(): allSyst[year][i+shift] = {}
				for sample,val in tempDict[systName_Alias[i]+shift].items():
					allSyst[year][i+shift].update({sample:val})
		MCunc = 0
	sigFiles = glob.glob(ssdl+"/*_sig.py")
	for f in sigFiles:
		wanted = [key for key in f.split("/")[-1].split("_") if key in systName_Alias.keys()]
		if not len(wanted)>0: continue
		SIGunc = importlib.import_module(f.split("/")[-1][:-3])
		tempDict = SIGunc.uncDict
		for i in wanted:
			for shift in ['Up','Dn']:
				if not i+shift in allSyst[year].keys(): allSyst[year][i+shift] = {}
				for sample,val in tempDict[systName_Alias[i]+shift].items():
					allSyst[year][i+shift].update({sample:val})
	sys.path.remove(ssdl)

#########################################################

dataCardTemp = os.getcwd()+"/"+templateFile
f = open(dataCardTemp, 'rU')
dataCardLines = f.readlines()
f.close()

with open(outDir+"/dataCard_"+tag+".py","w") as fout:
	for line in dataCardLines:
		if 'cb.cp().process(' in line:
			find_syst = [k for k in systName_Alias.keys() if "'"+k in line ]
			if len(find_syst) == 1 : syst = find_syst[0]
			elif len(find_syst) == 0 : 
				fout.write(line)
				continue
			else : print("ERROR! more than one systematic name found in line.", line)
			sample = line[line.find(".process(")+len(".process("):line.find(")",line.find(".process("),len(line))]
			if sample.startswith("['") : sample = sample[2:-2]
			if sample == "TTM":
				for year in era:
					newLine = line[:line.find("SystMap('mass')")+len("SystMap('mass')")]
					if syst+'Dn' in allSyst[year].keys(): 
						if not '$ERA' in line: newLine = newLine.replace("cb.cp().process(", "if era == '"+year+"' : cb.cp().process(")
						else: newLine = newLine.replace('$ERA',str(year))
						for m in Tmass:
							signal = sample+str(m)
							newLine+="(['"+str(m)+"'],("+str(allSyst[year][syst+'Dn'][signal])+","+str(allSyst[year][syst+'Up'][signal])+"))"
						newLine+=")\n"
						fout.write(newLine)
				#for m in Tmass:
				#	signal = sample+str(m)
				#	newLine = line[:line.find("SystMap('era')")+len("SystMap('era')")]
				#	newLine = newLine.replace(sample,"['"+signal+"']")
				#	for year in era: 
				#		if syst+'Dn' in allSyst[year].keys(): newLine+="(['"+year+"'],("+str(allSyst[year][syst+'Dn'][signal])+","+str(allSyst[year][syst+'Up'][signal])+"))"	
				#	newLine+=")\n"
				#	fout.write(newLine)
			elif sample == "BBM":
				for year in era:
					newLine = line[:line.find("SystMap('mass')")+len("SystMap('mass')")]
					if syst+'Dn' in allSyst[year].keys():
						if not '$ERA' in line: newLine = newLine.replace("cb.cp().process(", "if era == '"+year+"' : cb.cp().process(")
						else: newLine = newLine.replace('$ERA',str(year))
						for m in Bmass:
							signal = sample+str(m)
							newLine+="(['"+str(m)+"'],("+str(allSyst[year][syst+'Dn'][signal])+","+str(allSyst[year][syst+'Up'][signal])+"))"
						newLine+=")\n"
						fout.write(newLine)
				#for m in Bmass:
				#	signal = sample+str(m)
				#	newLine = line[:line.find("SystMap('era')")+len("SystMap('era')")]
				#	newLine = newLine.replace(sample,"['"+signal+"']")
				#	for year in era: 
				#		if syst+'Dn' in allSyst[year].keys(): newLine+="(['"+year+"'],("+str(allSyst[year][syst+'Dn'][signal])+","+str(allSyst[year][syst+'Up'][signal])+"))"
				#	newLine+=")\n"
				#	fout.write(newLine)
			else:
				newLine = line[:line.find("SystMap('era')")+len("SystMap('era')")]
				for year in era: 
					if syst+'Dn' in allSyst[year].keys(): newLine+="(['"+year+"'],("+str(allSyst[year][syst+'Dn'][sample])+","+str(allSyst[year][syst+'Up'][sample])+"))"
				newLine+=")\n"
				fout.write(newLine)
		else: fout.write(line)
			

