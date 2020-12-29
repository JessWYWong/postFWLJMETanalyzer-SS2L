#include <iostream>
#include <vector>
#include "TF2.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TMinuit.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "tdrstyle.C"
#include "TLatex.h"
using namespace std;

//float ptbins[13] = {20,30,40,50,70,100,130,160,200,250,300,400,600};
float ptbins[4] = {20,100,200,600};
const Int_t npar_pt = sizeof(ptbins) / sizeof(float) -1;
//float elEtabins[8]={-2.4,-1.57,-0.8,-0.4,0.4,0.8,1.57,2.4};
//float elEtabins[5]={0.0,0.4,0.8,1.57,2.4};
float elEtabins[7]={0.0,0.4,0.8,1.442,1.556,2.0,2.4};
const Int_t npar_eta = sizeof(elEtabins) / sizeof(float) -1;

TString ID = "MVA2017TightV2IsoTightRC";
TString dir = "CMID_likelihood_082020/";
TString era = "2017All";
TString indir = "ChargeMisID_likelihood_082020";

TFile* f = new TFile("../../test/"+indir+"/ChargeMisID_likelihood_Data_"+era+"_Electrons_"+ID+".root");
TH2F * num_lep12_eta = new TH2F("num_lep12_eta","num_lep12_eta", npar_eta, elEtabins, npar_eta, elEtabins);
TH2F * num_lep12_pt = new TH2F("num_lep12_pt","num_lep12_pt", npar_pt, ptbins, npar_pt, ptbins);
TH2F * den_lep12_eta = new TH2F("den_lep12_eta","den_lep12_eta", npar_eta, elEtabins, npar_eta, elEtabins);
TH2F * den_lep12_pt = new TH2F("den_lep12_pt","den_lep12_pt", npar_pt, ptbins, npar_pt, ptbins);
TH2F * num_lep12 = new TH2F("num_lep12", "num_lep12", npar_eta*npar_pt, 0,npar_eta*npar_pt, npar_eta*npar_pt, 0,npar_eta*npar_pt);
TH2F * den_lep12 = new TH2F("den_lep12", "den_lep12", npar_eta*npar_pt, 0,npar_eta*npar_pt, npar_eta*npar_pt, 0,npar_eta*npar_pt);

int findInd(float value, float * axis, int n){
  int Ind = -1;
  for ( int i = 1 ; i < n+1 ; ++i ){
    //cout<<axis[i-1]<< " " <<axis[i] <<endl;
    if (value > axis[i-1] && value <= axis[i]) {Ind = i;}
  }
  return Ind;
}

void nllFunc(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){
  Double_t result=0;
  int n_ij,nss_ij;
  Double_t p_ij;
  for ( int i = 0 ; i < npar; ++i ){
    for ( int j = 0 ; j < npar; ++j ){
      n_ij = den_lep12->GetBinContent(i+1,j+1);
      if (n_ij == 0) continue;
      nss_ij = num_lep12->GetBinContent(i+1,j+1);
      p_ij = par[j]*(1-par[i]) + par[i]*(1-par[j]);
      result += nss_ij*log(n_ij*p_ij) - n_ij*p_ij; //- log(TMath::Factorial(nss_ij));
      for ( int k = 0 ; k < nss_ij; ++k ){ result -= log(k+1); }
    }
  }
  f = -1.0*result;
}

