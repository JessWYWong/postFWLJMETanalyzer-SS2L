#ifndef TreeReader_h
#define TreeReader_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include "TLepton.h"
#include "TGenParticle.h"
#include "TElectron.h"
#include "TMuon.h"
#include "TBoostedJet.h"
#include "THadronicGenJet.h"
#include <stdio.h>
#include <vector>


class TreeReader{

 public:

  //constructor and desctructor
  TreeReader(TTree *tree,bool mc,bool latestVers);
  TreeReader(const TString &filename,const TString &treename,bool mc,bool latestVers);
  virtual ~TreeReader();

  //high level collections
  std::vector <TMuon*> allMuons, goodMuons,looseMuons;
  std::vector <TElectron*> allElectrons, goodElectrons, looseElectrons, cmsdasElectrons, loose50nsElectrons, good50nsElectrons;
  std::vector <TGenParticle*> genParticles;
  std::vector <TJet*> allAK4Jets,cleanedAK4Jets,simpleCleanedAK4Jets,newCleanedAK4Jets,cleanedAK4JetsScaleUp,cleanedAK4JetsScaleDown,cleanedAK4JetsSmearUp,cleanedAK4JetsSmearDown;
  std::vector <TJet*> genJets;
  std::vector <THadronicGenJet*> hadronicGenJets;
  std::vector<TBoostedJet*> allAK8Jets;

  //met
  double MET;
  TBranch* b_MET_DileptonCalc;
  double MET_phi;
  TBranch* b_MET_phi_DileptonCalc;

  TTree* tree;
  bool isMc;
  bool latestVersion;
  //function to get the data
  Int_t GetEntry(Long64_t entry);
  //initialize the tree
  virtual void Init(TTree* treetemp);

  /*collections to get from ljmet trees AND their corresponding branches*/

  //run,lumi,event
  int run;
  TBranch* b_run_CommonCalc;
  int lumi;
  TBranch* b_lumi_CommonCalc;
  long long event;
  TBranch* b_event_CommonCalc;
  double prefireWeight;
  TBranch* b_L1NonPrefiringProb_CommonCalc;
  double prefireWeightUp;
  TBranch* b_L1NonPrefiringProbUp_CommonCalc;
  double prefireWeightDown;
  TBranch* b_L1NonPrefiringProbDown_CommonCalc;

  //pileup
  int nPU;
  TBranch* b_nTrueInteractions_DileptonCalc;

  int nPrimaryVert;
  TBranch* b_nPV_DileptonCalc;

  //Electrons
  //kinematics
  std::vector<double>  *elPt;
  TBranch        *b_elPt_DileptonCalc;
  std::vector<double>  *elEta;
  TBranch        *b_elEta_DileptonCalc;
  std::vector<double>  *elPhi;
  TBranch        *b_elPhi_DileptonCalc;
  std::vector<double>  *elEnergy;
  TBranch        *b_elEnergy_DileptonCalc;

