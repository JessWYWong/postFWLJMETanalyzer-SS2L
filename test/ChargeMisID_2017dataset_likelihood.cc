#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TH2.h"
#include <vector>
#include "TLorentzVector.h"
#include "TChain.h"
#include "../interface/TreeReader.h"
#include <stdio.h>
#include <assert.h>
#include <map>
#include <string>
#include <sstream>
#include "../plugins/Macros.cc"
#include <memory>
#include <fstream>

//helper functions
std::vector<TLepton*> makeLeptons(std::vector<TElectron*> electrons, bool mc, bool FiftyNs, std::string ID);

//A script to get the prompt rate for electrons and muons. Usage is ./ChargeMisID.o <Data,MC> <El,Mu>

int main(int argc, char* argv[]){

  if(argc!=4){
    std::cout<<"Need to specify whether running on Data or MC and whether 25 or 50ns as well as the ID. The four possible ways of running are\n"
             <<"./ChargeMisID.o Data 50ns ID \n"
             <<"./ChargeMisID.o Data 25ns ID \n"
             <<"./ChargeMisID.o MC 50ns ID \n"
             <<"./ChargeMisID.o MC 25ns ID \n";
    return 0;
  }

  std::string argv1 = argv[1];
  std::string argv2 = argv[2];
  std::string ID = argv[3];

  bool correctusage=false;
  if(argc==4 && ( (argv1.find("Data")!=std::string::npos && argv2.find("2017")!=std::string::npos) || argv1.find("MC")!=std::string::npos )) correctusage=true;
  if(!correctusage){
    std::cout<<"Need to specify whether running on Data or MC and 25 or 50ns. The four possible ways of running are\n"
             <<"./ChargeMisID.o Data 50ns ID \n"
             <<"./ChargeMisID.o Data 25ns ID \n"
             <<"./ChargeMisID.o MC 50ns ID \n"
             <<"./ChargeMisID.o MC 25ns ID \n";
    return 0;
  }

  //get filename based on Data/MC
  std::string filename;
  bool data=true;
  bool FiftyNS=true;

  if(argv1=="Data" && argv2=="50ns") {filename="root://cmseos.fnal.gov//store/user/clint/Run2015B/ljmet_trees/ljmet_Data_ElEl.root"; data=true; FiftyNS=true;}
  else if(argv1=="Data"){
      data=true; FiftyNS=false;

      // Input file folder
      //std::string filedir = "user/wywong/FWLJMET102X_2lep2017_wywong_012020_hadds";
      std::string filedir = "group/lpcljm/FWLJMET102X_2lep2017_wywong_082020_hadds";

      // Input files
      //if(argv2=="2017B") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleEGRun2017B.root";
      //else if(argv2=="2017C") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleEGRun2017C.root";
      //else if(argv2=="2017D") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleEGRun2017D.root";
      //else if(argv2=="2017E") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleEGRun2017E.root";
      //else if(argv2=="2017F") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleEGRun2017F.root";
      //else if(argv2=="2017F_TEST") filename="root://cmseos.fnal.gov//store/group/lpcljm/"+filedir+"/DoubleEGRun2017F_TEST.root";
      if(argv2=="2017B") filename="root://cmseos.fnal.gov//store/"+filedir+"/DoubleEGRun2017B.root";
      else if(argv2=="2017C") filename="root://cmseos.fnal.gov//store/"+filedir+"/DoubleEGRun2017C.root";
      else if(argv2=="2017D") filename="root://cmseos.fnal.gov//store/"+filedir+"/DoubleEGRun2017D.root";
      else if(argv2=="2017E") filename="root://cmseos.fnal.gov//store/"+filedir+"/DoubleEGRun2017E.root";
      else if(argv2=="2017F") filename="root://cmseos.fnal.gov//store/"+filedir+"/DoubleEGRun2017F.root";
  }
  //else if(argv1=="MC" && argv2=="50ns") {filename="root://cmseos.fnal.gov//store/user/clint/PHYS14/50ns/ljmet_trees/ljmet_DYJets.root"; data=false; FiftyNS=true;}
  //else if(argv1=="MC" && argv2=="25ns") {filename="root://cmseos.fnal.gov//store/user/lpctlbsm/clint/Spring16/25ns/Feb16/ljmet_trees/ljmet_DYJets.root"; data=false; FiftyNS=false;}
  else if(argv1=="MC"){
      data=false; FiftyNS=false;
      std::string filedir = "user/wywong/FWLJMET102X_2lepSyst2017_wywong_082020_hadds";
      if(argv2=="DYM50") {filename="root://cmseos.fnal.gov//store/"+filedir+"/DYM50.root";}
  }
  else{
    std::cout<<"Need to specify whether running on Data or MC and 25 or 50ns. The four possible ways of running are\n"
             <<"./ChargeMisID.o Data 50ns \n"
             <<"./ChargeMisID.o Data 25ns \n"
             <<"./ChargeMisID.o MC 50ns \n"
             <<"./ChargeMisID.o MC 25ns \n";
  }

  //make output folder
  TString outdir = "ChargeMisID_likelihood_082020";
  system("mkdir -pv "+outdir);


  //make filename for output root file
  TString outname;
  if(data)outname=outdir+"/"+"ChargeMisID_likelihood_Data_"+argv2+"_Electrons_"+ID+".root";
  else outname=outdir+"/"+"ChargeMisID_likelihood_MC_"+argv2+"_Electrons_"+ID+".root";

  if(argv1=="MC") std::cout<< "outname : "<< outname<< endl;

  //open output file
  TFile* fout= new TFile(outname,"RECREATE");

  //get tree reader to read in data
  TreeReader* tr =  new TreeReader(filename.c_str(),"ljmet/ljmet",false,true);
  TTree* t=tr->tree;

  //now change back to output file
  fout->cd();
  gDirectory->pwd();

  //initialize needed histograms
  float ptbins[13] = {20,30,40,50,70,100,130,160,200,250,300,400,600};
  Int_t npar_pt = sizeof(ptbins) / sizeof(float) -1;
  float elEtabins[8]={-2.4,-1.57,-0.8,-0.4,0.4,0.8,1.57,2.4};
  Int_t npar_eta = sizeof(elEtabins) / sizeof(float) -1;
  TH2F* NumHist_all = new TH2F("NumHist_all", "p_{T} to eta of Same Sign Events",npar_eta,elEtabins,npar_pt,ptbins);
  TH2F* NumHist_lep1 = new TH2F("NumHist_lep1", "p_{T} to eta of Same Sign Events",npar_eta,elEtabins,npar_pt,ptbins);
  TH2F* NumHist_lep2 = new TH2F("NumHist_lep2", "p_{T} to eta of Same Sign Events",npar_eta,elEtabins,npar_pt,ptbins);
  TH2F* DenHist_all = new TH2F("DenHist_all", "p_{T} to eta of All Events",npar_eta,elEtabins,npar_pt,ptbins);
  TH2F* DenHist_lep1 = new TH2F("DenHist_lep1", "p_{T} to eta of All Events",npar_eta,elEtabins,npar_pt,ptbins);
  TH2F* DenHist_lep2 = new TH2F("DenHist_lep2", "p_{T} to eta of All Events",npar_eta,elEtabins,npar_pt,ptbins);
  TH2F * num_lep12_eta = new TH2F("num_lep12_eta","num_lep12_eta", npar_eta, elEtabins, npar_eta, elEtabins);
  TH2F * num_lep12_pt = new TH2F("num_lep12_pt","num_lep12_pt", npar_pt, ptbins, npar_pt, ptbins);
  TH2F * den_lep12_eta = new TH2F("den_lep12_eta","den_lep12_eta", npar_eta, elEtabins, npar_eta, elEtabins);
  TH2F * den_lep12_pt = new TH2F("den_lep12_pt","den_lep12_pt", npar_pt, ptbins, npar_pt, ptbins);

  //output tree
  TTree* outTree = new TTree("ChargeMisID","ChargeMisID");
  float Lep1Pt_,Lep1Eta_,Lep1Phi_,Lep1E_,Lep2Pt_,Lep2Eta_,Lep2Phi_,Lep2E_;
  int Lep1Charge_,Lep2Charge_;
  outTree->Branch("Lep1Pt",&Lep1Pt_);
  outTree->Branch("Lep1Eta",&Lep1Eta_);
  outTree->Branch("Lep1Phi",&Lep1Phi_);
  outTree->Branch("Lep1E",&Lep1E_);
  outTree->Branch("Lep1Charge",&Lep1Charge_);
  outTree->Branch("Lep2Pt",&Lep2Pt_);
  outTree->Branch("Lep2Eta",&Lep2Eta_);
  outTree->Branch("Lep2Phi",&Lep2Phi_);
  outTree->Branch("Lep2E",&Lep2E_);
  outTree->Branch("Lep2Charge",&Lep2Charge_);

  //get number of entries and start event loop
  int nEntries = t->GetEntries();
  for(int ient=0; ient<nEntries; ient++){

    //if (ient==1000) break;

    tr->GetEntry(ient);
    float weight;
    if(data) weight =1.0;
    else weight = tr->MCWeight >=0 ? 1.0 : -1.0;
    if(ient % 100000 ==0) std::cout<<"Completed "<<ient<<" out of "<<nEntries<<" events"<<std::endl;

    //make vector of leptons
    std::vector<TLepton*> leptons = makeLeptons(tr->allElectrons,!data,FiftyNS,ID);

    //skip if didn't find at least two tight leptons
    if(leptons.size()<2) continue;

    //get pair of leptons closest to z mass;
    float zmass = 91.1;
    float massDiff=9999;
    TLepton* lep1=0;
    TLepton* lep2=0;
    float pairmass=-9999;
    for(std::vector<TLepton*>::size_type ilep=0; ilep<leptons.size(); ilep++){
      //loop over remaining leptons
      for(std::vector<TLepton*>::size_type jlep=ilep+1; jlep<leptons.size(); jlep++){
          pairmass = (leptons.at(ilep)->lv + leptons.at(jlep)->lv).M();
          if(fabs(zmass-pairmass)<massDiff){
             massDiff = zmass-pairmass;
             lep1=leptons.at(ilep);
             lep2=leptons.at(jlep);
          }
      }//end second loop over leptons
    }//end loop over leptons

    //check that leptons are in Zpeak
    bool zpeak= fabs(massDiff)<10 ? true : false;

    if(!zpeak) continue;
    if( (fabs(lep1->eta)> 1.44 && fabs(lep1->eta) < 1.56) || (fabs(lep2->eta)> 1.44 && fabs(lep2->eta) < 1.56) ) continue;

    //fill tree
    Lep1Pt_=lep1->pt;
    Lep1Eta_=lep1->eta;
    Lep1Phi_=lep1->phi;
    Lep1E_=lep1->energy;
    Lep1Charge_=lep1->charge;
    Lep2Pt_=lep2->pt;
    Lep2Eta_=lep2->eta;
    Lep2Phi_=lep2->phi;
    Lep2E_=lep2->energy;
    Lep2Charge_=lep2->charge;

    outTree->Fill();

    //now fill histograms
    if(lep1->charge == lep2->charge){
      NumHist_lep1->Fill(lep1->eta,lep1->pt);
      NumHist_lep2->Fill(lep2->eta,lep2->pt);
      NumHist_all->Fill(lep1->eta,lep1->pt);
      NumHist_all->Fill(lep2->eta,lep2->pt);
      num_lep12_eta->Fill(lep1->eta,lep2->eta);
      num_lep12_pt->Fill(lep1->pt,lep2->pt);
    }

    //fill denominator
    DenHist_lep1->Fill(lep1->eta,lep1->pt);
    DenHist_lep2->Fill(lep2->eta,lep2->pt);
    DenHist_all->Fill(lep1->eta,lep1->pt);
    DenHist_all->Fill(lep2->eta,lep2->pt);
    den_lep12_eta->Fill(lep1->eta,lep2->eta);
    den_lep12_pt->Fill(lep1->pt,lep2->pt);

  }//end event loop

  NumHist_all->Write("NumHist_all");
  DenHist_all->Write("DenHist_all");
  NumHist_lep1->Write("NumHist_lep1");
  NumHist_lep2->Write("NumHist_lep2");
  DenHist_lep1->Write("DenHist_lep1");
  DenHist_lep2->Write("DenHist_lep2");
  num_lep12_eta->Write("num_lep12_eta");
  num_lep12_pt->Write("num_lep12_pt");
  den_lep12_eta->Write("den_lep12_eta");
  den_lep12_pt->Write("den_lep12_pt");

  
  fout->Write();
  fout->Close();

  return 0;
}


