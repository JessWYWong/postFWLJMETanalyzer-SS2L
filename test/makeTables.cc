#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string> //added by rizki
#include "../plugins/Macros.cc"
#include "TFile.h"
#include "TTree.h"
#include "TGraphErrors.h"
#include "../interface/unc.h"

std::string tableHeader(std::vector<std::string> vC, CutClass* c, std::string caption);
std::stringstream& printTable(std::stringstream& tablestring,std::vector<CutClass*> vCC, std::vector<std::string> vCS, int nmu,bool sig);
std::stringstream& printFinalTable(std::stringstream& tablestring,std::vector<Sample*> vBkgBF, std::vector<Sample*> vSigBF,std::vector<Sample*> vBkgEH, std::vector<Sample*> vSigEH, Sample* dataSample, std::vector<std::string> vCutString,std::string era);
std::stringstream& printFinalTable_v2(std::stringstream& tablestring,std::vector<Sample*> vBkgBF, std::vector<Sample*> vSigBF,std::vector<Sample*> vBkgEH, std::vector<Sample*> vSigEH, Sample* dataSample, std::vector<std::string> vCutString,std::string era); //added by rizki
std::stringstream& printFinalTable_v3(std::stringstream& tablestring,std::vector<Sample*> vBkgBF, std::vector<Sample*> vSigBF, Sample* dataSample, std::vector<std::string> vCutString,std::string era); //added by rizki
std::stringstream& printFinalTable_v4(std::stringstream& tablestring,std::vector<Sample*> vBkgBF, std::vector<Sample*> vSigBF, Sample* dataSample, std::vector<std::string> vCutString,std::string era); //added by rizki
std::stringstream& printEffTable(std::stringstream& tablestring,std::vector<CutClass*> vCC, std::vector<std::string> vCS, int nmu,bool sig);
std::stringstream& printEffTable_v2(std::stringstream& tablestring,std::vector<CutClass*> vCC, std::vector<std::string> vCS, int nmu,bool sig, int whichBR); //added by rizki
std::stringstream& printChargeMisIDTable_lpt(std::stringstream& chargeMisIDTable);
std::stringstream& printChargeMisIDTable_hpt(std::stringstream& chargeMisIDTable);
std::stringstream& printChargeMisIDTable_hhpt(std::stringstream& chargeMisIDTable);

/*
float uncPU_TTZ=0.06;
float uncPU_TTW=0.01;
float uncPU_TTH=0.01;
float uncPU_TTTT=0.03;
float uncPU_WZ=0.1;
float uncPU_ZZ=0.07;
float uncPU_WpWp=0.01;
float uncPU_WWZ=0.06;
float uncPU_WZZ=0.02;
float uncPU_ZZZ=0.;

float uncPDF_TTZ=0.03;
float uncPDF_TTW=0.13;
float uncPDF_TTH=0.03;
float uncPDF_TTTT=0.35;
float uncPDF_WZ=0.11;
float uncPDF_ZZ=0.02;
float uncPDF_WpWp=0.354;
float uncPDF_WWZ=0.02;
float uncPDF_WZZ=0.02;
float uncPDF_ZZZ=0.;

float uncSCALE_TTZ=0.11;
float uncSCALE_TTW=0.16;
float uncSCALE_TTH=0.27;
float uncSCALE_TTTT=0.26;
float uncSCALE_WZ=0.15;
float uncSCALE_ZZ=0.11;
float uncSCALE_WpWp=0.354;
float uncSCALE_WWZ=0.20;
float uncSCALE_WZZ=0.21;
float uncSCALE_ZZZ=0.;

float uncMC_TTZ=pow(uncSCALE_TTZ*uncSCALE_TTZ + uncPDF_TTZ*uncPDF_TTZ,0.5);
float uncMC_TTW=pow(uncSCALE_TTW*uncSCALE_TTW + uncPDF_TTW*uncPDF_TTW,0.5);
float uncMC_TTH=pow(uncSCALE_TTH*uncSCALE_TTH + uncPDF_TTH*uncPDF_TTH,0.5);
float uncMC_TTTT=pow(uncSCALE_TTTT*uncSCALE_TTTT + uncPDF_TTTT*uncPDF_TTTT,0.5);
float uncMC_WZ=pow(uncSCALE_WZ*uncSCALE_WZ + uncPDF_WZ*uncPDF_WZ,0.5);
float uncMC_ZZ=pow(uncSCALE_ZZ*uncSCALE_ZZ + uncPDF_ZZ*uncPDF_ZZ,0.5);
float uncMC_WpWp=pow(uncSCALE_WpWp*uncSCALE_WpWp + uncPDF_WpWp*uncPDF_WpWp,0.5);
float uncMC_WWZ=pow(uncSCALE_WWZ*uncSCALE_WWZ + uncPDF_WWZ*uncPDF_WWZ,0.5);
float uncMC_WZZ=pow(uncSCALE_WZZ*uncSCALE_WZZ + uncPDF_WZZ*uncPDF_WZZ,0.5);
float uncMC_ZZZ=pow(uncSCALE_ZZZ*uncSCALE_ZZZ + uncPDF_ZZZ*uncPDF_ZZZ,0.5);

float uncJES_TTZ=0.04;
float uncJES_TTW=0.03;
float uncJES_TTH=0.04;
float uncJES_TTTT=0.02;
float uncJES_WZ=0.09; 
float uncJES_ZZ=0.09; 
float uncJES_WpWp=0.09;
float uncJES_WWZ=0.19;
float uncJES_WZZ=0.09;
float uncJES_ZZZ=0;

float uncJER_TTZ=0.01;
float uncJER_TTW=0.01;
float uncJER_TTH=0.01;
float uncJER_TTTT=0.01;
float uncJER_WZ=0.01; 
float uncJER_ZZ=0.01;
float uncJER_WpWp=0.01;
float uncJER_WWZ=0.1;
float uncJER_WZZ=0.01; 
float uncJER_ZZZ=0;
*/

void makeTables(){

  //make output file
  std::ofstream outfile;
//   outfile.open("table_miniIsoTight_NonIsoTrig_Jan30-2019.txt"); //Check Macros.cc for input folder!! and adjust isoTrig below
//   outfile.open("table_miniIsoTight_IsoTrig_Jan30-2019.txt"); //Check Macros.cc for input folder!! and adjust isoTrig below
//   outfile.open("table_miniIsoTight_IsoTrig_Lep4035_Jan31-2019.txt"); //Check Macros.cc for input folder!! and adjust isoTrig below
  // outfile.open("table_miniIsoTight_NonIsoTrigHT_Mar3-2019.txt"); //Check Macros.cc for input folder!! and adjust nonisoTrigHT below
//   outfile.open("table_miniIsoTight_NonIsoTrigHT_37invfb_Mar27-2019.txt"); //Check Macros.cc for input folder!! and adjust nonisoTrigHT below
//   outfile.open("table_miniIsoTight_NonIsoTrigHT_41invfb_Mar27-2019.txt"); //Check Macros.cc for input folder!! and adjust nonisoTrigHT below
  //outfile.open("table_miniIsoTight_IsoTrigHT_ElPRUnity_Jan2-2019.txt"); //Check Macros.cc for input folder!! and adjust nonisoTrigHT below
  outfile.open("table_miniIsoTight_IsoTrig_Nov-2020.txt");  

  //set which cuts for which trigger
  bool isoTrig = true; // turned on by Jess
  std::cout << "isoTrig = " << isoTrig << std::endl;
  bool nonIso_HTtrig =false; // added by rizki, changed to false by Jess
  std::cout << "nonIso_HTtrig = " << nonIso_HTtrig << std::endl;

  //set precision

  //set desired luminosity
  float lumi1 = 41.56; //fb^-1 yr_eras = 2017B-F
//  float lumi1 = 37.6; //fb^-1 yr_eras = 2017C-F

  //get list of signal samples starting with ssdl cut
//   std::vector<Sample*> vSig2017BF = getInclusiveSigSampleVecForTable("sZVeto",lumi1,"MVA2017TightRC","CBTightMiniIsoTight","2017B-F"); //commented out by rizki
//   std::vector<Sample*> vSig2016EH = getInclusiveSigSampleVecForTable("sZVeto",lumi2,"MVA2017TightRC","CBTightMiniIsoTight","2016E-H"); //commented out by rizki

  //setting year and era string
  std::string yr_eras= "2017B-F";
  //std::string yr_eras= "2017B-F_UnityElPR";
  //std::string yr_eras = "2017C-F"; //2017B is dropped because no available triggers (as reported by AN2018_280).

  //TT SIGNAL <decay> - RIZKI 
  std::string elID = "MVA2017TightV2IsoTightRC";
  std::string muID = "CBTightMiniIsoTight";
  int whichBR = 0;
  std::vector<Sample*> vSigSamples1a = getInclusiveSigTTSampleVecForTable("sZVeto", lumi1, elID, muID, yr_eras,"BWBW",whichBR);
  std::vector<Sample*> vSigSamples2a = getInclusiveSigTTSampleVecForTable("sZVeto", lumi1, elID, muID, yr_eras,"THBW",whichBR);
  std::vector<Sample*> vSigSamples3a = getInclusiveSigTTSampleVecForTable("sZVeto", lumi1, elID, muID, yr_eras,"THTH",whichBR);
  std::vector<Sample*> vSigSamples4a = getInclusiveSigTTSampleVecForTable("sZVeto", lumi1, elID, muID, yr_eras,"TZBW",whichBR);
  std::vector<Sample*> vSigSamples5a = getInclusiveSigTTSampleVecForTable("sZVeto", lumi1, elID, muID, yr_eras,"TZTH",whichBR);
  std::vector<Sample*> vSigSamples6a = getInclusiveSigTTSampleVecForTable("sZVeto", lumi1, elID, muID, yr_eras,"TZTZ",whichBR);
  std::vector<Sample*> vSig2017BF = appendSampleVectors(vSigSamples1a,vSigSamples2a,vSigSamples3a,vSigSamples4a,vSigSamples5a,vSigSamples6a);
//   std::vector<Sample*> vSigSamples1b = getInclusiveSigTTSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H","BWBW",whichBR);
//   std::vector<Sample*> vSigSamples2b = getInclusiveSigTTSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H","THBW",whichBR);
//   std::vector<Sample*> vSigSamples3b = getInclusiveSigTTSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H","THTH",whichBR);
//   std::vector<Sample*> vSigSamples4b = getInclusiveSigTTSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H","TZBW",whichBR);
//   std::vector<Sample*> vSigSamples5b = getInclusiveSigTTSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H","TZTH",whichBR);
//   std::vector<Sample*> vSigSamples6b = getInclusiveSigTTSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H","TZTZ",whichBR);
//   std::vector<Sample*> vSig2016EH = appendSampleVectors(vSigSamples1b,vSigSamples2b,vSigSamples3b,vSigSamples4b,vSigSamples5b,vSigSamples6b);

  //BB SIGNAL <nominal BR> - RIZKI 
//   std::string elID = "MVA2017TightRC";
//   std::string muID = "CBTightMiniIsoTight";
//   int whichBR = 0;
//   std::vector<Sample*> vSigSamples1a = getInclusiveSigBBSampleVecForTable("sZVeto", lumi1, elID, muID, yr_eras,"TWTW",whichBR);
//   std::vector<Sample*> vSigSamples2a = getInclusiveSigBBSampleVecForTable("sZVeto", lumi1, elID, muID, yr_eras,"BHTW",whichBR);
//   std::vector<Sample*> vSigSamples3a = getInclusiveSigBBSampleVecForTable("sZVeto", lumi1, elID, muID, yr_eras,"BHBH",whichBR);
//   std::vector<Sample*> vSigSamples4a = getInclusiveSigBBSampleVecForTable("sZVeto", lumi1, elID, muID, yr_eras,"BZTW",whichBR);
//   std::vector<Sample*> vSigSamples5a = getInclusiveSigBBSampleVecForTable("sZVeto", lumi1, elID, muID, yr_eras,"BZBH",whichBR);
//   std::vector<Sample*> vSigSamples6a = getInclusiveSigBBSampleVecForTable("sZVeto", lumi1, elID, muID, yr_eras,"BZBZ",whichBR);
//   std::vector<Sample*> vSig2017BF = appendSampleVectors(vSigSamples1a,vSigSamples2a,vSigSamples3a,vSigSamples4a,vSigSamples5a,vSigSamples6a);
//   std::vector<Sample*> vSigSamples1b = getInclusiveSigBBSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H","TWTW",whichBR);
//   std::vector<Sample*> vSigSamples2b = getInclusiveSigBBSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H","BHTW",whichBR);
//   std::vector<Sample*> vSigSamples3b = getInclusiveSigBBSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H","BHBH",whichBR);
//   std::vector<Sample*> vSigSamples4b = getInclusiveSigBBSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H","BZTW",whichBR);
//   std::vector<Sample*> vSigSamples5b = getInclusiveSigBBSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H","BZBH",whichBR);
//   std::vector<Sample*> vSigSamples6b = getInclusiveSigBBSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H","BZBZ",whichBR);
//   std::vector<Sample*> vSig2016EH = appendSampleVectors(vSigSamples1b,vSigSamples2b,vSigSamples3b,vSigSamples4b,vSigSamples5b,vSigSamples6b);

  //BB SIGNAL <100% tW BR> - RIZKI 
//   std::string elID = "MVA2017TightRC";
//   std::string muID = "CBTightMiniIsoTight";
//   std::vector<Sample*> vSigSamples1a = getInclusiveSigBBSampleVecForTable("sZVeto", lumi1, elID, muID, yr_eras,"TWTW",1);
//   std::vector<Sample*> vSigSamples2a = getInclusiveSigBBSampleVecForTable("sZVeto", lumi1, elID, muID, yr_eras,"BHTW",1);
//   std::vector<Sample*> vSigSamples3a = getInclusiveSigBBSampleVecForTable("sZVeto", lumi1, elID, muID, yr_eras,"BHBH",1);
//   std::vector<Sample*> vSigSamples4a = getInclusiveSigBBSampleVecForTable("sZVeto", lumi1, elID, muID, yr_eras,"BZTW",1);
//   std::vector<Sample*> vSigSamples5a = getInclusiveSigBBSampleVecForTable("sZVeto", lumi1, elID, muID, yr_eras,"BZBH",1);
//   std::vector<Sample*> vSigSamples6a = getInclusiveSigBBSampleVecForTable("sZVeto", lumi1, elID, muID, yr_eras,"BZBZ",1);
//   std::vector<Sample*> vSig2017BF = appendSampleVectors(vSigSamples1a,vSigSamples2a,vSigSamples3a,vSigSamples4a,vSigSamples5a,vSigSamples6a);
//   std::vector<Sample*> vSigSamples1b = getInclusiveSigBBSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H","TWTW",1);
//   std::vector<Sample*> vSigSamples2b = getInclusiveSigBBSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H","BHTW",1);
//   std::vector<Sample*> vSigSamples3b = getInclusiveSigBBSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H","BHBH",1);
//   std::vector<Sample*> vSigSamples4b = getInclusiveSigBBSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H","BZTW",1);
//   std::vector<Sample*> vSigSamples5b = getInclusiveSigBBSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H","BZBH",1);
//   std::vector<Sample*> vSigSamples6b = getInclusiveSigBBSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H","BZBZ",1);
//   std::vector<Sample*> vSig2016EH = appendSampleVectors(vSigSamples1b,vSigSamples2b,vSigSamples3b,vSigSamples4b,vSigSamples5b,vSigSamples6b);


  //get vector of background samples
  std::vector<Sample*> vMCBkg2017BF = getMCBkgSampleVec("ss",lumi1,elID,muID, yr_eras);
//   std::vector<Sample*> vMCBkg2016EH = getMCBkgSampleVec("sZVeto",lumi2,"MVA2017TightRC","CBTightMiniIsoTight","2016E-H");
  std::vector<Sample*> vDDBkg2017BF =  getDDBkgSampleVec("ss",lumi1,elID,muID, yr_eras);
//   std::vector<Sample*> vDDBkg2016EH =  getDDBkgSampleVec("sZVeto",lumi1,"MVA2017TightRC","CBTightMiniIsoTight","2016E-H");

  std::vector<Sample*> vBkg2017BF = appendSampleVectors(vMCBkg2017BF,vDDBkg2017BF);
//   std::vector<Sample*> vBkg2016EH = appendSampleVectors(vMCBkg2016EH,vDDBkg2016EH);
  //get vector of data
  Sample* dataSample2017BF = getDataSample("sZVeto",elID,muID, yr_eras);
//   Sample* dataSample2016EH = getDataSample("sZVeto","MVA2017TightRC","CBTightMiniIsoTight","2016E-H");
  Sample* dataSampleFull = getDataSample("sZVeto",elID,muID, yr_eras);

  //now get vector of cuts
  std::vector<std::string> vCutString;
  if(isoTrig){
  	//vCutString = getCutString_isoTrig();
  	vCutString = getCutString_Lep4030();
  }
  else if(nonIso_HTtrig){
  	//vCutString = getCutString_nonIsoHTtrig();
  	vCutString = getCutString_Lep4030();
  }
  else{
  	vCutString = getCutString();
  }

  for(size_t i =0; i<vCutString.size();i++){
    std::cout<<vCutString.at(i)<<std::endl;
  }

  bool Bkg=false;
  bool Sig=true;
  std::stringstream tables;
  tables<<std::fixed<<std::setprecision(2);


  printFinalTable_v3(tables,vBkg2017BF,vSig2017BF,dataSampleFull,vCutString, yr_eras); //Clints original format but possibly uncertainties are even more outdated here.
  printFinalTable_v4(tables,vBkg2017BF,vSig2017BF,dataSampleFull,vCutString, yr_eras);

  //set precision
  tables<<std::fixed<<std::setprecision(2);
//   for(int nmu=-1; nmu<3; nmu++){
  for(int nmu=-1; nmu<0; nmu++){
    //now make a vector of cutClass for bkg
    std::vector<CutClass*> vCutMCBkg2017BF = getCutClassVector(vMCBkg2017BF,vCutString,nmu);
    std::vector<CutClass*> vCutDDBkg2017BF = getCutClassVector(vDDBkg2017BF,vCutString,nmu);
    std::vector<CutClass*> vCutBkg2017BF = appendCutClassVectors(vCutMCBkg2017BF,vCutDDBkg2017BF);

//     std::vector<CutClass*> vCutMCBkg2016EH = getCutClassVector(vMCBkg2016EH,vCutString,nmu);
//     std::vector<CutClass*> vCutDDBkg2016EH = getCutClassVector(vDDBkg2016EH,vCutString,nmu);
//     std::vector<CutClass*> vCutBkg2016EH = appendCutClassVectors(vCutMCBkg2016EH,vCutDDBkg2016EH);

    std::vector<CutClass*> vCutBkg = vCutBkg2017BF;
    //now make a vector of cutClass for sig
    std::vector<CutClass*> vCutSig2017BF = getCutClassVector(vSig2017BF,vCutString,nmu);
//     std::vector<CutClass*> vCutSig2016EH = getCutClassVector(vSig2016EH,vCutString,nmu);
    std::vector<CutClass*> vCutSig = vCutSig2017BF;
    //now print background table
    tables<<std::fixed<<std::setprecision(2);
    printTable(tables,vCutBkg,vCutString,nmu,Bkg);
    printTable(tables,vCutSig,vCutString,nmu,Sig);
    tables<<std::fixed<<std::setprecision(4);
    printEffTable(tables,vCutBkg,vCutString,nmu,Bkg);
    printEffTable(tables,vCutSig,vCutString,nmu,Sig);
	if(nmu!=-1) continue;
    printEffTable_v2(tables,vCutSig,vCutString,nmu,Sig,whichBR);
  }
  
//   //commented out by rizki
//   //make charge misID table
//   tables<<"\n";
//   printChargeMisIDTable_lpt(tables);
//   printChargeMisIDTable_hpt(tables);
//   printChargeMisIDTable_hhpt(tables);
  outfile<<tables.str();

  outfile.close();
  
  gApplication->Terminate();
}