  std::vector<int>     *elChargeConsistent;
  TBranch        *b_elChargeConsistent_DileptonCalc;
  std::vector<int>     *elCharge;
  TBranch        *b_elCharge_DileptonCalc;
  std::vector<int>     *elGsfCharge;
  TBranch        *b_elGsfCharge_DileptonCalc;
  std::vector<int>     *elCtfCharge;
  TBranch        *b_elCtfCharge_DileptonCalc;
  std::vector<int>     *elScPixCharge;
  TBranch        *b_elScPixCharge_DileptonCalc;
  std::vector<int>     *elIsEBEE;
  TBranch        *b_elIsEBEE_DileptonCalc;
  std::vector<int>     *elMHits;
  TBranch        *b_elMHits_DileptonCalc;
  std::vector<int>     *elPassConversionVeto;
  TBranch        *b_elPassConversionVeto_DileptonCalc;
//  std::vector<int>     *elQuality;
//  TBranch        *b_elQuality_DileptonCalc;
  std::vector<int>     *elVtxFitConv;
  TBranch        *b_elVtxFitConv_DileptonCalc;
  //track quality variables
  std::vector<double>  *elD0;
  TBranch        *b_elD0_DileptonCalc;
  std::vector<double>  *elDZ;
  TBranch        *b_elDZ_DileptonCalc;
  std::vector<double>  *elSIP3d;
  TBranch        *b_elSIP3d_DileptonCalc;
  std::vector<double>  *elDeta;
  TBranch        *b_elDeta_DileptonCalc;
  std::vector<double>  *elDphi;
  TBranch        *b_elDphi_DileptonCalc;
  std::vector<double>  *elDxy;
  TBranch        *b_elDxy_DileptonCalc;
  //electron quality variables
  std::vector<double>  *elHoE;
  TBranch        *b_elHoE_DileptonCalc;
  std::vector<double>  *elOoemoop;
  TBranch        *b_elOoemoop_DileptonCalc;
  std::vector<double>  *elSihih;
  TBranch        *b_elSihih_DileptonCalc;
  //Isolation Variables
  std::vector<double>  *elchIso;
  TBranch        *b_elchIso_DileptonCalc;
  std::vector<double>  *elpuIso;
  TBranch        *b_elpuIso_DileptonCalc;
  std::vector<double>  *elneutIso;
  TBranch        *b_elneutIso_DileptonCalc;
  std::vector<double>  *elphotIso;
  TBranch        *b_elphotIso_DileptonCalc;
  std::vector<double>  *elrhoIso;
  TBranch        *b_elrhoIso_DileptonCalc;
  std::vector<double>  *elMiniIso;
  TBranch        *b_elMiniIso_DileptonCalc;
  std::vector<double>  *elMiniIsoDB;
  TBranch        *b_elMiniIsoDB_DileptonCalc;
  std::vector<double>  *elAEff;
  TBranch        *b_elAEff_DileptonCalc;
  std::vector<double>  *elSusyIso;
  TBranch        *b_elSusyIso_DileptonCalc;
  //el mva value
  std::vector<double> *elMVAValue;
  TBranch        *b_elMVAValue_DileptonCalc;

  //el mva ID bools
  std::vector<double> *elIsMVATight80;
  TBranch        *b_elIsMVATight80_DileptonCalc;
  std::vector<double> *elIsMVATight90;
  TBranch        *b_elIsMVATight90_DileptonCalc;
  std::vector<double> *elIsMVALoose;
  TBranch        *b_elIsMVALoose_DileptonCalc;
  std::vector<double> *elIsMVATightIso80;
  TBranch        *b_elIsMVATightIso80_DileptonCalc;
  std::vector<double> *elIsMVATightIso90;
  TBranch        *b_elIsMVATightIso90_DileptonCalc;
  std::vector<double> *elIsMVALooseIso;
  TBranch        *b_elIsMVALooseIso_DileptonCalc;

  //el cut ID bools
  std::vector<double> *elIsTight;
  TBranch        *b_elIsTight_DileptonCalc;
  std::vector<double> *elIsMedium;
  TBranch        *b_elIsMedium_DileptonCalc;
  std::vector<double> *elIsLoose;
  TBranch        *b_elIsLoose_DileptonCalc;
  std::vector<double> *elIsVeto;
  TBranch        *b_elIsVeto_DileptonCalc;


  //Muons
  std::vector<int>     *muCharge;
  TBranch        *b_muCharge_DileptonCalc;
  std::vector<bool>     *muGlobal;
  TBranch        *b_muGlobal_DileptonCalc;
  std::vector<bool>     *muPF;
  TBranch        *b_muPF_DileptonCalc;
  std::vector<bool>     *muTracker;
  TBranch        *b_muTracker_DileptonCalc;
  std::vector<int>     *muNMatchedStations;
  TBranch        *b_muNMatchedStations_DileptonCalc;
  std::vector<int>     *muNTrackerLayers;
  TBranch        *b_muNTrackerLayers_DileptonCalc;
  std::vector<int>     *muNValMuHits;
  TBranch        *b_muNValMuHits_DileptonCalc;
  std::vector<int>     *muNValPixelHits;
  TBranch        *b_muNValPixelHits_DileptonCalc;
  std::vector<double>  *muChi2;
  TBranch        *b_muChi2_DileptonCalc;
  std::vector<double>  *muDxy;
  TBranch        *b_muDxy_DileptonCalc;
  std::vector<double>  *muDz;
  TBranch        *b_muDz_DileptonCalc;
  std::vector<double>  *muSIP3d;
  TBranch        *b_muSIP3d_DileptonCalc;
  std::vector<double>  *muEnergy;
  TBranch        *b_muEnergy_DileptonCalc;
  std::vector<double>  *muEta;
  TBranch        *b_muEta_DileptonCalc;
  std::vector<double>  *muPhi;
  TBranch        *b_muPhi_DileptonCalc;
  std::vector<double>  *muPt;
  TBranch        *b_muPt_DileptonCalc;
  std::vector<double>  *muRelIso;
  TBranch        *b_muRelIso_DileptonCalc;
  std::vector<double>  *muMiniIso;
  TBranch        *b_muMiniIso_DileptonCalc;
  std::vector<double>  *muMiniIsoDB;
  TBranch        *b_muMiniIsoDB_DileptonCalc;
  std::vector<double>  *muSusyIso;
  TBranch        *b_muSusyIso_DileptonCalc;
  std::vector<int>     *muIsTight;
  TBranch        *b_muIsTight_DileptonCalc;
  std::vector<int>     *muIsLoose;
  TBranch        *b_muIsLoose_DileptonCalc;

