#include "TTree.h"
#include "TFile.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "plugins/Sample.cc"
#include "plugins/CutClass.cc"
#include "interface/TreeReader.h"
#include "interface/PUweights.h"
PUweights PUweightsMap;
#include "TGraphAsymmErrors.h"
#include "TMath.h"
#include "Math/ProbFunc.h"

//std::string area = "/uscms_data/d3/clint/using_git/T53/ljmet/2016/EGamma80XID/CMSSW_8_0_26_patch1/src/AnalysisCode/X53ThirteenTeVAnalysisCode/";
//std::string MCarea = "/uscms_data/d3/clint/using_git/T53/ljmet/2016/EGamma80XID/CMSSW_8_0_26_patch1/src/AnalysisCode/X53ThirteenTeVAnalysisCode/";
std::string area = "/eos/uscms/store/user/wywong/FWLJMET102X_2lep2018_wywong_082020_hadds_postFWLJMETanalyzer_IsoTrig_IDSFunc_HEMveto/";
std::string MCarea = "/eos/uscms/store/user/wywong/FWLJMET102X_2lep2018_wywong_082020_hadds_postFWLJMETanalyzer_IsoTrig_IDSFunc_HEMveto/";

std::vector<Variable*> getVariableVec(){

  std::vector<Variable*> vVar;

  Variable* lep1ptEl = new Variable("Lep1PtEl",6,0,600,"Leading Lepton (electron) p_{T} (GeV)","N_{Events} / 20 GeV");
  vVar.push_back(lep1ptEl);
  Variable* lep1ptMu = new Variable("Lep1PtMu",6,0,600,"Leading Lepton (muon) p_{T} (GeV)","N_{Events} / 20 GeV");
  vVar.push_back(lep1ptMu);

  Variable* lep1pt = new Variable("Lep1Pt",6,0,600,"Leading Lepton p_{T} (GeV)","N_{Events} / 20 GeV");
  vVar.push_back(lep1pt);
  Variable* lep1eta = new Variable("Lep1Eta",25,-5,5,"Leading Lepton #eta","N_{Events}");
  vVar.push_back(lep1eta);
  Variable* lep1phi = new Variable("Lep1Phi",20,-3.5,3.5,"Leading Lepton #phi","N_{Events}");
  vVar.push_back(lep1phi);
  Variable* lep1RelIso = new Variable("Lep1RelIso",40,0,1,"Leading Lepton Relative Isolation","N_{Events}");
  vVar.push_back(lep1RelIso);
  Variable* lep1MiniIso = new Variable("Lep1MiniIso",40,0,1,"Leading Lepton MiniIsolation","N_{Events}");
  vVar.push_back(lep1MiniIso);
  Variable* lep1SusyIso = new Variable("Lep1SusyIso",40,0,1,"Leading Lepton Susy Isolation","N_{Events}");
  vVar.push_back(lep1SusyIso);

  Variable* lep2pt = new Variable("Lep2Pt",6,0,600,"subLeading Lepton p_{T} (GeV)","N_{Events} / 20 GeV");
  vVar.push_back(lep2pt);
  Variable* lep2eta = new Variable("Lep2Eta",25,-5,5,"subLeading Lepton #eta","N_{Events}");
  vVar.push_back(lep2eta);
  Variable* lep2phi = new Variable("Lep2Phi",20,-3.5,3.5,"subLeading Lepton #phi","N_{Events}");
  vVar.push_back(lep2phi);
  Variable* lep2RelIso = new Variable("Lep2RelIso",40,0,1,"Leading Lepton Relative Isolation","N_{Events}");
  vVar.push_back(lep2RelIso);
  Variable* lep2MiniIso = new Variable("Lep2MiniIso",40,0,1,"Leading Lepton MiniIsolation","N_{Events}");
  vVar.push_back(lep2MiniIso);
  Variable* lep2SusyIso = new Variable("Lep2SusyIso",40,0,1,"Leading Lepton Susy Isolation","N_{Events}");
  vVar.push_back(lep2SusyIso);


  /*Variable* ak4jet1pt = new Variable("AK4Jet1Pt",15,0,600,"Leading Jet p_{T} (GeV)","N_{Events}");
  vVar.push_back(ak4jet1pt);
  Variable* ak4jet1eta = new Variable("AK4Jet1Eta",25,-5,5,"Leading Jet #eta","N_{Events}");
  vVar.push_back(ak4jet1eta);
  Variable* ak4jet1phi = new Variable("AK4Jet1Phi",20,-3.5,3.5,"Leading Jet #phi","N_{Events}");
  vVar.push_back(ak4jet1phi);

  Variable* ak4jet2pt = new Variable("AK4Jet2Pt",15,0,600,"subLeading Jet p_{T} (GeV)","N_{Events}");
  vVar.push_back(ak4jet2pt);
  Variable* ak4jet2eta = new Variable("AK4Jet2Eta",25,-5,5,"subLeading Jet #eta","N_{Events}");
  vVar.push_back(ak4jet2eta);
  Variable* ak4jet2phi = new Variable("AK4Jet2Phi",20,-3.5,3.5,"subLeading Jet #phi","N_{Events}");
  vVar.push_back(ak4jet2phi);


  Variable* ak4ht = new Variable("AK4HT",20,0,3000,"AK4 H_{T} (GeV)","N_{Events}");
  vVar.push_back(ak4ht);
  Variable* nak4jets = new Variable("nAK4Jets",17,0,17,"N_{AK4 Jets}","N_{Events}");
  vVar.push_back(nak4jets);*/

  //cleaned jet variables
  Variable* cleanak4jet1pt = new Variable("cleanAK4Jet1Pt",6,0,600,"Leading Jet p_{T} (GeV)","N_{Events} / 100 GeV");
  vVar.push_back(cleanak4jet1pt);
  Variable* cleanak4jet1eta = new Variable("cleanAK4Jet1Eta",25,-5,5,"Leading Jet #eta","N_{Events}");
  vVar.push_back(cleanak4jet1eta);
  Variable* cleanak4jet1phi = new Variable("cleanAK4Jet1Phi",20,-3.5,3.5,"Leading Jet #phi","N_{Events}");
  vVar.push_back(cleanak4jet1phi);

  Variable* cleanak4jet2pt = new Variable("cleanAK4Jet2Pt",6,0,600,"subLeading Jet p_{T} (GeV)","N_{Events} / 100 GeV");
  vVar.push_back(cleanak4jet2pt);
  Variable* cleanak4jet2eta = new Variable("cleanAK4Jet2Eta",25,-5,5,"subLeading Jet #eta","N_{Events}");
  vVar.push_back(cleanak4jet2eta);
  Variable* cleanak4jet2phi = new Variable("cleanAK4Jet2Phi",20,-3.5,3.5,"subLeading Jet #phi","N_{Events}");
  vVar.push_back(cleanak4jet2phi);

  Variable* cleanak4htEl = new Variable("cleanAK4HTEl",25,0,3000,"H_{T}^{lep} (leading electron) (GeV)","N_{Events} / 120 GeV");
  vVar.push_back(cleanak4htEl);
  Variable* cleanak4htMu = new Variable("cleanAK4HTMu",25,0,3000,"H_{T}^{lep}(leading muon) (GeV)","N_{Events} / 120 GeV");
  vVar.push_back(cleanak4htMu);
  Variable* cleanak4ht = new Variable("cleanAK4HT",25,0,3000,"H_{T}^{lep} (GeV)","N_{Events} / 120 GeV");
  vVar.push_back(cleanak4ht);
  Variable* ncleanak4jets = new Variable("nCleanAK4Jets",17,0,17,"N_{AK4 Jets}","N_{Events}");
  vVar.push_back(ncleanak4jets);


  //new ak4 jets
  Variable* newCleanak4jet1pt = new Variable("newCleanAK4Jet1Pt",6,0,600,"Leading Jet p_{T} (GeV)","N_{Events} / 100 GeV");
  vVar.push_back(newCleanak4jet1pt);
  Variable* newCleanak4jet1eta = new Variable("newCleanAK4Jet1Eta",25,-5,5,"Leading Jet #eta","N_{Events}");
  vVar.push_back(newCleanak4jet1eta);
  Variable* newCleanak4jet1phi = new Variable("newCleanAK4Jet1Phi",20,-3.5,3.5,"Leading Jet #phi","N_{Events}");
  vVar.push_back(newCleanak4jet1phi);

  Variable* newCleanak4jet2pt = new Variable("newCleanAK4Jet2Pt",6,0,600,"subLeading Jet p_{T} (GeV)","N_{Events} / 100 GeV");
  vVar.push_back(newCleanak4jet2pt);
  Variable* newCleanak4jet2eta = new Variable("newCleanAK4Jet2Eta",25,-5,5,"subLeading Jet #eta","N_{Events}");
  vVar.push_back(newCleanak4jet2eta);
  Variable* newCleanak4jet2phi = new Variable("newCleanAK4Jet2Phi",20,-3.5,3.5,"subLeading Jet #phi","N_{Events}");
  vVar.push_back(newCleanak4jet2phi);

  Variable* newCleanak4ht = new Variable("newCleanAK4HT",25,0,3000,"H_{T}^{lep} (GeV)","N_{Events} / 120 GeV");
  vVar.push_back(newCleanak4ht);
  Variable* nnewCleanak4jets = new Variable("nNewCleanAK4Jets",17,0,17,"N_{AK4 Jets}","N_{Events}");
  vVar.push_back(nnewCleanak4jets);


  //ak8jets
  Variable* ak8jet1pt = new Variable("AK8Jet1Pt",6,0,600,"Leading Jet p_{T} (GeV)","N_{Events} / 100 GeV");
  vVar.push_back(ak8jet1pt);
  Variable* ak8jet1eta = new Variable("AK8Jet1Eta",25,-5,5,"Leading Jet #eta","N_{Events}");
  vVar.push_back(ak8jet1eta);
  Variable* ak8jet1phi = new Variable("AK8Jet1Phi",20,-3.5,3.5,"Leading Jet #phi","N_{Events}");
  vVar.push_back(ak8jet1phi);

  Variable* ak8jet1tau1 = new Variable("AK8Jet1Tau1",50,0,1,"#tau_{1}","N_{Events}");
  vVar.push_back(ak8jet1tau1);
  Variable* ak8jet1tau2 = new Variable("AK8Jet1Tau2",50,0,1,"#tau_{2}","N_{Events}");
  vVar.push_back(ak8jet1tau2);
  Variable* ak8jet1tau3 = new Variable("AK8Jet1Tau3",50,0,1,"#tau_{3}","N_{Events}");
  vVar.push_back(ak8jet1tau3);
  Variable* ak8jet1tau21 = new Variable("AK8Jet1Tau21",50,0,1,"#tau_{21}","N_{Events}");
  vVar.push_back(ak8jet1tau21);
  Variable* ak8jet1tau31 = new Variable("AK8Jet1Tau31",50,0,1,"#tau_{31}","N_{Events}");
  vVar.push_back(ak8jet1tau31);
  Variable* ak8jet1tau32 = new Variable("AK8Jet1Tau32",50,0,1,"#tau_{32}","N_{Events}");
  vVar.push_back(ak8jet1tau32);


  Variable* ak8jet1bdisc = new Variable("AK8Jet1SubjetMaxBDisc",50,0,1,"Max Subjet CSV Score","N_{Events}");
  vVar.push_back(ak8jet1bdisc);

  Variable* ak8jet1trimmass = new Variable("AK8Jet1TrimMass",100,0,250,"Trimmed Mass","N_{Events}");
  vVar.push_back(ak8jet1trimmass);
  Variable* ak8jet1prunemass = new Variable("AK8Jet1PruneMass",100,0,250,"Pruned Mass","N_{Events}");
  vVar.push_back(ak8jet1prunemass);
  Variable* ak8jet1filtmass = new Variable("AK8Jet1FiltMass",100,0,250,"Filtered Mass","N_{Events}");
  vVar.push_back(ak8jet1filtmass);
  Variable* ak8jet1sdmass = new Variable("AK8Jet1SDMass",100,0,250,"SoftDrop Mass","N_{Events}");
  vVar.push_back(ak8jet1sdmass);


  Variable* ak8jet2pt = new Variable("AK8Jet2Pt",6,0,600,"subLeading Jet p_{T} (GeV)","N_{Events} / 100 GeV");
  vVar.push_back(ak8jet2pt);
  Variable* ak8jet2eta = new Variable("AK8Jet2Eta",25,-5,5,"subLeading Jet #eta","N_{Events}");
  vVar.push_back(ak8jet2eta);
  Variable* ak8jet2phi = new Variable("AK8Jet2Phi",20,-3.5,3.5,"subLeading Jet #phi","N_{Events}");
  vVar.push_back(ak8jet2phi);

  Variable* ak8jet2tau1 = new Variable("AK8Jet2Tau1",50,0,1,"#tau_{1}","N_{Events}");
  vVar.push_back(ak8jet2tau1);
  Variable* ak8jet2tau2 = new Variable("AK8Jet2Tau2",50,0,1,"#tau_{2}","N_{Events}");
  vVar.push_back(ak8jet2tau2);
  Variable* ak8jet2tau3 = new Variable("AK8Jet2Tau3",50,0,1,"#tau_{3}","N_{Events}");
  vVar.push_back(ak8jet2tau3);
  Variable* ak8jet2tau21 = new Variable("AK8Jet2Tau21",50,0,1,"#tau_{21}","N_{Events}");
  vVar.push_back(ak8jet2tau21);
  Variable* ak8jet2tau31 = new Variable("AK8Jet2Tau31",50,0,1,"#tau_{31}","N_{Events}");
  vVar.push_back(ak8jet2tau31);
  Variable* ak8jet2tau32 = new Variable("AK8Jet2Tau32",50,0,1,"#tau_{32}","N_{Events}");
  vVar.push_back(ak8jet2tau32);


  Variable* ak8jet2bdisc = new Variable("AK8Jet2SubjetMaxBDisc",50,0,1,"Max Subjet CSV Score","N_{Events}");
  vVar.push_back(ak8jet2bdisc);

  Variable* ak8jet2trimmass = new Variable("AK8Jet2TrimMass",100,0,250,"Trimmed Mass","N_{Events}");
  vVar.push_back(ak8jet2trimmass);
  Variable* ak8jet2prunemass = new Variable("AK8Jet2PruneMass",100,0,250,"Pruned Mass","N_{Events}");
  vVar.push_back(ak8jet2prunemass);
  Variable* ak8jet2filtmass = new Variable("AK8Jet2FiltMass",100,0,250,"Filtered Mass","N_{Events}");
  vVar.push_back(ak8jet2filtmass);
  Variable* ak8jet2sdmass = new Variable("AK8Jet2SDMass",100,0,250,"SoftDrop Mass","N_{Events}");
  vVar.push_back(ak8jet2sdmass);


  //Variable* nnonssleps = new Variable("nNonSSLeps",17,0,17,"N_{Non-SS Leps}","N_{Events}");
  //vVar.push_back(nnonssleps);

  //simple cleaned jet variables
  /*Variable* simpleCleanak4jet1pt = new Variable("simpleCleanAK4Jet1Pt",6,0,600,"Leading Jet p_{T} (GeV)","N_{Events}");
  vVar.push_back(simpleCleanak4jet1pt);
  Variable* simpleCleanak4jet1eta = new Variable("simpleCleanAK4Jet1Eta",25,-5,5,"Leading Jet #eta","N_{Events}");
  vVar.push_back(simpleCleanak4jet1eta);
  Variable* simpleCleanak4jet1phi = new Variable("simpleCleanAK4Jet1Phi",20,-3.5,3.5,"Leading Jet #phi","N_{Events}");
  vVar.push_back(simpleCleanak4jet1phi);

  Variable* simpleCleanak4jet2pt = new Variable("simpleCleanAK4Jet2Pt",6,0,600,"subLeading Jet p_{T} (GeV)","N_{Events}");
  vVar.push_back(simpleCleanak4jet2pt);
  Variable* simpleCleanak4jet2eta = new Variable("simpleCleanAK4Jet2Eta",25,-5,5,"subLeading Jet #eta","N_{Events}");
  vVar.push_back(simpleCleanak4jet2eta);
  Variable* simpleCleanak4jet2phi = new Variable("simpleCleanAK4Jet2Phi",20,-3.5,3.5,"subLeading Jet #phi","N_{Events}");
  vVar.push_back(simpleCleanak4jet2phi);


  Variable* simpleCleanak4ht = new Variable("simpleCleanAK4HT",20,0,3000,"simpleCleanAK4 H_{T} (GeV)","N_{Events}");
  vVar.push_back(simpleCleanak4ht);
  Variable* nsimpleCleanak4jets = new Variable("nSimpleCleanAK4Jets",17,0,17,"N_{simpleCleanAK4 Jets}","N_{Events}");
  vVar.push_back(nsimpleCleanak4jets);*/

  Variable* nConst = new Variable("nConst",17,0,17,"N_{Const}","N_{Events}");
  vVar.push_back(nConst);

  Variable* nNewConst = new Variable("nNewConst",17,0,17,"N_{Boosted Const}","N_{Events}");
  vVar.push_back(nNewConst);

  Variable* nConstDiff = new Variable("nNewConst - nConst",10,-5,5,"N_{Boosted Const} - N_{Const}","N_{Events}");
  vVar.push_back(nConstDiff);

  Variable* NTopTags = new Variable("NTopTags",5,0,5,"N_{Top Tagged Jets}","N_{Events}");
  vVar.push_back(NTopTags);

  Variable* NWTags = new Variable("NWTags",5,0,5,"N_{W Tagged Jets}","N_{Events}");
  vVar.push_back(NWTags);

  Variable* X53Mass = new Variable("X53Mass",50,100,2000,"M_{X53}","N_{Events}");
  vVar.push_back(X53Mass);

  Variable* met = new Variable("MET",60,0,600,"MET (GeV)", "N_{Events}");
  vVar.push_back(met);
  Variable* dilepMass = new Variable("DilepMass",20,0,600,"Dilepton Mass (GeV)", "N_{Events} / 30 GeV");
  vVar.push_back(dilepMass);
  return vVar;

}