std::string tableHeader(std::vector<std::string> vC, CutClass* cC, std::string caption){

  std::stringstream str;

//   str<<"\\begin{table}\n"<<"\\centering\n\\scriptsize\n"<<"\\topcaption{"<<caption<<"}\n"<<"\\begin{tabular}{|"; //commented by rizki
  str<<"\\begin{table}\n"<<"\\centering\n\\scriptsize\n"<<"\\caption{"<<caption<<"}\n"<<"\\begin{tabular}{|"; // added by rizki 
  for(size_t i =0; i< (vC.size()+1); i++){
    str<<"c|";
  }
  str<<"}\n\\hline\\hline\n";

  str<<"Sample";
  for(size_t i =0; i< (cC->cutname).size(); i++){
    str<<" & "<<(cC->cutname).at(i);
  }

  str<<"\\\\ \n";
  //std::cout<<str.str()<<std::endl;
  return str.str();
};

std::stringstream& printTable(std::stringstream& tablestring,std::vector<CutClass*> vCC, std::vector<std::string> vCS, int nmu,bool sig){



  //caption
  std::string caption;
  if(sig){
    if(nmu==-1) caption = "Expected Number of Signal Events vs. Analysis Cut for all channels combined";
    if(nmu==0) caption = "Expected Number of Signal Events vs. Analysis Cut for di-electron channel";
    if(nmu==1) caption = "Expected Number of Signal Events vs. Analysis Cut for electron-muon channel";
    if(nmu==2) caption = "Expected Number of Signal Events vs. Analysis Cut for di-muon channel";
  }
  else{
    if(nmu==-1) caption = "Expected Number of Background Events vs. Analysis Cut for all channels combined";
    if(nmu==0) caption = "Expected Number of Background Events vs. Analysis Cut for di-electron channel";
    if(nmu==1) caption = "Expected Number of Background Events vs. Analysis Cut for electron-muon channel";
    if(nmu==2) caption = "Expected Number of Background Events vs. Analysis Cut for di-muon channel";
  }


  //label
  std::string label;
  if(sig){
    if(nmu==-1) label = "\\label{tab:ExpSigEvtsAll}";
    if(nmu==0) label = "\\label{tab:ExpSigEvtsElEl}";
    if(nmu==1) label = "\\label{tab:ExpSigEvtsElMu}";
    if(nmu==2) label = "\\label{tab:ExpSigEvtsMuMu}";
  }
  else{
    if(nmu==-1) label = "\\label{tab:ExpBkgEvtsAll}";
    if(nmu==0) label = "\\label{tab:ExpBkgEvtsElEl}";
    if(nmu==1) label = "\\label{tab:ExpBkgEvtsElMu}";
    if(nmu==2) label = "\\label{tab:ExpBkgEvtsMuMu}";
  }

  tablestring<<tableHeader(vCS, vCC.at(0), caption); tablestring<<"\\hline \n";
  for(size_t i=0; i < vCC.size(); i++){
    std::cout<<"sample:		"<<vCC.at(i)->samplename<<" evts at min cut		:"<<vCC.at(i)->nEvents.at(0)<<std::endl;
    float sys=0;
    if(vCC.at(i)->samplename=="NonPrompt") sys= pow(unc_NP, 2);
    else if(vCC.at(i)->samplename=="ChargeMisID") sys= pow(unc_CMID, 2);
    
    //CONTINUE HERE - Oct19-2017

//     else if(vCC.at(i)->samplename=="TTW") sys= pow(0.19, 2) + pow(0.03,2) + pow(0.06,2) + pow(0.02,2); //mc,jes,jer,pu
//     else if(vCC.at(i)->samplename=="TTZ") sys= pow(0.12, 2) + pow(0.03,2)+ pow(0.06,2) + pow(0.02,2);
//     else if(vCC.at(i)->samplename=="TTH") sys= pow(0.30, 2) + pow(0.03,2)+ pow(0.06,2) + pow(0.02,2);
//     else if(vCC.at(i)->samplename=="TTTT") sys= pow(0.5, 2) + pow(0.02,2)+ pow(0.06,2) + pow(0.02,2);
//     else if(vCC.at(i)->samplename=="WZ") sys= pow(0.24, 2) + pow(0.09,2)+ pow(0.06,2) + pow(0.02,2);
//     else if(vCC.at(i)->samplename=="ZZ") sys= pow(0.10, 2) + pow(0.04,2)+ pow(0.06,2) + pow(0.02,2);
//     else if(vCC.at(i)->samplename=="WpWp") sys= pow(0.5, 2) + pow(0.09,2)+ pow(0.06,2) + pow(0.02,2);
//     else if(vCC.at(i)->samplename=="WWZ") sys= pow(0.5, 2) + pow(0.09,2)+ pow(0.06,2) + pow(0.02,2);
//     else if(vCC.at(i)->samplename=="WZZ") sys= pow(0.5, 2) + pow(0.09,2)+ pow(0.06,2) + pow(0.02,2);
//     else if(vCC.at(i)->samplename=="ZZZ") sys= pow(0.5, 2) + pow(0.09,2)+ pow(0.06,2) + pow(0.02,2);

    else if(vCC.at(i)->samplename=="TTW") sys= pow(uncPDF_TTW, 2) + pow(uncSCALE_TTW, 2) + pow(uncJES_TTW,2) + pow(uncJER_TTW,2) + pow(uncPU_TTW,2); //pdf,scale,jes,jer,pu
    else if(vCC.at(i)->samplename=="TTZ") sys= pow(uncPDF_TTZ, 2) + pow(uncSCALE_TTZ, 2) + pow(uncJES_TTZ,2) + pow(uncJER_TTZ,2) + pow(uncPU_TTZ,2);
    else if(vCC.at(i)->samplename=="TTH") sys= pow(uncPDF_TTH, 2) + pow(uncSCALE_TTH, 2) + pow(uncJES_TTH,2) + pow(uncJER_TTH,2) + pow(uncPU_TTH,2);
    else if(vCC.at(i)->samplename=="TTTT") sys= pow(uncPDF_TTTT, 2) + pow(uncSCALE_TTTT, 2) + pow(uncJES_TTTT,2) + pow(uncJER_TTTT,2) + pow(uncPU_TTTT,2);
    else if(vCC.at(i)->samplename=="WZ") sys= pow(uncPDF_WZ, 2) + pow(uncSCALE_WZ, 2) + pow(uncJES_WZ,2) + pow(uncJER_WZ,2) + pow(uncPU_WZ,2);
    else if(vCC.at(i)->samplename=="ZZ") sys= pow(uncPDF_ZZ, 2) + pow(uncSCALE_ZZ, 2) + pow(uncJES_ZZ,2) + pow(uncJER_ZZ,2) + pow(uncPU_ZZ,2);
    else if(vCC.at(i)->samplename=="WpWp") sys= pow(uncPDF_WpWp, 2) + pow(uncSCALE_WpWp, 2) + pow(uncJES_WpWp,2) + pow(uncJER_WpWp,2) + pow(uncPU_WpWp,2);
    else if(vCC.at(i)->samplename=="WWZ") sys= pow(uncPDF_WWZ, 2) + pow(uncSCALE_WWZ, 2) + pow(uncJES_WWZ,2) + pow(uncJER_WWZ,2) + pow(uncPU_WWZ,2);
    else if(vCC.at(i)->samplename=="WZZ") sys= pow(uncPDF_WZZ, 2) + pow(uncSCALE_WZZ, 2) + pow(uncJES_WZZ,2) + pow(uncJER_WZZ,2) + pow(uncPU_WZZ,2);
    else if(vCC.at(i)->samplename=="ZZZ") sys= pow(uncPDF_ZZZ, 2) + pow(uncSCALE_ZZZ, 2) + pow(uncJES_ZZZ,2) + pow(uncJER_ZZZ,2) + pow(uncPU_ZZZ,2);
//     else if(vCC.at(i)->samplename.find("X53X53")!=std::string::npos) sys = pow(0.03,2)+ pow(0.01,2) + pow(0.01,2); //jes, jer, pu
    else if(vCC.at(i)->samplename.find("prime")!=std::string::npos) sys =  pow(uncPDF_sig, 2) + pow(uncSCALE_sig, 2) + pow(uncJES_sig,2)+ pow(uncJER_sig,2) + pow(uncPU_sig,2) ; //jes, jer, pu
    else sys = pow(0.5,2);

    if(vCC.at(i)->samplename=="TTZ") sys= sys + pow(uncTrigSF_TTZ,2) + pow(uncIDSF_TTZ,2); // trig plus id
    else if(vCC.at(i)->samplename=="TTW") sys= sys + pow(uncTrigSF_TTW,2) + pow(uncIDSF_TTW,2);
    else if(vCC.at(i)->samplename=="TTH") sys= sys + pow(uncTrigSF_TTH,2) + pow(uncIDSF_TTH,2);
    else if(vCC.at(i)->samplename=="TTTT") sys= sys + pow(uncTrigSF_TTTT,2) + pow(uncIDSF_TTTT,2);
    else if(vCC.at(i)->samplename=="WZ") sys= sys + pow(uncTrigSF_WZ,2) + pow(uncIDSF_WZ,2);
    else if(vCC.at(i)->samplename=="ZZ") sys= sys + pow(uncTrigSF_ZZ,2) + pow(uncIDSF_ZZ,2);
    else if(vCC.at(i)->samplename=="WpWp") sys= sys + pow(uncTrigSF_WpWp,2) + pow(uncIDSF_WpWp,2);
    else if(vCC.at(i)->samplename=="WWZ") sys= sys + pow(uncTrigSF_WWZ,2) + pow(uncIDSF_WWZ,2);
    else if(vCC.at(i)->samplename=="WZZ") sys= sys + pow(uncTrigSF_WZZ,2) + pow(uncIDSF_WZZ,2);
    else if(vCC.at(i)->samplename=="ZZZ") sys= sys + pow(uncTrigSF_ZZZ,2) + pow(uncIDSF_ZZZ,2);
    else if(vCC.at(i)->samplename.find("prime")!=std::string::npos) sys = sys + pow(uncTrigSF_sig,2) + pow(uncIDSF_sig,2);

    if(! (vCC.at(i)->samplename=="NonPrompt" || vCC.at(i)->samplename=="ChargeMisID")){
      sys = sys + pow(uncIsoSF,2) + pow(uncLumi,2); //iso plus lumi 
    }
    tablestring<<vCC.at(i)->samplename;
    for(size_t j =0; j < (vCC.at(i)->nEvents).size(); j++){
      float errsys = sys*pow((vCC.at(i)->nEvents).at(j),2);
      float staterr = vCC.at(i)->vErr.at(j);
      float err = pow((vCC.at(i)->vErr).at(j),2) + errsys;
      if(vCC.at(i)->samplename=="NonPrompt" && nmu==0)std::cout<<vCC.at(i)->samplename<<" sys: "<<sys<<" systematic errors"<<pow(errsys,0.5)<<" stat errors: "<<staterr<<" total errors "<<err<<" nEvents: "<<vCC.at(i)->nEvents.at(j)<<std::endl;
      err = pow(err,0.5);
      tablestring<<" & "<<(vCC.at(i)->nEvents).at(j)<<" $\\pm$ "<<err;
    }
    tablestring<<" \\\\"<<std::endl;
  }
  tablestring<<"\\hline \n\\end{tabular} \n"<<label<<'\n'<<"\\end{table} \n\n";

  return tablestring;
 
}