  //Gen Particles
  std::vector<int>     *genId;
  TBranch        *b_genID_DileptonCalc;
  std::vector<int>     *genIndex;
  TBranch        *b_genIndex_DileptonCalc;
  std::vector<int>     *genMotherId;
  TBranch        *b_genMotherID_DileptonCalc;
  std::vector<int>     *genMotherIndex;
  TBranch        *b_genMotherIndex_DileptonCalc;
  std::vector<int>     *genStatus;
  TBranch        *b_genStatus_DileptonCalc;
  std::vector<double>  *genEnergy;
  TBranch        *b_genEnergy_DileptonCalc;
  std::vector<double>  *genEta;
  TBranch        *b_genEta_DileptonCalc;
  std::vector<double>  *genPhi;
  TBranch        *b_genPhi_DileptonCalc;
  std::vector<double>  *genPt;
  TBranch        *b_genPt_DileptonCalc;
  std::vector<bool> *genIsFromPromptTau;
  TBranch        *b_genIsFromPromptTau_DileptonCalc;
  std::vector<bool> *genIsPrompt;
  TBranch        *b_genIsPrompt_DileptonCalc;
  std::vector<bool> *genPMotherHasC;
  TBranch        *b_genPMotherHasC_DileptonCalc;
  std::vector<bool> *genPMotherHasB;
  TBranch        *b_genPMotherHasB_DileptonCalc;
  std::vector<int> *genPMother;
  TBranch        *b_genPMother_DileptonCalc;

  //std::vector<double> *genCharge;
  //TBranch        *b_genCharge_DileptonCalc;
  //ak4 jets
  std::vector<double>  *AK4JetEnergy;
  TBranch        *b_AK4JetEnergy_DileptonCalc;
  std::vector<double>  *AK4JetEta;
  TBranch        *b_AK4JetEta_DileptonCalc;
  std::vector<double>  *AK4JetPhi;
  TBranch        *b_AK4JetPhi_DileptonCalc;
  std::vector<double>  *AK4JetPt;
  TBranch        *b_AK4JetPt_DileptonCalc;

  //get cleaned jets
  std::vector<double>  *cleanedAK4JetEnergy;
  TBranch        *b_cleanedAK4JetEnergy_DileptonCalc;
  std::vector<double>  *cleanedAK4JetEta;
  TBranch        *b_cleanedAK4JetEta_DileptonCalc;
  std::vector<double>  *cleanedAK4JetPhi;
  TBranch        *b_cleanedAK4JetPhi_DileptonCalc;
  std::vector<double>  *cleanedAK4JetPt;
  TBranch        *b_cleanedAK4JetPt_DileptonCalc;
  std::vector<double>  *cleanedAK4JetPtScaleUp;
  TBranch        *b_cleanedAK4JetPtScaleUp_DileptonCalc;
  std::vector<double>  *cleanedAK4JetPtScaleDown;
  TBranch        *b_cleanedAK4JetPtScaleDown_DileptonCalc;
  std::vector<double>  *cleanedAK4JetPtSmearUp;
  TBranch        *b_cleanedAK4JetPtSmearUp_DileptonCalc;
  std::vector<double>  *cleanedAK4JetPtSmearDown;
  TBranch        *b_cleanedAK4JetPtSmearDown_DileptonCalc;