void cmid_nllPlotter_2017data_pTeta() {
  //get tree
  TTree* t = (TTree*) f->Get("ChargeMisID");
  //assign variables
  float lep1pt, lep1eta, lep1phi, lep1e, lep2pt, lep2eta, lep2phi,lep2e;
  int lep1ch, lep2ch;
  t->SetBranchAddress("Lep1Pt",&lep1pt);
  t->SetBranchAddress("Lep1Eta",&lep1eta);
  t->SetBranchAddress("Lep1Phi",&lep1phi);
  t->SetBranchAddress("Lep1E",&lep1e);
  t->SetBranchAddress("Lep1Charge",&lep1ch);
  t->SetBranchAddress("Lep2Pt",&lep2pt);
  t->SetBranchAddress("Lep2Eta",&lep2eta);
  t->SetBranchAddress("Lep2Phi",&lep2phi);
  t->SetBranchAddress("Lep2E",&lep2e);
  t->SetBranchAddress("Lep2Charge",&lep2ch);

  int nEntries = t->GetEntries();
  for(int i=0; i< nEntries; i++){
    t->GetEntry(i);
    if( (fabs(lep1eta)> 1.44 && fabs(lep1eta) < 1.56) || (fabs(lep2eta)> 1.44 && fabs(lep2eta) < 1.56) ) continue;

    int ptInd1  = findInd(lep1pt,ptbins,npar_pt);
    int etaInd1 = findInd(fabs(lep1eta),elEtabins,npar_eta);
    int Ind1    = (ptInd1-1)*npar_eta + etaInd1 -1;
    int ptInd2  = findInd(lep2pt,ptbins,npar_pt);
    int etaInd2 = findInd(fabs(lep2eta),elEtabins,npar_eta);
    int Ind2    = (ptInd2-1)*npar_eta + etaInd2 -1;

    //cout<< lep1pt << " " <<ptInd1<< " "<< fabs(lep1eta)<< " " << etaInd1 <<" "<<Ind1<<endl;

    if(lep1ch==lep2ch){
      num_lep12_eta->Fill(fabs(lep1eta),fabs(lep2eta));
      num_lep12_pt->Fill(lep1pt,lep2pt);
      num_lep12->Fill(Ind1,Ind2);
    }
    den_lep12_eta->Fill(fabs(lep1eta),fabs(lep2eta));
    den_lep12_pt->Fill(lep1pt,lep2pt);
    den_lep12->Fill(Ind1,Ind2);
  } 

  //for ( int i = 0 ; i < npar_eta*npar_pt; ++i ){
  //  for ( int j = 0 ; j < npar_eta*npar_pt; ++j ){
      //cout<< num_lep12->GetBinContent(i+1,j+1) <<endl;
      //cout<< den_lep12->GetBinContent(i+1,j+1) <<endl;
  //}}

  TMinuit *gMinuit = new TMinuit(npar_eta*npar_pt);
  gMinuit->SetFCN(nllFunc);

  int ierflg = 0;
  for ( int i = 0 ; i < npar_eta*npar_pt ; ++i ){
    gMinuit->mnparm(i,Form("eta_le%.2f_pt_le%.2f",elEtabins[i%npar_eta+1],ptbins[i/npar_eta+1]),0.005, 0.0001, 0.0, 0.12, ierflg);
  }

  Double_t arglist[10];
  gMinuit->SetErrorDef(0.5);
  gMinuit->mnexcm("MIGRAD",arglist,0,ierflg);
  
  double outpar[npar_eta*npar_pt], err[npar_eta*npar_pt];
  for ( int i = 0 ; i < npar_eta*npar_pt ; ++i ){
    gMinuit->GetParameter(i,outpar[i],err[i]);
    cout<< Form("eta_le%.2f_pt_le%.2f %.2E +/- %.2E",elEtabins[i%npar_eta+1],ptbins[i/npar_eta+1],outpar[i],err[i]) <<endl;
    //cout<< Form("eta_le%.2f_pt_le%.2f",elEtabins[i%npar_eta+1],ptbins[i/npar_eta+1]) << " " << outpar[i] << "+/-"<< err[i]<<endl; 
  }

  // Write Ouput
  TFile* fout = new TFile("ChargeMisID_likelihood_Data_"+era+"_Electrons_"+ID+".root","RECREATE");
  system("mkdir -pv "+dir);

  num_lep12->Write("num_lep12");
  den_lep12->Write("den_lep12");

  gStyle->SetOptStat(kFALSE);
  setTDRStyle();
  gStyle->SetStripDecimals(kTRUE);
  TLatex* text1 = new TLatex(3.570061,23.08044,"CMS Preliminary");
  text1->SetNDC();
  text1->SetTextAlign(13);
  text1->SetY(0.918);
  text1->SetTextFont(42);
  text1->SetTextSizePixels(24);

  TString lumiStr = "41.6 fb^{-1} (13 TeV)";
  TLatex* text2 = new TLatex(3.570061,23.08044, lumiStr);
  text2->SetNDC();
  text2->SetTextAlign(13);
  text2->SetY(0.87);
  text2->SetTextFont(42);
  text2->SetTextSizePixels(24);
  text1->SetX(0.354);
  text2->SetX(0.354);

  TH1F* h_num_lpt = new TH1F("h_num_lpt","Charge MisID Rate - Electrons with p_{T} < 100 GeV",npar_eta, elEtabins);
  h_num_lpt->Sumw2();
  //TH1F* h_num_hpt = new TH1F("h_num_hpt","Charge MisID Rate - Electrons with p_{T} > 100 GeV",npar_eta, elEtabins);
  //h_num_hpt->Sumw2();
  TH1F* hpt_final = new TH1F("hpt_final","Charge MisID Rate - Electrons with p_{T} > 100 GeV",npar_eta, elEtabins);
  hpt_final->Sumw2();
  //TH1F* h_num_hhpt = new TH1F("h_num_hhpt","Charge MisID Rate - Electrons with p_{T} > 200 GeV",npar_eta, elEtabins);
  //h_num_hhpt->Sumw2();
  TH1F* hhpt_final = new TH1F("hhpt_final","Charge MisID Rate - Electrons with p_{T} > 200 GeV",npar_eta, elEtabins);
  hhpt_final->Sumw2();

  std::vector<TH1F*> misIDhists;
  misIDhists.push_back(h_num_lpt);
  misIDhists.push_back(hpt_final); //h_num_hpt);
  misIDhists.push_back(hhpt_final); //h_num_hhpt);

  for ( int i = 0 ; i < npar_eta*npar_pt ; ++i ){
    cout<< "hist"<<i/npar_eta<< " bin"<<i%npar_eta+1 <<" : "<< outpar[i]<<"+/-"<<err[i]<<endl;
    misIDhists.at(i/npar_eta)->SetBinContent(i%npar_eta+1,outpar[i]);
    misIDhists.at(i/npar_eta)->SetBinError(i%npar_eta+1,err[i]);
  }
  
  std::vector<float> y_up{0.004,0.04,0.04};

  TCanvas* c = new TCanvas();
  for (int i = 0; i < npar_pt ; ++i){
    misIDhists.at(i)->SetMarkerStyle(20);
    misIDhists.at(i)->GetYaxis()->SetRangeUser(0,y_up.at(i));
    misIDhists.at(i)->GetYaxis()->SetTitle("Probability of charge misidentification");
    misIDhists.at(i)->GetYaxis()->SetTitleOffset(2.0);
    misIDhists.at(i)->GetYaxis()->SetTitleSize(0.03);
    misIDhists.at(i)->GetYaxis()->SetLabelSize(0.03);
    misIDhists.at(i)->GetXaxis()->SetLabelSize(0.03);
    misIDhists.at(i)->GetYaxis()->SetNoExponent(kFALSE);
    misIDhists.at(i)->GetXaxis()->SetTitle("Electron #eta");
    misIDhists.at(i)->GetXaxis()->SetTitleSize(0.03);
    misIDhists.at(i)->Draw("pe");
    text1->Draw();
    text2->Draw();
    c->Print(dir+"ChargeMisID_likelihood_vAbsEta_"+(misIDhists.at(i)->GetName())+"_"+era+"_"+ID+".pdf");
  }

  //TH1F* hpt_final = (TH1F*) h_num_hpt->Clone("hpt_final");
  //TH1F* hhpt_final = (TH1F*) h_num_hhpt->Clone("hhpt_final")
  
  fout->Write();
  fout->Close();    

//  for ( int i = 0 ; i < npar_eta; ++i ){
//    for ( int j = 0 ; j < npar_eta; ++j ){
//      cout<< num_lep12_eta->GetBinContent(i+1,j+1) <<endl;
//      cout<< den_lep12_eta->GetBinContent(i+1,j+1) <<endl;
//      double test = 0;
//      test = num_lep12_eta->GetBinContent(i+1,j+1)*log(den_lep12_eta->GetBinContent(i+1,j+1)*0.1*0.9*2) - den_lep12_eta->GetBinContent(i+1,j+1)*0.1*0.9*2 ;
//      for ( int k = 0 ; k < num_lep12_eta->GetBinContent(i+1,j+1); ++k ){ test -= log(k+1); }
//      cout<< test <<endl;
//    }
//  }

//  TMinuit *gMinuit = new TMinuit(npar_eta); 
//  gMinuit->SetFCN(nllFunc_eta);

//  int ierflg = 0;
//  for ( int i = 0 ; i < npar_eta ; ++i ){
//    gMinuit->mnparm(i,Form("eta_%.2f_%.2f",elEtabins[i],elEtabins[i+1]),0.1, 0.01, 0.0, 0.5, ierflg);
//  }

//  Double_t arglist[10];
//  gMinuit->SetErrorDef(0.5);
//  gMinuit->mnexcm("MIGRAD",arglist,0,ierflg); 

//  double outpar[npar_eta], err[npar_eta];
//  for ( int i = 0 ; i < npar_eta ; ++i ){
//    gMinuit->GetParameter(i,outpar[i],err[i]);
//    cout << "e"<< i << " " << outpar[i] <<endl;
//  }




//  for ( int i = 0 ; i < npar_pt; ++i ){
//    for ( int j = 0 ; j < npar_pt; ++j ){
//      cout<< num_lep12_pt->GetBinContent(i+1,j+1) <<endl;
//      cout<< den_lep12_pt->GetBinContent(i+1,j+1) <<endl;
//      double test = 0;
//      test = num_lep12_pt->GetBinContent(i+1,j+1)*log(den_lep12_pt->GetBinContent(i+1,j+1)*0.1*0.9*2) - den_lep12_pt->GetBinContent(i+1,j+1)*0.1*0.9*2 ;
//      for ( int k = 0 ; k < num_lep12_pt->GetBinContent(i+1,j+1); ++k ){ test -= log(k+1); }
//      cout<< test <<endl;
//    }
//  }

//  TMinuit *pMinuit = new TMinuit(npar_pt);
//  pMinuit->SetFCN(nllFunc_pt);
//  for ( int i = 0 ; i < npar_pt ; ++i ){
//    pMinuit->mnparm(i,Form("pt_%.2f_%.2f",ptbins[i],ptbins[i+1]),0.1, 0.01, 0.0, 0.5, ierflg);
//  }
//  //Double_t arglist[10];
//  pMinuit->SetErrorDef(0.5);
//  pMinuit->mnexcm("MIGRAD",arglist,0,ierflg);

//  double outparp[npar_pt], errp[npar_pt];
//  for ( int i = 0 ; i < npar_pt ; ++i ){
//    pMinuit->GetParameter(i,outparp[i],errp[i]);
//    cout << "e"<< i << " " << outparp[i] <<endl;
//  } 

  gApplication->Terminate(); 
}

