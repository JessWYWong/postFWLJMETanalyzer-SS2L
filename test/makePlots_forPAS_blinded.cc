#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include "THStack.h"
#include "TChain.h"
//#include "../plugins/Sample.cc"
#include "../plugins/SetTDRStyle.cc"
//#include "../plugins/Variable.cc"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TROOT.h"
#include "../plugins/Macros.cc"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TString.h"
#include "../interface/unc.h"
//eaxample usage:
//root -b -q -l 'makePlots_forPAS.cc("MVA2016TightRC","CBTightMiniIsoTight")'

void DrawAndSaveCombined(Variable* Var, std::vector<Sample*> vBkg, std::vector<Sample*> vSig, Sample* dataSample, TFile* outfile, std::string elID, std::string muID, int nMu=-1, int cutIndex=0,std::string weightstring="");
TH1F* getPullPlot(TH1F* hData, THStack * h, Variable* var, TH1F* h_err);
double poissonErrors(TGraphAsymmErrors* tgae_err, TH1F* h, double sys_err, int ibin);
//below sys were obtaind from Julie documented in B2G Oct31 presentation
/*
float uncLumi = 0.025;
float uncIsoSF = 0.015;

float uncPU_TTZ=0.0084;
float uncPU_TTW=0.0076;
float uncPU_TTH=0.0008;
float uncPU_TTTT=0.0017;
float uncPU_WZ=0.0425;
float uncPU_ZZ=0.402;
float uncPU_WpWp=0.0049;
float uncPU_WWZ=0.0508;
float uncPU_WZZ=0.131;
float uncPU_ZZZ=0.0861;

float uncTrigSF_TTZ=0.019;
float uncTrigSF_TTW=0.0193;
float uncTrigSF_TTH=0.0196;
float uncTrigSF_TTTT=0.0192;
float uncTrigSF_WZ=0.0189;
float uncTrigSF_ZZ=0.0206;
float uncTrigSF_WpWp=0.0192;
float uncTrigSF_WWZ=0.0181;
float uncTrigSF_WZZ=0.0173;
float uncTrigSF_ZZZ=0.0206;

float uncIDSF_TTZ=0.0362;
float uncIDSF_TTW=0.0397;
float uncIDSF_TTH=0.0337;
float uncIDSF_TTTT=0.0365;
float uncIDSF_WZ=0.041;
float uncIDSF_ZZ=0.0408;
float uncIDSF_WpWp=0.0412;
float uncIDSF_WWZ=0.0395;
float uncIDSF_WZZ=0.0392;
float uncIDSF_ZZZ=0.0508;


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
*/
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
/*
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

float lumi1 = 59.74;

//folder to save plots - rizki
// const std::string saveFolder = "plots_nConst4_HT1200_0NonSSLep_NonIsoTrig_Jan30-2019"; //CHECK Macros.cc for input folder!!
//const std::string saveFolder = "plots_nConst4_HT1200_0NonSSLep_IsoTrig_Jan30-2019"; //CHECK Macros.cc for input folder!!
const std::string saveFolder = "plots_IsoTrig_blinded_Nov-2020";

void makePlots_forPAS_blinded(std::string elID, std::string muID){

  //make output file
  TFile* fout = new TFile((saveFolder+".root").c_str(),"RECREATE");


  //set TDRStyle
  setTDRStyle();

  //make folder to save plots - rizki
  system(("mkdir -v "+saveFolder).c_str());

  //desired lumi:
  //Json lumi with normtag: BCDEF - https://twiki.cern.ch/twiki/bin/view/CMS/PdmV2017Analysis - Nov 11, 2018.
  //float lumi = 59.74; //fb^-1
  //float lumi2 = 0; //fb^-1
  std::string weightstring = "PUWeight * ChargeMisIDWeight * NPWeight * IDSF * IsoSF * trigSF * GsfSF * MCWeight/abs(MCWeight) *";

  std::vector<Variable*> vVariables = getVariableVec();

  std::vector<Sample*> vMCBkgSamples = getMCBkgSampleVec("sZVeto", lumi1, elID, muID,"2018A-D");
  std::cout << "DONE Preparing MC bkg samples" << std::endl;
  std::vector<Sample*> vDDBkgSamples = getDDBkgSampleVec("sZVeto", lumi1, elID, muID,"2018A-D");
  std::cout << "DONE Preparing DDBkg samples" << std::endl;
  std::vector<Sample*> vBkgSamples = appendSampleVectors(vMCBkgSamples, vDDBkgSamples);
  std::cout << "DONE appending Bkg samples" << std::endl;

//   std::vector<Sample*> vSigSamples1 = getInclusiveSigSampleVecForTable("sZVeto", lumi1, elID, muID,"2016B-D");
//   std::vector<Sample*> vSigSamples2 = getInclusiveSigSampleVecForTable("sZVeto", lumi2, elID, muID,"2016E-H");

  //TT SIGNAL <decay> - RIZKI
  std::vector<Sample*> vSigSamples1 = getInclusiveSigTTSampleVecForTable("sZVeto", lumi1, elID, muID,"2018A-D","BWBW",0);
  std::cout << "DONE Preparing signal BWBW samples" << std::endl;

  std::vector<Sample*> vSigSamples2 = getInclusiveSigTTSampleVecForTable("sZVeto", lumi1, elID, muID,"2018A-D","THBW",0);
  std::cout << "DONE Preparing signal THBW samples" << std::endl;

  std::vector<Sample*> vSigSamples3 = getInclusiveSigTTSampleVecForTable("sZVeto", lumi1, elID, muID,"2018A-D","THTH",0);
  std::cout << "DONE Preparing signal THTH samples" << std::endl;

  std::vector<Sample*> vSigSamples4 = getInclusiveSigTTSampleVecForTable("sZVeto", lumi1, elID, muID,"2018A-D","TZBW",0);
  std::cout << "DONE Preparing signal TZBW samples" << std::endl;

  std::vector<Sample*> vSigSamples5 = getInclusiveSigTTSampleVecForTable("sZVeto", lumi1, elID, muID,"2018A-D","TZTH",0);
  std::cout << "DONE Preparing signal TZTH samples" << std::endl;

  std::vector<Sample*> vSigSamples6 = getInclusiveSigTTSampleVecForTable("sZVeto", lumi1, elID, muID,"2018A-D","TZTZ",0);
  std::cout << "DONE Preparing signal TZTZ samples" << std::endl;

  std::vector<Sample*> vSigSamples = appendSampleVectors(vSigSamples1,vSigSamples2,vSigSamples3,vSigSamples4,vSigSamples5,vSigSamples6);
  std::cout << "DONE appending signal samples" << std::endl;

  Sample* dataSample = getDataSample("sZVeto",elID,muID,"2018A-D");

  std::cout << "DONE Preparing data samples" << std::endl;

//   for(int j=2; j <5; j++){
  for(int j=2; j <3; j++){ // j is cutType //edited by rizki
    for(std::vector<Variable*>::size_type i=0; i<vVariables.size();i++){
      //    std::vector<TH1F*> vBkgHist = getHistVector(v);
      std::cout << "\nVARIABLE : "<< vVariables.at(i)->name <<"\n" << std::endl;
      DrawAndSaveCombined(vVariables.at(i),vBkgSamples,vSigSamples,dataSample, fout,elID,muID,-1,j,weightstring);
      DrawAndSaveCombined(vVariables.at(i),vBkgSamples,vSigSamples,dataSample, fout,elID,muID,0,j,weightstring);
      DrawAndSaveCombined(vVariables.at(i),vBkgSamples,vSigSamples,dataSample, fout,elID,muID,1,j,weightstring);
      DrawAndSaveCombined(vVariables.at(i),vBkgSamples,vSigSamples,dataSample, fout,elID,muID,2,j,weightstring);
      //gROOT->Reset();
    }
  }

  fout->Write();
  fout->Close();

}


void DrawAndSaveCombined(Variable* var, std::vector<Sample*> vBkg, std::vector<Sample*> vSig,Sample* dataSample, TFile* outfile, std::string elID, std::string muID, int nMu, int cutIndex,std::string weightstring){

  TCanvas* c1 = new TCanvas("c1","c1");

  c1->SetLogy();

  //make tpads
  TPad* pad1 = new TPad("pad1","pad1",0.01,0.,1,1);
  //TPad* pad2 = new TPad("pad2","pad2",0.01,0.,1,0.30);

  pad1->SetTopMargin(0.07);
  pad1->SetLogy();
  //pad2->SetTopMargin(0.0);
  pad1->SetBottomMargin(0.12);
  //pad2->SetBottomMargin(0.4);
  pad1->Draw();
  //pad2->Draw();
  //cd to top pad for drawing
  pad1->cd();
  std::stringstream cutstring;
  if(nMu>=0){
    if(cutIndex==0){                                                                                            cutstring<<"( "<<weightstring<<" (Channel=="<<nMu<<") )";}
    else if(cutIndex==1){                                                                                       cutstring<<"( "<<weightstring<<"( (Channel=="<<nMu<<"  && DilepMass >20 &&  ((Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0 ) ))";}
    else if(cutIndex==2){
      if(var->name=="Lep1PtEl" || var->name=="cleanAK4HTEl")            cutstring<<"( "<<weightstring<<"( (Channel=="<<nMu<<"  && DilepMass >20 && nCleanAK4Jets > 1&& Lep1Flavor==0 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0 ) ))";
      else if(var->name=="Lep1PtMu" || var->name=="cleanAK4HTMu")       cutstring<<"( "<<weightstring<<"( (Channel=="<<nMu<<"  && DilepMass >20 && nCleanAK4Jets > 1&& Lep1Flavor==1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0 ) ))";     
      else                                                                                                                      cutstring<<"( "<<weightstring<<"( (Channel=="<<nMu<<"  && DilepMass >20 && nCleanAK4Jets > 1 &&( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0 ) ))";
    }
    else if(cutIndex==3){
      if(var->name=="Lep1PtEl" || var->name=="cleanAK4HTEl")            cutstring<<"( "<<weightstring<<"( (Channel=="<<nMu<<"  && DilepMass >20 && nCleanAK4Jets > 1 && Lep1Flavor==0 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0 && nConst <=4 && nConst >=2) ))";
      else if(var->name=="Lep1PtMu" || var->name=="cleanAK4HTMu")       cutstring<<"( "<<weightstring<<"( (Channel=="<<nMu<<"  && DilepMass >20 && nCleanAK4Jets > 1 && Lep1Flavor==1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0  && nConst <=4 && nConst >=2 ) ))";
      else                                                                                                                      cutstring<<"( "<<weightstring<<"( (Channel=="<<nMu<<"  && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0  && nConst <=3 && nConst >=2 ) ))";}
    else if(cutIndex==4){                                                                                       cutstring<<"( "<<weightstring<<"( (Channel=="<<nMu<<"  && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0  && nConst >=4) ))";}
  }
  else {
    if(cutIndex==0){cutstring<<"( "<<     weightstring<<"( (Channel>=0 &&                                       ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0                              ) ))";}
    else if(cutIndex==1){cutstring<<"( "<<weightstring<<"( (Channel>=0 && DilepMass >20 &&                      ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0                              ) ))";}
    else if(cutIndex==2){cutstring<<"( "<<weightstring<<"( (Channel>=0 && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0                              ) ))";}
    else if(cutIndex==3){cutstring<<"( "<<weightstring<<"( (Channel>=0 && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0  && nConst <=3 && nConst >=2 ) ))";}
    else if(cutIndex==4){cutstring<<"( "<<weightstring<<"( (Channel>=0 && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0  && nConst >=4               ) ))";}
  }
/*
  if(nMu>=0){
    if(cutIndex==0){												cutstring<<"( "<<weightstring<<" (Channel=="<<nMu<<") )";}
    else if(cutIndex==1){											cutstring<<"( "<<weightstring<<"( !(Channel==2 && TMath::Abs(Lep1Phi - Lep2Phi) < 1.25 && ( (Lep1Eta>1.2 && Lep2Eta >1.2) || (Lep1Eta<-1.2 && Lep2Eta <-1.2))) &&(Channel=="<<nMu<<"  && DilepMass >20 &&  ((Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0 ) ))";}
    else if(cutIndex==2){
      if(var->name=="Lep1PtEl" || var->name=="cleanAK4HTEl") 		cutstring<<"( "<<weightstring<<"( !(Channel==2 && TMath::Abs(Lep1Phi - Lep2Phi) < 1.25 && ( (Lep1Eta>1.2 && Lep2Eta >1.2) || (Lep1Eta<-1.2 && Lep2Eta <-1.2))) &&(Channel=="<<nMu<<"  && DilepMass >20 && nCleanAK4Jets > 1&& Lep1Flavor==0 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0 ) ))";
      else if(var->name=="Lep1PtMu" || var->name=="cleanAK4HTMu") 	cutstring<<"( "<<weightstring<<"( !(Channel==2 && TMath::Abs(Lep1Phi - Lep2Phi) < 1.25 && ( (Lep1Eta>1.2 && Lep2Eta >1.2) || (Lep1Eta<-1.2 && Lep2Eta <-1.2))) &&(Channel=="<<nMu<<"  && DilepMass >20 && nCleanAK4Jets > 1&& Lep1Flavor==1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0 ) ))";
      else 															cutstring<<"( "<<weightstring<<"( !(Channel==2 && TMath::Abs(Lep1Phi - Lep2Phi) < 1.25 && ( (Lep1Eta>1.2 && Lep2Eta >1.2) || (Lep1Eta<-1.2 && Lep2Eta <-1.2))) &&(Channel=="<<nMu<<"  && DilepMass >20 && nCleanAK4Jets > 1 &&( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0 ) ))";
    }
    else if(cutIndex==3){
      if(var->name=="Lep1PtEl" || var->name=="cleanAK4HTEl") 		cutstring<<"( "<<weightstring<<"( !(Channel==2 && TMath::Abs(Lep1Phi - Lep2Phi) < 1.25 && ( (Lep1Eta>1.2 && Lep2Eta >1.2) || (Lep1Eta<-1.2 && Lep2Eta <-1.2))) &&(Channel=="<<nMu<<"  && DilepMass >20 && nCleanAK4Jets > 1 && Lep1Flavor==0 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0 && nConst <=4 && nConst >=2) ))";
      else if(var->name=="Lep1PtMu" || var->name=="cleanAK4HTMu") 	cutstring<<"( "<<weightstring<<"( !(Channel==2 && TMath::Abs(Lep1Phi - Lep2Phi) < 1.25 && ( (Lep1Eta>1.2 && Lep2Eta >1.2) || (Lep1Eta<-1.2 && Lep2Eta <-1.2))) &&(Channel=="<<nMu<<"  && DilepMass >20 && nCleanAK4Jets > 1 && Lep1Flavor==1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0  && nConst <=4 && nConst >=2 ) ))";
      else 															cutstring<<"( "<<weightstring<<"( !(Channel==2 && TMath::Abs(Lep1Phi - Lep2Phi) < 1.25 && ( (Lep1Eta>1.2 && Lep2Eta >1.2) || (Lep1Eta<-1.2 && Lep2Eta <-1.2))) &&(Channel=="<<nMu<<"  && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0  && nConst <=3 && nConst >=2 ) ))";}
    else if(cutIndex==4){											cutstring<<"( "<<weightstring<<"( !(Channel==2 && TMath::Abs(Lep1Phi - Lep2Phi) < 1.25 && ( (Lep1Eta>1.2 && Lep2Eta >1.2) || (Lep1Eta<-1.2 && Lep2Eta <-1.2))) &&(Channel=="<<nMu<<"  && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0  && nConst >=4) ))";}
  }
  else {
    if(cutIndex==0){cutstring<<"( "<<	  weightstring<<"( !(Channel==2 && TMath::Abs(Lep1Phi - Lep2Phi) < 1.25 && ( (Lep1Eta>1.2 && Lep2Eta >1.2) || (Lep1Eta<-1.2 && Lep2Eta <-1.2))) &&(Channel>=0 &&                                       ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0                              ) ))";}
    else if(cutIndex==1){cutstring<<"( "<<weightstring<<"( !(Channel==2 && TMath::Abs(Lep1Phi - Lep2Phi) < 1.25 && ( (Lep1Eta>1.2 && Lep2Eta >1.2) || (Lep1Eta<-1.2 && Lep2Eta <-1.2))) &&(Channel>=0 && DilepMass >20 &&                      ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0                              ) ))";}
    else if(cutIndex==2){cutstring<<"( "<<weightstring<<"( !(Channel==2 && TMath::Abs(Lep1Phi - Lep2Phi) < 1.25 && ( (Lep1Eta>1.2 && Lep2Eta >1.2) || (Lep1Eta<-1.2 && Lep2Eta <-1.2))) &&(Channel>=0 && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0                              ) ))";}
    else if(cutIndex==3){cutstring<<"( "<<weightstring<<"( !(Channel==2 && TMath::Abs(Lep1Phi - Lep2Phi) < 1.25 && ( (Lep1Eta>1.2 && Lep2Eta >1.2) || (Lep1Eta<-1.2 && Lep2Eta <-1.2))) &&(Channel>=0 && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0  && nConst <=3 && nConst >=2 ) ))";}
    else if(cutIndex==4){cutstring<<"( "<<weightstring<<"( !(Channel==2 && TMath::Abs(Lep1Phi - Lep2Phi) < 1.25 && ( (Lep1Eta>1.2 && Lep2Eta >1.2) || (Lep1Eta<-1.2 && Lep2Eta <-1.2))) &&(Channel>=0 && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nNonSSLeps==0  && nConst >=4               ) ))";}
  }
*/
  std::cout<<"cutstring: "<<cutstring.str()<<std::endl;
  THStack* tStack = new THStack("tStack","");
  TLegend* leg = new TLegend(0.65,0.6,0.9,0.9);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  //stupid bins for hT
  double htbins[21] = {0,120,270,420,570,720,870,1020,1170,1320,1470,1620,1770,1920,2070,2220,2370,2520,2670,2820,2970};
  int nhtbins=20;
  TH1F* h_err;
  TH1F* h_np;
  TH1F* h_npf;
  TH1F* h_cmid;
  TH1F* h_ttz;
  TH1F* h_ttw;
  TH1F* h_tth ;
  TH1F* h_tttt ;
  TH1F* h_wz ;
  TH1F* h_zz;
  TH1F* h_wwz;
  TH1F* h_wzz ;
  TH1F* h_zzz ;
  TH1F* h_wpwp ;
  TH1F* h_bos ;
  TH1F* h_ttX ;

  if(var->name=="dummycleanAK4HT"){
    //histogram for weights
     h_err = new TH1F("h_err",(var->name).c_str(),nhtbins,htbins);
     h_np = new TH1F("h_np",(var->name).c_str(),nhtbins,htbins);
     h_npf = new TH1F("h_npf",(var->name).c_str(),nhtbins,htbins);
     h_cmid = new TH1F("h_cmid",(var->name).c_str(),nhtbins,htbins);
     h_ttz = new TH1F("h_ttz",(var->name).c_str(),nhtbins,htbins);
     h_ttw = new TH1F("h_ttw",(var->name).c_str(),nhtbins,htbins); //edited by rizki
     h_tth = new TH1F("h_tth",(var->name).c_str(),nhtbins,htbins);
     h_tttt = new TH1F("h_tttt",(var->name).c_str(),nhtbins,htbins);
     h_wz = new TH1F("h_wz",(var->name).c_str(),nhtbins,htbins);
     h_zz = new TH1F("h_zz",(var->name).c_str(),nhtbins,htbins);
     h_wwz = new TH1F("h_wwz",(var->name).c_str(),nhtbins,htbins);
     h_wzz = new TH1F("h_wzz",(var->name).c_str(),nhtbins,htbins);
     h_zzz = new TH1F("h_zzz",(var->name).c_str(),nhtbins,htbins);
     h_wpwp = new TH1F("h_wpwp",(var->name).c_str(),nhtbins,htbins);
     h_bos = new TH1F("h_bos",(var->name).c_str(),nhtbins,htbins); //edited by rizki
     h_ttX = new TH1F("h_ttX",(var->name).c_str(),nhtbins,htbins); //edited by rizki
  }
  else{
     h_err = new TH1F("h_err",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
     h_np = new TH1F("h_np",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
     h_npf = new TH1F("h_npf",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
     h_cmid = new TH1F("h_cmid",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
     h_ttz = new TH1F("h_ttz",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
     h_ttw = new TH1F("h_ttw",(var->name).c_str(), var->nbins, var->xmin, var->xmax); //edited by rizki
     h_tth = new TH1F("h_tth",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
     h_tttt = new TH1F("h_tttt",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
     h_wz = new TH1F("h_wz",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
     h_zz = new TH1F("h_zz",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
     h_wwz = new TH1F("h_wwz",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
     h_wzz = new TH1F("h_wzz",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
     h_zzz = new TH1F("h_zzz",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
     h_wpwp = new TH1F("h_wpwp",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
     h_bos = new TH1F("h_bos",(var->name).c_str(), var->nbins, var->xmin, var->xmax); //edited by rizki
     h_ttX = new TH1F("h_ttX",(var->name).c_str(), var->nbins, var->xmin, var->xmax); //edited by rizki

  }

  //*******Now fill in THStack
  float lastbin = 0;
  for(std::vector<Sample*>::size_type uk=0; uk<vBkg.size(); uk++){

    //vBkg.at(uk)->setHist(var);
    Sample* s = vBkg.at(uk);
    s->setHist(var,(cutstring.str()).c_str());
  	std::cout << "-------------------------------------->>> Processing:" << s->name <<endl; //added by rizki
    TH1F* h;
    if(var->name=="dummycleanAK4HT"){
      h = new TH1F("h",(var->name).c_str(),nhtbins, htbins);
    }
    else{
      h = new TH1F("h",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
    }
    h->Sumw2();
    TTree* t = s->tree;
    if(var->name=="Lep1PtEl" || var->name=="Lep1PtMu"){ t->Project("h","Lep1Pt",(cutstring.str()).c_str());/*std::cout<<"command: t->Project(h,Lep1Pt,"<<cutstring.str()<<std::endl;*/} //edited by rizki
    else if(var->name=="cleanAK4HTEl" || var->name=="cleanAK4HTMu"){ t->Project("h","cleanAK4HT",(cutstring.str()).c_str());/*std::cout<<"command: t->Project(h,Lep1Pt,"<<cutstring.str()<<std::endl;*/} //edited by rizki
    else{ t->Project("h",(var->name).c_str(),(cutstring.str()).c_str()); /*std::cout<<"command:t->Project(h,"<<var->name<<","<<cutstring.str()<<std::endl;*/} //edited by rizki
    //set negative bins to zero - less than or equal to set overflow bin to zero
    for(int hbin=0; hbin <= h->GetNbinsX(); hbin++){
      if(h->GetBinContent(hbin+1)<0) h->SetBinContent(hbin+1,0);
    }


    //scale by weight - don't scale for data
    //skip non prompt if using data non prompt
    if( ( (s->name).find("NonPrompt")!=std::string::npos) && ((s->name).find("TT")==std::string::npos) ) { }
    //skip charge misID
    else if( (s->name).find("ChargeMisID")!=std::string::npos) {}
    else  {
      h->Scale(s->weight);
    }

    //add overflow to last bin
    float ovf =  (h)->GetBinContent( (h)->GetNbinsX()+1) + (h)->GetBinContent( (h)->GetNbinsX()) ;
    (h)->SetBinContent( (h)->GetNbinsX(),ovf);
    lastbin = lastbin + ovf;
    //std::cout<<"lastbin: "<<lastbin<<" and current overflow for sample "<<s->name<<" is: "<<ovf<<std::endl;

    if(s->name=="TTZ") h_ttz->Add(h);
    if(s->name=="TTW") h_ttw->Add(h);
    if(s->name=="TTH") h_tth->Add(h);
    if(s->name=="TTTT") h_tttt->Add(h);
    if(s->name=="WZ") h_wz->Add(h);
    if(s->name=="ZZ") h_zz->Add(h);
    if(s->name=="WWZ") h_wwz->Add(h);
    if(s->name=="WZZ") h_wzz->Add(h);
    if(s->name=="ZZZ") h_zzz->Add(h);
    if(s->name=="WpWp") h_wpwp->Add(h);

    //add to legend
    if(s->name=="TTZ") h_ttX->Add(h);
    else if(s->name=="TTW") h_ttX->Add(h);
    else if(s->name=="TTH") h_ttX->Add(h);
    else if(s->name=="TTTT") h_ttX->Add(h);
    else if(s->name=="WZ") h_bos->Add(h);
    else if(s->name=="ZZ") h_bos->Add(h);
    else if(s->name=="WWZ") h_bos->Add(h);
    else if(s->name=="WZZ") h_bos->Add(h);
    else if(s->name=="ZZZ") h_bos->Add(h);
    else if(s->name=="WpWp") h_bos->Add(h);

    else if(s->name=="NonPrompt") h_npf->Add(h);
    else if(s->name=="ChargeMisID")h_cmid->Add(h);

    //aesthetics
    h->SetFillColor(s->color);
    h->GetXaxis()->SetTitle(var->Xaxis.c_str());
    h->GetYaxis()->SetTitle(var->Yaxis.c_str());

    //else std::cout<<"sample "<<s->name<<" not added!!!"<<std::endl;
    assert(h);
    //tStack->Add(h);

    delete h; //added by rizki

  }

  //add to tStack
  h_bos->SetFillColor(kGreen+3);
  tStack->Add(h_bos);
  leg->AddEntry(h_bos,"VV(V)","f");
  h_ttX->SetFillColor(kYellow-3);
  tStack->Add(h_ttX);
  leg->AddEntry(h_ttX,"t#bar{t} + X","f");
  h_cmid->SetFillColor(kAzure+6);
  tStack->Add(h_cmid);
  leg->AddEntry(h_cmid,"ChargeMisID","f");
  h_npf->SetFillColor(kGray);
  tStack->Add(h_npf);
  leg->AddEntry(h_npf, "Non-prompt", "f");

  //add to error hist
  h_err->Add(h_bos);
  h_err->Add(h_ttX);
  h_err->Add(h_cmid);
  h_err->Add(h_npf);


  //make errors
  TGraphAsymmErrors *tgae_err = new TGraphAsymmErrors(); //for some reason this shifts the bins by 1.
  std::vector<float> errs;
  for(unsigned int ibin=1; ibin<= h_err->GetNbinsX(); ibin++){

    tgae_err->SetPoint(ibin,h_err->GetBinCenter(ibin),h_err->GetBinContent(ibin)); //make sure h_err = tgae_err

    //check bin contents vs error hist - just doing this here since looping over bins anyway
    std::cout<<"Bin: "<<ibin<<" ("<< h_err->GetBinLowEdge(ibin) <<")" << " err hist content: "<<h_err->GetBinContent(ibin)<<", summed bkg: "<<(h_ttX->GetBinContent(ibin)+h_bos->GetBinContent(ibin)+h_cmid->GetBinContent(ibin)+h_npf->GetBinContent(ibin))<<" NP bkg: "<<h_npf->GetBinContent(ibin)<<" CMID: "<<h_cmid->GetBinContent(ibin)<<" TTX: "<<h_ttX->GetBinContent(ibin)<<" BOSON: "<<h_bos->GetBinContent(ibin)<<std::endl;

    //nonprompt
    float etemp = 0.0;
    float etemp_stat = 0.0;
    float etemp_sys = 0.0;
    //if(h_npf->GetBinContent(ibin)!=0) etemp_stat = etemp_stat + pow(h_npf->GetBinError(ibin),2); //stat
    //else etemp_stat = etemp_stat + 1.8*1.8; //stat
    etemp_stat= etemp_stat + pow(h_npf->GetBinError(ibin),2); //stat
    etemp_sys = etemp_sys + pow( unc_NP*h_npf->GetBinContent(ibin),2);//sys
    //chargemisID
    //if(h_cmid->GetBinContent(ibin)!=0) etemp_stat = etemp_stat + pow(h_cmid->GetBinError(ibin),2);//stat
    //else etemp_stat = etemp_stat+1.8*1.8; //stat
    etemp_stat = etemp_stat + pow(h_cmid->GetBinError(ibin),2); //stat
    etemp_sys = etemp_sys + pow(unc_CMID*h_cmid->GetBinContent(ibin),2);//sys
    //TTZ
    etemp_stat= etemp_stat + pow(h_ttz->GetBinError(ibin),2);//stat
    etemp_sys = etemp_sys + pow(uncMC_TTZ*h_ttz->GetBinContent(ibin),2); //MC
    etemp_sys = etemp_sys + pow(uncJES_TTZ*h_ttz->GetBinContent(ibin),2); //JES
    etemp_sys = etemp_sys + pow(uncJER_TTZ*h_ttz->GetBinContent(ibin),2); //JER
    etemp_sys = etemp_sys + pow(uncLumi*h_ttz->GetBinContent(ibin),2); //lumi
    etemp_sys = etemp_sys + pow(uncPU_TTZ*h_ttz->GetBinContent(ibin),2); //pileup
    etemp_sys = etemp_sys + pow(uncTrigSF_TTZ*h_ttz->GetBinContent(ibin),2); //trigger
    etemp_sys = etemp_sys + pow(uncIsoSF*h_ttz->GetBinContent(ibin),2); //iso
    etemp_sys = etemp_sys + pow(uncIDSF_TTZ*h_ttz->GetBinContent(ibin),2); //id
    //etemp_sys = etemp_sys + 2*pow(0.02*h_ttz->GetBinContent(ibin),2); //id plus iso
    //TTW
    etemp_stat = etemp_stat + pow(h_ttw->GetBinError(ibin),2);//stat
    etemp_sys = etemp_sys + pow(uncMC_TTW*h_ttw->GetBinContent(ibin),2); //MC
    etemp_sys = etemp_sys + pow(uncJES_TTW*h_ttw->GetBinContent(ibin),2); //JES
    etemp_sys = etemp_sys + pow(uncJER_TTW*h_ttw->GetBinContent(ibin),2); //JER
    etemp_sys = etemp_sys + pow(uncLumi*h_ttw->GetBinContent(ibin),2); //lumi
    etemp_sys = etemp_sys + pow(uncPU_TTW*h_ttw->GetBinContent(ibin),2); //pileup
    etemp_sys = etemp_sys + pow(uncTrigSF_TTW*h_ttw->GetBinContent(ibin),2); //trigger
    etemp_sys = etemp_sys + pow(uncIsoSF*h_ttw->GetBinContent(ibin),2); //iso
    etemp_sys = etemp_sys + pow(uncIDSF_TTW*h_ttw->GetBinContent(ibin),2); //id
    //etemp_sys = etemp_sys + 2*pow(0.02*h_ttw->GetBinContent(ibin),2); //id plus iso
    //TTH
    etemp_stat = etemp_stat + pow(h_tth->GetBinError(ibin),2);//stat
    etemp_sys = etemp_sys + pow(uncMC_TTH*h_tth->GetBinContent(ibin),2); //MC
    etemp_sys = etemp_sys + pow(uncJES_TTH*h_tth->GetBinContent(ibin),2); //JES
    etemp_sys = etemp_sys + pow(uncJER_TTH*h_tth->GetBinContent(ibin),2); //JER
    etemp_sys = etemp_sys + pow(uncLumi*h_tth->GetBinContent(ibin),2); //lumi
    etemp_sys = etemp_sys + pow(uncPU_TTH*h_tth->GetBinContent(ibin),2); //pileup
    etemp_sys = etemp_sys + pow(uncTrigSF_TTH*h_tth->GetBinContent(ibin),2); //trigger
    etemp_sys = etemp_sys + pow(uncIsoSF*h_tth->GetBinContent(ibin),2); //iso
    etemp_sys = etemp_sys + pow(uncIDSF_TTH*h_tth->GetBinContent(ibin),2); //id
    //etemp_sys = etemp_sys + 2*pow(0.02*h_tth->GetBinContent(ibin),2); //id plus iso
    //TTTT
    etemp_stat = etemp_stat + pow(h_tttt->GetBinError(ibin),2);//stat
    etemp_sys = etemp_sys + pow(uncMC_TTTT*h_tttt->GetBinContent(ibin),2); //MC
    etemp_sys = etemp_sys + pow(uncJES_TTTT*h_tttt->GetBinContent(ibin),2); //JES
    etemp_sys = etemp_sys + pow(uncJER_TTTT*h_tttt->GetBinContent(ibin),2); //JER
    etemp_sys = etemp_sys + pow(uncLumi*h_tttt->GetBinContent(ibin),2); //lumi
    etemp_sys = etemp_sys + pow(uncPU_TTTT*h_tttt->GetBinContent(ibin),2); //pileup
    etemp_sys = etemp_sys + pow(uncTrigSF_TTTT*h_tttt->GetBinContent(ibin),2); //trigger
    etemp_sys = etemp_sys + pow(uncIsoSF*h_tttt->GetBinContent(ibin),2); //iso
    etemp_sys = etemp_sys + pow(uncIDSF_TTTT*h_tttt->GetBinContent(ibin),2); //id
    //etemp_sys = etemp_sys + 2*pow(0.02*h_tttt->GetBinContent(ibin),2); //id plus iso
    //WZ
    etemp_stat = etemp_stat + pow(h_wz->GetBinError(ibin),2);//stat
    etemp_sys = etemp_sys + pow(uncMC_WZ*h_wz->GetBinContent(ibin),2); //MC
    etemp_sys = etemp_sys + pow(uncJES_WZ*h_wz->GetBinContent(ibin),2); //JES
    etemp_sys = etemp_sys + pow(uncJER_WZ*h_wz->GetBinContent(ibin),2); //JER
    etemp_sys = etemp_sys + pow(uncLumi*h_wz->GetBinContent(ibin),2); //lumi
    etemp_sys = etemp_sys + pow(uncPU_WZ*h_wz->GetBinContent(ibin),2); //pileup
    etemp_sys = etemp_sys + pow(uncTrigSF_WZ*h_wz->GetBinContent(ibin),2); //trigger
    etemp_sys = etemp_sys + pow(uncIsoSF*h_wz->GetBinContent(ibin),2); //iso
    etemp_sys = etemp_sys + pow(uncIDSF_WZ*h_wz->GetBinContent(ibin),2); //id
    //etemp_sys = etemp_sys + 2*pow(0.02*h_wz->GetBinContent(ibin),2); //id plus iso
    //ZZ
    etemp_stat = etemp_stat + pow(h_zz->GetBinError(ibin),2);//stat
    etemp_sys = etemp_sys + pow(uncMC_ZZ*h_zz->GetBinContent(ibin),2); //MC
    etemp_sys = etemp_sys + pow(uncJES_ZZ*h_zz->GetBinContent(ibin),2); //JES
    etemp_sys = etemp_sys + pow(uncJER_ZZ*h_zz->GetBinContent(ibin),2); //JER
    etemp_sys = etemp_sys + pow(uncLumi*h_zz->GetBinContent(ibin),2); //lumi
    etemp_sys = etemp_sys + pow(uncPU_ZZ*h_zz->GetBinContent(ibin),2); //pileup
    etemp_sys = etemp_sys + pow(uncTrigSF_ZZ*h_zz->GetBinContent(ibin),2); //trigger
    etemp_sys = etemp_sys + pow(uncIsoSF*h_zz->GetBinContent(ibin),2); //iso
    etemp_sys = etemp_sys + pow(uncIDSF_ZZ*h_zz->GetBinContent(ibin),2); //id
    //etemp_sys = etemp_sys + 2*pow(0.02*h_zz->GetBinContent(ibin),2); //id plus iso
    //WPWP
    etemp_stat = etemp_stat + pow(h_wpwp->GetBinError(ibin),2);//stat
    etemp_sys = etemp_sys + pow(uncMC_WpWp*h_wpwp->GetBinContent(ibin),2); //MC
    etemp_sys = etemp_sys + pow(uncJES_WpWp*h_wpwp->GetBinContent(ibin),2); //JES
    etemp_sys = etemp_sys + pow(uncJER_WpWp*h_wpwp->GetBinContent(ibin),2); //JER
    etemp_sys = etemp_sys + pow(uncLumi*h_wpwp->GetBinContent(ibin),2); //lumi
    etemp_sys = etemp_sys + pow(uncPU_WpWp*h_wpwp->GetBinContent(ibin),2); //pileup
    etemp_sys = etemp_sys + pow(uncTrigSF_WpWp*h_wpwp->GetBinContent(ibin),2); //trigger
    etemp_sys = etemp_sys + pow(uncIsoSF*h_wpwp->GetBinContent(ibin),2); //iso
    etemp_sys = etemp_sys + pow(uncIDSF_WpWp*h_wpwp->GetBinContent(ibin),2); //id
    //etemp_sys = etemp_sys + 2*pow(0.02*h_wpwp->GetBinContent(ibin),2); //id plus iso
    //WWZ
    etemp_stat = etemp_stat + pow(h_wwz->GetBinError(ibin),2);//stat
    etemp_sys = etemp_sys + pow(uncMC_WWZ*h_wwz->GetBinContent(ibin),2); //MC
    etemp_sys = etemp_sys + pow(uncJES_WWZ*h_wwz->GetBinContent(ibin),2); //JES
    etemp_sys = etemp_sys + pow(uncJER_WWZ*h_wwz->GetBinContent(ibin),2); //JER
    etemp_sys = etemp_sys + pow(uncLumi*h_wwz->GetBinContent(ibin),2); //lumi
    etemp_sys = etemp_sys + pow(uncPU_WWZ*h_wwz->GetBinContent(ibin),2); //pileup
    etemp_sys = etemp_sys + pow(uncTrigSF_WWZ*h_wwz->GetBinContent(ibin),2); //trigger
    etemp_sys = etemp_sys + pow(uncIsoSF*h_wwz->GetBinContent(ibin),2); //iso
    etemp_sys = etemp_sys + pow(uncIDSF_WWZ*h_wwz->GetBinContent(ibin),2); //id
    //etemp_sys = etemp_sys + 2*pow(0.02*h_wwz->GetBinContent(ibin),2); //id plus iso
    //WZZ
    etemp_stat = etemp_stat + pow(h_wzz->GetBinError(ibin),2);//stat
    etemp_sys = etemp_sys + pow(uncMC_WZZ*h_wzz->GetBinContent(ibin),2); //MC
    etemp_sys = etemp_sys + pow(uncJES_WZZ*h_wzz->GetBinContent(ibin),2); //JES
    etemp_sys = etemp_sys + pow(uncJER_WZZ*h_wzz->GetBinContent(ibin),2); //JER
    etemp_sys = etemp_sys + pow(uncLumi*h_wzz->GetBinContent(ibin),2); //lumi
    etemp_sys = etemp_sys + pow(uncPU_WZZ*h_wzz->GetBinContent(ibin),2); //pileup
    etemp_sys = etemp_sys + pow(uncTrigSF_WZZ*h_wzz->GetBinContent(ibin),2); //trigger
    etemp_sys = etemp_sys + pow(uncIsoSF*h_wzz->GetBinContent(ibin),2); //iso
    etemp_sys = etemp_sys + pow(uncIDSF_WZZ*h_wzz->GetBinContent(ibin),2); //id
    //etemp_sys = etemp_sys + 2*pow(0.02*h_wzz->GetBinContent(ibin),2); //id plus iso
    //ZZZ
    etemp_stat = etemp_stat + pow(h_zzz->GetBinError(ibin),2);//stat
    etemp_sys = etemp_sys + pow(uncMC_ZZZ*h_zzz->GetBinContent(ibin),2); //MC
    etemp_sys = etemp_sys + pow(uncJES_ZZZ*h_zzz->GetBinContent(ibin),2); //JES
    etemp_sys = etemp_sys + pow(uncJER_ZZZ*h_zzz->GetBinContent(ibin),2); //JER
    etemp_sys = etemp_sys + pow(uncLumi*h_zzz->GetBinContent(ibin),2); //lumi
    etemp_sys = etemp_sys + pow(uncPU_ZZZ*h_zzz->GetBinContent(ibin),2); //pileup
    etemp_sys = etemp_sys + pow(uncTrigSF_ZZZ*h_zzz->GetBinContent(ibin),2); //trigger
    etemp_sys = etemp_sys + pow(uncIsoSF*h_zzz->GetBinContent(ibin),2); //iso
    etemp_sys = etemp_sys + pow(uncIDSF_ZZZ*h_zzz->GetBinContent(ibin),2); //id
    //etemp_sys = etemp_sys + 2*pow(0.02*h_zzz->GetBinContent(ibin),2); //id plus iso

    //e_temp = etemp_stat + etemp_sys;

	//process stat error with Asymm poisson error and combine with sys:
	etemp = poissonErrors(tgae_err,h_err,etemp_sys,ibin);
	//std::cout<<"Bin: "<<ibin<<" err hist content: "<<h_err->GetBinContent(ibin);
	std::cout<<"	  tgae: " << tgae_err->GetY()[ibin];
	std::cout<<" +" << tgae_err->GetErrorYhigh(ibin);
	std::cout<<" -" << tgae_err->GetErrorYlow(ibin);
	std::cout << std::endl;


	//actually everything above are squared errors
    etemp = pow(etemp,0.5);
    etemp_stat = pow(etemp_stat,0.5);
    etemp_sys = pow(etemp_sys,0.5);

	//h_err will be used for pull calculation.
    h_err->SetBinError(ibin,etemp);
    errs.push_back(etemp);

  }

  //now set err bin to have overflow content
  //h_err->SetBinContent(h_err->GetNbinsX(),lastbin);

  tStack->Draw("HIST");
  tStack->GetXaxis()->SetTitle(var->Xaxis.c_str());
  tStack->GetXaxis()->CenterTitle();
  //tStack->GetXaxis()->SetTitleSize(0.015);
  tStack->GetYaxis()->SetTitle(var->Yaxis.c_str());
  tStack->GetYaxis()->CenterTitle();
  //tStack->GetYaxis()->SetRangeUser(0.008,100000);
  tStack->SetMaximum(15 * tStack->GetMaximum());
  tStack->SetMinimum(0.008);
  tStack->Draw("HIST");
  //tStack->GetYaxis()->SetTitleSize(0.015);
  c1->RedrawAxis();
  c1->Update();
  //draw errors
//   h_err->SetFillStyle(3344);
//   h_err->SetFillColor(1);
//   h_err->Draw("SAME E2");
  tgae_err->SetFillStyle(3344);
  tgae_err->SetFillColor(1);
  tgae_err->Draw("SAME E2");
  if(var->name=="cleanAK4HT"){
    for(unsigned int jb=1; jb<=h_err->GetNbinsX(); jb++){
      float totbkg = h_npf->GetBinContent(jb) + h_cmid->GetBinContent(jb) + h_ttw->GetBinContent(jb) + h_ttz->GetBinContent(jb) + h_wz->GetBinContent(jb) + h_wpwp->GetBinContent(jb) + h_zz->GetBinContent(jb)+h_wwz->GetBinContent(jb) + h_tth->GetBinContent(jb) + h_tttt->GetBinContent(jb) + h_wzz->GetBinContent(jb) + h_zzz->GetBinContent(jb);
    float stacktot = ( (TH1F*)(tStack->GetStack()->Last()))->GetBinContent(jb);
    //std::cout<<"bin: "<<jb<<"stack: "<<stacktot<<" bkg tot: "<<totbkg<<" total in h_err: "<<h_err->GetBinContent(jb)<<" np tot: "<<h_npf->GetBinContent(jb)<<" cmid tot: "<<h_cmid->GetBinContent(jb)<<" error: "<<h_err->GetBinError(jb)<<std::endl;
      //std::cout<<"bin: "<<jb<<" error on fancy way: "<<h_np->GetBinError(jb)<<" error on standard way: "<<h_npf->GetBinError(jb)<<" fancy content: "<<h_np->GetBinContent(jb)<<" reg content: "<<h_npf->GetBinContent(jb)<<std::endl;
    }
  }

  //draw signal:
  TH1F* hSig_1 = new TH1F("hSig_1",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
  TH1F* hSig_2 = new TH1F("hSig_2",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
  for(std::vector<Sample*>::size_type i1=0;i1<vSig.size();i1++){

    Sample* s = vSig.at(i1);
    if(s->name.find("1200")==std::string::npos) continue;
  	std::cout << "-------------------------------------->>> Processing:" << s->name <<endl; //added by rizki
    TH1F* h = new TH1F("h",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
    TTree* t = s->tree;

    if(var->name=="Lep1PtEl" || var->name=="Lep1PtMu"){ t->Project("h","Lep1Pt",(cutstring.str()+" * pdfWeights4LHC[0] ").c_str());}
    else if(var->name=="cleanAK4HTEl" || var->name=="cleanAK4HTMu"){ t->Project("h","cleanAK4HT",(cutstring.str()+" * pdfWeights4LHC[0] ").c_str());std::cout<<"command: t->Project(h,Lep1Pt,"<<cutstring.str()+" * pdfWeights4LHC[0] "<<std::endl;}
    else{ t->Project("h",(var->name).c_str(),(cutstring.str()+" * pdfWeights4LHC[0] ").c_str());}

    float ovf =  (h)->GetBinContent( (h)->GetNbinsX()+1) + (h)->GetBinContent( (h)->GetNbinsX()) ;
    (h)->SetBinContent( (h)->GetNbinsX(),ovf);
    //scale by weight
    h->Scale(s->weight);
    //aesthetics
    h->SetFillStyle(0);
    h->SetLineColor(s->color);
    hSig_1->SetLineColor(s->color);
    h->SetLineWidth(2);
    hSig_1->SetLineWidth(2);
    h->SetLineStyle(s->linestyle);
    h->GetXaxis()->SetTitle(var->Xaxis.c_str());
    h->GetYaxis()->SetTitle(var->Yaxis.c_str());
    hSig_1->Add(h);
    hSig_1->SetLineStyle(s->linestyle);

    delete h; //added by rizki
  }
  hSig_1->Draw("hist same");
  leg->AddEntry(hSig_1,"T#bar{T}(1.2 Tev)","l");
  for(std::vector<Sample*>::size_type i1=0;i1<vSig.size();i1++){

    Sample* s = vSig.at(i1);
    if(s->name.find("1500")==std::string::npos) continue;
  	std::cout << "-------------------------------------->>> Processing:" << s->name <<endl; //added by rizki
    TH1F* h = new TH1F("h",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
    TTree* t = s->tree;

    if(var->name=="Lep1PtEl" || var->name=="Lep1PtMu"){ t->Project("h","Lep1Pt",(cutstring.str()+" * pdfWeights4LHC[0] ").c_str());}
    else if(var->name=="cleanAK4HTEl" || var->name=="cleanAK4HTMu"){ t->Project("h","cleanAK4HT",(cutstring.str()+" * pdfWeights4LHC[0] ").c_str());std::cout<<"command: t->Project(h,Lep1Pt,"<<cutstring.str()+" * pdfWeights4LHC[0] "<<std::endl;}
    else{ t->Project("h",(var->name).c_str(),(cutstring.str()+" * pdfWeights4LHC[0] ").c_str());}

    float ovf =  (h)->GetBinContent( (h)->GetNbinsX()+1) + (h)->GetBinContent( (h)->GetNbinsX()) ;
    (h)->SetBinContent( (h)->GetNbinsX(),ovf);
    //scale by weight
    h->Scale(s->weight);
    //aesthetics
    h->SetFillStyle(0);
    h->SetLineColor(s->color);
    hSig_2->SetLineColor(s->color);
    h->SetLineWidth(2);
    hSig_2->SetLineWidth(2);
    h->SetLineStyle(s->linestyle);
    h->GetXaxis()->SetTitle(var->Xaxis.c_str());
    h->GetYaxis()->SetTitle(var->Yaxis.c_str());
    hSig_2->Add(h);
    hSig_2->SetLineStyle(s->linestyle);

    delete h; //added by rizki
  }
  hSig_2->Draw("hist same");
  leg->AddEntry(hSig_2,"T#bar{T}(1.5 Tev)","l");


  //Draw data- merge trees into chain
  TH1F* hData = new TH1F("hData",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
  TChain* tData = new TChain("tEvts_sZVeto");
  tData->AddFile(dataSample->file->GetName());

  if(var->name=="Lep1PtEl" || var->name=="Lep1PtMu"){
    tData->Project("hData","Lep1Pt",(cutstring.str()).c_str());
  }
  else if(var->name=="cleanAK4HTEl" || var->name=="cleanAK4HTMu"){
    tData->Project("hData","cleanAK4HT",(cutstring.str()).c_str());std::cout<<"command: t->Project(h,Lep1Pt,"<<cutstring.str()<<std::endl;
  }
  else{
    tData->Project("hData",(var->name).c_str(),(cutstring.str()).c_str());
  }
  float ovf_d =  (hData)->GetBinContent( (hData)->GetNbinsX()+1) + (hData)->GetBinContent( (hData)->GetNbinsX()) ;
  (hData)->SetBinContent( (hData)->GetNbinsX(),ovf_d);
  TGraphAsymmErrors* gData = new TGraphAsymmErrors(hData);
  gData->SetMarkerStyle(20);
  for(unsigned int ig = 0; ig < gData->GetN(); ig++){
    int N = gData->GetY()[ig];
    if(N==0){
      //double L =  (N==0) ? 0  : (ROOT::Math::gamma_quantile((1.0-0.6827)/2,N,1.));
      //double U =  ROOT::Math::gamma_quantile_c((1.0-0.6827)/2,N+1,1) ;
      //gData->SetPointEYlow(ig, N-L);
      //gData->SetPointEYhigh(ig, U-N);
    }
    else{
      //gData->SetPointError(ig,0,gData->GetEY()[ig]);
    }
    gData->SetPointEXlow(ig,0);
    gData->SetPointEXhigh(ig,0);
  }
  //gData->Draw("pesame");
  //gData->Print();

  //leg->AddEntry(gData,"Data","p");

  std::cout << "-------------------------------------->>> Done processing Data "<<endl; //added by rizki

  //latex
  TLatex* cmstex = new TLatex();
  cmstex->SetNDC();
  cmstex->SetTextSize(0.04);
  TLatex* lumitex = new TLatex();
  lumitex->SetNDC();
  lumitex->SetTextSize(0.04);

  //draw legend, cms and lumi
  leg->Draw("same");

  //draw latex
  cmstex->DrawLatex(0.15,0.96,"CMS Preliminary");
  lumitex->DrawLatex(0.65,0.96,Form("%2f fb^{-1} (13 TeV)", lumi1));

  //draw latex for channels
  TLatex* chantex = new TLatex();
  chantex->SetNDC();
  chantex->SetTextSize(0.04);
  if(nMu==-1) chantex->DrawLatex(0.25,0.8,"All Channels");
  if(nMu==0) chantex->DrawLatex(0.25,0.8,"#bf{ee}");
  if(nMu==1) chantex->DrawLatex(0.25,0.8,"#bf{e#mu}");
  if(nMu==2) chantex->DrawLatex(0.25,0.8,"#bf{#mu#mu}");

  /*
  //now draw pull plot
  pad2->cd();
  TH1F* hpull = getPullPlot(hData, tStack, var, h_err);
  hpull->Draw("HIST");
  //This changes the range of the X axis
  TAxis* axis = hpull->GetXaxis();
  //ChangeAxisRange(title[i], hpull->GetXaxis());
  hpull->GetXaxis()->SetLabelSize(0.15);
  float xmin = hpull->GetBinLowEdge(axis->GetFirst());
  float xmax = hpull->GetBinLowEdge(axis->GetLast()+1);
  TLine* baseline = new TLine(xmin,0,xmax,0);
  TLine* plus2line = new TLine(xmin,2,xmax,2);
  TLine* minus2line = new TLine(xmin,-2,xmax,-2);
  baseline->SetLineColor(kBlack);
  baseline->SetLineStyle(2);
  plus2line->SetLineStyle(2);
  minus2line->SetLineStyle(2);
  baseline->Draw();
  plus2line->Draw();
  minus2line->Draw();
  TLine* plus1line = new TLine(xmin,1,xmax,1);
  TLine* minus1line = new TLine(xmin,-1,xmax,-1);
  plus1line->SetLineColor(kBlack);
  plus1line->SetLineStyle(3);
  minus1line->SetLineColor(kBlack);
  minus1line->SetLineStyle(3);
  plus1line->Draw();
  minus1line->Draw();
  //pad->RedrawAxis();
  c1->Update();
  */

  //rename plots by channel
  std::string channel;
  if(nMu==-1) channel="All";
  if(nMu==0) channel="ElEl";
  if(nMu==1) channel="ElMu";
  if(nMu==2) channel="MuMu";
  std::string cutname;
  if(cutIndex==0) cutname =  "sZVeto";
  if(cutIndex==1) cutname =  "QuarkoniaAndElElZVeto";
  if(cutIndex==2) cutname =  "TwoJets";
  if(cutIndex==3) cutname =  "nConst2to3"; //"nConst2to4";
  if(cutIndex==4) cutname =  "nConst4plus";//"nConst5plus";
  std::string pdfname = "./"+saveFolder+"/"+(var->name)+"_"+(vBkg[0]->cutname)+"_"+channel+"_"+cutname+"_Mu"+muID+"_El"+elID+".pdf";
  std::string pngname = "./"+saveFolder+"/"+(var->name)+"_"+(vBkg[0]->cutname)+"_"+channel+"_"+cutname+"_Mu"+muID+"_El"+elID+".png";
  std::string Cname = "./"+saveFolder+"/"+(var->name)+"_"+(vBkg[0]->cutname)+"_"+channel+"_"+cutname+"_Mu"+muID+"_El"+elID+".C";

  std::cout << "WRITING : " << pdfname << std::endl;
  c1->SaveAs(pdfname.c_str());
  c1->SaveAs(pngname.c_str());
  c1->SaveAs(Cname.c_str());
  outfile->WriteTObject(c1);

  delete c1;

//   delete pad1; //added by rizki
//   delete pad2; //added by rizki

  delete tStack; //added by rizki
  delete leg; //added by rizki

  delete h_err; //added by rizki
  delete h_np; //added by rizki
  delete h_npf; //added by rizki
  delete h_cmid; //added by rizki
  delete h_ttz; //added by rizki
  delete h_ttw; //added by rizki
  delete h_tth ; //added by rizki
  delete h_tttt ; //added by rizki
  delete h_wz ; //added by rizki
  delete h_zz; //added by rizki
  delete h_wwz; //added by rizki
  delete h_wzz ; //added by rizki
  delete h_zzz ; //added by rizki
  delete h_wpwp ; //added by rizki
  delete h_bos ; //added by rizki
  delete h_ttX ; //added by rizki

  delete hSig_1; //added by rizki
  delete hData; //added by rizki
  delete tData; //added by rizki
  delete gData; //added by rizki

  delete cmstex;
  delete lumitex;

  delete chantex; //added by rizki
  //delete baseline; //added by rizki
  //delete plus2line; //added by rizki
  //delete minus2line; //added by rizki
  //delete plus1line; //added by rizki
  //delete minus1line; //added by rizki

}

TH1F* getPullPlot(TH1F* hData, THStack * h, Variable* var, TH1F* h_err){

  string title = hData->GetTitle();
  title += " (Pull)";
  TH1F* hpull = (TH1F*)hData->Clone(title.c_str());

  int size = hData->GetNbinsX();
  //hpull->SetTitle(";;Residual");//#sigma(Data-MC)");
  hpull->SetTitle(";;#frac{(obs-bkg)}{#sigma}");
  //clint's edit to change title size
  hpull->GetYaxis()->SetTitleSize(0.12);
  hpull->GetYaxis()->SetTitleOffset(0.4);
  hpull->GetXaxis()->SetTitle((var->Xaxis).c_str());
  //hpull->GetXaxis()->SetTitle("L_{T}^{0e3#mu} #equiv #Sigma p_{T}^{#font[12]{l}} (GeV)");
  hpull->GetXaxis()->SetTitleSize(0.17);
  hpull->GetXaxis()->SetTitleOffset(1.02);
  hpull->GetXaxis()->CenterTitle();
  hpull->GetYaxis()->CenterTitle();
  //hpull->SetTitle(title.c_str());
  TH1F* hsum = (TH1F*) h->GetStack()->Last();
  for(int i=1; i<=size; ++i){

    float error;
    float diff;
    //diff = (hsum->GetBinContent(i) > 0) ? diff/hData->GetBinError(i) : 0.;
    //if(debug_) cout<<"diff for bin "<<i<<" is "<<diff<<endl;
    //cout<<"diff for bin "<<i<<" is "<<diff<<endl;
    if(hData->GetBinContent(i)>=1){
      int N = hData->GetBinContent(i);
      double L =  (N==0) ? 0  : (ROOT::Math::gamma_quantile((1.0-0.6827)/2,N,1.));
      double U = ROOT::Math::gamma_quantile_c((1.0-0.6827)/2.0,N+1,1);
      if(hData->GetBinContent(i)>h_err->GetBinContent(i)){
		error = pow( pow(h_err->GetBinError(i),2) + pow(N-L,2),0.5);
		diff = (hData->GetBinContent(i) - hsum->GetBinContent(i))/error;
      }
      else{
		error = pow( pow(h_err->GetBinError(i),2) + pow(U-N,2),0.5);
		diff = (hData->GetBinContent(i) - hsum->GetBinContent(i))/error;
      }
      hpull->SetBinContent(i,diff);
      hpull->SetBinError(i,0);//Cory: include mc error
    }
    else{
      double U = ROOT::Math::gamma_quantile_c((1.0-0.6827)/2.0,1,1);
      error=pow( pow(h_err->GetBinError(i),2) + pow(U,2),0.5);
      diff=((-1.0)*hsum->GetBinContent(i))/error;
      hpull->SetBinContent(i,diff);
      hpull->SetBinError(i,0);//Cory: include mc error
    }
  }
  hpull->SetAxisRange( -3, 3, "Y");
  //hpull->GetYaxis()->SetRangeUser(-2.5,2.5);
  hpull->GetYaxis()->SetNdivisions(203);
  hpull->GetYaxis()->SetTickLength(0.02);
  hpull->GetYaxis()->SetLabelSize(0.13);
  hpull->GetXaxis()->SetLabelSize(0.35);
  hpull->SetLineColor(kBlack);
  hpull->SetFillColor(kRed);
  return hpull;
}


double poissonErrors(TGraphAsymmErrors *tgae_err, TH1F* h, double sys_SqErr, int ibin){
	float alpha = 1. - 0.6827;
    double N = h->GetBinContent(ibin);
    double N_g = tgae_err->GetY()[ibin];
    double L =  (N==0.) ? 0  : (ROOT::Math::gamma_quantile((alpha)/2,N,1.));
    double U =  (N==0.) ? 0  : ROOT::Math::gamma_quantile_c((alpha)/2.0,N+1,1);
    double totSqErr_down = pow(N-L,2)+ sys_SqErr;
    double totSqErr_up =  pow(U-N,2)+ sys_SqErr;
//     std::cout << "Bin: "<<ibin<<" N = " << N;
//     std::cout << " N_g = " << N_g;
//     std::cout << " +" << pow(totSqErr_up,0.5);
//     std::cout << " -" << pow(totSqErr_down,0.5);
//     std::cout << "( N-L = " << N-L;
//     std::cout << ", U-N = " << U-N;
//     std::cout << ", sys_err = " << pow(sys_SqErr,0.5);
//     std::cout << " )"<<std::endl;
    tgae_err->SetPointEYlow(ibin,pow(totSqErr_down,0.5));
    tgae_err->SetPointEYhigh(ibin,pow(totSqErr_up,0.5));

    tgae_err->SetPointEXlow(ibin,h->GetBinWidth(ibin)/2);
    tgae_err->SetPointEXhigh(ibin,h->GetBinWidth(ibin)/2);

    //return the largest sensible error
    double totSqErr =  0.0;
    if (N-totSqErr_up < 0) totSqErr=totSqErr_down;
    else totSqErr=totSqErr_up;
    return totSqErr;
}