std::stringstream& printEffTable(std::stringstream& tablestring,std::vector<CutClass*> vCC, std::vector<std::string> vCS, int nmu,bool sig){



  //caption
  std::string caption;
  if(sig){
    if(nmu==-1) caption = "Signal Efficiency Post Associated Z Veto vs. Analysis Cut for all channels combined";
    if(nmu==0) caption = "Signal Efficiency Post Associated Z Veto vs. Analysis Cut for di-electron channel";
    if(nmu==1) caption = "Signal Efficiency Post Associated Z Veto vs. Analysis Cut for electron-muon channel";
    if(nmu==2) caption = "Signal Efficiency Post Associated Z Veto vs. Analysis Cut for di-muon channel";
  }
  else{
    if(nmu==-1) caption = "Background Rejection Post Associated Z Veto vs. Analysis Cut for all channels combined";
    if(nmu==0) caption = "Background Rejection Post Associated Z Veto vs. Analysis Cut for di-electron channel";
    if(nmu==1) caption = "Background Rejection Post Associated Z Veto vs. Analysis Cut for electron-muon channel";
    if(nmu==2) caption = "Background Rejection Post Associated Z Veto vs. Analysis Cut for di-muon channel";
  }


  //label
  std::string label;
  if(sig){
    if(nmu==-1) label = "\\label{tab:ExpSigEvtsAll}";
    if(nmu==0) label = "\\label{tab:ExpSigEvtsElEl}";
    if(nmu==1) label = "\\label{tab:ExpSigEvtsElMu}";
    if(nmu==2) label = "\\label{tab:ExpSigEvtsMuMu}";
  }
  else{
    if(nmu==-1) label = "\\label{tab:ExpBkgEvtsAll}";
    if(nmu==0) label = "\\label{tab:ExpBkgEvtsElEl}";
    if(nmu==1) label = "\\label{tab:ExpBkgEvtsElMu}";
    if(nmu==2) label = "\\label{tab:ExpBkgEvtsMuMu}";
  }

  tablestring<<tableHeader(vCS, vCC.at(0), caption); tablestring<<"\\hline \n";
  for(size_t i=0; i < vCC.size(); i++){
    tablestring<<vCC.at(i)->samplename;
    for(size_t j =0; j < (vCC.at(i)->nEvents).size(); j++){
      float eff = vCC.at(i)->nEvents.at(j)/vCC.at(i)->nEvents.at(0);
      if(sig) tablestring<<" & "<<eff;
      else tablestring<<" & "<<(1-eff);
    }
    tablestring<<" \\\\"<<std::endl;
  }
  tablestring<<"\\hline \n\\end{tabular} \n"<<label<<'\n'<<"\\end{table} \n\n";

  return tablestring;
 
}

//added by rizki
std::stringstream& printEffTable_v2(std::stringstream& tablestring,std::vector<CutClass*> vCC, std::vector<std::string> vCS, int nmu,bool sig, int BRtype){



  //caption
  std::string caption;
  if(sig){
    if(nmu==-1) caption = "Signal Efficiency (%) Post Associated Z Veto vs. Analysis Cut for all channels combined";
    if(nmu==0) caption = "Signal Efficiency (%) Post Associated Z Veto vs. Analysis Cut for di-electron channel";
    if(nmu==1) caption = "Signal Efficiency (%) Post Associated Z Veto vs. Analysis Cut for electron-muon channel";
    if(nmu==2) caption = "Signal Efficiency (%)  Post Associated Z Veto vs. Analysis Cut for di-muon channel";
  }
  else{
    if(nmu==-1) caption = "Background Rejection Post Associated Z Veto vs. Analysis Cut for all channels combined";
    if(nmu==0) caption = "Background Rejection Post Associated Z Veto vs. Analysis Cut for di-electron channel";
    if(nmu==1) caption = "Background Rejection Post Associated Z Veto vs. Analysis Cut for electron-muon channel";
    if(nmu==2) caption = "Background Rejection Post Associated Z Veto vs. Analysis Cut for di-muon channel";
  }


  //label
  std::string label;
  if(sig){
    if(nmu==-1) label = "\\label{tab:ExpSigEvtsAll}";
    if(nmu==0) label = "\\label{tab:ExpSigEvtsElEl}";
    if(nmu==1) label = "\\label{tab:ExpSigEvtsElMu}";
    if(nmu==2) label = "\\label{tab:ExpSigEvtsMuMu}";
  }
  else{
    if(nmu==-1) label = "\\label{tab:ExpBkgEvtsAll}";
    if(nmu==0) label = "\\label{tab:ExpBkgEvtsElEl}";
    if(nmu==1) label = "\\label{tab:ExpBkgEvtsElMu}";
    if(nmu==2) label = "\\label{tab:ExpBkgEvtsMuMu}";
  }

  tablestring<<tableHeader(vCS, vCC.at(0), caption); tablestring<<"\\hline \n";
  for(size_t i=0; i < vCC.size(); i++){
    tablestring<<vCC.at(i)->samplename;
    for(size_t j =0; j < (vCC.at(i)->nEvents).size(); j++){
    
      float lumi = 41.56*1000.; // inv pb

	  float BR=0.;
	
	  //Set TT
	  float BR_bW=0.5;
	  float BR_tZ=0.25;
	  float BR_tH=0.25;

	  if(BRtype==0){BR_bW=0.5	;BR_tZ=0.25	;BR_tH=0.25	;} //nominal 
	  if(BRtype==1){BR_bW=0.	;BR_tZ=1.0	;BR_tH=0.0	;} // 
	  if(BRtype==2){BR_bW=0.	;BR_tZ=0.8	;BR_tH=0.2	;} // 
	  if(BRtype==3){BR_bW=0.	;BR_tZ=0.6	;BR_tH=0.4	;} // 
	  if(BRtype==4){BR_bW=0.	;BR_tZ=0.4	;BR_tH=0.6	;} // 
	  if(BRtype==5){BR_bW=0.	;BR_tZ=0.2	;BR_tH=0.8	;} // 
	  if(BRtype==6){BR_bW=0.	;BR_tZ=0.0	;BR_tH=1.0	;} // 
	  if(BRtype==7){BR_bW=0.2	;BR_tZ=0.8	;BR_tH=0.0	;} // 
	  if(BRtype==8){BR_bW=0.2	;BR_tZ=0.6	;BR_tH=0.2	;} // 
	  if(BRtype==9){BR_bW=0.2	;BR_tZ=0.4	;BR_tH=0.4	;} // 
	  if(BRtype==10){BR_bW=0.2	;BR_tZ=0.2	;BR_tH=0.6	;} // 
	  if(BRtype==11){BR_bW=0.2	;BR_tZ=0.0	;BR_tH=0.8	;} // 
	  if(BRtype==12){BR_bW=0.4	;BR_tZ=0.6	;BR_tH=0.0	;} // 
	  if(BRtype==13){BR_bW=0.4	;BR_tZ=0.4	;BR_tH=0.2	;} // 
	  if(BRtype==14){BR_bW=0.4	;BR_tZ=0.2	;BR_tH=0.4	;} // 
	  if(BRtype==15){BR_bW=0.4	;BR_tZ=0.0	;BR_tH=0.6	;} // 
	  if(BRtype==16){BR_bW=0.6	;BR_tZ=0.4	;BR_tH=0.0	;} // 
	  if(BRtype==17){BR_bW=0.6	;BR_tZ=0.2	;BR_tH=0.2	;} // 
	  if(BRtype==18){BR_bW=0.6	;BR_tZ=0.0	;BR_tH=0.4	;} // 
	  if(BRtype==19){BR_bW=0.8	;BR_tZ=0.2	;BR_tH=0.0	;} // 
	  if(BRtype==20){BR_bW=0.8	;BR_tZ=0.0	;BR_tH=0.2	;} // 
	  if(BRtype==21){BR_bW=1.0	;BR_tZ=0.0	;BR_tH=0.0	;} // 
	  if(BRtype==22){BR_bW=0.0	;BR_tZ=0.5	;BR_tH=0.5	;} // 
	  if(vCC.at(i)->samplename.find("BWBW")!=std::string::npos) BR = BR_bW*BR_bW;
	  if(vCC.at(i)->samplename.find("TZTZ")!=std::string::npos) BR = BR_tZ*BR_tZ;
	  if(vCC.at(i)->samplename.find("THTH")!=std::string::npos) BR = BR_tH*BR_tH;
	  if(vCC.at(i)->samplename.find("THBW")!=std::string::npos) BR = 2*BR_tH*BR_bW;
	  if(vCC.at(i)->samplename.find("TZBW")!=std::string::npos) BR = 2*BR_tZ*BR_bW;
	  if(vCC.at(i)->samplename.find("TZTH")!=std::string::npos) BR = 2*BR_tZ*BR_tH;

	  //Set BB
	  float BR_tW=0.5;
	  float BR_bZ=0.25;
	  float BR_bH=0.25;

	  if(BRtype==0){BR_tW=0.5	;BR_bZ=0.25	;BR_bH=0.25	;} //nominal 
	  if(BRtype==1){BR_tW=0.	;BR_bZ=1.0	;BR_bH=0.0	;} // 
	  if(BRtype==2){BR_tW=0.	;BR_bZ=0.8	;BR_bH=0.2	;} // 
	  if(BRtype==3){BR_tW=0.	;BR_bZ=0.6	;BR_bH=0.4	;} // 
	  if(BRtype==4){BR_tW=0.	;BR_bZ=0.4	;BR_bH=0.6	;} // 
	  if(BRtype==5){BR_tW=0.	;BR_bZ=0.2	;BR_bH=0.8	;} // 
	  if(BRtype==6){BR_tW=0.	;BR_bZ=0.0	;BR_bH=1.0	;} // 
	  if(BRtype==7){BR_tW=0.2	;BR_bZ=0.8	;BR_bH=0.0	;} // 
	  if(BRtype==8){BR_tW=0.2	;BR_bZ=0.6	;BR_bH=0.2	;} // 
	  if(BRtype==9){BR_tW=0.2	;BR_bZ=0.4	;BR_bH=0.4	;} // 
	  if(BRtype==10){BR_tW=0.2	;BR_bZ=0.2	;BR_bH=0.6	;} // 
	  if(BRtype==11){BR_tW=0.2	;BR_bZ=0.0	;BR_bH=0.8	;} // 
	  if(BRtype==12){BR_tW=0.4	;BR_bZ=0.6	;BR_bH=0.0	;} // 
	  if(BRtype==13){BR_tW=0.4	;BR_bZ=0.4	;BR_bH=0.2	;} // 
	  if(BRtype==14){BR_tW=0.4	;BR_bZ=0.2	;BR_bH=0.4	;} // 
	  if(BRtype==15){BR_tW=0.4	;BR_bZ=0.0	;BR_bH=0.6	;} // 
	  if(BRtype==16){BR_tW=0.6	;BR_bZ=0.4	;BR_bH=0.0	;} // 
	  if(BRtype==17){BR_tW=0.6	;BR_bZ=0.2	;BR_bH=0.2	;} // 
	  if(BRtype==18){BR_tW=0.6	;BR_bZ=0.0	;BR_bH=0.4	;} // 
	  if(BRtype==19){BR_tW=0.8	;BR_bZ=0.2	;BR_bH=0.0	;} // 
	  if(BRtype==20){BR_tW=0.8	;BR_bZ=0.0	;BR_bH=0.2	;} // 
	  if(BRtype==21){BR_tW=1.0	;BR_bZ=0.0	;BR_bH=0.0	;} // 
	  if(BRtype==22){BR_tW=0.0	;BR_bZ=0.5	;BR_bH=0.5	;} // 
	  if(vCC.at(i)->samplename.find("TWTW")!=std::string::npos) BR = BR_tW*BR_tW;
	  if(vCC.at(i)->samplename.find("BZBZ")!=std::string::npos) BR = BR_bZ*BR_bZ;
	  if(vCC.at(i)->samplename.find("BHBH")!=std::string::npos) BR = BR_bH*BR_bH;
	  if(vCC.at(i)->samplename.find("BHTW")!=std::string::npos) BR = 2*BR_bH*BR_tW;
	  if(vCC.at(i)->samplename.find("BZTW")!=std::string::npos) BR = 2*BR_bZ*BR_tW;
	  if(vCC.at(i)->samplename.find("BZBH")!=std::string::npos) BR = 2*BR_bZ*BR_bH;


	  float initBR; //MC sample initially has equal BR's for T -> bW, tZ,tH
	  //TT
	  if(vCC.at(i)->samplename.find("BWBW")!=std::string::npos || vCC.at(i)->samplename.find("TZTZ")!=std::string::npos || vCC.at(i)->samplename.find("THTH")!=std::string::npos ) initBR=0.333*0.333;
	  if(vCC.at(i)->samplename.find("THBW")!=std::string::npos || vCC.at(i)->samplename.find("TZBW")!=std::string::npos || vCC.at(i)->samplename.find("TZTH")!=std::string::npos ) initBR=0.333*0.333*2;
	  //BB
	  if(vCC.at(i)->samplename.find("TWTW")!=std::string::npos || vCC.at(i)->samplename.find("BZBZ")!=std::string::npos || vCC.at(i)->samplename.find("BHBH")!=std::string::npos ) initBR=0.333*0.333;
	  if(vCC.at(i)->samplename.find("BHTW")!=std::string::npos || vCC.at(i)->samplename.find("BZTW")!=std::string::npos || vCC.at(i)->samplename.find("BZBH")!=std::string::npos ) initBR=0.333*0.333*2;

      float nSigEvts=0;
      float Xsec=0;
      //if(vCC.at(i)->samplename.find("TprimeTprime_M-800")!=std::string::npos){ nSigEvts = 795000.; Xsec=0.196 ;}       
      //if(vCC.at(i)->samplename.find("TprimeTprime_M-900")!=std::string::npos){ nSigEvts = 831200.; Xsec=0.0903 ;}
      if(vCC.at(i)->samplename.find("TprimeTprime_M-1000")!=std::string::npos){ nSigEvts = 1002517.455; Xsec=0.0440 ;}
      if(vCC.at(i)->samplename.find("TprimeTprime_M-1100")!=std::string::npos){ nSigEvts = 895042.045; Xsec=0.0224 ;}
      if(vCC.at(i)->samplename.find("TprimeTprime_M-1200")!=std::string::npos){ nSigEvts = 1003441.777; Xsec=0.0118 ;}
      if(vCC.at(i)->samplename.find("TprimeTprime_M-1300")!=std::string::npos){ nSigEvts = 1026082.979; Xsec=0.00639 ;}
      if(vCC.at(i)->samplename.find("TprimeTprime_M-1400")!=std::string::npos){ nSigEvts = 979603.275; Xsec=0.00354 ;}
      if(vCC.at(i)->samplename.find("TprimeTprime_M-1500")!=std::string::npos){ nSigEvts = 985756.619; Xsec=0.00200 ;}
      if(vCC.at(i)->samplename.find("TprimeTprime_M-1600")!=std::string::npos){ nSigEvts = 989851.281; Xsec=0.001148 ;}
      if(vCC.at(i)->samplename.find("TprimeTprime_M-1700")!=std::string::npos){ nSigEvts = 939135.751; Xsec=0.000666 ;}
      if(vCC.at(i)->samplename.find("TprimeTprime_M-1800")!=std::string::npos){ nSigEvts = 884660.318; Xsec=0.000391 ;}

      //if(vCC.at(i)->samplename.find("BprimeBprime_M-800")!=std::string::npos){ nSigEvts = 826200.; Xsec=0.196 ;}
      if(vCC.at(i)->samplename.find("BprimeBprime_M-900")!=std::string::npos){ nSigEvts = 959939.583 * 687000.0 / 817000.0; Xsec=0.0903 ;}
      if(vCC.at(i)->samplename.find("BprimeBprime_M-1000")!=std::string::npos){ nSigEvts = 1011510.550; Xsec=0.0440 ;}
      if(vCC.at(i)->samplename.find("BprimeBprime_M-1100")!=std::string::npos){ nSigEvts = 969137.000; Xsec=0.0224 ;}
      if(vCC.at(i)->samplename.find("BprimeBprime_M-1200")!=std::string::npos){ nSigEvts = 1027818.122; Xsec=0.0118 ;}
      if(vCC.at(i)->samplename.find("BprimeBprime_M-1300")!=std::string::npos){ nSigEvts = 917763.768; Xsec=0.00639 ;}
      if(vCC.at(i)->samplename.find("BprimeBprime_M-1400")!=std::string::npos){ nSigEvts = 877071.614; Xsec=0.00354 ;}
      if(vCC.at(i)->samplename.find("BprimeBprime_M-1500")!=std::string::npos){ nSigEvts = 804769.156; Xsec=0.00200 ;}
      if(vCC.at(i)->samplename.find("BprimeBprime_M-1600")!=std::string::npos){ nSigEvts = 895604.498; Xsec=0.001148 ;}
      if(vCC.at(i)->samplename.find("BprimeBprime_M-1700")!=std::string::npos){ nSigEvts = 946782.494; Xsec=0.000666 ;}
      if(vCC.at(i)->samplename.find("BprimeBprime_M-1800")!=std::string::npos){ nSigEvts = 915634.724; Xsec=0.000666 ;}
      //std::cout << "samplename: "<< vCC.at(i)->samplename << "	nSigEvts = " << nSigEvts << "	selected Evts = "<< vCC.at(i)->nEvents.at(j) <<std::endl;

      float weight = lumi * ( (Xsec*BR) / (nSigEvts* initBR) ); 

//       float eff= vCC.at(i)->nEvents.at(j)/weight /nSigEvts * 100; //added by rizki --> MISTAKE!
      float eff= vCC.at(i)->nEvents.at(j)/weight / (nSigEvts*initBR) * 100; //added by rizki --> corrected Mar5-2018.

      if(sig) tablestring<<" & "<<eff;
      else tablestring<<" & "<<(1-eff);
    }
    tablestring<<" \\\\"<<std::endl;
  }
  tablestring<<"\\hline \n\\end{tabular} \n"<<label<<'\n'<<"\\end{table} \n\n";

  return tablestring;
 
} 