std::vector<Variable*> getVariableVecForBoosted(){

  std::vector<Variable*> vVar;

  Variable* lep1pt = new Variable("Lep1Pt",6,0,600,"Leading Lepton p_{T} (GeV)","N_{Events} / 100 GeV");
  vVar.push_back(lep1pt);
  Variable* lep1eta = new Variable("Lep1Eta",25,-5,5,"Leading Lepton #eta","N_{Events}");
  vVar.push_back(lep1eta);
  Variable* lep1phi = new Variable("Lep1Phi",20,-3.5,3.5,"Leading Lepton #phi","N_{Events}");
  vVar.push_back(lep1phi);

  Variable* lep2pt = new Variable("Lep2Pt",6,0,600,"subLeading Lepton p_{T} (GeV)","N_{Events} / 100 GeV");
  vVar.push_back(lep2pt);
  Variable* lep2eta = new Variable("Lep2Eta",25,-5,5,"subLeading Lepton #eta","N_{Events}");
  vVar.push_back(lep2eta);
  Variable* lep2phi = new Variable("Lep2Phi",20,-3.5,3.5,"subLeading Lepton #phi","N_{Events}");
  vVar.push_back(lep2phi);
  /*
  //cleaned jet variables
  Variable* cleanak4jet1pt = new Variable("cleanAK4Jet1Pt",6,0,600,"Leading Jet p_{T} (GeV)","N_{Events} / 100 GeV");
  vVar.push_back(cleanak4jet1pt);
  Variable* cleanak4jet1eta = new Variable("cleanAK4Jet1Eta",25,-5,5,"Leading Jet #eta","N_{Events}");
  vVar.push_back(cleanak4jet1eta);
  Variable* cleanak4jet1phi = new Variable("cleanAK4Jet1Phi",20,-3.5,3.5,"Leading Jet #phi","N_{Events}");
  vVar.push_back(cleanak4jet1phi);

  Variable* cleanak4jet2pt = new Variable("cleanAK4Jet2Pt",6,0,600,"subLeading Jet p_{T} (GeV)","N_{Events} / 100 GeV");
  vVar.push_back(cleanak4jet2pt);
  Variable* cleanak4jet2eta = new Variable("cleanAK4Jet2Eta",25,-5,5,"subLeading Jet #eta","N_{Events}");
  vVar.push_back(cleanak4jet2eta);
  Variable* cleanak4jet2phi = new Variable("cleanAK4Jet2Phi",20,-3.5,3.5,"subLeading Jet #phi","N_{Events}");
  vVar.push_back(cleanak4jet2phi);

  Variable* cleanak4ht = new Variable("cleanAK4HT",25,0,3000,"H_{T}^{lep} (GeV)","N_{Events} / 120 GeV");
  vVar.push_back(cleanak4ht);
  Variable* ncleanak4jets = new Variable("nCleanAK4Jets",17,0,17,"N_{AK4 Jets}","N_{Events}");
  vVar.push_back(ncleanak4jets);
*/

  //new ak4 jets
  Variable* newCleanak4jet1pt = new Variable("newCleanAK4Jet1Pt",6,0,600,"Leading Jet p_{T} (GeV)","N_{Events} / 100 GeV");
  vVar.push_back(newCleanak4jet1pt);
  Variable* newCleanak4jet1eta = new Variable("newCleanAK4Jet1Eta",25,-5,5,"Leading Jet #eta","N_{Events}");
  vVar.push_back(newCleanak4jet1eta);
  Variable* newCleanak4jet1phi = new Variable("newCleanAK4Jet1Phi",20,-3.5,3.5,"Leading Jet #phi","N_{Events}");
  vVar.push_back(newCleanak4jet1phi);

  Variable* newCleanak4jet2pt = new Variable("newCleanAK4Jet2Pt",6,0,600,"subLeading Jet p_{T} (GeV)","N_{Events} / 100 GeV");
  vVar.push_back(newCleanak4jet2pt);
  Variable* newCleanak4jet2eta = new Variable("newCleanAK4Jet2Eta",25,-5,5,"subLeading Jet #eta","N_{Events}");
  vVar.push_back(newCleanak4jet2eta);
  Variable* newCleanak4jet2phi = new Variable("newCleanAK4Jet2Phi",20,-3.5,3.5,"subLeading Jet #phi","N_{Events}");
  vVar.push_back(newCleanak4jet2phi);

  Variable* newCleanak4ht = new Variable("newCleanAK4HT",25,0,3000,"H_{T}^{lep} (GeV)","N_{Events} / 120 GeV");
  vVar.push_back(newCleanak4ht);
  Variable* nnewCleanak4jets = new Variable("nNewCleanAK4Jets",17,0,17,"N_{AK4 Jets}","N_{Events}");
  vVar.push_back(nnewCleanak4jets);


  //ak8jets
  Variable* ak8jet1pt = new Variable("AK8Jet1Pt",6,0,600,"Leading Jet p_{T} (GeV)","N_{Events} / 100 GeV");
  vVar.push_back(ak8jet1pt);
  Variable* ak8jet1eta = new Variable("AK8Jet1Eta",25,-5,5,"Leading Jet #eta","N_{Events}");
  vVar.push_back(ak8jet1eta);
  Variable* ak8jet1phi = new Variable("AK8Jet1Phi",20,-3.5,3.5,"Leading Jet #phi","N_{Events}");
  vVar.push_back(ak8jet1phi);

  Variable* ak8jet1tau1 = new Variable("AK8Jet1Tau1",50,0,1,"#tau_{1}","N_{Events}");
  vVar.push_back(ak8jet1tau1);
  Variable* ak8jet1tau2 = new Variable("AK8Jet1Tau2",50,0,1,"#tau_{2}","N_{Events}");
  vVar.push_back(ak8jet1tau2);
  Variable* ak8jet1tau3 = new Variable("AK8Jet1Tau3",50,0,1,"#tau_{3}","N_{Events}");
  vVar.push_back(ak8jet1tau3);
  Variable* ak8jet1tau21 = new Variable("AK8Jet1Tau21",50,0,1,"#tau_{21}","N_{Events}");
  vVar.push_back(ak8jet1tau21);
  Variable* ak8jet1tau31 = new Variable("AK8Jet1Tau31",50,0,1,"#tau_{31}","N_{Events}");
  vVar.push_back(ak8jet1tau31);
  Variable* ak8jet1tau32 = new Variable("AK8Jet1Tau32",50,0,1,"#tau_{32}","N_{Events}");
  vVar.push_back(ak8jet1tau32);


  Variable* ak8jet1bdisc = new Variable("AK8Jet1SubjetMaxBDisc",50,0,1,"Max Subjet CSV Score","N_{Events}");
  vVar.push_back(ak8jet1bdisc);

  Variable* ak8jet1trimmass = new Variable("AK8Jet1TrimMass",100,0,250,"Trimmed Mass","N_{Events}");
  vVar.push_back(ak8jet1trimmass);
  Variable* ak8jet1prunemass = new Variable("AK8Jet1PruneMass",100,0,250,"Pruned Mass","N_{Events}");
  vVar.push_back(ak8jet1prunemass);
  Variable* ak8jet1filtmass = new Variable("AK8Jet1FiltMass",100,0,250,"Filtered Mass","N_{Events}");
  vVar.push_back(ak8jet1filtmass);
  Variable* ak8jet1sdmass = new Variable("AK8Jet1SDMass",100,0,250,"SoftDrop Mass","N_{Events}");
  vVar.push_back(ak8jet1sdmass);


  Variable* ak8jet2pt = new Variable("AK8Jet2Pt",6,0,600,"subLeading Jet p_{T} (GeV)","N_{Events} / 100 GeV");
  vVar.push_back(ak8jet2pt);
  Variable* ak8jet2eta = new Variable("AK8Jet2Eta",25,-5,5,"subLeading Jet #eta","N_{Events}");
  vVar.push_back(ak8jet2eta);
  Variable* ak8jet2phi = new Variable("AK8Jet2Phi",20,-3.5,3.5,"subLeading Jet #phi","N_{Events}");
  vVar.push_back(ak8jet2phi);

  Variable* ak8jet2tau1 = new Variable("AK8Jet2Tau1",50,0,1,"#tau_{1}","N_{Events}");
  vVar.push_back(ak8jet2tau1);
  Variable* ak8jet2tau2 = new Variable("AK8Jet2Tau2",50,0,1,"#tau_{2}","N_{Events}");
  vVar.push_back(ak8jet2tau2);
  Variable* ak8jet2tau3 = new Variable("AK8Jet2Tau3",50,0,1,"#tau_{3}","N_{Events}");
  vVar.push_back(ak8jet2tau3);
  Variable* ak8jet2tau21 = new Variable("AK8Jet2Tau21",50,0,1,"#tau_{21}","N_{Events}");
  vVar.push_back(ak8jet2tau21);
  Variable* ak8jet2tau31 = new Variable("AK8Jet2Tau31",50,0,1,"#tau_{31}","N_{Events}");
  vVar.push_back(ak8jet2tau31);
  Variable* ak8jet2tau32 = new Variable("AK8Jet2Tau32",50,0,1,"#tau_{32}","N_{Events}");
  vVar.push_back(ak8jet2tau32);


  Variable* ak8jet2bdisc = new Variable("AK8Jet2SubjetMaxBDisc",50,0,1,"Max Subjet CSV Score","N_{Events}");
  vVar.push_back(ak8jet2bdisc);

  Variable* ak8jet2trimmass = new Variable("AK8Jet2TrimMass",100,0,250,"Trimmed Mass","N_{Events}");
  vVar.push_back(ak8jet2trimmass);
  Variable* ak8jet2prunemass = new Variable("AK8Jet2PruneMass",100,0,250,"Pruned Mass","N_{Events}");
  vVar.push_back(ak8jet2prunemass);
  Variable* ak8jet2filtmass = new Variable("AK8Jet2FiltMass",100,0,250,"Filtered Mass","N_{Events}");
  vVar.push_back(ak8jet2filtmass);
  Variable* ak8jet2sdmass = new Variable("AK8Jet2SDMass",100,0,250,"SoftDrop Mass","N_{Events}");
  vVar.push_back(ak8jet2sdmass);


  //topJetjets
  Variable* topjet1pt = new Variable("TopJet1Pt",6,0,600,"Leading Jet p_{T} (GeV)","N_{Events} / 100 GeV");
  vVar.push_back(topjet1pt);
  Variable* topjet1eta = new Variable("TopJet1Eta",25,-5,5,"Leading Jet #eta","N_{Events}");
  vVar.push_back(topjet1eta);
  Variable* topjet1phi = new Variable("TopJet1Phi",20,-3.5,3.5,"Leading Jet #phi","N_{Events}");
  vVar.push_back(topjet1phi);

  Variable* topjet1tau1 = new Variable("TopJet1Tau1",50,0,1,"#tau_{1}","N_{Events}");
  vVar.push_back(topjet1tau1);
  Variable* topjet1tau2 = new Variable("TopJet1Tau2",50,0,1,"#tau_{2}","N_{Events}");
  vVar.push_back(topjet1tau2);
  Variable* topjet1tau3 = new Variable("TopJet1Tau3",50,0,1,"#tau_{3}","N_{Events}");
  vVar.push_back(topjet1tau3);
  Variable* topjet1tau21 = new Variable("TopJet1Tau21",50,0,1,"#tau_{21}","N_{Events}");
  vVar.push_back(topjet1tau21);
  Variable* topjet1tau31 = new Variable("TopJet1Tau31",50,0,1,"#tau_{31}","N_{Events}");
  vVar.push_back(topjet1tau31);
  Variable* topjet1tau32 = new Variable("TopJet1Tau32",50,0,1,"#tau_{32}","N_{Events}");
  vVar.push_back(topjet1tau32);


  Variable* topjet1bdisc = new Variable("TopJet1SubjetMaxBDisc",50,0,1,"Max Subjet CSV Score","N_{Events}");
  vVar.push_back(topjet1bdisc);

  Variable* topjet1trimmass = new Variable("TopJet1TrimMass",100,0,250,"Trimmed Mass","N_{Events}");
  vVar.push_back(topjet1trimmass);
  Variable* topjet1prunemass = new Variable("TopJet1PruneMass",100,0,250,"Pruned Mass","N_{Events}");
  vVar.push_back(topjet1prunemass);
  Variable* topjet1filtmass = new Variable("TopJet1FiltMass",100,0,250,"Filtered Mass","N_{Events}");
  vVar.push_back(topjet1filtmass);
  Variable* topjet1sdmass = new Variable("TopJet1SDMass",100,0,250,"SoftDrop Mass","N_{Events}");
  vVar.push_back(topjet1sdmass);


  Variable* topjet2pt = new Variable("TopJet2Pt",6,0,600,"subLeading Jet p_{T} (GeV)","N_{Events} / 100 GeV");
  vVar.push_back(topjet2pt);
  Variable* topjet2eta = new Variable("TopJet2Eta",25,-5,5,"subLeading Jet #eta","N_{Events}");
  vVar.push_back(topjet2eta);
  Variable* topjet2phi = new Variable("TopJet2Phi",20,-3.5,3.5,"subLeading Jet #phi","N_{Events}");
  vVar.push_back(topjet2phi);

  Variable* topjet2tau1 = new Variable("TopJet2Tau1",50,0,1,"#tau_{1}","N_{Events}");
  vVar.push_back(topjet2tau1);
  Variable* topjet2tau2 = new Variable("TopJet2Tau2",50,0,1,"#tau_{2}","N_{Events}");
  vVar.push_back(topjet2tau2);
  Variable* topjet2tau3 = new Variable("TopJet2Tau3",50,0,1,"#tau_{3}","N_{Events}");
  vVar.push_back(topjet2tau3);
  Variable* topjet2tau21 = new Variable("TopJet2Tau21",50,0,1,"#tau_{21}","N_{Events}");
  vVar.push_back(topjet2tau21);
  Variable* topjet2tau31 = new Variable("TopJet2Tau31",50,0,1,"#tau_{31}","N_{Events}");
  vVar.push_back(topjet2tau31);
  Variable* topjet2tau32 = new Variable("TopJet2Tau32",50,0,1,"#tau_{32}","N_{Events}");
  vVar.push_back(topjet2tau32);


  Variable* topjet2bdisc = new Variable("TopJet2SubjetMaxBDisc",50,0,1,"Max Subjet CSV Score","N_{Events}");
  vVar.push_back(topjet2bdisc);

  Variable* topjet2trimmass = new Variable("TopJet2TrimMass",100,0,250,"Trimmed Mass","N_{Events}");
  vVar.push_back(topjet2trimmass);
  Variable* topjet2prunemass = new Variable("TopJet2PruneMass",100,0,250,"Pruned Mass","N_{Events}");
  vVar.push_back(topjet2prunemass);
  Variable* topjet2filtmass = new Variable("TopJet2FiltMass",100,0,250,"Filtered Mass","N_{Events}");
  vVar.push_back(topjet2filtmass);
  Variable* topjet2sdmass = new Variable("TopJet2SDMass",100,0,250,"SoftDrop Mass","N_{Events}");
  vVar.push_back(topjet2sdmass);

  //wjets
  Variable* wjet1pt = new Variable("WJet1Pt",6,0,600,"Leading Jet p_{T} (GeV)","N_{Events} / 100 GeV");
  vVar.push_back(wjet1pt);
  Variable* wjet1eta = new Variable("WJet1Eta",25,-5,5,"Leading Jet #eta","N_{Events}");
  vVar.push_back(wjet1eta);
  Variable* wjet1phi = new Variable("WJet1Phi",20,-3.5,3.5,"Leading Jet #phi","N_{Events}");
  vVar.push_back(wjet1phi);

  Variable* wjet1tau1 = new Variable("WJet1Tau1",50,0,1,"#tau_{1}","N_{Events}");
  vVar.push_back(wjet1tau1);
  Variable* wjet1tau2 = new Variable("WJet1Tau2",50,0,1,"#tau_{2}","N_{Events}");
  vVar.push_back(wjet1tau2);
  Variable* wjet1tau3 = new Variable("WJet1Tau3",50,0,1,"#tau_{3}","N_{Events}");
  vVar.push_back(wjet1tau3);
  Variable* wjet1tau21 = new Variable("WJet1Tau21",50,0,1,"#tau_{21}","N_{Events}");
  vVar.push_back(wjet1tau21);
  Variable* wjet1tau31 = new Variable("WJet1Tau31",50,0,1,"#tau_{31}","N_{Events}");
  vVar.push_back(wjet1tau31);
  Variable* wjet1tau32 = new Variable("WJet1Tau32",50,0,1,"#tau_{32}","N_{Events}");
  vVar.push_back(wjet1tau32);


  Variable* wjet1bdisc = new Variable("WJet1SubjetMaxBDisc",50,0,1,"Max Subjet CSV Score","N_{Events}");
  vVar.push_back(wjet1bdisc);

  Variable* wjet1trimmass = new Variable("WJet1TrimMass",100,0,250,"Trimmed Mass","N_{Events}");
  vVar.push_back(wjet1trimmass);
  Variable* wjet1prunemass = new Variable("WJet1PruneMass",100,0,250,"Pruned Mass","N_{Events}");
  vVar.push_back(wjet1prunemass);
  Variable* wjet1filtmass = new Variable("WJet1FiltMass",100,0,250,"Filtered Mass","N_{Events}");
  vVar.push_back(wjet1filtmass);
  Variable* wjet1sdmass = new Variable("WJet1SDMass",100,0,250,"SoftDrop Mass","N_{Events}");
  vVar.push_back(wjet1sdmass);


  Variable* wjet2pt = new Variable("WJet2Pt",6,0,600,"subLeading Jet p_{T} (GeV)","N_{Events} / 100 GeV");
  vVar.push_back(wjet2pt);
  Variable* wjet2eta = new Variable("WJet2Eta",25,-5,5,"subLeading Jet #eta","N_{Events}");
  vVar.push_back(wjet2eta);
  Variable* wjet2phi = new Variable("WJet2Phi",20,-3.5,3.5,"subLeading Jet #phi","N_{Events}");
  vVar.push_back(wjet2phi);

  Variable* wjet2tau1 = new Variable("WJet2Tau1",50,0,1,"#tau_{1}","N_{Events}");
  vVar.push_back(wjet2tau1);
  Variable* wjet2tau2 = new Variable("WJet2Tau2",50,0,1,"#tau_{2}","N_{Events}");
  vVar.push_back(wjet2tau2);
  Variable* wjet2tau3 = new Variable("WJet2Tau3",50,0,1,"#tau_{3}","N_{Events}");
  vVar.push_back(wjet2tau3);
  Variable* wjet2tau21 = new Variable("WJet2Tau21",50,0,1,"#tau_{21}","N_{Events}");
  vVar.push_back(wjet2tau21);
  Variable* wjet2tau31 = new Variable("WJet2Tau31",50,0,1,"#tau_{31}","N_{Events}");
  vVar.push_back(wjet2tau31);
  Variable* wjet2tau32 = new Variable("WJet2Tau32",50,0,1,"#tau_{32}","N_{Events}");
  vVar.push_back(wjet2tau32);


  Variable* wjet2bdisc = new Variable("WJet2SubjetMaxBDisc",50,0,1,"Max Subjet CSV Score","N_{Events}");
  vVar.push_back(wjet2bdisc);

  Variable* wjet2trimmass = new Variable("WJet2TrimMass",100,0,250,"Trimmed Mass","N_{Events}");
  vVar.push_back(wjet2trimmass);
  Variable* wjet2prunemass = new Variable("WJet2PruneMass",100,0,250,"Pruned Mass","N_{Events}");
  vVar.push_back(wjet2prunemass);
  Variable* wjet2filtmass = new Variable("WJet2FiltMass",100,0,250,"Filtered Mass","N_{Events}");
  vVar.push_back(wjet2filtmass);
  Variable* wjet2sdmass = new Variable("WJet2SDMass",100,0,250,"SoftDrop Mass","N_{Events}");
  vVar.push_back(wjet2sdmass);
  Variable* nConst = new Variable("nConst",17,0,17,"N_{Const}","N_{Events}");
  vVar.push_back(nConst);

  Variable* nNewConst = new Variable("nNewConst",17,0,17,"N_{Boosted Const}","N_{Events}");
  vVar.push_back(nNewConst);

  Variable* nConstDiff = new Variable("nNewConst - nConst",10,-5,5,"N_{Boosted Const} - N_{Const}","N_{Events}");
  vVar.push_back(nConstDiff);

  Variable* HTDiff = new Variable("newCleanAK4HT - cleanAK4HT",100,-200,800,"H^{l}_{T}(Boosted) - H^{l}_{T}","N_{Events}");
  vVar.push_back(HTDiff);

  Variable* NTopTags = new Variable("NTopTags",5,0,5,"N_{Top Tagged Jets}","N_{Events}");
  vVar.push_back(NTopTags);

  Variable* NWTags = new Variable("NWTags",5,0,5,"N_{W Tagged Jets}","N_{Events}");
  vVar.push_back(NWTags);

  Variable* X53Mass = new Variable("X53Mass",50,100,2000,"M_{X53}","N_{Events}");
  vVar.push_back(X53Mass);

  Variable* met = new Variable("MET",60,0,600,"MET (GeV)", "N_{Events}");
  vVar.push_back(met);
  Variable* dilepMass = new Variable("DilepMass",20,0,600,"Dilepton Mass (GeV)", "N_{Events} / 30 GeV");
  vVar.push_back(dilepMass);
  return vVar;

}

