#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "tdrstyle.C"
#include "TLatex.h"
using namespace std;


int getEtaBin(float abseta){
  int bin=-1;
  if(abseta>2.0) bin=6;
  else if(abseta>1.556) bin=5;
  else if(abseta>1.442) bin=4;
  else if(abseta>0.8) bin=3;
  else if(abseta>0.4) bin=2;
  else if(abseta>=0.0) bin=1;

  return bin;
}


void cmid_plotter_2017data_test(){
  //turn of stats
  gStyle->SetOptStat(kFALSE);
  setTDRStyle();
  gStyle->SetStripDecimals(kTRUE);
  //TGaxis::SetMaxDigits(2);

  TLatex* text1 = new TLatex(3.570061,23.08044,"CMS Preliminary");
  text1->SetNDC();
  text1->SetTextAlign(13);
  text1->SetY(0.918);
  text1->SetTextFont(42);
  text1->SetTextSizePixels(24);

  TString lumiStr = "41.6 fb^{-1} (13 TeV)";
  //TString lumiStr = "37.6 fb^{-1} (13 TeV)";
  TLatex* text2 = new TLatex(3.570061,23.08044, lumiStr);
  text2->SetNDC();
  text2->SetTextAlign(13);
  text2->SetY(0.87);
  text2->SetTextFont(42);
  text2->SetTextSizePixels(24);

  text1->SetX(0.354);
  text2->SetX(0.354);

  std::vector<TString> eras; 
//   eras.push_back("2017C");
//   eras.push_back("2017D");
//   eras.push_back("2017E");
//  eras.push_back("2017F");
   eras.push_back("2017All");
  std::vector<TString> IDs; 
  IDs.push_back("MVA2017TightV2IsoRC");
  
  
  // create output directory
  TString dir = "CMID_082020/";
  system("mkdir -pv "+dir);

  for(unsigned int k=0; k<IDs.size();k++){ TString elID = IDs.at(k);

  for(unsigned int l=0; l< eras.size();l++){

    //input folder
    TString indir = "ChargeMisID_082020";

    //open file
    TFile* f = new TFile("../../test/"+indir+"/ChargeMisID_Data_"+eras.at(l)+"_Electrons_"+elID+".root");

    //outfile
    //TFile* fout = new TFile(dir+"ChargeMisID_Data_"+eras.at(l)+"_Electrons_"+elID+"_corrected.root","RECREATE");
    TFile* fout = new TFile("ChargeMisID_pTgt30_Data_"+eras.at(l)+"_Electrons_"+elID+"_corrected.root","RECREATE");

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
    //get lowpt cmid vs |eta|
    float abseta_bins[7] = {0.0,0.4,0.8,1.44,1.56,2.0,2.4};
    TH1F* h_num_lpt = new TH1F("h_num_lpt","Charge MisID Rate - Electrons with p_{T} < 100 GeV",6, abseta_bins);
    TH1F* h_den_lpt = new TH1F("h_den_lpt","Den Low pt",6, abseta_bins);
    float nSS_lpt=0.0;
float nAll_lpt=0.0;

int lep1etabin, lep2etabin;

for(int i=0; i< nEntries; i++){
t->GetEntry(i);
if( (fabs(lep1eta)> 1.44 && fabs(lep1eta) < 1.56) || (fabs(lep2eta)> 1.44 && fabs(lep2eta) < 1.56) ) continue;
//only consider low pt electrons in same |eta| bin
lep1etabin=getEtaBin(fabs(lep1eta));
lep2etabin=getEtaBin(fabs(lep2eta));
if(lep1pt < 100 && lep2pt<100 && lep1etabin==lep2etabin ){
	nAll_lpt+=1.0;
	//fill denominator - in same eta bin so only use eta from one
	h_den_lpt->Fill(fabs(lep1eta));
        //h_den_lpt->Fill(fabs(lep2eta));
	//check charge
	if(lep1ch==lep2ch){
	  nSS_lpt+=1.0;
	  h_num_lpt->Fill(fabs(lep1eta));
          //h_num_lpt->Fill(fabs(lep2eta));
	}      
}//end check on low pT
}//end loop over tree
//float cmid_lpt = 0.5*nSS_lpt / nAll_lpt;
float cmid_lpt = nSS_lpt / nAll_lpt;
//now divide to get rate and scale by 0.5
h_num_lpt->Sumw2();
//h_num_lpt->Scale(0.5);
h_num_lpt->Divide(h_den_lpt);
//h_num_lpt->Scale(0.5);

TCanvas* c = new TCanvas();

// ***** NOW WE HAVE LOW PT CHARGE MISID RATE AND NEED TO UNPACK FOR HIGHER PT ELECTRONS *******
//2D plots
TH2F* h_num2d_hpt = new TH2F("h_num2d_hpt","Charge MisID Rate - Electrons with p_{T} > 100 GeV",6, abseta_bins, 6, abseta_bins);
h_num2d_hpt->Sumw2();
TH2F* h_den2d_hpt = new TH2F("h_den2d_hpt","Den2d hpt",6, abseta_bins, 6, abseta_bins);
//eta plots for hpt
TH1F* h_num_hpt = new TH1F("h_num_hpt","Charge MisID Rate - Electrons with p_{T} > 100 GeV",6, abseta_bins);
h_num_hpt->Sumw2();
TH1F* h_num_hpt_l1 = new TH1F("h_num_hpt_l1","Charge MisID Rate - Electrons with p_{T} > 100 GeV",6, abseta_bins);
h_num_hpt_l1->Sumw2();
TH1F* h_weight_hpt = new TH1F("h_weight_hpt","Charge MisID Rate - Electrons with p_{T} > 100 GeV",6, abseta_bins);
h_weight_hpt->Sumw2();
TH1F* h_den_hpt = new TH1F("h_den_hpt","Den Low pt",6, abseta_bins);
TH1F* h_den_hpt_l1 = new TH1F("h_den_hpt_l1","Den Low pt",6, abseta_bins);
float xbins[6]= {0,100,150,200,300,500};
TH1F* h_numpt_hpt = new TH1F("h_numpt_hpt","Charge MisID Rate - Electrons with p_{T} > 100 GeV",5, xbins);
TH1F* h_denpt_hpt = new TH1F("h_denpt_hpt","Den Low pt",5, xbins);
TH1F* h_weightpt_hpt = new TH1F("h_weightpt_hpt","weightLow pt",5, xbins);

//TH1F* h_num_hpt_corr = new TH1F("h_num_hpt_corr","Charge MisID Rate - Electrons with p_{T} > 100 GeV - corrected",6, abseta_bins);  
// h_num_hpt_corr->Sumw2();

//2d plots high pt pair
TH2F* h_num2d_hhpt = new TH2F("h_num2d_hhpt","Charge MisID Rate - Electrons with p_{T} > 100 GeV",6, abseta_bins, 6, abseta_bins);
h_num2d_hhpt->Sumw2();
TH2F* h_den2d_hhpt = new TH2F("h_den2d_hhpt","Den2d hhpt",6, abseta_bins, 6, abseta_bins);
//eta plots
float abseta_bins_hhpt[5] = {0.0,0.8,1.44,1.56,2.4};
TH1F* h_num_hhpt = new TH1F("h_num_hhpt","Charge MisID Rate - Electrons with p_{T} > 100 GeV",4, abseta_bins_hhpt);
h_num_hhpt->Sumw2();
TH1F* h_num_hhpt_l1 = new TH1F("h_num_hhpt_l1","Charge MisID Rate - Electrons with p_{T} > 100 GeV",4, abseta_bins_hhpt);
h_num_hhpt_l1->Sumw2();
TH1F* h_weight_hhpt = new TH1F("h_weight_hhpt","Charge MisID Rate - Electrons with p_{T} > 100 GeV",4, abseta_bins_hhpt);
h_weight_hhpt->Sumw2();
TH1F* h_den_hhpt = new TH1F("h_den_hhpt","Den Low pt",4, abseta_bins_hhpt);
TH1F* h_den_hhpt_l1 = new TH1F("h_den_hhpt_l1","Den Low pt",4, abseta_bins_hhpt);
TH1F* h_numpt_hhpt = new TH1F("h_numpt_hhpt","Charge MisID Rate - Electrons with p_{T} > 100 GeV",5, xbins);
TH1F* h_denpt_hhpt = new TH1F("h_denpt_hhpt","Den Low pt",5, xbins);
TH1F* h_weightpt_hhpt = new TH1F("h_weightpt_hhpt","weight pt",5, xbins);

//TH1F* h_num_hhpt_corr = new TH1F("h_num_hhpt_corr","Charge MisID Rate - Electrons with p_{T} > 100 GeV - corrected",4, abseta_bins_hhpt);  
//h_num_hhpt_corr->Sumw2();

for(int j=0; j<nEntries;j++){
t->GetEntry(j);
//only consider electrons with one low pT and one high pT
if( (fabs(lep1eta)> 1.44 && fabs(lep1eta) < 1.56) || (fabs(lep2eta)> 1.44 && fabs(lep2eta) < 1.56) ) continue;
if( (lep1pt<100 && lep2pt>100) || (lep1pt>100 && lep2pt<100)){
      	//split by case which one is high pt
		if(lep1pt>100){	
		  if(lep1pt>200){
			//fill denominator
			h_den2d_hhpt->Fill(fabs(lep1eta),fabs(lep2eta));
			h_den_hhpt->Fill(fabs(lep1eta)); // 'h2'
			h_denpt_hhpt->Fill(lep1pt); //'h2 pt'
			int lpt_etabin = getEtaBin(fabs(lep2eta));
			float weight =  h_num_lpt->GetBinContent(lpt_etabin);
			h_weight_hhpt->Fill(fabs(lep1eta),weight); // 'h1'
			//h_ptweight_hhpt->Fill(
			if(lep1ch==lep2ch){
			  h_num_hhpt->Fill(fabs(lep1eta),1-weight); // 'h3'
			  h_numpt_hhpt->Fill(lep1pt,1.0-cmid_lpt);
			}
		  }
		  else{
			//fill denominator
			h_den_hpt->Fill(fabs(lep1eta)); // 'h2'
			h_denpt_hpt->Fill(lep1pt); // 'h2' pt
			int lpt_etabin = getEtaBin(fabs(lep2eta));
			float weight = h_num_lpt->GetBinContent(lpt_etabin);
			h_weight_hpt->Fill(fabs(lep1eta),weight); // 'h1'
			h_weightpt_hpt->Fill(weight); // 'h1'
			if(lep1ch==lep2ch){
			  h_num2d_hpt->Fill(fabs(lep1eta),fabs(lep2eta));
			  h_num_hpt->Fill(fabs(lep1eta),1-weight); // 'h3'
			  h_numpt_hpt->Fill(lep1pt,1.0);
			}
		  }
		}//end check lep1 is high pt
		else if(lep2pt>100){
		  if(lep2pt>200){
			//fill denominator
			h_den2d_hhpt->Fill(fabs(lep2eta),fabs(lep1eta));
			h_den_hhpt->Fill(fabs(lep2eta)); // 'h2'
			h_denpt_hhpt->Fill(lep2pt);
			int lpt_etabin = getEtaBin(fabs(lep1eta));
			float weight =  h_num_lpt->GetBinContent(lpt_etabin);
			h_weight_hhpt->Fill(fabs(lep2eta),weight); // 'h1'
			if(lep1ch==lep2ch){
			  h_num2d_hhpt->Fill(fabs(lep2eta),fabs(lep1eta));
			  h_num_hhpt->Fill(fabs(lep2eta),1-weight); // 'h3'
			  h_numpt_hhpt->Fill(lep2pt,1.0);
			}
		  }
		  else{
			//fill denominator
			h_den2d_hpt->Fill(fabs(lep2eta),fabs(lep1eta));
			h_den_hpt->Fill(fabs(lep2eta)); // 'h2'
			h_denpt_hpt->Fill(lep2pt);
			int lpt_etabin = getEtaBin(fabs(lep1eta));
			float weight =  h_num_lpt->GetBinContent(lpt_etabin);
			h_weight_hpt->Fill(fabs(lep2eta),weight); //'h1'
			if(lep1ch==lep2ch){
			  h_num2d_hpt->Fill(fabs(lep2eta),fabs(lep1eta));
			  h_num_hpt->Fill(fabs(lep2eta),1-weight); // 'h3'
			  h_numpt_hpt->Fill(lep2pt,1.0-cmid_lpt);
			}
		  }
		}//end check on lep2 is high pt
      }//end check on at least one is high pt
      
    }//end loop over tree
    
    TH1F* h_hpt_32 = (TH1F*) h_num_hpt->Clone("hpt_final");
    // h_hpt_32->Sumw2();
    h_hpt_32->Divide(h_den_hpt);
    
    TH1F* h_hpt_12 = (TH1F*) h_weight_hpt->Clone();
    //h_hpt_12->Sumw2();
    h_hpt_12->Divide(h_den_hpt);
    
    h_hpt_32->Add(h_hpt_12,-1.);
    h_hpt_32->SetMarkerStyle(20);
    h_hpt_32->GetYaxis()->SetRangeUser(0,0.09);
    h_hpt_32->GetYaxis()->SetTitle("Probability of charge misidentification");
    h_hpt_32->GetYaxis()->SetTitleOffset(1.5);
    h_hpt_32->GetYaxis()->SetLabelSize(0.03);
    h_hpt_32->GetXaxis()->SetLabelSize(0.03);
    h_hpt_32->GetYaxis()->SetNoExponent(kFALSE);
    h_hpt_32->GetXaxis()->SetTitle("Electron #eta");
    h_hpt_32->Draw("pe");
    std::cout << "\nhpt rate: \n" << std::endl;
    for(int l=1; l<=h_hpt_32->GetNbinsX(); l++){
      std::cout<<"\t bin "<<l<<": "<<h_hpt_32->GetBinContent(l)<<std::endl;
    }
    text1->Draw();
    text2->Draw();
//     fout->Append(h_hpt_32);
    c->Print(dir+"ChargeMisID_vAbsEta_hpt_corr_"+eras.at(l)+"_"+elID+".pdf");
    
    
    TH1F* h_hhpt_32 = (TH1F*) h_num_hhpt->Clone("hhpt_final");
    //h_hhpt_32->Sumw2();
    h_hhpt_32->Divide(h_den_hhpt);
    
    TH1F* h_hhpt_12 = (TH1F*) h_weight_hhpt->Clone();
    //h_hhpt_12->Sumw2();
    h_hhpt_12->Divide(h_den_hhpt);
    
    h_hhpt_32->Add(h_hhpt_12,-1.);
    h_hhpt_32->SetMarkerStyle(20);
    h_hhpt_32->GetYaxis()->SetRangeUser(0,0.2);
    h_hhpt_32->GetYaxis()->SetTitle("Probability of charge misidentification");
    h_hhpt_32->GetYaxis()->SetTitleOffset(1.5);
    h_hhpt_32->GetYaxis()->SetLabelSize(0.03);
    h_hhpt_32->GetXaxis()->SetLabelSize(0.03);
    h_hhpt_32->GetYaxis()->SetNoExponent(kFALSE);
    h_hhpt_32->GetXaxis()->SetTitle("Electron #eta");
    h_hhpt_32->Draw("pe");
    std::cout << "\nhhpt rate: \n" << std::endl;
    for(int l=1; l<=h_hhpt_32->GetNbinsX(); l++){
      std::cout<<"\t bin "<<l<<": "<<h_hhpt_32->GetBinContent(l)<<std::endl;
    }
    text1->Draw();
    text2->Draw();
    c->Print(dir+"ChargeMisID_vAbsEta_hhpt_corr_"+eras.at(l)+"_"+elID+".pdf");
//     fout->Append(h_hhpt_32);
    
    h_num_lpt->SetMarkerStyle(20);
    h_num_lpt->GetYaxis()->SetRangeUser(0,0.012);
    h_num_lpt->GetYaxis()->SetTitle("Probability of charge misidentification");
    h_num_lpt->GetYaxis()->SetTitleOffset(1.5);
    h_num_lpt->GetYaxis()->SetLabelSize(0.03);
    h_num_lpt->GetXaxis()->SetLabelSize(0.03);
    h_num_lpt->GetYaxis()->SetNoExponent(kFALSE);
    h_num_lpt->GetXaxis()->SetTitle("Electron #eta");
    h_num_lpt->Draw("pe");
    std::cout << "\nlpt rate: \n" << std::endl;
    for(int l=1; l<=h_num_lpt->GetNbinsX(); l++){
      std::cout<<"\t bin "<<l<<": "<<h_num_lpt->GetBinContent(l)<<std::endl;
    }
    text1->Draw();
    text2->Draw();
//     fout->Append(h_num_lpt);
    c->Print(dir+"ChargeMisID_vAbsEta_lpt_"+eras.at(l)+"_"+elID+".pdf");
    
    
    c->SetLogy();
    h_numpt_hpt->Sumw2();
    h_numpt_hpt->Divide(h_denpt_hpt);
    h_numpt_hpt->SetBinContent(1,cmid_lpt);
    h_numpt_hpt->SetMarkerStyle(20);
    //h_numpt_hpt->GetYaxis()->SetRangeUser(0.0003,0.4);
    h_numpt_hpt->GetYaxis()->SetRangeUser(0.0001,0.2);
    h_numpt_hpt->GetYaxis()->SetLabelSize(0.03);
    h_numpt_hpt->GetXaxis()->SetTitle("p_{T}");
    h_numpt_hpt->GetYaxis()->SetTitle("probability of charge misidentification");
    h_numpt_hpt->Draw("pe");
    
    h_numpt_hhpt->Sumw2();
    h_numpt_hhpt->Divide(h_denpt_hhpt);
    //correct each bin of h_numpt_hhpt
    for(int k=1; k<= h_numpt_hhpt->GetNbinsX();k++){
      h_numpt_hhpt->SetBinContent(k, h_numpt_hhpt->GetBinContent(k)-cmid_lpt);
    }
    h_numpt_hhpt->GetYaxis()->SetRangeUser(0.0001,0.2);
    h_numpt_hhpt->GetYaxis()->SetTitleSize(0.035);
    h_numpt_hhpt->SetMarkerStyle(20);
    h_numpt_hhpt->Draw("pe same");
    text1->Draw();
    text2->Draw();
    c->Print(dir+"ChargeMisID_vPT_"+eras.at(l)+"_"+elID+".pdf");
    TH1F* h_pt_final = (TH1F*) h_numpt_hpt->Clone("h_pt_final");
    //correct bins by hand
    h_pt_final->SetBinContent(1,cmid_lpt);
    h_pt_final->SetBinContent(4,h_numpt_hhpt->GetBinContent(4));
    h_pt_final->SetBinContent(5,h_numpt_hhpt->GetBinContent(5));
    h_pt_final->SetBinError(2,h_numpt_hpt->GetBinError(2));
    h_pt_final->SetBinError(3,h_numpt_hpt->GetBinError(3));
    h_pt_final->SetBinError(4,h_numpt_hhpt->GetBinError(4));
    h_pt_final->SetBinError(5,h_numpt_hhpt->GetBinError(5));
//     fout->Append(h_pt_final);
    c->SetLogy(0);

    //now check alternate pairing, one high pt and one medium pt
    //eta plots
    float abseta_bins_hmpt[5] = {0.0,0.8,1.44,1.56,2.4};
    TH1F* h_num_hmpt = new TH1F("h_num_hmpt","Charge MisID Rate - Electrons with p_{T} > 100 GeV",4, abseta_bins_hmpt);
    h_num_hmpt->Sumw2();
    TH1F* h_weight_hmpt = new TH1F("h_weight_hmpt","Charge MisID Rate - Electrons with p_{T} > 100 GeV",4, abseta_bins_hmpt);
    h_weight_hmpt->Sumw2();
    TH1F* h_den_hmpt = new TH1F("h_den_hmpt","Den Low pt",4, abseta_bins_hmpt);
    
    for(int j=0; j<nEntries;j++){
      t->GetEntry(j);
      //skip gap
      if( (fabs(lep1eta)> 1.44 && fabs(lep1eta) < 1.56) || (fabs(lep2eta)> 1.44 && fabs(lep2eta) < 1.56) ) continue;
      //only consider electrons with at leat one high pt electron
      if(lep1pt>200 || lep2pt>200){
	
		//split by case which one is high pt
		if(lep1pt>200){	
		  if(lep2pt>100 || lep2pt < 200){
			//fill denominator
			h_den_hmpt->Fill(fabs(lep1eta)); // 'h2'
			int lpt_etabin = getEtaBin(fabs(lep2eta));
			float weight =  h_hpt_32->GetBinContent(lpt_etabin);
			h_weight_hmpt->Fill(fabs(lep1eta),weight); // 'h1'
			//h_ptweight_hmpt->Fill(
			if(lep1ch==lep2ch){
			  h_num_hmpt->Fill(fabs(lep1eta)); // 'h3'
			}
		  }
		}
      } 
    }//end event loop
      
    
    TH1F* h_hmpt_32 = (TH1F*) h_num_hmpt->Clone("hmpt_final");
    //h_hmpt_32->Sumw2();                                                                                                                                                                                            
    h_hmpt_32->Divide(h_den_hmpt);
    
    TH1F* h_hmpt_12 = (TH1F*) h_weight_hmpt->Clone();
    //h_hmpt_12->Sumw2();                                                                                                                                                                                            
    h_hmpt_12->Divide(h_den_hmpt);
    
    h_hmpt_32->Add(h_hmpt_12,-1.);
    h_hmpt_32->SetMarkerStyle(20);
    h_hmpt_32->GetYaxis()->SetRangeUser(0,0.2);
    h_hmpt_32->GetYaxis()->SetTitle("Probability of charge misidentification");
    h_hmpt_32->GetYaxis()->SetTitleOffset(1.5);
    h_hmpt_32->GetYaxis()->SetLabelSize(0.03);
    h_hmpt_32->GetXaxis()->SetLabelSize(0.03);
    h_hmpt_32->GetYaxis()->SetNoExponent(kFALSE);
    h_hmpt_32->GetXaxis()->SetTitle("Electron #eta");
    h_hmpt_32->Draw("pe");
    text1->Draw();
    text2->Draw();
    c->Print(dir+"ChargeMisID_vAbsEta_hmpt_corr_"+eras.at(l)+"_"+elID+".pdf");
//     fout->Append(h_hmpt_32);      
    
    
    
    h_hmpt_32->SetMarkerStyle(22);
    h_hmpt_32->SetMarkerColor(kBlue);
    h_hmpt_32->Draw("pe");
    h_hhpt_32->Draw("pe same");
    TLegend* leg = new TLegend(0.2,0.4,0.6,0.9);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->AddEntry(h_hmpt_32,"medium-high pairing","p");
    leg->AddEntry(h_hhpt_32,"low-high pairing","p");
    leg->Draw("same");
    c->Print(dir+"ChargeMisID_PairComparison_hhpt_"+eras.at(l)+"_"+elID+".pdf");
    
        
    fout->Write();

	delete h_num2d_hpt;
	delete h_den2d_hpt;
	delete h_num_hpt;
	delete h_weight_hpt;
	delete h_den_hpt;
	delete h_numpt_hpt;
	delete h_denpt_hpt;
	delete h_weightpt_hpt;

	delete h_num2d_hhpt;
	delete h_den2d_hhpt;
	delete h_num_hhpt;
	delete h_weight_hhpt;
	delete h_den_hhpt;
	delete h_numpt_hhpt;
	delete h_denpt_hhpt;
	delete h_weightpt_hhpt;

    delete h_hpt_32;
    delete h_hhpt_32;
    delete h_num_lpt;
    delete h_den_lpt;
    delete h_pt_final;
    delete h_hmpt_32;

    fout->Close();
  }

  }//end loop on IDs
  
  gApplication->Terminate();
}


