#include <iostream>
#include <iomanip> 
#include <vector>
#include <string>
#include "TString.h"
#include "TRegexp.h"

bool DEBUG = true;


//float getEvents(TTree* t,TString cut){
//  float nEvents = t->Draw("Lep1Pt",cut);
//  return nEvents;
//}
float getEvents(TTree* t,TString cut){
  TH1F* h = new TH1F("h","h",1,0,1000000);
  t->Project("h","Lep1Pt",cut);
  float nEvents = h->Integral();
  delete h;

  return nEvents;
}


void printLineNPBkg(std::string sample, TString filename,std::stringstream& filecontent){

  TString nomcut = "ChargeMisIDWeight * NPWeight * IsoSF * trigSF * GsfSF * PUWeight * IDSF * MCWeight * (Lep1Pt>40 && nConst >=4 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";

  TRegexp reFileExt(".root");
  TString filename_ElPR = filename; filename_ElPR(reFileExt) = "_UnityElPR.root";
  TString filename_MuPR = filename; filename_MuPR(reFileExt) = "_UnityMuPR.root";

  TFile* f = new TFile(filename);
  TTree* t = (TTree*) f->Get("tEvts_sZVeto");
  TFile* f_ElPR = new TFile(filename_ElPR);
  TTree* t_ElPR = (TTree*) f_ElPR->Get("tEvts_sZVeto");
  TFile* f_MuPR = new TFile(filename_MuPR);
  TTree* t_MuPR = (TTree*) f_MuPR->Get("tEvts_sZVeto");

  float nom = getEvents(t,nomcut);
  //std::cout<< "nom : " << nom <<std::endl;

  float ElPR = getEvents(t_ElPR,nomcut);
  //std::cout<< "jesup : " << jesup <<std::endl;
  float ElPRPct = 1*(ElPR-nom)/nom;

  float MuPR = getEvents(t_MuPR,nomcut);
  //std::cout<< "jesdown : " << jesdown <<std::endl;
  float MuPRPct = 1*(MuPR-nom)/nom;

  //std::cout<<setprecision(3);
  std::cout<<sample<<" |  "<<1+ElPRPct<<" |  "<<1+MuPRPct<< std::endl;
  
  //check in NaN = 0 
  if(ElPRPct!=ElPRPct)ElPRPct=0;
  if(MuPRPct!=MuPRPct)MuPRPct=0;

  filecontent<<"ElPR['"<<sample << "']="<<1+ElPRPct<< "\n";
  filecontent<<"MuPR['"<<sample << "']="<<1+MuPRPct<< "\n";

  delete t; delete f;

}



void pr_jess(){

  TString elID = "MVA2017TightV2IsoTightRC";
  TString muID = "CBTightMiniIsoTight";
  TString era = "2018A-D";

  ///----------NP Bkg-------

  TString NPfolder = "/eos/uscms/store/user/wywong/FWLJMET102X_2lep2018_wywong_082020_hadds_postFWLJMETanalyzer_IsoTrig_IDSFunc/";

  std::ofstream outfile;
  outfile.open("unc_PR_NPBkg.py");

  std::stringstream filecontent;
  //filecontent<<std::fixed<<std::setprecision(3);
  
  filecontent<< "ElPR = {} \n";
  filecontent<< "MuPR = {} \n";

  std::cout<<" ==========MC Bkg==========="<<std::endl;
  std::cout<<" Sample | ElPRunc | MuPRunc "<<std::endl;

  printLineNPBkg("NP",NPfolder+"/NonPromptData_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
  //printLineNPBkg("ElPR",NPfolder+"/NonPromptData_Mu"+muID+"_El"+elID+"_"+era+"_UnityElPR.root",filecontent);
  //printLineNPBkg("MuPR",NPfolder+"/NonPromptData_Mu"+muID+"_El"+elID+"_"+era+"_UnityMuPR.root",filecontent);

  outfile<<filecontent.str();
  outfile.close();

  ///----------MC Bkg------- done
  
  //system("cat unc_PR_NPBkg.py unc_PR_TT_bW0p5tZ0p25tH0p25.py > unc_PR_NPBkg_TT_bW0p5tZ0p25tH0p25.py");
  //system("cat unc_PR_NPBkg.py unc_PR_TT_bW0p0tZ0p5tH0p5.py > unc_PR_NPBkg_TT_bW0p0tZ0p5tH0p5.py");
  //system("cat unc_PR_NPBkg.py unc_PR_TT_bW0p0tZ1p0tH0p0.py > unc_PR_NPBkg_TT_bW0p0tZ1p0tH0p0.py");

  
  gApplication->Terminate();

}