std::vector<TLepton*> makeLeptons(std::vector<TElectron*> electrons, bool mc, bool FiftyNs, std::string ID){

  std::vector<TLepton*> Leptons;

  //fill with  electrons
  for(unsigned int uiel=0; uiel<electrons.size(); uiel++){
    TElectron* iel = electrons.at(uiel);
    TLepton* iLep = new TLepton(iel->pt,iel->eta,iel->phi,iel->energy,iel->charge,iel->relIso,iel->miniIso,iel->susyIso);

    if(ID=="CBTight"){
      iLep->Tight=iel->cutBasedTight25nsSpring15MC();
      iLep->Loose=iel->cutBasedLoose25nsSpring15MC();
    }
    else if(ID=="CBLoose"){
      iLep->Tight=iel->cutBasedLoose25nsSpring15MC();
      iLep->Loose=true;
    }
    if(ID=="CBTightRC"){
      iLep->Tight=iel->cutBasedTight25nsSpring15MCRC();
      iLep->Loose=iel->cutBasedLoose25nsSpring15MCRC();
    }
    else if(ID=="CBLooseRC"){
      iLep->Tight=iel->cutBasedLoose25nsSpring15MCRC();
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
    else if(ID=="MVALooseNoIso"){
      iLep->Tight=iel->mvaLoose();
      iLep->Loose=true;
    }
    else if(ID=="MVATightRC"){
      iLep->Tight=iel->mvaTightRCIso();
      iLep->Loose=iel->mvaLooseRCIso();
    }
    else if(ID=="MVA80XTightRC"){
      iLep->Tight=iel->mva80XTightRCIso();
      iLep->Loose=iel->mva80XLooseRCIso();
    }
    else if(ID=="MVA2016TightRC"){
      iLep->Tight=iel->mva2016TightRCIso();
      iLep->Loose=iel->mvaJulieLooseRCIso();
    }
    else if(ID=="MVAJulieTightRC"){
      iLep->Tight=iel->mvaJulieTightRCIso();
      iLep->Loose=iel->mvaJulieLooseRCIso();
    }
    else if(ID=="MVAJulieNewTightRC"){
      iLep->Tight=iel->mvaJulieNewTightRCIso();
      iLep->Loose=iel->mvaJulieLooseRCIso();
    }
    else if(ID=="MVATightLC"){
      iLep->Tight=iel->mvaTightLCIso();
      iLep->Loose=iel->mvaLooseLCIso();
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
    else if(ID=="MVA2017TightIsoRC"){
                iLep->Tight=iel->mva94XTightV1_Iso_RC();
                iLep->Loose=iel->mva94XLooseV1_Iso_RC();
    }
    else if(ID=="MVA2017TightRC"){
                iLep->Tight=iel->mva94XTightV1_RC();
                iLep->Loose=iel->mva94XLooseV1_RC();
    }
    else if(ID=="MVA2017TightV2IsoRC"){
                iLep->Tight=iel->mva94XTightV2_90_Iso_RC();
                iLep->Loose=iel->mva94XLooseV2_Iso_RC();
    }
    else if(ID=="MVA2017TightV2IsoTightRC"){
                iLep->Tight=iel->mva94XTightV2_90_IsoTight_RC();
                iLep->Loose=iel->mva94XLooseV2_Iso_RC();
    }
    else if(ID=="MVA2017TightV2RC"){
                iLep->Tight=iel->mva94XTightV2_90_RC();
                iLep->Loose=iel->mva94XLooseV2_RC();
    }
    iLep->isMu = false;
    iLep->isEl = true;
    //only save if tight
    if(iLep->Tight){
      //apply pt cut
      //if(iLep->pt>20) Leptons.push_back(iLep);
      if(iLep->pt>30) Leptons.push_back(iLep);
    }
  }


  return Leptons;

}