  //ak8 jets
  std::vector<double>  *AK8JetEnergy;
  TBranch        *b_AK8JetEnergy_JetSubCalc;
  std::vector<double>  *AK8JetEta;
  TBranch        *b_AK8JetEta_JetSubCalc;
  std::vector<double>  *AK8JetPhi;
  TBranch        *b_AK8JetPhi_JetSubCalc;
  std::vector<double>  *AK8JetPt;
  TBranch        *b_AK8JetPt_JetSubCalc;
  //std::vector<double>  *AK8JetTrimMass;
  //TBranch        *b_AK8JetTrimMass_JetSubCalc;
  // std::vector<double>  *AK8JetFiltMass;
  // TBranch        *b_AK8JetFiltMass_JetSubCalc;
  std::vector<double>  *AK8JetPruneMass;
  TBranch        *b_AK8JetPruneMass_JetSubCalc;
  std::vector<double>  *AK8JetSDMass;
  TBranch        *b_AK8JetSDMass_JetSubCalc;
  std::vector<double>  *AK8JetTau1;
  TBranch        *b_AK8JetTau1_JetSubCalc;
  std::vector<double>  *AK8JetTau2;
  TBranch        *b_AK8JetTau2_JetSubCalc;
  std::vector<double>  *AK8JetTau3;
  TBranch        *b_AK8JetTau3_JetSubCalc;
  std::vector<double>  *AK8JetNSubjets;
  TBranch        *b_AK8JetNSubjets_JetSubCalc;
  std::vector<int>  *AK8JetSubjetIndex;
  TBranch        *b_AK8JetSubjetIndex_JetSubCalc;

  //subjets
  std::vector<double>  *subJetEta;
  TBranch        *b_subJetEta_JetSubCalc;
  std::vector<double>  *subJetPhi;
  TBranch        *b_subJetPhi_JetSubCalc;
  std::vector<double>  *subJetPt;
  TBranch        *b_subJetPt_JetSubCalc;
  std::vector<double>  *subJetBDisc;
  TBranch        *b_subJetBDisc_JetSubCalc;
  std::vector<double>  *subJetDeltaR;
  TBranch        *b_subJetDeltaR_JetSubCalc;
  std::vector<int>  *subJetBTag;
  TBranch        *b_subJetBTag_JetSubCalc;
  std::vector<double>  *subJetMass;
  TBranch        *b_subJetMass_JetSubCalc;


  //gen jets
  std::vector<double>  *genJetEnergy;
  TBranch        *b_genJetEnergy_DileptonCalc;
  std::vector<double>  *genJetEta;
  TBranch        *b_genJetEta_DileptonCalc;
  std::vector<double>  *genJetPhi;
  TBranch        *b_genJetPhi_DileptonCalc;
  std::vector<double>  *genJetPt;
  TBranch        *b_genJetPt_DileptonCalc;


  //hadronic boosted jets
  std::vector<double> *HadronicVHtPt;
  std::vector<double> *HadronicVHtEta;
  std::vector<double> *HadronicVHtPhi;
  std::vector<double> *HadronicVHtEnergy;
  std::vector<int> *HadronicVHtID;
  std::vector<int> *HadronicVHtStatus;
  std::vector<double> *HadronicVHtD0Pt;
  std::vector<double> *HadronicVHtD0Eta;
  std::vector<double> *HadronicVHtD0Phi;
  std::vector<double> *HadronicVHtD0E;
  std::vector<double> *HadronicVHtD1Pt;
  std::vector<double> *HadronicVHtD1Eta;
  std::vector<double> *HadronicVHtD1Phi;
  std::vector<double> *HadronicVHtD1E;
  std::vector<double> *HadronicVHtD2Pt;
  std::vector<double> *HadronicVHtD2Eta;
  std::vector<double> *HadronicVHtD2Phi;
  std::vector<double> *HadronicVHtD2E;

  TBranch *b_HadronicVHtPt_JetSubCalc;
  TBranch *b_HadronicVHtEta_JetSubCalc;
  TBranch *b_HadronicVHtPhi_JetSubCalc;
  TBranch *b_HadronicVHtEnergy_JetSubCalc;
  TBranch *b_HadronicVHtID_JetSubCalc;
  TBranch *b_HadronicVHtStatus_JetSubCalc;
  TBranch *b_HadronicVHtD0Pt_JetSubCalc;
  TBranch *b_HadronicVHtD0Eta_JetSubCalc;
  TBranch *b_HadronicVHtD0Phi_JetSubCalc;
  TBranch *b_HadronicVHtD0E_JetSubCalc;
  TBranch *b_HadronicVHtD1Pt_JetSubCalc;
  TBranch *b_HadronicVHtD1Eta_JetSubCalc;
  TBranch *b_HadronicVHtD1Phi_JetSubCalc;
  TBranch *b_HadronicVHtD1E_JetSubCalc;
  TBranch *b_HadronicVHtD2Pt_JetSubCalc;
  TBranch *b_HadronicVHtD2Eta_JetSubCalc;
  TBranch *b_HadronicVHtD2Phi_JetSubCalc;
  TBranch *b_HadronicVHtD2E_JetSubCalc;

