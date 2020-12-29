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
//#include "../plugins/Macros.cc"

bool debug = false;

//helper functions
std::vector<TLepton*> makeLeptons(std::vector<TElectron*> electrons, std::string ID);
bool matchToGenLep(TElectron* el, std::vector<TGenParticle*> genParticles);
TGenParticle* getMatchedGenParticle(TElectron* el, std::vector<TGenParticle*> genParticles);
//this script will produce histograms for charge misID for ttjets and DYjetss when ignoring the Z window and just matching tight electrons to 

int main(int argc, char* argv[]){

  //read in ID
  if(argc!=3){
    std::cout<<"Need to specify sample and electron ID. Exiting..."<<std::endl;
    return 0;
  }
  std::string sample(argv[1]);
  std::string elID(argv[2]);
  bool mc = true;
  //make treereader
  std::string filename;
  if(sample=="DYM50") filename = "/eos/uscms/store/user/wywong/FWLJMET102X_2lepSyst2017_wywong_082020_hadds/DYM50.root";
  else if (sample=="TTTo2L2Nu") filename = "/eos/uscms/store/user/wywong/FWLJMET102X_2lepSyst2017_wywong_082020_hadds/TTTo2L2Nu.root";
  else if (sample=="TTToSemiLeptonic") filename = "/eos/uscms/store/user/wywong/FWLJMET102X_2lepSyst2017_wywong_082020_hadds/TTToSemiLeptonic.root";
  else if (sample=="TTTo2L2Nu_FR") filename = "/eos/uscms/store/group/lpcljm/FWLJMET102X_2lepFakeRate2017_wywong_082020_hadds/TTTo2L2Nu.root";
  else if (sample=="TTToSemiLeptonic_FR") filename = "/eos/uscms/store/group/lpcljm/FWLJMET102X_2lepFakeRate2017_wywong_082020_hadds/TTToSemiLeptonic.root";
  else if (sample=="DYM50_FR") filename = "/eos/uscms/store/group/lpcljm/FWLJMET102X_2lepFakeRate2017_wywong_082020_hadds/DYM50.root";
  else {std::cout<<"You didn't pick a valid sample! Exiting..."<<std::endl; return 0;}

  TreeReader* tr;
  tr =  new TreeReader(filename.c_str(),"ljmet/ljmet",mc,true);
  TTree* t = tr->tree;

  //make outfile
  std::string outname = "Systematics_ChargeMisID_"+sample+".root";
  TFile* fout= new TFile(outname.c_str(),"RECREATE");

  float ptbins[4] = {20.,100.,200.,600.};
  unsigned int npar_pt = 3;//sizeof(ptbins) / sizeof(float) -1;
  float elEtabins[7]={0.0,0.4,0.8,1.442,1.556,2.0,2.4};
  unsigned int npar_eta = 6; //sizeof(elEtabins) / sizeof(float) -1;

  if(debug) std::cout<< "define hist"<< std::endl;
  //histos eta all
  TH1F* etaNumHist = new TH1F("etaNumHist","#eta of electrons",npar_eta, elEtabins);
  TH1F* etaDenHist = new TH1F("etaDenHist","#eta of electrons",npar_eta, elEtabins);
  //histos eta low pt
  TH1F* etaNumHist_lpt = new TH1F("etaNumHist_lpt","#eta of electrons",npar_eta, elEtabins);
  TH1F* etaDenHist_lpt = new TH1F("etaDenHist_lpt","#eta of electrons",npar_eta, elEtabins);
  //histos for high pt eta
  TH1F* etaNumHist_hpt = new TH1F("etaNumHist_hpt","#eta of electrons",npar_eta, elEtabins);
  TH1F* etaDenHist_hpt = new TH1F("etaDenHist_hpt","#eta of electrons",npar_eta, elEtabins);
  //histos for high high pt eta
  TH1F* etaNumHist_hhpt = new TH1F("etaNumHist_hhpt","#eta of electrons",npar_eta, elEtabins);
  TH1F* etaDenHist_hhpt = new TH1F("etaDenHist_hhpt","#eta of electrons",npar_eta, elEtabins);

  TH1F* ptNumHist = new TH1F("ptNumHist","p_{T} of electrons",npar_pt, ptbins);
  TH1F* ptDenHist = new TH1F("ptDenHist","p_{T} of electrons",npar_pt, ptbins);

  TH1F* etaCMID_lpt = new TH1F("etaCMID_lpt","#eta of electrons",npar_eta, elEtabins);
  TH1F* etaCMID_hpt = new TH1F("etaCMID_hpt","#eta of electrons",npar_eta, elEtabins);
  TH1F* etaCMID_hhpt = new TH1F("etaCMID_hhpt","#eta of electrons",npar_eta, elEtabins);
  

  if(debug) std::cout<< "GetEntries"<< std::endl;
  int nEntries = t->GetEntries();

  if (debug) std::cout<< "Loop Events "<< nEntries << std::endl;
  for( int ient=0; ient < nEntries; ient++){
    if(debug) std::cout<< ient << std::endl;
    tr->GetEntry(ient);
    if(debug) std::cout<< "Get Event"<< std::endl;
    
    if(ient % 100000 ==0) std::cout<<"Completed "<<ient<<" out of "<<nEntries<<" events"<<std::endl;

    //load in electrons
    std::vector<TElectron*> electrons = tr->allElectrons;
    
    for(std::vector<TElectron*>::size_type i =0; i < electrons.size(); i++){
      TElectron* iel = electrons.at(i);

      //skip if not tight
      bool tight = false;
      if(elID=="MVA2017TightV2IsoTightRC"){
	if(iel->mva94XTightV2_90_IsoTight_RC()) tight = true;
      }
      if(!tight) continue;
      if(debug) std::cout<< "Passed Tight el"<< std::endl;      

      //leptons pT above 30 GeV only
      if(iel->pt < 30 || fabs(iel->eta) > 2.4) continue;

      // njets > 4 for event topology?????
 
      //check ability to match to gen lepton
      bool matched = matchToGenLep(iel, tr->genParticles);
      if(!matched) continue;
      if(debug) std::cout<< "GenMatched"<< std::endl;
      //get matching gen particle
      TGenParticle* gp = getMatchedGenParticle(iel,tr->genParticles);
      //skip if genParticle not close or prompt
      float drtemp = pow( pow( iel->eta - gp->eta, 2 ) + pow( iel->phi - gp->phi, 2), 0.5);
      if(drtemp > 0.3 || !(gp->isPrompt)) continue;
      if(debug) std::cout<< "dR0p3 + Prompt"<< std::endl;
      //fill denominator hists
      etaDenHist->Fill(fabs(iel->eta),tr->MCWeight);
      ptDenHist->Fill(iel->pt,tr->MCWeight);
      if(debug) std::cout<< Form("pT %.1f eta %.3f MCWeight %.2f", iel->pt, fabs(iel->eta), tr->MCWeight) << std::endl;
      //fill numerator if charges aren't equal
      if(iel->charge != gp->charge){
	etaNumHist->Fill(fabs(iel->eta),tr->MCWeight);
	ptNumHist->Fill(iel->pt,tr->MCWeight);
        if(iel->pt >200) etaNumHist_hhpt->Fill(fabs(iel->eta),tr->MCWeight);
	else if(iel->pt >100) etaNumHist_hpt->Fill(fabs(iel->eta),tr->MCWeight);
       	else 	         etaNumHist_lpt->Fill(fabs(iel->eta),tr->MCWeight);
      }
      if(iel->pt >200) etaDenHist_hhpt->Fill(fabs(iel->eta),tr->MCWeight);
      else if(iel->pt >100) etaDenHist_hpt->Fill(fabs(iel->eta),tr->MCWeight);
      else 	       etaDenHist_lpt->Fill(fabs(iel->eta),tr->MCWeight);
      
    }//end loop over electrons
  }// end event loop

  etaCMID_lpt = (TH1F*) etaNumHist_lpt->Clone("etaCMID_lpt");
  etaCMID_lpt->Sumw2();
  etaCMID_lpt->Divide(etaDenHist_lpt);
  etaCMID_hpt = (TH1F*) etaNumHist_hpt->Clone("etaCMID_hpt");
  etaCMID_hpt->Sumw2();
  etaCMID_hpt->Divide(etaDenHist_hpt);
  etaCMID_hhpt = (TH1F*) etaNumHist_hhpt->Clone("etaCMID_hhpt");
  etaCMID_hhpt->Sumw2();
  etaCMID_hhpt->Divide(etaDenHist_hhpt);
  std::cout<< "////////// from "<< sample << "//////////"<<std::endl;
  for(unsigned int i = 1; i<npar_eta+1; i++){ std::cout<< Form("pt_le100.00_eta_le%.3f %.2E+/-%.2E", etaCMID_lpt->GetXaxis()->GetBinUpEdge(i),etaCMID_lpt->GetBinContent(i), etaCMID_lpt->GetBinError(i))<<std::endl;}
  for(unsigned int i = 1; i<npar_eta+1; i++){ std::cout<< Form("pt_le200.00_eta_le%.3f %.2E+/-%.2E", etaCMID_hpt->GetXaxis()->GetBinUpEdge(i),etaCMID_hpt->GetBinContent(i), etaCMID_hpt->GetBinError(i))<<std::endl;}
  for(unsigned int i = 1; i<npar_eta+1; i++){ std::cout<< Form("pt_le600.00_eta_le%.3f %.2E+/-%.2E", etaCMID_hhpt->GetXaxis()->GetBinUpEdge(i),etaCMID_hhpt->GetBinContent(i), etaCMID_hhpt->GetBinError(i))<<std::endl;}
  std::cout<<  Form("All Lepton Ration = %f/%f = %.2E",etaNumHist->Integral(),etaDenHist->Integral(),etaNumHist->Integral()/etaDenHist->Integral())<<std::endl;
  //write output file
  fout->Write();

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
      if (dR<0.1) break;
    }
  }

  //now we have a min deltaR between our electron and all electrons from hard scattering, if it is less than 0.15 let's consider it matched
  if(dR <0.1) return true;
  else return false;
  
}
    
TGenParticle* getMatchedGenParticle(TElectron* el, std::vector<TGenParticle*> genParticles){


  TGenParticle* gp=0;


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
      gp=genParticle;
    }
  }

  return gp;
}

