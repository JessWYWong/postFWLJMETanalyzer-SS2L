#include "../interface/TreeReader.h"

TreeReader::TreeReader(const TString &filename,const TString &treename, bool mc,bool latestVers)
{

  TFile *f = TFile::Open(filename);
  f->cd();
  TTree *treetemp = (TTree*)f->Get(treename);
  isMc=mc;
  latestVersion=latestVers;
  Init(treetemp);

}

TreeReader::TreeReader(TTree *treetemp,bool mc,bool latestVers)
{
  isMc=mc;
  latestVersion=latestVers;
  Init(treetemp);
}
TreeReader::~TreeReader(){

}
//need to actually get the data
Int_t TreeReader::GetEntry(Long64_t entry){

  bool debug = false;
  
  if(debug)

  //delete anything hanging out in memory
  for (unsigned int i = 0;i<allMuons.size();++i ) delete allMuons[i];
  for (unsigned int i = 0;i<allElectrons.size();++i ) delete allElectrons[i];
  for (unsigned int i = 0;i<allAK4Jets.size();++i ) delete allAK4Jets[i];
  for (unsigned int i = 0;i<cleanedAK4Jets.size();++i ) delete cleanedAK4Jets[i];
  for (unsigned int i = 0;i<newCleanedAK4Jets.size();++i ) delete newCleanedAK4Jets[i];
  if(debug) if(debug) std::cout<<"deleting ak8 jets"<<std::endl;
  for (unsigned int i = 0;i<allAK8Jets.size();++i ) delete allAK8Jets[i];
  if(debug) std::cout<<"deleted ak8 jets"<<std::endl;
  for (unsigned int i = 0;i<simpleCleanedAK4Jets.size();++i ) delete simpleCleanedAK4Jets[i];
  if(isMc){
    for (unsigned int i = 0;i<genJets.size();++i ) delete genJets[i];
    for (unsigned int i = 0;i<genParticles.size();++i ) delete genParticles[i];
    for (unsigned int i = 0;i<hadronicGenJets.size();++i ) delete hadronicGenJets[i];
  }
  if(debug) std::cout<<"clearing collections"<<std::endl;
  allMuons.clear();
  goodMuons.clear();
  looseMuons.clear();
  allElectrons.clear();
  goodElectrons.clear();
  looseElectrons.clear();
  cmsdasElectrons.clear();
  good50nsElectrons.clear();
  loose50nsElectrons.clear();
  allAK4Jets.clear();
  allAK8Jets.clear();
  cleanedAK4Jets.clear();
  newCleanedAK4Jets.clear();
  simpleCleanedAK4Jets.clear();
  if(isMc){
    genJets.clear();
    genParticles.clear();
    hadronicGenJets.clear();
  }

  //check to make sure not empty
  if (!tree) return 0;
  if(debug) std::cout<<"getting size of input vectors"<<std::endl;
  int stat =  tree->GetEntry(entry);
  unsigned int nMuons = muPt->size();
  unsigned int nElectrons = elPt->size();
  unsigned int nAK4Jets = AK4JetPt->size();
  unsigned int nCleanedAK4Jets = cleanedAK4JetPt->size();
  if(debug) std::cout<<"getting number of ak8 jets"<<std::endl;
  unsigned int nAK8Jets = AK8JetPt->size();
  if(debug) std::cout<<"making collections"<<std::endl;
  if(debug) std::cout<<"making electron collections"<<std::endl;
  //make all electrons
  for(unsigned int i=0; i<nElectrons;i++){
  	//NOTE: both mvaValue are the same (elMVAValue_DileptonCalc). -- rizki Mar 27,2019
    allElectrons.push_back(new TElectron(
                                         (*elPt)[i],
                                         (*elEta)[i],
                                         (*elPhi)[i],
                                         (*elEnergy)[i],
                                         (*elCharge)[i],
                                         (*elGsfCharge)[i],
                                         (*elCtfCharge)[i],
                                         (*elScPixCharge)[i],
                                         (*elDeta)[i],
                                         (*elDphi)[i],
                                         (*elDZ)[i],
                                         (*elSIP3d)[i],
                                         (*elD0)[i],
                                         (*elHoE)[i],
                                         (*elMHits)[i],
                                         (*elOoemoop)[i],
                                         (*elSihih)[i],
                                         (*elchIso)[i],
                                         (*elpuIso)[i],
                                         (*elneutIso)[i],
                                         (*elphotIso)[i],
                                         (*elrhoIso)[i],
                                         (*elAEff)[i],
                                         (*elPassConversionVeto)[i],
                                         (*elChargeConsistent)[i],
                                         (*elMVAValue)[i],
                                         (*elMVAValue)[i], 
                                         (*elMiniIso)[i], 
                                         (*elSusyIso)[i]
                                         ) 
                            );
    allElectrons.at(i)->set_mva94XTightV2_80((*elIsMVATight80)[i]);
    allElectrons.at(i)->set_mva94XTightV2_90((*elIsMVATight90)[i]);
    allElectrons.at(i)->set_mva94XLooseV2((*elIsMVALoose)[i]);
    allElectrons.at(i)->set_mva94XTightIsoV2_80((*elIsMVATightIso80)[i]); 
    allElectrons.at(i)->set_mva94XTightIsoV2_90((*elIsMVATightIso90)[i]);
    allElectrons.at(i)->set_mva94XLooseIsoV2((*elIsMVALooseIso)[i]);

  }

  if(debug) std::cout<<"making muon collections"<<std::endl;
  //make all muons
  for(unsigned int i=0; i<nMuons;i++){
    allMuons.push_back(new TMuon((*muPt)[i],
                                 (*muEta)[i], 
                                 (*muPhi)[i], 
                                 (*muEnergy)[i],
                                 (*muCharge)[i], 
                                 (*muIsLoose)[i],
                                 (*muIsTight)[i],
                                 (*muGlobal)[i],
                                 (*muPF)[i],
                                 (*muTracker)[i],
                                 (*muChi2)[i], 
                                 (*muNValMuHits)[i],  
                                 (*muNMatchedStations)[i],
                                 (*muDxy)[i], 
                                 (*muDz)[i],
                                 (*muSIP3d)[i],
                                 (*muNValPixelHits)[i],
                                 (*muNTrackerLayers)[i],
                                 (*muRelIso)[i], 
                                 (*muMiniIso)[i], 
                                 (*muSusyIso)[i]
                                 ) 
                            );
  }

  if(debug) std::cout<<"making jet collections"<<std::endl;
  //make all jets
  for (unsigned int i=0;i<nAK4Jets; i++){
    //require jet to be greater than 30 GeV and eta less than 2.4
    if( ( (*AK4JetPt)[i]<30) || fabs((*AK4JetEta)[i])>2.4) continue;
    allAK4Jets.push_back(new TJet( (*AK4JetPt)[i], (*AK4JetEta)[i], (*AK4JetPhi)[i],(*AK4JetEnergy)[i]) );
  }

  if(debug) std::cout<<"making AK8 collections"<<std::endl;
  //make AK8, that is, boosted jets
  for(unsigned int i=0; i<nAK8Jets; i++){
    allAK8Jets.push_back(new TBoostedJet( (*AK8JetPt)[i], (*AK8JetEta)[i], (*AK8JetPhi)[i], (*AK8JetEnergy)[i], 999, (*AK8JetPruneMass)[i], (*AK8JetSDMass)[i], 999, (*AK8JetTau1)[i],(*AK8JetTau2)[i], (*AK8JetTau3)[i], (*AK8JetNSubjets)[i]));
    int firstsub = (*AK8JetSubjetIndex)[i];
    int nsubs = (*AK8JetNSubjets)[i];
    for(int j = firstsub; j< firstsub + nsubs;j++){
      //if(debug) std::cout<<"making subjet: "<<j<<" out of: "<<(*AK8JetNSubjets)[i]<<" for ak8 jet number: "<<i<<" and total size of subjet vector is: "<<(*subJetPt).size()<<std::endl;
      TJet* subjet = new TJet((*subJetPt)[j],(*subJetEta)[j],(*subJetPhi)[j],(*subJetBDisc)[j],(*subJetDeltaR)[j], (*subJetMass)[j], (*subJetBTag)[j]);
      allAK8Jets.at(i)->AddSubJet(subjet);
    }
  }


  //make cleaned jets - original collection
  for (unsigned int i=0;i<nCleanedAK4Jets; i++){
    if( ( (*cleanedAK4JetPt)[i]<30) || fabs((*cleanedAK4JetEta)[i])>2.4) continue;
    //make floats for scale/smear up/down - set to default and overwrite for MC
    float scaleUpPt=(*cleanedAK4JetPt)[i];
    float scaleDownPt=(*cleanedAK4JetPt)[i];
    float smearUpPt=(*cleanedAK4JetPt)[i];
    float smearDownPt=(*cleanedAK4JetPt)[i];
    if(isMc && latestVersion){
      //add collections for smeared/scaled jets for MC
      scaleUpPt = (*cleanedAK4JetPtScaleUp)[i];
      scaleDownPt = (*cleanedAK4JetPtScaleDown)[i];
      smearUpPt = (*cleanedAK4JetPtSmearUp)[i];
      smearDownPt = (*cleanedAK4JetPtSmearDown)[i];
    }
    cleanedAK4Jets.push_back(new TJet( (*cleanedAK4JetPt)[i], (*cleanedAK4JetEta)[i], (*cleanedAK4JetPhi)[i],(*cleanedAK4JetEnergy)[i],scaleUpPt,scaleDownPt,smearUpPt,smearDownPt) );

  }

  //make cleaned jets - only save if not inside (i.e.dR<0.8 of AK8 jets) - DONE IN TREEMAKER NOW
  for (unsigned int i=0;i<nCleanedAK4Jets; i++){
    if( ( (*cleanedAK4JetPt)[i]<30) || fabs((*cleanedAK4JetEta)[i])>2.4) continue;

    for(unsigned int j=0; j<nAK8Jets; j++){
      //float dR = pow( pow(allAK8Jets.at(j)->eta - (*cleanedAK4JetEta)[i],2) + pow(allAK8Jets.at(j)->phi - (*cleanedAK4JetPhi)[i],2),0.5);
      //if(dR>0.8) newCleanedAK4Jets.push_back(new TJet( (*cleanedAK4JetPt)[i], (*cleanedAK4JetEta)[i], (*cleanedAK4JetPhi)[i],(*cleanedAK4JetEnergy)[i]) );
      newCleanedAK4Jets.push_back(new TJet( (*cleanedAK4JetPt)[i], (*cleanedAK4JetEta)[i], (*cleanedAK4JetPhi)[i],(*cleanedAK4JetEnergy)[i]) );
    }
  }



  if(isMc){
    unsigned int ngenJets = genJetPt->size();
    for (unsigned int i=0;i<ngenJets; i++){
      genJets.push_back(new TJet( (*genJetPt)[i], (*genJetEta)[i], (*genJetPhi)[i],(*genJetEnergy)[i]) );
    }


    //make genparticle collection
    for(unsigned int i=0; i< genPt->size() ; i++){
      genParticles.push_back(new TGenParticle( (*genPt)[i], (*genEta)[i], (*genPhi)[i],(*genEnergy)[i],(*genStatus)[i], (*genId)[i], (*genMotherId)[i], (*genIsPrompt)[i],(*genIsFromPromptTau)[i], (*genPMotherHasC)[i], (*genPMotherHasB)[i], (*genPMother)[i] ));
    }

    //boosted hadronic gets
    for(unsigned int i=0; i<HadronicVHtPt->size(); i++){
      hadronicGenJets.push_back(new THadronicGenJet( (*HadronicVHtPt)[i],(*HadronicVHtEta)[i], (*HadronicVHtPhi)[i], (*HadronicVHtEnergy)[i],(*HadronicVHtID)[i],(*HadronicVHtStatus)[i],(*HadronicVHtD0Pt)[i],(*HadronicVHtD0Eta)[i],(*HadronicVHtD0Phi)[i],(*HadronicVHtD0E)[i],(*HadronicVHtD1Pt)[i],(*HadronicVHtD1Eta)[i],(*HadronicVHtD1Phi)[i],(*HadronicVHtD1E)[i],(*HadronicVHtD2Pt)[i],(*HadronicVHtD2Eta)[i],(*HadronicVHtD2Phi)[i],(*HadronicVHtD2E)[i]) );
    }

  }

  //now from allMuons make goodMuons

  for(unsigned int imu =0; imu<allMuons.size(); imu++){
    if(allMuons.at(imu)->cutBasedTight()) goodMuons.push_back(allMuons.at(imu));
  }
  if(debug) std::cout<<"making loose muons"<<std::endl;
  //make loose muons
    for(unsigned int imu =0; imu<allMuons.size(); imu++){
    if(allMuons.at(imu)->cutBasedLooseMiniIso()) looseMuons.push_back(allMuons.at(imu));
  }

  //now from allElectrons make goodElectrons
  for(unsigned int iel=0; iel< allElectrons.size(); iel++){
    if(allElectrons.at(iel)->mva94XTightV2_90_Iso_RC()) goodElectrons.push_back(allElectrons.at(iel));
  }
  if(debug) std::cout<<"making loose electrons"<<std::endl;
  //now from allElectrons make looseElectrons
  for(unsigned int iel=0; iel< allElectrons.size(); iel++){
    if(allElectrons.at(iel)->mva94XLooseV2_Iso_RC()) looseElectrons.push_back(allElectrons.at(iel));
  }
  //now from allElectrons make cmsdasElectrons
  for(unsigned int iel=0; iel< allElectrons.size(); iel++){
    if(allElectrons.at(iel)->CMSDASTight()) cmsdasElectrons.push_back(allElectrons.at(iel));
  }
  //now from allElectrons make good50nsElectrons
  for(unsigned int iel=0; iel< allElectrons.size(); iel++){
    if(allElectrons.at(iel)->cutBasedTight50ns()) good50nsElectrons.push_back(allElectrons.at(iel));
  }
  //now from allElectrons make loose50nsElectrons
  for(unsigned int iel=0; iel< allElectrons.size(); iel++){
    if(allElectrons.at(iel)->cutBasedLoose50ns()) loose50nsElectrons.push_back(allElectrons.at(iel));
  }




  //make clean jets collection
  for(unsigned int ijet=0; ijet<allAK4Jets.size();ijet++){
    if(debug) std::cout<<"starting to make simple cleaned jet"<<ijet<<std::endl;;
    TJet* jet = allAK4Jets.at(ijet);
    TLorentzVector cleanLV= jet->lv;

    //clean any tight electrons from the jet
    for(unsigned int iel=0; iel<looseElectrons.size(); iel++){
      if(debug) std::cout<<"doing simple cleaning of jet "<<ijet<<" with electron "<<iel<<std::endl;
      TElectron* el = looseElectrons.at(iel);
      float deltaR = pow( pow( jet->eta - el->eta,2) + pow( jet->phi - el->phi,2), 0.5);
      if(deltaR <0.4) cleanLV= cleanLV - el->lv;
    }
    //clean any tight muons from the jet
    for(unsigned int imu=0; imu<looseMuons.size(); imu++){
      if(debug) std::cout<<"doing simple cleaning of jet "<<ijet<<" with muon "<<imu<<std::endl;
      TMuon* mu = looseMuons.at(imu);
      float deltaR = pow( pow( jet->eta - mu->eta,2) + pow( jet->phi - mu->phi,2), 0.5);
      if(deltaR <0.4) cleanLV= cleanLV - mu->lv;
    }
    if(debug) std::cout<<"adding simple cleaned jet"<<std::endl;
    //only add if pt still greater than 30
    if(cleanLV.Pt()>30) simpleCleanedAK4Jets.push_back(new TJet(cleanLV.Pt(),cleanLV.Eta(),cleanLV.Phi(),cleanLV.Energy()));

  }

  if(debug) std::cout<<"finished getting entry"<<std::endl;

  return stat;
}


