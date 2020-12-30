#ifndef TreeMaker_h
#define TreeMaker_h

#include "TTree.h"
#include "TreeReader.h"
#include "TLepton.h"
#include "TBoostedJet.h"
#include "THadronicGenJet.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

class TreeMaker {

 public:

  //constructor and destructor
  TreeMaker();
  virtual ~TreeMaker();

  //TTree
  TTree* tree;

  //event info
  int run_;
  int lumi_;
  long long event_;
  int nPrimaryVert_;
  //weight
  float weight_;
  int MCWeight_;
  //non prompt weight from tight/loose method;
  float NPWeight_;
  float NPAltWeight_;
  float NPSUSYWeight_;
  int nTL_;
  //scale factors for mc
  float trigSF_;
  float trigSFup_;
  float trigSFdn_;
  float IDSF_;
  float IDSFup_;
  float IDSFdn_;
  float IsoSF_;
  float gsfSF_;
  float JetTaggingSF_;

  float puweight_;
  float puweightUp_;
  float puweightDown_;

  std::vector<double> pdfweights_;
  std::vector<double> renormWeights_;
  std::vector<double> pdfNewWeights_;
  std::vector<double> pdfWeights4LHC_;
  std::vector<double> pdfWeightsMSTW_;
  double pdfNewNominalWeight_;

  //variables for tree
  float Lep1Pt_;
  float Lep1Eta_;
  float Lep1Phi_;
  float Lep1Energy_;
  int Lep1Flavor_;
  int Lep1Charge_;
  int Lep1Loose_;
  int Lep1Tight_;
  float Lep1MinDR_;
  float Lep1RelIso_;
  float Lep1MiniIso_;
  float Lep1SusyIso_;

  float Lep2Pt_;
  float Lep2Eta_;
  float Lep2Phi_;
  float Lep2Energy_;
  int Lep2Flavor_;
  int Lep2Charge_;
  int Lep2Loose_;
  int Lep2Tight_;
  float Lep2MinDR_;
  float Lep2RelIso_;
  float Lep2MiniIso_;
  float Lep2SusyIso_;

  int nConst_;
  int nAK4Jets_;
  int nNewConst_;
  int nNonSSLeps_;

  float AK4HT_;
  float MET_;
  float DilepMass_;
  float AssocMass_;
  float AK4Jet1Pt_;
  float AK4Jet1Eta_;
  float AK4Jet1Phi_;
  float AK4Jet1Energy_;

  float AK4Jet2Pt_;
  float AK4Jet2Eta_;
  float AK4Jet2Phi_;
  float AK4Jet2Energy_;

  //collections for cleaned jets
  float cleanAK4Jet1Pt_;
  float cleanAK4Jet1Eta_;
  float cleanAK4Jet1Phi_;
  float cleanAK4Jet1Energy_;

  float cleanAK4Jet2Pt_;
  float cleanAK4Jet2Eta_;
  float cleanAK4Jet2Phi_;
  float cleanAK4Jet2Energy_;

  int nCleanAK4Jets_;
  int nCleanAK4JetsScaleUp_;
  int nCleanAK4JetsScaleDown_;
  int nCleanAK4JetsSmearUp_;
  int nCleanAK4JetsSmearDown_;
  float cleanAK4HT_;
  float cleanAK4HTScaleUp_;
  float cleanAK4HTScaleDown_;
  float cleanAK4HTSmearUp_;
  float cleanAK4HTSmearDown_;

  //collections for simple cleaned jets
  float simpleCleanAK4Jet1Pt_;
  float simpleCleanAK4Jet1Eta_;
  float simpleCleanAK4Jet1Phi_;
  float simpleCleanAK4Jet1Energy_;

  float simpleCleanAK4Jet2Pt_;
  float simpleCleanAK4Jet2Eta_;
  float simpleCleanAK4Jet2Phi_;
  float simpleCleanAK4Jet2Energy_;

  int nSimpleCleanAK4Jets_;
  float simpleCleanAK4HT_;


  int nMu_; //the number of same-sign dileptons which are muons acts as standin for channel


  //ak8 jets
  float AK8Jet1Pt_;
  float AK8Jet1Eta_;
  float AK8Jet1Phi_;
  float AK8Jet1Energy_;
  float AK8Jet1TrimMass_;
  float AK8Jet1PruneMass_;
  float AK8Jet1SDMass_;
  float AK8Jet1FiltMass_;
  float AK8Jet1Tau1_;
  float AK8Jet1Tau2_;
  float AK8Jet1Tau3_;
  float AK8Jet1Tau21_;
  float AK8Jet1Tau31_;
  float AK8Jet1Tau32_;
  float AK8Jet1SubjetMaxBDisc_;

