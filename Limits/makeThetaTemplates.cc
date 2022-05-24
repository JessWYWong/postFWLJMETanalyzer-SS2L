#include <iostream>
#include <iomanip>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include <vector>
#include "TLorentzVector.h"
#include "TString.h"
#include "TChain.h"
#include "../interface/TreeReader.h"
#include <stdio.h>
#include <assert.h>
#include <map>
#include <string>
#include <sstream> 
#include <fstream>
#include "../plugins/Macros.cc"

/* the point of this script is to produce a card file suitable for use with higgs combine tool or theta 
   It needs to take in three arguments: leading lepton pT shift, subleading lepton pT shift, HT shift
   where the default values are 30, 30, and 900 GeV (i.e. those of 2012 analysis) */

int main(int argc, char* argv[]){
  //set errors
  TH1::SetDefaultSumw2();
  //debug, set to true by hand until sure script is working
  bool debug_ = true;


  //check ot make sure enough arguments have been passed
  if(argc<3){
    std::cout<<"Need to supply at least 2 arguments: X53 Mass and Chirality! Exiting....."<<std::endl;
    return 0;
  }

  //get arguments
  std::istringstream arg1(argv[1]);
  float mass=0;
  if(!(arg1>>mass)){ std::cout<<"Invalid number for X53 mass! Exiting..."<<std::endl; return 0;}
  else{arg1>>mass;}
  std::string chirality(argv[2]);
  if( chirality.find("RH")==std::string::npos && chirality.find("LH")==std::string::npos) {std::cout<<"Invalid chirality choice! Choose either \'RH\' or \'LH\'. Exiting...."<<std::endl; return 0;}


  //set cuts by hand
  float lep1cut = 40.0;  
  float lep2cut = 30.0;
  std::istringstream arg4(argv[4]);
  float lep2shift=0;
  arg4>>lep2shift;
  lep2cut = lep2cut+ lep2shift;
  std::istringstream arg3(argv[3]);
  float HTshift=0;
  if(!(arg3>>HTshift)){ std::cout<<"Invalid number for HT shift! Exiting..."<<std::endl; return 0;}
  else{arg3>>HTshift;}
  float HTcut   = 900.0+HTshift;

  //get desired lumi
  float lumi = std::atof(argv[5]); // fb^{-1}
  //get era
  std::string era(argv[6]);
  /*  std::istringstream arg4(argv[4]);
  int nConstShift=0;
  if(!(arg4>>nConstShift)){ std::cout<<"Invalid number for nConst shift! Exiting..."<<std::endl; return 0;}
  else{arg4>>nConstShift;}

  int nConst = 5+nConstShift;*/
  int nConst = 5;
  
  //first get our favorite vectors of samples
  std::vector<Sample*> vMCBkg = getMCBkgSampleVec("sZVeto",lumi,"MVA2016TightRC","CBTightMiniIsoTight",era);
  std::vector<Sample*> vDDBkg = getDDBkgSampleVec("sZVeto",lumi,"MVA2016TightRC","CBTightMiniIsoTight",era);
  std::vector<Sample*> vBkg = appendSampleVectors(vMCBkg,vDDBkg);
  std::vector<Sample*> vSig = getInclusiveSigSampleVecForTable("sZVeto",lumi,"MVA2016TightRC","CBTightMiniIsoTight",era);
  Sample* dataSample = getDataSample("sZVeto","MVA2016TightRC","CBTightMiniIsoTight",era);
  //now get only the signal one we care about, should be enough to ensure that both mass and chirality are present in name;
  Sample* sigSample=0;
  //convert mass to string...probably a better way exists
  std::stringstream mss; mss<<mass;
  std::string mstring = mss.str();
  for(std::vector<Sample*>::size_type i=0; i< vSig.size(); i++){
    if( vSig.at(i)->name.find(mstring)!=std::string::npos && vSig.at(i)->name.find(chirality)!=std::string::npos){
      sigSample = vSig.at(i);
      break;
    }
  }

  //make sure we got the sig sample correctly
  if(!sigSample){std::cout<<"Couldn't find the signal sample to use! Exiting....."<<std::endl; return 0;}

  //check to make sure we got the right one
  if(debug_) std::cout<<"Samples name is: "<<sigSample->name<<std::endl;


  std::stringstream cutSStream_elel;
  std::stringstream cutSStream_elmu;
  std::stringstream cutSStream_mumu;
  std::string weightstring = "PUWeight * IDSF * IsoSF * trigSF * GsfSF * MCWeight * ChargeMisIDWeight * NPWeight * prefireWeight* (";
  if(sigSample) weightstring = "pdfWeights4LHC[0] * "+weightstring;

  cutSStream_elel<<weightstring<<" ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1)) && (DilepMass>20) && (nConst>="<<nConst<<" ) && (Lep1Pt >"<<lep1cut<<") && (Lep2Pt > "<<lep2cut<<")"<<" && (Channel==0) )";
  cutSStream_elmu<<weightstring<<" ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1)) && (DilepMass>20) && (nConst>="<<nConst<<" ) && (Lep1Pt >"<<lep1cut<<") && (Lep2Pt > "<<lep2cut<<")"<<" && (Channel==1) )";
  cutSStream_mumu<<weightstring<<" ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1)) && (DilepMass>20) && (nConst>="<<nConst<<" ) && (Lep1Pt >"<<lep1cut<<") && (Lep2Pt > "<<lep2cut<<")"<<" && (Channel==2) )";
  std::string cutString_elel = cutSStream_elel.str();
  std::string cutString_elmu = cutSStream_elmu.str();
  std::string cutString_mumu = cutSStream_mumu.str();
  
  if(debug_) std::cout<<"Cutstring is: "<<cutSStream_elel.str()<<std::endl;

  //output root file
  std::stringstream rootfilename_all;
  rootfilename_all<<"ThetaTemplates_M"<<mass<<"_"<<chirality<<"_Ch_All_LL"<<lep1cut<<"_SL"<<lep2cut<<"_nConst"<<nConst;
  rootfilename_all<<"_"<<era<<".root";

  std::stringstream rootfilename_elel;
  rootfilename_elel<<"ThetaTemplates_M"<<mass<<"_"<<chirality<<"_Ch_ee_LL"<<lep1cut<<"_SL"<<lep2cut<<"_nConst"<<nConst;
  rootfilename_elel<<"_"<<era<<".root";

  std::stringstream rootfilename_elmu;
  rootfilename_elmu<<"ThetaTemplates_M"<<mass<<"_"<<chirality<<"_Ch_emu_LL"<<lep1cut<<"_SL"<<lep2cut<<"_nConst"<<nConst;
  rootfilename_elmu<<"_"<<era<<".root";

  std::stringstream rootfilename_mumu;
  rootfilename_mumu<<"ThetaTemplates_M"<<mass<<"_"<<chirality<<"_Ch_mumu_LL"<<lep1cut<<"_SL"<<lep2cut<<"_nConst"<<nConst;
  rootfilename_mumu<<"_"<<era<<".root";

  TFile* fout = new TFile((rootfilename_all.str()).c_str(),"RECREATE");
  TFile* fout_elel = new TFile((rootfilename_elel.str()).c_str(),"RECREATE");
  TFile* fout_elmu = new TFile((rootfilename_elmu.str()).c_str(),"RECREATE");
  TFile* fout_mumu = new TFile((rootfilename_mumu.str()).c_str(),"RECREATE");
 
  //define ht bins
  float htbins[14] = {120,240,360,480,600,720,840,960,1080,1200,1320,1500,2000,3000};
  
  //write observed
  TString erast="";
  if(era=="2016B-D") erast="BD";
  else erast="EH";
		       
  //TH1F* All__DATA = new TH1F("All__DATA","",3,0,3);  
  TH1F* elel__DATA = new TH1F("elel"+erast+"__DATA","",13,htbins);
  TH1F* elmu__DATA = new TH1F("elmu"+erast+"__DATA","",13,htbins);
  TH1F* mumu__DATA = new TH1F("mumu"+erast+"__DATA","",13,htbins);

  TTree* tData = dataSample->tree;
  tData->Project("elel"+erast+"__DATA","cleanAK4HT",cutString_elel.c_str());
  tData->Project("elmu"+erast+"__DATA","cleanAK4HT",cutString_elmu.c_str());
  tData->Project("mumu"+erast+"__DATA","cleanAK4HT",cutString_mumu.c_str());


  //fout->WriteTObject(all__DATA);
  fout->WriteTObject(elel__DATA);
  fout_elel->WriteTObject(elel__DATA);

  fout->WriteTObject(elmu__DATA);
  fout_elmu->WriteTObject(elmu__DATA);

  fout->WriteTObject(mumu__DATA);
  fout_mumu->WriteTObject(mumu__DATA);


  TH1F* elel__sig = new TH1F("elel"+erast+"__sig","",13,htbins);
  TTree* tSig = sigSample->tree;
  tSig->Project("elel"+erast+"__sig","cleanAK4HT",cutString_elel.c_str());
  //scale by lumi and then undo x-sec, it's heavy handed, but right
  elel__sig->Scale(sigSample->weight/sigSample->xsec);

  TH1F* elmu__sig = new TH1F("elmu"+erast+"__sig","",13,htbins);
  tSig->Project("elmu"+erast+"__sig","cleanAK4HT",cutString_elmu.c_str());
  //scale by lumi and then undo x-sec, it's heavy handed, but right
  elmu__sig->Scale(sigSample->weight/sigSample->xsec);

  TH1F* mumu__sig = new TH1F("mumu"+erast+"__sig","",13,htbins);
  tSig->Project("mumu"+erast+"__sig","cleanAK4HT",cutString_mumu.c_str());
  //scale by lumi and then undo x-sec, it's heavy handed, but right
  mumu__sig->Scale(sigSample->weight/sigSample->xsec);

  fout->WriteTObject(elel__sig);
  fout_elel->WriteTObject(elel__sig);

  fout->WriteTObject(elmu__sig);
  fout_elmu->WriteTObject(elmu__sig);

  fout->WriteTObject(mumu__sig);
  fout_mumu->WriteTObject(mumu__sig);

  //loop over background samples
  for(unsigned int i=0; i < vBkg.size(); i++){
    Sample* s = vBkg.at(i);
    TTree* t = s->tree;
    TString name_elel = "elel"+erast+"__"+s->name;
    TH1F* h_elel = new TH1F(name_elel,"h",13,htbins);
    t->Project(name_elel,"cleanAK4HT",cutString_elel.c_str());
    //scale MC
    if(s->name.find("NonPrompt")!=std::string::npos || s->name.find("ChargeMisID")!=std::string::npos){}
    else{
      h_elel->Scale(s->weight);
    }
    fout->WriteTObject(h_elel);
    fout_elel->WriteTObject(h_elel);

    TString name_elmu = "elmu"+erast+"__"+s->name;
    TH1F* h_elmu = new TH1F(name_elmu,"h",13,htbins);
    t->Project(name_elmu,"cleanAK4HT",cutString_elmu.c_str());
    //scale MC
    if(s->name.find("NonPrompt")!=std::string::npos || s->name.find("ChargeMisID")!=std::string::npos){}
    else{
      h_elmu->Scale(s->weight);
    }
    fout->WriteTObject(h_elmu);
    fout_elmu->WriteTObject(h_elmu);

    TString name_mumu = "mumu"+erast+"__"+s->name;
    TH1F* h_mumu = new TH1F(name_mumu,"h",13,htbins);
    t->Project(name_mumu,"cleanAK4HT",cutString_mumu.c_str());
    //scale MC
    if(s->name.find("NonPrompt")!=std::string::npos || s->name.find("ChargeMisID")!=std::string::npos){}
    else{
      h_mumu->Scale(s->weight);
    }
    fout->WriteTObject(h_mumu);
    fout_mumu->WriteTObject(h_mumu);
  }

  
  //fout->Write();
  fout->Close();
  fout_elel->Close();
  fout_elmu->Close();
  fout_mumu->Close();


  return 0;
}

