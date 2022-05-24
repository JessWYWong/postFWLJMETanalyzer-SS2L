#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include <vector>
#include "TLorentzVector.h"
#include "TChain.h"
#include "../interface/TreeReader.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <map>
#include <string>
#include <sstream> 
#include "../plugins/Macros.cc"

//helper functions
bool matchToGenLep(TMuon* mu, std::vector<TGenParticle*> genParticles);
bool matchToGenLep(TElectron* el, std::vector<TGenParticle*> genParticles);
TGenParticle* getMatchedGenParticle(TElectron* el, std::vector<TGenParticle*> genParticles);
TGenParticle* getMatchedGenParticle(TMuon* mu, std::vector<TGenParticle*> genParticles);


int main(int argc, char* argv[]){

  if(argc!=4){
    std::cout<<"Need to specify sample and electron and muon IDs used! Exiting..."<<std::endl;
    return 0; 
  }

  std::string sample = argv[1];
  std::string elID = argv[2];
  std::string muID = argv[3];

  bool mc=true;

  typedef std::map<std::string,std::string> StringMap;
  
  StringMap samples;
  //std::string eosarea = "root://cmsxrootd.fnal.gov//store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/";
  std::string eosarea = "/eos/uscms/store/user/lpcljm/FWLJMET102X_2lepFakeRate2018_wywong_082020_hadds/";
  samples["TTTo2L2Nu"] = eosarea+"TTTo2L2Nu.root";
  samples["TTToSemiLeptonic"] = eosarea+"TTToSemiLeptonic.root";
  /*
  samples["TTbar_pt0"] = eosarea+"ljmet_TTbar_pt0.root";
  samples["TTbar_pt1"] = eosarea+"ljmet_TTbar_pt1.root";
  samples["TTbar_pt2"] = eosarea+"ljmet_TTbar_pt2.root";
  samples["TTbar_pt3"] = eosarea+"ljmet_TTbar_pt3.root";
  samples["TTbar_pt4"] = eosarea+"ljmet_TTbar_pt4.root";
  samples["QCD_Pt-20to30_EMEnriched"] = eosarea+"ljmet_FakeRate_QCD_Pt-20to30_EMEnriched.root"; 
  samples["QCD_Pt-30to50_EMEnriched"] = eosarea+"ljmet_FakeRate_QCD_Pt-30to50_EMEnriched.root";
  samples["QCD_Pt-50to80_EMEnriched"] = eosarea+"ljmet_FakeRate_QCD_Pt-50to80_EMEnriched.root";
  samples["QCD_Pt-80to120_EMEnriched"] = eosarea+"ljmet_FakeRate_QCD_Pt-80to120_EMEnriched.root";
  samples["QCD_Pt-120to170_EMEnriched"] = eosarea+"ljmet_FakeRate_QCD_Pt-120to170_EMEnriched.root";
  samples["QCD_Pt-170to300_EMEnriched"] = eosarea+"ljmet_FakeRate_QCD_Pt-170to300_EMEnriched.root";
  samples["QCD_Pt-300toInf_EMEnriched"] = eosarea+"ljmet_FakeRate_QCD_Pt-300toInf_EMEnriched.root";
  samples["QCD_Pt-20to30_MuEnrichedPt5"] = eosarea+"ljmet_FakeRate_QCD_Pt-20to30_MuEnrichedPt5.root";
  samples["QCD_Pt-30to50_MuEnrichedPt5"] = eosarea+"ljmet_FakeRate_QCD_Pt-30to50_MuEnrichedPt5.root";
  samples["QCD_Pt-50to80_MuEnrichedPt5"] = eosarea+"ljmet_FakeRate_QCD_Pt-50to80_MuEnrichedPt5.root";
  samples["QCD_Pt-80to120_MuEnrichedPt5"] = eosarea+"ljmet_FakeRate_QCD_Pt-80to120_MuEnrichedPt5.root";
  samples["QCD_Pt-120to170_MuEnrichedPt5"] = eosarea+"ljmet_FakeRate_QCD_Pt-120to170_MuEnrichedPt5.root";
  samples["QCD_Pt-170to300_MuEnrichedPt5"] = eosarea+"ljmet_FakeRate_QCD_Pt-170to300_MuEnrichedPt5.root";
  samples["QCD_Pt-300to470_MuEnrichedPt5"] = eosarea+"ljmet_FakeRate_QCD_Pt-300to470_MuEnrichedPt5.root";
  samples["QCD_Pt-470to600_MuEnrichedPt5"] = eosarea+"ljmet_FakeRate_QCD_Pt-470to600_MuEnrichedPt5.root";
  samples["QCD_Pt-600to800_MuEnrichedPt5"] = eosarea+"ljmet_FakeRate_QCD_Pt-600to800_MuEnrichedPt5.root";
  samples["QCD_Pt-800to1000_MuEnrichedPt5"] = eosarea+"ljmet_FakeRate_QCD_Pt-800to1000_MuEnrichedPt5.root";
  samples["QCD_Pt-1000toInf_MuEnrichedPt5"] = eosarea+"ljmet_FakeRate_QCD_Pt-1000toInf_MuEnrichedPt5.root";
  */  
  if(samples.find(sample)==samples.end()) {std::cout<<"didn't pick a correct sample! Exiting..."<<std::endl; return 0;}
    
  std::string filename;
  filename = samples.find(sample)->second;

  
  TreeReader* tr = new TreeReader(filename.c_str(),"ljmet/ljmet",mc,true);
  //TreeReader* tr = new TreeReader("/uscms_data/d3/clint/using_git/T53/ljmet/CMSSW_7_4_14/src/LJMet/Com/python/ljmet_test.root",mc);
  TTree* t = tr->tree;
  std::string outname = "FakeRate_"+sample+".root";

  TFile* outfile = new TFile(outname.c_str(),"RECREATE");
  TTree* outTree = new TTree("FakeRateByFlavor","FakeRateByFlavor");
  int flavorSource,lepFlavor,isTight;
  float lepEta,lepPt,dxy,dz,sip3d;
  outTree->Branch("LepFlavor",&lepFlavor);
  outTree->Branch("LepSourceFlavor",&flavorSource);
  outTree->Branch("LepEta",&lepEta);
  outTree->Branch("LepPt",&lepPt);
  outTree->Branch("LepIsTight",&isTight);
  outTree->Branch("dXY",&dxy);
  outTree->Branch("dZ",&dz);
  outTree->Branch("SIP3D",&sip3d);
  float muEtabins[10]{-2.4,-2.1,-1.2,-0.9,-0.4,0.4,0.9,1.2,2.1,2.4};
  float elEtabins[8]={-2.4,-1.57,-0.8,-0.4,0.4,0.8,1.57,2.4};
  TH1F* elNumHist_lpt = new TH1F("elNumHist_lpt","elNumHist_lpt",6,0,6);
  TH1F* elDenHist_lpt = new TH1F("elDenHist_lpt","elDenHist_lpt",6,0,6);
  TH1F* elNumHist_hpt = new TH1F("elNumHist_hpt","elNumHist_hpt",6,0,6);
  TH1F* elDenHist_hpt = new TH1F("elDenHist_hpt","elDenHist_hpt",6,0,6);
  TH1F* elNumHist_all = new TH1F("elNumHist_all","elNumHist_all",6,0,6);
  TH1F* elDenHist_all = new TH1F("elDenHist_all","elDenHist_all",6,0,6);
  TH1F* elNumHist_lpt_eta = new TH1F("elNumHist_lpt_eta","elNumHist_lpt_eta",7,elEtabins);
  TH1F* elDenHist_lpt_eta = new TH1F("elDenHist_lpt_eta","elDenHist_lpt_eta",7,elEtabins);
  TH1F* elNumHist_hpt_eta = new TH1F("elNumHist_hpt_eta","elNumHist_hpt_eta",7,elEtabins);
  TH1F* elDenHist_hpt_eta = new TH1F("elDenHist_hpt_eta","elDenHist_hpt_eta",7,elEtabins);
  TH1F* elNumHist_all_eta = new TH1F("elNumHist_all_eta","elNumHist_all_eta",7,elEtabins);
  TH1F* elDenHist_all_eta = new TH1F("elDenHist_all_eta","elDenHist_all_eta",7,elEtabins);

  TH1F* muNumHist_lpt = new TH1F("muNumHist_lpt","muNumHist_lpt",6,0,6);
  TH1F* muDenHist_lpt = new TH1F("muDenHist_lpt","muDenHist_lpt",6,0,6);
  TH1F* muNumHist_hpt = new TH1F("muNumHist_hpt","muNumHist_hpt",6,0,6);
  TH1F* muDenHist_hpt = new TH1F("muDenHist_hpt","muDenHist_hpt",6,0,6);
  TH1F* muNumHist_all = new TH1F("muNumHist_all","muNumHist_all",6,0,6);
  TH1F* muDenHist_all = new TH1F("muDenHist_all","muDenHist_all",6,0,6);
  TH1F* muNumHist_lpt_eta = new TH1F("muNumHist_lpt_eta","muNumHist_lpt_eta",9,muEtabins);
  TH1F* muDenHist_lpt_eta = new TH1F("muDenHist_lpt_eta","muDenHist_lpt_eta",9,muEtabins);
  TH1F* muNumHist_hpt_eta = new TH1F("muNumHist_hpt_eta","muNumHist_hpt_eta",9,muEtabins);
  TH1F* muDenHist_hpt_eta = new TH1F("muDenHist_hpt_eta","muDenHist_hpt_eta",9,muEtabins);
  TH1F* muNumHist_all_eta = new TH1F("muNumHist_all_eta","muNumHist_all_eta",9,muEtabins);
  TH1F* muDenHist_all_eta = new TH1F("muDenHist_all_eta","muDenHist_all_eta",9,muEtabins);

  //set bin labels
  elNumHist_lpt->GetXaxis()->SetBinLabel(1,"Light Quarks");elNumHist_hpt->GetXaxis()->SetBinLabel(1,"Light Quarks");elNumHist_all->GetXaxis()->SetBinLabel(1,"Light Quarks");
  elNumHist_lpt->GetXaxis()->SetBinLabel(2,"Charm Quarks");elNumHist_hpt->GetXaxis()->SetBinLabel(2,"Charm Quarks");elNumHist_all->GetXaxis()->SetBinLabel(2,"Charm Quarks");
  elNumHist_lpt->GetXaxis()->SetBinLabel(3,"Bottom Quarks");elNumHist_hpt->GetXaxis()->SetBinLabel(3,"Bottom Quarks");elNumHist_all->GetXaxis()->SetBinLabel(3,"Bottom Quarks");
  elNumHist_lpt->GetXaxis()->SetBinLabel(4,"Fake");elNumHist_hpt->GetXaxis()->SetBinLabel(4,"Fake");elNumHist_all->GetXaxis()->SetBinLabel(4,"Fake");
  elNumHist_lpt->GetXaxis()->SetBinLabel(5,"Unmatched");elNumHist_hpt->GetXaxis()->SetBinLabel(5,"Unmatched");elNumHist_all->GetXaxis()->SetBinLabel(5,"Unmatched");
  elNumHist_lpt->GetXaxis()->SetBinLabel(6,"Average");elNumHist_hpt->GetXaxis()->SetBinLabel(6,"Average");elNumHist_all->GetXaxis()->SetBinLabel(6,"Average");

  muNumHist_lpt->GetXaxis()->SetBinLabel(1,"Light Quarks");muNumHist_hpt->GetXaxis()->SetBinLabel(1,"Light Quarks");muNumHist_all->GetXaxis()->SetBinLabel(1,"Light Quarks");
  muNumHist_lpt->GetXaxis()->SetBinLabel(2,"Charm Quarks");muNumHist_hpt->GetXaxis()->SetBinLabel(2,"Charm Quarks");muNumHist_all->GetXaxis()->SetBinLabel(2,"Charm Quarks");
  muNumHist_lpt->GetXaxis()->SetBinLabel(3,"Bottom Quarks");muNumHist_hpt->GetXaxis()->SetBinLabel(3,"Bottom Quarks");muNumHist_all->GetXaxis()->SetBinLabel(3,"Bottom Quarks");
  muNumHist_lpt->GetXaxis()->SetBinLabel(4,"Fake");muNumHist_hpt->GetXaxis()->SetBinLabel(4,"Fake");muNumHist_all->GetXaxis()->SetBinLabel(4,"Fake");
  muNumHist_lpt->GetXaxis()->SetBinLabel(5,"Unmatched");muNumHist_hpt->GetXaxis()->SetBinLabel(5,"Unmatched");muNumHist_all->GetXaxis()->SetBinLabel(5,"Unmatched");
  muNumHist_lpt->GetXaxis()->SetBinLabel(6,"Average");muNumHist_hpt->GetXaxis()->SetBinLabel(6,"Average");muNumHist_all->GetXaxis()->SetBinLabel(6,"Average");

  int nEntries = t->GetEntries();

  for(int ient=0; ient < nEntries; ient++){
    //for(int ient=0; ient < 2000; ient++){

    tr->GetEntry(ient);

    if(ient % 100000 ==0) std::cout<<"Completed "<<ient<<" out of "<<nEntries<<" events"<<std::endl;

    std::vector<TElectron*> electrons = tr->allElectrons;
    //event weight
    float eventweight = tr->MCWeight > 0 ? 1 : -1;

    for(unsigned int i=0; i<electrons.size();i++){
      TElectron* iel = electrons.at(i);
      bool loose = false;
      bool tight = false;
      bool save=false;
      if(elID=="MVA2016TightRC"){
	loose = iel->mvaJulieLooseRCIso();
	tight = iel->mva2016TightRCIso();
      }
      else if(elID=="MVA2017TightV2IsoTightRC"){
        loose = iel->mva94XLooseV2_Iso_RC();
        tight = iel->mva94XTightV2_90_IsoTight_RC();
      }
      else{
	std::cout<<"Didn't pick analysis ID! Are you sure? Exiting so you can think about it..."<<std::endl;
	return 0;
      }

      //skip any with pt below 25 GeV
      if(iel->pt<25) continue;
      //skip if not at least loose
      if(!loose) continue;
      //save variables for tree
      isTight=tight;
      lepPt=iel->pt;
      lepEta=iel->eta;
      lepFlavor=0;
      dxy=iel->d0;
      dz=iel->dZ;
      sip3d=iel->SIP3d;
      //get closest gen particle
      TGenParticle* gp = getMatchedGenParticle(iel,tr->genParticles);	
      
      float dR = pow( pow(gp->eta - iel->eta,2) + pow(gp->phi - iel->phi,2), 0.5);
      //first do average
      if( fabs(gp->id)!=11 || !(gp->isPrompt || gp->isFromPromptTau) ){
	
	elDenHist_all->Fill(5.5,eventweight);
        elDenHist_all_eta->Fill(lepEta, eventweight);
	if(iel->pt>35){
            elDenHist_hpt->Fill(5.5,eventweight);
            elDenHist_hpt_eta->Fill(lepEta, eventweight);
        }
	else{
            elDenHist_lpt->Fill(5.5,eventweight);
            elDenHist_lpt_eta->Fill(lepEta, eventweight);
        }
	if(tight){
	  elNumHist_all->Fill(5.5,eventweight);
          elNumHist_all_eta->Fill(lepEta, eventweight);
	  if(iel->pt>35){
            elNumHist_hpt->Fill(5.5,eventweight);
            elNumHist_hpt_eta->Fill(lepEta, eventweight);
          }
  	  else{
            elNumHist_lpt->Fill(5.5,eventweight);
            elNumHist_lpt_eta->Fill(lepEta,eventweight);
          }
	}
      }

      //now by bin - first unmatched
      if(dR>0.3){//unmatched
	save=true;
	flavorSource=4;
	//std::cout<<"UNMATCHED Electron Closest Gen Particle Info: dR = "<<dR<<"; id = "<<gp->id<<"; status = "<<gp->status<<"; motherId = "<<gp->motherId<<" promptMotherID: "<<gp->PromptMotherId<<std::endl;
	//if(tight) std::cout<<"UNMATCHED Electron passes tight ID with pT: "<<iel->pt<<" mva value: "<<iel->mvaValue<<" miniIso: "<<iel->miniIso<<std::endl;
	elDenHist_all->Fill(4.5,eventweight);
	if(iel->pt>35) elDenHist_hpt->Fill(4.5,eventweight);
	else elDenHist_lpt->Fill(4.5,eventweight);
	if(tight){
	  elNumHist_all->Fill(4.5,eventweight);
	  if(iel->pt>35) elNumHist_hpt->Fill(4.5,eventweight);
	  else elNumHist_lpt->Fill(4.5,eventweight);
	}
      
      }//end if about unmatched
      else{ 
	if(fabs(gp->id)!=11){//fake electron
	  save=true;
	  flavorSource=3;
	  elDenHist_all->Fill(3.5,eventweight);
	  //std::cout<<"FAKE Electron Closest Gen Particle Info: dR = "<<dR<<"; id = "<<gp->id<<"; status = "<<gp->status<<"; motherId = "<<gp->motherId<<"; promptMotherID: "<<gp->PromptMotherId<<std::endl;
	  //if(tight) std::cout<<"FAKE Electron passes tight ID with pT: "<<iel->pt<<" mva value: "<<iel->mvaValue<<" miniIso: "<<iel->miniIso<<std::endl;
	  if(iel->pt>35) elDenHist_hpt->Fill(3.5,eventweight);
	  else elDenHist_lpt->Fill(3.5,eventweight);
	  if(tight){
	    elNumHist_all->Fill(3.5,eventweight);
	    if(iel->pt>35) elNumHist_hpt->Fill(3.5,eventweight);
	    else elNumHist_lpt->Fill(3.5,eventweight);
	  }
	}//end fake electrons
	else{//real electron
	  if(gp->isPrompt || gp->isFromPromptTau) continue; //skip prompt electrons or electrons from prompt taus
	  //std::cout<<"REAL Electron Closest Gen Particle Info: dR = "<<dR<<"; id = "<<gp->id<<"; status = "<<gp->status<<"; motherId = "<<gp->motherId<<" promptMotherID: "<<gp->PromptMotherId<<std::endl;
	  //if(tight) std::cout<<"REAL Electron passes tight ID with pT: "<<iel->pt<<" mva value: "<<iel->mvaValue<<" miniIso: "<<iel->miniIso<<std::endl;
	  save=true;
	  if(gp->PromptMotherHasB){
	    flavorSource=2;
	    elDenHist_all->Fill(2.5,eventweight);	   
	    if(iel->pt>35) elDenHist_hpt->Fill(2.5,eventweight);
	    else elDenHist_lpt->Fill(2.5,eventweight);
	    if(tight){
	      elNumHist_all->Fill(2.5,eventweight);
	      if(iel->pt>35) elNumHist_hpt->Fill(2.5,eventweight);
	      else elNumHist_lpt->Fill(2.5,eventweight);
	    }
	  }//end matched to b quarks
	  else if(gp->PromptMotherHasC){
	    flavorSource=1;
	    elDenHist_all->Fill(1.5,eventweight);	   
	    if(iel->pt>35) elDenHist_hpt->Fill(1.5,eventweight);
	    else elDenHist_lpt->Fill(1.5,eventweight);
	    if(tight){
	      elNumHist_all->Fill(1.5,eventweight);
	      if(iel->pt>35) elNumHist_hpt->Fill(1.5,eventweight);
	      else elNumHist_lpt->Fill(1.5,eventweight);
	    }
	  }//end matched to c quarks
	  else {// light quarks/gluons
	    flavorSource=0;
	    elDenHist_all->Fill(0.5,eventweight);	   
	    if(iel->pt>35) elDenHist_hpt->Fill(0.5,eventweight);
	    else elDenHist_lpt->Fill(0.5,eventweight);
	    if(tight){
	      elNumHist_all->Fill(0.5,eventweight);
	      if(iel->pt>35) elNumHist_hpt->Fill(0.5,eventweight);
	      else elNumHist_lpt->Fill(0.5,eventweight);
	    }
	  }//end light quarks/gluons
	  
	}//end check on real electrons
      }//end matched electrons

      //fill tree for each nonPrompt electron
      if(save){
	outTree->Fill();
      }

    }//electron loop
    
    std::vector<TMuon*> muons = tr->allMuons;
    for(unsigned int i=0; i<muons.size(); i++){
      TMuon* imu = muons.at(i);
      bool save=false;
      bool loose=false;
      bool tight=false;
      if(muID=="CBTight"){
	loose = imu->cutBasedLoose();
	tight = imu->cutBasedTight();
      }
      else if(muID=="CBTightMiniIsoTight"){
	tight=imu->cutBasedTightMiniIsoTight();
	loose=imu->cutBasedLooseMiniIso();
      }
      else{
	std::cout<<"Muon ID selected not the one used in analysis! Are you sure?? Exiting to let you think about it..."<<std::endl;
	return 0;
      }

      //skip any with pt below 30 GeV
      if(imu->pt<25) continue;
      //skip if not at least loose
      if(!loose) continue;

      //set variables for tree
      lepPt=imu->pt;
      isTight=tight;
      lepEta=imu->eta;
      lepFlavor=1;
      dxy=imu->dxy;
      dz=imu->dz;
      sip3d=imu->SIP3d;
      //get closest gen particle
      TGenParticle* gp = getMatchedGenParticle(imu,tr->genParticles);	
      
      float dR = pow( pow(gp->eta - imu->eta,2) + pow(gp->phi - imu->phi,2), 0.5);
      //first do average bin
      //first do average
      if( fabs(gp->id)!=13 || !(gp->isPrompt || gp->isFromPromptTau) ){
	
	muDenHist_all->Fill(5.5,eventweight);
        muDenHist_all_eta->Fill(lepEta,eventweight);
	if(imu->pt>35){
            muDenHist_hpt->Fill(5.5,eventweight);
            muDenHist_hpt_eta->Fill(lepEta,eventweight);
        }
	else{
            muDenHist_lpt->Fill(5.5,eventweight);
            muDenHist_lpt_eta->Fill(lepEta,eventweight);
        }
	if(tight){
	  muNumHist_all->Fill(5.5,eventweight);
          muNumHist_all_eta->Fill(lepEta,eventweight);
	  if(imu->pt>35){
              muNumHist_hpt->Fill(5.5,eventweight);
              muNumHist_hpt_eta->Fill(lepEta,eventweight);
          }
  	  else{
              muNumHist_lpt->Fill(5.5,eventweight);
              muNumHist_lpt_eta->Fill(lepEta,eventweight);
          }
	}
      }

      //now do by bins, first unmatched
      if(dR>0.3){//unmatched
	save=true;
	flavorSource=4;
	//std::cout<<"UNMATCHED Muon Closest Gen Particle Info: dR = "<<dR<<"; id = "<<gp->id<<"; status = "<<gp->status<<"; motherId = "<<gp->motherId<<"; promptMotherId: "<<gp->PromptMotherId<<std::endl;
	//if(tight) std::cout<<"UNMATCHED Muon passes tight ID with pT: "<<imu->pt<<" relIso: "<<imu->relIso<<std::endl;

	muDenHist_all->Fill(4.5,eventweight);
	if(imu->pt>35) muDenHist_hpt->Fill(4.5,eventweight);
	else muDenHist_lpt->Fill(4.5,eventweight);
	if(tight){
	  muNumHist_all->Fill(4.5,eventweight);
	  if(imu->pt>35) muNumHist_hpt->Fill(4.5,eventweight);
	  else muNumHist_lpt->Fill(4.5,eventweight);
	}
      
      }//end if about unmatched
      else{ 
	if(fabs(gp->id)!=13){//fake muon
	  save=true;
	  flavorSource=3;
	  //std::cout<<"FAKE Muon Closest Gen Particle Info: dR = "<<dR<<"; id = "<<gp->id<<"; status = "<<gp->status<<"; motherId = "<<gp->motherId<<"; promptMotherId: "<<gp->PromptMotherId<<std::endl;
	  //if(tight) std::cout<<"FAKE Muon passes tight ID with pT: "<<imu->pt<<" relIso: "<<imu->relIso<<std::endl;
	  muDenHist_all->Fill(3.5,eventweight);	   
	  if(imu->pt>35) muDenHist_hpt->Fill(3.5,eventweight);
	  else muDenHist_lpt->Fill(3.5,eventweight);
	  if(tight){
	    muNumHist_all->Fill(3.5,eventweight);
	    if(imu->pt>35) muNumHist_hpt->Fill(3.5,eventweight);
	    else muNumHist_lpt->Fill(3.5,eventweight);
	  }
	}//end fake muons
	else{//real muon
	  if(gp->isPrompt || gp->isFromPromptTau) continue; //skip prompt muons or muons from prompt taus
	  //std::cout<<"REAL Muon Closest Gen Particle Info: dR = "<<dR<<"; id = "<<gp->id<<"; status = "<<gp->status<<"; motherId = "<<gp->motherId<<"; promptMotherId: "<<gp->PromptMotherId<<std::endl;
	  //if(tight) std::cout<<"REAL Muon passes tight ID with pT: "<<imu->pt<<" relIso: "<<imu->relIso<<std::endl;
	  save=true;
	  if(gp->PromptMotherHasB){
	    flavorSource=2;
	    muDenHist_all->Fill(2.5,eventweight);	   
	    if(imu->pt>35) muDenHist_hpt->Fill(2.5,eventweight);
	    else muDenHist_lpt->Fill(2.5,eventweight);
	    if(tight){
	      muNumHist_all->Fill(2.5,eventweight);
	      if(imu->pt>35) muNumHist_hpt->Fill(2.5,eventweight);
	      else muNumHist_lpt->Fill(2.5,eventweight);
	    }
	  }//end matched to c quarks
	  else if(gp->PromptMotherHasC){
	    flavorSource=1;
	    muDenHist_all->Fill(1.5,eventweight);	   
	    if(imu->pt>35) muDenHist_hpt->Fill(1.5,eventweight);
	    else muDenHist_lpt->Fill(1.5,eventweight);
	    if(tight){
	      muNumHist_all->Fill(1.5,eventweight);
	      if(imu->pt>35) muNumHist_hpt->Fill(1.5,eventweight);
	      else muNumHist_lpt->Fill(1.5,eventweight);
	    }
	  }//end matched to c quarks
	  else {// light quarks/gluons
	    flavorSource=0;
	    muDenHist_all->Fill(0.5,eventweight);	   
	    if(imu->pt>35) muDenHist_hpt->Fill(0.5,eventweight);
	    else muDenHist_lpt->Fill(0.5,eventweight);
	    if(tight){
	      muNumHist_all->Fill(0.5,eventweight);
	      if(imu->pt>35) muNumHist_hpt->Fill(0.5,eventweight);
	      else muNumHist_lpt->Fill(0.5,eventweight);
	    }
	  }//end light quarks/gluons
	  
	}//end check on real muons
      }//end matched muons

      if(save){
	outTree->Fill();
      }

    }//muon loop

      

  }//end event loop

  elNumHist_lpt->Sumw2();
  elNumHist_hpt->Sumw2();
  elNumHist_all->Sumw2();
  elDenHist_lpt->Sumw2();
  elDenHist_hpt->Sumw2();
  elDenHist_all->Sumw2();
  elNumHist_lpt_eta->Sumw2();
  elNumHist_hpt_eta->Sumw2();
  elNumHist_all_eta->Sumw2();
  elDenHist_lpt_eta->Sumw2();
  elDenHist_hpt_eta->Sumw2();
  elDenHist_all_eta->Sumw2();

  muNumHist_lpt->Sumw2();
  muNumHist_hpt->Sumw2();
  muNumHist_all->Sumw2();
  muDenHist_lpt->Sumw2();
  muDenHist_hpt->Sumw2();
  muDenHist_all->Sumw2();
  muNumHist_lpt_eta->Sumw2();
  muNumHist_hpt_eta->Sumw2();
  muNumHist_all_eta->Sumw2();
  muDenHist_lpt_eta->Sumw2();
  muDenHist_hpt_eta->Sumw2();
  muDenHist_all_eta->Sumw2();


  //elNumHist_lpt->Divide(elDenHist_lpt);
  //elNumHist_hpt->Divide(elDenHist_hpt);
  //elNumHist_all->Divide(elDenHist_all);
  std::cout<<"Electron fake rate for pt 25-35 electrons: "<<elNumHist_lpt->GetBinContent(1)/elDenHist_lpt->GetBinContent(1)<<" +/- "<<elNumHist_lpt->GetBinError(1)<<std::endl;
  std::cout<<"Electron fake rate for pt>25 electrons: "<<elNumHist_hpt->GetBinContent(1)/elDenHist_hpt->GetBinContent(1)<<" +/- "<<elNumHist_hpt->GetBinError(1)<<std::endl;
  std::cout<<"Electron fake rate for all electrons: "<<elNumHist_all->GetBinContent(1)/elDenHist_all->GetBinContent(1)<<" +/- "<<elNumHist_all->GetBinError(1)<<std::endl;

  //muNumHist_lpt->Divide(muDenHist_lpt);
  //muNumHist_hpt->Divide(muDenHist_hpt);
  //muNumHist_all->Divide(muDenHist_all);
  std::cout<<"Muon fake rate for pt 25-35 muons: "<<muNumHist_lpt->GetBinContent(1)/muDenHist_lpt->GetBinContent(1)<<" +/- "<<muNumHist_lpt->GetBinError(1)<<std::endl;
  std::cout<<"Muon fake rate for pt>25 muons: "<<muNumHist_hpt->GetBinContent(1)/muDenHist_hpt->GetBinContent(1)<<" +/- "<<muNumHist_hpt->GetBinError(1)<<std::endl;
  std::cout<<"Muon fake rate for all muons: "<<muNumHist_all->GetBinContent(1)/muDenHist_all->GetBinContent(1)<<" +/- "<<muNumHist_all->GetBinError(1)<<std::endl;

  /*
  outfile->Append(elNumHist_lpt);
  outfile->Append(elNumHist_hpt);
  outfile->Append(elNumHist_all);
  outfile->Append(elDenHist_lpt);
  outfile->Append(elDenHist_hpt);
  outfile->Append(elDenHist_all);

  outfile->Append(muNumHist_lpt);
  outfile->Append(muNumHist_hpt);
  outfile->Append(muNumHist_all);
  outfile->Append(muDenHist_lpt);
  outfile->Append(muDenHist_hpt);
  outfile->Append(muDenHist_all);
  */
  //outfile->WriteTObject(outTree);
  outfile->Write();
  outfile->Close();

  return 0;

}

