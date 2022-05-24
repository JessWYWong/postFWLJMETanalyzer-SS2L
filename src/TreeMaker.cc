#include "../interface/TreeMaker.h"
#include <assert.h>

//constructor
TreeMaker::TreeMaker(){}


//destructor
TreeMaker::~TreeMaker(){}

//Initialize tree
void TreeMaker::InitTree(std::string treename){
  tree = new TTree(treename.c_str(),treename.c_str());

  //event info
  tree->Branch("Run",&run_);
  tree->Branch("Lumi",&lumi_);
  tree->Branch("Event",&event_);
  tree->Branch("prefireWeight",&prefireWeight_);
  tree->Branch("prefireWeightUp",&prefireWeightUp_);
  tree->Branch("prefireWeightDown",&prefireWeightDown_);

  //weight
  tree->Branch("ChargeMisIDWeight",&weight_);
  tree->Branch("MCWeight",&MCWeight_);
  tree->Branch("NPWeight",&NPWeight_);
  tree->Branch("NPWeightElUp",&NPWeightElUp_);
  tree->Branch("NPWeightElDown",&NPWeightElDown_);
  tree->Branch("NPWeightMuUp",&NPWeightMuUp_);
  tree->Branch("NPWeightMuDown",&NPWeightMuDown_);
  tree->Branch("NPWeightScan",&NPWeightScan_);
  tree->Branch("NPWeightElPRUp",&NPWeightElPRUp_);
  tree->Branch("NPWeightElPRDown",&NPWeightElPRDown_);
  tree->Branch("NPWeightMuPRUp",&NPWeightMuPRUp_);
  tree->Branch("NPWeightMuPRDown",&NPWeightMuPRDown_);
  tree->Branch("NPAltWeight",&NPAltWeight_);
  tree->Branch("NPSUSYWeight",&NPSUSYWeight_);
  tree->Branch("nTL",&nTL_);
  tree->Branch("Lep1Pt",&Lep1Pt_);

  //scalefactors
  tree->Branch("trigSF",&trigSF_);
  tree->Branch("trigSFup",&trigSFup_);
  tree->Branch("trigSFdn",&trigSFdn_);
  tree->Branch("IDSF",&IDSF_);
  tree->Branch("IDSFup",&IDSFup_);
  tree->Branch("IDSFdn",&IDSFdn_);
  tree->Branch("IsoSF",&IsoSF_);
  tree->Branch("GsfSF",&gsfSF_);
  tree->Branch("JetTaggingSF",&JetTaggingSF_);

  tree->Branch("PUWeight",&puweight_);
  tree->Branch("PUWeightUp",&puweightUp_);
  tree->Branch("PUWeightDown",&puweightDown_);

  tree->Branch("pdfweights",&pdfweights_);
  tree->Branch("renormWeights",&renormWeights_);
  tree->Branch("pdfNewWeights",&pdfNewWeights_);
  tree->Branch("pdfWeights4LHC",&pdfWeights4LHC_);
  tree->Branch("pdfWeightsMSTW",&pdfWeightsMSTW_);
  tree->Branch("pdfNewNominalWeight",&pdfNewNominalWeight_);

  tree->Branch("Lep1Eta",&Lep1Eta_);
  tree->Branch("Lep1Phi",&Lep1Phi_);
  tree->Branch("Lep1Energy",&Lep1Energy_);
  tree->Branch("Lep1Flavor",&Lep1Flavor_);
  tree->Branch("Lep1Charge",&Lep1Charge_);
  tree->Branch("Lep1Loose",&Lep1Loose_);
  tree->Branch("Lep1Tight",&Lep1Tight_);
  tree->Branch("Lep1MinDR",&Lep1MinDR_);
  tree->Branch("Lep1RelIso",&Lep1RelIso_);
  tree->Branch("Lep1MiniIso",&Lep1MiniIso_);
  tree->Branch("Lep1SusyIso",&Lep1SusyIso_);

  tree->Branch("Lep2Pt",&Lep2Pt_);
  tree->Branch("Lep2Eta",&Lep2Eta_);
  tree->Branch("Lep2Phi",&Lep2Phi_);
  tree->Branch("Lep2Energy",&Lep2Energy_);
  tree->Branch("Lep2Flavor",&Lep2Flavor_);
  tree->Branch("Lep2Charge",&Lep2Charge_);
  tree->Branch("Lep2Loose",&Lep2Loose_);
  tree->Branch("Lep2Tight",&Lep2Tight_);
  tree->Branch("Lep2MinDR",&Lep2MinDR_);
  tree->Branch("Lep2RelIso",&Lep2RelIso_);
  tree->Branch("Lep2MiniIso",&Lep2MiniIso_);
  tree->Branch("Lep2SusyIso",&Lep2SusyIso_);

  //number of constiuents, cleaned ak4 jets and non ssdl leptons
  tree->Branch("nConst",&nConst_);
  tree->Branch("nNewConst",&nNewConst_);
  tree->Branch("nNonSSLeps",&nNonSSLeps_);

  //tree->Branch("NTightLeptons",&NTightLeptons_);
  tree->Branch("nAK4Jets",&nAK4Jets_);

  tree->Branch("AK4HT",&AK4HT_);
  tree->Branch("MET",&MET_);

  tree->Branch("AK4Jet1Pt",&AK4Jet1Pt_);
  tree->Branch("AK4Jet1Eta",&AK4Jet1Eta_);
  tree->Branch("AK4Jet1Phi",&AK4Jet1Phi_);
  tree->Branch("AK4Jet1Energy",&AK4Jet1Energy_);

  tree->Branch("AK4Jet2Pt",&AK4Jet2Pt_);
  tree->Branch("AK4Jet2Eta",&AK4Jet2Eta_);
  tree->Branch("AK4Jet2Phi",&AK4Jet2Phi_);
  tree->Branch("AK4Jet2Energy",&AK4Jet2Energy_);  


  //cleaned jets
  tree->Branch("nCleanAK4Jets",&nCleanAK4Jets_);
  tree->Branch("nCleanAK4JetsScaleUp",&nCleanAK4JetsScaleUp_);
  tree->Branch("nCleanAK4JetsScaleDown",&nCleanAK4JetsScaleDown_);
  tree->Branch("nCleanAK4JetsSmearUp",&nCleanAK4JetsSmearUp_);
  tree->Branch("nCleanAK4JetsSmearDown",&nCleanAK4JetsSmearDown_);
  tree->Branch("cleanAK4HT",&cleanAK4HT_);
  tree->Branch("cleanAK4HTScaleUp",&cleanAK4HTScaleUp_);
  tree->Branch("cleanAK4HTScaleDown",&cleanAK4HTScaleDown_);
  tree->Branch("cleanAK4HTSmearUp",&cleanAK4HTSmearUp_);
  tree->Branch("cleanAK4HTSmearDown",&cleanAK4HTSmearDown_);
  tree->Branch("cleanAK4Jet1Pt",&cleanAK4Jet1Pt_);
  tree->Branch("cleanAK4Jet1Eta",&cleanAK4Jet1Eta_);
  tree->Branch("cleanAK4Jet1Phi",&cleanAK4Jet1Phi_);
  tree->Branch("cleanAK4Jet1Energy",&cleanAK4Jet1Energy_);

  tree->Branch("cleanAK4Jet2Pt",&cleanAK4Jet2Pt_);
  tree->Branch("cleanAK4Jet2Eta",&cleanAK4Jet2Eta_);
  tree->Branch("cleanAK4Jet2Phi",&cleanAK4Jet2Phi_);
  tree->Branch("cleanAK4Jet2Energy",&cleanAK4Jet2Energy_);  

  //simple cleaned jets
  tree->Branch("nSimpleCleanAK4Jets",&nSimpleCleanAK4Jets_);
  tree->Branch("simpleCleanAK4HT",&simpleCleanAK4HT_);
  tree->Branch("simpleCleanAK4Jet1Pt",&simpleCleanAK4Jet1Pt_);
  tree->Branch("simpleCleanAK4Jet1Eta",&simpleCleanAK4Jet1Eta_);
  tree->Branch("simpleCleanAK4Jet1Phi",&simpleCleanAK4Jet1Phi_);
  tree->Branch("simpleCleanAK4Jet1Energy",&simpleCleanAK4Jet1Energy_);

  tree->Branch("simpleCleanAK4Jet2Pt",&simpleCleanAK4Jet2Pt_);
  tree->Branch("simpleCleanAK4Jet2Eta",&simpleCleanAK4Jet2Eta_);
  tree->Branch("simpleCleanAK4Jet2Phi",&simpleCleanAK4Jet2Phi_);
  tree->Branch("simpleCleanAK4Jet2Energy",&simpleCleanAK4Jet2Energy_);  

  //ak4jets
  tree->Branch("AK8Jet1Pt",&AK8Jet1Pt_);
  tree->Branch("AK8Jet1Eta",&AK8Jet1Eta_);
  tree->Branch("AK8Jet1Phi",&AK8Jet1Phi_);
  tree->Branch("AK8Jet1Energy",&AK8Jet1Energy_);
  tree->Branch("AK8Jet1TrimMass",&AK8Jet1TrimMass_);
  tree->Branch("AK8Jet1PruneMass",&AK8Jet1PruneMass_);
  tree->Branch("AK8Jet1SDMass",&AK8Jet1SDMass_);
  tree->Branch("AK8Jet1FiltMass",&AK8Jet1FiltMass_);
  tree->Branch("AK8Jet1Tau1",&AK8Jet1Tau1_);
  tree->Branch("AK8Jet1Tau2",&AK8Jet1Tau2_);
  tree->Branch("AK8Jet1Tau3",&AK8Jet1Tau3_);
  tree->Branch("AK8Jet1Tau21",&AK8Jet1Tau21_);
  tree->Branch("AK8Jet1Tau31",&AK8Jet1Tau31_);
  tree->Branch("AK8Jet1Tau32",&AK8Jet1Tau32_);
  tree->Branch("AK8Jet1SubjetMaxBDisc",&AK8Jet1SubjetMaxBDisc_);

  tree->Branch("AK8Jet2Pt",&AK8Jet2Pt_);
  tree->Branch("AK8Jet2Eta",&AK8Jet2Eta_);
  tree->Branch("AK8Jet2Phi",&AK8Jet2Phi_);
  tree->Branch("AK8Jet2Energy",&AK8Jet2Energy_);
  tree->Branch("AK8Jet2TrimMass",&AK8Jet2TrimMass_);
  tree->Branch("AK8Jet2PruneMass",&AK8Jet2PruneMass_);
  tree->Branch("AK8Jet2SDMass",&AK8Jet2SDMass_);
  tree->Branch("AK8Jet2FiltMass",&AK8Jet2FiltMass_);
  tree->Branch("AK8Jet2Tau1",&AK8Jet2Tau1_);
  tree->Branch("AK8Jet2Tau2",&AK8Jet2Tau2_);
  tree->Branch("AK8Jet2Tau3",&AK8Jet2Tau3_);
  tree->Branch("AK8Jet2Tau21",&AK8Jet2Tau21_);
  tree->Branch("AK8Jet2Tau31",&AK8Jet2Tau31_);
  tree->Branch("AK8Jet2Tau32",&AK8Jet2Tau32_);
  tree->Branch("AK8Jet2SubjetMaxBDisc",&AK8Jet2SubjetMaxBDisc_);



  //topjets
  tree->Branch("TopJet1Pt",&TopJet1Pt_);
  tree->Branch("TopJet1Eta",&TopJet1Eta_);
  tree->Branch("TopJet1Phi",&TopJet1Phi_);
  tree->Branch("TopJet1Energy",&TopJet1Energy_);
  tree->Branch("TopJet1TrimMass",&TopJet1TrimMass_);
  tree->Branch("TopJet1PruneMass",&TopJet1PruneMass_);
  tree->Branch("TopJet1SDMass",&TopJet1SDMass_);
  tree->Branch("TopJet1FiltMass",&TopJet1FiltMass_);
  tree->Branch("TopJet1Tau1",&TopJet1Tau1_);
  tree->Branch("TopJet1Tau2",&TopJet1Tau2_);
  tree->Branch("TopJet1Tau3",&TopJet1Tau3_);
  tree->Branch("TopJet1Tau21",&TopJet1Tau21_);
  tree->Branch("TopJet1Tau31",&TopJet1Tau31_);
  tree->Branch("TopJet1Tau32",&TopJet1Tau32_);
  tree->Branch("TopJet1SubjetMaxBDisc",&TopJet1SubjetMaxBDisc_);

  tree->Branch("TopJet2Pt",&TopJet2Pt_);
  tree->Branch("TopJet2Eta",&TopJet2Eta_);
  tree->Branch("TopJet2Phi",&TopJet2Phi_);
  tree->Branch("TopJet2Energy",&TopJet2Energy_);
  tree->Branch("TopJet2TrimMass",&TopJet2TrimMass_);
  tree->Branch("TopJet2PruneMass",&TopJet2PruneMass_);
  tree->Branch("TopJet2SDMass",&TopJet2SDMass_);
  tree->Branch("TopJet2FiltMass",&TopJet2FiltMass_);
  tree->Branch("TopJet2Tau1",&TopJet2Tau1_);
  tree->Branch("TopJet2Tau2",&TopJet2Tau2_);
  tree->Branch("TopJet2Tau3",&TopJet2Tau3_);
  tree->Branch("TopJet2Tau21",&TopJet2Tau21_);
  tree->Branch("TopJet2Tau31",&TopJet2Tau31_);
  tree->Branch("TopJet2Tau32",&TopJet2Tau32_);
  tree->Branch("TopJet2SubjetMaxBDisc",&TopJet2SubjetMaxBDisc_);

  //W jets
  tree->Branch("WJet1Pt",&WJet1Pt_);
  tree->Branch("WJet1Eta",&WJet1Eta_);
  tree->Branch("WJet1Phi",&WJet1Phi_);
  tree->Branch("WJet1Energy",&WJet1Energy_);
  tree->Branch("WJet1TrimMass",&WJet1TrimMass_);
  tree->Branch("WJet1PruneMass",&WJet1PruneMass_);
  tree->Branch("WJet1SDMass",&WJet1SDMass_);
  tree->Branch("WJet1FiltMass",&WJet1FiltMass_);
  tree->Branch("WJet1Tau1",&WJet1Tau1_);
  tree->Branch("WJet1Tau2",&WJet1Tau2_);
  tree->Branch("WJet1Tau3",&WJet1Tau3_);
  tree->Branch("WJet1Tau21",&WJet1Tau21_);
  tree->Branch("WJet1Tau31",&WJet1Tau31_);
  tree->Branch("WJet1Tau32",&WJet1Tau32_);
  tree->Branch("WJet1SubjetMaxBDisc",&WJet1SubjetMaxBDisc_);

  tree->Branch("WJet2Pt",&WJet2Pt_);
  tree->Branch("WJet2Eta",&WJet2Eta_);
  tree->Branch("WJet2Phi",&WJet2Phi_);
  tree->Branch("WJet2Energy",&WJet2Energy_);
  tree->Branch("WJet2TrimMass",&WJet2TrimMass_);
  tree->Branch("WJet2PruneMass",&WJet2PruneMass_);
  tree->Branch("WJet2SDMass",&WJet2SDMass_);
  tree->Branch("WJet2FiltMass",&WJet2FiltMass_);
  tree->Branch("WJet2Tau1",&WJet2Tau1_);
  tree->Branch("WJet2Tau2",&WJet2Tau2_);
  tree->Branch("WJet2Tau3",&WJet2Tau3_);
  tree->Branch("WJet2Tau21",&WJet2Tau21_);
  tree->Branch("WJet2Tau31",&WJet2Tau31_);
  tree->Branch("WJet2Tau32",&WJet2Tau32_);
  tree->Branch("WJet2SubjetMaxBDisc",&WJet2SubjetMaxBDisc_);


  tree->Branch("NTopTags",&nTopTaggedJets_);
  tree->Branch("NWTags",&nWTaggedJets_);
  tree->Branch("X53Mass",&X53mass_);

  //cleaned jets
  tree->Branch("nNewCleanAK4Jets",&nNewCleanAK4Jets_);
  tree->Branch("newCleanAK4HT",&newCleanAK4HT_);
  tree->Branch("newCleanAK4Jet1Pt",&newCleanAK4Jet1Pt_);
  tree->Branch("newCleanAK4Jet1Eta",&newCleanAK4Jet1Eta_);
  tree->Branch("newCleanAK4Jet1Phi",&newCleanAK4Jet1Phi_);
  tree->Branch("newCleanAK4Jet1Energy",&newCleanAK4Jet1Energy_);

  tree->Branch("newCleanAK4Jet2Pt",&newCleanAK4Jet2Pt_);
  tree->Branch("newCleanAK4Jet2Eta",&newCleanAK4Jet2Eta_);
  tree->Branch("newCleanAK4Jet2Phi",&newCleanAK4Jet2Phi_);
  tree->Branch("newCleanAK4Jet2Energy",&newCleanAK4Jet2Energy_);  

  tree->Branch("DilepMass",&DilepMass_);
  tree->Branch("AssocMass",&AssocMass_);
  tree->Branch("Channel",&nMu_);
  tree->Branch("nPV",&nPrimaryVert_);
}