void TreeReader::Init(TTree *treetemp)
{

  if (!treetemp) return;

  tree=treetemp;

  //intialize vectors to 0 because ROOT

  elD0 = 0;
  elDZ = 0;
  elSIP3d = 0;
  elDeta = 0;
  elDphi = 0;
  elDxy = 0;
  elEnergy = 0;
  elEta = 0;
  elHoE = 0;
  elOoemoop = 0;
  elPhi = 0;
  elPt = 0;
  elchIso = 0;
  elpuIso = 0;
  elneutIso = 0;
  elphotIso = 0;
  elrhoIso = 0;
  elMiniIso = 0;
  elMiniIsoDB = 0;
  elSusyIso = 0;
  elAEff= 0;
  elSihih = 0;
  elChargeConsistent = 0;
  elCharge = 0;
  elGsfCharge = 0;
  elCtfCharge = 0;
  elScPixCharge = 0;

  elIsEBEE = 0;
  elMHits = 0;
  elPassConversionVeto = 0;
  elQuality = 0;
  elVtxFitConv = 0;
  elMVAValue = 0;
  elIsMVATight80 = 0;
  elIsMVATight90 = 0;
  elIsMVALoose = 0;
  elIsMVATightIso80 = 0;
  elIsMVATightIso90 = 0;
  elIsMVALooseIso = 0;
  elIsTight = 0;
  elIsMedium = 0;
  elIsLoose = 0;
  elIsVeto = 0;
  

  //gen particles
  genEnergy = 0;
  genEta = 0;
  genPhi = 0;
  genPt = 0;
  genId = 0;
  genIndex = 0;
  genMotherId = 0;
  genMotherIndex = 0;
  genStatus = 0;
  genIsFromPromptTau = 0;
  genIsPrompt = 0;
  genPMotherHasC = 0;
  genPMotherHasB = 0;
  genPMother = 0;

  LHEWeights = 0;
  LHEWeightIDs = 0;

  //muons
  muChi2 = 0;
  muDxy = 0;
  muDz = 0;
  muSIP3d = 0;
  muEnergy = 0;
  muEta = 0;
  muPhi = 0;
  muPt = 0;
  muRelIso = 0;
  muMiniIso = 0;
  muMiniIsoDB = 0;
  muSusyIso = 0;
  muCharge = 0;
  muGlobal = 0;
  muPF     = 0;
  muTracker =0;
  muNMatchedStations = 0;
  muNTrackerLayers = 0;
  muNValMuHits = 0;
  muNValPixelHits = 0;
  muIsTight = 0;
  muIsLoose = 0;

  //ak4 jets
  AK4JetEnergy = 0;
  AK4JetEta = 0;
  AK4JetPhi = 0;
  AK4JetPt = 0;

  //ak4 jets
  cleanedAK4JetEnergy = 0;
  cleanedAK4JetEta = 0;
  cleanedAK4JetPhi = 0;
  cleanedAK4JetPt = 0;
  cleanedAK4JetPtScaleUp = 0;
  cleanedAK4JetPtScaleDown = 0;
  cleanedAK4JetPtSmearUp = 0;
  cleanedAK4JetPtSmearDown = 0;

  //ak8 jets
  AK8JetEnergy = 0;
  AK8JetEta = 0;
  AK8JetPhi = 0;
  AK8JetPt = 0;
  //AK8JetTrimMass = 0;
  AK8JetPruneMass = 0;
  AK8JetSDMass = 0;
  //AK8JetFiltMass=0;
  AK8JetTau1=0;
  AK8JetTau2=0;
  AK8JetTau3=0;
  AK8JetNSubjets=0;
  AK8JetSubjetIndex=0;

  //subjets
  subJetEta=0;
  subJetPhi=0;
  subJetPt=0;
  subJetBDisc=0;
  subJetDeltaR=0;
  subJetBTag=0;
  subJetMass=0;




  if(isMc){
    //gen jets
    genJetEnergy = 0;
    genJetEta = 0;
    genJetPhi = 0;
    genJetPt = 0;
  }

  //common
  tree->SetBranchAddress("run_CommonCalc",&run,&b_run_CommonCalc);
  tree->SetBranchAddress("lumi_CommonCalc",&lumi,&b_lumi_CommonCalc);
  tree->SetBranchAddress("event_CommonCalc",&event,&b_event_CommonCalc);

  //pileup
  tree->SetBranchAddress("nTrueInteractions_PileUpCalc",&nPU,&b_nTrueInteractions_PileUpCalc);
  tree->SetBranchAddress("nPV_DileptonCalc",&nPrimaryVert,&b_nPV_DileptonCalc);
  //Electrons
  tree->SetBranchAddress("elChargeConsistent_DileptonCalc", &elChargeConsistent, &b_elChargeConsistent_DileptonCalc);
  tree->SetBranchAddress("elCharge_DileptonCalc", &elCharge, &b_elCharge_DileptonCalc);
  tree->SetBranchAddress("elGsfCharge_DileptonCalc", &elGsfCharge, &b_elGsfCharge_DileptonCalc);
  tree->SetBranchAddress("elCtfCharge_DileptonCalc", &elCtfCharge, &b_elCtfCharge_DileptonCalc);
  tree->SetBranchAddress("elScPixCharge_DileptonCalc", &elScPixCharge, &b_elScPixCharge_DileptonCalc);
  tree->SetBranchAddress("elIsEBEE_DileptonCalc", &elIsEBEE, &b_elIsEBEE_DileptonCalc);
  tree->SetBranchAddress("elMHits_DileptonCalc", &elMHits, &b_elMHits_DileptonCalc);
  tree->SetBranchAddress("elVtxFitConv_DileptonCalc", &elPassConversionVeto, &b_elPassConversionVeto_DileptonCalc);
  tree->SetBranchAddress("elQuality_DileptonCalc", &elQuality, &b_elQuality_DileptonCalc);
  tree->SetBranchAddress("elD0_DileptonCalc", &elD0, &b_elD0_DileptonCalc);
  tree->SetBranchAddress("elDZ_DileptonCalc", &elDZ, &b_elDZ_DileptonCalc);
  tree->SetBranchAddress("elSIP3D_DileptonCalc", &elSIP3d, &b_elSIP3d_DileptonCalc);
  tree->SetBranchAddress("elDeta_DileptonCalc", &elDeta, &b_elDeta_DileptonCalc);
  tree->SetBranchAddress("elDphi_DileptonCalc", &elDphi, &b_elDphi_DileptonCalc);
  tree->SetBranchAddress("elDxy_DileptonCalc", &elDxy, &b_elDxy_DileptonCalc);
  tree->SetBranchAddress("elEnergy_DileptonCalc", &elEnergy, &b_elEnergy_DileptonCalc);
  tree->SetBranchAddress("elEta_DileptonCalc", &elEta, &b_elEta_DileptonCalc);
  tree->SetBranchAddress("elHoE_DileptonCalc", &elHoE, &b_elHoE_DileptonCalc);
  tree->SetBranchAddress("elOoemoop_DileptonCalc", &elOoemoop, &b_elOoemoop_DileptonCalc);
  tree->SetBranchAddress("elPhi_DileptonCalc", &elPhi, &b_elPhi_DileptonCalc);
  tree->SetBranchAddress("elPt_DileptonCalc", &elPt, &b_elPt_DileptonCalc);
  tree->SetBranchAddress("elChIso_DileptonCalc", &elchIso, &b_elchIso_DileptonCalc);
  tree->SetBranchAddress("elPUIso_DileptonCalc", &elpuIso, &b_elpuIso_DileptonCalc);
  tree->SetBranchAddress("elNhIso_DileptonCalc", &elneutIso, &b_elneutIso_DileptonCalc);
  tree->SetBranchAddress("elPhIso_DileptonCalc", &elphotIso, &b_elphotIso_DileptonCalc);
  tree->SetBranchAddress("elRhoIso_DileptonCalc", &elrhoIso, &b_elrhoIso_DileptonCalc);
  tree->SetBranchAddress("elMiniIsoEA_DileptonCalc", &elMiniIso, &b_elMiniIso_DileptonCalc);
  tree->SetBranchAddress("elMiniIsoDB_DileptonCalc", &elMiniIsoDB, &b_elMiniIsoDB_DileptonCalc);
  tree->SetBranchAddress("elMiniIsoSUSY_DileptonCalc", &elSusyIso, &b_elSusyIso_DileptonCalc);
  tree->SetBranchAddress("elAEff_DileptonCalc", &elAEff, &b_elAEff_DileptonCalc);
  tree->SetBranchAddress("elSihih_DileptonCalc", &elSihih, &b_elSihih_DileptonCalc);
  tree->SetBranchAddress("elMVAValue_DileptonCalc", &elMVAValue, &b_elMVAValue_DileptonCalc);
  tree->SetBranchAddress("elIsMVATight80_DileptonCalc", &elIsMVATight80, &b_elIsMVATight80_DileptonCalc);
  tree->SetBranchAddress("elIsMVATight90_DileptonCalc", &elIsMVATight90, &b_elIsMVATight90_DileptonCalc);
  tree->SetBranchAddress("elIsMVALoose_DileptonCalc", &elIsMVALoose, &b_elIsMVALoose_DileptonCalc);
  tree->SetBranchAddress("elIsMVATightIso80_DileptonCalc", &elIsMVATightIso80, &b_elIsMVATightIso80_DileptonCalc);
  tree->SetBranchAddress("elIsMVATightIso90_DileptonCalc", &elIsMVATightIso90, &b_elIsMVATightIso90_DileptonCalc);
  tree->SetBranchAddress("elIsMVALooseIso_DileptonCalc", &elIsMVALooseIso, &b_elIsMVALooseIso_DileptonCalc);
  tree->SetBranchAddress("elIsTight_DileptonCalc", &elIsTight, &b_elIsTight_DileptonCalc);
  tree->SetBranchAddress("elIsMedium_DileptonCalc", &elIsMedium, &b_elIsMedium_DileptonCalc);
  tree->SetBranchAddress("elIsLoose_DileptonCalc", &elIsLoose, &b_elIsLoose_DileptonCalc);
  tree->SetBranchAddress("elIsVeto_DileptonCalc", &elIsVeto, &b_elIsVeto_DileptonCalc);


  //Muons
  tree->SetBranchAddress("muCharge_DileptonCalc", &muCharge, &b_muCharge_DileptonCalc);
  tree->SetBranchAddress("muGlobal_DileptonCalc", &muGlobal, &b_muGlobal_DileptonCalc);
  tree->SetBranchAddress("muPF_DileptonCalc", &muPF, &b_muPF_DileptonCalc);
  tree->SetBranchAddress("muTracker_DileptonCalc", &muTracker, &b_muTracker_DileptonCalc);
  tree->SetBranchAddress("muNMatchedStations_DileptonCalc", &muNMatchedStations, &b_muNMatchedStations_DileptonCalc);
  tree->SetBranchAddress("muNTrackerLayers_DileptonCalc", &muNTrackerLayers, &b_muNTrackerLayers_DileptonCalc);
  tree->SetBranchAddress("muNValMuHits_DileptonCalc", &muNValMuHits, &b_muNValMuHits_DileptonCalc);
  tree->SetBranchAddress("muNValPixelHits_DileptonCalc", &muNValPixelHits, &b_muNValPixelHits_DileptonCalc);
  tree->SetBranchAddress("muChi2_DileptonCalc", &muChi2, &b_muChi2_DileptonCalc);
  tree->SetBranchAddress("muDxy_DileptonCalc", &muDxy, &b_muDxy_DileptonCalc);
  tree->SetBranchAddress("muDz_DileptonCalc", &muDz, &b_muDz_DileptonCalc);
  tree->SetBranchAddress("muSIP3D_DileptonCalc", &muSIP3d, &b_muSIP3d_DileptonCalc);
  tree->SetBranchAddress("muEnergy_DileptonCalc", &muEnergy, &b_muEnergy_DileptonCalc);
  tree->SetBranchAddress("muEta_DileptonCalc", &muEta, &b_muEta_DileptonCalc);
  tree->SetBranchAddress("muPhi_DileptonCalc", &muPhi, &b_muPhi_DileptonCalc);
  tree->SetBranchAddress("muPt_DileptonCalc", &muPt, &b_muPt_DileptonCalc);
  tree->SetBranchAddress("muRelIso_DileptonCalc", &muRelIso, &b_muRelIso_DileptonCalc);
  tree->SetBranchAddress("muMiniIsoEA_DileptonCalc", &muMiniIso, &b_muMiniIso_DileptonCalc);
  tree->SetBranchAddress("muMiniIsoDB_DileptonCalc", &muMiniIsoDB, &b_muMiniIsoDB_DileptonCalc);
  tree->SetBranchAddress("muMiniIsoSUSY_DileptonCalc", &muSusyIso, &b_muSusyIso_DileptonCalc);
  tree->SetBranchAddress("muIsTight_DileptonCalc",&muIsTight,&b_muIsTight_DileptonCalc);
  tree->SetBranchAddress("muIsLoose_DileptonCalc",&muIsLoose,&b_muIsLoose_DileptonCalc);

  //ak4 jets
  tree->SetBranchAddress("AK4JetEnergy_DileptonCalc", &AK4JetEnergy, &b_AK4JetEnergy_DileptonCalc);
  tree->SetBranchAddress("AK4JetEta_DileptonCalc", &AK4JetEta, &b_AK4JetEta_DileptonCalc);
  tree->SetBranchAddress("AK4JetPhi_DileptonCalc", &AK4JetPhi, &b_AK4JetPhi_DileptonCalc);
  tree->SetBranchAddress("AK4JetPt_DileptonCalc", &AK4JetPt, &b_AK4JetPt_DileptonCalc);

  //cleaned ak4 jets
  tree->SetBranchAddress("cleanedAK4JetEnergy_DileptonCalc", &cleanedAK4JetEnergy, &b_cleanedAK4JetEnergy_DileptonCalc);
  tree->SetBranchAddress("cleanedAK4JetEta_DileptonCalc", &cleanedAK4JetEta, &b_cleanedAK4JetEta_DileptonCalc);
  tree->SetBranchAddress("cleanedAK4JetPhi_DileptonCalc", &cleanedAK4JetPhi, &b_cleanedAK4JetPhi_DileptonCalc);
  tree->SetBranchAddress("cleanedAK4JetPt_DileptonCalc", &cleanedAK4JetPt, &b_cleanedAK4JetPt_DileptonCalc);
  tree->SetBranchAddress("cleanedAK4JetPtScaleUp_DileptonCalc", &cleanedAK4JetPtScaleUp, &b_cleanedAK4JetPtScaleUp_DileptonCalc);
  tree->SetBranchAddress("cleanedAK4JetPtScaleDown_DileptonCalc", &cleanedAK4JetPtScaleDown, &b_cleanedAK4JetPtScaleDown_DileptonCalc);
  tree->SetBranchAddress("cleanedAK4JetPtSmearUp_DileptonCalc", &cleanedAK4JetPtSmearUp, &b_cleanedAK4JetPtSmearUp_DileptonCalc);
  tree->SetBranchAddress("cleanedAK4JetPtSmearDown_DileptonCalc", &cleanedAK4JetPtSmearDown, &b_cleanedAK4JetPtSmearDown_DileptonCalc);

  //ak8jets
  tree->SetBranchAddress("theJetAK8Energy_JetSubCalc", &AK8JetEnergy, &b_AK8JetEnergy_JetSubCalc);
  tree->SetBranchAddress("theJetAK8Eta_JetSubCalc", &AK8JetEta, &b_AK8JetEta_JetSubCalc);
  tree->SetBranchAddress("theJetAK8Phi_JetSubCalc", &AK8JetPhi, &b_AK8JetPhi_JetSubCalc);
  tree->SetBranchAddress("theJetAK8Pt_JetSubCalc", &AK8JetPt, &b_AK8JetPt_JetSubCalc);
  //tree->SetBranchAddress("theJetAK8TrimmedMass_JetSubCalc", &AK8JetTrimMass, &b_AK8JetTrimMass_JetSubCalc);
  tree->SetBranchAddress("theJetAK8SoftDropMass_JetSubCalc", &AK8JetSDMass, &b_AK8JetSDMass_JetSubCalc);
  tree->SetBranchAddress("theJetAK8PrunedMass_JetSubCalc", &AK8JetPruneMass, &b_AK8JetPruneMass_JetSubCalc);
  //tree->SetBranchAddress("theJetAK8FilteredMass_JetSubCalc", &AK8JetFiltMass, &b_AK8JetFiltMass_JetSubCalc);
  tree->SetBranchAddress("theJetAK8NjettinessTau1_JetSubCalc", &AK8JetTau1, &b_AK8JetTau1_JetSubCalc);
  tree->SetBranchAddress("theJetAK8NjettinessTau2_JetSubCalc", &AK8JetTau2, &b_AK8JetTau2_JetSubCalc);
  tree->SetBranchAddress("theJetAK8NjettinessTau3_JetSubCalc", &AK8JetTau3, &b_AK8JetTau3_JetSubCalc);
  tree->SetBranchAddress("theJetAK8SDSubjetSize_JetSubCalc", &AK8JetNSubjets, &b_AK8JetNSubjets_JetSubCalc);
  tree->SetBranchAddress("theJetAK8SDSubjetIndex_JetSubCalc", &AK8JetSubjetIndex, &b_AK8JetSubjetIndex_JetSubCalc);

  //subjets
  tree->SetBranchAddress("theJetAK8SDSubjetPt_JetSubCalc", &subJetPt, &b_subJetPt_JetSubCalc);
  tree->SetBranchAddress("theJetAK8SDSubjetEta_JetSubCalc", &subJetEta, &b_subJetEta_JetSubCalc);
  tree->SetBranchAddress("theJetAK8SDSubjetPhi_JetSubCalc", &subJetPhi, &b_subJetPhi_JetSubCalc);
  tree->SetBranchAddress("theJetAK8SDSubjetCSV_JetSubCalc", &subJetBDisc, &b_subJetBDisc_JetSubCalc);
  tree->SetBranchAddress("theJetAK8SDSubjetBTag_JetSubCalc", &subJetBTag, &b_subJetBTag_JetSubCalc);
  tree->SetBranchAddress("theJetAK8SDSubjetDR_JetSubCalc", &subJetDeltaR, &b_subJetDeltaR_JetSubCalc);
  tree->SetBranchAddress("theJetAK8SDSubjetMass_JetSubCalc", &subJetMass, &b_subJetMass_JetSubCalc);


  if(isMc){
    //gen jets
    tree->SetBranchAddress("genJetEnergy_DileptonCalc", &genJetEnergy, &b_genJetEnergy_DileptonCalc);
    tree->SetBranchAddress("genJetEta_DileptonCalc", &genJetEta, &b_genJetEta_DileptonCalc);
    tree->SetBranchAddress("genJetPhi_DileptonCalc", &genJetPhi, &b_genJetPhi_DileptonCalc);
    tree->SetBranchAddress("genJetPt_DileptonCalc", &genJetPt, &b_genJetPt_DileptonCalc);

    //Gen Info
    tree->SetBranchAddress("genID_DileptonCalc", &genId, &b_genID_DileptonCalc);
    tree->SetBranchAddress("genIndex_DileptonCalc", &genIndex, &b_genIndex_DileptonCalc);
    tree->SetBranchAddress("genMotherID_DileptonCalc", &genMotherId, &b_genMotherID_DileptonCalc);
    tree->SetBranchAddress("genMotherIndex_DileptonCalc", &genMotherIndex, &b_genMotherIndex_DileptonCalc);
    tree->SetBranchAddress("genStatus_DileptonCalc", &genStatus, &b_genStatus_DileptonCalc);
    tree->SetBranchAddress("genIsFromPromptTau_DileptonCalc",&genIsFromPromptTau,&b_genIsFromPromptTau_DileptonCalc);
    tree->SetBranchAddress("genIsPrompt_DileptonCalc",&genIsPrompt,&b_genIsPrompt_DileptonCalc);
    tree->SetBranchAddress("genPMotherHasC_DileptonCalc",&genPMotherHasC,&b_genPMotherHasC_DileptonCalc);
    tree->SetBranchAddress("genPMotherHasB_DileptonCalc",&genPMotherHasB,&b_genPMotherHasB_DileptonCalc);
    tree->SetBranchAddress("genPMother_DileptonCalc",&genPMother,&b_genPMother_DileptonCalc);

    tree->SetBranchAddress("genEnergy_DileptonCalc", &genEnergy, &b_genEnergy_DileptonCalc);
    tree->SetBranchAddress("genEta_DileptonCalc", &genEta, &b_genEta_DileptonCalc);
    tree->SetBranchAddress("genPhi_DileptonCalc", &genPhi, &b_genPhi_DileptonCalc);
    tree->SetBranchAddress("genPt_DileptonCalc", &genPt, &b_genPt_DileptonCalc);
    //  tree->SetBranchAddress("genCharge_DileptonCalc", &genCharge, &b_genCharge_DileptonCalc);

    //boosted jets
    tree->SetBranchAddress("HadronicVHtPt_JetSubCalc", &HadronicVHtPt, &b_HadronicVHtPt_JetSubCalc);
    tree->SetBranchAddress("HadronicVHtEta_JetSubCalc", &HadronicVHtEta, &b_HadronicVHtEta_JetSubCalc);
    tree->SetBranchAddress("HadronicVHtPhi_JetSubCalc", &HadronicVHtPhi, &b_HadronicVHtPhi_JetSubCalc);
    tree->SetBranchAddress("HadronicVHtEnergy_JetSubCalc", &HadronicVHtEnergy, &b_HadronicVHtEnergy_JetSubCalc);
    tree->SetBranchAddress("HadronicVHtID_JetSubCalc", &HadronicVHtID, &b_HadronicVHtID_JetSubCalc);
    tree->SetBranchAddress("HadronicVHtStatus_JetSubCalc", &HadronicVHtStatus, &b_HadronicVHtStatus_JetSubCalc);
    tree->SetBranchAddress("HadronicVHtD0Pt_JetSubCalc", &HadronicVHtD0Pt, &b_HadronicVHtD0Pt_JetSubCalc);
    tree->SetBranchAddress("HadronicVHtD0Eta_JetSubCalc", &HadronicVHtD0Eta, &b_HadronicVHtD0Eta_JetSubCalc);
    tree->SetBranchAddress("HadronicVHtD0Phi_JetSubCalc", &HadronicVHtD0Phi, &b_HadronicVHtD0Phi_JetSubCalc);
    tree->SetBranchAddress("HadronicVHtD0E_JetSubCalc", &HadronicVHtD0E, &b_HadronicVHtD0E_JetSubCalc);
    tree->SetBranchAddress("HadronicVHtD1Pt_JetSubCalc", &HadronicVHtD1Pt, &b_HadronicVHtD1Pt_JetSubCalc);
    tree->SetBranchAddress("HadronicVHtD1Eta_JetSubCalc", &HadronicVHtD1Eta, &b_HadronicVHtD1Eta_JetSubCalc);
    tree->SetBranchAddress("HadronicVHtD1Phi_JetSubCalc", &HadronicVHtD1Phi, &b_HadronicVHtD1Phi_JetSubCalc);
    tree->SetBranchAddress("HadronicVHtD1E_JetSubCalc", &HadronicVHtD1E, &b_HadronicVHtD1E_JetSubCalc);
    tree->SetBranchAddress("HadronicVHtD2Pt_JetSubCalc", &HadronicVHtD2Pt, &b_HadronicVHtD2Pt_JetSubCalc);
    tree->SetBranchAddress("HadronicVHtD2Eta_JetSubCalc", &HadronicVHtD2Eta, &b_HadronicVHtD2Eta_JetSubCalc);
    tree->SetBranchAddress("HadronicVHtD2Phi_JetSubCalc", &HadronicVHtD2Phi, &b_HadronicVHtD2Phi_JetSubCalc);
    tree->SetBranchAddress("HadronicVHtD2E_JetSubCalc", &HadronicVHtD2E, &b_HadronicVHtD2E_JetSubCalc);


  }

  //met
  tree->SetBranchAddress("met_DileptonCalc", &MET, &b_MET_DileptonCalc);
  tree->SetBranchAddress("met_phi_DileptonCalc", &MET_phi, &b_MET_phi_DileptonCalc);

  //MC weight info
  tree->SetBranchAddress("MCWeight_DileptonCalc",&MCWeight,&b_MCWeight_DileptonCalc);
  tree->SetBranchAddress("LHEWeights_DileptonCalc",&LHEWeights,&b_LHEWeights_DileptonCalc);
  tree->SetBranchAddress("LHEWeightIDs_DileptonCalc",&LHEWeightIDs,&b_LHEWeightIDs_DileptonCalc);


  //trigger info

  //double electron
  tree->SetBranchAddress("HLT_Ele27_Ele37_DileptonCalc", &HLT_Ele27_Ele37,&b_HLT_Ele27_Ele37_DileptonCalc);
  tree->SetBranchAddress("HLT_DoubleEle25_DileptonCalc",&HLT_DoubleEle25,&b_HLT_DoubleEle25_DileptonCalc);
  tree->SetBranchAddress("HLT_DoubleEle27_DileptonCalc",&HLT_DoubleEle27,&b_HLT_DoubleEle27_DileptonCalc);
  tree->SetBranchAddress("HLT_DoubleEle33_DileptonCalc",&HLT_DoubleEle33,&b_HLT_DoubleEle33_DileptonCalc);

  tree->SetBranchAddress("HLT_DoubleEle24_eta2p1_WPTight_Gsf_v_DileptonCalc", &HLT_DoubleEle24_eta2p1_WPTight_Gsf_v,&b_HLT_DoubleEle24_eta2p1_WPTight_Gsf_v_DileptonCalc);
  tree->SetBranchAddress("HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_DZ_PFHT350_v_DileptonCalc", &HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_DZ_PFHT350_v,&b_HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_DZ_PFHT350_v_DileptonCalc);
  tree->SetBranchAddress("HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT350_v_DileptonCalc", &HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT350_v,&b_HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT350_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v_DileptonCalc", &HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v,&b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v_DileptonCalc", &HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v,&b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v_DileptonCalc);

  //single electron
  tree->SetBranchAddress("HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v_DileptonCalc", &HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v,&b_HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v_DileptonCalc", &HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v,&b_HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v_DileptonCalc", &HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v,&b_HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v_DileptonCalc);

  tree->SetBranchAddress("HLT_Ele20_WPLoose_Gsf_v_DileptonCalc", &HLT_Ele20_WPLoose_Gsf_v,&b_HLT_Ele20_WPLoose_Gsf_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Ele20_eta2p1_WPLoose_Gsf_v_DileptonCalc", &HLT_Ele20_eta2p1_WPLoose_Gsf_v,&b_HLT_Ele20_eta2p1_WPLoose_Gsf_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Ele27_WPTight_Gsf_v_DileptonCalc", &HLT_Ele27_WPTight_Gsf_v,&b_HLT_Ele27_WPTight_Gsf_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Ele32_WPTight_Gsf_v_DileptonCalc", &HLT_Ele32_WPTight_Gsf_v,&b_HLT_Ele32_WPTight_Gsf_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Ele35_WPTight_Gsf_v_DileptonCalc", &HLT_Ele35_WPTight_Gsf_v,&b_HLT_Ele35_WPTight_Gsf_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Ele35_WPTight_Gsf_L1EGMT_v_DileptonCalc", &HLT_Ele35_WPTight_Gsf_L1EGMT_v,&b_HLT_Ele35_WPTight_Gsf_L1EGMT_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Ele38_WPTight_Gsf_v_DileptonCalc", &HLT_Ele38_WPTight_Gsf_v,&b_HLT_Ele38_WPTight_Gsf_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Ele40_WPTight_Gsf_v_DileptonCalc", &HLT_Ele40_WPTight_Gsf_v,&b_HLT_Ele40_WPTight_Gsf_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Ele32_WPTight_Gsf_L1DoubleEG_v_DileptonCalc", &HLT_Ele32_WPTight_Gsf_L1DoubleEG_v,&b_HLT_Ele32_WPTight_Gsf_L1DoubleEG_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned_v_DileptonCalc", &HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned_v,&b_HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Ele28_eta2p1_WPTight_Gsf_HT150_v_DileptonCalc", &HLT_Ele28_eta2p1_WPTight_Gsf_HT150_v,&b_HLT_Ele28_eta2p1_WPTight_Gsf_HT150_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Ele28_HighEta_SC20_Mass55_v_DileptonCalc", &HLT_Ele28_HighEta_SC20_Mass55_v,&b_HLT_Ele28_HighEta_SC20_Mass55_v_DileptonCalc);

  tree->SetBranchAddress("HLT_Ele8_CaloIdM_TrackIdM_PFJet30_v_DileptonCalc", &HLT_Ele8_CaloIdM_TrackIdM_PFJet30_v,&b_HLT_Ele8_CaloIdM_TrackIdM_PFJet30_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v_DileptonCalc", &HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v,&b_HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Ele23_CaloIdM_TrackIdM_PFJet30_v_DileptonCalc", &HLT_Ele23_CaloIdM_TrackIdM_PFJet30_v,&b_HLT_Ele23_CaloIdM_TrackIdM_PFJet30_v_DileptonCalc);


  //double muon
  tree->SetBranchAddress("HLT_Mu37_TkMu27_DileptonCalc",&HLT_Mu37_TkMu27,&b_HLT_Mu37_TkMu27_DileptonCalc);
  tree->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_DileptonCalc", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8,&b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_DileptonCalc);
  tree->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_DileptonCalc", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8,&b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_DileptonCalc);
  tree->SetBranchAddress("HLT_DoubleMu8_Mass8_PFHT350_DileptonCalc",&HLT_DoubleMu8_Mass8_PFHT350,&b_HLT_DoubleMu8_Mass8_PFHT350_DileptonCalc);
  tree->SetBranchAddress("HLT_DoubleMu4_Mass8_DZ_PFHT350_DileptonCalc",&HLT_DoubleMu4_Mass8_DZ_PFHT350,&b_HLT_DoubleMu4_Mass8_DZ_PFHT350_DileptonCalc);

  //single muon
  tree->SetBranchAddress("HLT_Mu8_DileptonCalc", &HLT_Mu8,&b_HLT_Mu8_DileptonCalc);
  tree->SetBranchAddress("HLT_Mu17_DileptonCalc", &HLT_Mu17,&b_HLT_Mu17_DileptonCalc);
  tree->SetBranchAddress("HLT_Mu19_DileptonCalc", &HLT_Mu19,&b_HLT_Mu19_DileptonCalc);
  tree->SetBranchAddress("HLT_Mu20_DileptonCalc", &HLT_Mu20,&b_HLT_Mu20_DileptonCalc);
  tree->SetBranchAddress("HLT_Mu27_DileptonCalc", &HLT_Mu27,&b_HLT_Mu27_DileptonCalc);
  tree->SetBranchAddress("HLT_Mu8_TrkIsoVVL_DileptonCalc", &HLT_Mu8_TrkIsoVVL,&b_HLT_Mu8_TrkIsoVVL_DileptonCalc);
  tree->SetBranchAddress("HLT_Mu17_TrkIsoVVL_DileptonCalc", &HLT_Mu17_TrkIsoVVL,&b_HLT_Mu17_TrkIsoVVL_DileptonCalc);
  tree->SetBranchAddress("HLT_Mu19_TrkIsoVVL_DileptonCalc", &HLT_Mu19_TrkIsoVVL,&b_HLT_Mu19_TrkIsoVVL_DileptonCalc);
  tree->SetBranchAddress("HLT_Mu3_PFJet40_DileptonCalc", &HLT_Mu3_PFJet40,&b_HLT_Mu3_PFJet40_DileptonCalc);

  //cross triggers
  tree->SetBranchAddress("HLT_Mu37Ele27_DileptonCalc",&HLT_Mu37Ele27,&b_HLT_Mu37Ele27_DileptonCalc);
  tree->SetBranchAddress("HLT_Mu27Ele37_DileptonCalc",&HLT_Mu27Ele37,&b_HLT_Mu27Ele37_DileptonCalc);

  tree->SetBranchAddress("HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT350_DZ_v_DileptonCalc", &HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT350_DZ_v,&b_HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT350_DZ_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v_DileptonCalc", &HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v,&b_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT350_v_DileptonCalc", &HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT350_v,&b_HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT350_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v_DileptonCalc", &HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v,&b_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v_DileptonCalc", &HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v,&b_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v_DileptonCalc", &HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v,&b_HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v_DileptonCalc", &HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v,&b_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v_DileptonCalc);
  tree->SetBranchAddress("HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v_DileptonCalc", &HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v,&b_HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v_DileptonCalc);

  //hadronic triggers
  tree->SetBranchAddress("HLT_PFHT900_DileptonCalc",&HLT_PFHT900,&b_HLT_PFHT900_DileptonCalc);
  tree->SetBranchAddress("HLT_AK8PFJet360TrimMass30_DileptonCalc",&HLT_AK8PFJet360TrimMass30,&b_HLT_AK8PFJet360TrimMass30_DileptonCalc);

  //TT sig decay info
  tree->SetBranchAddress("isTZTZ_TpTpCalc",&isTZTZ,&b_isTZTZ_TpTpCalc);
  tree->SetBranchAddress("isTZTH_TpTpCalc",&isTZTH,&b_isTZTH_TpTpCalc);
  tree->SetBranchAddress("isTZBW_TpTpCalc",&isTZBW,&b_isTZBW_TpTpCalc);
  tree->SetBranchAddress("isTHTH_TpTpCalc",&isTHTH,&b_isTHTH_TpTpCalc);
  tree->SetBranchAddress("isTHBW_TpTpCalc",&isTHBW,&b_isTHBW_TpTpCalc);
  tree->SetBranchAddress("isBWBW_TpTpCalc",&isBWBW,&b_isBWBW_TpTpCalc);

  //BB sig decay info
  tree->SetBranchAddress("isBZBZ_TpTpCalc",&isBZBZ,&b_isBZBZ_TpTpCalc);
  tree->SetBranchAddress("isBZBH_TpTpCalc",&isBZBH,&b_isBZBH_TpTpCalc);
  tree->SetBranchAddress("isBZTW_TpTpCalc",&isBZTW,&b_isBZTW_TpTpCalc);
  tree->SetBranchAddress("isBHBH_TpTpCalc",&isBHBH,&b_isBHBH_TpTpCalc);
  tree->SetBranchAddress("isBHTW_TpTpCalc",&isBHTW,&b_isBHTW_TpTpCalc);
  tree->SetBranchAddress("isTWTW_TpTpCalc",&isTWTW,&b_isTWTW_TpTpCalc);


}