bool matchToGenLep(TElectron* el, std::vector<TGenParticle*> genParticles){

  float dR=999;
  //run through gen particle collection:
  for(unsigned int igen=0; igen<genParticles.size(); igen++){
    TGenParticle* genParticle = genParticles.at(igen);
    //only run over electrons from hard scattering
    if( ! (fabs(genParticle->id)==11) ) continue;
    if( ! (genParticle->status==23 || genParticle->status==1) ) continue;

    float drtemp = pow( pow( el->eta - genParticle->eta, 2 ) + pow( el->phi - genParticle->phi, 2), 0.5);
    if(drtemp < dR){
      dR = drtemp;
    }
  }

  //now we have a min deltaR between our electron and all electrons from hard scattering, if it is less than 0.15 let's consider it matched
  if(dR <0.3) return true;
  else return false;
  
}

bool matchToGenLep(TMuon* mu, std::vector<TGenParticle*> genParticles){

  float dR=999;
  //run through gen particle collection:
  for(unsigned int igen=0; igen<genParticles.size(); igen++){
    TGenParticle* genParticle = genParticles.at(igen);
    //only run over muons from hard scattering
    if( ! (fabs(genParticle->id)==13) ) continue;
    if( ! (genParticle->status==23 || genParticle->status==1) ) continue;

    float drtemp = pow( pow( mu->eta - genParticle->eta, 2 ) + pow( mu->phi - genParticle->phi, 2), 0.5);
    if(drtemp < dR){
      dR = drtemp;
    }
  }

  //now we have a min deltaR between our muon and all muons from hard scattering, if it is less than 0.15 let's consider it matched
  if(dR <0.3) return true;
  else return false;
  
}