  //MC weight info
  double          MCWeight;
  TBranch*    b_MCWeight_DileptonCalc;

  //pdf weights
  std::vector<double> *LHEWeights;
  TBranch*            b_LHEWeights_DileptonCalc;
  std::vector<int>    *LHEWeightIDs;
  TBranch*            b_LHEWeightIDs_DileptonCalc;
  std::vector<double> *NewPDFweights;
  TBranch*            b_NewPDFweights_DileptonCalc;

  // Trigger Info

  //double electron
  bool HLT_Ele27_Ele37;
  bool HLT_DoubleEle25;
  bool HLT_DoubleEle27;
  bool HLT_DoubleEle33;

  bool HLT_DoubleEle24_eta2p1_WPTight_Gsf_v;
  bool HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_DZ_PFHT350_v;
  bool HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT350_v;
  bool HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v;
  bool HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v;

  TBranch* b_HLT_Ele27_Ele37_DileptonCalc;
  TBranch* b_HLT_DoubleEle25_DileptonCalc;
  TBranch* b_HLT_DoubleEle27_DileptonCalc;
  TBranch* b_HLT_DoubleEle33_DileptonCalc;

  TBranch* b_HLT_DoubleEle24_eta2p1_WPTight_Gsf_v_DileptonCalc;
  TBranch* b_HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_DZ_PFHT350_v_DileptonCalc;
  TBranch* b_HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT350_v_DileptonCalc;
  TBranch* b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v_DileptonCalc;
  TBranch* b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v_DileptonCalc;

  //single electron
  bool HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v;
  bool HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v;
  bool HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v;

  bool HLT_Ele20_WPLoose_Gsf_v;
  bool HLT_Ele20_eta2p1_WPLoose_Gsf_v;
  bool HLT_Ele27_WPTight_Gsf_v;
  bool HLT_Ele32_WPTight_Gsf_v;
  bool HLT_Ele35_WPTight_Gsf_v;
  bool HLT_Ele35_WPTight_Gsf_L1EGMT_v;
  bool HLT_Ele38_WPTight_Gsf_v;
  bool HLT_Ele40_WPTight_Gsf_v;
  bool HLT_Ele32_WPTight_Gsf_L1DoubleEG_v;
  bool HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned_v;
  bool HLT_Ele28_eta2p1_WPTight_Gsf_HT150_v;
  bool HLT_Ele28_HighEta_SC20_Mass55_v;

  bool HLT_Ele8_CaloIdM_TrackIdM_PFJet30_v;
  bool HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v;
  bool HLT_Ele23_CaloIdM_TrackIdM_PFJet30_v;

  TBranch* b_HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v_DileptonCalc;
  TBranch* b_HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v_DileptonCalc;
  TBranch* b_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v_DileptonCalc;

  TBranch* b_HLT_Ele20_WPLoose_Gsf_v_DileptonCalc;
  TBranch* b_HLT_Ele20_eta2p1_WPLoose_Gsf_v_DileptonCalc;
  TBranch* b_HLT_Ele27_WPTight_Gsf_v_DileptonCalc;
  TBranch* b_HLT_Ele32_WPTight_Gsf_v_DileptonCalc;
  TBranch* b_HLT_Ele35_WPTight_Gsf_v_DileptonCalc;
  TBranch* b_HLT_Ele35_WPTight_Gsf_L1EGMT_v_DileptonCalc;
  TBranch* b_HLT_Ele38_WPTight_Gsf_v_DileptonCalc;
  TBranch* b_HLT_Ele40_WPTight_Gsf_v_DileptonCalc;
  TBranch* b_HLT_Ele32_WPTight_Gsf_L1DoubleEG_v_DileptonCalc;
  TBranch* b_HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned_v_DileptonCalc;
  TBranch* b_HLT_Ele28_eta2p1_WPTight_Gsf_HT150_v_DileptonCalc;
  TBranch* b_HLT_Ele28_HighEta_SC20_Mass55_v_DileptonCalc;

