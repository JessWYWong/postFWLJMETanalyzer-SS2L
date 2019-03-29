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
#include <map>
#include <string>
#include <sstream> 
#include <algorithm>
#include "../plugins/Macros.cc"

//helper functions
TLepton* makeTagLepton(std::vector<TMuon*> muons,std::vector<TElectron*> electrons,bool Muons,bool mc,bool FiftyNs,std::string ID);
std::vector<TLepton*> makeProbeLeptons(TLepton* tag, std::vector<TMuon*> muons, std::vector<TElectron*> electrons, bool Muons, bool mc, bool FiftyNs, std::string ID);
std::vector<TLepton*> findBestPair(TLepton* tag, std::vector<TLepton*> probes);
std::vector<TLepton*> makeAramLeptons(std::vector<TMuon*> muons,std::vector<TElectron*> electrons,bool MuonChannel,bool mc,bool FiftyNs,std::string ID);
bool sortByPhi(TLepton* lep1, TLepton* lep2){return lep1->phi > lep2->phi;};
bool sortByPt(TLepton* lep1, TLepton* lep2){return lep1->pt > lep2->pt;};

//A script to get the prompt rate for electrons and muons. Usage is ./PromptRate.o <Data,MC> <El,Mu> 

int main(int argc, char* argv[]){

  bool isoTrig = false;
  std::cout << "isoTrig ="<< isoTrig << std::endl;

  if(argc<4){
    std::cout<<"Need to specify whether running on Data or MC and whether running for electrons or muons as well as the ID used. The four possible ways of running are\n"
	     <<"./PromptRate.o Data El ID\n"
	     <<"./PromptRate.o Data Mu ID\n"
	     <<"./PromptRate.o MC El ID\n"
	     <<"./PromptRate.o MC Mu ID\n\n";
    std::cout<<"The IDs for electrons are: CBTight, CBLoose, MVATight, MVALoose. The IDS for muons are: CBTight, CBLoose."<<std::endl;
    return 0;
  }

  std::string argv1 = argv[1];
  std::string argv2 = argv[2];
  std::string ID = argv[3];
  std::string argv4 = argv[4];

  bool correctusage=false;
  if(argc>=4 && (argv1.find("Data")!=std::string::npos || argv1.find("MC")!=std::string::npos ) && (argv2.find("El")!=std::string::npos || argv2.find("Mu")!=std::string::npos)  ) correctusage=true;
  if(!correctusage){
    std::cout<<"Need to specify whether running on Data or MC and whether running for electrons or muons. The four possible ways of running are\n"
	     <<"./PromptRate.o Data El\n"
	     <<"./PromptRate.o Data Mu\n"
	     <<"./PromptRate.o MC El\n"
	     <<"./PromptRate.o MC Mu\n";
    return 0;
  }

  //get filename based on Data/MC
  std::string filename;
  bool data;

  //get channel based on El/Mu
  bool MuonChannel;
  if(argv2=="Mu") MuonChannel=true;
  else MuonChannel=false;
  if(argv1=="Data") {
    data=true;
    //std::string filedir = "LJMet94x_2lepTT_2017datasets_2019_1_13_rizki_hadds";
    std::string filedir = "LJMet94x_2lepTT_2017datasets_2019_3_15_rizki_hadds";
    if(MuonChannel){
      if(argv4=="2017B") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleMuon_RRB.root";
      else if(argv4=="2017C") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleMuon_RRC.root";
      else if(argv4=="2017D") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleMuon_RRD.root";
      else if(argv4=="2017E") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleMuon_RRE.root";
      else if(argv4=="2017F") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleMuon_RRF.root";
      else if(argv4=="2017F_1") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleMuon_RRF_1.root";
      else if(argv4=="2017F_2_1") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleMuon_RRF_2_1.root";
      else if(argv4=="2017F_2_2") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleMuon_RRF_2_2.root";
      else if(argv4=="2017F_2_3") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleMuon_RRF_2_3.root";
      else if(argv4=="2017F_2_4") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleMuon_RRF_2_4.root";
      else if(argv4=="2017F_2_5") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleMuon_RRF_2_5.root";
    }
    else{            
      if(argv4=="2017B") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleEG_RRB.root";
      else if(argv4=="2017C") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleEG_RRC.root";
      else if(argv4=="2017D") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleEG_RRD.root";
      else if(argv4=="2017E") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleEG_RRE.root";
      else if(argv4=="2017F") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleEG_RRF.root";

    }
  }
  else {filename="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/Nov17/ljmet_trees/ljmet_DYJets.root"; data=false;}
  bool FiftyNs=data;
  //make filename for output root file
  std::string outname;
  if(MuonChannel){
    if(data)outname="PromptRate_Data_"+argv4+"_Muons_"+ID+"_SortByPhi.root"; 
    else outname="PromptRate_MC_Muons_"+ID+"_SortByPhi.root"; 
  }
  else{
    if(data)outname="PromptRate_Data_"+argv4+"_Electrons_"+ID+"_SortByPhi.root"; 
    else outname="PromptRate_MC_Electrons_"+ID+"_SortByPhi.root"; 
  }

  //open output file
  TFile* fout= new TFile(outname.c_str(),"RECREATE");

  //output tree
  TTree* outTree = new TTree("FakeRate","FakeRate");
  float LepPt_,LepEta_,LepPhi_,LepE_,LepMiniIso_,LepMinDR_,LepSusyIso_;
  int LepIsLoose_,LepIsTight_,LepCharge_;
  outTree->Branch("LepPt",&LepPt_);
  outTree->Branch("LepEta",&LepEta_);
  outTree->Branch("LepPhi",&LepPhi_);
  outTree->Branch("LepE",&LepE_);
  outTree->Branch("LepCharge",&LepCharge_);
  outTree->Branch("LepMiniIso",&LepMiniIso_);
  outTree->Branch("LepSusyIso",&LepSusyIso_);
  outTree->Branch("LepIsLoose",&LepIsLoose_);
  outTree->Branch("LepIsTight",&LepIsTight_);
  outTree->Branch("LepMinDR",&LepMinDR_);


  //get tree reader to read in data
  TreeReader* tr= new TreeReader(filename.c_str(),!data,false);
  TTree* t=tr->tree;

  //initialize needed histograms
  TH1F* ptNumHist_Aram = new TH1F("ptNumHist_Aram","p_{T} of Tight Leptons",20,0,600);
  TH1F* ptDenHist_Aram = new TH1F("ptDenHist_Aram","p_{T} of Loose Leptons",20,0,600);
  TH1F* etaNumHist_Aram = new TH1F("etaNumHist_Aram","#eta of Tight Leptons",12,-5,5);
  TH1F* etaDenHist_Aram = new TH1F("etaDenHist_Aram","#eta of Loose Leptons",12,-5,5);

  //pt bins
  float ptbins[11] = {40,50,70,100,130,160,200,250,300,400,600};


  //initialize needed histograms
  TH1F* ptNumHist = new TH1F("ptNumHist","p_{T} of Tight Leptons",20,0,600);
  TH1F* ptDenHist = new TH1F("ptDenHist","p_{T} of Loose Leptons",20,0,600);
  TH1F* etaNumHist = new TH1F("etaNumHist","#eta of Tight Leptons",12,-5,5);
  TH1F* etaDenHist = new TH1F("etaDenHist","#eta of Loose Leptons",12,-5,5);

  TH1F* pairMassHist_pass = new TH1F("pairMassHist_pass","Best Pair Mass Passing Probe",40,50,130);
  TH1F* pairMassHist_fail = new TH1F("pairMassHist_fail","Best Pair Mass Failing Probe",40,50,130);
  TH1F* pairMassHist_all = new TH1F("pairMassHist_all","Best Pair Mass All Probes",40,50,130);

  //get number of entries and start event loop
  int nEntries = t->GetEntries();
  for(int ient=0; ient<nEntries; ient++){

    tr->GetEntry(ient);

    //if muon channel require muon triggers - IsoTrig are triggers used by trilepton, nonIsoHT are used by SSDL (as used by AN2018_230).
    bool passTrig=false;
    if(data) passTrig=true;
    else{
      if(MuonChannel){
      	if(isoTrig){
          if(tr->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8) passTrig=true;
      	}
      	else{
          if(tr->HLT_DoubleMu4_Mass8_DZ_PFHT350) passTrig=true;      	
      	}
      }
      else{
      	if(isoTrig){
          if(tr->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v) passTrig=true;
      	}
      	else{
          if(tr->HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT350_v) passTrig=true;
        }
      }
    }
    if(!passTrig) continue;

    if(ient % 100000==0) std::cout<<"Completed "<<ient<<" out of "<<nEntries<<" events"<<std::endl;

    //make tag lepton
    TLepton* tagLepton = makeTagLepton(tr->allMuons,tr->allElectrons,MuonChannel,!data,FiftyNs, ID);
    
    //if no tag lepton found in the event a dummy is returned with pt==-999 so skip event if it is this value (i.e. if no tag lepton found in event)
    if(tagLepton->pt==-999) continue;
    std::vector<TLepton*> probeLeptons = makeProbeLeptons(tagLepton,tr->allMuons,tr->allElectrons,MuonChannel,!data,FiftyNs, ID);
    
    std::vector<TLepton*> leptons = findBestPair(tagLepton,probeLeptons); //only returns two leptons if a pair is found within z window
    
    if(leptons.size()!=2) continue; //skip if no good pair found

    //get pair mass
    float pairMass = (leptons.at(0)->lv + leptons.at(1)->lv).M();
    pairMassHist_all->Fill(pairMass,tr->MCWeight);
    
    //revert to Aram's method
    std::vector<TLepton*> AramLeptons = makeAramLeptons(tr->allMuons,tr->allElectrons,MuonChannel,!data,FiftyNs, ID);
    //make sure we got at least two leptons
    if(AramLeptons.size()>=2){
      //make sure at least one is tight
      if(AramLeptons.at(0)->Tight || AramLeptons.at(1)->Tight){
	//make sure mass is with Z mass window
	float AramPairMass = (AramLeptons.at(0)->lv + AramLeptons.at(1)->lv).M();
	if(AramPairMass > 81 && AramPairMass<101){
	  ptDenHist_Aram->Fill(0.,tr->MCWeight);
	  if(AramLeptons.at(0)->Tight && AramLeptons.at(1)->Tight) ptNumHist_Aram->Fill(0.,tr->MCWeight);
	  etaDenHist_Aram->Fill(0.,tr->MCWeight);
	  if(AramLeptons.at(0)->Tight && AramLeptons.at(1)->Tight) etaNumHist_Aram->Fill(0.,tr->MCWeight);
	}//end check on mass
      }//end check on at least one tight
    }//end checl on number of Aram Leptons

    //fill tree
    TLepton* probe = leptons.at(1);
    LepPt_ = probe->pt;
    LepEta_ = probe->eta;
    LepPhi_ = probe->phi;
    LepE_ = probe->energy;
    LepCharge_ = probe->charge;
    LepMiniIso_ = probe->miniIso;
    LepSusyIso_ = probe->susyIso;
    LepIsTight_ = probe->Tight;
    LepIsLoose_ = probe->Loose;

    //find minDR
    float minDR=999;
    for(unsigned int i=0; i< tr->cleanedAK4Jets.size();i++){
      float drtemp = pow( pow(probe->eta - tr->cleanedAK4Jets.at(i)->eta, 2) + pow( probe->phi - tr->cleanedAK4Jets.at(0)->phi, 2) , 0.5);
      if(drtemp<minDR) minDR = drtemp;
    }
    LepMinDR_ = minDR;
    outTree->Fill();


    //now fill histograms, tag lepton will always be first so get element 1 for probe
    ptDenHist->Fill(leptons.at(1)->pt,tr->MCWeight);
    if(leptons.at(1)->Tight){
      ptNumHist->Fill(leptons.at(1)->pt,tr->MCWeight);
      pairMassHist_pass->Fill(pairMass,tr->MCWeight);
    }
    else pairMassHist_fail->Fill(pairMass,tr->MCWeight);
    etaDenHist->Fill(leptons.at(1)->eta,tr->MCWeight);
    if(leptons.at(1)->Tight) etaNumHist->Fill(leptons.at(1)->eta,tr->MCWeight);

  }//end event loop

  std::cout<<"Mean of num pt hist is: "<<ptNumHist->GetMean()<<std::endl;
  std::cout<<"Mean of den pt hist is: "<<ptDenHist->GetMean()<<std::endl;
  std::cout<<"Mean of num eta hist is: "<<etaNumHist->GetMean()<<std::endl;
  std::cout<<"Mean of den eta hist is: "<<etaDenHist->GetMean()<<std::endl;

  //make tgraphs for promptrate
  TGraphAsymmErrors* ptGraph = new TGraphAsymmErrors(ptNumHist,ptDenHist);
  TGraphAsymmErrors* etaGraph = new TGraphAsymmErrors(etaNumHist,etaDenHist);

  //append everything to output file
  fout->WriteTObject(outTree);
  fout->Append(ptNumHist);
  fout->Append(ptDenHist);
  fout->Append(etaNumHist);
  fout->Append(etaDenHist);
  fout->Append(ptNumHist_Aram);
  fout->Append(ptDenHist_Aram);
  fout->Append(etaNumHist_Aram);
  fout->Append(etaDenHist_Aram);
  fout->Append(ptGraph);
  fout->Append(etaGraph);
  fout->Write();


  return 0;
}