std::vector<Variable*> getVariablesForClosureTest(){

  std::vector<Variable*> vVar;

  Variable* lep1pt = new Variable("Lep1Pt",6,0,600,"Leading Lepton p_{T} (GeV)","N_{Events}");
  vVar.push_back(lep1pt);
  /*Variable* lep1eta = new Variable("Lep1Eta",25,-5,5,"Leading Lepton #eta","N_{Events}");
  vVar.push_back(lep1eta);
  Variable* lep1phi = new Variable("Lep1Phi",20,-3.5,3.5,"Leading Lepton #phi","N_{Events}");
  vVar.push_back(lep1phi);*/

  Variable* lep2pt = new Variable("Lep2Pt",6,0,600,"subLeading Lepton p_{T} (GeV)","N_{Events}");
  vVar.push_back(lep2pt);
  /*Variable* lep2eta = new Variable("Lep2Eta",25,-5,5,"subLeading Lepton #eta","N_{Events}");
  vVar.push_back(lep2eta);
  Variable* lep2phi = new Variable("Lep2Phi",20,-3.5,3.5,"subLeading Lepton #phi","N_{Events}");
  vVar.push_back(lep2phi);*/

  //cleaned jet variables
  Variable* cleanak4jet1pt = new Variable("cleanAK4Jet1Pt",6,0,600,"Leading Jet p_{T} (GeV)","N_{Events}");
  vVar.push_back(cleanak4jet1pt);
  /*Variable* cleanak4jet1eta = new Variable("cleanAK4Jet1Eta",25,-5,5,"Leading Jet #eta","N_{Events}");
  vVar.push_back(cleanak4jet1eta);
  Variable* cleanak4jet1phi = new Variable("cleanAK4Jet1Phi",20,-3.5,3.5,"Leading Jet #phi","N_{Events}");
  vVar.push_back(cleanak4jet1phi);*/

  Variable* cleanak4jet2pt = new Variable("cleanAK4Jet2Pt",6,0,600,"subLeading Jet p_{T} (GeV)","N_{Events}");
  vVar.push_back(cleanak4jet2pt);
  /*Variable* cleanak4jet2eta = new Variable("cleanAK4Jet2Eta",25,-5,5,"subLeading Jet #eta","N_{Events}");
  vVar.push_back(cleanak4jet2eta);
  Variable* cleanak4jet2phi = new Variable("cleanAK4Jet2Phi",20,-3.5,3.5,"subLeading Jet #phi","N_{Events}");
  vVar.push_back(cleanak4jet2phi);*/


  Variable* cleanak4ht = new Variable("cleanAK4HT",20,0,3000,"H_{T} (GeV)","N_{Events}");
  vVar.push_back(cleanak4ht);
  Variable* ncleanak4jets = new Variable("nCleanAK4Jets",17,0,17,"N_{AK4 Jets}","N_{Events}");
  vVar.push_back(ncleanak4jets);

  //Variable* nConst = new Variable("nConst",17,0,17,"N_{Const}","N_{Events}");
  //vVar.push_back(nConst);

  //Variable* met = new Variable("MET",60,0,600,"MET (GeV)", "N_{Events}");
  //vVar.push_back(met);
  Variable* dilepMass = new Variable("DilepMass",60,0,600,"Dilepton Mass (GeV)", "N_{Events}");
  vVar.push_back(dilepMass);
  return vVar;

}


std::vector<TH1F*> getNPHistos(int nMu){

  std::vector<TH1F*> hists;
  std::string channel;
  if(nMu==-1) channel="All";
  if(nMu==0) channel="ElEl";
  if(nMu==1) channel="ElMu";
  if(nMu==2) channel="MuMu";

  std::string nt00 = channel+"_HT_nConstCut_NT00";
  std::string nt01 = channel+"_HT_nConstCut_NT01";
  std::string nt10 = channel+"_HT_nConstCut_NT10";
  std::string nt11 = channel+"_HT_nConstCut_NT11";

  //10 gev bins sot hat later I can use bin number ask cut
  hists.push_back(new TH1F(nt00.c_str(),nt00.c_str(),1000,0,10000) );
  hists.push_back(new TH1F(nt01.c_str(),nt01.c_str(),1000,0,10000) );
  hists.push_back(new TH1F(nt10.c_str(),nt10.c_str(),1000,0,10000) );
  hists.push_back(new TH1F(nt11.c_str(),nt11.c_str(),1000,0,10000) );

  return hists;
}


Sample* getDataSample(std::string cut,std::string elID, std::string muID){

  std::string filename = area+"test/Data_Mu"+muID+"_El"+elID+".root";
  TFile* tfile = new TFile(filename.c_str());
  Sample* dataSample = new Sample("Data",tfile,1,1,cut,kBlack,1);
  return dataSample;
}


std::vector<Sample*> getSamplesForClosureTest(std::string cut, float lumi, std::string elID, std::string muID){  
  std::vector<std::string> vBkgNames;  std::vector<float> vXsec;  std::vector<float> vNEvts;
  vBkgNames.push_back("TTJets");  vXsec.push_back(831.76);  vNEvts.push_back(42730273 * 0.331582);
  vBkgNames.push_back("NonPromptTTJets");  vXsec.push_back(831.76);  vNEvts.push_back(42730273 * 0.331582);
  vBkgNames.push_back("TTbar");  vXsec.push_back(831.76);  vNEvts.push_back(115091972);
  vBkgNames.push_back("NonPromptTTbar");  vXsec.push_back(831.76);  vNEvts.push_back(115091972);

  //now make vector to hold weights;
  std::vector<float> vWeights;
  for(std::vector<float>::size_type ui=0; ui<vXsec.size(); ui++){
    vWeights.push_back( lumi * 1000 * ( vXsec.at(ui) / vNEvts.at(ui) ) ); //factor of 1000 to convert lumi to pb^-1
  }


  std::vector<Sample*> vSample;
  std::string tt = MCarea+"test/TTJets_Mu"+muID+"_El"+elID+".root";
  TFile* ttfile = new TFile(tt.c_str());
  Sample* ttSample = new Sample(vBkgNames.at(0),ttfile, vWeights.at(0),vXsec.at(0),cut,kRed+2);
  vSample.push_back(ttSample);


  std::string nptt = MCarea+"test/NonPromptTTJets_Mu"+muID+"_El"+elID+".root";
  TFile* npttfile = new TFile(nptt.c_str());
  Sample* npttSample = new Sample(vBkgNames.at(1),npttfile, vWeights.at(1),vXsec.at(1),cut,kBlue+2);
  vSample.push_back(npttSample);

  std::string ttb = MCarea+"test/TTbar-powheg_Mu"+muID+"_El"+elID+".root";
  TFile* ttbfile = new TFile(ttb.c_str());
  Sample* ttbSample = new Sample(vBkgNames.at(2),ttbfile, vWeights.at(2),vXsec.at(2),cut,kGreen+2);
  vSample.push_back(ttbSample);

  std::string npttb = MCarea+"test/NonPromptTTbar-powheg_Mu"+muID+"_El"+elID+".root";
  TFile* npttbfile = new TFile(npttb.c_str());
  Sample* npttbSample = new Sample(vBkgNames.at(3),npttbfile, vWeights.at(3),vXsec.at(3),cut,kBlack);
  vSample.push_back(npttbSample);

  return vSample;
}

std::vector<Sample*> getBkgSampleVec(std::string cut, float lumi, std::string elID, std::string muID){  

   //setup info for list of samples, xsec and events run  //make vector of actual number of events run MULTIPLIED BY AMCATNLO WEIGHT
  std::vector<std::string> vBkgNames;  std::vector<float> vXsec;  std::vector<float> vNEvts;
/*
  // ************** MC *************
 //vBkgNames.push_back("TTbar");  vXsec.push_back(831.76);  vNEvts.push_back(42730273 * 0.331582);
  vBkgNames.push_back("TTZ");    vXsec.push_back(0.2529);  vNEvts.push_back(398600 * 0.464706);
  vBkgNames.push_back("TTW");    vXsec.push_back(0.2043);  vNEvts.push_back(252673*0.515587);
  vBkgNames.push_back("TTH");    vXsec.push_back(0.215);  vNEvts.push_back(9984160 *0.296787);
  vBkgNames.push_back("TTTT");    vXsec.push_back(0.009103);  vNEvts.push_back(989025 *0.417453);
  vBkgNames.push_back("WZ");     vXsec.push_back(4.42965); vNEvts.push_back(2000000 * 1);
  //vBkgNames.push_back("WJets");  vXsec.push_back(61526.7); vNEvts.push_back(24151270 * 0.683948);
  //vBkgNames.push_back("DYJets"); vXsec.push_back(6025.2);  vNEvts.push_back(28825132 * 0.6693);
  vBkgNames.push_back("ZZ");     vXsec.push_back(1.212);  vNEvts.push_back(6638328 * 1);
  vBkgNames.push_back("VH");     vXsec.push_back(0.952);  vNEvts.push_back(993464 * 1);
  vBkgNames.push_back("WpWp");   vXsec.push_back(0.03711); vNEvts.push_back( 118350* 1);
  vBkgNames.push_back("WW-mpi"); vXsec.push_back(1.64);   vNEvts.push_back( 843514* 1);  
  vBkgNames.push_back("WWZ");    vXsec.push_back(0.1651); vNEvts.push_back(249200*0.885963);
  vBkgNames.push_back("WZZ");    vXsec.push_back(0.05565); vNEvts.push_back(249800*0.876645);
  vBkgNames.push_back("ZZZ");    vXsec.push_back(0.01398); vNEvts.push_back(250000* 0.8554);
*/

  /////////////Updated on 14 May 2021 ////////////////////
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns
  vBkgNames.push_back("TTZ");    vXsec.push_back(0.2529);  vNEvts.push_back(6274046); // https://cms-gen-dev.cern.ch/xsdb/?columns=37879808&currentPage=0&ordDirection=1&ordFieldName=cross_section&pageSize=10&searchQuery=DAS%3DTTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8
  vBkgNames.push_back("TTW");    vXsec.push_back(0.2043);  vNEvts.push_back(2686095); // https://cms-gen-dev.cern.ch/xsdb/?columns=37879808&currentPage=0&ordDirection=1&ordFieldName=cross_section&pageSize=10&searchQuery=DAS%3DTTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8
  vBkgNames.push_back("TTH");    vXsec.push_back(0.215);  vNEvts.push_back(7368333); // https://cms-gen-dev.cern.ch/xsdb/?columns=37879808&currentPage=0&ordDirection=1&ordFieldName=cross_section&pageSize=10&searchQuery=DAS%3DttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8
  vBkgNames.push_back("TTTT");   vXsec.push_back(0.009103);vNEvts.push_back(882074);  // https://cms-gen-dev.cern.ch/xsdb/?columns=37879808&currentPage=0&ordDirection=1&ordFieldName=cross_section&pageSize=10&searchQuery=DAS%3DTTTT_TuneCP5_13TeV-amcatnlo-pythia8
  vBkgNames.push_back("WZ");     vXsec.push_back(4.42965);   vNEvts.push_back(1955248); // https://cms-gen-dev.cern.ch/xsdb/?columns=40042496&currentPage=0&pageSize=10&searchQuery=DAS%3DWZTo3LNu_TuneCP5
  vBkgNames.push_back("ZZ");     vXsec.push_back(1.256);   vNEvts.push_back(6622242); // https://cms-gen-dev.cern.ch/xsdb/?columns=40042496&currentPage=0&pageSize=10&searchQuery=DAS%3DZZTo4L_TuneCP5_13TeV_powheg_pythia8
  vBkgNames.push_back("WpWp");   vXsec.push_back(0.03711); vNEvts.push_back(149368);  // https://cms-gen-dev.cern.ch/xsdb/?columns=40042496&currentPage=0&pageSize=10&searchQuery=DAS%3DWpWpJJ_EWK-QCD_TuneCP5_13TeV-madgraph-pythia8
  vBkgNames.push_back("WWZ");    vXsec.push_back(0.1651);  vNEvts.push_back(219910);  // https://cms-gen-dev.cern.ch/xsdb/?columns=40042496&currentPage=0&pageSize=10&searchQuery=DAS%3DWWZ_TuneCP5_13TeV-amcatnlo-pythia8
  vBkgNames.push_back("WZZ");    vXsec.push_back(0.05565); vNEvts.push_back(219250);  // https://cms-gen-dev.cern.ch/xsdb/?columns=40042496&currentPage=0&pageSize=10&searchQuery=DAS%3DWZZ_TuneCP5_13TeV-amcatnlo-pythia8
  vBkgNames.push_back("ZZZ");    vXsec.push_back(0.01398); vNEvts.push_back(214282);  // https://cms-gen-dev.cern.ch/xsdb/?columns=40042496&currentPage=0&pageSize=10&searchQuery=DAS%3DZZZ_TuneCP5_13TeV-amcatnlo-pythia8
  ///////////// End updated on 14 May 2021 by Jess ////////////////////

  //******* Non Prompt**********
  //vBkgNames.push_back("NonPromptMC");  vXsec.push_back(831.76);  vNEvts.push_back(42730273 * 0.331582);
  vBkgNames.push_back("NonPrompt"); vXsec.push_back(1); vNEvts.push_back(1);
  vBkgNames.push_back("ChargeMisID"); vXsec.push_back(1); vNEvts.push_back(1);
  //now make vector to hold weights;
  std::vector<float> vWeights;
  for(std::vector<float>::size_type ui=0; ui<vXsec.size(); ui++){
    vWeights.push_back( lumi * 1000 * ( vXsec.at(ui) / vNEvts.at(ui) ) ); //factor of 1000 to convert lumi to pb^-1
  }

  //now make samples and add to vector
  std::vector<Sample*> vSample;
  //TFile* ttfile = new TFile(MCarea+"test/TTbar.root");
  //Sample* ttSample = new Sample(vBkgNames.at(0),ttfile, vWeights.at(0),vXsec.at(0),cut,kRed+2);
  //vSample.push_back(ttSample);

  std::string ttZfilename = MCarea+"test/TTZ_Mu"+muID+"_El"+elID+".root";
  TFile* ttZfile = new TFile(ttZfilename.c_str());
  Sample* ttZSample = new Sample(vBkgNames.at(0),ttZfile, vWeights.at(0),vXsec.at(0),cut,kRed);
  vSample.push_back(ttZSample);

  std::string ttWfilename = MCarea+"test/TTW_Mu"+muID+"_El"+elID+".root";
  TFile* ttwfile = new TFile(ttWfilename.c_str());
  Sample* ttwSample = new Sample(vBkgNames.at(1),ttwfile, vWeights.at(1),vXsec.at(1),cut,kYellow-2);
  vSample.push_back(ttwSample);

  std::string tthfilename = MCarea+"test/TTH_Mu"+muID+"_El"+elID+".root";
  TFile* tthfile = new TFile(tthfilename.c_str());
  Sample* tthSample = new Sample(vBkgNames.at(2),tthfile, vWeights.at(2),vXsec.at(2),cut,kYellow);
  vSample.push_back(tthSample);

  std::string ttttfilename = MCarea+"test/TTTT_Mu"+muID+"_El"+elID+".root";
  TFile* ttttfile = new TFile(ttttfilename.c_str());
  Sample* ttttSample = new Sample(vBkgNames.at(3),ttttfile, vWeights.at(3),vXsec.at(3),cut,kRed+2);
  vSample.push_back(ttttSample);


  std::string wzfilename=MCarea+"test/WZ_Mu"+muID+"_El"+elID+".root";
  TFile* wzfile = new TFile(wzfilename.c_str());
  Sample* wzSample = new Sample(vBkgNames.at(4),wzfile, vWeights.at(4),vXsec.at(4),cut,kBlue-3);
  vSample.push_back(wzSample);

  //TFile* wjfile = new TFile(MCarea+"test/WJets_Mu"+muID+"_El"+elID+".root");
  //Sample* wjSample = new Sample(vBkgNames.at(4),wjfile, vWeights.at(4),vXsec.at(4),cut,kGreen+2);
  //vSample.push_back(wjSample);

  /*std::string dyfilename =  MCarea+"test/DYJets_Mu"+muID+"_El"+elID+".root";
  TFile* dyjfile = new TFile(dyfilename.c_str());
  Sample* dyjSample = new Sample(vBkgNames.at(3),dyjfile, vWeights.at(3),vXsec.at(3),cut,kMagenta+2);
  vSample.push_back(dyjSample);*/
  
  std::string zzfilename = MCarea+"test/ZZ_Mu"+muID+"_El"+elID+".root";
  TFile* zzjfile = new TFile(zzfilename.c_str());
  Sample* zzjSample = new Sample(vBkgNames.at(5),zzjfile, vWeights.at(5),vXsec.at(5),cut,kOrange+1);
  //std::cout<<"weight for ZZ is: "<<vWeights.at(5)<<std::endl;
  vSample.push_back(zzjSample);

  //std::string vhfilename = MCarea+"test/VH_Mu"+muID+"_El"+elID+".root";
  //TFile* vhjfile = new TFile(vhfilename.c_str());
  //Sample* vhjSample = new Sample(vBkgNames.at(6),vhjfile, vWeights.at(6),vXsec.at(6),cut,kBlue);
  //std::cout<<"weight for VH is: "<<vWeights.at(6)<<std::endl;
  //vSample.push_back(vhjSample);

  std::string wpwpfilename = MCarea+"test/WpWp_Mu"+muID+"_El"+elID+".root";
  TFile* wpwpfile = new TFile(wpwpfilename.c_str());
  Sample* wpwpSample = new Sample(vBkgNames.at(6),wpwpfile, vWeights.at(6),vXsec.at(6),cut,kGreen+1);
  //std::cout<<"weight for ZZ is: "<<vWeights.at(5)<<std::endl;
  vSample.push_back(wpwpSample);

  /*std::string wwmpifilename = MCarea+"test/WW-mpi_Mu"+muID+"_El"+elID+".root";
  TFile* wwmpifile = new TFile(wwmpifilename.c_str());
  Sample* wwmpiSample = new Sample(vBkgNames.at(8),wwmpifile, vWeights.at(8),vXsec.at(8),cut,kGreen-1);
  //std::cout<<"weight for ZZ is: "<<vWeights.at(5)<<std::endl;
  vSample.push_back(wwmpiSample);*/


  std::string wwzfilename = MCarea+"test/WWZ_Mu"+muID+"_El"+elID+".root";
  TFile* wwzfile = new TFile(wwzfilename.c_str());
  Sample* wwzSample = new Sample(vBkgNames.at(7),wwzfile, vWeights.at(7),vXsec.at(7),cut,kViolet+1);
  //std::cout<<"weight for ZZ is: "<<vWeights.at(5)<<std::endl;
  vSample.push_back(wwzSample);

  std::string wzzfilename = MCarea+"test/WZZ_Mu"+muID+"_El"+elID+".root";
  TFile* wzzfile = new TFile(wzzfilename.c_str());
  Sample* wzzSample = new Sample(vBkgNames.at(8),wzzfile, vWeights.at(8),vXsec.at(8),cut,kViolet+3);
  //std::cout<<"weight for ZZ is: "<<vWeights.at(5)<<std::endl;
  vSample.push_back(wzzSample);

  std::string zzzfilename = MCarea+"test/ZZZ_Mu"+muID+"_El"+elID+".root";
  TFile* zzzfile = new TFile(zzzfilename.c_str());
  Sample* zzzSample = new Sample(vBkgNames.at(9),zzzfile, vWeights.at(9),vXsec.at(9),cut,kViolet);
  vSample.push_back(zzzSample);


  //********** Nonprompt ***************
  /*std::string npTTfilename = area+"test/NonPromptTTJets_Mu"+muID+"_El"+elID+".root";
  TFile* npttfile = new TFile(nptTfilename.c_str());
  Sample* npttSample = new Sample(vBkgNames.at(-1),npttfile,vWeights.at(-1),vXsec.at(01),cut,kBlue);
  vSample.push_back(npttSample)*/

  std::string npfilename = area+"test/NonPromptData_Mu"+muID+"_El"+elID+".root";
  TFile* npfile = new TFile(npfilename.c_str());
  Sample* npSample = new Sample(vBkgNames.at(10),npfile,vWeights.at(10),vXsec.at(10),cut,kGray);
  vSample.push_back(npSample);

  //********ChargeMisID**********
  std::string cmidfilename = area+"test/ChargeMisID_Mu"+muID+"_El"+elID+".root";
  TFile* cmidfile = new TFile(cmidfilename.c_str());
  Sample* cmidSample = new Sample(vBkgNames.at(11),cmidfile,vWeights.at(11),vXsec.at(11),cut,kAzure+6); //force charge misID to start here since only at this point do we filter events
  vSample.push_back(cmidSample);


  return vSample;

}


