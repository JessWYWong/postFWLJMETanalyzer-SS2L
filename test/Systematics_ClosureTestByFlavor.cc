#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <map>
#include <string>
#include <sstream> 
#include <algorithm>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "TChain.h"

#include "../interface/TreeReader.h"
#include "JetAnalyzer.cc"
#include "GenAnalyzer.cc"
#include "../interface/TreeMaker.h"
#include "../plugins/Macros.cc"
//#include "../plugins/EventFilterFromFile_MuonEG.cc"
//#include "../plugins/EventFilterFromFile_DoubleMu.cc"
//#include "../plugins/EventFilterFromFile_DoubleEG.cc"

bool testing = false;

std::vector<TLepton*> makeLeptons(std::vector<TMuon*>, std::vector<TElectron*>, float, std::string, std::string, bool,std::vector<TGenParticle*>);
std::vector<TLepton*> makeSSLeptons(std::vector<TLepton*>);
bool checkSameSignLeptons(std::vector<TLepton*>);
bool checkSecondaryZVeto(std::vector<TLepton*> leps, std::vector<TMuon*> muons, std::vector<TElectron*> electrons);
std::vector<TLepton*> pruneSSLep(std::vector<TLepton*> allLeps, std::vector<TLepton*> ssLeps);
bool sortByPt(TLepton* lep1, TLepton* lep2){return lep1->pt > lep2->pt;};
TGenParticle* getMatchedGenParticle(TElectron* el, std::vector<TGenParticle*> genParticles);
TGenParticle* getMatchedGenParticle(TMuon* mu, std::vector<TGenParticle*> genParticles);
void printParticle(TLepton*);

//the point of this script is to produce HT histograms for events where one of the leptons is prompt and the second is fake. It will produce one HT histogram for each source of fake lepton

