import os,sys
from ROOT import *

inDir = "/store/group/lpcljm/FWLJMET102X_2lep2017_wywong_082020_hadds/"

MClist = [
#### 2017
        "BpBp900.root",
        "BpBp1000.root",
        "BpBp1100.root",
        "BpBp1200.root",
        "BpBp1300.root",
        "BpBp1400.root",
        "BpBp1500.root",
        "BpBp1600.root",
        "BpBp1700.root",
        "BpBp1800.root",
        "TTH.root",
        "TTTT.root",
        "TTW.root",
        "TTZ.root",
        "TpTp700.root",
        "TpTp1000.root",
        "TpTp1100.root",
        "TpTp1200.root",
        "TpTp1300.root",
        "TpTp1400.root",
        "TpTp1500.root",
        "TpTp1600.root",
        "TpTp1700.root",
        "TpTp1800.root",
        "WJets.root",
        "WWW.root",
        "WWZ.root",
        "WZ.root",
        "WZZ.root",
        "WpWp.root",
        "ZZ.root",
        "ZZZ.root",

]

DATACENTRAL = TFile.Open("PileupHistogram-goldenJSON-13tev-2017-69200ub.root")
dataCentral = DATACENTRAL.Get("pileup")
DATADOWN = TFile.Open("PileupHistogram-goldenJSON-13tev-2017-66000ub.root")
dataDown = DATADOWN.Get("pileup")
DATAUP = TFile.Open("PileupHistogram-goldenJSON-13tev-2017-72400ub.root")
dataUp = DATAUP.Get("pileup")
dataCentral.SetDirectory(0)
dataDown.SetDirectory(0)
dataUp.SetDirectory(0)
dataCentral.Scale(1.0/dataCentral.Integral());
dataDown.Scale(1.0/dataDown.Integral());
dataUp.Scale(1.0/dataUp.Integral());

ratioCentral = dataCentral.Clone("ratioCentral");
ratioDown = dataDown.Clone("ratioDown");
ratioUp = dataUp.Clone("ratioUp");

sys.stdout = open('PUweights.h_diLep', 'w')
print('#ifndef PUWEIGHTS_H')
print('#define PUWEIGHTS_H')
print('#include "vector"')
print('#include <TString.h>')
print('#include <map>')
print('#include <string.h>')
print('using namespace std;')
print('')

print('class PUweights{')
print('public:')
print('    double GetPUcentral(string FileName, int index){return pileup_central.at(FileName).at(index);}')
print('    double GetPUup(string FileName, int index){return pileup_up.at(FileName).at(index);}')
print('    double GetPUdown(string FileName, int index){return pileup_down.at(FileName).at(index);}')

print('')
print('private:')

print('map<string, vector<double> > pileup_central={')
for MC in MClist:
    SampleName = MC[:-5]
    MCfile = TFile.Open('root://cmseos.fnal.gov/'+inDir+MC)
    MChist = MCfile.Get("mcweightanalyzer/NumTrueHist") ## for FWLJMET
    MChist.SetDirectory(0);
    MChist.Scale(1.0/MChist.Integral())

    ratioCentral.Divide(dataCentral,MChist,1,1)

    print('{ "'+SampleName+'", {'),
    for ibin in range(1,ratioCentral.GetNbinsX()+2):
        print('%.3e, ' % ratioCentral.GetBinContent(ibin)),

    print('} },')
    MCfile.Close()
print('};')

print('map<string, vector<double> > pileup_down={')
for MC in MClist:
    SampleName = MC[:-5]
    MCfile = TFile.Open('root://cmseos.fnal.gov/'+inDir+MC)
    MChist = MCfile.Get("mcweightanalyzer/NumTrueHist")
    MChist.SetDirectory(0);
    MChist.Scale(1.0/MChist.Integral())

    ratioDown.Divide(dataDown,MChist,1,1)

    print('{ "'+SampleName+'", {'),
    for ibin in range(1,ratioDown.GetNbinsX()+2):
        print('%.3e, ' % ratioDown.GetBinContent(ibin)),

    print('} },')
    MCfile.Close()
print('};')

print('map<string, vector<double> > pileup_up={')
for MC in MClist:
    SampleName = MC[:-5]
    MCfile = TFile.Open('root://cmseos.fnal.gov/'+inDir+MC)
    MChist = MCfile.Get("mcweightanalyzer/NumTrueHist")
    MChist.SetDirectory(0);
    MChist.Scale(1.0/MChist.Integral())

    ratioUp.Divide(dataUp,MChist,1,1)

    print('{ "'+SampleName+'", {'),
    for ibin in range(1,ratioUp.GetNbinsX()+2):
        print('%.3e, ' % ratioUp.GetBinContent(ibin)),

    print('} },')
    MCfile.Close()
print('};')

print('};')
print('#endif')