std::vector<std::string> getCutString(){

  std::vector<std::string> vString;

  //quarkonia-veto
  std::string null = "Lep1Pt>0";
  vString.push_back(null);
  std::string qv = "("+null+" && (DilepMass>20) )";
  vString.push_back(qv);
  //z mass veto for ee channel only
  std::string zv = "("+qv+" && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) )";
  vString.push_back(zv);
  //Leading Lep pT
  std::string lep1pt = "("+zv+"&& Lep1Pt > 40)" ;
  vString.push_back(lep1pt);
  //SubLeadingLep pT
  //std::string lep2pt = "("+lep1pt+"&& Lep2Pt > 30)" ;
  //vString.push_back(lep2pt);
  //Leading nJetscut
  std::string nConstCut = "("+lep1pt+"&& nConst >= 5)";
  vString.push_back(nConstCut);
  //SubLeading Jet Pt
  //std::string jet2pt = "("+jet1pt+"&& cleanAK4Jet2Pt > 150)";
  //vString.push_back(jet2pt);
  //HT cut
  std::string htcut = "("+nConstCut+"&& cleanAK4HT > 900)";
  vString.push_back(htcut);

  std::string centrallepcut =  "("+htcut+"&& TMath::Abs(Lep1Eta) < 0.9)";
  //vString.push_back(centrallepcut);
  return vString;
}

std::vector<std::string> getBoostedCutString(){

  std::vector<std::string> vString;

  //quarkonia-veto
  std::string null = "Lep1Pt>0";
  vString.push_back(null);
  std::string qv = "("+null+" && (DilepMass>20) )";
  vString.push_back(qv);
  //z mass veto for ee channel only
  std::string zv = "("+qv+" && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) )";
  vString.push_back(zv);
  //Leading Lep pT
  std::string lep1pt = "("+zv+"&& Lep1Pt > 40)" ;
  vString.push_back(lep1pt);
  //SubLeadingLep pT
  //std::string lep2pt = "("+lep1pt+"&& Lep2Pt > 30)" ;
  //vString.push_back(lep2pt);
  //Leading nJetscut
  std::string nConstCut = "("+lep1pt+"&& nNewConst >= 5)";
  vString.push_back(nConstCut);
  //SubLeading Jet Pt
  //std::string jet2pt = "("+jet1pt+"&& cleanAK4Jet2Pt > 150)";
  //vString.push_back(jet2pt);
  //HT cut
  std::string htcut = "("+nConstCut+"&& newCleanAK4HT > 900)";
  vString.push_back(htcut);
  return vString;
}


std::pair<float,float> getNEvtsAndError(Sample* s, std::string cut, int nMu, bool scale_){
  bool scale = scale_;
  TTree* t = s->tree;
  TH1F* hdummy = new TH1F("hdummy","hdummy",1,0,10000);
  hdummy->Sumw2();
  //make cut string based on channel, should always be outside of parantheses for other cuts so a simply && should work also apply chargeMisIDWeight
  std::stringstream channel;
  if(nMu>=0)  channel<<"&& (Channel =="<<nMu<<")";
  else  channel<<"";

  //std::string cutstring= " PUWeight* MCWeight*ChargeMisIDWeight * NPWeight* ( "+cut+channel.str()+")";
  std::string cutstring= " PUWeight * IDSF * IsoSF * trigSF * GsfSF * MCWeight * ChargeMisIDWeight * NPWeight* ( "+cut+channel.str()+")";
  if( (s->name).find("Tprime")!=std::string::npos || (s->name).find("Bprime")!=std::string::npos  ) cutstring = "pdfWeights4LHC[0] * "+cutstring;
  else  cutstring = " 1/abs(MCWeight) * " + cutstring;

  //draw the last variable to cut on just to be safe though it shouldn't matter
  t->Project("hdummy","AK4HT",cutstring.c_str());
  
  //std::cout<<"Sample: "<<s->name<<" and cut: "<<cutstring<<" unweighted events: "<<nEvts<<" weight: "<<s->weight<<std::endl;
  //now weight properly - for Data don't apply
  if( ( (s->name).find("NonPrompt")!=std::string::npos) && ((s->name).find("TTJets")==std::string::npos) ) { scale=false; }
  else if( ( (s->name).find("ChargeMisID")!=std::string::npos) && ((s->name).find("MC")==std::string::npos) ) { scale=false; }
  if(scale){
    hdummy->Scale(s->weight);
    //std::cout<<"Weight for sample: "<<s->name<<" is: "<<s->weight<<std::endl;
  }
  
  float nEvts = hdummy->Integral(1,hdummy->GetNbinsX()+1);
  double_t err;
  hdummy->IntegralAndError(1,hdummy->GetNbinsX()+1,err);
  delete hdummy;
  std::pair<float,float> nEvtsAndError(nEvts,(float)err);
  return nEvtsAndError;
};



std::string getPrettyCutString(std::string cut){

  //find the positions of && in the cut string
  std::vector<size_t> positions; // holds all the positions that sub occurs within str

  size_t pos = cut.find("&&", 0);
  while(pos != std::string::npos)
    {
      positions.push_back(pos);
      pos = cut.find("&&",pos+1);
    }

  //now take the last of them since what we want is the cut applied to the right of the last &&
  if(positions.size()==0) return "SSDL";
  else if(positions.size()==2) return "EE-ZVeto";
  size_t last = positions.at(positions.size()-1);
  size_t length = cut.size() - last; //should give number of characters remaining in string
  length = length -3; //decrement by one to ignor final ')'
  
  std::string prettyString(cut, last+2, length);
  
  return prettyString;

};


CutClass* makeCutClass(Sample* s, std::vector<std::string> vCuts,int nMu)
{


  std::vector<float> vEvts,vErr;
  std::vector<std::string> vCutsPretty;

  for(size_t j=0; j < vCuts.size(); j++){
    
    //get number of events and save to vector - IMPORTANT TO DO THIS WITH THE FULL CUT STRING AND NOT A PRETTY VERSION
    bool scale = true; //helper function will set to false if need be
    std::pair<float,float> evtsAndErr = getNEvtsAndError(s, vCuts.at(j), nMu, scale);
    vEvts.push_back(evtsAndErr.first);
    vErr.push_back(evtsAndErr.second);
    //now trim the cuts string to write prettily
    
    std::string cutPretty = getPrettyCutString(vCuts.at(j));
    vCutsPretty.push_back(cutPretty);
  }

  CutClass* c = new CutClass(s->name,vCutsPretty,vEvts,vErr,s->xsec);
  return c;

};


std::vector<CutClass*> getCutClassVector(std::vector<Sample*> vS, std::vector<std::string> vCuts, int nMu){

  std::vector<CutClass*> vCC;

  for(size_t i =0; i < vS.size(); i++){
    CutClass* c = makeCutClass(vS.at(i),vCuts,nMu);
    vCC.push_back(c);
  }

  return vCC;

};


std::vector<float> getEtaWeights_lpt(TFile* weightfile){
  
  //TGraphAsymmErrors* g = (TGraphAsymmErrors*) weightfile->Get("divide_etaNumHist_by_etaDenHist");
  TH1F* h = (TH1F*) weightfile->Get("h_num_lpt");
  //TH1F* den = (TH1F*) weightfile->Get("etaDenHist");
  //h->Divide(den);
  std::vector<float> etaWeights;

  for(int i=1; i<= h->GetNbinsX(); i++){
    etaWeights.push_back(h->GetBinContent(i));
    std::cout<<"lpt etaweight for bin: "<<i<<" is: "<<h->GetBinContent(i)<<std::endl;
  }
  
  return etaWeights;
  
};

std::vector<float> getEtaWeights_hpt(TFile* weightfile){
  
  //TGraphAsymmErrors* g = (TGraphAsymmErrors*) weightfile->Get("divide_etaNumHist_by_etaDenHist");
  TH1F* h = (TH1F*) weightfile->Get("hpt_final");
  //TH1F* den = (TH1F*) weightfile->Get("etaDenHist");
  //h->Divide(den);
  std::vector<float> etaWeights;

  for(int i=1; i<= h->GetNbinsX(); i++){
    etaWeights.push_back(h->GetBinContent(i));
    std::cout<<"hpt etaweight for bin: "<<i<<" is: "<<h->GetBinContent(i)<<std::endl;
  }
  
  return etaWeights;
  
};

std::vector<float> getEtaWeights_hhpt(TFile* weightfile){
  
  //TGraphAsymmErrors* g = (TGraphAsymmErrors*) weightfile->Get("divide_etaNumHist_by_etaDenHist");
  TH1F* h = (TH1F*) weightfile->Get("hhpt_final");
  //TH1F* den = (TH1F*) weightfile->Get("etaDenHist");
  //h->Divide(den);
  std::vector<float> etaWeights;

  for(int i=1; i<= h->GetNbinsX(); i++){
    etaWeights.push_back(h->GetBinContent(i));
    std::cout<<"hhpt etaweight for bin: "<<i<<" is: "<<h->GetBinContent(i)<<std::endl;
  }
  
  return etaWeights;
  
};



float getEtaWeight_hhpt(float abseta, std::vector<float> etaWeights){
  float weight=0.0;

  if(abseta>1.55) weight = etaWeights.at(3);
  else if(abseta>1.442) weight = 0.0;
  else if(abseta>0.8) weight = etaWeights.at(1);
  else weight = etaWeights.at(0);

  return weight;
}

float getEtaWeight_hpt(float abseta, std::vector<float> etaWeights){
  float weight=0.0;

  if(abseta>2.0) weight = etaWeights.at(5);
  else if(abseta>1.55) weight = etaWeights.at(4);
  else if(abseta>1.442) weight = 0.0;
  else if(abseta>0.8) weight = etaWeights.at(2);
  else if(abseta>0.4) weight = etaWeights.at(1);
  else weight = etaWeights.at(0);

  return weight;
}

float getEtaWeight_lpt(float abseta, std::vector<float> etaWeights){
  float weight=0.0;

  if(abseta>2.0) weight = etaWeights.at(5);
  else if(abseta>1.55) weight = etaWeights.at(4);
  else if(abseta>1.442) weight = 0.0;
  else if(abseta>0.8) weight = etaWeights.at(2);
  else if(abseta>0.4) weight = etaWeights.at(1);
  else weight = etaWeights.at(0);

  return weight;
}

float getEtaWeight(std::vector<float> etaWeights, std::vector<TLepton*> leptons){

  float weight1=1;
  float weight2=1;
  TLepton* Lep1=leptons.at(0);
  TLepton* Lep2=leptons.at(1);

  if(Lep1->isEl){
    if(Lep1->eta<-2.6) weight1 = 0;//no electrons greater than 2.6
    else if(Lep1->eta<-2.2) weight1 = weight1*etaWeights.at(0);
    else if(Lep1->eta<-1.8) weight1 = weight1*etaWeights.at(1);
    else if(Lep1->eta<-1.4) weight1 = weight1*etaWeights.at(2);
    else if(Lep1->eta< -1.0) weight1 = weight1*etaWeights.at(3);
    else if(Lep1->eta< -0.6) weight1 = weight1*etaWeights.at(4);
    else if(Lep1->eta< -0.2) weight1 = weight1*etaWeights.at(5);
    else if(Lep1->eta< 0.2) weight1 = weight1*etaWeights.at(6);
    else if(Lep1->eta< 0.6) weight1 = weight1*etaWeights.at(7);
    else if(Lep1->eta< 1.0) weight1 = weight1*etaWeights.at(8);
    else if(Lep1->eta< 1.4) weight1 = weight1*etaWeights.at(9);
    else if(Lep1->eta< 1.8) weight1 = weight1*etaWeights.at(10);
    else if(Lep1->eta< 2.2) weight1 = weight1*etaWeights.at(11);
    else if(Lep1->eta< 2.6) weight1 = weight1*etaWeights.at(12);
    else if(Lep1->eta< 3.0) weight1 = 0; //no electrons w/ eta greater than 2.6

  }

  if(Lep2->isEl){
    if(Lep2->eta<-2.6) weight2 = 0;//no electrons outside eta 2.6
    else if(Lep2->eta<-2.2) weight2 = weight2*etaWeights.at(0);
    else if(Lep2->eta<-1.8) weight2 = weight2*etaWeights.at(1);
    else if(Lep2->eta<-1.4) weight2 = weight2*etaWeights.at(2);
    else if(Lep2->eta< -1.0) weight2 = weight2*etaWeights.at(3);
    else if(Lep2->eta< -0.6) weight2 = weight2*etaWeights.at(4);
    else if(Lep2->eta< -0.2) weight2 = weight2*etaWeights.at(5);
    else if(Lep2->eta< 0.2) weight2 = weight2*etaWeights.at(6);
    else if(Lep2->eta< 0.6) weight2 = weight2*etaWeights.at(7);
    else if(Lep2->eta< 1.0) weight2 = weight2*etaWeights.at(8);
    else if(Lep2->eta< 1.4) weight2 = weight2*etaWeights.at(9);
    else if(Lep2->eta< 1.8) weight2 = weight2*etaWeights.at(10);
    else if(Lep2->eta< 2.2) weight2 = weight2*etaWeights.at(11);
    else if(Lep2->eta< 2.6) weight2 = weight2*etaWeights.at(12);
    else if(Lep2->eta< 3.0) weight2 = 0; //no electrons outside eta 2.6
  }


  float weight=1;
  if(Lep1->isEl && Lep2->isEl) weight = weight1 + weight2 - weight1*weight2;
  else if( (Lep1->isEl && !Lep2->isEl) || (!Lep1->isEl && Lep2->isEl) ){
    if(Lep1->isEl) weight = weight1;
    else weight = weight2;
  }


  return weight;

};