std::vector<TLepton*> makeProbeLeptons(TLepton* tag, std::vector<TMuon*> muons, std::vector<TElectron*> electrons, bool Muons,bool mc, bool FiftyNs, std::string ID){

  std::vector<TLepton*> Leptons;

  if(Muons){
    //fill with  muons
    for(unsigned int uimu=0; uimu<muons.size(); uimu++){
      TMuon* imu = muons.at(uimu);
      TLepton* iLep = new TLepton(imu->pt,imu->eta,imu->phi,imu->energy,imu->charge,imu->relIso,imu->miniIso,imu->susyIso);
      //skip if same as tag
      if(imu->pt==tag->pt && imu->eta==tag->eta && imu->phi==tag->eta) continue;
      //skip if probe has same sign charge as tag
      if(imu->charge == tag->charge) continue;
      if(ID=="CBTight"){
	iLep->Tight=imu->cutBasedTight();
	iLep->Loose=imu->cutBasedLoose();
      }
      else if(ID=="CBTightMiniIso"){
	iLep->Tight=imu->cutBasedTight_NoIso();
	iLep->Loose=imu->cutBasedLooseMiniIso();
      }
      else if(ID=="CBLoose"){
	iLep->Tight=imu->cutBasedLoose();
	iLep->Loose=true; //in case 'loose ID' is specified as 'tight', take any muon as loose ID
      }
    else if(ID=="CBTightMiniIsoTight"){ //added by rizki
      iLep->Tight=imu->cutBasedTightMiniIsoTight();
      iLep->Loose=imu->cutBasedLooseMiniIso();
    }
      iLep->isMu = true;
      iLep->isEl = false;
      //only save if at least loose
      if(iLep->Loose){
	//apply pt cut
	if(iLep->pt>30) Leptons.push_back(iLep);
      }
    }
  }

  else{
    //fill with  electrons
    for(unsigned int uiel=0; uiel<electrons.size(); uiel++){
      TElectron* iel = electrons.at(uiel);
      TLepton* iLep = new TLepton(iel->pt,iel->eta,iel->phi,iel->energy,iel->charge,iel->relIsoEA,iel->miniIso,iel->susyIso);
      //skip if same as tag
      if(iel->pt==tag->pt && iel->eta==tag->eta && iel->phi==tag->eta) continue;
      if(ID=="CBTight"){
	iLep->Tight=iel->cutBasedTight25nsSpring15MC();
	iLep->Loose=iel->cutBasedLoose25nsSpring15MC();
      }
      else if(ID=="CBTightRC"){
	iLep->Tight=iel->cutBasedTight25nsSpring15MCRC();
	iLep->Loose=iel->cutBasedLoose25nsSpring15MCRC();
      }
      else if(ID=="CBLoose"){
	iLep->Tight=iel->cutBasedLoose25nsSpring15MC();
	iLep->Loose=true;
      }
      else if(ID=="MVATightNew"){
	iLep->Tight=iel->mvaTightNew();
	iLep->Loose=iel->mvaLooseNew();
      }
      else if(ID=="MVATightNewRC"){
	iLep->Tight=iel->mvaTightNewRC();
	iLep->Loose=iel->mvaLooseNewRC();
      }
      else if(ID=="MVATight"){
	iLep->Tight=iel->mvaTightIso();
	iLep->Loose=iel->mvaLooseIso();
      }
      else if(ID=="MVATightRC"){
	iLep->Tight=iel->mvaTightRC();
	iLep->Loose=iel->mvaLooseRC();
      }
      else if(ID=="MVA2016TightRC"){
	iLep->Tight=iel->mva2016TightRC();
	iLep->Loose=iel->mvaJulieLooseRC();
      }
      else if(ID=="MVA80XTightRC"){
	iLep->Tight=iel->mva80XTightRC();
	iLep->Loose=iel->mva80XLooseRC();
      }
      else if(ID=="MVAJulieTightRC"){
	iLep->Tight=iel->mvaJulieTightRC();
	iLep->Loose=iel->mvaJulieLooseRC();
      }
      else if(ID=="MVAJulieNewTightRC"){
	iLep->Tight=iel->mvaJulieNewTightRC();
	iLep->Loose=iel->mvaJulieLooseRC();
      }
      else if(ID=="MVATightNoIso"){
	iLep->Tight=iel->mvaTight();
	iLep->Loose=iel->mvaLoose();
      }
      else if(ID=="MVALoose"){
	iLep->Tight=iel->mvaLooseIso();
	iLep->Loose=true;
      }
      else if(ID=="MVALooseNoIso"){
	iLep->Tight=iel->mvaLoose();
	iLep->Loose=true;
      }
      else if(ID=="MVATightCC"){
	iLep->Tight=iel->mvaTightCCIso();
	iLep->Loose=iel->mvaLooseCCIso();
      }
      else if(ID=="MVATightCCNoIso"){
	iLep->Tight=iel->mvaTightCC();
	iLep->Loose=iel->mvaLooseCC();
      }
      else if(ID=="MVALooseCC"){
	iLep->Tight=iel->mvaLooseCCIso();
	iLep->Loose=true;
      }
      else if(ID=="MVALooseNoIso"){
	iLep->Tight=iel->mvaLoose();
	iLep->Loose=true;
      }
      else if(ID=="SUSYTight"){
	iLep->Tight=iel->susyTight();
	iLep->Loose=iel->susyLoose();
      }
      else if(ID=="MVA2017TightIsoRC"){ //added by rizki
		  iLep->Tight=iel->mva94XTightV1_Iso_RC();
		  iLep->Loose=iel->mva94XLooseV1_Iso_RC();
      }
      else if(ID=="MVA2017TightRC"){ //added by rizki
		  iLep->Tight=iel->mva94XTightV1_RC();
		  iLep->Loose=iel->mva94XLooseV1_RC();
      }
      
      iLep->isMu = false;
      iLep->isEl = true;
      //save if loose
      if(iLep->Loose){
	//apply pt cut
	if(iLep->pt>30) Leptons.push_back(iLep);
      }
    }
  }

  return Leptons;

}