  TBranch* b_HLT_Ele8_CaloIdM_TrackIdM_PFJet30_v_DileptonCalc;
  TBranch* b_HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v_DileptonCalc;
  TBranch* b_HLT_Ele23_CaloIdM_TrackIdM_PFJet30_v_DileptonCalc;

  //double muon
  bool HLT_Mu37_TkMu27;
  bool HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8;
  bool HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8;
  bool HLT_DoubleMu8_Mass8_PFHT350;
  bool HLT_DoubleMu4_Mass8_DZ_PFHT350;

  TBranch* b_HLT_Mu37_TkMu27_DileptonCalc;
  TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v_DileptonCalc;
  TBranch* b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v_DileptonCalc;
  TBranch* b_HLT_DoubleMu8_Mass8_PFHT350_DileptonCalc;
  TBranch* b_HLT_DoubleMu4_Mass8_DZ_PFHT350_DileptonCalc;


  //single muon
  bool HLT_Mu8;
  bool HLT_Mu17;
  bool HLT_Mu19;
  bool HLT_Mu20;
  bool HLT_Mu27;
  bool HLT_Mu8_TrkIsoVVL;
  bool HLT_Mu17_TrkIsoVVL;
  bool HLT_Mu19_TrkIsoVVL;
  bool HLT_Mu3_PFJet40;
  bool HLT_IsoMu27;
  bool HLT_IsoMu24;

  TBranch* b_HLT_Mu8_DileptonCalc;
  TBranch* b_HLT_Mu17_DileptonCalc;
  TBranch* b_HLT_Mu19_DileptonCalc;
  TBranch* b_HLT_Mu20_DileptonCalc;
  TBranch* b_HLT_Mu27_DileptonCalc;
  TBranch* b_HLT_Mu8_TrkIsoVVL_DileptonCalc;
  TBranch* b_HLT_Mu17_TrkIsoVVL_DileptonCalc;
  TBranch* b_HLT_Mu19_TrkIsoVVL_DileptonCalc;
  TBranch* b_HLT_Mu3_PFJet40_DileptonCalc;
  TBranch* b_HLT_IsoMu27_DileptonCalc;
  TBranch* b_HLT_IsoMu24_DileptonCalc;

  //cross triggers
  bool HLT_Mu37Ele27;
  bool HLT_Mu27Ele37;

  bool HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT350_DZ_v;
  bool HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v;
  bool HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT350_v;
  bool HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v;
  bool HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v;
  bool HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v;
  bool HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v;
  bool HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v;

  TBranch* b_HLT_Mu37Ele27_DileptonCalc;
  TBranch* b_HLT_Mu27Ele37_DileptonCalc;

  TBranch* b_HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT350_DZ_v_DileptonCalc;
  TBranch* b_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v_DileptonCalc;
  TBranch* b_HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT350_v_DileptonCalc;
  TBranch* b_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v_DileptonCalc;
  TBranch* b_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v_DileptonCalc;
  TBranch* b_HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v_DileptonCalc;
  TBranch* b_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v_DileptonCalc;
  TBranch* b_HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v_DileptonCalc;


  //hadronic triggers
//  bool HLT_PFHT900;
//  TBranch* b_HLT_PFHT900_DileptonCalc;
//  bool HLT_AK8PFJet360TrimMass30;
//  TBranch* b_HLT_AK8PFJet360TrimMass30_DileptonCalc;


  //TT signal BR info - added by rizi
  bool isTZTZ;
  TBranch* b_isTZTZ_TpTpCalc;
  bool isTZTH;
  TBranch* b_isTZTH_TpTpCalc;
  bool isTZBW;
  TBranch* b_isTZBW_TpTpCalc;
  bool isTHTH;
  TBranch* b_isTHTH_TpTpCalc;
  bool isTHBW;
  TBranch* b_isTHBW_TpTpCalc;
  bool isBWBW;
  TBranch* b_isBWBW_TpTpCalc;

  //BB signal BR info - added by rizki
  bool isBZBZ;
  TBranch* b_isBZBZ_TpTpCalc;
  bool isBZBH;
  TBranch* b_isBZBH_TpTpCalc;
  bool isBZTW;
  TBranch* b_isBZTW_TpTpCalc;
  bool isBHBH;
  TBranch* b_isBHBH_TpTpCalc;
  bool isBHTW;
  TBranch* b_isBHTW_TpTpCalc;
  bool isTWTW;
  TBranch* b_isTWTW_TpTpCalc;

};

#endif