float getPairMass(TLepton* lep1, TLepton* lep2){

  float mass = (lep1->lv + lep2->lv).M();
  return mass;

}

float getPairMass(TLepton* lep1, TMuon* mu){
  float mass = (lep1->lv + mu->lv).M();
  return mass;
}

float getPairMass(TLepton* lep1, TElectron* el){
  float mass = (lep1->lv + el->lv).M();
  return mass;
}

float getPairMass(TMuon* mu1, TMuon* mu2){
  float mass = (mu1->lv + mu2->lv).M();
  return mass;
}

float getPairMass(TElectron* el1, TElectron* el2){
  float mass = (el1->lv + el2->lv).M();
  return mass;
}


float getPrate(TFile* f){

  TH1F* h1 = (TH1F*) f->Get("ptNumHist");
  TH1F* h2 = (TH1F*) f->Get("ptDenHist");
  
  float rate = (float) h1->GetEntries();
  rate = rate / h2->GetEntries();
  return rate;

}


//functions to get weights for tight/loose method
float getMuFakeRate(std::string ID){

  if(ID=="CBTight") return 0.371;
  else return 0.371;
}

float getMuPromptRate(std::string ID){

  if(ID=="CBTight") return 0.94;
  else return 0.94;
		      
}

float getElFakeRate(std::string ID){

  if(ID=="CBTight") return 0.43;
  else if(ID=="MVATightRC") return 0.298;
  else if(ID=="MVATightCC") return 0.298;
  else if(ID=="CBTightRC") return 0.43;
  else return 0.298;

}

float getElPromptRate(std::string ID){

  if(ID=="CBTight") return 0.7259;
  else if(ID=="MVATightRC") return 0.873;
  else if(ID=="MVATightCC") return 0.873;
  else if(ID=="CBTightRC") return 0.7259;
  else return 0.873;

}  

//method weights
//same flavor
float WeightSF_T0(float pr, float fr){
  float weight = (-1.0)*( pow( fr*pr,2)) / (pow( pr - fr, 2));
  return weight;
}

float WeightSF_T0(std::string elID, std::string muID, int nMu){
  float prate = nMu==2 ? getMuPromptRate(muID) : getElPromptRate(elID);
  float frate = nMu==2 ? getMuFakeRate(muID) : getElFakeRate(elID);
  float weight = WeightSF_T0(prate,frate);
  return weight;
}

float WeightSF_T1(float pr, float fr){
  float weight= (pr*pr * (fr - fr*fr) ) / (pow( (pr -fr),2) );
  return weight;
}

float WeightSF_T1(std::string elID, std::string muID, int nMu){
  float prate = nMu==2 ? getMuPromptRate(muID) : getElPromptRate(elID);
  float frate = nMu==2 ? getMuFakeRate(muID) : getElFakeRate(elID);
  float weight = WeightSF_T1(prate,frate);
  return weight;
}

float WeightSF_T2(float pr, float fr){
  float weight= ( (fr) / (pow( (pr -fr),2)) ) * ( fr*(pow(1.0-pr,2)) - 2.0*pr*(1.0-pr)*(1.0-fr) ) ;
  return weight;
}

float WeightSF_T2(std::string elID, std::string muID, int nMu){
  float prate = nMu==2 ? getMuPromptRate(muID) : getElPromptRate(elID);
  float frate = nMu==2 ? getMuFakeRate(muID) : getElFakeRate(elID);
  float weight = WeightSF_T2(prate,frate);
  return weight;
}


//opposite flavor
float WeightOF_T00(float elpr, float elfr, float mupr, float mufr){
  float weight = (-1.0 * elfr * elpr * mufr * mupr);
  weight = weight * ( 1.0 / ( (elpr - elfr)*(mupr-mufr)) );
  return weight;
}

float WeightOF_T00(std::string elID, std::string muID){

  float elpr = getElPromptRate(elID);
  float elfr = getElFakeRate(elID);
  float mupr = getMuPromptRate(muID);
  float mufr = getMuFakeRate(muID);

  float weight = WeightOF_T00(elpr,elfr,mupr,mufr);
  return weight;

}

float WeightOF_T10(float elpr, float elfr, float mupr, float mufr){
  float weight = elpr * ( 1.0 - elfr) * mupr * mufr;
  weight = weight * ( 1.0 / ( (elpr - elfr)*(mupr-mufr)) );
  return weight;

}

float WeightOF_T10(std::string elID, std::string muID){

  float elpr = getElPromptRate(elID);
  float elfr = getElFakeRate(elID);
  float mupr = getMuPromptRate(muID);
  float mufr = getMuFakeRate(muID);

  float weight = WeightOF_T10(elpr,elfr,mupr,mufr);
  return weight;

}

float WeightOF_T01(float elpr, float elfr, float mupr, float mufr){
  float weight = elpr * elfr * mupr * (1.0 - mufr);
  weight = weight * ( 1.0 / ( (elpr - elfr)*(mupr-mufr)) );
  return weight;
}

float WeightOF_T01(std::string elID, std::string muID){

  float elpr = getElPromptRate(elID);
  float elfr = getElFakeRate(elID);
  float mupr = getMuPromptRate(muID);
  float mufr = getMuFakeRate(muID);

  float weight = WeightOF_T01(elpr,elfr,mupr,mufr);
  return weight;

}


float WeightOF_T11(float elpr, float elfr, float mupr, float mufr){
  float weight = ( elfr*(1.0-elpr)*mufr*(1.0-mupr) ) - ( elfr*(1.0-elpr)*mupr*(1.0-mufr) )  - ( elpr*(1.0-elfr)*mufr*(1.0-mupr) ) ;
  weight = weight * ( 1.0 / ( (elpr - elfr)*(mupr-mufr)) );
  return weight;
}

float WeightOF_T11(std::string elID, std::string muID){

  float elpr = getElPromptRate(elID);
  float elfr = getElFakeRate(elID);
  float mupr = getMuPromptRate(muID);
  float mufr = getMuFakeRate(muID);

  float weight = WeightOF_T11(elpr,elfr,mupr,mufr);
  return weight;

}

float getGsfSF(TLepton* lep){

  float sf=0.0;
  float eta = lep->eta;
  // updated by Jess 27 Nov 2020. 2018 Gsf Tracking scale factor extracted from https://twiki.cern.ch/twiki/bin/view/CMS/EgammaRunIIRecommendations#E_gamma_RECO
  if (lep->pt < 45){
      if( eta < -2.000) sf = 0.986556 ;
      else if( eta < -1.566) sf = 0.986639 ;
      else if( eta < -1.444) sf = 0.986095 ;
      else if( eta < -1.000) sf = 0.987526 ;
      else if( eta < -0.500) sf = 0.991770 ;
      else if( eta < 0.000) sf = 0.989680 ;
      else if( eta < 0.500) sf = 0.996894 ;
      else if( eta < 1.000) sf = 0.998968 ;
      else if( eta < 1.444) sf = 0.987500 ;
      else if( eta < 1.566) sf = 0.973193 ;
      else if( eta < 2.000) sf = 0.987680 ;
      else sf = 0.983556 ;
  }
  else if (lep->pt < 75){
      if( eta < -2.000) sf = 0.987680 ;
      else if( eta < -1.566) sf = 0.988764 ;
      else if( eta < -1.444) sf = 0.967672 ;
      else if( eta < -1.000) sf = 0.989691 ;
      else if( eta < -0.500) sf = 0.989775 ;
      else if( eta < 0.000) sf = 0.989754 ;
      else if( eta < 0.500) sf = 0.996914 ;
      else if( eta < 1.000) sf = 0.997947 ;
      else if( eta < 1.444) sf = 0.989669 ;
      else if( eta < 1.566) sf = 0.980392 ;
      else if( eta < 2.000) sf = 0.990816 ;
      else sf = 0.987730 ;
  }
  else if (lep->pt < 100){
      if( eta < -2.000) sf = 1.002056 ;
      else if( eta < -1.566) sf = 1.018367 ;
      else if( eta < -1.444) sf = 1.043812 ;
      else if( eta < -1.000) sf = 1.008180 ;
      else if( eta < -0.500) sf = 1.008155 ;
      else if( eta < 0.000) sf = 1.003055 ;
      else if( eta < 0.500) sf = 1.021561 ;
      else if( eta < 1.000) sf = 1.019467 ;
      else if( eta < 1.444) sf = 1.010278 ;
      else if( eta < 1.566) sf = 1.035332 ;
      else if( eta < 2.000) sf = 1.010183 ;
      else sf = 1.007165 ;
  }
  else{
      if( eta < -2.000) sf = 0.983673 ;
      else if( eta < -1.566) sf = 0.997967 ;
      else if( eta < -1.444) sf = 1.036677 ;
      else if( eta < -1.000) sf = 1.007179 ;
      else if( eta < -0.500) sf = 1.006104 ;
      else if( eta < 0.000) sf = 1.003043 ;
      else if( eta < 0.500) sf = 1.003043 ;
      else if( eta < 1.000) sf = 1.006104 ;
      else if( eta < 1.444) sf = 1.007179 ;
      else if( eta < 1.566) sf = 1.036677 ;
      else if( eta < 2.000) sf = 0.997967 ;
      else sf = 0.983673 ;
  }
/*
  if(eta<-2.3) sf       =1.02463;
  else if(eta<-2.2)   sf=1.01364;
  else if(eta<-2.0)   sf=1.00728;
  else if(eta<-1.8)   sf=0.994819;
  else if(eta<-1.63)  sf=0.994786;
  else if(eta<-1.556) sf=0.991632;
  else if(eta<-1.2)   sf=0.989701;
  else if(eta<-1.0)   sf=0.985656;
  else if(eta<-0.6)   sf=0.981595;
  else if(eta<-0.4)   sf=0.984678;
  else if(eta<-0.2)   sf=0.981614;
  else if(eta<0.0)    sf=0.980433;
  else if(eta<0.2)    sf=0.984552;
  else if(eta<0.4)    sf=0.988764;
  else if(eta<0.6)    sf=0.987743;
  else if(eta<1.0)    sf=0.987743;
  else if(eta<1.2)    sf=0.987743;
  else if(eta<1.445)  sf=0.98768;
  else if(eta<1.63)   sf=0.989627;
  else if(eta<1.8)    sf=0.992761;
  else if(eta<2.0)    sf=0.991761;
  else if(eta<2.2)    sf=0.99794;
  else if(eta<2.3)    sf=1.00104;
  else                sf=0.989507;
*/
  return sf;

}

float getMu17EffData(float pt, float abseta){
  float eff = 0.0;
  if(abseta < 0.4){
    if ( pt >=500) eff= 0.0;
    else if(pt>200) eff = 0.834333;
    else if(pt>100) eff = 0.900924;
    else if(pt>60) eff = 0.903286;
    else if(pt>40) eff =  0.912421;
    else if(pt>=35) eff = 0.91533;
    else eff = 0.912142;
  }
  else if(abseta < 0.9){
    if ( pt >=500) eff= 0.0;
    else if(pt>200) eff = 0.727319;
    else if(pt>100) eff =  0.92838;
    else if(pt>60) eff = 0.945439;
    else if(pt>40) eff = 0.945843;
    else if(pt>=35) eff = 0.947075;
    else eff = 0.945442;
  }
  else if(abseta < 1.2){
    if ( pt >=500) eff= 0.0;
    else if(pt>200) eff = 0.79981;
    else if(pt>100) eff =  0.90117;
    else if(pt>60) eff = 0.898373 ;
    else if(pt>40) eff = 0.893675 ;
    else if(pt>=35) eff = 0.895495;
    else eff = 0.890953;
  }
  else if(abseta < 2.1){
    if ( pt >=500) eff= 0.0;
    else if(pt>200) eff =  1.0;
    else if(pt>100) eff = 0.891117;
    else if(pt>60) eff = 0.917323;
    else if(pt>40) eff = 0.931366;
    else if(pt>=35) eff =0.932818;
    else eff = 0.933015;
  }
  else if(abseta <= 2.4){
    if ( pt >=500) eff= 0.0;
    else if(pt>200) eff = 1.0;
    else if(pt>100) eff = 0.924275;
    else if(pt>60) eff = 0.910782;
    else if(pt>40) eff = 0.879932;
    else if(pt>=35) eff = 0.866578;
    else eff = 0.846039;
  }

  return eff;

}

float getMu17EffMC(float pt, float abseta){
  float eff = 0.0;
  if(abseta < 0.4){
    if(pt>200) eff = 0.932395;
    else if(pt>100) eff = 0.944177;
    else if(pt>60) eff = 0.954295 ;
    else if(pt>40) eff = 0.957497;
    else if(pt>=35) eff = 0.957751;
    else eff = 0.958989;
  }
  else if(abseta < 0.9){
    if(pt>200) eff = 0.957081;
    else if(pt>100) eff = 0.957001;
    else if(pt>60) eff = 0.959748;
    else if(pt>40) eff = 0.962925;
    else if(pt>=35) eff = 0.963928;
    else eff = 0.965838;
  }
  else if(abseta < 1.2){
    if(pt>200) eff = 0.980884;
    else if(pt>100) eff = 0.972713;
    else if(pt>60) eff = 0.973453;
    else if(pt>40) eff = 0.973526;
    else if(pt>=35) eff = 0.97359;
    else eff =0.972546;
  }
  else if(abseta < 2.1){
    if(pt>200) eff =  0.932101;
    else if(pt>100) eff =  0.952838;
    else if(pt>60) eff = 0.954336;
    else if(pt>40) eff = 0.953637;
    else if(pt>=35) eff = 0.952111;
    else eff = 0.951186;
  }
  else if(abseta <= 2.4){
    if(pt>200) eff = 0.935849;
    else if(pt>100) eff = 0.943539;
    else if(pt>60) eff = 0.942816;
    else if(pt>40) eff = 0.935807;
    else if(pt>=35) eff = 0.92948;
    else eff = 0.922905;
  }

  return eff;

}


float getMu17EffNearbyPhiData(float pt, float abseta){
  float eff = 0.0;
  if(abseta < 0.4){
    if(pt>200) eff = 0.891609;
    else if(pt>100) eff = 0.889164;
    else if(pt>60) eff =  0.925135;
    else if(pt>40) eff = 0.932446;
    else if(pt>=35) eff = 0.933394;
    else eff = 0.896896;
  }
  else if(abseta < 0.9){
    if(pt>200) eff = 0.915328;
    else if(pt>100) eff = 0.937656;
    else if(pt>60) eff = 0.927412;
    else if(pt>40) eff = 0.955635;
    else if(pt>=35) eff = 0.942308;
    else eff = 0.978809;
  }
  else if(abseta < 1.2){
    if(pt>200) eff = 0.857144;
    else if(pt>100) eff = 0.922488;
    else if(pt>60) eff = 0.898345;
    else if(pt>40) eff = 0.913704;
    else if(pt>=35) eff = 0.940167;
    else eff = 0.860298;
  }
  else if(abseta < 2.1){
    if(pt>200) eff = 0.638807;
    else if(pt>100) eff = 0.813527;
    else if(pt>60) eff = 0.918241;
    else if(pt>40) eff = 0.918656;
    else if(pt>=35) eff = 0.948342;
    else eff = 0.93803;
  }
  else if(abseta <= 2.4){
    if(pt>200) eff = 0.599919;
    else if(pt>100) eff = 0.806903;
    else if(pt>60) eff =  0.835923;
    else if(pt>40) eff = 0.796745;
    else if(pt>=35) eff = 1.;
    else eff = 0.806318;
  }

  return eff;

}

float getMu17EffNearbyPhiMC(float pt, float abseta){
  float eff = 0.0;
  if(abseta < 0.4){
    if(pt>200) eff = 0.930309;
    else if(pt>100) eff = 0.948507;
    else if(pt>60) eff =  0.95718;
    else if(pt>40) eff = 0.957901;
    else if(pt>=35) eff = 0.952687;
    else eff = 0.953507;
  }
  else if(abseta < 0.9){
    if(pt>200) eff = 0.932319;
    else if(pt>100) eff = 0.952987;
    else if(pt>60) eff = 0.961281;
    else if(pt>40) eff = 0.962954;
    else if(pt>=35) eff = 0.965018;
    else eff = 0.966145;
  }
  else if(abseta < 1.2){
    if(pt>200) eff = 0.942255;
    else if(pt>100) eff = 0.975508;
    else if(pt>60) eff = 0.965795;
    else if(pt>40) eff = 0.976901;
    else if(pt>=35) eff = 0.969025;
    else eff = 0.971571;
  }
  else if(abseta < 2.1){
    if(pt>200) eff =  0.952413;
    else if(pt>100) eff = 0.949975;
    else if(pt>60) eff = 0.954618;
    else if(pt>40) eff = 0.954047;
    else if(pt>=35) eff = 0.952887;
    else eff = 0.951593;
  }
  else if(abseta <= 2.4){
    if(pt>200) eff = 0.933073;
    else if(pt>100) eff = 0.937694;
    else if(pt>60) eff =  0.936396;
    else if(pt>40) eff = 0.937846;
    else if(pt>=35) eff = 0.936747;
    else eff = 0.919099;
  }

  return eff;

}