std::stringstream& printChargeMisIDTable_lpt(std::stringstream& table){


  table<<"\\begin{table}\n\\centering\n";
//   table<<"\\topcaption{Charge misID rate for electrons where both electrons have a \\pt less than 100 GeV. Measured in data requiring two tight electrons with \\pt greater than 30 GeV and an invariant mass within 10 GeV of the Z-boson mass.}"; //commented by rizki
  table<<"\\caption{Charge misID rate for electrons where both electrons have a \\pt less than 100 GeV. Measured in data requiring two tight electrons with \\pt greater than 30 GeV and an invariant mass within 10 GeV of the Z-boson mass.}"; //added by rizki
  table<<"\\begin{tabular}{|";
  for(int i=0;i<2;i++){
    table<<"c|";
  }

  table<<"}\n\\hline\\hline\n";

  table<<"Electron $\\eta$ & Charge MisID Rate\\\\\n\\hline\n";

  //TFile* weightfile = new TFile("../scripts/ChargeMisID/ChargeMisID_Data_All_Electrons_MVA2017TightIsoRC_corrected.root");
  TFile* weightfile = new TFile("../scripts/ChargeMisID/ChargeMisID_Data_2017All_Electrons_MVA2017TightV2IsoRC_corrected.root");

  TH1F* h = (TH1F*) weightfile->Get("h_num_lpt");
  //TH1F* den = (TH1F*) weightfile->Get("etaDenHist_lpt");
  //h->Divide(den);
  TGraphErrors* g = new TGraphErrors(h);
  for(unsigned int j=0; j< g->GetN(); j++){
    std::cout<<"making point: "<<j<<" x bin: "<<g->GetX()[j]<<"+/-"<<g->GetErrorX(j)<<" and y value: "<<g->GetY()[j]<<std::endl;
    float xlow = g->GetX()[j] - g->GetErrorX(j);
    float xhigh = g->GetX()[j] + g->GetErrorX(j);
    std::string etabin = Form("%.1f $>\\eta>$ %.1f",xlow,xhigh);
    std::string misIDRate = Form("%1.5f",g->GetY()[j]);
    table<<etabin<<" & "<<misIDRate<<"\\\\\n";
  }

  table<<"\\hline\n\\end{tabular}\\end{table}";

  std::cout<<"made everything but table footer"<<std::endl;

  std::cout<<"made table footer"<<std::endl;

  return table;

}

std::stringstream& printChargeMisIDTable_hpt(std::stringstream& table){


  table<<"\\begin{table}\n\\centering\n";
//   table<<"\\topcaption{Charge misID rate for electrons where either of the electron pair has \\pt above 100 GeV. Measured in data requiring two tight electrons with \\pt greater than 30 GeV and an invariant mass within 10 GeV of the Z-boson mass.}"; //commented by rizki
  table<<"\\caption{Charge misID rate for electrons where either of the electron pair has \\pt above 100 GeV. Measured in data requiring two tight electrons with \\pt greater than 30 GeV and an invariant mass within 10 GeV of the Z-boson mass.}"; //added by rizki
  table<<"\\begin{tabular}{|";
  for(int i=0;i<2;i++){
    table<<"c|";
  }

  table<<"}\n\\hline\\hline\n";

  table<<"Electron $\\eta$ & Charge MisID Rate\\\\\n\\hline\n";

  //TFile* weightfile = new TFile("../scripts/ChargeMisID/ChargeMisID_Data_All_Electrons_MVA2017TightIsoRC_corrected.root");
  TFile* weightfile = new TFile("../scripts/ChargeMisID/ChargeMisID_Data_2017All_Electrons_MVA2017TightV2IsoRC_corrected.root");

  TH1F* h = (TH1F*) weightfile->Get("hpt_final");
  //TH1F* den = (TH1F*) weightfile->Get("etaDenHist_hpt");
  //h->Divide(den);
  TGraphErrors* g = new TGraphErrors(h);
  for(unsigned int j=0; j< g->GetN(); j++){
    std::cout<<"making point: "<<j<<" x bin: "<<g->GetX()[j]<<"+/-"<<g->GetErrorX(j)<<" and y value: "<<g->GetY()[j]<<std::endl;
    float xlow = g->GetX()[j] - g->GetErrorX(j);
    float xhigh = g->GetX()[j] + g->GetErrorX(j);
    std::string etabin = Form("%.1f $>\\eta>$ %.1f",xlow,xhigh);
    std::string misIDRate = Form("%1.5f",g->GetY()[j]);
    table<<etabin<<" & "<<misIDRate<<"\\\\\n";
  }

  table<<"\\hline\n\\end{tabular}\\end{table}";

  std::cout<<"made everything but table footer"<<std::endl;

  std::cout<<"made table footer"<<std::endl;

  return table;

}


std::stringstream& printChargeMisIDTable_hhpt(std::stringstream& table){


  table<<"\\begin{table}\n\\centering\n";
//   table<<"\\topcaption{Charge misID rate for electrons where both electrons have \\pt above 100 GeV. Measured in data requiring two tight electrons with \\pt greater than 30 GeV and an invariant mass within 10 GeV of the Z-boson mass.}"; //commented by rizki
  table<<"\\caption{Charge misID rate for electrons where both electrons have \\pt above 100 GeV. Measured in data requiring two tight electrons with \\pt greater than 30 GeV and an invariant mass within 10 GeV of the Z-boson mass.}"; //added by rizki
  table<<"\\begin{tabular}{|";
  for(int i=0;i<2;i++){
    table<<"c|";
  }

  table<<"}\n\\hline\\hline\n";

  table<<"Electron $\\eta$ & Charge MisID Rate\\\\\n\\hline\n";

  //TFile* weightfile = new TFile("../scripts/ChargeMisID/ChargeMisID_Data_All_Electrons_MVA2017TightIsoRC_corrected.root");
  TFile* weightfile = new TFile("../scripts/ChargeMisID/ChargeMisID_Data_2017All_Electrons_MVA2017TightV2IsoRC_corrected.root");

  TH1F* h = (TH1F*) weightfile->Get("hhpt_final");
  //TH1F* den = (TH1F*) weightfile->Get("etaDenHist_hpt");
  //h->Divide(den);
  TGraphErrors* g = new TGraphErrors(h);
  for(unsigned int j=0; j< g->GetN(); j++){
    std::cout<<"making point: "<<j<<" x bin: "<<g->GetX()[j]<<"+/-"<<g->GetErrorX(j)<<" and y value: "<<g->GetY()[j]<<std::endl;
    float xlow = g->GetX()[j] - g->GetErrorX(j);
    float xhigh = g->GetX()[j] + g->GetErrorX(j);
    std::string etabin = Form("%.1f $>\\eta>$ %.1f",xlow,xhigh);
    std::string misIDRate = Form("%1.5f",g->GetY()[j]);
    table<<etabin<<" & "<<misIDRate<<"\\\\\n";
  }

  table<<"\\hline\n\\end{tabular}\\end{table}";

  std::cout<<"made everything but table footer"<<std::endl;

  std::cout<<"made table footer"<<std::endl;

  return table;

}