std::vector<TLepton*> findBestPair(TLepton* tag, std::vector<TLepton*> probes){

  std::vector<TLepton*> leps;

  //get pair of leptons closest to z mass;
  float zmass = 91.1;
  float massDiff=9999;
  float pairmass=-9999;

  int index = 0;
  int dummy = 0;

  for(std::vector<TLepton*>::size_type jlep=0; jlep<probes.size(); jlep++){
    
    pairmass = (tag->lv + probes.at(jlep)->lv).M();
   
    if(fabs(zmass-pairmass)<massDiff){
      massDiff = fabs(zmass-pairmass);
      index=dummy;
    }
    //iterate dummy index
    dummy++;
  }//end loop over probes
  
  
  //check that leptons are in Zpeak
  bool zpeak= fabs(massDiff)<10 ? true : false;

  
  if(zpeak){
    TLepton* probe = probes.at(index);
    leps.push_back(tag);leps.push_back(probe);
  }
  
  return leps;

}

TLepton* makeTagLepton(std::vector<TMuon*> muons,std::vector<TElectron*> electrons,bool Muons,bool mc,bool FiftyNs,std::string ID){
    
  std::vector<TLepton*> Leptons;

  if(Muons){
    //fill with  muons
    for(unsigned int uimu=0; uimu<muons.size(); uimu++){
      TMuon* imu = muons.at(uimu);
      TLepton* iLep = new TLepton(imu->pt,imu->eta,imu->phi,imu->energy,imu->charge,imu->relIso,imu->miniIso,imu->susyIso);
      if(ID=="CBTight"){
	iLep->Tight=imu->cutBasedTight();
	iLep->Loose=imu->cutBasedLoose();
      }
      else if(ID=="CBTightMiniIso"){
	iLep->Tight=imu->cutBasedTightMiniIso();
	iLep->Loose=imu->cutBasedLooseMiniIso();
      }
      else if(ID=="CBLoose"){
	iLep->Tight=imu->cutBasedLoose();
	iLep->Loose=true; //in case 'loose ID' is specified as 'tight', take any muon as loose ID
      }
    else if(ID=="CBTightMiniIsoTight"){
      iLep->Tight=imu->cutBasedTightMiniIsoTight();
      iLep->Loose=imu->cutBasedLooseMiniIso();
    }
      iLep->isMu = true;
      iLep->isEl = false;
      //only save if tight
      if(iLep->Tight){
	//apply pt cut
	if(iLep->pt>30) Leptons.push_back(iLep);
      }
    }
  }

  else{
    //fill with  electrons
    for(unsigned int uiel=0; uiel<electrons.size(); uiel++){
      TElectron* iel = electrons.at(uiel);
      TLepton* iLep = new TLepton(iel->pt,iel->eta,iel->phi,iel->energy,iel->charge,iel->relIsoEA,iel->miniIso,iel->susyIso);

      if(ID=="CBTight"){
	iLep->Tight=iel->cutBasedTight25nsSpring15MC();
	iLep->Loose=iel->cutBasedLoose25nsSpring15MC();
      }
      else if(ID=="CBLoose"){
	iLep->Tight=iel->cutBasedLoose25nsSpring15MC();
	iLep->Loose=true;
      }
      else if(ID=="MVATightNew"){
	iLep->Tight=iel->mvaTightNew();
	iLep->Loose=iel->mvaLooseNew();
      }
      else if(ID=="MVATightNewRC"){
	iLep->Tight=iel->mvaTightNewRC();
	iLep->Loose=iel->mvaLooseNewRC();
      }
      else if(ID=="MVATight"){
	iLep->Tight=iel->mvaTightIso();
	iLep->Loose=iel->mvaLooseIso();
      }
      else if(ID=="MVATightNoIso"){
	iLep->Tight=iel->mvaTight();
	iLep->Loose=iel->mvaLoose();
      }
      else if(ID=="MVALoose"){
	iLep->Tight=iel->mvaLooseIso();
	iLep->Loose=true;
      }
      else if(ID=="MVALooseNoIso"){
	iLep->Tight=iel->mvaLoose();
	iLep->Loose=true;
      }
      else if(ID=="MVATightCC"){
	iLep->Tight=iel->mvaTightCCIso();
	iLep->Loose=iel->mvaLooseCCIso();
      }
      else if(ID=="MVATightCCNoIso"){
	iLep->Tight=iel->mvaTightCC();
	iLep->Loose=iel->mvaLooseCC();
      }
      else if(ID=="MVALooseCC"){
	iLep->Tight=iel->mvaLooseCCIso();
	iLep->Loose=true;
      }
      else if(ID=="MVALooseCCNoIso"){
	iLep->Tight=iel->mvaLooseCC();
	iLep->Loose=true;
      }
      else if(ID=="MVATightRC"){
	iLep->Tight=iel->mvaTightRCIso();
	iLep->Loose=iel->mvaLooseRCIso();
      }
      else if(ID=="MVALooseRC"){
	iLep->Tight=iel->mvaLooseRCIso();
	iLep->Loose=true;
      }
      else if(ID=="SUSYTight"){
	iLep->Tight=iel->susyTight();
	iLep->Loose=iel->susyLoose();
      }
      else if(ID=="SUSYLoose"){
	iLep->Tight=iel->susyLoose();
	iLep->Loose=true;
      }
      else if(ID=="SUSYTightRC"){
	iLep->Tight=iel->susyTightRC();
	iLep->Loose=iel->susyLooseRC();
      }
      else if(ID=="MVA2017TightRC"){
		  iLep->Tight=iel->mva94XTightV1_Iso_RC();
		  iLep->Loose=iel->mva94XLooseV1_Iso_RC();
      }

      iLep->isMu = false;
      iLep->isEl = true;
      //only save if tight
      if(iLep->Tight){
	//apply pt cut
	if(iLep->pt>30) Leptons.push_back(iLep);
      }
    }
  }

  //make sure there is at least one tight lepton in the event or return dummy
  TLepton* dummy = new TLepton(-999,-999,-999,-999,-999,-999,-999,-999);
  if(Leptons.size()==0) return dummy;
  
  //if at least one tight sort the leptons by phi
  std::sort(Leptons.begin(),Leptons.end(),sortByPhi);
  //return the leading-in-phi lepton
  TLepton* tag = Leptons.at(0);
  
  return tag;
  
 
}