float getAvgMu17(TLepton* mu){

  float eta = fabs(mu->eta);
  float eff = 0.0;
  if(eta>2.1) eff = 0.902;
  else if(eta > 1.2) eff = 0.944;
  else if(eta > 0.9) eff = 0.871;
  else if(eta > 0.4) eff = 0.935;
  else eff = 0.897;

  return eff;
}

float getDimuonEff2016BD(float pt1, float eta1, float phi1, float pt2, float eta2, float phi2){
  //get conditional efficiency
  float sf = 0.0;
  float cond_eff_data = 0.0;
  float cond_eff_mc = 0.0;
  //excluding nearby phi so just take wide phi efficiences
  //if(fabs(phi1 - phi2) > 1 ){
  cond_eff_data = 1.0 - (1.0 - getMu17EffData(pt1,eta1))*(1.0 - getMu17EffData(pt2,eta2));
  cond_eff_mc = 1.0 - (1.0 - getMu17EffMC(pt1,eta1))*(1.0 - getMu17EffMC(pt2,eta2));
    //} 
  //else {
  // cond_eff_data = 1.0 - (1.0 - ( getMu17EffData(pt2,eta2) * getMu17EffNearbyPhiData(pt1,eta1) + (1 - getMu17EffData(pt2,eta2) )* getMu17EffData(pt1,eta1) ) ) * (1.0 - getMu17EffData(pt2,eta2) ) ;
  // cond_eff_mc = 1.0 - (1.0 - ( getMu17EffMC(pt2,eta2) * getMu17EffNearbyPhiMC(pt1,eta1) + (1 - getMu17EffMC(pt2,eta2) )* getMu17EffMC(pt1,eta1) ) ) * (1.0 - getMu17EffMC(pt2,eta2) ) ;
  //}
  float cond_eff;
  if(cond_eff_mc!=0) cond_eff= cond_eff_data / cond_eff_mc;
  else {
    std::cout<<"mc eff is zero! lep1mu17: "<<getMu17EffMC(pt1,eta1)<<" lep1mu17-np: "<<getMu17EffNearbyPhiMC(pt1,eta1)<<" lep2mu17: "<<getMu17EffMC(pt2,eta2)<<" lep2mu17-np: "<<getMu17EffNearbyPhiMC(pt2,eta2)<<std::endl;
      cond_eff = 0;
  }
  float soup_eff = 0.0;    
  if(eta1 > 2.1){
    if(eta2>2.1) soup_eff=0.8947;
    else if(eta2>1.2) soup_eff=0.9312;
    else if(eta2>0.9) soup_eff=0.9367;
    else if(eta2>0.4) soup_eff=0.9631;
    else soup_eff=0.8853;
  }
  else if(eta1 > 1.2){
    if(eta2>2.1) soup_eff=0.9359;
    else if(eta2>1.2) soup_eff=1.0119;
    else if(eta2>0.9) soup_eff=1.0614;
    else if(eta2>0.4) soup_eff=1.0150;
    else soup_eff=0.9782;
  }
  else if(eta1 > 0.9){
    if(eta2>2.1) soup_eff=0.9098;
    else if(eta2>1.2) soup_eff=1.0402;
    else if(eta2>0.9) soup_eff=1.0010;
    else if(eta2>0.4) soup_eff=0.9618;
    else soup_eff=0.9667;
  }
  else if(eta1 > 0.4){
    if(eta2>2.1) soup_eff=0.9670;
    else if(eta2>1.2) soup_eff=1.0109;
    else if(eta2>0.9) soup_eff=0.9762;
    else if(eta2>0.4) soup_eff=0.9692;
    else soup_eff=0.9715;
  }
  else{
    if(eta2>2.1) soup_eff=0.8769;
    else if(eta2>1.2) soup_eff=0.9513;
    else if(eta2>0.9) soup_eff=0.9645;
    else if(eta2>0.4) soup_eff=0.9592;
    else soup_eff=0.9565;
  }
  sf = cond_eff*soup_eff;
  return sf;
}

float getDimuonEff(float pt1, float eta1, float phi1, float pt2, float eta2,float phi2, std::string era){
  float eff = 0.0;
  if(era=="2016BD") eff = getDimuonEff2016BD(pt1,eta1,phi1,pt2,eta2,phi2);
  else eff = getDimuonEff2016BD(pt1,eta1,phi1,pt2,eta2,phi2); // DUMMY FOR NOW - TAKING BOTH TO BE THE SAME
  return eff;
}



float getElTrigEffCrossTrigger2016BD(float pt, float eta){
  float eff = 0.0;
  if(eta<0.8) eff = 0.9878;
  else if(eta<1.5) eff  = 0.9916;
  else{
    if(pt<40) eff= (0.9921/0.9940)*( ROOT::Math::normal_cdf(pt-33.50,1.524)); //take from fitted function
    else eff = 0.9981;
  }

  return eff;
}

float getEl37TrigEffCrossTrigger2016EH(float pt, float eta){
  float eff = 0.0;
  if(eta<0.8) eff = 0.9932;
  else if(eta<1.5) eff  = 0.9900;
  else{
    if(pt<45) eff= (0.9882/0.9944)*( ROOT::Math::normal_cdf(pt-37.55,1.650)); //take from fitted function
    else eff = 0.9938;
  }

  return eff;
}

float getEl27TrigEffCrossTrigger2016EH(float pt, float eta){
  float eff = 0.0;
  if(eta<0.8) eff = 0.9942;
  else if(eta<1.5){
    if(pt<35){
      eff= (0.9869/0.947)*( ROOT::Math::normal_cdf(pt-26.91,1.435)); //take from fitted function
    }
    else{
    eff  = 0.9922;
    }
  }
  else{
    if(pt<35) eff= (0.9924/0.9940)*( ROOT::Math::normal_cdf(pt-27.60,1.62)); //take from fitted function
    else eff = 0.9984;
  }

  return eff;
}

float getElTrigEffCrossTrigger(float pt, float eta, std::string era){
  float eff = 0.0;
  if(era=="2016BD") eff = getElTrigEffCrossTrigger2016BD(pt,eta);
  else eff = getEl37TrigEffCrossTrigger2016EH(pt,eta); //leading so ele37
  return eff;
}

float getSubLeadingElTrigEffCrossTrigger(float pt, float eta, std::string era){
  float eff = 0.0;
  if(era=="2016BD") eff = getElTrigEffCrossTrigger2016BD(pt,eta); //no differences from leading
  else eff = getEl27TrigEffCrossTrigger2016EH(pt,eta); //ele27
  return eff;
}

float getMu30TrigEffCrossTrigger2016BD(float pt, float eta){
  float eff = 0.0;
  if(eta>2.1){
    if(pt>500) eff = 0.0;
    else if(pt>200) eff = 0.883891;
    else if(pt>100) eff = 0.97691;
    else if(pt>60) eff = 0.948464;
    else if(pt>40) eff = 0.916142;
    else if(pt>35) eff =0.884849;      
  }
  else if(eta>1.2){
    if(pt>500) eff = 0.0;
    else if(pt>200) eff = 0.945474;
    else if(pt>100) eff =0.955029;
    else if(pt>60) eff = 0.970437;
    else if(pt>40) eff = 0.977069;
    else if(pt>35) eff = 0.972568;
  }
  else if(eta>0.9){
    if(pt>500) eff = 0.0;
    else if(pt>200) eff = 0.93892;
    else if(pt>100) eff = 0.943425;
    else if(pt>60) eff = 0.937789;
    else if(pt>40) eff = 0.937023;
    else if(pt>35) eff = 0.93713;
  }
  else if(eta>0.4){
    if(pt>500) eff = 0.0;
    else if(pt>200) eff = 0.990883;
    else if(pt>100) eff = 0.989853;
    else if(pt>60) eff = 0.981196;
    else if(pt>40) eff = 0.976831;
    else if(pt>35) eff = 0.977478;
  }
  else if(eta>=0.0){
    if(pt>500) eff = 0.0;
    else if(pt>200) eff = 0.970112;
    else if(pt>100) eff =  0.978085;
    else if(pt>60) eff = 0.949755;
    else if(pt>40) eff = 0.94399;
    else if(pt>35) eff = 0.944744;
  }
  
  return eff;
}

float getMu30TrigSFCrossTrigger2016BD(float pt, float eta){
  float eff = 0.0;
  if(eta>2.1){
    if(pt>500) eff = 0.0;
    else if(pt>200) eff = 0.883891;
    else if(pt>100) eff = 0.97691;
    else if(pt>60) eff = 0.948464;
    else if(pt>40) eff = 0.916142;
    else if(pt>35) eff = 0.884849;      
  }
  else if(eta>1.2){
    if(pt>500) eff = 0.0;
    else if(pt>200) eff = 0.945474;
    else if(pt>100) eff = 0.955029;
    else if(pt>60) eff = 0.970437;
    else if(pt>40) eff = 0.977069;
    else if(pt>35) eff = 0.972568;
  }
  else if(eta>0.9){
    if(pt>500) eff = 0.0;
    else if(pt>200) eff =  0.93892;
    else if(pt>100) eff = 0.943425;
    else if(pt>60) eff = 0.937789;
    else if(pt>40) eff = 0.937023;
    else if(pt>35) eff = 0.93713;
  }
  else if(eta>0.4){
    if(pt>500) eff = 0.0;
    else if(pt>200) eff = 0.990883;
    else if(pt>100) eff = 0.989853;
    else if(pt>60) eff =  0.981196;
    else if(pt>40) eff =  0.976831;
    else if(pt>35) eff =  0.977478;
  }
  else if(eta>=0.0){
    if(pt>500) eff = 0.0;
    else if(pt>200) eff = 0.970112;
    else if(pt>100) eff = 0.978085;
    else if(pt>60) eff = 0.949755;
    else if(pt>40) eff = 0.94399;
    else if(pt>35) eff = 0.944744;
  }
  
  return eff;
}

float getMu37TrigEffCrossTrigger2016EH(float pt, float eta){
  float eff = 0.0;
  if(eta>2.1){
    if(pt>200) eff = 0.99002;
    else if(pt>100) eff = 1.01448;
    else if(pt>60) eff = 0.970333;
    else if(pt>40) eff = 0.930914;      
  }
  else if(eta>1.2){
    if(pt>200) eff = 1.00204;
    else if(pt>100) eff = 1.00016;
    else if(pt>60) eff = 0.988236;
    else if(pt>40) eff = 0.979471;      
  }
  else if(eta>0.9){
    if(pt>200) eff = 0.987375;
    else if(pt>100) eff = 0.972674;
    else if(pt>60) eff = 0.96073;
    else if(pt>40) eff = 0.955453;      
  }
  else if(eta>0.4){
    if(pt>200) eff = 0.995882;
    else if(pt>100) eff = 1.00135;
    else if(pt>60) eff = 0.98955;
    else if(pt>40) eff = 0.981639;      
  }
  else if(eta>=0.0){
    if(pt>200) eff = 1.012;
    else if(pt>100) eff =0.999639;
    else if(pt>60) eff = 0.974904;
    else if(pt>40) eff = 0.964368;      
  }
  
  return eff;
}

float getMu27TrigEffCrossTrigger2016EH(float pt, float eta){
  float eff = 0.0;
  if(eta>2.1){
    if(pt>200) eff = 0.995647;
    else if(pt>100) eff = 1.01539;
    else if(pt>60) eff = 0.972176;
    else if(pt>40) eff =  0.939755;      
    else if(pt>30) eff = 0.894049;
  }
  else if(eta>1.2){
    if(pt>200) eff = 1.00204;
    else if(pt>100) eff = 1.00024;
    else if(pt>60) eff = 0.988377;
    else if(pt>40) eff = 0.981811;      
    else if(pt>30) eff = 0.962891;
  }
  else if(eta>0.9){
    if(pt>200) eff = 0.987375;
    else if(pt>100) eff = 0.972605;
    else if(pt>60) eff = 0.960727;
    else if(pt>40) eff = 0.956786;      
    else if(pt>30) eff = 0.952868;
  }
  else if(eta>0.4){
    if(pt>200) eff = 0.995882;
    else if(pt>100) eff = 1.00137;
    else if(pt>60) eff = 0.989568;
    else if(pt>40) eff = 0.982119;      
    else if(pt>30) eff = 0.982987;
  }
  else if(eta>=0.0){
    if(pt>200) eff = 1.012;
    else if(pt>100) eff = 0.999639;
    else if(pt>60) eff =  0.974953;
    else if(pt>40) eff =  0.964935;      
    else if(pt>30) eff = 0.964917;
  }
  
  return eff;
}


float getLeadingMuTrigEffCrossTrigger(float pt, float eta, std::string era){
  float eff = 0.0;
  if(era=="2016BD") eff = getMu30TrigEffCrossTrigger2016BD(pt,eta);
  else eff = getMu37TrigEffCrossTrigger2016EH(pt,eta);
  return eff;
}

float getSubLeadingMuTrigEffCrossTrigger(float pt, float eta, std::string era){
  float eff = 0.0;
  if(era=="2016BD") eff = getMu30TrigEffCrossTrigger2016BD(pt,eta);
  else eff = getMu27TrigEffCrossTrigger2016EH(pt,eta);
  return eff;
}