std::stringstream& printFinalTable(std::stringstream& tablestring,std::vector<Sample*> vBkgBF, std::vector<Sample*> vSigBF,std::vector<Sample*> vBkgEH, std::vector<Sample*> vSigEH, Sample* dataSample, std::vector<std::string> vCutString,std::string era){

  std::cout << "--------> printing Final Table ("  << era <<  ")" << std::endl; //added by rizki

  //tablestring<<"\\begin{table}\n\\centering\n\\scriptsize\n\\begin{tabular}{|c|cccc|c|c|}\\hline\\hline\n"; //commented by rizki
  tablestring<<"\\begin{table}\n\\centering\n\\scriptsize\n\\begin{tabular}{|c|cccc|c|c|c|}\\hline\\hline\n"; //added by rizki
  //caption
  std::string caption;
  caption = "Summary of expected and observed events for all channels for "+era+". We show also the representative yields for an 1000 \\GeV \\xft. All systematic uncertainties are included.";
//   tablestring<<"PSS MC&NonPrompt&ChargeMisID& Total Background & 1000 \\GeV \\xft&Observed\\\\\n"; //commented by rizki
  tablestring<<"& PSS MC&NonPrompt&ChargeMisID& Total Background & 1000 GeV &Observed & $\\frac{S}{\\sqrt{S+B}}$\\\\\n"; // added by rizki
//   tablestring<<"& PSS MC&NonPrompt&ChargeMisID& Total Background & 1200 GeV &Observed & $\\frac{S}{\\sqrt{S+B}}$\\\\\n"; // added by rizki
  std::string label;
  //label
  label = "\\label{tab:FinalYields"+era+"}";


  for(int nmu=-1; nmu<3; nmu++){
	std::cout << "--------> channel # of mu: " << nmu << std::endl; //added by rizki

    //now make a vector of cutClass for bkg
    std::vector<CutClass*> vCutBkgBF = getCutClassVector(vBkgBF,vCutString,nmu);
    //now make a vector of cutClass for sig
    std::vector<CutClass*> vCutSigBF = getCutClassVector(vSigBF,vCutString,nmu);
    //now make a vector of cutClass for bkg
    std::vector<CutClass*> vCutBkgEH = getCutClassVector(vBkgEH,vCutString,nmu);
    //now make a vector of cutClass for sig
    std::vector<CutClass*> vCutSigEH = getCutClassVector(vSigEH,vCutString,nmu);
    CutClass* cutSig = 0;
    std::vector<CutClass*> cutSig_M1000; //added by rizki


    //now pick based on era
    std::vector<CutClass*> vCutBkg;
    if(era.find("2017B-F")) vCutBkg = vCutBkgBF;
    else if(era.find("2016EH")) vCutBkg = vCutBkgEH;
    else{ vCutBkg = addCutClassVectors(vCutBkgBF,vCutBkgEH);} //else full era
    std::vector<CutClass*> vCutSig;
    if(era.find("2017B-F")) vCutSig = vCutSigBF;
    else if(era.find("2016EH")) vCutSig = vCutSigEH;
    else{ vCutSig = addCutClassVectors(vCutSigBF,vCutSigEH); }//else full era


    for(unsigned int j=0; j<vCutSig.size();j++){
//       if(vCutSig.at(j)->samplename.find("1000")!=std::string::npos && vCutSig.at(j)->samplename.find("RH")!=std::string::npos) cutSig = vCutSig.at(j);
      if(vCutSig.at(j)->samplename.find("1000")!=std::string::npos) cutSig_M1000.push_back(vCutSig.at(j)); //edited by rizki
//       if(vCutSig.at(j)->samplename.find("1200")!=std::string::npos) cutSig_M1000.push_back(vCutSig.at(j)); //edited by rizki
    }


    //now get cutcalss fordata
    CutClass* cutData = makeCutClass(dataSample, vCutString, nmu);


    //first machinery to get events in nice format
    float events_mctot=0;
    float errors_mctot=0;
    float events_nptot=0;
    float errors_nptot=0;
    float events_cmtot=0;
    float errors_cmtot=0;
    float events_tot = 0;
    float errors_tot = 0;

    //get background events
    std::cout << "Printing Final Table: Processing bkg ..." << std::endl; //added by rizki
    for(size_t i=0; i < vCutBkg.size(); i++){
      //std::cout << "	" << vCutBkg.at(i)->samplename << std::endl; //added by rizki
      float sys=0;

      if(vCutBkg.at(i)->samplename=="NonPrompt") sys= pow(unc_NP, 2);
      else if(vCutBkg.at(i)->samplename=="ChargeMisID") sys= pow(unc_CMID, 2);
      
      else if(vCutBkg.at(i)->samplename=="TTW") sys= pow(uncPDF_TTW, 2) + pow(uncSCALE_TTW, 2) + pow(uncJES_TTW,2) + pow(uncJER_TTW,2) + pow(uncPU_TTW,2); //pdf,scale,jes,jer,pu
      else if(vCutBkg.at(i)->samplename=="TTZ") sys= pow(uncPDF_TTZ, 2) + pow(uncSCALE_TTZ, 2) + pow(uncJES_TTZ,2) + pow(uncJER_TTZ,2) + pow(uncPU_TTZ,2);
      else if(vCutBkg.at(i)->samplename=="TTH") sys= pow(uncPDF_TTH, 2) + pow(uncSCALE_TTH, 2) + pow(uncJES_TTH,2) + pow(uncJER_TTH,2) + pow(uncPU_TTH,2);
      else if(vCutBkg.at(i)->samplename=="TTTT") sys= pow(uncPDF_TTTT, 2) + pow(uncSCALE_TTTT, 2) + pow(uncJES_TTTT,2) + pow(uncJER_TTTT,2) + pow(uncPU_TTTT,2);
      else if(vCutBkg.at(i)->samplename=="WZ") sys= pow(uncPDF_WZ, 2) + pow(uncSCALE_WZ, 2) + pow(uncJES_WZ,2) + pow(uncJER_WZ,2) + pow(uncPU_WZ,2);
      else if(vCutBkg.at(i)->samplename=="ZZ") sys= pow(uncPDF_ZZ, 2) + pow(uncSCALE_ZZ, 2) + pow(uncJES_ZZ,2) + pow(uncJER_ZZ,2) + pow(uncPU_ZZ,2);
      else if(vCutBkg.at(i)->samplename=="WpWp") sys= pow(uncPDF_WpWp, 2) + pow(uncSCALE_WpWp, 2) + pow(uncJES_WpWp,2) + pow(uncJER_WpWp,2) + pow(uncPU_WpWp,2);
      else if(vCutBkg.at(i)->samplename=="WWZ") sys= pow(uncPDF_WWZ, 2) + pow(uncSCALE_WWZ, 2) + pow(uncJES_WWZ,2) + pow(uncJER_WWZ,2) + pow(uncPU_WWZ,2);
      else if(vCutBkg.at(i)->samplename=="WZZ") sys= pow(uncPDF_WZZ, 2) + pow(uncSCALE_WZZ, 2) + pow(uncJES_WZZ,2) + pow(uncJER_WZZ,2) + pow(uncPU_WZZ,2);
      else if(vCutBkg.at(i)->samplename=="ZZZ") sys= pow(uncPDF_ZZZ, 2) + pow(uncSCALE_ZZZ, 2) + pow(uncJES_ZZZ,2) + pow(uncJER_ZZZ,2) + pow(uncPU_ZZZ,2);

//       else if(vCutBkg.at(i)->samplename.find("X53X53")!=std::string::npos) sys = pow(0.03,2) + pow(0.01,2) + pow(0.01,2); //jes, jer, pu
      else if(vCutSig.at(i)->samplename.find("prime")!=std::string::npos) sys = pow(uncPDF_sig, 2) + pow(uncSCALE_sig, 2) + pow(uncJES_sig,2)+ pow(uncJER_sig,2) + pow(uncPU_sig,2) ; //jes, jer, pu
      else sys = pow(0.5,2);

      if(vCutBkg.at(i)->samplename=="TTZ") sys= sys + pow(uncTrigSF_TTZ,2) + pow(uncIDSF_TTZ,2); // trig plus id
      else if(vCutBkg.at(i)->samplename=="TTW") sys= sys + pow(uncTrigSF_TTW,2) + pow(uncIDSF_TTW,2);
      else if(vCutBkg.at(i)->samplename=="TTH") sys= sys + pow(uncTrigSF_TTH,2) + pow(uncIDSF_TTH,2);
      else if(vCutBkg.at(i)->samplename=="TTTT") sys= sys + pow(uncTrigSF_TTTT,2) + pow(uncIDSF_TTTT,2);
      else if(vCutBkg.at(i)->samplename=="WZ") sys= sys + pow(uncTrigSF_WZ,2) + pow(uncIDSF_WZ,2);
      else if(vCutBkg.at(i)->samplename=="ZZ") sys= sys + pow(uncTrigSF_ZZ,2) + pow(uncIDSF_ZZ,2);
      else if(vCutBkg.at(i)->samplename=="WpWp") sys= sys + pow(uncTrigSF_WpWp,2) + pow(uncIDSF_WpWp,2);
      else if(vCutBkg.at(i)->samplename=="WWZ") sys= sys + pow(uncTrigSF_WWZ,2) + pow(uncIDSF_WWZ,2);
      else if(vCutBkg.at(i)->samplename=="WZZ") sys= sys + pow(uncTrigSF_WZZ,2) + pow(uncIDSF_WZZ,2);
      else if(vCutBkg.at(i)->samplename=="ZZZ") sys= sys + pow(uncTrigSF_ZZZ,2) + pow(uncIDSF_ZZZ,2);
      else if(vCutBkg.at(i)->samplename.find("prime")!=std::string::npos) sys = sys + pow(uncTrigSF_sig,2) + pow(uncIDSF_sig,2);

      if(! (vCutBkg.at(i)->samplename=="NonPrompt" || vCutBkg.at(i)->samplename=="ChargeMisID")){
        sys = sys + pow(uncIsoSF,2) + pow(uncLumi,2); //iso plus lumi 
      }

      std::cout<<"sample: "<<vCutBkg.at(i)->samplename<<" and total systematic: "<<pow(sys,0.5)<<std::endl;
      
      for(size_t j =0; j < (vCutBkg.at(i)->nEvents).size(); j++){
		if(j < (vCutBkg.at(i)->nEvents).size() -1) continue; //only get the last yields
		float errsys = sys*pow((vCutBkg.at(i)->nEvents).at(j),2);
		float err = pow((vCutBkg.at(i)->vErr).at(j),2) + errsys;
// 		if(! (vCutBkg.at(i)->samplename=="NonPrompt" || vCutBkg.at(i)->samplename=="ChargeMisID")){ //only add together if MC background //commented by rizki
		if(! (vCutBkg.at(i)->samplename=="NonPrompt" || vCutBkg.at(i)->samplename=="ChargeMisID" || vCutBkg.at(i)->samplename.find("prime")!=std::string::npos)){ //only add together if MC background //added edited by rizki
		  events_mctot = events_mctot + (vCutBkg.at(i)->nEvents).at(j);
		  errors_mctot = errors_mctot + err;

		  events_tot = events_tot + (vCutBkg.at(i)->nEvents).at(j);
		  errors_tot = errors_tot + err;
		}      
		else if(vCutBkg.at(i)->samplename!="NonPrompt" && vCutBkg.at(i)->samplename=="ChargeMisID"){ //only add together if MC background
		  events_tot = events_tot + (vCutBkg.at(i)->nEvents).at(j);
		  errors_tot = errors_tot + err;
		  events_cmtot = events_cmtot + (vCutBkg.at(i)->nEvents).at(j);
		  errors_cmtot = err;	  
		}
		else if(vCutBkg.at(i)->samplename=="NonPrompt" && vCutBkg.at(i)->samplename!="ChargeMisID"){ //only add together if MC background
		  events_tot = events_tot + (vCutBkg.at(i)->nEvents).at(j);
		  errors_tot = errors_tot + err;
		  events_nptot = events_nptot + (vCutBkg.at(i)->nEvents).at(j);
		  errors_nptot = err;	  
		}

      }


      
    }

    //now signal
//     float sig = (cutSig->nEvents).at( (cutSig->nEvents).size()-1); //commented y rizki
    float sig = 0; //added by rizki
    float errors_sig=0; ; //added by rizki
	float errstat =0; //added by rizki		
    std::cout << "Printing Final Table: Processing signal ..." << std::endl;
    for(int i=0; i<cutSig_M1000.size();i++){ 
    	sig = sig+(cutSig_M1000.at(i)->nEvents).at( (cutSig_M1000.at(i)->nEvents).size()-1); //added by rizki
		errstat = errstat + pow((cutSig_M1000.at(i)->vErr).at( (cutSig_M1000.at(i)->nEvents).size()-1),2); //added by rizki		
    	std::cout << "	" << cutSig_M1000.at(i)->samplename << " : "<< (cutSig_M1000.at(i)->nEvents).at( (cutSig_M1000.at(i)->nEvents).size()-1) << " +- " << (cutSig_M1000.at(i)->vErr).at( (cutSig_M1000.at(i)->nEvents).size()-1) << "(stat)" << std::endl;

    }
	float sys = pow(0.01,2)+ pow(0.02,2) + pow(0.01,2); //jes, jer, pu //added by rizki
	sys = sys + 2*pow(0.02,2) + pow(0.03,2) + pow(.025,2); //id iso and trigger  plus lumi  plus pileup plus //added by rizki
	float errsys = sys*pow(sig,2); //added by rizki
	errors_sig = errsys + errstat;	 //added by rizki
    
    //ok now I have all background, so get observed with no errors
   float obs =  (cutData->nEvents).at( (cutData->nEvents).size()-1);

   //write channel
   if(nmu==0) tablestring<<"Di-electron&";
   else if(nmu==1) tablestring<<"Electron-Muon&";
   else if(nmu==2) tablestring<<"Di-muon&";
   else tablestring<<"All&";
    //now write table line -- BLIND FOR NOW
   tablestring<<"$"<<events_mctot<<"\\pm"<<pow(errors_mctot,0.5)<<"$ & $"<<events_nptot<<"\\pm"<<pow(errors_nptot,0.5)<<"$ & $"<<events_cmtot<<"\\pm"<<pow(errors_cmtot,0.5)<<"$ &$ "<<events_tot<<"\\pm"<<pow(errors_tot,0.5)<<"$&"<<sig<<"\\pm"<<pow(errors_sig,0.5)<<" & "<<obs<< " & " << sig / sqrt(sig+events_tot) <<"\\\\\n";  //added by rizki
   //tablestring<<"$"<<events_mctot<<"\\pm"<<pow(errors_mctot,0.5)<<"$&$"<<events_nptot<<"\\pm"<<pow(errors_nptot,0.5)<<"$&$"<<events_cmtot<<"\\pm"<<pow(errors_cmtot,0.5)<<"$ &$ "<<events_tot<<"\\pm"<<pow(errors_tot,0.5)<<"$&"<<sig<<" & "<<obs<<"\\\\\n"; //commented by rizki 
   //tablestring<<"$"<<events_mctot<<"\\pm"<<pow(errors_mctot,0.5)<<"$&$"<<events_nptot<<"\\pm"<<pow(errors_nptot,0.5)<<"$&$"<<events_cmtot<<"\\pm"<<pow(errors_cmtot,0.5)<<"$ &$ "<<events_tot<<"\\pm"<<pow(errors_tot,0.5)<<"$&"<<sig<<" & XXX\\\\\n"; 
  }//end loop over channels

  tablestring<<"\\hline \n\\end{tabular} \n"<<label<<'\n'<<"\\end{table} \n\n";

  return tablestring;


    
}