int main(int argc, char* argv[]){


  bool bg_np = true;
  std::string elID = "MVA2017TightV2IsoTightRC";
  std::string muID = "CBTightMiniIsoTight";

  //get input sample
  std::string sample(argv[1]);
  std::string inputname, outname;
  //std::string eosname="root://cmseos.fnal.gov//eos/uscms/store/user/lpctlbsm/clint/Spring16/25ns/Jan09/ljmet_trees/";
  std::string eosname="/eos/uscms/store/user/lpcljm/FWLJMET102X_2lepFakeRate2017_wywong_082020_hadds/";
  if(sample=="TTTo2L2Nu"){ outname="SmartClosure_TTTo2L2Nu.root"; inputname=eosname+"TTTo2L2Nu.root";}
  if(sample=="TTTo2L2Nu_1"){ outname="SmartClosure_TTTo2L2Nu_1.root"; inputname=eosname+"TTTo2L2Nu_1.root";}
  if(sample=="TTTo2L2Nu_2"){ outname="SmartClosure_TTTo2L2Nu_2.root"; inputname=eosname+"TTTo2L2Nu_2.root";}
  if(sample=="TTTo2L2Nu_3"){ outname="SmartClosure_TTTo2L2Nu_3.root"; inputname=eosname+"TTTo2L2Nu_3.root";}
  if(sample=="TTTo2L2Nu_4"){ outname="SmartClosure_TTTo2L2Nu_4.root"; inputname=eosname+"TTTo2L2Nu_4.root";}
  if(sample=="TTTo2L2Nu_5"){ outname="SmartClosure_TTTo2L2Nu_5.root"; inputname=eosname+"TTTo2L2Nu_5.root";}
  if(sample=="TTTo2L2Nu_6"){ outname="SmartClosure_TTTo2L2Nu_6.root"; inputname=eosname+"TTTo2L2Nu_6.root";}
  //if(sample=="TTbar_pt5"){ outname="SmartClosure_TTbar_pt5.root"; inputname=eosname+"ljmet_TTbar_pt5.root";}
  //if(sample=="TTbar_pt6"){ outname="SmartClosure_TTbar_pt6.root"; inputname=eosname+"ljmet_TTbar_pt6.root";}
  //if(sample=="TTbar_pt7"){ outname="SmartClosure_TTbar_pt7.root"; inputname=eosname+"ljmet_TTbar_pt7.root";}
  //if(sample=="TTbar_pt8"){ outname="SmartClosure_TTbar_pt8.root"; inputname=eosname+"ljmet_TTbar_pt8.root";}
  //if(sample=="TTbar_pt9"){ outname="SmartClosure_TTbar_pt9.root"; inputname=eosname+"ljmet_TTbar_pt9.root";}

  //treereader
  bool mc = true;
  TreeReader* tr = new TreeReader(inputname.c_str(),"ljmet/ljmet",mc,true);
  TTree* t = tr->tree;
  //output file
  TFile* fout = new TFile(outname.c_str(),"RECREATE");
  TTree* outTree = new TTree("ClosureTest","ClosureTest");
  float st,sumJetPt,lepEta,lepPt;
  int lepFlavor,flavorSource,observed;
  bool isEE,isEM,isMM;
  outTree->Branch("HT",&st);
  outTree->Branch("sumJetPt",&sumJetPt);
  outTree->Branch("LepPt",&lepPt);
  outTree->Branch("LepEta",&lepEta);
  outTree->Branch("LepFlavor",&lepFlavor);
  outTree->Branch("LepFlavorSource",&flavorSource);
  outTree->Branch("Observed",&observed);
  outTree->Branch("isEE", &isEE);
  outTree->Branch("isEM", &isEM);
  outTree->Branch("isMM", &isMM);
  //histograms
  TH1F* h_el_obs_light = new TH1F("h_el_obs_light","",1000,0,5000);
  TH1F* h_el_obs_charm = new TH1F("h_el_obs_charm","",1000,0,5000);
  TH1F* h_el_obs_bottom = new TH1F("h_el_obs_bottom","",1000,0,5000);
  TH1F* h_el_obs_fake = new TH1F("h_el_obs_fake","",1000,0,5000);
  TH1F* h_el_obs_unm = new TH1F("h_el_obs_unm","",1000,0,5000);
  TH1F* h_el_pred_light = new TH1F("h_el_pred_light","",1000,0,5000);
  TH1F* h_el_pred_charm = new TH1F("h_el_pred_charm","",1000,0,5000);
  TH1F* h_el_pred_bottom = new TH1F("h_el_pred_bottom","",1000,0,5000);
  TH1F* h_el_pred_fake = new TH1F("h_el_pred_fake","",1000,0,5000);
  TH1F* h_el_pred_unm = new TH1F("h_el_pred_unm","",1000,0,5000);

  TH1F* h_mu_obs_light = new TH1F("h_mu_obs_light","",1000,0,5000);
  TH1F* h_mu_obs_charm = new TH1F("h_mu_obs_charm","",1000,0,5000);
  TH1F* h_mu_obs_bottom = new TH1F("h_mu_obs_bottom","",1000,0,5000);
  TH1F* h_mu_obs_fake = new TH1F("h_mu_obs_fake","",1000,0,5000);
  TH1F* h_mu_obs_unm = new TH1F("h_mu_obs_unm","",1000,0,5000);
  TH1F* h_mu_pred_light = new TH1F("h_mu_pred_light","",1000,0,5000);
  TH1F* h_mu_pred_charm = new TH1F("h_mu_pred_charm","",1000,0,5000);
  TH1F* h_mu_pred_bottom = new TH1F("h_mu_pred_bottom","",1000,0,5000);
  TH1F* h_mu_pred_fake = new TH1F("h_mu_pred_fake","",1000,0,5000);
  TH1F* h_mu_pred_unm = new TH1F("h_mu_pred_unm","",1000,0,5000);

  //pt
  TH1F* h_pt_el_obs_light = new TH1F("h_pt_el_obs_light","",1000,0,5000);
  TH1F* h_pt_el_obs_charm = new TH1F("h_pt_el_obs_charm","",1000,0,5000);
  TH1F* h_pt_el_obs_bottom = new TH1F("h_pt_el_obs_bottom","",1000,0,5000);
  TH1F* h_pt_el_obs_fake = new TH1F("h_pt_el_obs_fake","",1000,0,5000);
  TH1F* h_pt_el_obs_unm = new TH1F("h_pt_el_obs_unm","",1000,0,5000);
  TH1F* h_pt_el_pred_light = new TH1F("h_pt_el_pred_light","",1000,0,5000);
  TH1F* h_pt_el_pred_charm = new TH1F("h_pt_el_pred_charm","",1000,0,5000);
  TH1F* h_pt_el_pred_bottom = new TH1F("h_pt_el_pred_bottom","",1000,0,5000);
  TH1F* h_pt_el_pred_fake = new TH1F("h_pt_el_pred_fake","",1000,0,5000);
  TH1F* h_pt_el_pred_unm = new TH1F("h_pt_el_pred_unm","",1000,0,5000);

  TH1F* h_pt_mu_obs_light = new TH1F("h_pt_mu_obs_light","",1000,0,5000);
  TH1F* h_pt_mu_obs_charm = new TH1F("h_pt_mu_obs_charm","",1000,0,5000);
  TH1F* h_pt_mu_obs_bottom = new TH1F("h_pt_mu_obs_bottom","",1000,0,5000);
  TH1F* h_pt_mu_obs_fake = new TH1F("h_pt_mu_obs_fake","",1000,0,5000);
  TH1F* h_pt_mu_obs_unm = new TH1F("h_pt_mu_obs_unm","",1000,0,5000);
  TH1F* h_pt_mu_pred_light = new TH1F("h_pt_mu_pred_light","",1000,0,5000);
  TH1F* h_pt_mu_pred_charm = new TH1F("h_pt_mu_pred_charm","",1000,0,5000);
  TH1F* h_pt_mu_pred_bottom = new TH1F("h_pt_mu_pred_bottom","",1000,0,5000);
  TH1F* h_pt_mu_pred_fake = new TH1F("h_pt_mu_pred_fake","",1000,0,5000);
  TH1F* h_pt_mu_pred_unm = new TH1F("h_pt_mu_pred_unm","",1000,0,5000);



  int nEntries = t->GetEntries();

  for(int ient=0; ient<nEntries; ient++){

    if(ient % 100000 ==0) std::cout<<"Completed "<<ient<<" out of "<<nEntries<<" events"<<std::endl;
    if(testing && ient > 2000000) break;
    
    tr->GetEntry(ient);
    

    //make vector of good Leptons change based on data/mc   
    std::vector<TLepton*> goodLeptons;
    goodLeptons = makeLeptons(tr->allMuons, tr->allElectrons,30.0,elID,muID,bg_np,tr->genParticles);

    //reorder the leptons by pt to remove flavor ordering
    std::sort(goodLeptons.begin(),goodLeptons.end(),sortByPt);
    bool samesign;


    samesign = checkSameSignLeptons(goodLeptons);


    if(!samesign) continue;


    //now make vector of same-sign leptons, for DY make vector containing opposite sign leptons closest to Z mass
    std::vector<TLepton*> vSSLep;
    vSSLep = makeSSLeptons(goodLeptons);
    
    //dummy check to make sure the vector got filled properly
    assert(vSSLep.size() > 1);

    //now make sure that at least one is tight for this test
    if(! (vSSLep.at(0)->Tight || vSSLep.at(1)->Tight) ) continue;

    //now prune the goodleptons of the ssleptons
    std::vector<TLepton*> vNonSSLep = pruneSSLep(goodLeptons,vSSLep);

    //now get dilepton mass
    float dilepMass = (vSSLep.at(0)->lv + vSSLep.at(1)->lv).M();

    float HT=0;
    HT+=vSSLep.at(0)->pt+vSSLep.at(1)->pt;
    for(unsigned int uijet=0; uijet<tr->allAK4Jets.size();uijet++){
      HT+=tr->allAK4Jets.at(uijet)->pt;
    }
    sumJetPt = 0;
    for(unsigned int uijet=0; uijet<tr->cleanedAK4Jets.size();uijet++){
      sumJetPt+=tr->cleanedAK4Jets.at(uijet)->pt;
    }

    //bools for channels
    bool mumu=false;
    bool elmu=false;
    bool elel=false;

    //ok at this point only have events with same-sign leptons, now let's do simple check to see how many of each channel we have:
    if(vSSLep.at(0)->isMu && vSSLep.at(1)->isMu){ mumu=true;}
    else if( ( vSSLep.at(0)->isEl && vSSLep.at(1)->isMu) || (vSSLep.at(0)->isMu && vSSLep.at(1)->isEl)){ elmu=true;}
    else {elel=true;}

    //since we have the two same-sign leptons, now make sure neither of them reconstructs with any other tight lepton in the event to form a Z
    bool secondaryZVeto = checkSecondaryZVeto(vSSLep,tr->looseMuons,tr->looseElectrons);
    if(secondaryZVeto) continue;

    isEE = elel;
    isEM = elmu;
    isMM = mumu;

    st = vSSLep.at(0)->pt + vSSLep.at(1)->pt;
    for(unsigned int j=0; j < vNonSSLep.size(); j++){
      st = st + vNonSSLep.at(j)->pt;
    }
    for(unsigned int j=0; j <tr->cleanedAK4Jets.size();j++){
      st = st + tr->cleanedAK4Jets.at(j)->pt;
    }

    //skip events in dielectron which reconstruct to z
    if(elel){
      if(dilepMass < 106 && dilepMass>76) continue;
    }

    //quarkonia veto
    if(!(dilepMass>20)) continue;

    if(vSSLep.at(0)->pt<40) continue; //skip events with leading lepton pt less than 40
    
    int nconst = tr->cleanedAK4Jets.size() + vNonSSLep.size();
    if(nconst<4) continue; //nconst cut

    //no need to split by channel, just need to fill histograms based on whether fake lepton is electron or muon

    bool p1 = vSSLep.at(0)->isPrompt;
    bool p2 = vSSLep.at(1)->isPrompt;
    bool t1 = vSSLep.at(0)->Tight;
    bool t2 = vSSLep.at(1)->Tight;
    //now skip events where both are prompt - this should never happen so let's add a printout
    if(p1 && p2){ std::cout<<"found two prompt same-sign electrons...."<<std::endl;
      continue;}
    //skip where both are fake
    else if(!(p1 || p2)){ continue;}
    //now we have one prompt and one fake

    if(p1){//p2 is fake
      //skip if prompt lepton isn't tight
      if(!t1) continue;
      //get flavor of lepton source
      TLepton::flavor source = vSSLep.at(1)->SourceFlavor;
      lepPt=vSSLep.at(1)->pt;
      lepEta=vSSLep.at(1)->eta;
      
      //separate into 'observed' where both are tight, and 'predicted' where fake lepton is loose only
      if(t2){ //lep2 is tight so this is 'observed'
	observed=1;
	//now fill observed histograms by flavor
	if(vSSLep.at(1)->isEl){	
	  lepFlavor=0;
	  if(source==TLepton::flavor::Light) {h_el_obs_light->Fill(st);h_pt_el_obs_light->Fill(vSSLep.at(1)->pt); flavorSource=0;}
	  if(source==TLepton::flavor::Charm) {h_el_obs_charm->Fill(st);h_pt_el_obs_charm->Fill(vSSLep.at(1)->pt);flavorSource=1;}
	  if(source==TLepton::flavor::Bottom) {h_el_obs_bottom->Fill(st);h_pt_el_obs_bottom->Fill(vSSLep.at(1)->pt);flavorSource=2;}
	  if(source==TLepton::flavor::Fake) {h_el_obs_fake->Fill(st);h_pt_el_obs_fake->Fill(vSSLep.at(1)->pt);flavorSource=3;}
	  if(source==TLepton::flavor::Unmatched) {h_el_obs_unm->Fill(st);h_pt_el_obs_unm->Fill(vSSLep.at(1)->pt);flavorSource=4;}
	}
	else{
	  lepFlavor=1;
	  if(source==TLepton::flavor::Light) {h_mu_obs_light->Fill(st); h_pt_mu_obs_light->Fill(vSSLep.at(1)->pt);flavorSource=0;}
	  if(source==TLepton::flavor::Charm) {h_mu_obs_charm->Fill(st);h_pt_mu_obs_charm->Fill(vSSLep.at(1)->pt);flavorSource=1;}
	  if(source==TLepton::flavor::Bottom) {h_mu_obs_bottom->Fill(st);h_pt_mu_obs_bottom->Fill(vSSLep.at(1)->pt);flavorSource=2;}
	  if(source==TLepton::flavor::Fake) {h_mu_obs_fake->Fill(st);h_pt_mu_obs_fake->Fill(vSSLep.at(1)->pt);flavorSource=3;}
	  if(source==TLepton::flavor::Unmatched) {h_mu_obs_unm->Fill(st);h_pt_mu_obs_unm->Fill(vSSLep.at(1)->pt);flavorSource=4;}
	}
      }//end observed
      //so do predicted
      else{
	observed=0;
	//now fill predicted histograms by flavor
	if(vSSLep.at(1)->isEl){	  
	  lepFlavor=0;
	  if(source==TLepton::flavor::Light) {h_el_pred_light->Fill(st);h_pt_el_pred_light->Fill(vSSLep.at(1)->pt);flavorSource=0;}
	  if(source==TLepton::flavor::Charm) {h_el_pred_charm->Fill(st);h_pt_el_pred_charm->Fill(vSSLep.at(1)->pt);flavorSource=1;}
	  if(source==TLepton::flavor::Bottom) {h_el_pred_bottom->Fill(st);h_pt_el_pred_bottom->Fill(vSSLep.at(1)->pt);flavorSource=2;}
	  if(source==TLepton::flavor::Fake) {h_el_pred_fake->Fill(st);h_pt_el_pred_fake->Fill(vSSLep.at(1)->pt);flavorSource=3;}
	  if(source==TLepton::flavor::Unmatched) {h_el_pred_unm->Fill(st);h_pt_el_pred_unm->Fill(vSSLep.at(1)->pt);flavorSource=4;}
	}
	else{
	  lepFlavor=1;
	  if(source==TLepton::flavor::Light) {h_mu_pred_light->Fill(st); h_pt_mu_pred_light->Fill(vSSLep.at(1)->pt);flavorSource=0;}
	  if(source==TLepton::flavor::Charm) {h_mu_pred_charm->Fill(st);h_pt_mu_pred_charm->Fill(vSSLep.at(1)->pt);flavorSource=1;}
	  if(source==TLepton::flavor::Bottom) {h_mu_pred_bottom->Fill(st);h_pt_mu_pred_bottom->Fill(vSSLep.at(1)->pt);flavorSource=2;}
	  if(source==TLepton::flavor::Fake) {h_mu_pred_fake->Fill(st);h_pt_mu_pred_fake->Fill(vSSLep.at(1)->pt);flavorSource=3;}
	  if(source==TLepton::flavor::Unmatched) {h_mu_pred_unm->Fill(st);h_pt_mu_pred_unm->Fill(vSSLep.at(1)->pt);flavorSource=4;}
	}
      }//end predicted
      
    }//end check on whether lep1 is prompt, now do case where it's lep2
    
    else{//p1 is fake and p2 is prompt
    
      //skip if prompt lepton isn't tight
      if(!t2) continue;
      
      //get flavor of lepton source
      TLepton::flavor source = vSSLep.at(0)->SourceFlavor;
      lepPt=vSSLep.at(0)->pt;
      lepEta=vSSLep.at(0)->eta;      
      //separate into 'observed' where both are tight, and 'predicted' where fake lepton is loose only
      if(t1){ //lep1 is tight so this is 'observed'
	observed=1;
	//now fill observed histograms by flavor	  
	if(vSSLep.at(0)->isEl){	
	  lepFlavor=0;
	  if(source==TLepton::flavor::Light) {h_el_obs_light->Fill(st);h_pt_el_obs_light->Fill(vSSLep.at(0)->pt);flavorSource=0;}
	  if(source==TLepton::flavor::Charm) {h_el_obs_charm->Fill(st);h_pt_el_obs_charm->Fill(vSSLep.at(0)->pt);flavorSource=1;}
	  if(source==TLepton::flavor::Bottom) {h_el_obs_bottom->Fill(st);h_pt_el_obs_bottom->Fill(vSSLep.at(0)->pt);flavorSource=2;}
	  if(source==TLepton::flavor::Fake) {h_el_obs_fake->Fill(st);h_pt_el_obs_fake->Fill(vSSLep.at(0)->pt);flavorSource=3;}
	  if(source==TLepton::flavor::Unmatched) {h_el_obs_unm->Fill(st);h_pt_el_obs_unm->Fill(vSSLep.at(0)->pt);flavorSource=4;}
	}
	else{
	  lepFlavor=1;
	  if(source==TLepton::flavor::Light) {h_mu_obs_light->Fill(st); h_pt_mu_obs_light->Fill(vSSLep.at(0)->pt);flavorSource=0;}
	  if(source==TLepton::flavor::Charm) {h_mu_obs_charm->Fill(st);h_pt_mu_obs_charm->Fill(vSSLep.at(0)->pt);flavorSource=1;}
	  if(source==TLepton::flavor::Bottom) {h_mu_obs_bottom->Fill(st);h_pt_mu_obs_bottom->Fill(vSSLep.at(0)->pt);flavorSource=2;}
	  if(source==TLepton::flavor::Fake) {h_mu_obs_fake->Fill(st);h_pt_mu_obs_fake->Fill(vSSLep.at(0)->pt);flavorSource=3;}
	  if(source==TLepton::flavor::Unmatched) {h_mu_obs_unm->Fill(st);h_pt_mu_obs_unm->Fill(vSSLep.at(0)->pt);flavorSource=4;}
	}
      }//end observed
      //so do predicted
      else{
	observed=0;
	//now fill observed histograms by flavor	
	if(vSSLep.at(0)->isEl){	
	  lepFlavor=0;
	  if(source==TLepton::flavor::Light) {h_el_pred_light->Fill(st);h_pt_el_pred_light->Fill(vSSLep.at(0)->pt);flavorSource=0;}
	  if(source==TLepton::flavor::Charm) {h_el_pred_charm->Fill(st);h_pt_el_pred_charm->Fill(vSSLep.at(0)->pt);flavorSource=1;}
	  if(source==TLepton::flavor::Bottom) {h_el_pred_bottom->Fill(st);h_pt_el_pred_bottom->Fill(vSSLep.at(0)->pt);flavorSource=2;}
	  if(source==TLepton::flavor::Fake) {h_el_pred_fake->Fill(st);h_pt_el_pred_fake->Fill(vSSLep.at(0)->pt);flavorSource=3;}
	  if(source==TLepton::flavor::Unmatched) {h_el_pred_unm->Fill(st);h_pt_el_pred_unm->Fill(vSSLep.at(0)->pt);flavorSource=4;}
	}
	else{
	  lepFlavor=1;
	  if(source==TLepton::flavor::Light) {h_mu_pred_light->Fill(st); h_pt_mu_pred_light->Fill(vSSLep.at(0)->pt);flavorSource=0;}
	  if(source==TLepton::flavor::Charm) {h_mu_pred_charm->Fill(st);h_pt_mu_pred_charm->Fill(vSSLep.at(0)->pt);flavorSource=1;}
	  if(source==TLepton::flavor::Bottom) {h_mu_pred_bottom->Fill(st);h_pt_mu_pred_bottom->Fill(vSSLep.at(0)->pt);flavorSource=2;}
	  if(source==TLepton::flavor::Fake) {h_mu_pred_fake->Fill(st);h_pt_mu_pred_fake->Fill(vSSLep.at(0)->pt);flavorSource=3;}
	  if(source==TLepton::flavor::Unmatched) {h_mu_pred_unm->Fill(st);h_pt_mu_pred_unm->Fill(vSSLep.at(0)->pt);flavorSource=4;}
	}

      }//end predicted
    }//end check on whether lep2 is prompt
    outTree->Fill();
  }//end event loop
  //write tFile
  fout->WriteTObject(h_el_obs_light);
  fout->WriteTObject(h_el_obs_charm);
  fout->WriteTObject(h_el_obs_bottom);
  fout->WriteTObject(h_el_obs_fake);
  fout->WriteTObject(h_el_obs_unm);
  fout->WriteTObject(h_el_pred_light);
  fout->WriteTObject(h_el_pred_charm);
  fout->WriteTObject(h_el_pred_bottom);
  fout->WriteTObject(h_el_pred_fake);
  fout->WriteTObject(h_el_pred_unm);

  fout->WriteTObject(h_mu_obs_light);
  fout->WriteTObject(h_mu_obs_charm);
  fout->WriteTObject(h_mu_obs_bottom);
  fout->WriteTObject(h_mu_obs_fake);
  fout->WriteTObject(h_mu_obs_unm);
  fout->WriteTObject(h_mu_pred_light);
  fout->WriteTObject(h_mu_pred_charm);
  fout->WriteTObject(h_mu_pred_bottom);
  fout->WriteTObject(h_mu_pred_fake);
  fout->WriteTObject(h_mu_pred_unm);

  fout->WriteTObject(h_pt_el_obs_light);
  fout->WriteTObject(h_pt_el_obs_charm);
  fout->WriteTObject(h_pt_el_obs_bottom);
  fout->WriteTObject(h_pt_el_obs_fake);
  fout->WriteTObject(h_pt_el_obs_unm);
  fout->WriteTObject(h_pt_el_pred_light);
  fout->WriteTObject(h_pt_el_pred_charm);
  fout->WriteTObject(h_pt_el_pred_bottom);
  fout->WriteTObject(h_pt_el_pred_fake);
  fout->WriteTObject(h_pt_el_pred_unm);

  fout->WriteTObject(h_pt_mu_obs_light);
  fout->WriteTObject(h_pt_mu_obs_charm);
  fout->WriteTObject(h_pt_mu_obs_bottom);
  fout->WriteTObject(h_pt_mu_obs_fake);
  fout->WriteTObject(h_pt_mu_obs_unm);
  fout->WriteTObject(h_pt_mu_pred_light);
  fout->WriteTObject(h_pt_mu_pred_charm);
  fout->WriteTObject(h_pt_mu_pred_bottom);
  fout->WriteTObject(h_pt_mu_pred_fake);
  fout->WriteTObject(h_pt_mu_pred_unm);

  fout->WriteTObject(outTree);

  fout->Close();
}