  float AK8Jet2Pt_;
  float AK8Jet2Eta_;
  float AK8Jet2Phi_;
  float AK8Jet2Energy_;
  float AK8Jet2TrimMass_;
  float AK8Jet2PruneMass_;
  float AK8Jet2SDMass_;
  float AK8Jet2FiltMass_;
  float AK8Jet2Tau1_;
  float AK8Jet2Tau2_;
  float AK8Jet2Tau3_;
  float AK8Jet2Tau21_;
  float AK8Jet2Tau31_;
  float AK8Jet2Tau32_;
  float AK8Jet2SubjetMaxBDisc_;


  //top tagged jets
  float TopJet1Pt_;
  float TopJet1Eta_;
  float TopJet1Phi_;
  float TopJet1Energy_;
  float TopJet1TrimMass_;
  float TopJet1PruneMass_;
  float TopJet1SDMass_;
  float TopJet1FiltMass_;
  float TopJet1Tau1_;
  float TopJet1Tau2_;
  float TopJet1Tau3_;
  float TopJet1Tau21_;
  float TopJet1Tau31_;
  float TopJet1Tau32_;
  float TopJet1SubjetMaxBDisc_;

  float TopJet2Pt_;
  float TopJet2Eta_;
  float TopJet2Phi_;
  float TopJet2Energy_;
  float TopJet2TrimMass_;
  float TopJet2PruneMass_;
  float TopJet2SDMass_;
  float TopJet2FiltMass_;
  float TopJet2Tau1_;
  float TopJet2Tau2_;
  float TopJet2Tau3_;
  float TopJet2Tau21_;
  float TopJet2Tau31_;
  float TopJet2Tau32_;
  float TopJet2SubjetMaxBDisc_;


  //w jets
  float WJet1Pt_;
  float WJet1Eta_;
  float WJet1Phi_;
  float WJet1Energy_;
  float WJet1TrimMass_;
  float WJet1PruneMass_;
  float WJet1SDMass_;
  float WJet1FiltMass_;
  float WJet1Tau1_;
  float WJet1Tau2_;
  float WJet1Tau3_;
  float WJet1Tau21_;
  float WJet1Tau31_;
  float WJet1Tau32_;
  float WJet1SubjetMaxBDisc_;

  float WJet2Pt_;
  float WJet2Eta_;
  float WJet2Phi_;
  float WJet2Energy_;
  float WJet2TrimMass_;
  float WJet2PruneMass_;
  float WJet2SDMass_;
  float WJet2FiltMass_;
  float WJet2Tau1_;
  float WJet2Tau2_;
  float WJet2Tau3_;
  float WJet2Tau21_;
  float WJet2Tau31_;
  float WJet2Tau32_;
  float WJet2SubjetMaxBDisc_;


  //collections for newCleaned jets
  float newCleanAK4Jet1Pt_;
  float newCleanAK4Jet1Eta_;
  float newCleanAK4Jet1Phi_;
  float newCleanAK4Jet1Energy_;

  float newCleanAK4Jet2Pt_;
  float newCleanAK4Jet2Eta_;
  float newCleanAK4Jet2Phi_;
  float newCleanAK4Jet2Energy_;

  int nNewCleanAK4Jets_;
  float newCleanAK4HT_;

  int nTopTaggedJets_;
  int nWTaggedJets_;
  float X53mass_;

  //functions
  //WriteHists(TreeReader* tr, TTree* t, std::vector<TLepton*> vSSLep);
  void InitTree(std::string treename);
  void FillTree(std::vector<TLepton*> vSSLep, std::vector<TJet*> AK4jets, std::vector<TJet*> cleanAK4jets, std::vector<TJet*> simpleCleanAK4Jets, float HTtemp, float METtemp, float DilepMasstemp, int nMu, float weight,std::vector<TLepton*> vNonSSLep,float mcweight, float NpWeighttemp, float NPaltTemp, float SUSYNPTemp, int nTLtemp,float trSF,float trSFup,float trSFdn, float idSF, float idSFup, float idSFdn, float isoSF,float gsfSF,float puwtemp,float puUptemp, float puDowntemp,float amasst,std::vector<TBoostedJet*> AK8Jets,std::vector<THadronicGenJet*> hadronicGenJets, bool mc,int run, int lumi, int event,int nPV, std::vector<double> pdfweightstemp, std::vector<double> renormWeightstemp, std::vector<double> pdfNewWeightstemp, std::vector<double> pdfWeights4LHCtemp, std::vector<double> pdfWeightsMSTWtemp, double pdfNewNominalWeighttemp);

};


#endif