//added by rizki
std::stringstream& printFinalTable_v2(std::stringstream& tablestring,std::vector<Sample*> vBkgBF, std::vector<Sample*> vSigBF,std::vector<Sample*> vBkgEH, std::vector<Sample*> vSigEH, Sample* dataSample, std::vector<std::string> vCutString,std::string era){

  std::cout << "--------> printing Final Table ("  << era <<  ")" << std::endl; //added by rizki

  //tablestring<<"\\begin{table}\n\\centering\n\\scriptsize\n\\begin{tabular}{|c|cccc|c|c|}\\hline\\hline\n"; //commented by rizki
  tablestring<<"\\begin{table}\n\\centering\n\\scriptsize\n\\begin{tabular}{|c|c|c|c|}\\hline\\hline\n"; //added by rizki
  //caption
  std::string caption;
  caption = "Summary of expected and observed events for all channels for "+era+". We show also the representative yields for an 1000 \\GeV \\xft. All systematic uncertainties are included.";
//   tablestring<<"PSS MC&NonPrompt&ChargeMisID& Total Background & 1000 \\GeV \\xft&Observed\\\\\n"; //commented by rizki
  tablestring<<"Sample & $ee$ & $e\\mu$ & $\\mu\\mu$ \\\\ \\hline \n"; // added by rizki
  std::string label;
  //label
  label = "\\label{tab:FinalYields"+era+"}";
  
  
  //added by rizki - start
  float sig_ee;
  float sigErr_ee;
  float sig2_ee;
  float sig2Err_ee;
  float mcbkg_ee;
  float mcbkgErr_ee;
  float np_ee;
  float npErr_ee;
  float chmid_ee;
  float chmidErr_ee;
  float totalbkg_ee;
  float totalbkgErr_ee;
  float data_ee;

  float sig_em;
  float sigErr_em;
  float sig2_em;
  float sig2Err_em;
  float mcbkg_em;
  float mcbkgErr_em;
  float np_em;
  float npErr_em;
  float chmid_em;
  float chmidErr_em;
  float totalbkg_em;
  float totalbkgErr_em;
  float data_em;

  float sig_mm;
  float sigErr_mm;
  float sig2_mm;
  float sig2Err_mm;
  float mcbkg_mm;
  float mcbkgErr_mm;
  float np_mm;
  float npErr_mm;
  float chmid_mm;
  float chmidErr_mm;
  float totalbkg_mm;
  float totalbkgErr_mm;
  float data_mm;
  //added by rizki - end


  for(int nmu=-1; nmu<3; nmu++){
	std::cout << "--------> channel # of mu: " << nmu << std::endl; //added by rizki

    //now make a vector of cutClass for bkg
    std::vector<CutClass*> vCutBkgBF = getCutClassVector(vBkgBF,vCutString,nmu);
    //now make a vector of cutClass for sig
    std::vector<CutClass*> vCutSigBF = getCutClassVector(vSigBF,vCutString,nmu);
    //now make a vector of cutClass for bkg
    std::vector<CutClass*> vCutBkgEH = getCutClassVector(vBkgEH,vCutString,nmu);
    //now make a vector of cutClass for sig
    std::vector<CutClass*> vCutSigEH = getCutClassVector(vSigEH,vCutString,nmu);
    CutClass* cutSig = 0;
    std::vector<CutClass*> cutSig_M1000; //added by rizki
    std::vector<CutClass*> cutSig_M1200; //added by rizki


    //now pick based on era
    std::vector<CutClass*> vCutBkg;
    if(era.find("2017B-F")) vCutBkg = vCutBkgBF;
    else if(era.find("2016EH")) vCutBkg = vCutBkgEH;
    else{ vCutBkg = addCutClassVectors(vCutBkgBF,vCutBkgEH);} //else full era
    std::vector<CutClass*> vCutSig;
    if(era.find("2017B-F")) vCutSig = vCutSigBF;
    else if(era.find("2016EH")) vCutSig = vCutSigEH;
    else{ vCutSig = addCutClassVectors(vCutSigBF,vCutSigEH); }//else full era


    for(unsigned int j=0; j<vCutSig.size();j++){
      if(vCutSig.at(j)->samplename.find("1000")!=std::string::npos) cutSig_M1000.push_back(vCutSig.at(j)); //edited by rizki
      if(vCutSig.at(j)->samplename.find("1200")!=std::string::npos) cutSig_M1200.push_back(vCutSig.at(j)); //edited by rizki
    }


    //now get cutcalss fordata
    CutClass* cutData = makeCutClass(dataSample, vCutString, nmu);


    //first machinery to get events in nice format
    float events_mctot=0;
    float errors_mctot=0;
    float events_nptot=0;
    float errors_nptot=0;
    float events_cmtot=0;
    float errors_cmtot=0;
    float events_tot = 0;
    float errors_tot = 0;

    //get background events
    std::cout << "Printing Final Table: Processing bkg ..." << std::endl; //added by rizki
    for(size_t i=0; i < vCutBkg.size(); i++){
      //std::cout << "	" << vCutBkg.at(i)->samplename << std::endl; //added by rizki
      float sys=0;

      if(vCutBkg.at(i)->samplename=="NonPrompt") sys= pow(unc_NP, 2);
      else if(vCutBkg.at(i)->samplename=="ChargeMisID") sys= pow(unc_CMID, 2);
      
      else if(vCutBkg.at(i)->samplename=="TTW") sys= pow(uncPDF_TTW, 2) + pow(uncSCALE_TTW, 2) + pow(uncJES_TTW,2) + pow(uncJER_TTW,2) + pow(uncPU_TTW,2); //pdf,scale,jes,jer,pu
      else if(vCutBkg.at(i)->samplename=="TTZ") sys= pow(uncPDF_TTZ, 2) + pow(uncSCALE_TTZ, 2) + pow(uncJES_TTZ,2) + pow(uncJER_TTZ,2) + pow(uncPU_TTZ,2);
      else if(vCutBkg.at(i)->samplename=="TTH") sys= pow(uncPDF_TTH, 2) + pow(uncSCALE_TTH, 2) + pow(uncJES_TTH,2) + pow(uncJER_TTH,2) + pow(uncPU_TTH,2);
      else if(vCutBkg.at(i)->samplename=="TTTT") sys= pow(uncPDF_TTTT, 2) + pow(uncSCALE_TTTT, 2) + pow(uncJES_TTTT,2) + pow(uncJER_TTTT,2) + pow(uncPU_TTTT,2);
      else if(vCutBkg.at(i)->samplename=="WZ") sys= pow(uncPDF_WZ, 2) + pow(uncSCALE_WZ, 2) + pow(uncJES_WZ,2) + pow(uncJER_WZ,2) + pow(uncPU_WZ,2);
      else if(vCutBkg.at(i)->samplename=="ZZ") sys= pow(uncPDF_ZZ, 2) + pow(uncSCALE_ZZ, 2) + pow(uncJES_ZZ,2) + pow(uncJER_ZZ,2) + pow(uncPU_ZZ,2);
      else if(vCutBkg.at(i)->samplename=="WpWp") sys= pow(uncPDF_WpWp, 2) + pow(uncSCALE_WpWp, 2) + pow(uncJES_WpWp,2) + pow(uncJER_WpWp,2) + pow(uncPU_WpWp,2);
      else if(vCutBkg.at(i)->samplename=="WWZ") sys= pow(uncPDF_WWZ, 2) + pow(uncSCALE_WWZ, 2) + pow(uncJES_WWZ,2) + pow(uncJER_WWZ,2) + pow(uncPU_WWZ,2);
      else if(vCutBkg.at(i)->samplename=="WZZ") sys= pow(uncPDF_WZZ, 2) + pow(uncSCALE_WZZ, 2) + pow(uncJES_WZZ,2) + pow(uncJER_WZZ,2) + pow(uncPU_WZZ,2);
      else if(vCutBkg.at(i)->samplename=="ZZZ") sys= pow(uncPDF_ZZZ, 2) + pow(uncSCALE_ZZZ, 2) + pow(uncJES_ZZZ,2) + pow(uncJER_ZZZ,2) + pow(uncPU_ZZZ,2);

//       else if(vCutBkg.at(i)->samplename.find("X53X53")!=std::string::npos) sys = pow(0.03,2) + pow(0.01,2) + pow(0.01,2); //jes, jer, pu
      else if(vCutBkg.at(i)->samplename.find("prime")!=std::string::npos) sys = pow(uncPDF_sig, 2) + pow(uncSCALE_sig, 2) + pow(uncJES_sig,2)+ pow(uncJER_sig,2) + pow(uncPU_sig,2) ; //jes, jer, pu
      //else if(vCutSig.at(i)->samplename.find("prime")!=std::string::npos) sys = pow(0.01,2)+ pow(0.02,2) + pow(0.01,2); //jes, jer, pu
      //else sys = pow(0.5,2);
 
      if(vCutBkg.at(i)->samplename=="TTZ") sys= sys + pow(uncTrigSF_TTZ,2) + pow(uncIDSF_TTZ,2); // trig plus id
      else if(vCutBkg.at(i)->samplename=="TTW") sys= sys + pow(uncTrigSF_TTW,2) + pow(uncIDSF_TTW,2);
      else if(vCutBkg.at(i)->samplename=="TTH") sys= sys + pow(uncTrigSF_TTH,2) + pow(uncIDSF_TTH,2);
      else if(vCutBkg.at(i)->samplename=="TTTT") sys= sys + pow(uncTrigSF_TTTT,2) + pow(uncIDSF_TTTT,2);
      else if(vCutBkg.at(i)->samplename=="WZ") sys= sys + pow(uncTrigSF_WZ,2) + pow(uncIDSF_WZ,2);
      else if(vCutBkg.at(i)->samplename=="ZZ") sys= sys + pow(uncTrigSF_ZZ,2) + pow(uncIDSF_ZZ,2);
      else if(vCutBkg.at(i)->samplename=="WpWp") sys= sys + pow(uncTrigSF_WpWp,2) + pow(uncIDSF_WpWp,2);
      else if(vCutBkg.at(i)->samplename=="WWZ") sys= sys + pow(uncTrigSF_WWZ,2) + pow(uncIDSF_WWZ,2);
      else if(vCutBkg.at(i)->samplename=="WZZ") sys= sys + pow(uncTrigSF_WZZ,2) + pow(uncIDSF_WZZ,2);
      else if(vCutBkg.at(i)->samplename=="ZZZ") sys= sys + pow(uncTrigSF_ZZZ,2) + pow(uncIDSF_ZZZ,2);
      else if(vCutBkg.at(i)->samplename.find("prime")!=std::string::npos) sys = sys + pow(uncTrigSF_sig,2) + pow(uncIDSF_sig,2);

      if(! (vCutBkg.at(i)->samplename=="NonPrompt" || vCutBkg.at(i)->samplename=="ChargeMisID")){
        sys = sys + pow(uncIsoSF,2) + pow(uncLumi,2); //iso plus lumi 
      }

      std::cout<<"sample: "<<vCutBkg.at(i)->samplename<<" and total systematic: "<<pow(sys,0.5)<<std::endl;
      
      for(size_t j =0; j < (vCutBkg.at(i)->nEvents).size(); j++){
		if(j < (vCutBkg.at(i)->nEvents).size() -1) continue; //only get the last yields
		float errsys = sys*pow((vCutBkg.at(i)->nEvents).at(j),2);
		float err = pow((vCutBkg.at(i)->vErr).at(j),2) + errsys;
// 		if(! (vCutBkg.at(i)->samplename=="NonPrompt" || vCutBkg.at(i)->samplename=="ChargeMisID")){ //only add together if MC background //commented by rizki
		if(! (vCutBkg.at(i)->samplename=="NonPrompt" || vCutBkg.at(i)->samplename=="ChargeMisID" || vCutBkg.at(i)->samplename.find("prime")!=std::string::npos)){ //only add together if MC background //added edited by rizki
		  events_mctot = events_mctot + (vCutBkg.at(i)->nEvents).at(j);
		  errors_mctot = errors_mctot + err;

		  events_tot = events_tot + (vCutBkg.at(i)->nEvents).at(j);
		  errors_tot = errors_tot + err;
		}      
		else if(vCutBkg.at(i)->samplename!="NonPrompt" && vCutBkg.at(i)->samplename=="ChargeMisID"){ //only add together if MC background
		  events_tot = events_tot + (vCutBkg.at(i)->nEvents).at(j);
		  errors_tot = errors_tot + err;
		  events_cmtot = (vCutBkg.at(i)->nEvents).at(j);
		  errors_cmtot = err;	  
		}
		else if(vCutBkg.at(i)->samplename=="NonPrompt" && vCutBkg.at(i)->samplename!="ChargeMisID"){ //only add together if MC background
		  events_tot = events_tot + (vCutBkg.at(i)->nEvents).at(j);
		  errors_tot = errors_tot + err;
		  events_nptot = (vCutBkg.at(i)->nEvents).at(j);
		  errors_nptot = err;	  
		}

      }


      
    }

    //now signal
	float sys = pow(0.01,2)+ pow(0.02,2) + pow(0.01,2); //jes, jer, pu //added by rizki
	sys = sys + 2*pow(0.02,2) + pow(0.03,2) + pow(.025,2); //id iso and trigger  plus lumi  plus pileup plus //added by rizki

    float sig = 0; //added by rizki
    float errors_sig=0; ; //added by rizki
	float errstat_sig =0; //added by rizki		
    std::cout << "Printing Final Table: Processing signal ..." << std::endl;
    for(int i=0; i<cutSig_M1000.size();i++){ 
    	sig = sig+(cutSig_M1000.at(i)->nEvents).at( (cutSig_M1000.at(i)->nEvents).size()-1); //added by rizki
		errstat_sig = errstat_sig + pow((cutSig_M1000.at(i)->vErr).at( (cutSig_M1000.at(i)->nEvents).size()-1),2); //added by rizki		
    	std::cout << "	" << cutSig_M1000.at(i)->samplename << " : "<< (cutSig_M1000.at(i)->nEvents).at( (cutSig_M1000.at(i)->nEvents).size()-1) << " +- " << (cutSig_M1000.at(i)->vErr).at( (cutSig_M1000.at(i)->nEvents).size()-1) << "(stat)" << std::endl;
    }
	float errsys_sig = sys*pow(sig,2); //added by rizki
	errors_sig = errsys_sig + errstat_sig;	 //added by rizki

    float sig2 = 0; //added by rizki
    float errors_sig2=0; ; //added by rizki
	float errstat_sig2 =0; //added by rizki		
    for(int i=0; i<cutSig_M1200.size();i++){ 
    	sig2 = sig2+(cutSig_M1200.at(i)->nEvents).at( (cutSig_M1200.at(i)->nEvents).size()-1); //added by rizki
		errstat_sig2 = errstat_sig2 + pow((cutSig_M1200.at(i)->vErr).at( (cutSig_M1200.at(i)->nEvents).size()-1),2); //added by rizki		
    	std::cout << "	" << cutSig_M1200.at(i)->samplename << " : "<< (cutSig_M1200.at(i)->nEvents).at( (cutSig_M1200.at(i)->nEvents).size()-1) << " +- " << (cutSig_M1200.at(i)->vErr).at( (cutSig_M1200.at(i)->nEvents).size()-1) << "(stat)" << std::endl;
    }
	float errsys_sig2 = sys*pow(sig2,2); //added by rizki
	errors_sig2 = errsys_sig2 + errstat_sig2;	 //added by rizki
    
    //ok now I have all background, so get observed with no errors
   float obs =  (cutData->nEvents).at( (cutData->nEvents).size()-1);

   //write channel
    //now write table line -- BLIND FOR NOW
   //tablestring<<"$"<<events_mctot<<"\\pm"<<pow(errors_mctot,0.5)<<"$&$"<<events_nptot<<"\\pm"<<pow(errors_nptot,0.5)<<"$&$"<<events_cmtot<<"\\pm"<<pow(errors_cmtot,0.5)<<"$ &$ "<<events_tot<<"\\pm"<<pow(errors_tot,0.5)<<"$&"<<sig<<"\\pm"<<pow(errors_sig,0.5)<<" & "<<obs<< " & " << sig / sqrt(sig+events_tot) <<"\\\\\n";  //added by rizki
   
   if(nmu==0){
	  sig_ee=sig;
	  sigErr_ee=pow(errors_sig,0.5);
	  sig2_ee=sig2;
	  sig2Err_ee=pow(errors_sig2,0.5);
	  mcbkg_ee=events_mctot;
	  mcbkgErr_ee=pow(errors_mctot,0.5);
	  np_ee=events_nptot;
	  npErr_ee=pow(errors_nptot,0.5);
	  chmid_ee=events_cmtot;
	  chmidErr_ee=pow(errors_cmtot,0.5);
	  totalbkg_ee=events_tot;
	  totalbkgErr_ee=pow(errors_tot,0.5);
	  data_ee=obs;
	}
   if(nmu==1){
	  sig_em=sig;
	  sigErr_em=pow(errors_sig,0.5);
	  sig2_em=sig2;
	  sig2Err_em=pow(errors_sig2,0.5);
	  mcbkg_em=events_mctot;
	  mcbkgErr_em=pow(errors_mctot,0.5);
	  np_em=events_nptot;
	  npErr_em=pow(errors_nptot,0.5);
	  chmid_em=events_cmtot;
	  chmidErr_em=pow(errors_sig,0.5);
	  totalbkg_em=events_tot;
	  totalbkgErr_em=pow(errors_tot,0.5);
	  data_em=obs;
	}
   if(nmu==2){
	  sig_mm=sig;
	  sigErr_mm=pow(errors_sig,0.5);
	  sig2_mm=sig2;
	  sig2Err_mm=pow(errors_sig2,0.5);
	  mcbkg_mm=events_mctot;
	  mcbkgErr_mm=pow(errors_mctot,0.5);
	  np_mm=events_nptot;
	  npErr_mm=pow(errors_nptot,0.5);
	  chmid_mm=events_cmtot;
	  chmidErr_mm=pow(errors_sig,0.5);
	  totalbkg_mm=events_tot;
	  totalbkgErr_mm=pow(errors_tot,0.5);
	  data_mm=obs;   		
   }
  }//end loop over channels
  
  tablestring<<"T$\\bar{T}$ (1.0) &$"<<sig_ee<<"\\pm"<<sigErr_ee<<"$ & $"<<sig_em<<"\\pm"<<sigErr_em<<"$ & $"<<sig_mm<<"\\pm"<<sigErr_mm<<"$ \\\\\n";  //added by rizki  
  tablestring<<"T$\\bar{T}$ (1.2) &$"<<sig2_ee<<"\\pm"<<sig2Err_ee<<"$ & $"<<sig2_em<<"\\pm"<<sig2Err_em<<"$ & $"<<sig2_mm<<"\\pm"<<sig2Err_mm<<"$  \\\\ \\hline\n";  //added by rizki  
  tablestring<<"MC &$"<<mcbkg_ee<<"\\pm"<<mcbkgErr_ee<<"$ & $"<<mcbkg_em<<"\\pm"<<mcbkgErr_em<<"$ & $"<<mcbkg_mm<<"\\pm"<<mcbkgErr_mm<<"$ \\\\\n";  //added by rizki
  tablestring<<"Non-prompt &$"<<np_ee<<"\\pm"<<npErr_ee<<"$ & $"<<np_em<<"\\pm"<<npErr_em<<"$ & $"<<np_mm<<"\\pm"<<npErr_mm<<"$ \\\\\n";  //added by rizki
  tablestring<<"Chargemis-ID &$"<<chmid_ee<<"\\pm"<<chmidErr_ee<<"$ & $"<<chmid_em<<"\\pm"<<chmidErr_em<<"$ & $"<<chmid_mm<<"\\pm"<<chmidErr_mm<<"$ \\\\ \\hline\n";  //added by rizki
  tablestring<<"Total Bkg &$"<<totalbkg_ee<<"\\pm"<<totalbkgErr_ee<<"$ & $"<<totalbkg_em<<"\\pm"<<totalbkgErr_em<<"$ & $"<<totalbkg_mm<<"\\pm"<<totalbkgErr_mm<<"$ \\\\\n";  //added by rizki
  tablestring<<"Data &$"<<data_ee<<"$ & $"<<data_em<<"$ & $"<<data_mm<<"$ \\\\ \\hline\n";  //added by rizki
  tablestring<<"Data/Bkg &$"<<data_ee/totalbkg_ee<<"\\pm"<<data_ee/totalbkg_ee*pow( (1/data_ee) + pow(totalbkgErr_ee/totalbkg_ee,2),0.5)<<"$ & $"<<data_em/totalbkg_em<<"\\pm"<<data_em/totalbkg_em*pow((1/data_em)+pow(totalbkgErr_em/totalbkg_em,2),0.5)<<"$ & $"<<data_mm/totalbkg_mm<<"\\pm"<<data_mm/totalbkg_mm*pow((1/data_mm)+pow(totalbkgErr_mm/totalbkg_mm,2),0.5)<<"$ \\\\\n";  //added by rizki

  tablestring<<"\\hline \n\\end{tabular} \n"<<label<<'\n'<<"\\end{table} \n\n";

  return tablestring;


    
}