TGenParticle* getMatchedGenParticle(TElectron* el, std::vector<TGenParticle*> genParticles){


  TGenParticle* gp=0;

  bool nearbyEl = false;
  
  float dR=999;
  //run through gen particle collection:
  for(unsigned int igen=0; igen<genParticles.size(); igen++){
    TGenParticle* genParticle = genParticles.at(igen);
    float drtemp = pow( pow( el->eta - genParticle->eta, 2 ) + pow( el->phi - genParticle->phi, 2), 0.5);
    //std::cout<<"gen particle "<<igen<< " of "<<genParticles.size()<<" with id: "<< genParticle->id<<" and status: "<<genParticle->status<<" and mother: "<<genParticle->motherId<<" and dR: "<<drtemp<<std::endl;
    if(drtemp < dR){
      dR = drtemp;
      gp=genParticle;
    }
    if(drtemp<0.1 && fabs(genParticle->id)==11 && genParticle->status==1) nearbyEl=true;
  }

  if( !nearbyEl) return gp; //return the closest particle unless there is a nearby final state electron
  else{//run through again and only check electrons
    //reset dR
    dR=999;
    //std::cout<<"nearby electron"<<std::endl;
    for(unsigned int igen=0; igen<genParticles.size(); igen++){
      TGenParticle* genParticle = genParticles.at(igen);
      if(fabs(genParticle->id)!=11 || genParticle->status!=1) continue; //only check final state electrons
      float drtemp = pow( pow( el->eta - genParticle->eta, 2 ) + pow( el->phi - genParticle->phi, 2), 0.5);      
      if(drtemp < dR){
	dR = drtemp;
	gp=genParticle;
      }
    }//end loop over gen electrons
  
  }

  return gp;

}