std::vector<float> getTrigSF(std::vector<TLepton*> vLep,std::string era){

  float sf=0.0, sf_up=0.0, sf_dn=0.0;
  float eta1 = fabs(vLep.at(0)->eta);
  float eta2 = fabs(vLep.at(1)->eta);
  float pt1 = vLep.at(0)->pt;
  float pt2 = vLep.at(1)->pt;

  // Trigger Eff. SF (updated by Jess 11/27/2020) from AN2018-280
  float TrigSFpTbin[8] = {30., 35., 40., 50., 75., 100., 200., 300.};
  float elTrigSFetabin[4] = {0., 0.8, 1.48, 2.0};
  float muTrigSFetabin[5] = {0., 0.8, 1.25, 1.6, 2.1};

  float eeTrigLeadUp[4][8] = {
    {0.002,0.001,0.003,0.001,0.000,0.000,0.000,0.000},
    {0.009,0.002,0.002,0.001,0.000,0.000,0.000,0.000},
    {0.020,0.003,0.001,0.000,0.000,0.000,0.000,0.000},
    {0.007,0.002,0.001,0.000,0.001,0.001,0.001,0.001}
  };
  float eeTrigLeadDn[4][8] = {
    {0.002,0.001,0.002,0.001,0.000,0.000,0.000,0.000},
    {0.009,0.001,0.001,0.000,0.000,0.000,0.000,0.000},
    {0.018,0.002,0.000,0.000,0.000,0.000,0.000,0.000},
    {0.007,0.002,0.001,0.000,0.000,0.000,0.000,0.000}
  };

  float eeTrigLead[4][8] = {
    {0.97,0.97,0.97,0.99,0.99,0.99,0.98,0.99},
    {0.96,0.97,0.98,0.98,0.99,0.99,0.98,0.99},
    {0.95,0.98,0.99,0.99,0.98,0.99,0.99,0.99},
    {0.98,0.99,0.99,0.99,0.99,0.99,0.99,0.97}
  };
  float eeTrigTrail[4][8] = {
    {0.98,0.99,0.99,0.99,0.99,0.99,0.98,0.99},
    {0.98,0.99,0.99,0.99,0.99,0.99,0.98,0.99},
    {0.98,0.98,0.99,0.99,0.98,0.99,0.99,0.99},
    {0.99,1.00,1.00,0.99,0.99,0.99,1.00,0.98}
  };

  float mmTrigLead[5][8] = {
    {0.98,0.98,0.98,0.98,0.98,0.97,0.97,0.96},
    {0.98,0.98,0.98,0.98,0.97,0.97,0.95,0.95},
    {1.04,1.04,1.03,1.02,1.01,1.01,1.01,0.99},
    {1.00,1.00,1.00,0.99,0.99,0.99,0.98,0.99},
    {1.00,1.00,1.00,0.99,0.99,0.99,0.97,0.94}
  };
  float mmTrigTrail[5][8] = {
    {0.98,0.98,0.98,0.98,0.98,0.97,0.97,0.96},
    {0.98,0.98,0.98,0.98,0.97,0.97,0.95,0.95},
    {1.03,1.03,1.03,1.02,1.01,1.01,1.01,0.99},
    {1.00,1.00,1.00,0.99,0.99,0.99,0.98,0.99},
    {1.00,1.00,1.00,0.99,0.99,0.99,0.97,0.94}
  };

  float emTrigLead[4][8] = {
    {1.00,1.00,0.99,0.99,0.99,0.99,0.99,0.99},
    {0.99,1.00,0.99,0.99,0.99,0.99,0.98,0.99},
    {0.98,0.98,0.99,0.99,0.98,0.98,0.98,1.01},
    {0.98,0.99,0.99,0.99,0.99,0.99,0.99,1.02}
  };
  float emTrigTrail[5][8] = {
    {0.98,0.98,0.98,0.98,0.98,0.97,0.97,0.97},
    {0.98,0.97,0.98,0.97,0.96,0.96,0.97,0.91},
    {1.02,1.03,1.02,1.01,1.01,1.00,1.01,0.92},
    {0.98,0.97,0.97,0.97,0.97,0.97,0.95,1.11},
    {0.99,0.97,0.98,0.98,0.97,0.98,0.83,1.05}
  };

  float meTrigLead[5][8] = {
    {0.98,0.98,0.98,0.98,0.97,0.97,0.96,0.98},
    {0.98,0.97,0.98,0.97,0.96,0.96,0.98,0.91},
    {1.04,1.04,1.03,1.01,1.01,1.01,1.01,1.04},
    {1.00,0.99,0.99,0.98,0.99,0.98,0.94,1.10},
    {0.97,0.96,0.98,0.99,0.98,1.00,0.85,1.08}
  };
  float meTrigTrail[4][8] = {
    {1.00,1.00,0.99,0.99,0.99,0.99,0.99,0.99},
    {0.98,0.99,0.99,0.99,0.99,0.99,0.98,0.99},
    {0.98,0.98,0.99,0.99,0.99,0.98,0.98,1.01},
    {0.99,1.00,0.99,0.99,0.99,0.99,0.99,1.02}
  };
  //--- END Trigger Eff. SF ---

  unsigned int pt1Bin  = std::distance(TrigSFpTbin,std::find_if(TrigSFpTbin,TrigSFpTbin+8,[&pt1](float x){return x>pt1;}))-1;
  unsigned int pt2Bin  = std::distance(TrigSFpTbin,std::find_if(TrigSFpTbin,TrigSFpTbin+8,[&pt2](float x){return x>pt2;}))-1;
  unsigned int NptBin  = sizeof(TrigSFpTbin) / sizeof(TrigSFpTbin[0]); 
  if((pt1Bin>NptBin-1) || (pt1>TrigSFpTbin[NptBin-1])) pt1Bin = NptBin-1;
  if((pt2Bin>NptBin-1) || (pt2>TrigSFpTbin[NptBin-1])) pt2Bin = NptBin-1;

  //std::cout<< " Before getting sf : Channel  = "<< vLep.at(0)->isMu << vLep.at(1)->isMu<< std::endl;
  if(vLep.at(0)->isMu && vLep.at(1)->isMu){ //dimuon channel 
    unsigned int eta1Bin = std::distance(muTrigSFetabin,std::find_if(muTrigSFetabin,muTrigSFetabin+5,[&eta1](float x){return x>eta1;}))-1;
    unsigned int eta2Bin = std::distance(muTrigSFetabin,std::find_if(muTrigSFetabin,muTrigSFetabin+5,[&eta2](float x){return x>eta2;}))-1;
    if (pt1>pt2) sf = mmTrigLead[eta1Bin][pt1Bin] * mmTrigTrail[eta2Bin][pt2Bin];
    else sf = mmTrigLead[eta2Bin][pt2Bin] * mmTrigTrail[eta1Bin][pt1Bin];
    sf_up = sf + TMath::Sqrt(2*pow(0.01+0.02,2.0));
    sf_dn = sf - TMath::Sqrt(2*pow(0.01+0.02,2.0));
  }
  else if(vLep.at(0)->isEl && vLep.at(1)->isEl){//dielectron channel
    unsigned int eta1Bin = std::distance(elTrigSFetabin,std::find_if(elTrigSFetabin,elTrigSFetabin+4,[&eta1](float x){return x>eta1;}))-1;
    unsigned int eta2Bin = std::distance(elTrigSFetabin,std::find_if(elTrigSFetabin,elTrigSFetabin+4,[&eta2](float x){return x>eta2;}))-1;
    if (pt1>pt2) {
      sf = eeTrigLead[eta1Bin][pt1Bin] * eeTrigTrail[eta2Bin][pt2Bin];
      sf_up = sf + TMath::Sqrt(pow(eeTrigLeadUp[eta1Bin][pt1Bin]+0.02,2.0)+pow(0.01+0.02,2.0)+0.003*0.003);
      sf_dn = sf - TMath::Sqrt(pow(eeTrigLeadDn[eta1Bin][pt1Bin]+0.02,2.0)+pow(0.01+0.02,2.0)+0.003*0.003);
    }
    else {
      sf = eeTrigLead[eta2Bin][pt2Bin] * eeTrigTrail[eta1Bin][pt1Bin];
      sf_up = sf + TMath::Sqrt(pow(eeTrigLeadUp[eta2Bin][pt2Bin]+0.02,2.0)+pow(0.01+0.02,2.0)+0.003*0.003);
      sf_dn = sf - TMath::Sqrt(pow(eeTrigLeadDn[eta2Bin][pt2Bin]+0.02,2.0)+pow(0.01+0.02,2.0)+0.003*0.003);
    }
  }
  else{ //cross channel
    //get efficiency - first check if leading lepton is electron
    if(vLep.at(0)->isEl){
      unsigned int eta1Bin = std::distance(elTrigSFetabin,std::find_if(elTrigSFetabin,elTrigSFetabin+4,[&eta1](float x){return x>eta1;}))-1;
      unsigned int eta2Bin = std::distance(muTrigSFetabin,std::find_if(muTrigSFetabin,muTrigSFetabin+5,[&eta2](float x){return x>eta2;}))-1;
      if (pt1>pt2) sf = emTrigLead[eta1Bin][pt1Bin] * emTrigTrail[eta2Bin][pt2Bin];
      else sf = meTrigLead[eta2Bin][pt2Bin] * meTrigTrail[eta1Bin][pt1Bin];
      sf_up = sf + TMath::Sqrt(2*pow(0.01+0.02,2.0));
      sf_dn = sf - TMath::Sqrt(2*pow(0.01+0.02,2.0));
    }
    else{ //elEff is for subleading
      unsigned int eta1Bin = std::distance(muTrigSFetabin,std::find_if(muTrigSFetabin,muTrigSFetabin+5,[&eta1](float x){return x>eta1;}))-1;
      unsigned int eta2Bin = std::distance(elTrigSFetabin,std::find_if(elTrigSFetabin,elTrigSFetabin+4,[&eta2](float x){return x>eta2;}))-1;
      //cout << "Bin " << eta1Bin<<"," << pt1Bin <<"," << eta2Bin<<"," << pt2Bin<< endl;
      if (pt1>pt2) sf = meTrigLead[eta1Bin][pt1Bin] * meTrigTrail[eta2Bin][pt2Bin];
      else sf = emTrigLead[eta2Bin][pt2Bin] * emTrigTrail[eta1Bin][pt1Bin];
      sf_up = sf + TMath::Sqrt(2*pow(0.01+0.02,2.0));
      sf_dn = sf - TMath::Sqrt(2*pow(0.01+0.02,2.0));
    }//end check on electron being subleading or leading
  }//end cross channel

  std::vector<float> return_value;
  return_value.push_back(sf);
  return_value.push_back(sf_dn);
  return_value.push_back(sf_up);
  return return_value;

/*
  float phi1 = vLep.at(0)->phi;
  float phi2 = vLep.at(1)->phi;

  if(vLep.at(0)->isMu && vLep.at(1)->isMu){ //dimuon channel    
    sf = getDimuonEff(pt1,eta1,phi1,pt2,eta2,phi2,era);
    //std::cout<<"soup eff for leading lepton with eta: "<<eta1<<" and pt: "<<vLep.at(0)->pt<<"and  for subleading  with eta: "<<eta2<<" and pt: "<<vLep.at(1)->pt<<" is: "<<soup_eff<<" conditional eff is: "<<cond_eff<<"and total sf is: "<<sf<<std::endl;
  }

  else if(vLep.at(0)->isEl && vLep.at(1)->isEl){//dielectron channel
    float w1 = 0.0;
    float w2 = 0.0;
    if(era=="2016B-D"){
      //do weight for first electron
      if(eta1<0.8) w1 = 97.97/99.18; //divide by plateau efficiencies
      else if(eta1<1.5) w1  =  98.58/99.42; //divide by plateau efficiencies
      else{
	if(vLep.at(0)->pt<40) w1= (99.21/99.40)*( ROOT::Math::normal_cdf(vLep.at(0)->pt-33.50,1.524)); //take from fitted function, but divide by mc plateau since mc has turned on
	else w1 = 99.21/99.40;
      }
      //weight for second electron
      if(eta2<0.8) w2 = 97.97/99.18;
      else if(eta2<1.5) w2  = 98.58/99.42;
      else{
	if(vLep.at(1)->pt<40) w2= (99.21/99.40)*( ROOT::Math::normal_cdf(vLep.at(1)->pt-33.50,1.524)); //take from fitted function
	else w2 = 99.21/99.40;
      }
    }
    else{//asymmetric trigger
      //do weight for first electron - Ele37 leg
      if(eta1<0.8) w1 = 98.48/99.15; //divide by plateau efficiencies
      else if(eta1<1.5){
	w1  =  98.38/99.37; //divide by plateau efficiencies
      }
      else{
	if(vLep.at(0)->pt<45) w1= (98.82/99.44)*( ROOT::Math::normal_cdf(vLep.at(0)->pt-37.55,1.650)); //take from fitted function, but divide by mc plateau since mc has turned on
	else w1 = 98.82/99.44;
      }
      //weight for second electron - ele27 leg
      if(eta2<0.8) w2 = 98.62/99.20;
      else if(eta2<1.5){
	if(vLep.at(1)->pt > 35) w2  = 98.69/99.47;
	else w2= (98.69/99.47)*( ROOT::Math::normal_cdf(vLep.at(1)->pt-26.91,1.435)); //take from fitted function
      }
      else{
	if(vLep.at(1)->pt<35) w2= (0.9924/0.9940)*( ROOT::Math::normal_cdf(vLep.at(1)->pt-27.60,1.62)); //take from fitted function
	else w2 = 0.9984;      
      }
    }
    sf = w1*w2;
    //std::cout<<"weight for leading lepton with eta: "<<eta1<<" and pt: "<<vLep.at(0)->pt<<" is: "<<w1<<" weight for subleading  with eta: "<<eta2<<" and pt: "<<vLep.at(1)->pt<<" is: "<<w2<<" total sf = "<<sf<<std::endl;    
  }
  else{ //cross channel

    float elEff = 0.0;
    float muEff = 0.0;
    //get efficiency - first check if leading lepton is electron
    if(vLep.at(0)->isEl){
      elEff = getElTrigEffCrossTrigger(pt1,eta1,era);
      muEff = getSubLeadingMuTrigEffCrossTrigger(pt2,eta2,era);
    }
    else{ //elEff is for subleading
      elEff = getSubLeadingElTrigEffCrossTrigger(pt2,eta2,era);
      muEff = getLeadingMuTrigEffCrossTrigger(pt1,eta1,era);
    }//end check on electron being subleading or leading    
    
    sf = muEff*elEff;
    
  }//end cross channel
  return sf;
*/
}

float getLepIDSF(TLepton* lep){

  float sf=0.;

  if(lep->isMu){ // updated by Jess 27 Nov 2020. 2018 Cut-based Tight scale factors from https://twiki.cern.ch/twiki/bin/view/CMS/MuonReferenceEffs2018
    if (lep->pt < 40){
        if( fabs(lep->eta) < 0.900) sf = 0.991376 ;
        else if( fabs(lep->eta) < 1.200) sf = 0.983824 ;
        else if( fabs(lep->eta) < 2.100) sf = 0.990798 ;
        else sf = 0.973965 ;
    }
    else if (lep->pt < 50){
        if( fabs(lep->eta) < 0.900) sf = 0.991441 ;
        else if( fabs(lep->eta) < 1.200) sf = 0.984217 ;
        else if( fabs(lep->eta) < 2.100) sf = 0.989993 ;
        else sf = 0.973888 ;
    }
    else if (lep->pt < 60){
        if( fabs(lep->eta) < 0.900) sf = 0.991686 ;
        else if( fabs(lep->eta) < 1.200) sf = 0.983000 ;
        else if( fabs(lep->eta) < 2.100) sf = 0.990780 ;
        else sf = 0.973708 ;
    }
    else{
        if( fabs(lep->eta) < 0.900) sf = 0.990580 ;
        else if( fabs(lep->eta) < 1.200) sf = 0.981689 ;
        else if( fabs(lep->eta) < 2.100) sf = 0.988637 ;
        else sf = 0.967750 ;
    }
/*
    if(lep->pt<40){
      if(fabs(lep->eta)>2.1) sf = 0.9711;
      else if(fabs(lep->eta)>1.2) sf = 0.9865;
      else if(fabs(lep->eta)>0.9) sf = 0.9707;
      else sf = 0.9831;
    }
    else if(lep->pt<50){
      if(fabs(lep->eta)>2.1) sf = 0.9718;
      else if(fabs(lep->eta)>1.2) sf = 0.9874;
      else if(fabs(lep->eta)>0.9) sf = 0.9724;
      else sf = 0.9830;
    }
    else if(lep->pt<60){
      if(fabs(lep->eta)>2.1) sf = 0.9676;
      else if(fabs(lep->eta)>1.2) sf = 0.9849;
      else if(fabs(lep->eta)>0.9) sf = 0.9729;
      else sf = 0.9722;
    }
    else if(lep->pt<100){
      if(fabs(lep->eta)>2.1) sf = 0.9533;
      else if(fabs(lep->eta)>1.2) sf = 0.9701;
      else if(fabs(lep->eta)>0.9) sf = 0.9611;
      else sf =0.9608;
    }
    else {
      if(fabs(lep->eta)>2.1) sf = 0.986;
      else if(fabs(lep->eta)>1.2) sf = 1.0027;
      else if(fabs(lep->eta)>0.9) sf = 0.9917;
      else sf =0.9980;
    }    
*/
  }
  else{//electron
    // update by Jess 27 Nov 2020. 2018 MVA-based ID scale factors extracted from https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipesRun2#102X_series_Dataset_2018_Autumn
    if (lep->pt < 35){
        if( lep->eta < -2.000) sf = 0.997558 ;
        else if( lep->eta < -1.566) sf = 0.954233 ;
        else if( lep->eta < -1.444) sf = 1.000000 ;
        else if( lep->eta < -0.800) sf = 0.976581 ;
        else if( lep->eta < 0.000) sf = 0.980140 ;
        else if( lep->eta < 0.800) sf = 0.986014 ;
        else if( lep->eta < 1.444) sf = 0.975207 ;
        else if( lep->eta < 1.566) sf = 1.000000 ;
        else if( lep->eta < 2.000) sf = 0.947429 ;
        else sf = 1.004957 ;
    }
    else if (lep->pt < 50){
        if( lep->eta < -2.000) sf = 0.994344 ;
        else if( lep->eta < -1.566) sf = 0.968581 ;
        else if( lep->eta < -1.444) sf = 1.000000 ;
        else if( lep->eta < -0.800) sf = 0.977876 ;
        else if( lep->eta < 0.000) sf = 0.979098 ;
        else if( lep->eta < 0.800) sf = 0.977998 ;
        else if( lep->eta < 1.444) sf = 0.977778 ;
        else if( lep->eta < 1.566) sf = 1.000000 ;
        else if( lep->eta < 2.000) sf = 0.968581 ;
        else sf = 0.993135 ;
    }
    else if (lep->pt < 100){
        if( lep->eta < -2.000) sf = 0.988901 ;
        else if( lep->eta < -1.566) sf = 0.975661 ;
        else if( lep->eta < -1.444) sf = 1.000000 ;
        else if( lep->eta < -0.800) sf = 0.977149 ;
        else if( lep->eta < 0.000) sf = 0.978355 ;
        else if( lep->eta < 0.800) sf = 0.979415 ;
        else if( lep->eta < 1.444) sf = 0.974863 ;
        else if( lep->eta < 1.566) sf = 1.000000 ;
        else if( lep->eta < 2.000) sf = 0.976695 ;
        else sf = 0.975446 ;
    }
    else if (lep->pt < 200){
        if( lep->eta < -2.000) sf = 0.983352 ;
        else if( lep->eta < -1.566) sf = 0.982143 ;
        else if( lep->eta < -1.444) sf = 1.000000 ;
        else if( lep->eta < -0.800) sf = 0.984848 ;
        else if( lep->eta < 0.000) sf = 0.984865 ;
        else if( lep->eta < 0.800) sf = 0.977346 ;
        else if( lep->eta < 1.444) sf = 1.000000 ;
        else if( lep->eta < 1.566) sf = 1.000000 ;
        else if( lep->eta < 2.000) sf = 0.971816 ;
        else sf = 0.987654 ;
    }
    else{
        if( lep->eta < -2.000) sf = 0.953356 ;
        else if( lep->eta < -1.566) sf = 0.965702 ;
        else if( lep->eta < -1.444) sf = 1.000000 ;
        else if( lep->eta < -0.800) sf = 0.997727 ;
        else if( lep->eta < 0.000) sf = 1.006550 ;
        else if( lep->eta < 0.800) sf = 0.965848 ;
        else if( lep->eta < 1.444) sf = 0.965821 ;
        else if( lep->eta < 1.566) sf = 1.000000 ;
        else if( lep->eta < 2.000) sf = 0.973517 ;
        else sf = 1.013857 ;
    }
/*
    if(fabs(lep->eta)>2.4) sf =0; //shouldn't happen
    if(fabs(lep->eta)>2.0){
      if(lep->pt>200) sf = 0.9714;
      else if(lep->pt>100) sf = 1.0129;
      else if(lep->pt>60)  sf = 0.9784;
      else if(lep->pt>50)  sf = 0.9728;
      else if(lep->pt>40)  sf = 0.9704;
      else if(lep->pt>30)  sf = 0.9629;
    }
    else if(fabs(lep->eta)>1.566){
      if(lep->pt>200) sf = 1.0095;
      else if(lep->pt>100) sf = 0.9916;
      else if(lep->pt>60)  sf = 0.9714;
      else if(lep->pt>50)  sf = 0.9701;
      else if(lep->pt>40)  sf = 0.9645;
      else if(lep->pt>30)  sf = 0.9475;
    }
    else if(fabs(lep->eta) > 1.4442) sf =0; //shouldn't happen
    else if(fabs(lep->eta)>0.8){
      if(lep->pt>200) sf = 0.9415;
      else if(lep->pt>100) sf = 0.9742;
      else if(lep->pt>60)  sf = 0.9562;
      else if(lep->pt>50)  sf = 0.9513;
      else if(lep->pt>40)  sf = 0.9522;
      else if(lep->pt>30)  sf = 0.9465;
    }
    else{
      if(lep->pt>200) sf = 0.9850;
      else if(lep->pt>100) sf = 0.9851;
      else if(lep->pt>60)  sf = 0.9766;
      else if(lep->pt>50)  sf = 0.9734;
      else if(lep->pt>40)  sf = 0.9733;
      else if(lep->pt>30)  sf = 0.9707;
    }
*/

  }
/* muon tracking SF = 1 
  //now add tracking SF for muons
  if(lep->isMu){
    if(lep->eta <-2.1) sf= sf*0.991237;
    else if(lep->eta <-1.6) sf= sf*0.994853;
    else if(lep->eta <-1.2) sf= sf*0.996413;
    else if(lep->eta <-0.9) sf= sf*0.997157;
    else if(lep->eta <-0.6) sf= sf*0.997512;
    else if(lep->eta <-0.3) sf= sf*0.997559;
    else if(lep->eta <-0.2) sf= sf*0.996745;
    else if(lep->eta <0.2) sf= sf*0.996997;
    else if(lep->eta <0.3) sf= sf*0.99772;
    else if(lep->eta <0.6) sf= sf*0.998604;
    else if(lep->eta <0.9) sf= sf*0.998321;
    else if(lep->eta <1.2) sf= sf*0.997682;
    else if(lep->eta <1.6) sf= sf*0.995252;
    else if(lep->eta <2.1) sf= sf*0.994919;
    else if(lep->eta <2.4) sf= sf*0.987334;
  }
*/
  return sf;
}

