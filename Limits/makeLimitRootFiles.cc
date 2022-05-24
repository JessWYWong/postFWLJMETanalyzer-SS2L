#include <iostream>
#include <iomanip>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include <vector>
#include "TLorentzVector.h"
#include "TChain.h"
#include "../interface/TreeReader.h"
#include <stdio.h>
#include <assert.h>
#include <map>
#include <string>
#include <sstream> 
#include <fstream>
#include "../plugins/Macros.cc"
#include <math.h>  //added by rizki

/* the point of this script is to produce a card file suitable for use with higgs combine tool or theta 
   It needs to take in three arguments: leading lepton pT shift, subleading lepton pT shift, HT shift
   where the default values are 30, 30, and 900 GeV (i.e. those of 2012 analysis) */

int main(int argc, char* argv[]){

  //debug, set to true by hand until sure script is working
  bool debug_ = true;
  std::string elID = "MVA2017TightV2IsoTightRC";
  std::string muID = "CBTightMiniIsoTight";

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
  std::string whichSignal(argv[2]);
  if( whichSignal.find("TT")==std::string::npos && whichSignal.find("BB")==std::string::npos) {std::cout<<"Invalid signal choice! Choose either \'TT\' or \'BB\'. Exiting...."<<std::endl; return 0;}


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
  float HTcut   = 1000.0+HTshift; //edited by rizki

  //get desired lumi
  float lumi = std::atof(argv[5]); // fb^{-1}
  //get era
  std::string era(argv[6]);
  /*  std::istringstream arg4(argv[4]);
  int nConstShift=0;
  if(!(arg4>>nConstShift)){ std::cout<<"Invalid number for nConst shift! Exiting..."<<std::endl; return 0;}
  else{arg4>>nConstShift;}

//   int nConst = 5+nConstShift;*/
  int nConst = 4; // edited by rizki
//   int nConst = 5; // edited by rizki

  //first get our favorite vectors of samples
  std::vector<Sample*> vMCBkg = getMCBkgSampleVec("sZVeto",lumi,elID,muID,era);
  std::vector<Sample*> vDDBkg = getDDBkgSampleVec("sZVeto",lumi,elID,muID,era);
  std::vector<Sample*> vBkg = appendSampleVectors(vMCBkg,vDDBkg);
//   std::vector<Sample*> vSig = getInclusiveSigSampleVecForTable("sZVeto",lumi,elID,muID,era);


  //get BR for signal  
  int whichBR = std::stoi(argv[7]);
  std::string BRstr;
  std::vector<Sample*> vSigSamples1, vSigSamples2, vSigSamples3, vSigSamples4, vSigSamples5, vSigSamples6, vSig;

  if(whichSignal == "TT"){
    if(whichBR==0) BRstr = "bW0p5_tZ0p25_tH0p25";
    if(whichBR==1) BRstr = "bW0p0_tZ1p0_tH0p0";
    if(whichBR==2) BRstr = "bW0p0_tZ0p8_tH0p2";
    if(whichBR==3) BRstr = "bW0p0_tZ0p6_tH0p4";
    if(whichBR==4) BRstr = "bW0p0_tZ0p4_tH0p6";
    if(whichBR==5) BRstr = "bW0p0_tZ0p2_tH0p8";
    if(whichBR==6) BRstr = "bW0p0_tZ0p0_tH0p0";
    if(whichBR==7) BRstr = "bW0p2_tZ0p8_tH0p0";
    if(whichBR==8) BRstr = "bW0p2_tZ0p6_tH0p2";
    if(whichBR==9) BRstr = "bW0p2_tZ0p4_tH0p4";
    if(whichBR==10) BRstr = "bW0p2_tZ0p2_tH0p6";
    if(whichBR==11) BRstr = "bW0p2_tZ0p0_tH0p8";
    if(whichBR==12) BRstr = "bW0p4_tZ0p6_tH0p0";
    if(whichBR==13) BRstr = "bW0p4_tZ0p4_tH0p2";
    if(whichBR==14) BRstr = "bW0p4_tZ0p2_tH0p4";
    if(whichBR==15) BRstr = "bW0p4_tZ0p0_tH0p6";
    if(whichBR==16) BRstr = "bW0p6_tZ0p4_tH0p0";
    if(whichBR==17) BRstr = "bW0p6_tZ0p2_tH0p2";
    if(whichBR==18) BRstr = "bW0p6_tZ0p0_tH0p4";
    if(whichBR==19) BRstr = "bW0p8_tZ0p2_tH0p0";
    if(whichBR==20) BRstr = "bW0p8_tZ0p0_tH0p2";
    if(whichBR==21) BRstr = "bW1p0_tZ0p0_tH0p0";
    if(whichBR==22) BRstr = "bW0p0_tZ0p5_tH0p5";

    //TT SIGNAL <decay> - RIZKI 
    //int whichBR = 1 ; //0 = singlet, 1 = doublet, 2 = tZ 100%
    vSigSamples1 = getInclusiveSigTTSampleVecForTable("sZVeto", lumi,elID,muID,era,"BWBW",whichBR);
    vSigSamples2 = getInclusiveSigTTSampleVecForTable("sZVeto", lumi,elID,muID,era,"THBW",whichBR);
    std::vector<Sample*> vSigSamples3 = getInclusiveSigTTSampleVecForTable("sZVeto", lumi,elID,muID,era,"THTH",whichBR);
    std::vector<Sample*> vSigSamples4 = getInclusiveSigTTSampleVecForTable("sZVeto", lumi,elID,muID,era,"TZBW",whichBR);
    std::vector<Sample*> vSigSamples5 = getInclusiveSigTTSampleVecForTable("sZVeto", lumi,elID,muID,era,"TZTH",whichBR);
    std::vector<Sample*> vSigSamples6 = getInclusiveSigTTSampleVecForTable("sZVeto", lumi,elID,muID,era,"TZTZ",whichBR);
    std::vector<Sample*> vSig = appendSampleVectors(vSigSamples1,vSigSamples2,vSigSamples3,vSigSamples4,vSigSamples5,vSigSamples6);
  }
  else{
    if(whichBR==0) BRstr = "tW0p5_bZ0p25_bH0p25";
    if(whichBR==1) BRstr = "tW0p0_bZ1p0_bH0p0";
    if(whichBR==2) BRstr = "tW0p0_bZ0p8_bH0p2";
    if(whichBR==3) BRstr = "tW0p0_bZ0p6_bH0p4";
    if(whichBR==4) BRstr = "tW0p0_bZ0p4_bH0p6";
    if(whichBR==5) BRstr = "tW0p0_bZ0p2_bH0p8";
    if(whichBR==6) BRstr = "tW0p0_bZ0p0_bH0p0";
    if(whichBR==7) BRstr = "tW0p2_bZ0p8_bH0p0";
    if(whichBR==8) BRstr = "tW0p2_bZ0p6_bH0p2";
    if(whichBR==9) BRstr = "tW0p2_bZ0p4_bH0p4";
    if(whichBR==10) BRstr = "tW0p2_bZ0p2_bH0p6";
    if(whichBR==11) BRstr = "tW0p2_bZ0p0_bH0p8";
    if(whichBR==12) BRstr = "tW0p4_bZ0p6_bH0p0";
    if(whichBR==13) BRstr = "tW0p4_bZ0p4_bH0p2";
    if(whichBR==14) BRstr = "tW0p4_bZ0p2_bH0p4";
    if(whichBR==15) BRstr = "tW0p4_bZ0p0_bH0p6";
    if(whichBR==16) BRstr = "tW0p6_bZ0p4_bH0p0";
    if(whichBR==17) BRstr = "tW0p6_bZ0p2_bH0p2";
    if(whichBR==18) BRstr = "tW0p6_bZ0p0_bH0p4";
    if(whichBR==19) BRstr = "tW0p8_bZ0p2_bH0p0";
    if(whichBR==20) BRstr = "tW0p8_bZ0p0_bH0p2";
    if(whichBR==21) BRstr = "tW1p0_bZ0p0_bH0p0";
    if(whichBR==22) BRstr = "tW0p0_bZ0p5_bH0p5";
    //BB SIGNAL <decay> - RIZKI 
    //int whichBR = 0 ; //0 = singlet, 1 = doublet, 2 = tW 100%
    std::vector<Sample*> vSigSamples1 = getInclusiveSigBBSampleVecForTable("sZVeto", lumi,elID,muID,era,"TWTW",whichBR);
    std::vector<Sample*> vSigSamples2 = getInclusiveSigBBSampleVecForTable("sZVeto", lumi,elID,muID,era,"BHTW",whichBR);
    std::vector<Sample*> vSigSamples3 = getInclusiveSigBBSampleVecForTable("sZVeto", lumi,elID,muID,era,"BHBH",whichBR);
    std::vector<Sample*> vSigSamples4 = getInclusiveSigBBSampleVecForTable("sZVeto", lumi,elID,muID,era,"BZTW",whichBR);
    std::vector<Sample*> vSigSamples5 = getInclusiveSigBBSampleVecForTable("sZVeto", lumi,elID,muID,era,"BZBH",whichBR);
    std::vector<Sample*> vSigSamples6 = getInclusiveSigBBSampleVecForTable("sZVeto", lumi,elID,muID,era,"BZBZ",whichBR);
    std::vector<Sample*> vSig = appendSampleVectors(vSigSamples1,vSigSamples2,vSigSamples3,vSigSamples4,vSigSamples5,vSigSamples6);
  }

  Sample* dataSample = getDataSample("sZVeto",elID,muID,era);

  //now get only the signal one we care about, should be enough to ensure that both mass and chirality are present in name;
  std::vector<Sample*> sigSample; //edited by rizki and everything to do with this.
  //convert mass to string...probably a better way exists
  std::stringstream mss; mss<<mass;
  std::string mstring = mss.str();
  for(std::vector<Sample*>::size_type i=0; i< vSig.size(); i++){
    //if( vSig.at(i)->name.find(mstring)!=std::string::npos && vSig.at(i)->name.find(chirality)!=std::string::npos){
    if( vSig.at(i)->name.find(mstring)!=std::string::npos){
      sigSample.push_back(vSig.at(i));
      //break;
    }
  }

  //make sure we got the sig sample correctly
  //if(!sigSample){std::cout<<"Couldn't find the signal sample to use! Exiting....."<<std::endl; return 0;}


  //now make cut string according to cuts specified:
  std::vector<std::string> vCutString;
  std::stringstream cutSStream;
//   cutSStream<<" ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1)) && (DilepMass>20) && (nConst>="<<nConst<<" ) && (Lep1Pt >"<<lep1cut<<") && (Lep2Pt > "<<lep2cut<<") && ( cleanAK4HT > "<<HTcut<<")";
  cutSStream<<" ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1)) && (DilepMass>20) && (nConst>="<<nConst<<" ) && (Lep1Pt >"<<lep1cut<<") && (Lep2Pt > "<<lep2cut<<") && ( cleanAK4HT > "<<HTcut<<") && ( nNonSSLeps == 0)"; //added by rizki, add exactly 2 leptons (SS) only.

  vCutString.push_back(cutSStream.str());

  if(debug_) std::cout<<"Cutstring is: "<<cutSStream.str()<<std::endl;

  //output root file
  std::stringstream rootfilename_all;
//   rootfilename_all<<"Limits_M"<<mass<<"_"<<chirality<<"_Ch_All_LL"<<lep1cut<<"_SL"<<lep2cut<<"_HT"<<HTcut<<"_nConst"<<nConst;
  rootfilename_all<<"Limits_"+whichSignal+"M"<<mass<<"_"<<BRstr<<"_Ch_All_LL"<<lep1cut<<"_SL"<<lep2cut<<"_HT"<<HTcut<<"_nConst"<<nConst;
  rootfilename_all<<"_"<<era<<".root";

  std::stringstream rootfilename_elel;
//   rootfilename_elel<<"Limits_M"<<mass<<"_"<<chirality<<"_Ch_ee_LL"<<lep1cut<<"_SL"<<lep2cut<<"_HT"<<HTcut<<"_nConst"<<nConst;
  rootfilename_elel<<"Limits_"+whichSignal+"M"<<mass<<"_"<<BRstr<<"_Ch_ee_LL"<<lep1cut<<"_SL"<<lep2cut<<"_HT"<<HTcut<<"_nConst"<<nConst;
  rootfilename_elel<<"_"<<era<<".root";

  std::stringstream rootfilename_elmu;
//   rootfilename_elmu<<"Limits_M"<<mass<<"_"<<chirality<<"_Ch_emu_LL"<<lep1cut<<"_SL"<<lep2cut<<"_HT"<<HTcut<<"_nConst"<<nConst;
  rootfilename_elmu<<"Limits_"+whichSignal+"M"<<mass<<"_"<<BRstr<<"_Ch_emu_LL"<<lep1cut<<"_SL"<<lep2cut<<"_HT"<<HTcut<<"_nConst"<<nConst;
  rootfilename_elmu<<"_"<<era<<".root";

  std::stringstream rootfilename_mumu;
//   rootfilename_mumu<<"Limits_M"<<mass<<"_"<<chirality<<"_Ch_mumu_LL"<<lep1cut<<"_SL"<<lep2cut<<"_HT"<<HTcut<<"_nConst"<<nConst;
  rootfilename_mumu<<"Limits_"+whichSignal+"M"<<mass<<"_"<<BRstr<<"_Ch_mumu_LL"<<lep1cut<<"_SL"<<lep2cut<<"_HT"<<HTcut<<"_nConst"<<nConst;
  rootfilename_mumu<<"_"<<era<<".root";

  TFile* fout = new TFile((rootfilename_all.str()).c_str(),"RECREATE");
  TFile* fout_elel = new TFile((rootfilename_elel.str()).c_str(),"RECREATE");
  TFile* fout_elmu = new TFile((rootfilename_elmu.str()).c_str(),"RECREATE");
  TFile* fout_mumu = new TFile((rootfilename_mumu.str()).c_str(),"RECREATE");

  //write observed
  TString erast="";
  if(era.find("2018A-D")!=std::string::npos) erast="2018AD";
  else erast="UnknownEra";
		       
  //TH1F* All__DATA = new TH1F("All__DATA","",3,0,3);
  TH1F* elel__DATA = new TH1F("elel"+erast+"__DATA","",1,0,1);
  TH1F* elmu__DATA = new TH1F("elmu"+erast+"__DATA","",1,0,1);
  TH1F* mumu__DATA = new TH1F("mumu"+erast+"__DATA","",1,0,1);

  //CutClass* cutData = makeCutClass(dataSample,vCutString,-1);
  //int nData = (cutData->nEvents).at(0);
  CutClass* cutData0 = makeCutClass(dataSample,vCutString,0);
  int nData0 = (cutData0->nEvents).at(0);
  CutClass* cutData1 = makeCutClass(dataSample,vCutString,1);
  int nData1 = (cutData1->nEvents).at(0);
  CutClass* cutData2 = makeCutClass(dataSample,vCutString,2);
  int nData2 = (cutData2->nEvents).at(0);
  
  
  //for(int i=0; i<nData; i++){
  //all__DATA->Fill(0.5);
  //}
  for(int i=0; i<nData0; i++){
    elel__DATA->Fill(0.5);
  }
  for(int i=0; i<nData1; i++){
    elmu__DATA->Fill(0.5);
  }
  for(int i=0; i<nData2; i++){
    mumu__DATA->Fill(0.5);
  }


  //fout->WriteTObject(all__DATA);
  fout->WriteTObject(elel__DATA);
  fout_elel->WriteTObject(elel__DATA);

  fout->WriteTObject(elmu__DATA);
  fout_elmu->WriteTObject(elmu__DATA);

  fout->WriteTObject(mumu__DATA);
  fout_mumu->WriteTObject(mumu__DATA);



  //histos for signal
  //TH1F* All_sig = new TH1F("All_sig","",3,0,3);
  //TH1F* elel_sig = new TH1F("elel_sig","",1,0,1);
  //TH1F* elmu_sig = new TH1F("elmu_sig","",1,0,1);
  //TH1F* mumu_sig = new TH1F("mumu_sig","",1,0,1);
  //float all_sig = cutSig_all->nEvents.at(0) / cutSig->xsec;
  //float errAll_sig = cutSig_all->vErr.at(0);
  //All_sig->SetBinContent(1,all_sig); All_sig->SetBinError(1,errAll_sig);

  TH1F* elel__sig = new TH1F("elel"+erast+"__sig","",1,0,1);
  TH1F* elmu__sig = new TH1F("elmu"+erast+"__sig","",1,0,1);
  TH1F* mumu__sig = new TH1F("mumu"+erast+"__sig","",1,0,1);


  float nSig_elel = 0;
  float errElel_sig = 0;
  float nSig_elmu = 0;
  float errElmu_sig = 0;
  float nSig_mumu = 0;
  float errMumu_sig = 0;
  
  //need to loop for combining all TT decays -rizki
  for(unsigned int i=0; i<sigSample.size();i++){

	  //check to make sure we got the right one
	  if(debug_) std::cout<<"Samples name is: "<<sigSample.at(i)->name<<std::endl;

	  //get cut class for signal
	  //CutClass* cutSig_all = makeCutClass(sigSample,vCutString,-1);
	  CutClass* cutSig_elel = makeCutClass(sigSample.at(i),vCutString,0);
	  CutClass* cutSig_elmu = makeCutClass(sigSample.at(i),vCutString,1);
	  CutClass* cutSig_mumu = makeCutClass(sigSample.at(i),vCutString,2);


	  nSig_elel = nSig_elel + ( cutSig_elel->nEvents.at(0) / cutSig_elel->xsec );
	  errElel_sig = errElel_sig + ( cutSig_elel->vErr.at(0) * cutSig_elel->vErr.at(0) ); //add errors in quadrature
	  if(debug_) std::cout<<"	events	:"<<( cutSig_elel->nEvents.at(0) / cutSig_elel->xsec )<< " +/- "<< cutSig_elel->vErr.at(0) <<std::endl;
	  if(debug_) std::cout<<"						total	:"<< nSig_elel << " +/- "<< sqrt(errElel_sig) << std::endl;

	  nSig_elmu = nSig_elmu + ( cutSig_elmu->nEvents.at(0) / cutSig_elmu->xsec );
	  errElmu_sig = errElmu_sig + ( cutSig_elmu->vErr.at(0) * cutSig_elmu->vErr.at(0) ); //add errors in quadrature
	  if(debug_) std::cout<<"	events	:"<<( cutSig_elmu->nEvents.at(0) / cutSig_elmu->xsec )<< " +/- "<< cutSig_elmu->vErr.at(0) <<std::endl;
	  if(debug_) std::cout<<"						total	:"<< nSig_elmu << " +/- "<< sqrt(errElmu_sig) << std::endl;

	  nSig_mumu = nSig_mumu + ( cutSig_mumu->nEvents.at(0) / cutSig_mumu->xsec );
	  errMumu_sig = errMumu_sig + ( cutSig_mumu->vErr.at(0) * cutSig_mumu->vErr.at(0) ); //add errors in quadrature
	  if(debug_) std::cout<<"	events	:"<<( cutSig_mumu->nEvents.at(0) / cutSig_mumu->xsec )<< " +/- "<< cutSig_mumu->vErr.at(0) <<std::endl;
	  if(debug_) std::cout<<"						total	:"<< nSig_mumu << " +/- "<< sqrt(errMumu_sig) << std::endl;
  }

  elel__sig->SetBinContent(1,nSig_elel); elel__sig->SetBinError(1,sqrt(errElel_sig));
  elmu__sig->SetBinContent(1,nSig_elmu); elmu__sig->SetBinError(1,sqrt(errElmu_sig));
  mumu__sig->SetBinContent(1,nSig_mumu); mumu__sig->SetBinError(1,sqrt(errMumu_sig));


  //fout->WriteTObject(all_sig);
  fout->WriteTObject(elel__sig);
  fout_elel->WriteTObject(elel__sig);

  fout->WriteTObject(elmu__sig);
  fout_elmu->WriteTObject(elmu__sig);

  fout->WriteTObject(mumu__sig);
  fout_mumu->WriteTObject(mumu__sig);


  //get cut class vector for background
  //std::vector<CutClass*> vCutBkg_all  = getCutClassVector(vBkg,vCutString,-1);
  std::vector<CutClass*> vCutBkg_elel = getCutClassVector(vBkg,vCutString,0);
  std::vector<CutClass*> vCutBkg_elmu = getCutClassVector(vBkg,vCutString,1);
  std::vector<CutClass*> vCutBkg_mumu = getCutClassVector(vBkg,vCutString,2);


  //variables for background yields and stat errors
  float nFakeRate0=0;  float nFakeRate1=0;  float nFakeRate2=0;
  float errFakeRate0=0;  float errFakeRate1=0;  float errFakeRate2=0;
  float nChargeMisID0=0;  float nChargeMisID1=0;  float nChargeMisID2=0;
  float errChargeMisID0=0;  float errChargeMisID1=0;  float errChargeMisID2=0;

  //need one histogram per process which has it's own unique systematic values
  float nTTW0=0;  float nTTW1=0;  float nTTW2=0;
  float errTTW0=0;  float errTTW1=0;  float errTTW2=0;

  float nTTZ0=0;  float nTTZ1=0;  float nTTZ2=0;
  float errTTZ0=0;  float errTTZ1=0;  float errTTZ2=0;

  float nTTH0=0;  float nTTH1=0;  float nTTH2=0;
  float errTTH0=0;  float errTTH1=0;  float errTTH2=0;

  float nTTTT0=0;  float nTTTT1=0;  float nTTTT2=0;
  float errTTTT0=0;  float errTTTT1=0;  float errTTTT2=0;

  float nWZ0=0;  float nWZ1=0;  float nWZ2=0;
  float errWZ0=0;  float errWZ1=0;  float errWZ2=0;

  float nZZ0=0;  float nZZ1=0;  float nZZ2=0;
  float errZZ0=0;  float errZZ1=0;  float errZZ2=0;

  float nWpWp0=0;  float nWpWp1=0;  float nWpWp2=0;
  float errWpWp0=0;  float errWpWp1=0;  float errWpWp2=0;

  float nWWZ0=0;  float nWWZ1=0;  float nWWZ2=0;
  float errWWZ0=0;  float errWWZ1=0;  float errWWZ2=0;

  float nWZZ0=0;  float nWZZ1=0;  float nWZZ2=0;
  float errWZZ0=0;  float errWZZ1=0;  float errWZZ2=0;

  //float nZZZ0=0;  float nZZZ1=0;  float nZZZ2=0;
  //float errZZZ0=0;  float errZZZ1=0;  float errZZZ2=0;


  for(std::vector<CutClass*>::size_type i =0; i< vCutBkg_elel.size(); i++){
    if(vCutBkg_elel.at(i)->samplename=="NonPrompt"){
      nFakeRate0 = vCutBkg_elel.at(i)->nEvents.at(0);
      errFakeRate0 = vCutBkg_elel.at(i)->vErr.at(0);
    }
    if(vCutBkg_elel.at(i)->samplename=="ChargeMisID"){
      nChargeMisID0 = vCutBkg_elel.at(i)->nEvents.at(0);
      errChargeMisID0 = vCutBkg_elel.at(i)->vErr.at(0);
    }
    if(vCutBkg_elel.at(i)->samplename=="TTW"){
      nTTW0 = vCutBkg_elel.at(i)->nEvents.at(0);
      errTTW0 = vCutBkg_elel.at(i)->vErr.at(0);
    }
    if(vCutBkg_elel.at(i)->samplename=="TTZ"){
      nTTZ0 = vCutBkg_elel.at(i)->nEvents.at(0);
      errTTZ0 = vCutBkg_elel.at(i)->vErr.at(0);
    }
    if(vCutBkg_elel.at(i)->samplename=="TTH"){
      nTTH0 = vCutBkg_elel.at(i)->nEvents.at(0);
      errTTH0 = vCutBkg_elel.at(i)->vErr.at(0);
    }
    if(vCutBkg_elel.at(i)->samplename=="TTTT"){
      nTTTT0 = vCutBkg_elel.at(i)->nEvents.at(0);
      errTTTT0 = vCutBkg_elel.at(i)->vErr.at(0);
    }
    if(vCutBkg_elel.at(i)->samplename=="WZ"){
      nWZ0 = vCutBkg_elel.at(i)->nEvents.at(0);
      errWZ0 = vCutBkg_elel.at(i)->vErr.at(0);
    }
    if(vCutBkg_elel.at(i)->samplename=="ZZ"){
      nZZ0 = vCutBkg_elel.at(i)->nEvents.at(0);
      errZZ0 = vCutBkg_elel.at(i)->vErr.at(0);
    }
    if(vCutBkg_elel.at(i)->samplename=="WpWp"){
      nWpWp0 = vCutBkg_elel.at(i)->nEvents.at(0);
      errWpWp0 = vCutBkg_elel.at(i)->vErr.at(0);
    }
    if(vCutBkg_elel.at(i)->samplename=="WWZ"){
      nWWZ0 = vCutBkg_elel.at(i)->nEvents.at(0);
      errWWZ0 = vCutBkg_elel.at(i)->vErr.at(0);
    }
    if(vCutBkg_elel.at(i)->samplename=="WZZ"){
      nWZZ0 = vCutBkg_elel.at(i)->nEvents.at(0);
      errWZZ0 = vCutBkg_elel.at(i)->vErr.at(0);
    }
    //if(vCutBkg_elel.at(i)->samplename=="ZZZ"){
    //nZZZ0 = vCutBkg_elel.at(i)->nEvents.at(0);
    //errZZZ0 = vCutBkg_elel.at(i)->vErr.at(0);
    //}
  }//end loop over elel bkg vector

  for(std::vector<CutClass*>::size_type i =0; i< vCutBkg_elmu.size(); i++){
    if(vCutBkg_elmu.at(i)->samplename=="NonPrompt"){
      nFakeRate1 = vCutBkg_elmu.at(i)->nEvents.at(0);
      errFakeRate1 = vCutBkg_elmu.at(i)->vErr.at(0);
    }
    if(vCutBkg_elmu.at(i)->samplename=="ChargeMisID"){
      nChargeMisID1 = vCutBkg_elmu.at(i)->nEvents.at(0);
      errChargeMisID1 = vCutBkg_elmu.at(i)->vErr.at(0);
    }
    if(vCutBkg_elmu.at(i)->samplename=="TTW"){
      nTTW1 = vCutBkg_elmu.at(i)->nEvents.at(0);
      errTTW1 = vCutBkg_elmu.at(i)->vErr.at(0);
    }
    if(vCutBkg_elmu.at(i)->samplename=="TTZ"){
      nTTZ1 = vCutBkg_elmu.at(i)->nEvents.at(0);
      errTTZ1 = vCutBkg_elmu.at(i)->vErr.at(0);
    }
    if(vCutBkg_elmu.at(i)->samplename=="TTH"){
      nTTH1 = vCutBkg_elmu.at(i)->nEvents.at(0);
      errTTH1 = vCutBkg_elmu.at(i)->vErr.at(0);
    }
    if(vCutBkg_elmu.at(i)->samplename=="TTTT"){
      nTTTT1 = vCutBkg_elmu.at(i)->nEvents.at(0);
      errTTTT1 = vCutBkg_elmu.at(i)->vErr.at(0);
    }
    if(vCutBkg_elmu.at(i)->samplename=="WZ"){
      nWZ1 = vCutBkg_elmu.at(i)->nEvents.at(0);
      errWZ1 = vCutBkg_elmu.at(i)->vErr.at(0);
    }
    if(vCutBkg_elmu.at(i)->samplename=="ZZ"){
      nZZ1 = vCutBkg_elmu.at(i)->nEvents.at(0);
      errZZ1 = vCutBkg_elmu.at(i)->vErr.at(0);
    }
    if(vCutBkg_elmu.at(i)->samplename=="WpWp"){
      nWpWp1 = vCutBkg_elmu.at(i)->nEvents.at(0);
      errWpWp1 = vCutBkg_elmu.at(i)->vErr.at(0);
    }
    if(vCutBkg_elmu.at(i)->samplename=="WWZ"){
      nWWZ1 = vCutBkg_elmu.at(i)->nEvents.at(0);
      errWWZ1 = vCutBkg_elmu.at(i)->vErr.at(0);
    }
    if(vCutBkg_elmu.at(i)->samplename=="WZZ"){
      nWZZ1 = vCutBkg_elmu.at(i)->nEvents.at(0);
      errWZZ1 = vCutBkg_elmu.at(i)->vErr.at(0);
    }
    //if(vCutBkg_elmu.at(i)->samplename=="ZZZ"){
    //nZZZ1 = vCutBkg_elmu.at(i)->nEvents.at(0);
    // errZZZ1 = vCutBkg_elmu.at(i)->vErr.at(0);
    //}
  }//end loop over elmu bkg vector

  for(std::vector<CutClass*>::size_type i =0; i< vCutBkg_mumu.size(); i++){
    if(vCutBkg_mumu.at(i)->samplename=="NonPrompt"){
      nFakeRate2 = vCutBkg_mumu.at(i)->nEvents.at(0);
      errFakeRate2 = vCutBkg_mumu.at(i)->vErr.at(0);
    }
    if(vCutBkg_mumu.at(i)->samplename=="ChargeMisID"){
      nChargeMisID2 = vCutBkg_mumu.at(i)->nEvents.at(0);
      errChargeMisID2 = vCutBkg_mumu.at(i)->vErr.at(0);
    }
    if(vCutBkg_mumu.at(i)->samplename=="TTW"){
      nTTW2 = vCutBkg_mumu.at(i)->nEvents.at(0);
      errTTW2 = vCutBkg_mumu.at(i)->vErr.at(0);
    }
    if(vCutBkg_mumu.at(i)->samplename=="TTZ"){
      nTTZ2 = vCutBkg_mumu.at(i)->nEvents.at(0);
      errTTZ2 = vCutBkg_mumu.at(i)->vErr.at(0);
    }
    if(vCutBkg_mumu.at(i)->samplename=="TTH"){
      nTTH2 = vCutBkg_mumu.at(i)->nEvents.at(0);
      errTTH2 = vCutBkg_mumu.at(i)->vErr.at(0);
    }
    if(vCutBkg_mumu.at(i)->samplename=="TTTT"){
      nTTTT2 = vCutBkg_mumu.at(i)->nEvents.at(0);
      errTTTT2 = vCutBkg_mumu.at(i)->vErr.at(0);
    }
    if(vCutBkg_mumu.at(i)->samplename=="WZ"){
      nWZ2 = vCutBkg_mumu.at(i)->nEvents.at(0);
      errWZ2 = vCutBkg_mumu.at(i)->vErr.at(0);
    }
    if(vCutBkg_mumu.at(i)->samplename=="ZZ"){
      nZZ2 = vCutBkg_mumu.at(i)->nEvents.at(0);
      errZZ2 = vCutBkg_mumu.at(i)->vErr.at(0);
    }
    if(vCutBkg_mumu.at(i)->samplename=="WpWp"){
      nWpWp2 = vCutBkg_mumu.at(i)->nEvents.at(0);
      errWpWp2 = vCutBkg_mumu.at(i)->vErr.at(0);
    }
    if(vCutBkg_mumu.at(i)->samplename=="WWZ"){
      nWWZ2 = vCutBkg_mumu.at(i)->nEvents.at(0);
      errWWZ2 = vCutBkg_mumu.at(i)->vErr.at(0);
    }
    if(vCutBkg_mumu.at(i)->samplename=="WZZ"){
      nWZZ2 = vCutBkg_mumu.at(i)->nEvents.at(0);
      errWZZ2 = vCutBkg_mumu.at(i)->vErr.at(0);
    }
    //if(vCutBkg_mumu.at(i)->samplename=="ZZZ"){
    //nZZZ2 = vCutBkg_mumu.at(i)->nEvents.at(0);
    //errZZZ2 = vCutBkg_mumu.at(i)->vErr.at(0);
    //}
  }//end loop over mumu bkg vector

  /* for(std::vector<CutClass*>::size_type i =0; i< vCutBkg_elmu.size(); i++){
    if(vCutBkg.at(i)->samplename=="NonPrompt"){
      nFakeRate1 = vCutBkg.at(i)->nEvents.at(0);
      errFakeRate1 = vCutBkg.at(i)->vErr.at(0);
    }
    else if(vCutBkg.at(i)->samplename=="ChargeMisID"){
      nChargeMisID1 = vCutBkg.at(i)->nEvents.at(0);
      errChargeMisID1 = vCutBkg.at(i)->vErr.at(0);
    }

    else{
      nPSSMC1 = nPSSMC1 + vCutBkg.at(i)->nEvents.at(0);
      errPSSMC1 = errPSSMC1 + pow(vCutBkg.at(i)->vErr.at(0),2); //add in quadrature, take square root later
    }
  }//end loop over elel bkg vector

  //take square root for error on PSSMC
  errPSSMC1 = pow(errPSSMC1,0.5);

  for(std::vector<CutClass*>::size_type i =0; i< vCutBkg_mumu.size(); i++){
    if(vCutBkg.at(i)->samplename=="NonPrompt"){
      nFakeRate2 = vCutBkg.at(i)->nEvents.at(0);
      errFakeRate2 = vCutBkg.at(i)->vErr.at(0);
    }
    else if(vCutBkg.at(i)->samplename=="ChargeMisID"){
      nChargeMisID2 = vCutBkg.at(i)->nEvents.at(0);
      errChargeMisID2 = vCutBkg.at(i)->vErr.at(0);
    }

    else{
      nPSSMC2 = nPSSMC2 + vCutBkg.at(i)->nEvents.at(0);
      errPSSMC2 = errPSSMC2 + pow(vCutBkg.at(i)->vErr.at(0),2); //add in quadrature, take square root later
    }
  }//end loop over elel bkg vector

  //take square root for error on PSSMC
  errPSSMC2 = pow(errPSSMC2,0.5);
  */

  //now histograms for the different background processes
  //TH1F* All__FakeRate = new TH1F("All__FakeRate","",3,0,3);
  TH1F* elel__FakeRate = new TH1F("elel"+erast+"__FakeRate","",1,0,1);
  TH1F* elmu__FakeRate = new TH1F("elmu"+erast+"__FakeRate","",1,0,1);
  TH1F* mumu__FakeRate = new TH1F("mumu"+erast+"__FakeRate","",1,0,1);
  elel__FakeRate->SetBinContent(1,nFakeRate0);  elel__FakeRate->SetBinError(1,errFakeRate0);
  elmu__FakeRate->SetBinContent(1,nFakeRate1);  elmu__FakeRate->SetBinError(1,errFakeRate1);
  mumu__FakeRate->SetBinContent(1,nFakeRate2);  mumu__FakeRate->SetBinError(1,errFakeRate2);

  //TH1F* All__ChargeMisID = new TH1F("All__ChargeMisID","",3,0,3);
  TH1F* elel__ChargeMisID = new TH1F("elel"+erast+"__ChargeMisID","",1,0,1);
  TH1F* elmu__ChargeMisID = new TH1F("elmu"+erast+"__ChargeMisID","",1,0,1);
  TH1F* mumu__ChargeMisID = new TH1F("mumu"+erast+"__ChargeMisID","",1,0,1);
  elel__ChargeMisID->SetBinContent(1,nChargeMisID0);  elel__ChargeMisID->SetBinError(1,errChargeMisID0);
  elmu__ChargeMisID->SetBinContent(1,nChargeMisID1);  elmu__ChargeMisID->SetBinError(1,errChargeMisID1);
  mumu__ChargeMisID->SetBinContent(1,nChargeMisID2);  mumu__ChargeMisID->SetBinError(1,errChargeMisID2);


  //TH1F* All__TTW = new TH1F("All__TTW","",3,0,3);
  TH1F* elel__TTW = new TH1F("elel"+erast+"__TTW","",1,0,1);
  TH1F* elmu__TTW = new TH1F("elmu"+erast+"__TTW","",1,0,1);
  TH1F* mumu__TTW = new TH1F("mumu"+erast+"__TTW","",1,0,1);
  elel__TTW->SetBinContent(1,nTTW0);  elel__TTW->SetBinError(1,errTTW0);
  elmu__TTW->SetBinContent(1,nTTW1);  elmu__TTW->SetBinError(1,errTTW1);
  mumu__TTW->SetBinContent(1,nTTW2);  mumu__TTW->SetBinError(1,errTTW2);

  //TH1F* All__TTZ = new TH1F("All__TTZ","",3,0,3);
  TH1F* elel__TTZ = new TH1F("elel"+erast+"__TTZ","",1,0,1);
  TH1F* elmu__TTZ = new TH1F("elmu"+erast+"__TTZ","",1,0,1);
  TH1F* mumu__TTZ = new TH1F("mumu"+erast+"__TTZ","",1,0,1);
  elel__TTZ->SetBinContent(1,nTTZ0);  elel__TTZ->SetBinError(1,errTTZ0);
  elmu__TTZ->SetBinContent(1,nTTZ1);  elmu__TTZ->SetBinError(1,errTTZ1);
  mumu__TTZ->SetBinContent(1,nTTZ2);  mumu__TTZ->SetBinError(1,errTTZ2);

  //TH1F* All__TTH = new TH1F("All__TTH","",3,0,3);
  TH1F* elel__TTH = new TH1F("elel"+erast+"__TTH","",1,0,1);
  TH1F* elmu__TTH = new TH1F("elmu"+erast+"__TTH","",1,0,1);
  TH1F* mumu__TTH = new TH1F("mumu"+erast+"__TTH","",1,0,1);
  elel__TTH->SetBinContent(1,nTTH0);  elel__TTH->SetBinError(1,errTTH0);
  elmu__TTH->SetBinContent(1,nTTH1);  elmu__TTH->SetBinError(1,errTTH1);
  mumu__TTH->SetBinContent(1,nTTH2);  mumu__TTH->SetBinError(1,errTTH2);

  //TH1F* All__TTTT = new TH1F("All__TTTT","",3,0,3);
  TH1F* elel__TTTT = new TH1F("elel"+erast+"__TTTT","",1,0,1);
  TH1F* elmu__TTTT = new TH1F("elmu"+erast+"__TTTT","",1,0,1);
  TH1F* mumu__TTTT = new TH1F("mumu"+erast+"__TTTT","",1,0,1);
  elel__TTTT->SetBinContent(1,nTTTT0);  elel__TTTT->SetBinError(1,errTTTT0);
  elmu__TTTT->SetBinContent(1,nTTTT1);  elmu__TTTT->SetBinError(1,errTTTT1);
  mumu__TTTT->SetBinContent(1,nTTTT2);  mumu__TTTT->SetBinError(1,errTTTT2);

  //TH1F* All__WZ = new TH1F("All__WZ","",3,0,3);
  TH1F* elel__WZ = new TH1F("elel"+erast+"__WZ","",1,0,1);
  TH1F* elmu__WZ = new TH1F("elmu"+erast+"__WZ","",1,0,1);
  TH1F* mumu__WZ = new TH1F("mumu"+erast+"__WZ","",1,0,1);
  elel__WZ->SetBinContent(1,nWZ0);  elel__WZ->SetBinError(1,errWZ0);
  elmu__WZ->SetBinContent(1,nWZ1);  elmu__WZ->SetBinError(1,errWZ1);
  mumu__WZ->SetBinContent(1,nWZ2);  mumu__WZ->SetBinError(1,errWZ2);

  //TH1F* All__ZZ = new TH1F("All__ZZ","",3,0,3);
  TH1F* elel__ZZ = new TH1F("elel"+erast+"__ZZ","",1,0,1);
  TH1F* elmu__ZZ = new TH1F("elmu"+erast+"__ZZ","",1,0,1);
  TH1F* mumu__ZZ = new TH1F("mumu"+erast+"__ZZ","",1,0,1);
  elel__ZZ->SetBinContent(1,nZZ0);  elel__ZZ->SetBinError(1,errZZ0);
  elmu__ZZ->SetBinContent(1,nZZ1);  elmu__ZZ->SetBinError(1,errZZ1);
  mumu__ZZ->SetBinContent(1,nZZ2);  mumu__ZZ->SetBinError(1,errZZ2);

  //TH1F* All__WpWp = new TH1F("All__WpWp","",3,0,3);
  TH1F* elel__WpWp = new TH1F("elel"+erast+"__WpWp","",1,0,1);
  TH1F* elmu__WpWp = new TH1F("elmu"+erast+"__WpWp","",1,0,1);
  TH1F* mumu__WpWp = new TH1F("mumu"+erast+"__WpWp","",1,0,1);
  elel__WpWp->SetBinContent(1,nWpWp0);  elel__WpWp->SetBinError(1,errWpWp0);
  elmu__WpWp->SetBinContent(1,nWpWp1);  elmu__WpWp->SetBinError(1,errWpWp1);
  mumu__WpWp->SetBinContent(1,nWpWp2);  mumu__WpWp->SetBinError(1,errWpWp2);


  //TH1F* All__WZ = new TH1F("All__WZ","",3,0,3);
  TH1F* elel__WWZ = new TH1F("elel"+erast+"__WWZ","",1,0,1);
  TH1F* elmu__WWZ = new TH1F("elmu"+erast+"__WWZ","",1,0,1);
  TH1F* mumu__WWZ = new TH1F("mumu"+erast+"__WWZ","",1,0,1);
  elel__WWZ->SetBinContent(1,nWWZ0);  elel__WWZ->SetBinError(1,errWWZ0);
  elmu__WWZ->SetBinContent(1,nWWZ1);  elmu__WWZ->SetBinError(1,errWWZ1);
  mumu__WWZ->SetBinContent(1,nWWZ2);  mumu__WWZ->SetBinError(1,errWWZ2);

  //TH1F* All__WZZ = new TH1F("All__WZZ","",3,0,3);
  TH1F* elel__WZZ = new TH1F("elel"+erast+"__WZZ","",1,0,1);
  TH1F* elmu__WZZ = new TH1F("elmu"+erast+"__WZZ","",1,0,1);
  TH1F* mumu__WZZ = new TH1F("mumu"+erast+"__WZZ","",1,0,1);
  elel__WZZ->SetBinContent(1,nWZZ0);  elel__WZZ->SetBinError(1,errWZZ0);
  elmu__WZZ->SetBinContent(1,nWZZ1);  elmu__WZZ->SetBinError(1,errWZZ1);
  mumu__WZZ->SetBinContent(1,nWZZ2);  mumu__WZZ->SetBinError(1,errWZZ2);

  //TH1F* All__ZZZ = new TH1F("All__ZZZ","",3,0,3);
  /*TH1F* elel__ZZZ = new TH1F("elel__ZZZ","",1,0,1);
  TH1F* elmu__ZZZ = new TH1F("elmu__ZZZ","",1,0,1);
  TH1F* mumu__ZZZ = new TH1F("mumu__ZZZ","",1,0,1);
  elel__ZZZ->SetBinContent(1,nZZZ0);  elel__ZZZ->SetBinError(1,errZZZ0);
  elmu__ZZZ->SetBinContent(1,nZZZ1);  elmu__ZZZ->SetBinError(1,errZZZ1);
  mumu__ZZZ->SetBinContent(1,nZZZ2);  mumu__ZZZ->SetBinError(1,errZZZ2);*/



  fout->WriteTObject(elel__FakeRate);
  fout_elel->WriteTObject(elel__FakeRate);

  fout->WriteTObject(elmu__FakeRate);
  fout_elmu->WriteTObject(elmu__FakeRate);

  fout->WriteTObject(mumu__FakeRate);
  fout_mumu->WriteTObject(mumu__FakeRate);

  fout->WriteTObject(elel__ChargeMisID);
  fout_elel->WriteTObject(elel__ChargeMisID);

  fout->WriteTObject(elmu__ChargeMisID); 
  fout_elmu->WriteTObject(elmu__ChargeMisID);

  fout->WriteTObject(mumu__ChargeMisID);
  fout_mumu->WriteTObject(mumu__ChargeMisID);

  fout->WriteTObject(elel__TTW);
  fout_elel->WriteTObject(elel__TTW);

  fout->WriteTObject(elmu__TTW);
  fout_elmu->WriteTObject(elmu__TTW);

  fout->WriteTObject(mumu__TTW);
  fout_mumu->WriteTObject(mumu__TTW);

  fout->WriteTObject(elel__TTZ);
  fout_elel->WriteTObject(elel__TTZ);

  fout->WriteTObject(elmu__TTZ);
  fout_elmu->WriteTObject(elmu__TTZ);

  fout->WriteTObject(mumu__TTZ);
  fout_mumu->WriteTObject(mumu__TTZ);

  fout->WriteTObject(elel__TTH);
  fout_elel->WriteTObject(elel__TTH);

  fout->WriteTObject(elmu__TTH);
  fout_elmu->WriteTObject(elmu__TTH);

  fout->WriteTObject(mumu__TTH);
  fout_mumu->WriteTObject(mumu__TTH);

  fout->WriteTObject(elel__TTTT);
  fout_elel->WriteTObject(elel__TTTT);

  fout->WriteTObject(elmu__TTTT);
  fout_elmu->WriteTObject(elmu__TTTT);

  fout->WriteTObject(mumu__TTTT);
  fout_mumu->WriteTObject(mumu__TTTT);

  fout->WriteTObject(elel__WZ);
  fout_elel->WriteTObject(elel__WZ);

  fout->WriteTObject(elmu__WZ);
  fout_elmu->WriteTObject(elmu__WZ);

  fout->WriteTObject(mumu__WZ);
  fout_mumu->WriteTObject(mumu__WZ);

  fout->WriteTObject(elel__ZZ);
  fout_elel->WriteTObject(elel__ZZ);

  fout->WriteTObject(elmu__ZZ);
  fout_elmu->WriteTObject(elmu__ZZ);

  fout->WriteTObject(mumu__ZZ);
  fout_mumu->WriteTObject(mumu__ZZ);

  fout->WriteTObject(elel__WpWp);
  fout_elel->WriteTObject(elel__WpWp);

  fout->WriteTObject(elmu__WpWp);
  fout_elmu->WriteTObject(elmu__WpWp);

  fout->WriteTObject(mumu__WpWp);
  fout_mumu->WriteTObject(mumu__WpWp);

  fout->WriteTObject(elel__WWZ);
  fout_elel->WriteTObject(elel__WWZ);

  fout->WriteTObject(elmu__WWZ);
  fout_elmu->WriteTObject(elmu__WWZ);

  fout->WriteTObject(mumu__WWZ);
  fout_mumu->WriteTObject(mumu__WWZ);

  fout->WriteTObject(elel__WZZ);
  fout_elel->WriteTObject(elel__WZZ);

  fout->WriteTObject(elmu__WZZ);
  fout_elmu->WriteTObject(elmu__WZZ);

  fout->WriteTObject(mumu__WZZ);
  fout_mumu->WriteTObject(mumu__WZZ);

  /* fout->WriteTObject(elel__ZZZ);
  fout_elel->WriteTObject(elel__ZZZ);

  fout->WriteTObject(elmu__ZZZ);
  fout_elmu->WriteTObject(elmu__ZZZ);

  fout->WriteTObject(mumu__ZZZ);
  fout_mumu->WriteTObject(mumu__ZZZ);*/


  
  //fout->Write();
  fout->Close();
  fout_elel->Close();
  fout_elmu->Close();
  fout_mumu->Close();


  return 0;
}