//added by rizki
std::stringstream& printFinalTable_v3(std::stringstream& tablestring,std::vector<Sample*> vBkgBF, std::vector<Sample*> vSigBF, Sample* dataSample, std::vector<std::string> vCutString,std::string era){

  std::cout << "--------> printing Final Table ("  << era <<  ")" << std::endl; //added by rizki

  //tablestring<<"\\begin{table}\n\\centering\n\\scriptsize\n\\begin{tabular}{|c|cccc|c|c|}\\hline\\hline\n"; //commented by rizki
  tablestring<<"\\begin{table}\n\\centering\n\\scriptsize\n\\begin{tabular}{|c|cccc|c|c|c|}\\hline\\hline\n"; //added by rizki
  //caption
  std::string caption;
  caption = "Summary of expected and observed events for all channels for "+era+". We show also the representative yields for an 1000 \\GeV \\xft. All systematic uncertainties are included.";
//   tablestring<<"PSS MC&NonPrompt&ChargeMisID& Total Background & 1000 \\GeV \\xft&Observed\\\\\n"; //commented by rizki
  tablestring<<"& PSS MC&NonPrompt&ChargeMisID& Total Background & 1000 GeV &Observed & $\\frac{S}{\\sqrt{S+B}}$\\\\\n"; // added by rizki
//   tablestring<<"& PSS MC&NonPrompt&ChargeMisID& Total Background & 1200 GeV &Observed & $\\frac{S}{\\sqrt{S+B}}$\\\\\n"; // added by rizki
  std::string label;
  //label
  label = "\\label{tab:FinalYields"+era+"}";


  for(int nmu=-1; nmu<3; nmu++){
	std::cout << "--------> channel # of mu: " << nmu << std::endl; //added by rizki

    //now make a vector of cutClass for bkg
    std::vector<CutClass*> vCutBkgBF = getCutClassVector(vBkgBF,vCutString,nmu);
    //now make a vector of cutClass for sig
    std::vector<CutClass*> vCutSigBF = getCutClassVector(vSigBF,vCutString,nmu);
    CutClass* cutSig = 0;
    std::vector<CutClass*> cutSig_M1000; //added by rizki


    std::vector<CutClass*> vCutBkg;
    vCutBkg = vCutBkgBF;
    std::vector<CutClass*> vCutSig;
    vCutSig = vCutSigBF;


    for(unsigned int j=0; j<vCutSig.size();j++){
//       if(vCutSig.at(j)->samplename.find("1000")!=std::string::npos && vCutSig.at(j)->samplename.find("RH")!=std::string::npos) cutSig = vCutSig.at(j);
      if(vCutSig.at(j)->samplename.find("1000")!=std::string::npos) cutSig_M1000.push_back(vCutSig.at(j)); //edited by rizki
//       if(vCutSig.at(j)->samplename.find("1200")!=std::string::npos) cutSig_M1000.push_back(vCutSig.at(j)); //edited by rizki
    }


    //now get cutcalss fordata
    CutClass* cutData = makeCutClass(dataSample, vCutString, nmu);


    //first machinery to get events in nice format
    float events_mctot=0;
    float errors_mctot=0;
    float events_nptot=0;
    float errors_nptot=0;
    float events_cmtot=0;
    float errors_cmtot=0;
    float events_tot = 0;
    float errors_tot = 0;

    //get background events
    std::cout << "Printing Final Table: Processing bkg ..." << std::endl; //added by rizki
    for(size_t i=0; i < vCutBkg.size(); i++){
      //std::cout << "	" << vCutBkg.at(i)->samplename << std::endl; //added by rizki
      float sys=0;

      if(vCutBkg.at(i)->samplename=="NonPrompt") sys= pow(unc_NP, 2);
      else if(vCutBkg.at(i)->samplename=="ChargeMisID") sys= pow(unc_CMID, 2);
      
      else if(vCutBkg.at(i)->samplename=="TTW") sys= pow(uncPDF_TTW, 2) + pow(uncSCALE_TTW, 2) + pow(uncJES_TTW,2) + pow(uncJER_TTW,2) + pow(uncPU_TTW,2); //pdf,scale,jes,jer,pu
      else if(vCutBkg.at(i)->samplename=="TTZ") sys= pow(uncPDF_TTZ, 2) + pow(uncSCALE_TTZ, 2) + pow(uncJES_TTZ,2) + pow(uncJER_TTZ,2) + pow(uncPU_TTZ,2);
      else if(vCutBkg.at(i)->samplename=="TTH") sys= pow(uncPDF_TTH, 2) + pow(uncSCALE_TTH, 2) + pow(uncJES_TTH,2) + pow(uncJER_TTH,2) + pow(uncPU_TTH,2);
      else if(vCutBkg.at(i)->samplename=="TTTT") sys= pow(uncPDF_TTTT, 2) + pow(uncSCALE_TTTT, 2) + pow(uncJES_TTTT,2) + pow(uncJER_TTTT,2) + pow(uncPU_TTTT,2);
      else if(vCutBkg.at(i)->samplename=="WZ") sys= pow(uncPDF_WZ, 2) + pow(uncSCALE_WZ, 2) + pow(uncJES_WZ,2) + pow(uncJER_WZ,2) + pow(uncPU_WZ,2);
      else if(vCutBkg.at(i)->samplename=="ZZ") sys= pow(uncPDF_ZZ, 2) + pow(uncSCALE_ZZ, 2) + pow(uncJES_ZZ,2) + pow(uncJER_ZZ,2) + pow(uncPU_ZZ,2);
      else if(vCutBkg.at(i)->samplename=="WpWp") sys= pow(uncPDF_WpWp, 2) + pow(uncSCALE_WpWp, 2) + pow(uncJES_WpWp,2) + pow(uncJER_WpWp,2) + pow(uncPU_WpWp,2);
      else if(vCutBkg.at(i)->samplename=="WWZ") sys= pow(uncPDF_WWZ, 2) + pow(uncSCALE_WWZ, 2) + pow(uncJES_WWZ,2) + pow(uncJER_WWZ,2) + pow(uncPU_WWZ,2);
      else if(vCutBkg.at(i)->samplename=="WZZ") sys= pow(uncPDF_WZZ, 2) + pow(uncSCALE_WZZ, 2) + pow(uncJES_WZZ,2) + pow(uncJER_WZZ,2) + pow(uncPU_WZZ,2);
      else if(vCutBkg.at(i)->samplename=="ZZZ") sys= pow(uncPDF_ZZZ, 2) + pow(uncSCALE_ZZZ, 2) + pow(uncJES_ZZZ,2) + pow(uncJER_ZZZ,2) + pow(uncPU_ZZZ,2);

//       else if(vCutBkg.at(i)->samplename.find("X53X53")!=std::string::npos) sys = pow(0.03,2) + pow(0.01,2) + pow(0.01,2); //jes, jer, pu
      else if(vCutSig.at(i)->samplename.find("prime")!=std::string::npos) sys =  pow(uncPDF_sig, 2) + pow(uncSCALE_sig, 2) + pow(uncJES_sig,2)+ pow(uncJER_sig,2) + pow(uncPU_sig,2); //jes, jer, pu
      //else sys = pow(0.5,2);

      if(vCutBkg.at(i)->samplename=="TTZ") sys= sys + pow(uncTrigSF_TTZ,2) + pow(uncIDSF_TTZ,2); // trig plus id
      else if(vCutBkg.at(i)->samplename=="TTW") sys= sys + pow(uncTrigSF_TTW,2) + pow(uncIDSF_TTW,2);
      else if(vCutBkg.at(i)->samplename=="TTH") sys= sys + pow(uncTrigSF_TTH,2) + pow(uncIDSF_TTH,2);
      else if(vCutBkg.at(i)->samplename=="TTTT") sys= sys + pow(uncTrigSF_TTTT,2) + pow(uncIDSF_TTTT,2);
      else if(vCutBkg.at(i)->samplename=="WZ") sys= sys + pow(uncTrigSF_WZ,2) + pow(uncIDSF_WZ,2);
      else if(vCutBkg.at(i)->samplename=="ZZ") sys= sys + pow(uncTrigSF_ZZ,2) + pow(uncIDSF_ZZ,2);
      else if(vCutBkg.at(i)->samplename=="WpWp") sys= sys + pow(uncTrigSF_WpWp,2) + pow(uncIDSF_WpWp,2);
      else if(vCutBkg.at(i)->samplename=="WWZ") sys= sys + pow(uncTrigSF_WWZ,2) + pow(uncIDSF_WWZ,2);
      else if(vCutBkg.at(i)->samplename=="WZZ") sys= sys + pow(uncTrigSF_WZZ,2) + pow(uncIDSF_WZZ,2);
      else if(vCutBkg.at(i)->samplename=="ZZZ") sys= sys + pow(uncTrigSF_ZZZ,2) + pow(uncIDSF_ZZZ,2);
      else if(vCutSig.at(i)->samplename.find("prime")!=std::string::npos) sys = sys + pow(uncTrigSF_sig,2) + pow(uncIDSF_sig,2);

      if(! (vCutBkg.at(i)->samplename=="NonPrompt" || vCutBkg.at(i)->samplename=="ChargeMisID")){
        sys = sys + pow(uncIsoSF,2) + pow(uncLumi,2); //iso plus lumi 
      }

      std::cout<<"sample: "<<vCutBkg.at(i)->samplename<<" and total systematic: "<<pow(sys,0.5)<<std::endl;
      
      for(size_t j =0; j < (vCutBkg.at(i)->nEvents).size(); j++){
		if(j < (vCutBkg.at(i)->nEvents).size() -1) continue; //only get the last yields
		float errsys = sys*pow((vCutBkg.at(i)->nEvents).at(j),2);
		float err = pow((vCutBkg.at(i)->vErr).at(j),2) + errsys;
// 		if(! (vCutBkg.at(i)->samplename=="NonPrompt" || vCutBkg.at(i)->samplename=="ChargeMisID")){ //only add together if MC background //commented by rizki
		if(! (vCutBkg.at(i)->samplename=="NonPrompt" || vCutBkg.at(i)->samplename=="ChargeMisID" || vCutBkg.at(i)->samplename.find("prime")!=std::string::npos)){ //only add together if MC background //added edited by rizki
		  events_mctot = events_mctot + (vCutBkg.at(i)->nEvents).at(j);
		  errors_mctot = errors_mctot + err;

		  events_tot = events_tot + (vCutBkg.at(i)->nEvents).at(j);
		  errors_tot = errors_tot + err;
		}      
		else if(vCutBkg.at(i)->samplename!="NonPrompt" && vCutBkg.at(i)->samplename=="ChargeMisID"){ //only add together if MC background
		  events_tot = events_tot + (vCutBkg.at(i)->nEvents).at(j);
		  errors_tot = errors_tot + err;
		  events_cmtot += (vCutBkg.at(i)->nEvents).at(j);
		  errors_cmtot += err;	  
		}
		else if(vCutBkg.at(i)->samplename=="NonPrompt" && vCutBkg.at(i)->samplename!="ChargeMisID"){ //only add together if MC background
		  events_tot = events_tot + (vCutBkg.at(i)->nEvents).at(j);
		  errors_tot = errors_tot + err;
		  events_nptot += (vCutBkg.at(i)->nEvents).at(j);
		  errors_nptot += err;	  
		}

      }


      
    }

    //now signal
//     float sig = (cutSig->nEvents).at( (cutSig->nEvents).size()-1); //commented y rizki
    float sig = 0; //added by rizki
    float errors_sig=0; ; //added by rizki
	float errstat =0; //added by rizki		
    std::cout << "Printing Final Table: Processing signal ..." << std::endl;
    for(int i=0; i<cutSig_M1000.size();i++){ 
    	sig = sig+(cutSig_M1000.at(i)->nEvents).at( (cutSig_M1000.at(i)->nEvents).size()-1); //added by rizki
		errstat = errstat + pow((cutSig_M1000.at(i)->vErr).at( (cutSig_M1000.at(i)->nEvents).size()-1),2); //added by rizki		
    	std::cout << "	" << cutSig_M1000.at(i)->samplename << " : "<< (cutSig_M1000.at(i)->nEvents).at( (cutSig_M1000.at(i)->nEvents).size()-1) << " +- " << (cutSig_M1000.at(i)->vErr).at( (cutSig_M1000.at(i)->nEvents).size()-1) << "(stat)" << std::endl;

    }
	float sys = pow(0.01,2)+ pow(0.02,2) + pow(0.01,2); //jes, jer, pu //added by rizki
	sys = sys + 2*pow(0.02,2) + pow(0.03,2) + pow(.025,2); //id iso and trigger  plus lumi  plus pileup plus //added by rizki
	float errsys = sys*pow(sig,2); //added by rizki
	errors_sig = errsys + errstat;	 //added by rizki
    
    //ok now I have all background, so get observed with no errors
   float obs =  (cutData->nEvents).at( (cutData->nEvents).size()-1);

   //write channel
   if(nmu==0) tablestring<<"Di-electron&";
   else if(nmu==1) tablestring<<"Electron-Muon&";
   else if(nmu==2) tablestring<<"Di-muon&";
   else tablestring<<"All&";
    //now write table line -- BLIND FOR NOW
   tablestring<<"$"<<events_mctot<<"\\pm"<<pow(errors_mctot,0.5)<<"$ & $"<<events_nptot<<"\\pm"<<pow(errors_nptot,0.5)<<"$ & $"<<events_cmtot<<"\\pm"<<pow(errors_cmtot,0.5)<<"$ & $ "<<events_tot<<"\\pm"<<pow(errors_tot,0.5)<<"$ & $"<<sig<<"\\pm"<<pow(errors_sig,0.5)<<"$ & "<<obs<< " & " << sig / sqrt(sig+events_tot) <<"\\\\\n";  //added by rizki
   //tablestring<<"$"<<events_mctot<<"\\pm"<<pow(errors_mctot,0.5)<<"$&$"<<events_nptot<<"\\pm"<<pow(errors_nptot,0.5)<<"$&$"<<events_cmtot<<"\\pm"<<pow(errors_cmtot,0.5)<<"$ &$ "<<events_tot<<"\\pm"<<pow(errors_tot,0.5)<<"$&"<<sig<<" & "<<obs<<"\\\\\n"; //commented by rizki 
   //tablestring<<"$"<<events_mctot<<"\\pm"<<pow(errors_mctot,0.5)<<"$&$"<<events_nptot<<"\\pm"<<pow(errors_nptot,0.5)<<"$&$"<<events_cmtot<<"\\pm"<<pow(errors_cmtot,0.5)<<"$ &$ "<<events_tot<<"\\pm"<<pow(errors_tot,0.5)<<"$&"<<sig<<" & XXX\\\\\n"; 
  }//end loop over channels

  tablestring<<"\\hline \n\\end{tabular} \n"<<label<<'\n'<<"\\end{table} \n\n";

  return tablestring;


    
}