void TreeMaker::FillTree(std::vector<TLepton*> vSSLep, std::vector<TJet*> AK4Jets, std::vector<TJet*> cleanAK4Jets, std::vector<TJet*> simpleCleanAK4Jets, float HTtemp, float METtemp, float DilepMasstemp, int nMu, float weight, std::vector<TLepton*> vNonSSLep,float mcweight, float NPWeighttemp,float NPWeightElUptemp,float NPWeightElDowntemp,float NPWeightMuUptemp, float NPWeightMuDowntemp,std::vector<float> NPWeightScantemp,float NPWeightElPRUptemp,float NPWeightElPRDowntemp,float NPWeightMuPRUptemp, float NPWeightMuPRDowntemp,float NPaltTemp,float SUSYNPTemp, int nTLtemp, float trSF, float trSFup, float trSFdn, float idSF, float idSFup, float idSFdn, float isoSF,float gsfSF, float puwtemp,float puUptemp, float puDowntemp, double prefirewtemp, double prefireUptemp, double prefireDowntemp, float amasst, std::vector<TBoostedJet*> AK8Jets,std::vector<THadronicGenJet*> hadronicGenJets,bool mc,int run, int lumi, int event,int nPV, std::vector<double> pdfweightstemp, std::vector<double> renormWeightstemp, std::vector<double> pdfNewWeightstemp, std::vector<double> pdfWeights4LHCtemp, std::vector<double> pdfWeightsMSTWtemp, double pdfNewNominalWeighttemp){

  bool debug = false;

  run_=run;
  lumi_=lumi;
  event_=event;
  nPrimaryVert_ = nPV;
  weight_=weight;
  NPWeight_=NPWeighttemp;
  NPWeightElUp_=NPWeightElUptemp;
  NPWeightElDown_=NPWeightElDowntemp;
  NPWeightMuUp_=NPWeightMuUptemp;
  NPWeightMuDown_=NPWeightMuDowntemp;
  NPWeightScan_ = NPWeightScantemp;
  NPWeightElPRUp_=NPWeightElPRUptemp;
  NPWeightElPRDown_=NPWeightElPRDowntemp;
  NPWeightMuPRUp_=NPWeightMuPRUptemp;
  NPWeightMuPRDown_=NPWeightMuPRDowntemp;
  NPAltWeight_=NPaltTemp;
  NPSUSYWeight_=SUSYNPTemp;
  nTL_ = nTLtemp;
  if(mcweight>=0) MCWeight_=1;
  else MCWeight_=-1;

  trigSF_=trSF;
  trigSFup_=trSFup;
  trigSFdn_=trSFdn;
  IDSF_ = idSF;
  IDSFup_ = idSFup;
  IDSFdn_ = idSFdn;
  IsoSF_ = isoSF;
  gsfSF_ = gsfSF;
  puweight_=puwtemp;
  puweightUp_ = puUptemp;
  puweightDown_ = puDowntemp;
  prefireWeight_ = prefirewtemp;
  prefireWeightUp_ = prefireUptemp;
  prefireWeightDown_ = prefireDowntemp;

  pdfweights_ = pdfweightstemp;
  renormWeights_ = renormWeightstemp;
  pdfNewWeights_ = pdfNewWeightstemp;
  pdfWeights4LHC_ = pdfWeights4LHCtemp;
  pdfWeightsMSTW_ = pdfWeightsMSTWtemp;
  pdfNewNominalWeight_ = pdfNewNominalWeighttemp;
  if(debug) std::cout<< "pdfweights_ size " << pdfweights_.size() << std::endl;

  assert(vSSLep.size()>1);

  Lep1Pt_=vSSLep.at(0)->pt;
  Lep1Eta_=vSSLep.at(0)->eta;
  Lep1Phi_=vSSLep.at(0)->phi;
  Lep1Energy_=vSSLep.at(0)->energy;
  vSSLep.at(0)->isMu ? Lep1Flavor_=1 : Lep1Flavor_=0;
  Lep1RelIso_ = vSSLep.at(0)->relIso;
  Lep1MiniIso_ = vSSLep.at(0)->miniIso;
  Lep1SusyIso_ = vSSLep.at(0)->susyIso;
  Lep1Charge_ = vSSLep.at(0)->charge;

  Lep2Pt_=vSSLep.at(1)->pt;
  Lep2Eta_=vSSLep.at(1)->eta;
  Lep2Phi_=vSSLep.at(1)->phi;
  Lep2Energy_=vSSLep.at(1)->energy;
  vSSLep.at(1)->isMu ? Lep2Flavor_=1 : Lep2Flavor_=0;
  Lep2RelIso_ = vSSLep.at(1)->relIso;
  Lep2MiniIso_ = vSSLep.at(1)->miniIso;
  Lep2SusyIso_ = vSSLep.at(1)->susyIso;
  Lep2Charge_ = vSSLep.at(1)->charge;
  //if(Lep2Pt_<20) std::cout<<"Lep2Pt "<<vSSLep.at(1)->pt<<std::endl;

  AK4HT_=HTtemp;
  AK4HT_+=Lep1Pt_+Lep2Pt_;
  for(unsigned int ilep=0; ilep < vNonSSLep.size(); ilep++){
    AK4HT_+=vNonSSLep.at(ilep)->pt;
  }
  MET_=METtemp;

  nAK4Jets_ = AK4Jets.size();

  if(nAK4Jets_>0){
    AK4Jet1Pt_=AK4Jets.at(0)->pt;    
    AK4Jet1Eta_=AK4Jets.at(0)->eta;
    AK4Jet1Phi_=AK4Jets.at(0)->phi;
    AK4Jet1Energy_=AK4Jets.at(0)->energy;
  }
  else{
    AK4Jet1Pt_=-999;
    AK4Jet1Eta_=-999;
    AK4Jet1Phi_=-999;
    AK4Jet1Energy_=-999;
  }

  if(nAK4Jets_>1){
    AK4Jet2Pt_=AK4Jets.at(1)->pt;    
    AK4Jet2Eta_=AK4Jets.at(1)->eta;
    AK4Jet2Phi_=AK4Jets.at(1)->phi;
    AK4Jet2Energy_=AK4Jets.at(1)->energy;
  }
  else{
    AK4Jet2Pt_=-999;
    AK4Jet2Eta_=-999;
    AK4Jet2Phi_=-999;
    AK4Jet2Energy_=-999;
  }


  //cleaned jets
  nCleanAK4Jets_ = cleanAK4Jets.size();
  nCleanAK4JetsScaleUp_ = 0;
  nCleanAK4JetsScaleDown_ = 0;
  nCleanAK4JetsSmearUp_ = 0;
  nCleanAK4JetsSmearDown_ = 0;
  cleanAK4HT_=Lep1Pt_+Lep2Pt_;
  cleanAK4HTScaleUp_ = Lep1Pt_+Lep2Pt_;
  cleanAK4HTScaleDown_ = Lep1Pt_+Lep2Pt_;
  cleanAK4HTSmearUp_ = Lep1Pt_+Lep2Pt_;
  cleanAK4HTSmearDown_ = Lep1Pt_+Lep2Pt_;
  for(int i=0; i<nCleanAK4Jets_;i++){
    cleanAK4HT_+=cleanAK4Jets.at(i)->pt;
    cleanAK4HTScaleUp_ += cleanAK4Jets.at(i)->scaleUpPt;
    cleanAK4HTScaleDown_ += cleanAK4Jets.at(i)->scaleDownPt;
    cleanAK4HTSmearUp_ += cleanAK4Jets.at(i)->smearUpPt;
    cleanAK4HTSmearDown_ += cleanAK4Jets.at(i)->smearDownPt;

    if(cleanAK4Jets.at(i)->scaleUpPt > 30){ nCleanAK4JetsScaleUp_++; }
    if(cleanAK4Jets.at(i)->scaleDownPt > 30){ nCleanAK4JetsScaleDown_++; }
    if(cleanAK4Jets.at(i)->smearUpPt > 30){ nCleanAK4JetsSmearUp_++; }
    if(cleanAK4Jets.at(i)->smearDownPt > 30){ nCleanAK4JetsSmearDown_++; }

  }

  for(unsigned int ilep=0; ilep < vNonSSLep.size(); ilep++){
    cleanAK4HT_+=vNonSSLep.at(ilep)->pt;
    cleanAK4HTScaleUp_+=vNonSSLep.at(ilep)->pt;
    cleanAK4HTScaleDown_+=vNonSSLep.at(ilep)->pt;
    cleanAK4HTSmearUp_+=vNonSSLep.at(ilep)->pt;
    cleanAK4HTSmearDown_+=vNonSSLep.at(ilep)->pt;
  }

  if(nCleanAK4Jets_>0){
    cleanAK4Jet1Pt_=cleanAK4Jets.at(0)->pt;    
    cleanAK4Jet1Eta_=cleanAK4Jets.at(0)->eta;
    cleanAK4Jet1Phi_=cleanAK4Jets.at(0)->phi;
    cleanAK4Jet1Energy_=cleanAK4Jets.at(0)->energy;
  }
  else{
    cleanAK4Jet1Pt_=-999;
    cleanAK4Jet1Eta_=-999;
    cleanAK4Jet1Phi_=-999;
    cleanAK4Jet1Energy_=-999;
  }

  if(nCleanAK4Jets_>1){
    cleanAK4Jet2Pt_=cleanAK4Jets.at(1)->pt;    
    cleanAK4Jet2Eta_=cleanAK4Jets.at(1)->eta;
    cleanAK4Jet2Phi_=cleanAK4Jets.at(1)->phi;
    cleanAK4Jet2Energy_=cleanAK4Jets.at(1)->energy;
  }
  else{
    cleanAK4Jet2Pt_=-999;
    cleanAK4Jet2Eta_=-999;
    cleanAK4Jet2Phi_=-999;
    cleanAK4Jet2Energy_=-999;
  }

  //cleaned jets
  nSimpleCleanAK4Jets_ = simpleCleanAK4Jets.size();
  simpleCleanAK4HT_=Lep1Pt_+Lep2Pt_;
  for(int i=0; i<nSimpleCleanAK4Jets_;i++){
    simpleCleanAK4HT_+=simpleCleanAK4Jets.at(i)->pt;
  }
  for(unsigned int ilep=0; ilep < vNonSSLep.size(); ilep++){
    AK4HT_+=vNonSSLep.at(ilep)->pt;
  }

  if(nSimpleCleanAK4Jets_ >0){
    //std::cout<<"filling cleaned jet 0 "<<std::endl;
    simpleCleanAK4Jet1Pt_=simpleCleanAK4Jets.at(0)->pt;    
    simpleCleanAK4Jet1Eta_=simpleCleanAK4Jets.at(0)->eta;
    simpleCleanAK4Jet1Phi_=simpleCleanAK4Jets.at(0)->phi;
    simpleCleanAK4Jet1Energy_=simpleCleanAK4Jets.at(0)->energy;
  }
  else{
    simpleCleanAK4Jet1Pt_=-999;
    simpleCleanAK4Jet1Eta_=-999;
    simpleCleanAK4Jet1Phi_=-999;
    simpleCleanAK4Jet1Energy_=-999;
  }

  if(nSimpleCleanAK4Jets_>1){
    simpleCleanAK4Jet2Pt_=simpleCleanAK4Jets.at(1)->pt;    
    simpleCleanAK4Jet2Eta_=simpleCleanAK4Jets.at(1)->eta;
    simpleCleanAK4Jet2Phi_=simpleCleanAK4Jets.at(1)->phi;
    simpleCleanAK4Jet2Energy_=simpleCleanAK4Jets.at(1)->energy;
  }
  else{
    simpleCleanAK4Jet2Pt_=-999;
    simpleCleanAK4Jet2Eta_=-999;
    simpleCleanAK4Jet2Phi_=-999;
    simpleCleanAK4Jet2Energy_=-999;
  }


  //ak8jets
  int nAK8Jets_ = AK8Jets.size();
  //make tagged ak8 jets
  std::vector<TBoostedJet*> taggedAK8Jets;
  std::vector<TBoostedJet*> topTaggedJets;
  std::vector<TBoostedJet*> wTaggedJets;
  for(int i=0; i<nAK8Jets_; i++){
    if(AK8Jets.at(i)->isWTag_JMAR() || AK8Jets.at(i)->isTopTag_BTag_LJets()){
      //std::cout<<"Pruned jet mass for jet: "<<i<<" is: "<<AK8Jets.at(i)->prunedMass<<std::endl;
      //std::cout<<"Tau21 for jet: "<<i<<" is: "<<AK8Jets.at(i)->tau2 / AK8Jets.at(i)->tau1<<std::endl;
      taggedAK8Jets.push_back(AK8Jets.at(i));
      if(AK8Jets.at(i)->isTopTag_LJets()) topTaggedJets.push_back(AK8Jets.at(i));
      if(AK8Jets.at(i)->isWTag_JMAR() && !(AK8Jets.at(i)->isTopTag_LJets())) wTaggedJets.push_back(AK8Jets.at(i));
    }
  }

  nTopTaggedJets_ = topTaggedJets.size();
  nWTaggedJets_ = wTaggedJets.size();

  int nTaggedAK8Jets_ = taggedAK8Jets.size();

  //get tagging scale factors
  if(mc){
    //set toptagged jets to matched
    for(unsigned int i=0; i<topTaggedJets.size(); i++){
      TBoostedJet* tj = topTaggedJets.at(i);
      float minDR = 9999.9;
      THadronicGenJet* testjet=0;
      //find closest jet
      for(unsigned int j=0; j< hadronicGenJets.size(); j++){
	THadronicGenJet* thj = hadronicGenJets.at(j);
	float dR = tj->lv.DeltaR(thj->lv);
	if(dR< minDR){testjet = thj; minDR=dR;}
      }
      //now make sure testjet is within dR0.8 of tagged jet
      if(minDR<0.8){
	//make sure test jet has correct id
	if(abs(testjet->id)==6){
	  //now makesure the daughters are inside the ak8jet
	  TLorentzVector lvd0; lvd0.SetPtEtaPhiE(testjet->daughter0Pt,testjet->daughter0Eta,testjet->daughter0Phi,testjet->daughter0Energy);
	  TLorentzVector lvd1; lvd1.SetPtEtaPhiE(testjet->daughter1Pt,testjet->daughter1Eta,testjet->daughter1Phi,testjet->daughter1Energy);
	  TLorentzVector lvd2; lvd2.SetPtEtaPhiE(testjet->daughter2Pt,testjet->daughter2Eta,testjet->daughter2Phi,testjet->daughter2Energy);
	  float d0dR = tj->lv.DeltaR(lvd0);
	  float d1dR = tj->lv.DeltaR(lvd1);
	  float d2dR = tj->lv.DeltaR(lvd2);
	  if( d0dR < 0.8 && d1dR < 0.8 && d2dR<0.8){
	    //pass all requirements for matching!
	    topTaggedJets.at(i)->SetGenMatched();
	  }//end check on daughters being with top jet
	}//end check on hadronic jet being from top quark
      }//end check on mindr <0.8
    }//end loop on top tagged jets

    //now set wtagged jets to matched
    for(unsigned int i=0; i<wTaggedJets.size(); i++){
      TBoostedJet* tj = wTaggedJets.at(i);
      float minDR = 9999.9;
      THadronicGenJet* testjet=0;
      //find closest jet
      for(unsigned int j=0; j< hadronicGenJets.size(); j++){
	THadronicGenJet* thj = hadronicGenJets.at(j);
	float dR = tj->lv.DeltaR(thj->lv);
	if(dR< minDR){testjet = thj; minDR=dR;}
      }
      //now make sure testjet is within dR0.8 of tagged jet
      if(minDR<0.8){
	//make sure test jet has correct id
	if(abs(testjet->id)==24){
	  //now makesure the daughters are inside the ak8jet
	  TLorentzVector lvd0; lvd0.SetPtEtaPhiE(testjet->daughter0Pt,testjet->daughter0Eta,testjet->daughter0Phi,testjet->daughter0Energy);
	  TLorentzVector lvd1; lvd1.SetPtEtaPhiE(testjet->daughter1Pt,testjet->daughter1Eta,testjet->daughter1Phi,testjet->daughter1Energy);
	  float d0dR = tj->lv.DeltaR(lvd0);
	  float d1dR = tj->lv.DeltaR(lvd1);
	  if( d0dR < 0.8 && d1dR < 0.8){
	    //pass all requirements for matching!
	    wTaggedJets.at(i)->SetGenMatched();
	  }//end check on daughters being with w jet
	}//end check on hadronic jet being from w quark
      }//end check on mindr <0.8
    }//end loop on wjets
  }
 
  //ok, have info on whether the jets are matched, so now calculate scalefactors
  JetTaggingSF_=1.0;
  //should never get to modify scale factor unless mc, but for safety
  if(mc){
    for(unsigned int i=0; i<topTaggedJets.size();i++){
      TBoostedJet *tj = topTaggedJets.at(i);
      if(tj->genMatched){
	if(tj->pt>400 && tj->pt<550) JetTaggingSF_ *= 0.89;
	else if(tj->pt>55) JetTaggingSF_ *= 1.08;
      }//end check on genMatched
    }//end loop on top jets
    
    for(unsigned int i=0; i<wTaggedJets.size();i++){
      TBoostedJet *tj = wTaggedJets.at(i);
      if(tj->genMatched){
	//if(tj->pt>400 && tj->pt<550) JetTaggingSF_ *= 0.89;
	//else if(tj->pt>55) JetTaggingSF_ *= 1.08;
      }//end check on genMatched
    }//end loop on w jets
  }
  if(nTaggedAK8Jets_>0){
    AK8Jet1Pt_=taggedAK8Jets.at(0)->pt;    
    AK8Jet1Eta_=taggedAK8Jets.at(0)->eta;
    AK8Jet1Phi_=taggedAK8Jets.at(0)->phi;
    AK8Jet1Energy_=taggedAK8Jets.at(0)->energy;
    AK8Jet1TrimMass_=taggedAK8Jets.at(0)->trimmedMass;
    AK8Jet1PruneMass_=taggedAK8Jets.at(0)->prunedMass;
    AK8Jet1SDMass_=taggedAK8Jets.at(0)->softDropMass;
    AK8Jet1FiltMass_=taggedAK8Jets.at(0)->filteredMass;
    AK8Jet1Tau1_ = taggedAK8Jets.at(0)->tau1;
    AK8Jet1Tau2_ = taggedAK8Jets.at(0)->tau2;
    AK8Jet1Tau3_ = taggedAK8Jets.at(0)->tau3;
    AK8Jet1Tau21_ = taggedAK8Jets.at(0)->tau2/taggedAK8Jets.at(0)->tau1;
    AK8Jet1Tau31_ = taggedAK8Jets.at(0)->tau3/taggedAK8Jets.at(0)->tau1;
    AK8Jet1Tau32_ = taggedAK8Jets.at(0)->tau3/taggedAK8Jets.at(0)->tau2;
    float maxBDisc1=-2;
    for(int ib=0; ib<taggedAK8Jets.at(0)->nSubjets; ib++){
      if(taggedAK8Jets.at(0)->subjetBDisc.at(ib) >= maxBDisc1) maxBDisc1=taggedAK8Jets.at(0)->subjetBDisc.at(ib);
    }
    AK8Jet1SubjetMaxBDisc_ = maxBDisc1;
  }
  else{
    AK8Jet1Pt_=-999;
    AK8Jet1Eta_=-999;
    AK8Jet1Phi_=-999;
    AK8Jet1Energy_=-999;
    AK8Jet1TrimMass_=-999;
    AK8Jet1PruneMass_=-999;
    AK8Jet1SDMass_=-999;
    AK8Jet1FiltMass_=-999;
    AK8Jet1Tau1_ = -999;
    AK8Jet1Tau2_ = -999;
    AK8Jet1Tau3_ = -999;
    AK8Jet1Tau21_ = -999;
    AK8Jet1Tau31_ = -999;
    AK8Jet1Tau32_ = -999;
    AK8Jet1SubjetMaxBDisc_ =-999;
  }

  if(nTaggedAK8Jets_>1){
    AK8Jet2Pt_=taggedAK8Jets.at(1)->pt;    
    AK8Jet2Eta_=taggedAK8Jets.at(1)->eta;
    AK8Jet2Phi_=taggedAK8Jets.at(1)->phi;
    AK8Jet2Energy_=taggedAK8Jets.at(1)->energy;
    AK8Jet2TrimMass_=taggedAK8Jets.at(1)->trimmedMass;
    AK8Jet2PruneMass_=taggedAK8Jets.at(1)->prunedMass;
    AK8Jet2SDMass_=taggedAK8Jets.at(1)->softDropMass;
    AK8Jet2FiltMass_=taggedAK8Jets.at(1)->filteredMass;
    AK8Jet2Tau1_ = taggedAK8Jets.at(1)->tau1;
    AK8Jet2Tau2_ = taggedAK8Jets.at(1)->tau2;
    AK8Jet2Tau3_ = taggedAK8Jets.at(1)->tau3;
    AK8Jet2Tau21_ = taggedAK8Jets.at(1)->tau2/taggedAK8Jets.at(1)->tau1;
    AK8Jet2Tau31_ = taggedAK8Jets.at(1)->tau3/taggedAK8Jets.at(1)->tau1;
    AK8Jet2Tau32_ = taggedAK8Jets.at(1)->tau3/taggedAK8Jets.at(1)->tau2;
    float maxBDisc=-2;
    for(int ib=0; ib<taggedAK8Jets.at(1)->nSubjets; ib++){
      if(taggedAK8Jets.at(1)->subjetBDisc.at(ib) >= maxBDisc) maxBDisc=taggedAK8Jets.at(1)->subjetBDisc.at(ib);
    }
    AK8Jet2SubjetMaxBDisc_ = maxBDisc;
  }
  else{
    AK8Jet2Pt_=-999;
    AK8Jet2Eta_=-999;
    AK8Jet2Phi_=-999;
    AK8Jet2Energy_=-999;
    AK8Jet2TrimMass_=-999;
    AK8Jet2PruneMass_=-999;
    AK8Jet2SDMass_=-999;
    AK8Jet2FiltMass_=-999;
    AK8Jet2Tau1_ = -999;
    AK8Jet2Tau2_ = -999;
    AK8Jet2Tau3_ = -999;
    AK8Jet2Tau21_ = -999;
    AK8Jet2Tau31_ = -999;
    AK8Jet2Tau32_ = -999;
    AK8Jet2SubjetMaxBDisc_ =-999;
  }


  if(nTopTaggedJets_>0){
    TopJet1Pt_=topTaggedJets.at(0)->pt;    
    TopJet1Eta_=topTaggedJets.at(0)->eta;
    TopJet1Phi_=topTaggedJets.at(0)->phi;
    TopJet1Energy_=topTaggedJets.at(0)->energy;
    TopJet1TrimMass_=topTaggedJets.at(0)->trimmedMass;
    TopJet1PruneMass_=topTaggedJets.at(0)->prunedMass;
    TopJet1SDMass_=topTaggedJets.at(0)->softDropMass;
    TopJet1FiltMass_=topTaggedJets.at(0)->filteredMass;
    TopJet1Tau1_ = topTaggedJets.at(0)->tau1;
    TopJet1Tau2_ = topTaggedJets.at(0)->tau2;
    TopJet1Tau3_ = topTaggedJets.at(0)->tau3;
    TopJet1Tau21_ = topTaggedJets.at(0)->tau2/topTaggedJets.at(0)->tau1;
    TopJet1Tau31_ = topTaggedJets.at(0)->tau3/topTaggedJets.at(0)->tau1;
    TopJet1Tau32_ = topTaggedJets.at(0)->tau3/topTaggedJets.at(0)->tau2;
    float maxBDisc1=-2;
    for(int ib=0; ib<topTaggedJets.at(0)->nSubjets; ib++){
      if(topTaggedJets.at(0)->subjetBDisc.at(ib) >= maxBDisc1) maxBDisc1=topTaggedJets.at(0)->subjetBDisc.at(ib);
    }
    TopJet1SubjetMaxBDisc_ = maxBDisc1;
  }
  else{
    TopJet1Pt_=-999;
    TopJet1Eta_=-999;
    TopJet1Phi_=-999;
    TopJet1Energy_=-999;
    TopJet1TrimMass_=-999;
    TopJet1PruneMass_=-999;
    TopJet1SDMass_=-999;
    TopJet1FiltMass_=-999;
    TopJet1Tau1_ = -999;
    TopJet1Tau2_ = -999;
    TopJet1Tau3_ = -999;
    TopJet1Tau21_ = -999;
    TopJet1Tau31_ = -999;
    TopJet1Tau32_ = -999;
    TopJet1SubjetMaxBDisc_ =-999;
  }

  if(nTopTaggedJets_>1){
    TopJet2Pt_=topTaggedJets.at(1)->pt;    
    TopJet2Eta_=topTaggedJets.at(1)->eta;
    TopJet2Phi_=topTaggedJets.at(1)->phi;
    TopJet2Energy_=topTaggedJets.at(1)->energy;
    TopJet2TrimMass_=topTaggedJets.at(1)->trimmedMass;
    TopJet2PruneMass_=topTaggedJets.at(1)->prunedMass;
    TopJet2SDMass_=topTaggedJets.at(1)->softDropMass;
    TopJet2FiltMass_=topTaggedJets.at(1)->filteredMass;
    TopJet2Tau1_ = topTaggedJets.at(1)->tau1;
    TopJet2Tau2_ = topTaggedJets.at(1)->tau2;
    TopJet2Tau3_ = topTaggedJets.at(1)->tau3;
    TopJet2Tau21_ = topTaggedJets.at(1)->tau2/topTaggedJets.at(1)->tau1;
    TopJet2Tau31_ = topTaggedJets.at(1)->tau3/topTaggedJets.at(1)->tau1;
    TopJet2Tau32_ = topTaggedJets.at(1)->tau3/topTaggedJets.at(1)->tau2;
    float maxBDisc=-2;
    for(int ib=0; ib<topTaggedJets.at(1)->nSubjets; ib++){
      if(topTaggedJets.at(1)->subjetBDisc.at(ib) >= maxBDisc) maxBDisc=topTaggedJets.at(1)->subjetBDisc.at(ib);
    }
    TopJet2SubjetMaxBDisc_ = maxBDisc;
  }
  else{
    TopJet2Pt_=-999;
    TopJet2Eta_=-999;
    TopJet2Phi_=-999;
    TopJet2Energy_=-999;
    TopJet2TrimMass_=-999;
    TopJet2PruneMass_=-999;
    TopJet2SDMass_=-999;
    TopJet2FiltMass_=-999;
    TopJet2Tau1_ = -999;
    TopJet2Tau2_ = -999;
    TopJet2Tau3_ = -999;
    TopJet2Tau21_ = -999;
    TopJet2Tau31_ = -999;
    TopJet2Tau32_ = -999;
    TopJet2SubjetMaxBDisc_ =-999;
  }


  if(nWTaggedJets_>0){
    WJet1Pt_=wTaggedJets.at(0)->pt;    
    WJet1Eta_=wTaggedJets.at(0)->eta;
    WJet1Phi_=wTaggedJets.at(0)->phi;
    WJet1Energy_=wTaggedJets.at(0)->energy;
    WJet1TrimMass_=wTaggedJets.at(0)->trimmedMass;
    WJet1PruneMass_=wTaggedJets.at(0)->prunedMass;
    WJet1SDMass_=wTaggedJets.at(0)->softDropMass;
    WJet1FiltMass_=wTaggedJets.at(0)->filteredMass;
    WJet1Tau1_ = wTaggedJets.at(0)->tau1;
    WJet1Tau2_ = wTaggedJets.at(0)->tau2;
    WJet1Tau3_ = wTaggedJets.at(0)->tau3;
    WJet1Tau21_ = wTaggedJets.at(0)->tau2/wTaggedJets.at(0)->tau1;
    WJet1Tau31_ = wTaggedJets.at(0)->tau3/wTaggedJets.at(0)->tau1;
    WJet1Tau32_ = wTaggedJets.at(0)->tau3/wTaggedJets.at(0)->tau2;
    float maxBDisc1=-2;
    for(int ib=0; ib<wTaggedJets.at(0)->nSubjets; ib++){
      if(wTaggedJets.at(0)->subjetBDisc.at(ib) >= maxBDisc1) maxBDisc1=wTaggedJets.at(0)->subjetBDisc.at(ib);
    }
    WJet1SubjetMaxBDisc_ = maxBDisc1;
  }
  else{
    WJet1Pt_=-999;
    WJet1Eta_=-999;
    WJet1Phi_=-999;
    WJet1Energy_=-999;
    WJet1TrimMass_=-999;
    WJet1PruneMass_=-999;
    WJet1SDMass_=-999;
    WJet1FiltMass_=-999;
    WJet1Tau1_ = -999;
    WJet1Tau2_ = -999;
    WJet1Tau3_ = -999;
    WJet1Tau21_ = -999;
    WJet1Tau31_ = -999;
    WJet1Tau32_ = -999;
    WJet1SubjetMaxBDisc_ =-999;
  }

  if(nWTaggedJets_>1){
    WJet2Pt_=wTaggedJets.at(1)->pt;    
    WJet2Eta_=wTaggedJets.at(1)->eta;
    WJet2Phi_=wTaggedJets.at(1)->phi;
    WJet2Energy_=wTaggedJets.at(1)->energy;
    WJet2TrimMass_=wTaggedJets.at(1)->trimmedMass;
    WJet2PruneMass_=wTaggedJets.at(1)->prunedMass;
    WJet2SDMass_=wTaggedJets.at(1)->softDropMass;
    WJet2FiltMass_=wTaggedJets.at(1)->filteredMass;
    WJet2Tau1_ = wTaggedJets.at(1)->tau1;
    WJet2Tau2_ = wTaggedJets.at(1)->tau2;
    WJet2Tau3_ = wTaggedJets.at(1)->tau3;
    WJet2Tau21_ = wTaggedJets.at(1)->tau2/wTaggedJets.at(1)->tau1;
    WJet2Tau31_ = wTaggedJets.at(1)->tau3/wTaggedJets.at(1)->tau1;
    WJet2Tau32_ = wTaggedJets.at(1)->tau3/wTaggedJets.at(1)->tau2;
    float maxBDisc=-2;
    for(int ib=0; ib<wTaggedJets.at(1)->nSubjets; ib++){
      if(wTaggedJets.at(1)->subjetBDisc.at(ib) >= maxBDisc) maxBDisc=wTaggedJets.at(1)->subjetBDisc.at(ib);
    }
    WJet2SubjetMaxBDisc_ = maxBDisc;
  }
  else{
    WJet2Pt_=-999;
    WJet2Eta_=-999;
    WJet2Phi_=-999;
    WJet2Energy_=-999;
    WJet2TrimMass_=-999;
    WJet2PruneMass_=-999;
    WJet2SDMass_=-999;
    WJet2FiltMass_=-999;
    WJet2Tau1_ = -999;
    WJet2Tau2_ = -999;
    WJet2Tau3_ = -999;
    WJet2Tau21_ = -999;
    WJet2Tau31_ = -999;
    WJet2Tau32_ = -999;
    WJet2SubjetMaxBDisc_ =-999;
  }


  //new clean ak4 jets
  std::vector<TJet*> newCleanAK4Jets;
  for(unsigned int i=0; i<cleanAK4Jets.size(); i++){
    bool save = true;
    TJet* jet = cleanAK4Jets.at(i);
    for(int j=0; j<nTaggedAK8Jets_; j++){
      float dR = pow( pow(jet->eta - taggedAK8Jets.at(j)->eta,2) + pow(jet->phi - taggedAK8Jets.at(j)->phi, 2), 0.5);
      //std::cout<<"dR for ak4 jet: "<<i<<" and ak8 jet: "<<j<<" is: "<<dR<<std::endl;
      if(dR < 0.8){
	save=false;
	//std::cout<<"vetoing jet"<<std::endl;
      }
    }
    if(save) {
      //std::cout<<"saving jet with index: "<<i<<std::endl;
      newCleanAK4Jets.push_back(jet);
    }

  }

  nNewCleanAK4Jets_ = newCleanAK4Jets.size();
  //if(nNewCleanAK4Jets_>10) std::cout<<"Size of new AK4 jets: "<<nNewCleanAK4Jets_<<" and size of old: "<<nCleanAK4Jets_<<std::endl;
  if(nNewCleanAK4Jets_ > nCleanAK4Jets_) std::cout<<"something is broken because new ak4 collection is larger than old"<<std::endl;
  //new nConst
  nNewConst_ = newCleanAK4Jets.size() + vNonSSLep.size();
  nNonSSLeps_ = vNonSSLep.size();
  newCleanAK4HT_=Lep1Pt_+Lep2Pt_;
  for(int i=0; i<nNewCleanAK4Jets_;i++){
    newCleanAK4HT_+=newCleanAK4Jets.at(i)->pt;
  }
  for(unsigned int ilep=0; ilep < vNonSSLep.size(); ilep++){
    newCleanAK4HT_+=vNonSSLep.at(ilep)->pt;
  }
  for(int i=0; i<nTaggedAK8Jets_;i++){
    
    newCleanAK4HT_+=taggedAK8Jets.at(i)->pt;
    
    if(taggedAK8Jets.at(i)->isTopTag_BTag_LJets()) nNewConst_= nNewConst_+3;
    else if(taggedAK8Jets.at(i)->isWTag_JMAR() && !taggedAK8Jets.at(i)->isTopTag_BTag_LJets()) nNewConst_ = nNewConst_+2;
  }
  
  if(nNewCleanAK4Jets_>0){
    newCleanAK4Jet1Pt_=newCleanAK4Jets.at(0)->pt;    
    newCleanAK4Jet1Eta_=newCleanAK4Jets.at(0)->eta;
    newCleanAK4Jet1Phi_=newCleanAK4Jets.at(0)->phi;
    newCleanAK4Jet1Energy_=newCleanAK4Jets.at(0)->energy;
  }
  else{
    newCleanAK4Jet1Pt_=-999;
    newCleanAK4Jet1Eta_=-999;
    newCleanAK4Jet1Phi_=-999;
    newCleanAK4Jet1Energy_=-999;
  }

  if(nNewCleanAK4Jets_>1){
    newCleanAK4Jet2Pt_=newCleanAK4Jets.at(1)->pt;    
    newCleanAK4Jet2Eta_=newCleanAK4Jets.at(1)->eta;
    newCleanAK4Jet2Phi_=newCleanAK4Jets.at(1)->phi;
    newCleanAK4Jet2Energy_=newCleanAK4Jets.at(1)->energy;
  }
  else{
    newCleanAK4Jet2Pt_=-999;
    newCleanAK4Jet2Eta_=-999;
    newCleanAK4Jet2Phi_=-999;
    newCleanAK4Jet2Energy_=-999;
  }


  //mass reconstruction
  X53mass_=-99;
  TLorentzVector topjet, wjet;
  bool foundTop=false;
  bool foundW = false;
  //case for when the event has a top jet
  if(nTopTaggedJets_>0){
    //if any top tagged jets this is true
    foundTop=true;
    if(nTopTaggedJets_==1) topjet = topTaggedJets.at(0)->lv;
    //else more than one top tagged jet
    else{
      float mDiff=999;
      int index=0; //default to leading jet
      for(int i=0; i<nTopTaggedJets_;i++){
	float mDtemp = 173.3 - topTaggedJets.at(i)->softDropMass;
	if( fabs(mDtemp) < mDiff){
	  index = i;
	  mDiff = mDtemp;
	}
      }
      topjet = topTaggedJets.at(index)->lv;    
    }//end more than one top jet
    //now find w
    if(nWTaggedJets_>0){     
      //take wjet closest to wmass
      int ind=0; //default to leading;
      float maxDiff=9999;
      for(int i=0; i<nWTaggedJets_;i++){
	float tempdiff = fabs(wTaggedJets.at(i)->trimmedMass - 80.385);
	if(tempdiff<maxDiff) {ind = i; maxDiff=tempdiff;}
      }
      wjet=wTaggedJets.at(ind)->lv;
      foundW=true;
    }
    //no wtagged jet, find pair of jets that most closely gives wMass
    else{
      int ind1=0; int ind2=0;
      bool foundpair=false;
      float mdiff=9999999;
      for(int i=0; i<nNewCleanAK4Jets_;i++){
	for(int j =i+1; j<nNewCleanAK4Jets_;j++){
	  float mtemp = (newCleanAK4Jets.at(i)->lv + newCleanAK4Jets.at(j)->lv).M();
	  //look for pair closest to w and require the mass to be within 20 GeV
	  if(fabs( mtemp - 80.385) <mdiff && fabs(mtemp-80.385)<20){ ind1=i;ind2=j; foundpair=true;}	    
	}
      }
      if(foundpair) {wjet= newCleanAK4Jets.at(ind1)->lv+newCleanAK4Jets.at(ind2)->lv; foundW=true;}
      else wjet.SetPxPyPzE(0,0,0,0);    
    }//end else about no w tagged jet      
  }//end if has at least one top jet
  else{ //no top jets in event
    TLorentzVector  wjet1,wjet2, bjet;
    std::vector<int> usedJets;
    if(nWTaggedJets_>2){ // more than 2 w tags take two as candidates with closest trimmed masses to w mass (pruning skews peak to lower values while trimming doesn't)
      int ind1=0; int ind2=0;
      float diff1=99999;
      float diff2=99999;
      for(int i=0; i<nWTaggedJets_;i++){
	float diffTemp = 80.385 - wTaggedJets.at(i)->trimmedMass;
	if(fabs(diffTemp)<diff1) {ind1=i;diff1=diffTemp;}
      }
      for(int i=0; i<nWTaggedJets_;i++){
	float diffTemp = 80.385 - wTaggedJets.at(i)->trimmedMass;
	if(fabs(diffTemp)<diff2 && !(fabs(diffTemp)==diff1)) ind2=i;
      }
      wjet1=wTaggedJets.at(ind1)->lv;
      wjet2=wTaggedJets.at(ind2)->lv;
      //bjet is highest pT remaining ak4 jet
      if(nNewCleanAK4Jets_>0) bjet=newCleanAK4Jets.at(0)->lv;
      else bjet.SetPxPyPzE(0,0,0,0);
      
    }//end more than two w tags
  
    else if(nWTaggedJets_==2){// two w tags
      wjet1=wTaggedJets.at(0)->lv;
      wjet2=wTaggedJets.at(1)->lv;
      //bjet is highest pT remaining ak4 jet
      if(nNewCleanAK4Jets_>0) bjet=newCleanAK4Jets.at(0)->lv;
      else bjet.SetPxPyPzE(0,0,0,0);
      
    }//end exactly two w tags

    else if(nWTaggedJets_==1){//one wtag
      wjet1=wTaggedJets.at(0)->lv;
      //find second w and bjetEligibility conditions
      if(nNewCleanAK4Jets_==0) {wjet2.SetPxPyPzE(0,0,0,0);bjet.SetPxPyPzE(0,0,0,0);} //set to empty four vectors
      else if(nNewCleanAK4Jets_==1) {wjet2=newCleanAK4Jets.at(0)->lv;bjet.SetPxPyPzE(0,0,0,0);} //take only other existing jet - set bjet to empty four vector
      else if(nNewCleanAK4Jets_==2) {wjet2=newCleanAK4Jets.at(0)->lv; bjet=newCleanAK4Jets.at(1)->lv;} //take leading as wjet, subleading as b jet
      else{
	int ind1=0;int ind2=0;
	bool foundpair=false;
	float mdiff=9999999;
	for(int i=0; i<nNewCleanAK4Jets_;i++){
	  for(int j =i+1; j<nNewCleanAK4Jets_;j++){
	    float mtemp = (newCleanAK4Jets.at(i)->lv + newCleanAK4Jets.at(j)->lv).M();
	    if(fabs( mtemp - 80.385) <mdiff && fabs(mtemp-80.385)<20){ ind1=i;ind2=j; foundpair=true;}	    
	  }
	}
	if(foundpair) {wjet2 = newCleanAK4Jets.at(ind1)->lv + newCleanAK4Jets.at(ind2)->lv; usedJets.push_back(ind1); usedJets.push_back(ind2);}
	else wjet2.SetPxPyPzE(0,0,0,0);
	//set bjet to highest pt remaining jet
	for(int i=0; i<nNewCleanAK4Jets_; i++){
	  if( i!=ind1 && i!=ind2) { bjet=newCleanAK4Jets.at(i)->lv; break;}
	}
      }
    }//end one w tag
    else{//no w tags - require at least 5 jets otherwise it doesn't make sense
      if(nNewCleanAK4Jets_>4){
	int ind1=0; int ind2=0; int ind3=0;int ind4=0;
	bool foundpair1=false; bool foundpair2=false;
	float mdiff = 99999;
	//find w jet
	for(int i=0; i<nNewCleanAK4Jets_;i++){
	  for(int j =i+1; j<nNewCleanAK4Jets_;j++){
	    float mtemp = (newCleanAK4Jets.at(i)->lv + newCleanAK4Jets.at(j)->lv).M();
	    if(fabs( mtemp - 80.385) <mdiff && fabs(mtemp-80.385)<20){ ind1=i;ind2=j; foundpair1=true;}	    
	  }
	}
	mdiff=9999;
	for(int i=0; i<nNewCleanAK4Jets_;i++){
	  for(int j =i+1; j<nNewCleanAK4Jets_;j++){
	    float mtemp = (newCleanAK4Jets.at(i)->lv + newCleanAK4Jets.at(j)->lv).M();
	    if(! (ind1==i || ind1==j || ind2==i || ind2==j) && (fabs( mtemp - 80.385) <mdiff) && fabs(mtemp-80.385)<20){ ind3=i;ind4=j; foundpair2=true;}
	  }
	}
	if(foundpair1) {wjet1 = newCleanAK4Jets.at(ind1)->lv + newCleanAK4Jets.at(ind2)->lv; usedJets.push_back(ind1); usedJets.push_back(ind2);}
	else wjet1.SetPxPyPzE(0,0,0,0);
	if(foundpair2) {wjet2 = newCleanAK4Jets.at(ind3)->lv + newCleanAK4Jets.at(ind4)->lv; usedJets.push_back(ind3);usedJets.push_back(ind4);}
	else wjet2.SetPxPyPzE(0,0,0,0);
	//take highest pt remaining jet as b jet
	for(int i=0; i<nNewCleanAK4Jets_;i++){
	  if(i!=ind1 && i!=ind2 && i!=ind3 &&i!=ind4) bjet=newCleanAK4Jets.at(i)->lv;        
	}
      }//end if at least 5 jets
      else{//set lorentz vectors to 0
	wjet1.SetPxPyPzE(0,0,0,0);
	wjet2.SetPxPyPzE(0,0,0,0);
	bjet.SetPxPyPzE(0,0,0,0);
      }
    }//end no wTags

    /*    //now find bjet
    int bjet1ind=0;
    int bjet2ind=0;
    float md1=999;
    for(unsigned int i=0; i< newCleanAK4Jets.size(); i++){
      bool used=false;
      //check against all jets used and make sure the current jet doesn't match index of a used jet
      for(unsigned int j=0; j<usedJets.size();j++){
	if(i==j) used=true;
      }
      if(used) continue;
      //find best top mass
      float md1t = fabs( (wjet1+newCleanAK4Jets.at(i)->lv).M() - 173.3);
      if(md1t<md1) {md1=md1t; bjet1ind=i;}
    }
    //now find best top mass with second jet
    float md2=999;
    for(unsigned int i=0; i< newCleanAK4Jets.size(); i++){
      bool used=false;
      //check against all jets used and make sure the current jet doesn't match index of a used jet
      for(unsigned int j=0; j<usedJets.size();j++){
	if(i==j) used=true;
      }
      if(used) continue;
      //find best top mass
      float md2t = fabs( (wjet2+newCleanAK4Jets.at(i)->lv).M() - 173.3);
      if(md2t<md2) {md2=md2t; bjet2ind=i;}
    }
    //found best top reco for both Ws, now take one closest to top mass
    if(md1<=md2){//wjet1 has best combo
      //require to be within 30 GeV of top mass
      if(md1<30){
	bjet=newCleanAK4Jets.at(bjet1ind)->lv;
	foundTop=true;
	topjet = wjet1+bjet;
	//require other wjet not to be zeros
	if(wjet2.Pt() > 0){
	  foundW=true;
	  wjet = wjet2;
	}       
      }//end check on top mass
    }//end md1<md2
    else{//md2<md1
      //require to be within 30 GeV of top mass
      if(md2<30){
	bjet=newCleanAK4Jets.at(bjet2ind)->lv;
	foundTop=true;
	topjet = wjet2+bjet;
	//require other wjet not to be zeros
	if(wjet1.Pt() > 0){
	  foundW=true;
	  wjet = wjet1;
	}       
      }//end check on top mass
      }//end else*/


    //now set global variables for topjet and bjet
    float md1 = fabs( (wjet1+bjet).M() - 173.3);
    float md2 = fabs( (wjet2+bjet).M() - 173.3);
    if( md1 < 30){
      foundTop=true;
      topjet = wjet1+bjet;
      //require other wjet not to be zeros
      if(wjet2.Pt() > 0){
	foundW=true;
	wjet = wjet2;
      }
    }
    else if(md2 < 30){
      foundTop=true;
      topjet = wjet2+bjet;
      //require other wjet not to be zeros
      if(wjet1.Pt() > 0){
	foundW=true;
	wjet = wjet1;
      }
    }
    
  }// end no top tag

  if(foundW && foundTop) X53mass_ = (wjet+topjet).M();

  nConst_=nCleanAK4Jets_+vNonSSLep.size();
  DilepMass_ = DilepMasstemp;
  AssocMass_ = amasst;
  nMu_= nMu;


  //now get minDeltaR
  float minDR1=9999;
  float minDR2=9999;
  for(unsigned int i =0; i< cleanAK4Jets.size();i++){
    float dr1temp = pow( pow( Lep1Eta_ - cleanAK4Jets.at(i)->eta, 2) + pow(Lep1Phi_ - cleanAK4Jets.at(i)->phi,2), 0.5);
    float dr2temp = pow( pow( Lep2Eta_ - cleanAK4Jets.at(i)->eta, 2) + pow(Lep2Phi_ - cleanAK4Jets.at(i)->phi,2), 0.5);
    if(dr1temp<minDR1){ minDR1=dr1temp;}
    if(dr2temp<minDR2){ minDR2=dr2temp;}
  }

  Lep1MinDR_ = minDR1;
  Lep2MinDR_ = minDR2;

  tree->Fill();

}