TGenParticle* getMatchedGenParticle(TMuon* mu, std::vector<TGenParticle*> genParticles){


  TGenParticle* gp=0;

  bool nearbyMu = false;

  float dR=999;
  //run through gen particle collection:
  for(unsigned int igen=0; igen<genParticles.size(); igen++){
    TGenParticle* genParticle = genParticles.at(igen);
    float drtemp = pow( pow( mu->eta - genParticle->eta, 2 ) + pow( mu->phi - genParticle->phi, 2), 0.5);
    //std::cout<<"gen particle "<<igen<< " of "<<genParticles.size()<<" with id: "<< genParticle->id<<" and status: "<<genParticle->status<<" and mother: "<<genParticle->motherId<<" and dR: "<<drtemp<<std::endl;
    if(drtemp < dR){
      dR = drtemp;
      gp=genParticle;
    }
    if(drtemp<0.1 && fabs(genParticle->id)==13 && genParticle->status==1) nearbyMu = true;
  }

  if(!nearbyMu) return gp;
  else{
    //reset dr
    dR=999;
    for(unsigned int igen=0; igen<genParticles.size(); igen++){
      TGenParticle* genParticle = genParticles.at(igen);
      if( fabs(genParticle->id)!=13 || genParticle->status!=1) continue; //only loop over final state muons
      float drtemp = pow( pow( mu->eta - genParticle->eta, 2 ) + pow( mu->phi - genParticle->phi, 2), 0.5);
      if(drtemp < dR){
	dR = drtemp;
	gp=genParticle;
      }
      if(drtemp<0.1 && fabs(genParticle->id)==13 && genParticle->status==1) nearbyMu = true;
    }
  
  }

  return gp;

}