std::vector<TLepton*> makeLeptons(std::vector<TMuon*> muons, std::vector<TElectron*> electrons, float ptCut, std::string elID, std::string muID, bool doFakes,std::vector<TGenParticle*> genParticles){

  std::vector<TLepton*> Leptons;

  //fill with  muons
  for(unsigned int uimu=0; uimu<muons.size(); uimu++){
    TMuon* imu = muons.at(uimu);
    TLepton* iLep = new TLepton(imu->pt,imu->eta,imu->phi,imu->energy,imu->charge,imu->relIso,imu->miniIso,imu->susyIso);

    if(muID=="CBTight"){
      iLep->Tight=imu->cutBasedTight();
      iLep->Loose=imu->cutBasedLoose();
    }
    else if(muID=="CBTightMiniIso"){
      iLep->Tight=imu->cutBasedTightMiniIso();
      iLep->Loose=imu->cutBasedLooseMiniIso();
    }
    else if(muID=="CBTightMiniIsoTight"){
      iLep->Tight=imu->cutBasedTightMiniIsoTight();
      iLep->Loose=imu->cutBasedLooseMiniIso();
    }
    else if(muID=="CBLoose"){
      iLep->Tight=imu->cutBasedLoose();
      iLep->Loose=true; //in case 'loose ID' is specified as 'tight', take any muon as loose ID
    }

    iLep->isMu = true;
    iLep->isEl = false;
    //skip for smaller than pT cut
    if(iLep->pt<ptCut) continue;
    //get matched particle
    TGenParticle* gp = getMatchedGenParticle(imu,genParticles);
    if( fabs(gp->id)==13 && (gp->isPrompt || gp->isFromPromptTau) ) iLep->isPrompt = true;
    else iLep->isPrompt = false;

    //now do by flavor
    float dR = pow( pow(imu->eta - gp->eta,2) + pow(imu->phi - gp->phi,2), 0.5);    
    //now save based on ID requirments if 'normal' running then require tight, else save if loose
    if(!doFakes){
      if(iLep->Tight){
	if(dR > 0.3) iLep->SourceFlavor = TLepton::flavor::Unmatched;
	else{
	  if(iLep->isPrompt) iLep->SourceFlavor = TLepton::flavor::Prompt;
	  else if( fabs(gp->id)!=13) iLep->SourceFlavor = TLepton::flavor::Fake;
	  else if( gp->PromptMotherHasB) iLep->SourceFlavor = TLepton::flavor::Bottom;
	  else if( gp->PromptMotherHasC) iLep->SourceFlavor = TLepton::flavor::Charm;
	  else iLep->SourceFlavor = TLepton::flavor::Light;
	}
	Leptons.push_back(iLep);
      } 
    }
    else{ //if doing fake estimate save if loose
      if(iLep->Loose){
	//get matched particle
	if(dR > 0.3) iLep->SourceFlavor = TLepton::flavor::Unmatched;
	else{
	  if(iLep->isPrompt) iLep->SourceFlavor = TLepton::flavor::Prompt;
	  else if( fabs(gp->id)!=13) iLep->SourceFlavor = TLepton::flavor::Fake;
	  else if( gp->PromptMotherHasB) iLep->SourceFlavor = TLepton::flavor::Bottom;
	  else if( gp->PromptMotherHasC) iLep->SourceFlavor = TLepton::flavor::Charm;
	  else iLep->SourceFlavor = TLepton::flavor::Light;
	}
	Leptons.push_back(iLep);
      }

    }
  }
  
  
  //fill with  electrons
  for(unsigned int uiel=0; uiel<electrons.size(); uiel++){
    TElectron* iel = electrons.at(uiel);
    TLepton* iLep = new TLepton(iel->pt,iel->eta,iel->phi,iel->energy,iel->charge,iel->relIsoEA,iel->miniIso,iel->susyIso);
    iLep->isMu = false;
    iLep->isEl = true;
    if(elID=="CBTight"){
      iLep->Tight=iel->cutBasedTight25nsSpring15MC();
      iLep->Loose=iel->cutBasedLoose25nsSpring15MC();
    }
    else if(elID=="CBLoose"){
      iLep->Tight=iel->cutBasedLoose25nsSpring15MC();
      iLep->Loose=true;
    }
    if(elID=="CBTightRC"){
      iLep->Tight=iel->cutBasedTight25nsSpring15MCRC();
      iLep->Loose=iel->cutBasedLoose25nsSpring15MCRC();
    }
    else if(elID=="CBLooseRC"){
      iLep->Tight=iel->cutBasedLoose25nsSpring15MCRC();
      iLep->Loose=true;
    }
    else if(elID=="MVATight"){
      iLep->Tight=iel->mvaTightIso();
      iLep->Loose=iel->mvaLooseIso();
    }
    else if(elID=="MVATightNew"){
      iLep->Tight=iel->mvaTightNew();
      iLep->Loose=iel->mvaLooseNew();
    }
    else if(elID=="MVATightNewRC"){
      iLep->Tight=iel->mvaTightNewRC();
      iLep->Loose=iel->mvaLooseNewRC();
    }
    else if(elID=="MVATightNoIso"){
      iLep->Tight=iel->mvaTight();
      iLep->Loose=iel->mvaLoose();
    }
    else if(elID=="MVALoose"){
      iLep->Tight=iel->mvaLooseIso();
      iLep->Loose=true;
    }
    else if(elID=="MVALooseNoIso"){
      iLep->Tight=iel->mvaLoose();
      iLep->Loose=true;
    }
    else if(elID=="MVATightCC"){
      iLep->Tight=iel->mvaTightCCIso();
      iLep->Loose=iel->mvaLooseCCIso();
    }
    else if(elID=="MVATightCCNoIso"){
      iLep->Tight=iel->mvaTightCC();
      iLep->Loose=iel->mvaLooseCC();
    }
    else if(elID=="MVALooseCC"){
      iLep->Tight=iel->mvaLooseCCIso();
      iLep->Loose=true;
    }
    else if(elID=="MVALooseNoIso"){
      iLep->Tight=iel->mvaLoose();
      iLep->Loose=true;
    }  
    else if(elID=="MVATightRC"){
      iLep->Tight=iel->mvaTightRCIso();
      iLep->Loose=iel->mvaLooseRCIso();
    }
    else if(elID=="MVA2016TightRC"){
      iLep->Tight=iel->mva2016TightRCIso();
      iLep->Loose=iel->mvaJulieLooseRCIso();
    }
    else if(elID=="MVALooseRC"){
      iLep->Tight=iel->mvaLooseRCIso();
      iLep->Loose=true;
    }
    else if(elID=="SUSYTight"){
      iLep->Tight=iel->susyTight();
      iLep->Loose=iel->susyLoose();
    }
    else if(elID=="SUSYLoose"){
	iLep->Tight=iel->susyLoose();
	iLep->Loose=true;
    }
    else if(elID=="SUSYTightRC"){
      iLep->Tight=iel->susyTightRC();
      iLep->Loose=iel->susyLooseRC();
    }
    else if(elID=="MVA2017TightV2IsoTightRC"){
      iLep->Tight=iel->mva94XTightV2_90_IsoTight_RC();
      iLep->Loose=iel->mva94XLooseV2_Iso_RC();
    } 
    if(iLep->pt<ptCut) continue;
    //get matched particle
    TGenParticle* gp = getMatchedGenParticle(iel,genParticles);
    if( fabs(gp->id)==11 && (gp->isPrompt || gp->isFromPromptTau) ) iLep->isPrompt = true;
    else iLep->isPrompt = false;
    //now do by flavor
    float dR = pow( pow(iel->eta - gp->eta,2) + pow(iel->phi - gp->phi,2), 0.5);

    //now save based on elID requirments if 'normal' running then require tight, else save if loose
    if(!doFakes){
      if(dR > 0.3) iLep->SourceFlavor = TLepton::flavor::Unmatched;
      else{
	if(iLep->isPrompt) iLep->SourceFlavor = TLepton::flavor::Prompt;
	else if( fabs(gp->id)!=11) iLep->SourceFlavor = TLepton::flavor::Fake;
	else if( gp->PromptMotherHasB) iLep->SourceFlavor = TLepton::flavor::Bottom;
	else if( gp->PromptMotherHasC) iLep->SourceFlavor = TLepton::flavor::Charm;
	else iLep->SourceFlavor = TLepton::flavor::Light;
      }
      if(iLep->Tight) Leptons.push_back(iLep);
    } 
    else{ //if doing fake estimate save if loose
      if(iLep->Loose){
	if(dR > 0.3) iLep->SourceFlavor = TLepton::flavor::Unmatched;
	else{
	  if(iLep->isPrompt) iLep->SourceFlavor = TLepton::flavor::Prompt;
	  else if( fabs(gp->id)!=11) iLep->SourceFlavor = TLepton::flavor::Fake;
	  else if( gp->PromptMotherHasB) iLep->SourceFlavor = TLepton::flavor::Bottom;
	  else if( gp->PromptMotherHasC) iLep->SourceFlavor = TLepton::flavor::Charm;
	  else iLep->SourceFlavor = TLepton::flavor::Light;
	}
	Leptons.push_back(iLep);
      }
    }

 
  }
  
  return Leptons;

}

