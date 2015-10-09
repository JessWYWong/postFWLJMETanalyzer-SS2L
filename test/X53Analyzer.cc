#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include <vector>
#include "TLorentzVector.h"
#include "TChain.h"
#include "../interface/TreeReader.h"
#include <stdio.h>
#include "JetAnalyzer.cc"
#include "GenAnalyzer.cc"
#include "../interface/TreeMaker.h"
#include <assert.h>
#include <map>
#include <string>
#include <sstream> 
#include "../plugins/Macros.cc"

std::vector<TLepton*> makeLeptons(std::vector<TMuon*>, std::vector<TElectron*>, float);
std::vector<TLepton*> makeSSLeptons(std::vector<TLepton*>);
std::vector<TLepton*> makeOSLeptonsForDY(std::vector<TLepton*>);
bool checkSameSignLeptons(std::vector<TLepton*>);
bool checkOppositeSignLeptonsForDY(std::vector<TLepton*>);
int getNSSDLGen(std::vector<TGenParticle*>, int);
void printParticle(TGenParticle*);
void printParticle(TLepton*);
bool checkSecondaryZVeto(std::vector<TLepton*> leps, std::vector<TMuon*> muons, std::vector<TElectron*> electrons);
std::vector<TLepton*> pruneSSLep(std::vector<TLepton*> allLeps, std::vector<TLepton*> ssLeps);
int main(int argc, char* argv[]){

  if(argc!=2) return 0;
  std::string argv1=argv[1];

  typedef std::map<std::string,std::string> StringMap;
  
  StringMap bg_samples, sig_samples,data_samples;
  bg_samples["TTbar"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_TTJets.root";
  bg_samples["TTW"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_TTW.root";
  bg_samples["TTZ"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_TTZ.root";
  bg_samples["TTWW"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_TTWW.root";
  bg_samples["WW"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_WW.root";
  bg_samples["WWW"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_WWW.root";
  bg_samples["WZ"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_WZ.root";
  bg_samples["ZZ"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_ZZ.root";
  bg_samples["WJets"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_WJets.root";
  bg_samples["DYJets"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_DYJets.root";

  
  sig_samples["X53X53m700RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m700RH.root";
  sig_samples["X53X53m800RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m800RH.root";
  sig_samples["X53X53m900RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m900RH.root";
  //sig_samples["X53X53m1000RH"]="ljmet_X53X53m1000RH.root";
  sig_samples["X53X53m1000RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m1000RH.root";
  sig_samples["X53X53m1100RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m1100RH.root";
  sig_samples["X53X53m1200RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m1200RH.root";
  sig_samples["X53X53m1300RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m1300RH.root";
  sig_samples["X53X53m1400RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m1400RH.root";
  sig_samples["X53X53m1500RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m1500RH.root";
  sig_samples["X53X53m1600RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m1600RH.root";
  sig_samples["X53X53m700LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m700LH.root";
  sig_samples["X53X53m800LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m800LH.root";
  sig_samples["X53X53m900LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m900LH.root";
  sig_samples["X53X53m1000LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m1000LH.root";
  sig_samples["X53X53m1100LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m1100LH.root";
  sig_samples["X53X53m1200LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m1200LH.root";
  sig_samples["X53X53m1300LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m1300LH.root";
  sig_samples["X53X53m1400LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m1400LH.root";
  sig_samples["X53X53m1500LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m1500LH.root";
  sig_samples["X53X53m1600LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53X53m1600LH.root";
  
  sig_samples["X53Tm700RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-700_right.root";
  sig_samples["X53Tm800RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-800_right.root";
  sig_samples["X53Tm900RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-900_right.root";
  sig_samples["X53Tm1000RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-1000_right.root";
  sig_samples["X53Tm1100RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-1100_right.root";
  sig_samples["X53Tm1200RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-1200_right.root";
  sig_samples["X53Tm1300RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-1300_right.root";
  sig_samples["X53Tm1400RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-1400_right.root";
  sig_samples["X53Tm1500RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-1500_right.root";
  sig_samples["X53Tm1600RH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-1600_right.root";
  sig_samples["X53Tm700LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-700_left.root";
  sig_samples["X53Tm800LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-800_left.root";
  sig_samples["X53Tm900LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-900_left.root";
  sig_samples["X53Tm1000LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-1000_left.root";
  sig_samples["X53Tm1100LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-1100_left.root";
  sig_samples["X53Tm1200LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-1200_left.root";
  sig_samples["X53Tm1300LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-1300_left.root";
  sig_samples["X53Tm1400LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-1400_left.root";
  sig_samples["X53Tm1500LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-1500_left.root";
  sig_samples["X53Tm1600LH"]="/eos/uscms/store/user/lpctlbsm/clint/Spring15/25ns/ljmet_trees/ljmet_X53TToAll_M-1600_left.root";

  //data samples
  data_samples["data"]="/eos/uscms/store/user/lpctlbsm/clint/Run2015B/ljmet_trees/ljmet_Data_All.root";
  
  bool signal=false;
  bool bg_mc=false;
  bool bg_mc_dd = false; //bool to run over mc using tight loose method
  //bool bg_dd=false;
  bool data=false;

  //check usage
  bool correctusage=true;
  if(argc!=2 || ( bg_samples.find(argv[1])==bg_samples.end() && sig_samples.find(argv[1])==sig_samples.end() && data_samples.find(argv[1])==data_samples.end() && argv1!="NonPromptMC") ) correctusage=false;
  if(!correctusage){
    std::cout<<"Need to supply argument for sample name of one of the following"<<std::endl;
    std::cout<<std::endl<<"********** Background *********"<<std::endl;
    for(std::map<std::string,std::string>::iterator iter=bg_samples.begin(); iter!= bg_samples.end(); iter++){
      std::cout<<iter->first<<std::endl;
    }
    std::cout<<std::endl<<"********** Signal *********"<<std::endl;
    for(std::map<std::string,std::string>::iterator iter=sig_samples.begin(); iter!= sig_samples.end(); iter++){
      std::cout<<iter->first<<std::endl;
    }  
    std::cout<<"********* OR Specify \'NonPromptMC\' to run over MC using Data-Driven Method"<<std::endl;
    return 0;
  }

  if(bg_samples.find(argv[1])!=bg_samples.end()) bg_mc=true;
  if(sig_samples.find(argv[1])!=sig_samples.end()) signal=true;
  if(data_samples.find(argv[1])!=data_samples.end()) data=true;
  
  //make TreeReader
  std::string filename;
  if(bg_mc) filename = bg_samples.find(argv[1])->second;
  if(signal) filename = sig_samples.find(argv[1])->second;
  if(data) filename = data_samples.find(argv[1])->second;
  if(bg_mc_dd) filename = "NonPrompt";
  std::cout<<"running file: "<<filename<<std::endl;
  //make output file
  std::stringstream outnamestream;
  outnamestream<<argv[1]<<".root";
  std::string outname = outnamestream.str();
  TFile* fsig = new TFile(outname.c_str(),"RECREATE");

  TreeMaker* tm_ssdl = new TreeMaker();
  tm_ssdl->InitTree("tEvts_ssdl");
  TreeMaker* tm_sZVeto = new TreeMaker();
  tm_sZVeto->InitTree("tEvts_sZVeto");

  TreeReader* tr;
  TChain* chain; //chain used for nonPromptMC 
  if(!bg_mc_dd){
    tr = new TreeReader(filename.c_str(),!data);
  }
  else{
    chain->Add("/eos/uscms/store/user/lpctlbsm/Spring15/25ns/Oct06/ljmet_trees/ljmet_TTJets.root");
    chain->Add("/eos/uscms/store/user/lpctlbsm/Spring15/25ns/Oct06/ljmet_trees/ljmet_WJets.root");
    tr = new TreeReader( (TTree*) chain, !data);
  }

  TTree* t=tr->tree;

  //histogram for cutflow
  TH1F* h_MuCutFlow = new TH1F("h_MuCutFlow","Cut Flow For Muons",13,0,13);

  //histogram for trigger studies
  TH1F* h_DoubleEle33Num = new TH1F("h_DoubleEle33Num","",60,0,600);
  TH1F* h_DoubleEle33_MWNum = new TH1F("h_DoubleEle33_MWNum","",60,0,600);
  TH1F* h_Ele27WP85Num = new TH1F("h_Ele27WP85Num","",60,0,600);
  TH1F* h_Mu30TkMu11Num = new TH1F("h_Mu30TkMu11Num","",60,0,600);
  TH1F* h_Mu40Num = new TH1F("h_Mu40Num","",60,0,600);
  TH1F* h_IsoTkMu24Num = new TH1F("h_IsoTkMu24Num","",60,0,600);
  TH1F* h_DoubleMu33NoFiltersNoVtxNum = new TH1F("h_DoubleMu33NoFiltersNoVtxNum","",60,0,600);
  TH1F* h_Mu23Ele12Num = new TH1F("h_Mu23Ele12Num","",60,0,600);
  TH1F* h_Mu8Ele23Num = new TH1F("h_Mu8Ele23Num","",60,0,600);
  TH1F* h_PFHT900Num = new TH1F("h_PFHT900Num","",60,0,600);
  TH1F* h_AK8PFJet360TrimMass30Num = new TH1F("h_AK8PFJet360TrimMass30Num","",60,0,600);

  TH1F* h_DoubleEle33Den = new TH1F("h_DoubleEle33Den","",60,0,600);
  TH1F* h_DoubleEle33_MWDen = new TH1F("h_DoubleEle33_MWDen","",60,0,600);
  TH1F* h_Ele27WP85Den = new TH1F("h_Ele27WP85Den","",60,0,600);
  TH1F* h_Mu30TkMu11Den = new TH1F("h_Mu30TkMu11Den","",60,0,600);
  TH1F* h_Mu40Den = new TH1F("h_Mu40Den","",60,0,600);
  TH1F* h_IsoTkMu24Den = new TH1F("h_IsoTkMu24Den","",60,0,600);
  TH1F* h_DoubleMu33NoFiltersNoVtxDen = new TH1F("h_DoubleMu33NoFiltersNoVtxDen","",60,0,600);
  TH1F* h_Mu23Ele12Den = new TH1F("h_Mu23Ele12Den","",60,0,600);
  TH1F* h_Mu8Ele23Den = new TH1F("h_Mu8Ele23Den","",60,0,600);
  TH1F* h_PFHT900Den = new TH1F("h_PFHT900Den","",60,0,600);
  TH1F* h_AK8PFJet360TrimMass30Den = new TH1F("h_AK8PFJet360TrimMass30Den","",60,0,600);

  //load eta weights in:
  TFile* eWfile = new TFile("ChargeMisID_Data_Electrons.root");
  std::vector<float> etaWeights = getEtaWeights(eWfile);


  //doGenPlots(fsig,t,tr);
  //cd back to main directory after making gen plots
  fsig->cd();

  int nEntries = t->GetEntries();

  int nMuMu=0;
  int nElMu=0;
  int nElEl=0;
  int nGenMuMu=0;
  int nGenElMu=0;
  int nGenElEl=0;

  //floats for trigger eff
  float nMu40=0;
  float nIsoTkMu24=0;
  float nMu30TkMu11=0;
  float nMu27TkMu8=0;
  float nEle27=0;
  float nDoubleEle33=0;
  float nEle17Ele12=0;
  float nMu30Ele30=0;
  float nMu17Ele12=0;
  float nMu8Ele17=0;
  float nMu30Ele30ORMu17Ele12ORMu8Ele17=0;




  for(int ient=0; ient<nEntries; ient++){

    if(ient % 1000 ==0) std::cout<<"Completed "<<ient<<" out of "<<nEntries<<" events"<<std::endl;
    
    tr->GetEntry(ient);
    

    //bool GenSamesign=false;
    //std::vector<TGenParticle*> vSSGenLep;
    //get number of generated same-sign dilepton events gen particle collection (second parameter is number of muons to search for)
    nGenMuMu += getNSSDLGen(tr->genParticles, 2);
    nGenElMu += getNSSDLGen(tr->genParticles, 1);
    nGenElEl += getNSSDLGen(tr->genParticles, 0);

  


    //Muon CutFlow                                                                                                                                                                                               
    for(unsigned int uimu=0; uimu<tr->allMuons.size();uimu++){
      TMuon* imu = tr->allMuons.at(uimu);

      //run through gen particle collection:                                                                                                                                                                      
      float dR=999;
      for(unsigned int igen=0; igen<tr->genParticles.size(); igen++){
        //only run over muons from hard scattering                                                                                                     
        if(!( ( fabs(tr->genParticles.at(igen)->id)==13) && (tr->genParticles.at(igen)->status==23 || tr->genParticles.at(igen)->status==1)) ) continue;
        float drtemp = pow( pow( imu->eta - tr->genParticles.at(igen)->eta, 2 ) + pow( imu->phi - tr->genParticles.at(igen)->phi, 2), 0.5);
        if(drtemp < dR){
          dR = drtemp;
        }
      }

      //now check to make sure the muon is matched by requiring deltaR be les than 0.15                                                                                                                           
      if(dR >= 0.15) continue;

      if(imu->PFMuon) h_MuCutFlow->Fill(0.5,1);
      else continue;
      if(imu->Global || imu->Tracker) h_MuCutFlow->Fill(1.5,1);
      else continue;
      if(imu->Global) h_MuCutFlow->Fill(2.5,1);
      else continue;
      if(imu->pt>30) h_MuCutFlow->Fill(3.5,1);
      else continue;
      if(imu->eta < 2.4) h_MuCutFlow->Fill(4.5,1);
      else continue;
      if(imu->chi2<10) h_MuCutFlow->Fill(5.5,1);
      else continue;
      if(imu->dz < 0.5) h_MuCutFlow->Fill(6.5,1);
      else continue;
      if(imu->dxy < 0.2) h_MuCutFlow->Fill(7.5,1);
      else continue;
      if(imu->nValMuHits>=1) h_MuCutFlow->Fill(8.5,1);
      else continue;
      if(imu->nMatchedStations >=2) h_MuCutFlow->Fill(9.5,1);
      else continue;
      if(imu->nValPixelHits >=1) h_MuCutFlow->Fill(10.5,1);
      else continue;
      if(imu->nTrackerLayers > 5) h_MuCutFlow->Fill(11.5,1);
      else continue;
      if(imu->relIso < 0.2) h_MuCutFlow->Fill(12.5,1);
    }


    //make vector of good Leptons change based on data/mc   
    std::vector<TLepton*> goodLeptons;
    if(data) goodLeptons = makeLeptons(tr->goodMuons, tr->goodElectrons,30.0);
    else goodLeptons = makeLeptons(tr->goodMuons, tr->goodElectrons,30.0);
    bool samesign;

    //get chargeMisID rate for DY and save DY events outside of Z-peak (71-111 GeV) with weights for chargeMisID
    bool zLeps = true;
    float weight;
    if(outname.find("DYJets")!=std::string::npos){
      samesign = checkOppositeSignLeptonsForDY(goodLeptons); //returns true if find opposite sign leptons outside Zpeak (71-111GeV)
    }
    //now that we have good leptons, if it's not DY sample just check for two with same-sign charge and assign weight of 1
    else{
      samesign = checkSameSignLeptons(goodLeptons);
      weight=1;
    }


    if(!zLeps || !samesign) continue;

    //now make vector of same-sign leptons, for DY make vector containing opposite sign leptons closest to Z mass
    std::vector<TLepton*> vSSLep;
    if(outname.find("DYJets")!=std::string::npos){
      
      vSSLep = makeOSLeptonsForDY(goodLeptons);
    }
    else vSSLep = makeSSLeptons(goodLeptons);

    //dummy check to make sure the vector got filled properly
    assert(vSSLep.size() > 1);

    //now prune the goodleptons of the ssleptons
    std::vector<TLepton*> vNonSSLep = pruneSSLep(goodLeptons,vSSLep);
    //with vector now get weight for DY Events
    if(outname.find("DYJets")!=std::string::npos) weight = getEtaWeight(etaWeights,vSSLep);


    //now get dilepton mass
    float dilepMass = (vSSLep.at(0)->lv + vSSLep.at(1)->lv).M();

    //quarkonia veto
    //if(dilepMass<=20) continue;

    float HT=0;
    HT+=vSSLep.at(0)->pt+vSSLep.at(1)->pt;
    for(unsigned int uijet=0; uijet<tr->allAK4Jets.size();uijet++){
      HT+=tr->allAK4Jets.at(uijet)->pt;
    }

    //get channel
    int nMu=0;
    if(vSSLep.at(0)->isMu) nMu++;
    if(vSSLep.at(1)->isMu) nMu++;

    //bools for channels
    bool mumu=false;
    bool elmu=false;
    bool elel=false;


    //ok at this point only have events with same-sign leptons, now let's do simple check to see how many of each channel we have:

    if(vSSLep.at(0)->isMu && vSSLep.at(1)->isMu){ nMuMu+=1; mumu=true;}
    else if( ( vSSLep.at(0)->isEl && vSSLep.at(1)->isMu) || (vSSLep.at(0)->isMu && vSSLep.at(1)->isEl)){ nElMu+=1; elmu=true;}
    else {nElEl+=1; elel=true;}

    //now veto on dielectron events coming from Z bosons
    //if(elel && (dilepMass>71.1 && dilepMass<111.1)) continue;

    //fill muon trigger histograms
    if(mumu){
      //HLTMu40 - check efficiency of leading lepton
      h_Mu40Den->Fill(vSSLep.at(0)->pt);
      if(tr->HLT_Mu40){
	h_Mu40Num->Fill(vSSLep.at(0)->pt);
	nMu40+=1;
      }
      //HLT_IsoTkMu24 - also check for leading lepton
      h_IsoTkMu24Den->Fill(vSSLep.at(0)->pt);
      if(tr->HLT_IsoTkMu24){
	h_IsoTkMu24Num->Fill(vSSLep.at(0)->pt);
	nIsoTkMu24+=1;
      }
      //HLT_Mu30TkMu11 - check for subleading lepton
      h_Mu30TkMu11Den->Fill(vSSLep.at(1)->pt);
      if(tr->HLT_Mu30TkMu11){
	h_Mu30TkMu11Num->Fill(vSSLep.at(1)->pt);
	nMu30TkMu11+=1;
      }
      if(tr->HLT_Mu27TkMu8) nMu27TkMu8+=1;
      //HLT_DoubleMu33NoFiltersNoVtx - check fo subleading lepton
      h_DoubleMu33NoFiltersNoVtxDen->Fill(vSSLep.at(1)->pt);
      if(tr->HLT_DoubleMu33NoFiltersNoVtx) h_DoubleMu33NoFiltersNoVtxNum->Fill(vSSLep.at(1)->pt);
    }
    //now electron trigger
    if(elel){
      //HLt_Ele27 - check vs leading lepton
      h_Ele27WP85Den->Fill(vSSLep.at(0)->pt);
      if(tr->HLT_Ele27WP85){
	h_Ele27WP85Num->Fill(vSSLep.at(0)->pt);
	nEle27+=1;
      }
      //HLT_DoubleEle33 - check vs subleading lepton
      h_DoubleEle33Den->Fill(vSSLep.at(1)->pt);
      if(tr->HLT_DoubleEle33){
	h_DoubleEle33Num->Fill(vSSLep.at(1)->pt);
	nDoubleEle33+=1;
      }
      //HLT_DoubleEle33_MW - check vs subleading lepton
      h_DoubleEle33_MWDen->Fill(vSSLep.at(1)->pt);
      if(tr->HLT_DoubleEle33_MW) h_DoubleEle33_MWNum->Fill(vSSLep.at(1)->pt);
      if(tr->HLT_Ele17Ele12) nEle17Ele12+=1;
    }

    //if cross channel
    if(elmu){
      if(tr->HLT_Mu30Ele30) nMu30Ele30+=1;
      if(tr->HLT_Mu17Ele12) nMu17Ele12+=1;
      if(tr->HLT_Mu17Ele12) nMu8Ele17+=1;
      //also get OR
      if( (tr->HLT_Mu30Ele30) || (tr->HLT_Mu17Ele12) || (tr->HLT_Mu8Ele17) )nMu30Ele30ORMu17Ele12ORMu8Ele17+=1;
      
    }

    //require OR of triggers
    if(! ( tr->HLT_Mu8Ele23 || tr->HLT_Mu23Ele12 || tr->HLT_Mu8Ele17 || tr->HLT_Mu17Ele12 || tr->HLT_Mu30Ele30 || tr->HLT_Mu27TkMu8 || tr->HLT_Mu30TkMu11 || tr->HLT_Mu40TkMu11 || tr->HLT_DoubleEle33 || tr->HLT_Ele17Ele12 ) ) continue;

    //fill tree for post ssdl cut since that is all that we've applied so far
    tm_ssdl->FillTree(vSSLep, tr->allAK4Jets, tr->cleanedAK4Jets, tr->simpleCleanedAK4Jets, HT, tr->MET, dilepMass,nMu,weight,vNonSSLep,tr->MCWeight);
    //since we have the two same-sign leptons, now make sure neither of them reconstructs with any other tight lepton in the event to form a Z
    bool secondaryZVeto = checkSecondaryZVeto(vSSLep,tr->looseMuons,tr->looseElectrons);
    if(secondaryZVeto) continue;
    //fill tree for post secondary z veto
    tm_sZVeto->FillTree(vSSLep, tr->allAK4Jets, tr->cleanedAK4Jets, tr->simpleCleanedAK4Jets, HT, tr->MET, dilepMass,nMu,weight,vNonSSLep,tr->MCWeight);
    

  }//end event loop

  //write the trees
  tm_ssdl->tree->Write();
  tm_sZVeto->tree->Write();

  //normalize cutflow to first bin                                                                                                                                                                                
  h_MuCutFlow->Scale( 1. / h_MuCutFlow->GetBinContent(1) );

  //set bin labels                                                                                                                                                                                                
  h_MuCutFlow->GetXaxis()->SetBinLabel(1,"All Loose Muons");
  h_MuCutFlow->GetXaxis()->SetBinLabel(2,"is GlobalMuon");
  h_MuCutFlow->GetXaxis()->SetBinLabel(3,"is PF Muon");
  h_MuCutFlow->GetXaxis()->SetBinLabel(4,"p_{T} >30");
  h_MuCutFlow->GetXaxis()->SetBinLabel(5,"#eta <2.4");
  h_MuCutFlow->GetXaxis()->SetBinLabel(6,"#chi^{2} <10");
  h_MuCutFlow->GetXaxis()->SetBinLabel(7,"dZ<0.5");
  h_MuCutFlow->GetXaxis()->SetBinLabel(8,"dXY<0.2");
  h_MuCutFlow->GetXaxis()->SetBinLabel(9,"N_{ValidMuHits}>=1");
  h_MuCutFlow->GetXaxis()->SetBinLabel(10,"N_{MatchStations}>=2");
  h_MuCutFlow->GetXaxis()->SetBinLabel(11,"N_{ValidPixHits}>=1");
  h_MuCutFlow->GetXaxis()->SetBinLabel(12,"N_{TrackerLayer}>5");
  h_MuCutFlow->GetXaxis()->SetBinLabel(13,"RelIso <0.2");

  //write the histograms
  fsig->WriteTObject(h_MuCutFlow);

  fsig->WriteTObject(h_DoubleEle33Num); 
  fsig->WriteTObject(h_DoubleEle33_MWNum);
  fsig->WriteTObject(h_Ele27WP85Num); 
  fsig->WriteTObject(h_Mu30TkMu11Num); 
  fsig->WriteTObject(h_Mu40Num); 
  fsig->WriteTObject(h_IsoTkMu24Num); 
  fsig->WriteTObject(h_DoubleMu33NoFiltersNoVtxNum);
  //fsig->WriteTObject(h_Mu23Ele12Num);
  //fsig->WriteTObject(h_Mu8Ele23Num);
  //fsig->WriteTObject(h_PFHT900Num);
  //fsig->WriteTObject(h_AK8PFJet360TrimMass30Num);

  fsig->WriteTObject(h_DoubleEle33Den); 
  fsig->WriteTObject(h_DoubleEle33_MWDen);
  fsig->WriteTObject(h_Ele27WP85Den);
  fsig->WriteTObject(h_Mu30TkMu11Den);
  fsig->WriteTObject(h_Mu40Den);
  fsig->WriteTObject(h_IsoTkMu24Den);
  fsig->WriteTObject(h_DoubleMu33NoFiltersNoVtxDen);
  //fsig->WriteTObject(h_Mu23Ele12Den);
  //fsig->WriteTObject(h_Mu8Ele23Den);
  //fsig->WriteTObject(h_PFHT900Den);
  //fsig->WriteTObject(h_AK8PFJet360TrimMass30Den);
  


  fsig->Write();
  fsig->Close();

  std::cout<<"Number of Gen MuMu: "<<nGenMuMu<<std::endl;
  std::cout<<"Number of MuMu: "<<nMuMu<<std::endl;
  std::cout<<"Number of ElMu: "<<nElMu<<std::endl;
  std::cout<<"Number of ElEl: "<<nElEl<<std::endl;


  std::cout<<"Efficiency of Mu40 for MuMu: "<<nMu40/(nMuMu)<<std::endl;
  std::cout<<"Efficiency of Mu40 for MuMu+ElMu: "<<nMu40/(nMuMu + nElMu)<<std::endl;
  std::cout<<"Efficiency of IsoTkmu24 for MuMu: "<<nIsoTkMu24/(nMuMu)<<std::endl;
  std::cout<<"Efficiency of IsoTkmu24 for MuMu+ElMu: "<<nIsoTkMu24/(nMuMu+nElMu)<<std::endl;

  TH1F* hNMu30TkMu11Num = new TH1F("hNMu30TkMu11Num","",1,0,10000);
  TH1F* hNMu30TkMu11Den = new TH1F("hNMu30TkMu11Den","",1,0,10000);
  for(int j =0; j<nMu30TkMu11;j++) hNMu30TkMu11Num->Fill(j);
  for(int j =0; j<nMuMu;j++) hNMu30TkMu11Den->Fill(j);
  TGraphAsymmErrors* Mu30TkMu11Graph = new TGraphAsymmErrors(hNMu30TkMu11Num,hNMu30TkMu11Den);
  std::cout<<"Efficiency of Mu30TkMu11: "<<nMu30TkMu11/nMuMu<<" +"<<Mu30TkMu11Graph->GetErrorYhigh(0)<<" -"<<Mu30TkMu11Graph->GetErrorYlow(0)<<std::endl<<"\n";
  std::cout<<"Efficiency of Ele27 for ElEl: "<<nEle27/nElEl<<std::endl;
  std::cout<<"Efficiency of Ele27 for ElEl + ElMu: "<<nEle27/(nElMu +nElEl)<<std::endl;

  TH1F* hNMu27TkMu8Num = new TH1F("hNMu27TkMu8Num","",1,0,10000);
  TH1F* hNMu27TkMu8Den = new TH1F("hNMu27TkMu8Den","",1,0,10000);
  for(int j =0; j<nMu27TkMu8;j++) hNMu27TkMu8Num->Fill(j);
  for(int j =0; j<nMuMu;j++) hNMu27TkMu8Den->Fill(j);
  TGraphAsymmErrors* Mu27TkMu8Graph = new TGraphAsymmErrors(hNMu27TkMu8Num,hNMu27TkMu8Den);
  std::cout<<"Efficiency of Mu27TkMu8: "<<nMu27TkMu8/nMuMu<<" +"<<Mu27TkMu8Graph->GetErrorYhigh(0)<<" -"<<Mu27TkMu8Graph->GetErrorYlow(0)<<std::endl<<"\n";

  TH1F* hNDoubleEle33Num = new TH1F("hNDoubleEle33Num","",1,0,10000);
  TH1F* hNDoubleEle33Den = new TH1F("hNDoubleEle33Den","",1,0,10000);
  for(int j =0; j<nDoubleEle33;j++) hNDoubleEle33Num->Fill(j);
  for(int j =0; j<nElEl;j++) hNDoubleEle33Den->Fill(j);
  TGraphAsymmErrors* DoubleEle33Graph = new TGraphAsymmErrors(hNDoubleEle33Num,hNDoubleEle33Den);
  std::cout<<"Efficiency of DoubleEle33: "<<nDoubleEle33/nElEl<<" +"<<DoubleEle33Graph->GetErrorYhigh(0)<<" -"<<DoubleEle33Graph->GetErrorYlow(0)<<std::endl<<"\n";

  TH1F* hNEle17Ele12Num = new TH1F("hNEle17Ele12Num","",1,0,10000);
  TH1F* hNEle17Ele12Den = new TH1F("hNEle17Ele12Den","",1,0,10000);
  for(int j =0; j<nEle17Ele12;j++) hNEle17Ele12Num->Fill(j);
  for(int j =0; j<nElEl;j++) hNEle17Ele12Den->Fill(j);
  TGraphAsymmErrors* Ele17Ele12Graph = new TGraphAsymmErrors(hNEle17Ele12Num,hNEle17Ele12Den);
  std::cout<<"Efficiency of Ele17Ele12: "<<nEle17Ele12/nElMu<<" +"<<Ele17Ele12Graph->GetErrorYhigh(0)<<" -"<<Ele17Ele12Graph->GetErrorYlow(0)<<std::endl<<"\n";
 
  TH1F* hNMu30Ele30Num = new TH1F("hNMu30Ele30Num","",1,0,10000);
  TH1F* hNMu30Ele30Den = new TH1F("hNMu30Ele30Den","",1,0,10000);
  for(int j =0; j<nMu30Ele30;j++) hNMu30Ele30Num->Fill(j);
  for(int j =0; j<nElMu;j++) hNMu30Ele30Den->Fill(j);
  TGraphAsymmErrors* Mu30Ele30Graph = new TGraphAsymmErrors(hNMu30Ele30Num,hNMu30Ele30Den);
  std::cout<<"Efficiency of Mu30Ele30: "<<nMu30Ele30/nElMu<<" +"<<Mu30Ele30Graph->GetErrorYhigh(0)<<" -"<<Mu30Ele30Graph->GetErrorYlow(0)<<std::endl<<"\n";

  TH1F* hNMu8Ele17Num = new TH1F("hNMu8Ele17Num","",1,0,10000);
  TH1F* hNMu8Ele17Den = new TH1F("hNMu8Ele17Den","",1,0,10000);
  for(int j =0; j<nMu8Ele17;j++) hNMu8Ele17Num->Fill(j);
  for(int j =0; j<nElMu;j++) hNMu8Ele17Den->Fill(j);
  TGraphAsymmErrors* Mu8Ele17Graph = new TGraphAsymmErrors(hNMu8Ele17Num,hNMu8Ele17Den);
  std::cout<<"Efficiency of Mu8Ele17: "<<nMu8Ele17/nElMu<<" +"<<Mu8Ele17Graph->GetErrorYhigh(0)<<" -"<<Mu8Ele17Graph->GetErrorYlow(0)<<std::endl<<"\n";

  TH1F* hNMu17Ele12Num = new TH1F("hNMu17Ele12Num","",1,0,10000);
  TH1F* hNMu17Ele12Den = new TH1F("hNMu17Ele12Den","",1,0,10000);
  for(int j =0; j<nMu17Ele12;j++) hNMu17Ele12Num->Fill(j);
  for(int j =0; j<nElMu;j++) hNMu17Ele12Den->Fill(j);
  TGraphAsymmErrors* Mu17Ele12Graph = new TGraphAsymmErrors(hNMu17Ele12Num,hNMu17Ele12Den);
  std::cout<<"Efficiency of Mu17Ele12: "<<nMu17Ele12/nElMu<<" +"<<Mu17Ele12Graph->GetErrorYhigh(0)<<" -"<<Mu17Ele12Graph->GetErrorYlow(0)<<std::endl<<"\n";
  std::cout<<"Efficiency of Mu17Ele12: "<<nMu17Ele12/nElMu<<std::endl;
  std::cout<<"Efficiency of Mu30Ele30 OR Mu17Ele12 OR Mu8Ele17: "<<nMu30Ele30ORMu17Ele12ORMu8Ele17/nElMu<<std::endl;
}


std::vector<TLepton*> makeLeptons(std::vector<TMuon*> muons, std::vector<TElectron*> electrons, float ptCut){

  std::vector<TLepton*> Leptons;

  //fill with  muons
  for(unsigned int uimu=0; uimu<muons.size(); uimu++){
    TMuon* imu = muons.at(uimu);
    TLepton* iLep = new TLepton(imu->pt,imu->eta,imu->phi,imu->energy,imu->charge);
    iLep->isMu = true;
    iLep->isEl = false;
    if(iLep->pt<ptCut) continue;
    Leptons.push_back(iLep);
 
  }
  
  //fill with  electrons
  for(unsigned int uiel=0; uiel<electrons.size(); uiel++){
    TElectron* iel = electrons.at(uiel);
    TLepton* iLep = new TLepton(iel->pt,iel->eta,iel->phi,iel->energy,iel->charge);
    iLep->isMu = false;
    iLep->isEl = true;
    if(iLep->pt<ptCut) continue;
    Leptons.push_back(iLep);
 
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

bool checkOppositeSignLeptonsForDY(std::vector<TLepton*> leptons){

  bool outsidePeak=false;
  bool oppositeSign=false;

  float minDiff=99999;
  float pairMass=91.1;

  TLepton* Lep1;
  TLepton* Lep2;

  for(unsigned int uilep=0; uilep<leptons.size(); uilep++){
    TLepton* lep1 = leptons.at(uilep);
    for(unsigned int ujlep=uilep+1; ujlep<leptons.size(); ujlep++){
      TLepton* lep2 = leptons.at(ujlep);
      float m = (lep1->lv + lep2->lv).M();
      float diff = fabs(91.1 - m);
      if( diff< minDiff){
	minDiff=diff;
	pairMass=m;
	Lep1=lep1;
	Lep2=lep2;
      }
    }
  }

  if(minDiff!=99999){
    if(minDiff>15) outsidePeak=true;
    if(Lep1->charge != Lep2->charge) oppositeSign=true;
  }

  if(! ( outsidePeak && oppositeSign) ) return false;
  else if( Lep1->isMu && Lep2->isMu) return false; //don't allow mumu events to pass
  else return true;

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

  return vSSLep;
  
}

std::vector<TLepton*> makeOSLeptonsForDY(std::vector<TLepton*> leptons){

  std::vector<TLepton*> vSSLep;
  float minDiff=99999;
  float pairMass=91.1;

  TLepton* Lep1;
  TLepton* Lep2;

  for(unsigned int uilep=0; uilep<leptons.size(); uilep++){
    TLepton* lep1 = leptons.at(uilep);
    for(unsigned int ujlep=uilep+1; ujlep<leptons.size(); ujlep++){
      TLepton* lep2 = leptons.at(ujlep);
      float m = (lep1->lv + lep2->lv).M();
      float diff = fabs(91.1 - m);
      if( diff< minDiff){
	minDiff=diff;
	pairMass=m;
	Lep1=lep1;
	Lep2=lep2;
      }
    }
  }

  vSSLep.push_back(Lep1);
  vSSLep.push_back(Lep2);


  return vSSLep;

}

int getNSSDLGen(std::vector<TGenParticle*> genParticles, int nMu){

  bool samesign;
 
  int flav1=-999;
  int flav2=-999;

  for(unsigned int uigen=0; uigen < genParticles.size(); uigen++){
    //only check particles from the hard scattering
    //if( genParticles.at(uigen)->status!=23) continue;
    //only check electrons and muons
    if( !( abs(genParticles.at(uigen)->id)==11 || abs(genParticles.at(uigen)->id)==13) ) continue;
    for(unsigned int ujgen=0;  ujgen < genParticles.size(); ujgen++){
      //only check particles from the hard scattering
    if( genParticles.at(ujgen)->status!=23) continue;
      if(ujgen==uigen) continue;
      //std::cout<<"flavor 1: "<<genParticles.at(uigen)->id<<" w/ charge: "<<genParticles.at(uigen)->charge<<" flavor 2: "<<genParticles.at(ujgen)->id<<" w/ charge: "<<genParticles.at(ujgen)->charge<<std::endl;
      //only check electrons and muons
      if( !( abs(genParticles.at(ujgen)->id)==11 || abs(genParticles.at(ujgen)->id)==13) ) continue;
      if( genParticles.at(uigen)->charge == genParticles.at(ujgen)->charge){
	samesign = true;
	flav1 = abs(genParticles.at(uigen)->id);
	flav2 = abs(genParticles.at(ujgen)->id);
      }
    }

  }

  //std::cout<<"\n *** End Event *** \n"<<std::endl;

  if(!samesign) return 0;

  if(nMu==2){
    if( flav1 ==13 && flav2==13) return 1;
    else return 0;
  }

  if(nMu==1){
    if( (flav1==11 && flav2==13) || (flav1==13 || flav2==11) ) return 1;
    else return 0;
  }


  if(nMu==0){
    if( flav1==11 && flav2==11) return 1;
    else return 0;
  }

  //in case none above 
  return 0;

}


void printParticle(TGenParticle* p){
	std::cout<<"genP id: "<<p->id<<" charge "<<p->charge<<" status: "<<p->status<<" pt: "<<p->pt<<" eta: "<<p->eta<<" phi: "<<p->phi<<std::endl;
}

void printParticle(TLepton* l){
	std::cout<<"charge "<<l->charge<<" pt: "<<l->pt<<" eta: "<<l->eta<<" phi: "<<l->phi<<std::endl;
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
    if( (nonSSLeps.at(ilep)->pt==ssLeps.at(0)->pt && nonSSLeps.at(ilep)->eta==ssLeps.at(0)->eta && nonSSLeps.at(ilep)->phi==ssLeps.at(0)->phi) || (nonSSLeps.at(ilep)->pt==ssLeps.at(1)->pt && nonSSLeps.at(ilep)->eta==ssLeps.at(1)->eta && nonSSLeps.at(ilep)->phi==ssLeps.at(1)->phi) ){
      nonSSLeps.erase(nonSSLeps.begin()+remIndex);
    }
    remIndex++;
  }

  return nonSSLeps;
}
