#include <iostream>
#include "TFile.h"
#include "TGraphAsymmErrors.h"
void fr_plotter(){


  //TFile* f = new TFile("FakeRate_TTbar.root");
  TFile* f = new TFile("FakeRate_TTTo2L2Nu.root");
  TTree* t = (TTree*) f->Get("FakeRateByFlavor");
  
  float LepPt,LepEta;
  int LepIsTight,LepFlavor,LepSourceFlavor;

  t->SetBranchAddress("LepPt",&LepPt);
  t->SetBranchAddress("LepEta",&LepEta);
  t->SetBranchAddress("LepIsTight",&LepIsTight);
  t->SetBranchAddress("LepFlavor",&LepFlavor);
  t->SetBranchAddress("LepSourceFlavor",&LepSourceFlavor);

  TFile* outFile = new TFile("Sys_FakeRateByFlavor.root","RECREATE");
  float el_etabins[9] = {-2.4,-1.566,-1.4442,-0.8,0.0,0.8,1.4442,1.566,2.4};
  float mu_etabins[9] = {-2.4,-2.1,-1.2,-0.9,0.0,0.9,1.2,2.1,2.4};
  float ptbins[11] = {20,25,30,35,50,75,100,150,200,500,1000};
  //electron histograms
  TH1F* h_num_el_b_eta = new TH1F("h_num_el_b_eta","Electron Fake Rate - bottom quarks",8,el_etabins);
  TH1F* h_den_el_b_eta = new TH1F("h_den_el_b_eta","Electron Fake Rate - bottom quarks",8,el_etabins);

  TH1F* h_num_el_c_eta = new TH1F("h_num_el_c_eta","Electron Fake Rate - charm quarks",8,el_etabins);
  TH1F* h_den_el_c_eta = new TH1F("h_den_el_c_eta","Electron Fake Rate - charm quarks",8,el_etabins);

  TH1F* h_num_el_l_eta = new TH1F("h_num_el_l_eta","Electron Fake Rate - light quarks",8,el_etabins);
  TH1F* h_den_el_l_eta = new TH1F("h_den_el_l_eta","Electron Fake Rate - light quarks",8,el_etabins);

  TH1F* h_num_el_u_eta = new TH1F("h_num_el_u_eta","Electron Fake Rate - unmatched",8,el_etabins);
  TH1F* h_den_el_u_eta = new TH1F("h_den_el_u_eta","Electron Fake Rate - unmatched",8,el_etabins);

  TH1F* h_num_el_f_eta = new TH1F("h_num_el_f_eta","Electron Fake Rate - fake lepton",8,el_etabins);
  TH1F* h_den_el_f_eta = new TH1F("h_den_el_f_eta","Electron Fake Rate - fake lepton",8,el_etabins);

  TH1F* h_num_el_b_pt = new TH1F("h_num_el_b_pt","Electron Fake Rate - bottom quarks",10,ptbins);
  TH1F* h_den_el_b_pt = new TH1F("h_den_el_b_pt","Electron Fake Rate - bottom quarks",10,ptbins);

  TH1F* h_num_el_c_pt = new TH1F("h_num_el_c_pt","Electron Fake Rate - charm quarks",10,ptbins);
  TH1F* h_den_el_c_pt = new TH1F("h_den_el_c_pt","Electron Fake Rate - charm quarks",10,ptbins);

  TH1F* h_num_el_l_pt = new TH1F("h_num_el_l_pt","Electron Fake Rate - light quarks",10,ptbins);
  TH1F* h_den_el_l_pt = new TH1F("h_den_el_l_pt","Electron Fake Rate - light quarks",10,ptbins);

  TH1F* h_num_el_u_pt = new TH1F("h_num_el_u_pt","Electron Fake Rate - unmatched",10,ptbins);
  TH1F* h_den_el_u_pt = new TH1F("h_den_el_u_pt","Electron Fake Rate - unmatched",10,ptbins);

  TH1F* h_num_el_f_pt = new TH1F("h_num_el_f_pt","Electron Fake Rate - fake lepton",10,ptbins);
  TH1F* h_den_el_f_pt = new TH1F("h_den_el_f_pt","Electron Fake Rate - fake lepton",10,ptbins);


  //muon histograms
  TH1F* h_num_mu_b_eta = new TH1F("h_num_mu_b_eta","Muon Fake Rate - bottom quarks",8,mu_etabins);
  TH1F* h_den_mu_b_eta = new TH1F("h_den_mu_b_eta","Muon Fake Rate - bottom quarks",8,mu_etabins);

  TH1F* h_num_mu_c_eta = new TH1F("h_num_mu_c_eta","Muon Fake Rate - charm quarks",8,mu_etabins);
  TH1F* h_den_mu_c_eta = new TH1F("h_den_mu_c_eta","Muon Fake Rate - charm quarks",8,mu_etabins);

  TH1F* h_num_mu_l_eta = new TH1F("h_num_mu_l_eta","Muon Fake Rate - light quarks",8,mu_etabins);
  TH1F* h_den_mu_l_eta = new TH1F("h_den_mu_l_eta","Muon Fake Rate - light quarks",8,mu_etabins);

  TH1F* h_num_mu_u_eta = new TH1F("h_num_mu_u_eta","Muon Fake Rate - unmatched",8,mu_etabins);
  TH1F* h_den_mu_u_eta = new TH1F("h_den_mu_u_eta","Muon Fake Rate - unmatched",8,mu_etabins);

  TH1F* h_num_mu_f_eta = new TH1F("h_num_mu_f_eta","Muon Fake Rate - fake lepton",8,mu_etabins);
  TH1F* h_den_mu_f_eta = new TH1F("h_den_mu_f_eta","Muon Fake Rate - fake lepton",8,mu_etabins);

  TH1F* h_num_mu_b_pt = new TH1F("h_num_mu_b_pt","Muon Fake Rate - bottom quarks",10,ptbins);
  TH1F* h_den_mu_b_pt = new TH1F("h_den_mu_b_pt","Muon Fake Rate - bottom quarks",10,ptbins);

  TH1F* h_num_mu_c_pt = new TH1F("h_num_mu_c_pt","Muon Fake Rate - charm quarks",10,ptbins);
  TH1F* h_den_mu_c_pt = new TH1F("h_den_mu_c_pt","Muon Fake Rate - charm quarks",10,ptbins);

  TH1F* h_num_mu_l_pt = new TH1F("h_num_mu_l_pt","Muon Fake Rate - light quarks",10,ptbins);
  TH1F* h_den_mu_l_pt = new TH1F("h_den_mu_l_pt","Muon Fake Rate - light quarks",10,ptbins);

  TH1F* h_num_mu_u_pt = new TH1F("h_num_mu_u_pt","Muon Fake Rate - unmatched",10,ptbins);
  TH1F* h_den_mu_u_pt = new TH1F("h_den_mu_u_pt","Muon Fake Rate - unmatched",10,ptbins);

  TH1F* h_num_mu_f_pt = new TH1F("h_num_mu_f_pt","Muon Fake Rate - fake lepton",10,ptbins);
  TH1F* h_den_mu_f_pt = new TH1F("h_den_mu_f_pt","Muon Fake Rate - fake lepton",10,ptbins);



  for(int i=0; i<t->GetEntries();i++){
    t->GetEntry(i);
    if (LepPt<25 || LepPt>35) continue;
    
    if(LepFlavor==0){//electron
      
      if(LepSourceFlavor==0){//light
	h_den_el_l_eta->Fill(LepEta);
	if(LepIsTight) h_num_el_l_eta->Fill(LepEta);
      }
      else if(LepSourceFlavor==1){//charm
	h_den_el_c_eta->Fill(LepEta);
	if(LepIsTight) h_num_el_c_eta->Fill(LepEta);
      }
      else if(LepSourceFlavor==2){//bottom
	h_den_el_b_eta->Fill(LepEta);
	if(LepIsTight) h_num_el_b_eta->Fill(LepEta);
      }
      else if(LepSourceFlavor==3){//fake
	h_den_el_f_eta->Fill(LepEta);
	if(LepIsTight) h_num_el_f_eta->Fill(LepEta);
      }
      else if(LepSourceFlavor==4){//unmatched
	h_den_el_u_eta->Fill(LepEta);
	if(LepIsTight) h_num_el_u_eta->Fill(LepEta);
      }
      //pt
      if(LepSourceFlavor==0){//light
	h_den_el_l_pt->Fill(LepPt);
	if(LepIsTight) h_num_el_l_pt->Fill(LepPt);
      }
      else if(LepSourceFlavor==1){//charm
	h_den_el_c_pt->Fill(LepPt);
	if(LepIsTight) h_num_el_c_pt->Fill(LepPt);
      }
      else if(LepSourceFlavor==2){//bottom
	h_den_el_b_pt->Fill(LepPt);
	if(LepIsTight) h_num_el_b_pt->Fill(LepPt);
      }
      else if(LepSourceFlavor==3){//fake
	h_den_el_f_pt->Fill(LepPt);
	if(LepIsTight) h_num_el_f_pt->Fill(LepPt);
      }
      else if(LepSourceFlavor==4){//unmatched
	h_den_el_u_pt->Fill(LepPt);
	if(LepIsTight) h_num_el_u_pt->Fill(LepPt);
      }




    }//end check on electron

    else{//muon      
      if(LepSourceFlavor==0){//light
	h_den_mu_l_eta->Fill(LepEta);
	if(LepIsTight) h_num_mu_l_eta->Fill(LepEta);
      }
      else if(LepSourceFlavor==1){//charm
	h_den_mu_c_eta->Fill(LepEta);
	if(LepIsTight) h_num_mu_c_eta->Fill(LepEta);
      }
      else if(LepSourceFlavor==2){//bottom
	h_den_mu_b_eta->Fill(LepEta);
	if(LepIsTight) h_num_mu_b_eta->Fill(LepEta);
      }
      else if(LepSourceFlavor==3){//fake
	h_den_mu_f_eta->Fill(LepEta);
	if(LepIsTight) h_num_mu_f_eta->Fill(LepEta);
      }
      else if(LepSourceFlavor==4){//unmatched
	h_den_mu_u_eta->Fill(LepEta);
	if(LepIsTight) h_num_mu_u_eta->Fill(LepEta);
      }
      //pt
      if(LepSourceFlavor==0){//light
	h_den_mu_l_pt->Fill(LepPt);
	if(LepIsTight) h_num_mu_l_pt->Fill(LepPt);
      }
      else if(LepSourceFlavor==1){//charm
	h_den_mu_c_pt->Fill(LepPt);
	if(LepIsTight) h_num_mu_c_pt->Fill(LepPt);
      }
      else if(LepSourceFlavor==2){//bottom
	h_den_mu_b_pt->Fill(LepPt);
	if(LepIsTight) h_num_mu_b_pt->Fill(LepPt);
      }
      else if(LepSourceFlavor==3){//fake
	h_den_mu_f_pt->Fill(LepPt);
	if(LepIsTight) h_num_mu_f_pt->Fill(LepPt);
      }
      else if(LepSourceFlavor==4){//unmatched
	h_den_mu_u_pt->Fill(LepPt);
	if(LepIsTight) h_num_mu_u_pt->Fill(LepPt);
      }
      
    }//end check on lep is muon
    
  }//end event loop


  TH1F* h_rate_el_l_eta = (TH1F*) h_num_el_l_eta->Clone("h_rate_el_l_eta");
  h_rate_el_l_eta->Divide(h_den_el_l_eta);
  TH1F* h_rate_el_c_eta = (TH1F*) h_num_el_c_eta->Clone("h_rate_el_c_eta");
  h_rate_el_c_eta->Divide(h_den_el_c_eta);
  TH1F* h_rate_el_b_eta = (TH1F*) h_num_el_b_eta->Clone("h_rate_el_b_eta");
  h_rate_el_b_eta->Divide(h_den_el_b_eta);
  TH1F* h_rate_el_u_eta = (TH1F*) h_num_el_u_eta->Clone("h_rate_el_u_eta");
  h_rate_el_u_eta->Divide(h_den_el_u_eta);
  TH1F* h_rate_el_f_eta = (TH1F*) h_num_el_f_eta->Clone("h_rate_el_f_eta");
  h_rate_el_f_eta->Divide(h_den_el_f_eta);

  TH1F* h_rate_mu_l_eta = (TH1F*) h_num_mu_l_eta->Clone("h_rate_mu_l_eta");
  h_rate_mu_l_eta->Divide(h_den_mu_l_eta);
  TH1F* h_rate_mu_c_eta = (TH1F*) h_num_mu_c_eta->Clone("h_rate_mu_c_eta");
  h_rate_mu_c_eta->Divide(h_den_mu_c_eta);
  TH1F* h_rate_mu_b_eta = (TH1F*) h_num_mu_b_eta->Clone("h_rate_mu_b_eta");
  h_rate_mu_b_eta->Divide(h_den_mu_b_eta);
  TH1F* h_rate_mu_u_eta = (TH1F*) h_num_mu_u_eta->Clone("h_rate_mu_u_eta");
  h_rate_mu_u_eta->Divide(h_den_mu_u_eta);
  TH1F* h_rate_mu_f_eta = (TH1F*) h_num_mu_f_eta->Clone("h_rate_mu_f_eta");
  h_rate_mu_f_eta->Divide(h_den_mu_f_eta);


  outFile->WriteTObject(h_num_el_l_eta);
  outFile->WriteTObject(h_num_el_c_eta);
  outFile->WriteTObject(h_num_el_b_eta);
  outFile->WriteTObject(h_num_el_u_eta);
  outFile->WriteTObject(h_num_el_f_eta);
  outFile->WriteTObject(h_num_el_l_pt);
  outFile->WriteTObject(h_num_el_c_pt);
  outFile->WriteTObject(h_num_el_b_pt);
  outFile->WriteTObject(h_num_el_u_pt);
  outFile->WriteTObject(h_num_el_f_pt);

  outFile->WriteTObject(h_num_mu_l_eta);
  outFile->WriteTObject(h_num_mu_c_eta);
  outFile->WriteTObject(h_num_mu_b_eta);
  outFile->WriteTObject(h_num_mu_u_eta);
  outFile->WriteTObject(h_num_mu_f_eta);
  outFile->WriteTObject(h_num_mu_l_pt);
  outFile->WriteTObject(h_num_mu_c_pt);
  outFile->WriteTObject(h_num_mu_b_pt);
  outFile->WriteTObject(h_num_mu_u_pt);
  outFile->WriteTObject(h_num_mu_f_pt);

  outFile->WriteTObject(h_rate_el_l_eta);
  outFile->WriteTObject(h_rate_el_c_eta);
  outFile->WriteTObject(h_rate_el_b_eta);
  outFile->WriteTObject(h_rate_el_u_eta);
  outFile->WriteTObject(h_rate_el_f_eta);
  outFile->WriteTObject(h_rate_mu_l_eta);
  outFile->WriteTObject(h_rate_mu_c_eta);
  outFile->WriteTObject(h_rate_mu_b_eta);
  outFile->WriteTObject(h_rate_mu_u_eta);
  outFile->WriteTObject(h_rate_mu_f_eta);



  //plot
  TCanvas* c = new TCanvas();
  TGraphAsymmErrors* g_el_l_eta = new TGraphAsymmErrors(h_num_el_l_eta,h_den_el_l_eta);
  g_el_l_eta->Draw("apl");
  g_el_l_eta->GetYaxis()->SetRangeUser(0,0.5);
  c->Print("ElectronFakeRate_vEta_LightQuarks.pdf");
  TGraphAsymmErrors* g_el_c_eta = new TGraphAsymmErrors(h_num_el_c_eta,h_den_el_c_eta);
  g_el_c_eta->Draw("apl");
  g_el_c_eta->GetYaxis()->SetRangeUser(0,0.5);
  c->Print("ElectronFakeRate_vEta_CharmQuarks.pdf");
  TGraphAsymmErrors* g_el_b_eta = new TGraphAsymmErrors(h_num_el_b_eta,h_den_el_b_eta);
  g_el_b_eta->Draw("apl");
  g_el_b_eta->GetYaxis()->SetRangeUser(0,0.5);
  c->Print("ElectronFakeRate_vEta_BottomQuarks.pdf");
  TGraphAsymmErrors* g_el_u_eta = new TGraphAsymmErrors(h_num_el_u_eta,h_den_el_u_eta);
  g_el_u_eta->Draw("apl");
  g_el_u_eta->GetYaxis()->SetRangeUser(0,0.5);
  c->Print("ElectronFakeRate_vEta_Unmatched.pdf");
  TGraphAsymmErrors* g_el_f_eta = new TGraphAsymmErrors(h_num_el_f_eta,h_den_el_f_eta);
  g_el_f_eta->Draw("apl");
  g_el_f_eta->GetYaxis()->SetRangeUser(0,0.5);
  c->Print("ElectronFakeRate_vEta_Fakes.pdf");

  TGraphAsymmErrors* g_el_l_pt = new TGraphAsymmErrors(h_num_el_l_pt,h_den_el_l_pt);
  g_el_l_pt->Draw("apl");
  c->Print("ElectronFakeRate_vPt_LightQuarks.pdf");
  TGraphAsymmErrors* g_el_c_pt = new TGraphAsymmErrors(h_num_el_c_pt,h_den_el_c_pt);
  g_el_c_pt->Draw("apl");
  c->Print("ElectronFakeRate_vPt_CharmQuarks.pdf");
  TGraphAsymmErrors* g_el_b_pt = new TGraphAsymmErrors(h_num_el_b_pt,h_den_el_b_pt);
  g_el_b_pt->Draw("apl");
  c->Print("ElectronFakeRate_vPt_BottomQuarks.pdf");
  TGraphAsymmErrors* g_el_u_pt = new TGraphAsymmErrors(h_num_el_u_pt,h_den_el_u_pt);
  g_el_u_pt->Draw("apl");
  c->Print("ElectronFakeRate_vPt_Unmatched.pdf");
  TGraphAsymmErrors* g_el_f_pt = new TGraphAsymmErrors(h_num_el_f_pt,h_den_el_f_pt);
  g_el_f_pt->Draw("apl");
  c->Print("ElectronFakeRate_vPt_Fakes.pdf");


  TGraphAsymmErrors* g_mu_l_eta = new TGraphAsymmErrors(h_num_mu_l_eta,h_den_mu_l_eta);
  g_mu_l_eta->Draw("apl");
  g_mu_l_eta->GetYaxis()->SetRangeUser(0,0.5);
  c->Print("MuonFakeRate_vEta_LightQuarks.pdf");
  TGraphAsymmErrors* g_mu_c_eta = new TGraphAsymmErrors(h_num_mu_c_eta,h_den_mu_c_eta);
  g_mu_c_eta->Draw("apl");
  g_mu_c_eta->GetYaxis()->SetRangeUser(0,0.5);
  c->Print("MuonFakeRate_vEta_CharmQuarks.pdf");
  TGraphAsymmErrors* g_mu_b_eta = new TGraphAsymmErrors(h_num_mu_b_eta,h_den_mu_b_eta);
  g_mu_b_eta->Draw("apl");
  g_mu_b_eta->GetYaxis()->SetRangeUser(0,0.5);
  c->Print("MuonFakeRate_vEta_BottomQuarks.pdf");
  TGraphAsymmErrors* g_mu_u_eta = new TGraphAsymmErrors(h_num_mu_u_eta,h_den_mu_u_eta);
  g_mu_u_eta->Draw("apl");
  g_mu_u_eta->GetYaxis()->SetRangeUser(0,0.5);
  c->Print("MuonFakeRate_vEta_Unmatched.pdf");
  TGraphAsymmErrors* g_mu_f_eta = new TGraphAsymmErrors(h_num_mu_f_eta,h_den_mu_f_eta);
  g_mu_f_eta->Draw("apl");
  g_mu_f_eta->GetYaxis()->SetRangeUser(0,0.5);
  c->Print("MuonFakeRate_vEta_Fakes.pdf");

  TGraphAsymmErrors* g_mu_l_pt = new TGraphAsymmErrors(h_num_mu_l_pt,h_den_mu_l_pt);
  g_mu_l_pt->Draw("apl");
  c->Print("MuonFakeRate_vPt_LightQuarks.pdf");
  TGraphAsymmErrors* g_mu_c_pt = new TGraphAsymmErrors(h_num_mu_c_pt,h_den_mu_c_pt);
  g_mu_c_pt->Draw("apl");
  c->Print("MuonFakeRate_vPt_CharmQuarks.pdf");
  TGraphAsymmErrors* g_mu_b_pt = new TGraphAsymmErrors(h_num_mu_b_pt,h_den_mu_b_pt);
  g_mu_b_pt->Draw("apl");
  c->Print("MuonFakeRate_vPt_BottomQuarks.pdf");
  TGraphAsymmErrors* g_mu_u_pt = new TGraphAsymmErrors(h_num_mu_u_pt,h_den_mu_u_pt);
  g_mu_u_pt->Draw("apl");
  c->Print("MuonFakeRate_vPt_Unmatched.pdf");
  TGraphAsymmErrors* g_mu_f_pt = new TGraphAsymmErrors(h_num_mu_f_pt,h_den_mu_f_pt);
  g_mu_f_pt->Draw("apl");
  c->Print("MuonFakeRate_vPt_Fakes.pdf");

}