std::vector<TLepton*> makeAramLeptons(std::vector<TMuon*> muons,std::vector<TElectron*> electrons,bool MuonChannel,bool mc,bool FiftyNs,std::string ID){
  std::vector<TLepton*> Leptons;

  if(MuonChannel){
    //fill with  muons
    for(unsigned int uimu=0; uimu<muons.size(); uimu++){
      TMuon* imu = muons.at(uimu);
      TLepton* iLep = new TLepton(imu->pt,imu->eta,imu->phi,imu->energy,imu->charge,imu->relIso,imu->miniIso,imu->susyIso);

      if(ID=="CBTight"){
	iLep->Tight=imu->cutBasedTight();
	iLep->Loose=imu->cutBasedLoose();
      }
      else if(ID=="CBTightMiniIso"){
	iLep->Tight=imu->cutBasedTightMiniIso();
	iLep->Loose=imu->cutBasedLooseMiniIso();
      }
      else if(ID=="CBLoose"){
	iLep->Tight=imu->cutBasedLoose();
	iLep->Loose=true; //in case 'loose ID' is specified as 'tight', take any muon as loose ID
      }
    else if(ID=="CBTightMiniIsoTight"){
      iLep->Tight=imu->cutBasedTightMiniIsoTight();
      iLep->Loose=imu->cutBasedLooseMiniIso();
    }
      iLep->isMu = true;
      iLep->isEl = false;
      //only save if at least loose
      if(iLep->Loose){
	//apply pt cut
	if(iLep->pt>40) Leptons.push_back(iLep);
      }
    }
  }

  else{
    //fill with  electrons
    for(unsigned int uiel=0; uiel<electrons.size(); uiel++){
      TElectron* iel = electrons.at(uiel);
      TLepton* iLep = new TLepton(iel->pt,iel->eta,iel->phi,iel->energy,iel->charge,iel->relIsoEA,iel->miniIso,iel->susyIso);
      if(ID=="CBTight"){
	iLep->Tight=iel->cutBasedTight25nsSpring15MC();
	iLep->Loose=iel->cutBasedLoose25nsSpring15MC();
      }
      else if(ID=="CBLoose"){
	iLep->Tight=iel->cutBasedLoose25nsSpring15MC();
	iLep->Loose=true;
      }
      else if(ID=="MVATight"){
	iLep->Tight=iel->mvaTightIso();
	iLep->Loose=iel->mvaLooseIso();
      }
      else if(ID=="MVATightNoIso"){
	iLep->Tight=iel->mvaTight();
	iLep->Loose=iel->mvaLoose();
      }
      else if(ID=="MVALoose"){
	iLep->Tight=iel->mvaLooseIso();
	iLep->Loose=true;
      }
      else if(ID=="MVALooseNoIso"){
	iLep->Tight=iel->mvaLoose();
	iLep->Loose=true;
      }
      else if(ID=="MVATightCC"){
	iLep->Tight=iel->mvaTightCCIso();
	iLep->Loose=iel->mvaLooseCCIso();
      }
      else if(ID=="MVATightCCNoIso"){
	iLep->Tight=iel->mvaTightCC();
	iLep->Loose=iel->mvaLooseCC();
      }
      else if(ID=="MVALooseCC"){
	iLep->Tight=iel->mvaLooseCCIso();
	iLep->Loose=true;
      }
      else if(ID=="MVALooseCCNoIso"){
	iLep->Tight=iel->mvaLooseCC();
	iLep->Loose=true;
      }
      else if(ID=="MVATightRC"){
	iLep->Tight=iel->mvaTightRCIso();
	iLep->Loose=iel->mvaLooseRCIso();
      }
      else if(ID=="MVALooseRC"){
	iLep->Tight=iel->mvaLooseRCIso();
	iLep->Loose=true;
      }
      else if(ID=="SUSYTight"){
	iLep->Tight=iel->susyTight();
	iLep->Loose=iel->susyLoose();
      }
      else if(ID=="SUSYLoose"){
	iLep->Tight=iel->susyLoose();
	iLep->Loose=true;
      }
      else if(ID=="SUSYTightRC"){
	iLep->Tight=iel->susyTightRC();
	iLep->Loose=iel->susyLooseRC();
      }      
    else if(ID=="MVA2017TightRC"){
      iLep->Tight=iel->mva94XTightV1_Iso_RC();
      iLep->Loose=iel->mva94XLooseV1_Iso_RC();
    }
      
      iLep->isMu = false;
      iLep->isEl = true;
      //save if loose
      if(iLep->Loose){
	//apply pt cut
	if(iLep->pt>40) Leptons.push_back(iLep);
      }
    }
  }

  //sort by pt
  std::sort(Leptons.begin(), Leptons.end(), sortByPt);

  //now just take highest pt opposite charge pair
  std::vector<TLepton*> AramLeptons;
  bool found = false;
  for(std::vector<TLepton*>::size_type ilep=0; ilep<Leptons.size(); ilep++){
    for(std::vector<TLepton*>::size_type jlep=ilep+1; jlep<Leptons.size(); jlep++){
      if(Leptons.at(ilep)->charge != Leptons.at(jlep)->charge){
	AramLeptons.push_back(Leptons.at(ilep));AramLeptons.push_back(Leptons.at(jlep));
	found=true;
	break;
      }
    }//end inner lepton loop
    if(found) break;
  }//end outer lepton loop
  return AramLeptons;


}