bool checkSameSignLeptons(std::vector<TLepton*> leptons){

  bool samesign=false;

  for(unsigned int uilep=0; uilep<leptons.size(); uilep++){
    for(unsigned int ujlep=uilep+1; ujlep<leptons.size(); ujlep++){
      if(leptons.at(uilep)->charge == leptons.at(ujlep)->charge){
	samesign=true;
      }
    }
  }

  return samesign;
  
}

std::vector<TLepton*> makeSSLeptons(std::vector<TLepton*> leptons){

  std::vector<TLepton*> vSSLep;

  for(unsigned int uilep=0; uilep<leptons.size(); uilep++){
    for(unsigned int ujlep=uilep+1; ujlep<leptons.size(); ujlep++){
      if(leptons.at(uilep)->charge == leptons.at(ujlep)->charge){
	
	vSSLep.push_back(leptons.at(uilep));
	vSSLep.push_back(leptons.at(ujlep));

      }
    }
  }

  //special for this closure test, I'm going to force it so that only one of the leptons is prompt, and require the prompt lepton to be tight
  


  return vSSLep;
  
}

bool checkSecondaryZVeto(std::vector<TLepton*> leps, std::vector<TMuon*> muons, std::vector<TElectron*> electrons){

  bool veto=false;
  float zmass=91.1;

  for(std::vector<TLepton*>::size_type ilep=0; ilep < leps.size(); ilep++){
    // get lepton
    TLepton* lep = leps.at(ilep);

    //if muon check to find mass w/ other muons
    if(lep->isMu){
      for(std::vector<TMuon*>::size_type imu=0; imu< muons.size(); imu++){
	//skip if loose lepton has pt <= 15 GeV
	if(muons.at(imu)->pt<=15) continue;

	//skip if looking at any of the SS leptons:
	bool skip=false;
	for(std::vector<TLepton*>::size_type jlep =0; jlep<leps.size(); jlep++){
	  if(leps.at(jlep)->pt==muons.at(imu)->pt && leps.at(jlep)->phi==muons.at(imu)->phi){skip=true; break;}
	}
	if(skip) continue; 

	float diff =  getPairMass(lep,muons.at(imu)) - zmass;
	if(fabs(diff) < 15){ veto=true; break;}
      }
    }
    //else check mass w/ other electrons
    else{
      for(std::vector<TElectron*>::size_type iel=0; iel< electrons.size(); iel++){
	//skip if loose electron pt <= 15 GeV
	if(electrons.at(iel)->pt <= 15) continue;

	//skip if looking at any of the SS leptons:
	bool skip=false;
	for(std::vector<TLepton*>::size_type jlep =0; jlep<leps.size(); jlep++){
	  if(leps.at(jlep)->pt==electrons.at(iel)->pt && leps.at(jlep)->phi==electrons.at(iel)->phi){skip=true; break;}
	}
	if(skip) continue; 

	float diff =  getPairMass(lep,electrons.at(iel)) - zmass;
	if(fabs(diff) < 15){ veto=true; break;}
      }
    }
  }

  return veto;

}
std::vector<TLepton*> pruneSSLep(std::vector<TLepton*> allLeps, std::vector<TLepton*> ssLeps){

  std::vector<TLepton*> nonSSLeps=allLeps;

  unsigned int remIndex=0;
  for(std::vector<TLepton*>::size_type ilep=0; ilep<nonSSLeps.size();ilep++){
    if( (nonSSLeps.at(ilep)->pt==ssLeps.at(0)->pt && nonSSLeps.at(ilep)->eta==ssLeps.at(0)->eta && nonSSLeps.at(ilep)->phi==ssLeps.at(0)->phi) || (nonSSLeps.at(ilep)->pt==ssLeps.at(1)->pt && nonSSLeps.at(ilep)->eta==ssLeps.at(1)->eta && nonSSLeps.at(ilep)->phi==ssLeps.at(1)->phi) || (!nonSSLeps.at(ilep)->Tight)){
      nonSSLeps.erase(nonSSLeps.begin()+remIndex);
    }
    remIndex++;
  }

  return nonSSLeps;
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

void printParticle(TLepton* l){
	std::cout<<"charge "<<l->charge<<" pt: "<<l->pt<<" eta: "<<l->eta<<" phi: "<<l->phi<<std::endl;
}