//added by rizki
std::stringstream& printFinalTable_v4(std::stringstream& tablestring,std::vector<Sample*> vBkgBF, std::vector<Sample*> vSigBF,Sample* dataSample, std::vector<std::string> vCutString,std::string era){

  std::cout << "--------> printing Final Table ("  << era <<  ")" << std::endl; //added by rizki

  //tablestring<<"\\begin{table}\n\\centering\n\\scriptsize\n\\begin{tabular}{|c|cccc|c|c|}\\hline\\hline\n"; //commented by rizki
  tablestring<<"\\begin{table}\n\\centering\n\\scriptsize\n\\begin{tabular}{|c|c|c|c|}\\hline\\hline\n"; //added by rizki
  //caption
  std::string caption;
  caption = "Summary of expected and observed events for all channels for "+era+". We show also the representative yields for an 1000 \\GeV \\xft. All systematic uncertainties are included.";
//   tablestring<<"PSS MC&NonPrompt&ChargeMisID& Total Background & 1000 \\GeV \\xft&Observed\\\\\n"; //commented by rizki
  tablestring<<"Sample & $ee$ & $e\\mu$ & $\\mu\\mu$ \\\\ \\hline \n"; // added by rizki
  std::string label;
  //label
  label = "\\label{tab:FinalYields"+era+"}";
  
  
  //added by rizki - start
  float sig_ee;
  float sigErr_ee;
  float sig2_ee;
  float sig2Err_ee;
  float mcbkg_ee;
  float mcbkgErr_ee;
  float np_ee;
  float npErr_ee;
  float chmid_ee;
  float chmidErr_ee;
  float totalbkg_ee;
  float totalbkgErr_ee;
  float data_ee;

  float sig_em;
  float sigErr_em;
  float sig2_em;
  float sig2Err_em;
  float mcbkg_em;
  float mcbkgErr_em;
  float np_em;
  float npErr_em;
  float chmid_em;
  float chmidErr_em;
  float totalbkg_em;
  float totalbkgErr_em;
  float data_em;

  float sig_mm;
  float sigErr_mm;
  float sig2_mm;
  float sig2Err_mm;
  float mcbkg_mm;
  float mcbkgErr_mm;
  float np_mm;
  float npErr_mm;
  float chmid_mm;
  float chmidErr_mm;
  float totalbkg_mm;
  float totalbkgErr_mm;
  float data_mm;
  //added by rizki - end


  for(int nmu=-1; nmu<3; nmu++){
	std::cout << "--------> channel # of mu: " << nmu << std::endl; //added by rizki

    //now make a vector of cutClass for bkg
    std::vector<CutClass*> vCutBkgBF = getCutClassVector(vBkgBF,vCutString,nmu);
    //now make a vector of cutClass for sig
    std::vector<CutClass*> vCutSigBF = getCutClassVector(vSigBF,vCutString,nmu);
    CutClass* cutSig = 0;
    std::vector<CutClass*> cutSig_M1000; //added by rizki
    std::vector<CutClass*> cutSig_M1200; //added by rizki


    std::vector<CutClass*> vCutBkg;
    vCutBkg = vCutBkgBF;
    std::vector<CutClass*> vCutSig;
    vCutSig = vCutSigBF;

    for(unsigned int j=0; j<vCutSig.size();j++){
      if(vCutSig.at(j)->samplename.find("1000")!=std::string::npos) cutSig_M1000.push_back(vCutSig.at(j)); //edited by rizki
      if(vCutSig.at(j)->samplename.find("1200")!=std::string::npos) cutSig_M1200.push_back(vCutSig.at(j)); //edited by rizki
    }


    //now get cutcalss fordata
    CutClass* cutData = makeCutClass(dataSample, vCutString, nmu);


    //first machinery to get events in nice format
    float events_mctot=0;
    float errors_mctot=0;
    float events_nptot=0;
    float errors_nptot=0;
    float events_cmtot=0;
    float errors_cmtot=0;
    float events_tot = 0;
    float errors_tot = 0;

    //get background events
    std::cout << "Printing Final Table: Processing bkg ..." << std::endl; //added by rizki
    for(size_t i=0; i < vCutBkg.size(); i++){
      //std::cout << "	" << vCutBkg.at(i)->samplename << std::endl; //added by rizki
      float sys=0;

      if(vCutBkg.at(i)->samplename=="NonPrompt") sys= pow(unc_NP, 2);
      else if(vCutBkg.at(i)->samplename=="ChargeMisID") sys= pow(unc_CMID, 2);
      
      else if(vCutBkg.at(i)->samplename=="TTW") sys= pow(uncPDF_TTW, 2) + pow(uncSCALE_TTW, 2) + pow(uncJES_TTW,2) + pow(uncJER_TTW,2) + pow(uncPU_TTW,2); //pdf,scale,jes,jer,pu
      else if(vCutBkg.at(i)->samplename=="TTZ") sys= pow(uncPDF_TTZ, 2) + pow(uncSCALE_TTZ, 2) + pow(uncJES_TTZ,2) + pow(uncJER_TTZ,2) + pow(uncPU_TTZ,2);
      else if(vCutBkg.at(i)->samplename=="TTH") sys= pow(uncPDF_TTH, 2) + pow(uncSCALE_TTH, 2) + pow(uncJES_TTH,2) + pow(uncJER_TTH,2) + pow(uncPU_TTH,2);
      else if(vCutBkg.at(i)->samplename=="TTTT") sys= pow(uncPDF_TTTT, 2) + pow(uncSCALE_TTTT, 2) + pow(uncJES_TTTT,2) + pow(uncJER_TTTT,2) + pow(uncPU_TTTT,2);
      else if(vCutBkg.at(i)->samplename=="WZ") sys= pow(uncPDF_WZ, 2) + pow(uncSCALE_WZ, 2) + pow(uncJES_WZ,2) + pow(uncJER_WZ,2) + pow(uncPU_WZ,2);
      else if(vCutBkg.at(i)->samplename=="ZZ") sys= pow(uncPDF_ZZ, 2) + pow(uncSCALE_ZZ, 2) + pow(uncJES_ZZ,2) + pow(uncJER_ZZ,2) + pow(uncPU_ZZ,2);
      else if(vCutBkg.at(i)->samplename=="WpWp") sys= pow(uncPDF_WpWp, 2) + pow(uncSCALE_WpWp, 2) + pow(uncJES_WpWp,2) + pow(uncJER_WpWp,2) + pow(uncPU_WpWp,2);
      else if(vCutBkg.at(i)->samplename=="WWZ") sys= pow(uncPDF_WWZ, 2) + pow(uncSCALE_WWZ, 2) + pow(uncJES_WWZ,2) + pow(uncJER_WWZ,2) + pow(uncPU_WWZ,2);
      else if(vCutBkg.at(i)->samplename=="WZZ") sys= pow(uncPDF_WZZ, 2) + pow(uncSCALE_WZZ, 2) + pow(uncJES_WZZ,2) + pow(uncJER_WZZ,2) + pow(uncPU_WZZ,2);
      else if(vCutBkg.at(i)->samplename=="ZZZ") sys= pow(uncPDF_ZZZ, 2) + pow(uncSCALE_ZZZ, 2) + pow(uncJES_ZZZ,2) + pow(uncJER_ZZZ,2) + pow(uncPU_ZZZ,2);

//       else if(vCutBkg.at(i)->samplename.find("X53X53")!=std::string::npos) sys = pow(0.03,2) + pow(0.01,2) + pow(0.01,2); //jes, jer, pu
      else if(vCutSig.at(i)->samplename.find("prime")!=std::string::npos) sys = pow(uncPDF_sig, 2) + pow(uncSCALE_sig, 2) + pow(uncJES_sig,2)+ pow(uncJER_sig,2) + pow(uncPU_sig,2) ; //jes, jer, pu
      else sys = pow(0.5,2);

      if(vCutBkg.at(i)->samplename=="TTZ") sys= sys + pow(uncTrigSF_TTZ,2) + pow(uncIDSF_TTZ,2); // trig plus id
      else if(vCutBkg.at(i)->samplename=="TTW") sys= sys + pow(uncTrigSF_TTW,2) + pow(uncIDSF_TTW,2);
      else if(vCutBkg.at(i)->samplename=="TTH") sys= sys + pow(uncTrigSF_TTH,2) + pow(uncIDSF_TTH,2);
      else if(vCutBkg.at(i)->samplename=="TTTT") sys= sys + pow(uncTrigSF_TTTT,2) + pow(uncIDSF_TTTT,2);
      else if(vCutBkg.at(i)->samplename=="WZ") sys= sys + pow(uncTrigSF_WZ,2) + pow(uncIDSF_WZ,2);
      else if(vCutBkg.at(i)->samplename=="ZZ") sys= sys + pow(uncTrigSF_ZZ,2) + pow(uncIDSF_ZZ,2);
      else if(vCutBkg.at(i)->samplename=="WpWp") sys= sys + pow(uncTrigSF_WpWp,2) + pow(uncIDSF_WpWp,2);
      else if(vCutBkg.at(i)->samplename=="WWZ") sys= sys + pow(uncTrigSF_WWZ,2) + pow(uncIDSF_WWZ,2);
      else if(vCutBkg.at(i)->samplename=="WZZ") sys= sys + pow(uncTrigSF_WZZ,2) + pow(uncIDSF_WZZ,2);
      else if(vCutBkg.at(i)->samplename=="ZZZ") sys= sys + pow(uncTrigSF_ZZZ,2) + pow(uncIDSF_ZZZ,2);
      else if(vCutBkg.at(i)->samplename.find("prime")!=std::string::npos) sys = sys + pow(uncTrigSF_sig,2) + pow(uncIDSF_sig,2);

      if(! (vCutBkg.at(i)->samplename=="NonPrompt" || vCutBkg.at(i)->samplename=="ChargeMisID")){
        sys = sys + pow(uncIsoSF,2) + pow(uncLumi,2); //iso plus lumi 
      }
      std::cout<<"sample: "<<vCutBkg.at(i)->samplename<<" and total systematic: "<<pow(sys,0.5)<<std::endl;
      
      for(size_t j =0; j < (vCutBkg.at(i)->nEvents).size(); j++){
		if(j < (vCutBkg.at(i)->nEvents).size() -1) continue; //only get the last yields
		float errsys = sys*pow((vCutBkg.at(i)->nEvents).at(j),2);
		float err = pow((vCutBkg.at(i)->vErr).at(j),2) + errsys;
// 		if(! (vCutBkg.at(i)->samplename=="NonPrompt" || vCutBkg.at(i)->samplename=="ChargeMisID")){ //only add together if MC background //commented by rizki
		if(! (vCutBkg.at(i)->samplename=="NonPrompt" || vCutBkg.at(i)->samplename=="ChargeMisID" || vCutBkg.at(i)->samplename.find("prime")!=std::string::npos)){ //only add together if MC background //added edited by rizki
		  events_mctot = events_mctot + (vCutBkg.at(i)->nEvents).at(j);
		  errors_mctot = errors_mctot + err;

		  events_tot = events_tot + (vCutBkg.at(i)->nEvents).at(j);
		  errors_tot = errors_tot + err;
		}      
		else if(vCutBkg.at(i)->samplename!="NonPrompt" && vCutBkg.at(i)->samplename=="ChargeMisID"){ //only add together if MC background
		  events_tot = events_tot + (vCutBkg.at(i)->nEvents).at(j);
		  errors_tot = errors_tot + err;
		  events_cmtot += (vCutBkg.at(i)->nEvents).at(j);
		  errors_cmtot += err;	  
		}
		else if(vCutBkg.at(i)->samplename=="NonPrompt" && vCutBkg.at(i)->samplename!="ChargeMisID"){ //only add together if MC background
		  events_tot = events_tot + (vCutBkg.at(i)->nEvents).at(j);
		  errors_tot = errors_tot + err;
		  events_nptot += (vCutBkg.at(i)->nEvents).at(j);
		  errors_nptot += err;	  
		}

      }


      
    }

    //now signal
	float sys = pow(0.01,2)+ pow(0.02,2) + pow(0.01,2); //jes, jer, pu //added by rizki
	sys = sys + 2*pow(0.02,2) + pow(0.03,2) + pow(.025,2); //id iso and trigger  plus lumi  plus pileup plus //added by rizki

    float sig = 0; //added by rizki
    float errors_sig=0; ; //added by rizki
	float errstat_sig =0; //added by rizki		
    std::cout << "Printing Final Table: Processing signal ..." << std::endl;
    for(int i=0; i<cutSig_M1000.size();i++){ 
    	sig = sig+(cutSig_M1000.at(i)->nEvents).at( (cutSig_M1000.at(i)->nEvents).size()-1); //added by rizki
		errstat_sig = errstat_sig + pow((cutSig_M1000.at(i)->vErr).at( (cutSig_M1000.at(i)->nEvents).size()-1),2); //added by rizki		
    	std::cout << "	" << cutSig_M1000.at(i)->samplename << " : "<< (cutSig_M1000.at(i)->nEvents).at( (cutSig_M1000.at(i)->nEvents).size()-1) << " +- " << (cutSig_M1000.at(i)->vErr).at( (cutSig_M1000.at(i)->nEvents).size()-1) << "(stat)" << std::endl;
    }
	float errsys_sig = sys*pow(sig,2); //added by rizki
	errors_sig = errsys_sig + errstat_sig;	 //added by rizki

    float sig2 = 0; //added by rizki
    float errors_sig2=0; ; //added by rizki
	float errstat_sig2 =0; //added by rizki		
    for(int i=0; i<cutSig_M1200.size();i++){ 
    	sig2 = sig2+(cutSig_M1200.at(i)->nEvents).at( (cutSig_M1200.at(i)->nEvents).size()-1); //added by rizki
		errstat_sig2 = errstat_sig2 + pow((cutSig_M1200.at(i)->vErr).at( (cutSig_M1200.at(i)->nEvents).size()-1),2); //added by rizki		
    	std::cout << "	" << cutSig_M1200.at(i)->samplename << " : "<< (cutSig_M1200.at(i)->nEvents).at( (cutSig_M1200.at(i)->nEvents).size()-1) << " +- " << (cutSig_M1200.at(i)->vErr).at( (cutSig_M1200.at(i)->nEvents).size()-1) << "(stat)" << std::endl;
    }
	float errsys_sig2 = sys*pow(sig2,2); //added by rizki
	errors_sig2 = errsys_sig2 + errstat_sig2;	 //added by rizki
    
    //ok now I have all background, so get observed with no errors
   float obs =  (cutData->nEvents).at( (cutData->nEvents).size()-1);

   //write channel
    //now write table line -- BLIND FOR NOW
   //tablestring<<"$"<<events_mctot<<"\\pm"<<pow(errors_mctot,0.5)<<"$&$"<<events_nptot<<"\\pm"<<pow(errors_nptot,0.5)<<"$&$"<<events_cmtot<<"\\pm"<<pow(errors_cmtot,0.5)<<"$ &$ "<<events_tot<<"\\pm"<<pow(errors_tot,0.5)<<"$&"<<sig<<"\\pm"<<pow(errors_sig,0.5)<<" & "<<obs<< " & " << sig / sqrt(sig+events_tot) <<"\\\\\n";  //added by rizki
   
   if(nmu==0){
	  sig_ee=sig;
	  sigErr_ee=pow(errors_sig,0.5);
	  sig2_ee=sig2;
	  sig2Err_ee=pow(errors_sig2,0.5);
	  mcbkg_ee=events_mctot;
	  mcbkgErr_ee=pow(errors_mctot,0.5);
	  np_ee=events_nptot;
	  npErr_ee=pow(errors_nptot,0.5);
	  chmid_ee=events_cmtot;
	  chmidErr_ee=pow(errors_cmtot,0.5);
	  totalbkg_ee=events_tot;
	  totalbkgErr_ee=pow(errors_tot,0.5);
	  data_ee=obs;
	}
   if(nmu==1){
	  sig_em=sig;
	  sigErr_em=pow(errors_sig,0.5);
	  sig2_em=sig2;
	  sig2Err_em=pow(errors_sig2,0.5);
	  mcbkg_em=events_mctot;
	  mcbkgErr_em=pow(errors_mctot,0.5);
	  np_em=events_nptot;
	  npErr_em=pow(errors_nptot,0.5);
	  chmid_em=events_cmtot;
	  chmidErr_em=pow(errors_sig,0.5);
	  totalbkg_em=events_tot;
	  totalbkgErr_em=pow(errors_tot,0.5);
	  data_em=obs;
	}
   if(nmu==2){
	  sig_mm=sig;
	  sigErr_mm=pow(errors_sig,0.5);
	  sig2_mm=sig2;
	  sig2Err_mm=pow(errors_sig2,0.5);
	  mcbkg_mm=events_mctot;
	  mcbkgErr_mm=pow(errors_mctot,0.5);
	  np_mm=events_nptot;
	  npErr_mm=pow(errors_nptot,0.5);
	  chmid_mm=events_cmtot;
	  chmidErr_mm=pow(errors_sig,0.5);
	  totalbkg_mm=events_tot;
	  totalbkgErr_mm=pow(errors_tot,0.5);
	  data_mm=obs;   		
   }
  }//end loop over channels
  
  tablestring<<"T$\\bar{T}$ (1.0) &$"<<sig_ee<<"\\pm"<<sigErr_ee<<"$ & $"<<sig_em<<"\\pm"<<sigErr_em<<"$ & $"<<sig_mm<<"\\pm"<<sigErr_mm<<"$ \\\\\n";  //added by rizki  
  tablestring<<"T$\\bar{T}$ (1.2) &$"<<sig2_ee<<"\\pm"<<sig2Err_ee<<"$ & $"<<sig2_em<<"\\pm"<<sig2Err_em<<"$ & $"<<sig2_mm<<"\\pm"<<sig2Err_mm<<"$  \\\\ \\hline\n";  //added by rizki  
  tablestring<<"MC &$"<<mcbkg_ee<<"\\pm"<<mcbkgErr_ee<<"$ & $"<<mcbkg_em<<"\\pm"<<mcbkgErr_em<<"$ & $"<<mcbkg_mm<<"\\pm"<<mcbkgErr_mm<<"$ \\\\\n";  //added by rizki
  tablestring<<"Non-prompt &$"<<np_ee<<"\\pm"<<npErr_ee<<"$ & $"<<np_em<<"\\pm"<<npErr_em<<"$ & $"<<np_mm<<"\\pm"<<npErr_mm<<"$ \\\\\n";  //added by rizki
  tablestring<<"Chargemis-ID &$"<<chmid_ee<<"\\pm"<<chmidErr_ee<<"$ & $"<<chmid_em<<"\\pm"<<chmidErr_em<<"$ & $"<<chmid_mm<<"\\pm"<<chmidErr_mm<<"$ \\\\ \\hline\n";  //added by rizki
  tablestring<<"Total Bkg &$"<<totalbkg_ee<<"\\pm"<<totalbkgErr_ee<<"$ & $"<<totalbkg_em<<"\\pm"<<totalbkgErr_em<<"$ & $"<<totalbkg_mm<<"\\pm"<<totalbkgErr_mm<<"$ \\\\\n";  //added by rizki
  tablestring<<"Data &$"<<data_ee<<"$ & $"<<data_em<<"$ & $"<<data_mm<<"$ \\\\ \\hline\n";  //added by rizki
  tablestring<<"Data/Bkg &$"<<data_ee/totalbkg_ee<<"\\pm"<<data_ee/totalbkg_ee*pow( (1/data_ee) + pow(totalbkgErr_ee/totalbkg_ee,2),0.5)<<"$ & $"<<data_em/totalbkg_em<<"\\pm"<<data_em/totalbkg_em*pow((1/data_em)+pow(totalbkgErr_em/totalbkg_em,2),0.5)<<"$ & $"<<data_mm/totalbkg_mm<<"\\pm"<<data_mm/totalbkg_mm*pow((1/data_mm)+pow(totalbkgErr_mm/totalbkg_mm,2),0.5)<<"$ \\\\\n";  //added by rizki

  tablestring<<"\\hline \n\\end{tabular} \n"<<label<<'\n'<<"\\end{table} \n\n";

  return tablestring;


    
}