float getElIDSFunc(TLepton* lep){

  float sf_unc=0.;

  if(lep->isMu){ sf_unc = 0.0; }
  else{
    if (lep->pt < 35){
        if( lep->eta < -2.000) sf_unc = 0.020 ;
        else if( lep->eta < -1.566) sf_unc = 0.019 ;
        else if( lep->eta < -1.444) sf_unc = 1.000 ;
        else if( lep->eta < -0.800) sf_unc = 0.024 ;
        else if( lep->eta < 0.000) sf_unc = 0.016 ;
        else if( lep->eta < 0.800) sf_unc = 0.016 ;
        else if( lep->eta < 1.444) sf_unc = 0.024 ;
        else if( lep->eta < 1.566) sf_unc = 1.000 ;
        else if( lep->eta < 2.000) sf_unc = 0.019 ;
        else sf_unc = 0.020 ;
    }
    else if (lep->pt < 50){
        if( lep->eta < -2.000) sf_unc = 0.006 ;
        else if( lep->eta < -1.566) sf_unc = 0.004 ;
        else if( lep->eta < -1.444) sf_unc = 1.000 ;
        else if( lep->eta < -0.800) sf_unc = 0.003 ;
        else if( lep->eta < 0.000) sf_unc = 0.003 ;
        else if( lep->eta < 0.800) sf_unc = 0.003 ;
        else if( lep->eta < 1.444) sf_unc = 0.003 ;
        else if( lep->eta < 1.566) sf_unc = 1.000 ;
        else if( lep->eta < 2.000) sf_unc = 0.004 ;
        else sf_unc = 0.006 ;
    }
    else if (lep->pt < 100){
        if( lep->eta < -2.000) sf_unc = 0.005 ;
        else if( lep->eta < -1.566) sf_unc = 0.003 ;
        else if( lep->eta < -1.444) sf_unc = 1.000 ;
        else if( lep->eta < -0.800) sf_unc = 0.002 ;
        else if( lep->eta < 0.000) sf_unc = 0.006 ;
        else if( lep->eta < 0.800) sf_unc = 0.006 ;
        else if( lep->eta < 1.444) sf_unc = 0.002 ;
        else if( lep->eta < 1.566) sf_unc = 1.000 ;
        else if( lep->eta < 2.000) sf_unc = 0.003 ;
        else sf_unc = 0.005 ;
    }
    else if (lep->pt < 200){
        if( lep->eta < -2.000) sf_unc = 0.023 ;
        else if( lep->eta < -1.566) sf_unc = 0.011 ;
        else if( lep->eta < -1.444) sf_unc = 1.000 ;
        else if( lep->eta < -0.800) sf_unc = 0.015 ;
        else if( lep->eta < 0.000) sf_unc = 0.007 ;
        else if( lep->eta < 0.800) sf_unc = 0.007 ;
        else if( lep->eta < 1.444) sf_unc = 0.015 ;
        else if( lep->eta < 1.566) sf_unc = 1.000 ;
        else if( lep->eta < 2.000) sf_unc = 0.010 ;
        else sf_unc = 0.023 ;
    }
    else{
        if( lep->eta < -2.000) sf_unc = 0.092 ;
        else if( lep->eta < -1.566) sf_unc = 0.041 ;
        else if( lep->eta < -1.444) sf_unc = 1.000 ;
        else if( lep->eta < -0.800) sf_unc = 0.033 ;
        else if( lep->eta < 0.000) sf_unc = 0.030 ;
        else if( lep->eta < 0.800) sf_unc = 0.029 ;
        else if( lep->eta < 1.444) sf_unc = 0.033 ;
        else if( lep->eta < 1.566) sf_unc = 1.000 ;
        else if( lep->eta < 2.000) sf_unc = 0.030 ;
        else sf_unc = 0.075 ;
    }
  }
  return sf_unc;
}

std::vector<float> getLepIDSF(std::vector<TLepton*> vLep){

  float sf1,sf2;
  sf1 = getLepIDSF(vLep.at(0));
  sf2 = getLepIDSF(vLep.at(1));
  float sf1_unc,sf2_unc;
  sf1_unc = getElIDSFunc(vLep.at(0));
  sf2_unc = getElIDSFunc(vLep.at(1));
  float sf = sf1*sf2;
  float dsf = sf1_unc+sf2_unc;
  float sf_up = sf+dsf;
  float sf_dn = sf-dsf;
  std::vector<float> sf_vec{sf, sf_up, sf_dn};
  return sf_vec;

}

float getLepIsoSF(TLepton* lep){

  float sf;
  //if(lep->isMu) sf = 1.0; //no scale factor for muon iso
  if(lep->isMu){ // added by Jess 27 Nov 2020. 2018 Muon SF for MiniIso<0.1 from https://wiwong.web.cern.ch/wiwong/Muon_fit_Eff_Plots/2018_Efficiency20_3miniTight_Tight_abseta_mrange85/ 
        if (lep->pt < 40){
            if( fabs(lep->eta) < 0.900) sf = 0.995853 ;
            else if( fabs(lep->eta) < 1.200) sf = 0.995681 ;
            else if( fabs(lep->eta) < 2.100) sf = 1.007643 ;
            else sf = 1.010080 ;
        }
        else if (lep->pt < 50){
            if( fabs(lep->eta) < 0.900) sf = 0.998145 ;
            else if( fabs(lep->eta) < 1.200) sf = 0.997785 ;
            else if( fabs(lep->eta) < 2.100) sf = 1.003970 ;
            else sf = 1.005910 ;
        }
        else if (lep->pt < 60){
            if( fabs(lep->eta) < 0.900) sf = 0.998896 ;
            else if( fabs(lep->eta) < 1.200) sf = 0.998224 ;
            else if( fabs(lep->eta) < 2.100) sf = 1.001900 ;
            else sf = 1.002886 ;
        }
        else if (lep->pt < 120){
            if( fabs(lep->eta) < 0.900) sf = 0.999724 ;
            else if( fabs(lep->eta) < 1.200) sf = 0.999496 ;
            else if( fabs(lep->eta) < 2.100) sf = 1.000063 ;
            else sf = 1.000305 ;
        }
        else if (lep->pt < 200){
            if( fabs(lep->eta) < 0.900) sf = 1.000031 ;
            else if( fabs(lep->eta) < 1.200) sf = 1.001590 ;
            else if( fabs(lep->eta) < 2.100) sf = 0.998330 ;
            else sf = 1.000000 ;
        }
        else{
            if( fabs(lep->eta) < 0.900) sf = 1.000000 ;
            else if( fabs(lep->eta) < 1.200) sf = 1.000000 ;
            else if( fabs(lep->eta) < 2.100) sf = 1.000000 ;
            else sf = 1.000000 ;
        }
  }
  else{ // added by Jess 27 Nov 2020. Reveiw slides in https://indico.cern.ch/event/842068/contributions/3636508/attachments/1943612/3223940/EleSF_review.pdf
        if(fabs(lep->eta) < 0.800000){
            if (lep->pt < 40) sf = 0.996901 ;
            else if (lep->pt < 50) sf = 0.997970 ;
            else if (lep->pt < 60) sf = 0.998989 ;
            else if (lep->pt < 100) sf = 1.000000 ;
            else if (lep->pt < 200) sf = 1.001007 ;
            else sf = 1.000000 ;
        }
        else if(fabs(lep->eta) < 1.444000){
            if (lep->pt < 40) sf = 1.000000 ;
            else if (lep->pt < 50) sf = 0.997963 ;
            else if (lep->pt < 60) sf = 0.998988 ;
            else if (lep->pt < 100) sf = 1.000000 ;
            else if (lep->pt < 200) sf = 1.002010 ;
            else sf = 0.998997 ;
        }
        else if(fabs(lep->eta) < 1.566000){
            if (lep->pt < 40) sf = 1.044864 ;
            else if (lep->pt < 50) sf = 1.024017 ;
            else if (lep->pt < 60) sf = 1.034935 ;
            else if (lep->pt < 100) sf = 1.031694 ;
            else if (lep->pt < 200) sf = 1.018481 ;
            else sf = 0.998998 ;
        }
        else if(fabs(lep->eta) < 2.000000){
            if (lep->pt < 40) sf = 1.011470 ;
            else if (lep->pt < 50) sf = 1.006129 ;
            else if (lep->pt < 60) sf = 1.003039 ;
            else if (lep->pt < 100) sf = 1.000000 ;
            else if (lep->pt < 200) sf = 1.001002 ;
            else sf = 1.001001 ;
        }
        else{
            if (lep->pt < 40) sf = 1.018750 ;
            else if (lep->pt < 50) sf = 1.010225 ;
            else if (lep->pt < 60) sf = 1.008122 ;
            else if (lep->pt < 100) sf = 1.001007 ;
            else if (lep->pt < 200) sf = 1.003015 ;
            else sf = 1.003012 ;
        }
//    if(fabs(lep->eta) > 2.0){
//      if(lep->pt>200) sf = 0.997;
//      else if(lep->pt>40) sf = 1.0;
//      else if (lep->pt>30) sf = 0.9874;
//    }
//    else sf=1.0; //it's one for all other bins
    
  }

  return sf;

}

float getLepIsoSF(std::vector<TLepton*> leps){

  float sf1 = getLepIsoSF(leps.at(0));
  float sf2 = getLepIsoSF(leps.at(1));
  float sf = sf1* sf2;
  return sf;

}

std::pair<float,float> getFakeRateByFlavor (std::string flavor){

  float mufr;
  float elfr;

  if(flavor=="data"){
    mufr=0.347;
    elfr=0.288;
  }

  if(flavor=="light"){
    mufr = 0.120;
    elfr = 0.209;
  }

  if(flavor=="charm"){
    mufr=0.376;
    elfr=0.215;
  }

  if(flavor=="bottom"){
    mufr=0.283;
    elfr=0.180;
  }
  if(flavor=="fakes"){
    mufr=0.109;
    elfr=0.278;
  }
  if(flavor=="average"){
    mufr=0.281;
    elfr=0.203;
  }
  if(flavor=="light_qcd"){
    mufr=0.193;
    elfr=0.585;
  }
  if(flavor=="charm_qcd"){
    mufr=0.330;
    elfr=0.323;
  }
  if(flavor=="bottom_qcd"){
    mufr=0.390;
    elfr=0.211;
  }
  if(flavor=="fakes_qcd"){
    mufr=0.354;
    elfr=0.374;
  }
  if(flavor=="average_qcd"){
    mufr=0.392;
    elfr=0.371;
  }

  if(flavor=="light_ttb"){
    mufr=0.405;
    elfr=0.249;
  }
  if(flavor=="charm_ttb"){
    mufr=0.376;
    elfr=0.209;
  }
  if(flavor=="bottom_ttb"){
    mufr=0.297;
    elfr=0.185;
  }
  if(flavor=="fakes_ttb"){
    mufr=0.117;
    elfr=0.263;
  }
  if(flavor=="unmatched_ttb"){
    mufr=0.692;
    elfr=0.116;
  }
  if(flavor=="average_ttb"){
    mufr=0.294;
    elfr=0.202;
  }


  std::pair<float,float> fr(mufr,elfr);
  return fr;
}

std::vector<float> weights_elel(std::string flavor){

  std::vector<float> weights;
  float elfr = getFakeRateByFlavor(flavor).second;
  float elpr = getElPromptRate("MVATightRC");
  //dielectron channel so only same flavor weights, order is 0: nt00, 1: nt01, 2: nt10, 3: nt11
  weights.push_back(WeightSF_T0(elpr,elfr));
  weights.push_back(WeightSF_T1(elpr,elfr));
  weights.push_back(WeightSF_T1(elpr,elfr));
  weights.push_back(WeightSF_T2(elpr,elfr));
  return weights;
}

std::vector<float> weights_elmu(std::string flavor){

  std::vector<float> weights;
  float elfr = getFakeRateByFlavor(flavor).second;
  float elpr = getElPromptRate("MVATightRC");
  float mufr = getFakeRateByFlavor(flavor).first;
  float mupr = getMuPromptRate("CBTight");

  //cross channel, but order remains the same
  weights.push_back(WeightOF_T00(elpr,elfr,mupr,mufr));
  weights.push_back(WeightOF_T01(elpr,elfr,mupr,mufr));
  weights.push_back(WeightOF_T10(elpr,elfr,mupr,mufr));
  weights.push_back(WeightOF_T11(elpr,elfr,mupr,mufr));

  return weights;
}

std::vector<float> weights_mumu(std::string flavor){

  std::vector<float> weights;
  float mufr = getFakeRateByFlavor(flavor).first;
  float mupr = getMuPromptRate("MVATightRC");
  //dimuectron channel so only same flavor weights, order is 0: nt00, 1: nt01, 2: nt10, 3: nt11
  weights.push_back(WeightSF_T0(mupr,mufr));
  weights.push_back(WeightSF_T1(mupr,mufr));
  weights.push_back(WeightSF_T1(mupr,mufr));
  weights.push_back(WeightSF_T2(mupr,mufr));
  return weights;
}

float getPUWeight(std::string FileName, int index, int shift){

  float weight = -1;
  //std::cout<<"nInteractions: "<< index <<std::endl;
  if (shift==0) weight = PUweightsMap.GetPUcentral(FileName, index);
  else if(shift==1) weight = PUweightsMap.GetPUup(FileName, index);
  else if(shift==-1) weight = PUweightsMap.GetPUdown(FileName, index);
  else std::cout<<"FATAL ERROR!!!! unrecognised shift for file " << FileName <<std::endl;
  //weight = h->GetBinContent(nPU+1); // plus one because 1st bin is 0-0.9999 so pilupe of 1 falls 
  //weight = h->GetBinContent(nPU+1); // plus one because 1st bin is 0-0.9999 so pilupe of 1 falls in bin 2
  //std::cout<<"nInteractions: "<<nPU<<" and weight: "<<weight<<std::endl;
  return weight;

};



std::vector<TH1F*> initHistos(std::vector<Variable*> vVar, int nMu, std::string cutname){

  //make vector
  std::vector<TH1F*> hists;
  std::string chan ="";
  if(nMu==-1) chan ="All";
  if(nMu==0) chan ="ElEl";
  if(nMu==1) chan ="ElMu";
  if(nMu==2) chan ="MuMu";  
  
  for(unsigned int i=0; i< vVar.size(); i++){
    Variable* var = vVar.at(i);
    std::string histname = chan+"_"+cutname+"_"+var->name;
    hists.push_back(new TH1F(histname.c_str(),(var->name).c_str(), var->nbins, var->xmin, var->xmax));
  }
  for(unsigned int i=0; i< hists.size();i++){
    //save weights
    hists.at(i)->Sumw2();
  }
  return hists;

};


void fillHistos(std::vector<TH1F*> hists,std::vector<TLepton*> vSSLep, std::vector<TLepton*> vNonSSLep, std::vector<TJet*> jets, float MET, float DilepMass, float weight){
  
  
  //make nConst and HT
  int nConst = vNonSSLep.size() + jets.size();
  float HT = vSSLep.at(0)->pt + vSSLep.at(1)->pt;
  for(unsigned int i=0; i< vNonSSLep.size(); i++){
    HT = HT + vNonSSLep.at(i)->pt;
  }
  for(unsigned int i=0; i< jets.size(); i++){
    HT = HT + jets.at(i)->pt;
  }
  
  //fill histograms
  for(unsigned int i=0; i<hists.size(); i++){
   
    std::string title = hists.at(i)->GetTitle();
    if(title=="Lep1Pt") hists.at(i)->Fill(vSSLep.at(0)->pt,weight);
    if(title=="Lep1Eta") hists.at(i)->Fill(vSSLep.at(0)->eta,weight);
    if(title=="Lep1Phi") hists.at(i)->Fill(vSSLep.at(0)->phi,weight);
    
    if(title=="Lep2Pt") hists.at(i)->Fill(vSSLep.at(1)->pt,weight);
    if(title=="Lep2Eta") hists.at(i)->Fill(vSSLep.at(1)->eta,weight);
    if(title=="Lep2Phi") hists.at(i)->Fill(vSSLep.at(1)->phi,weight);
    
    if(title=="cleanAK4Jet1Pt") {
      if(jets.size() >0) hists.at(i)->Fill(jets.at(0)->pt,weight);
      else hists.at(i)->Fill(-999,weight);
    }
    if(title=="cleanAK4Jet1Eta") {
      if(jets.size() >0) hists.at(i)->Fill(jets.at(0)->eta,weight);
      else hists.at(i)->Fill(-999,weight);
    }
    if(title=="cleanAK4Jet1Phi"){
      if(jets.size() >0) hists.at(i)->Fill(jets.at(0)->phi,weight);
      else hists.at(i)->Fill(-999,weight);
    }
    
    if(title=="cleanAK4Jet2Pt"){
      if(jets.size()>1) hists.at(i)->Fill(jets.at(1)->pt,weight);
      else hists.at(i)->Fill(-999,weight);
    }
    if(title=="cleanAK4Jet2Eta"){
      if(jets.size()>1) hists.at(i)->Fill(jets.at(1)->eta,weight);
      else hists.at(i)->Fill(-999,weight);
    }
    if(title=="cleanAK4Jet2Phi"){
      if(jets.size()>1) hists.at(i)->Fill(jets.at(1)->phi,weight);
      else hists.at(i)->Fill(-999,weight);
    }
    
    if(title=="cleanAK4HT") hists.at(i)->Fill(HT,weight);
    if(title=="nCleanAK4Jets") hists.at(i)->Fill(jets.size(),weight);
    if(title=="nNonSSLeps") hists.at(i)->Fill(vNonSSLep.size(),weight);
    if(title=="MET") hists.at(i)->Fill(MET,weight);
    if(title=="DilepMass") hists.at(i)->Fill(DilepMass,weight);
    if(title=="nConst") hists.at(i)->Fill(nConst,weight);
    
    
  }
  
  
};
		      
void writeHistos(TFile* f, std::vector<TH1F*> hists){

  for(unsigned int i=0; i< hists.size();i++){
    f->WriteTObject(hists.at(i));
  }
};

