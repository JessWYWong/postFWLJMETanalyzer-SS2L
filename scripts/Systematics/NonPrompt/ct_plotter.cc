#include <iostream>
#include <string>
#include "TFile.h"
#include "TH1.h"
#include "tdrstyle.C"

float weight(float fr){
  float weight = fr / (1 - fr);
  return weight;
};

int getBin(TH1F* h, float val){
  int bin = h->FindBin(val);
  return val;
}

void DrawAndSave(TH1F* h_pred, TH1F* h_obs, std::string pdfname, std::string flavor, std::string lepflavor){

  //rebin, currently bins are 5 GeV so rebin by 20 to make 100 GeV bins
  if(flavor.find("qcd")==std::string::npos){
    h_pred->Rebin(20);
    h_obs->Rebin(20);
    h_pred->Sumw2();
    h_obs->Sumw2();
  }
  else{};

  TCanvas* c = new TCanvas();
  c->SetLogy();
  //make tpads
  TPad* pad1 = new TPad("pad1","pad1",0.01,0.2,1,1);
  TPad* pad2 = new TPad("pad2","pad2",0.01,0.,1,0.2);

  pad1->SetTopMargin(0.1);
  pad1->SetLogy();
  pad2->SetTopMargin(0.0);
  pad1->SetBottomMargin(0.0);
  pad2->SetBottomMargin(0.3);
  pad1->Draw();
  pad2->Draw();
  //cd to top pad for drawing
  pad1->cd();

  std::string title = "Predicted vs. Observed H_{T}^{Lep} for "+flavor+" "+lepflavor+";H_{T};N_{Events}";
  h_pred->SetTitle(title.c_str());
  h_pred->SetTitleSize(0.12);
  h_pred->GetYaxis()->SetTitleOffset(1.4);
  h_pred->GetYaxis()->CenterTitle();
  h_pred->SetLineColor(kRed+1);
  h_obs->SetMarkerStyle(20);
  h_obs->SetMarkerColor(kBlack);

  //make lumi weight
  float lumiweight = 59.74*1000 * (687.1 / 100728760);
  if(flavor.find("qcd")==std::string::npos){
    h_pred->Scale(lumiweight);
    h_obs->Scale(lumiweight);
  }
  
  //get fake rate weight
  float frweight=0;
  if(flavor=="light" && lepflavor=="Electrons") frweight = weight(0.161); 
  if(flavor=="charm"&& lepflavor=="Electrons") frweight = weight(0.139); 
  if(flavor=="bottom"&& lepflavor=="Electrons") frweight = weight(0.150); 
  if(flavor=="fake"&& lepflavor=="Electrons") frweight = weight(0.177); 
  if(flavor=="unmatched"&& lepflavor=="Electrons") frweight = weight(0.451); 
  if(flavor=="data"&& lepflavor=="Electrons") frweight = weight(0.205); 

  if(flavor=="qcdlight" && lepflavor=="Electrons") frweight = weight(0.585); 
  if(flavor=="qcdcharm"&& lepflavor=="Electrons") frweight = weight(0.323); 
  if(flavor=="qcdbottom"&& lepflavor=="Electrons") frweight = weight(0.211); 
  if(flavor=="qcdfake"&& lepflavor=="Electrons") frweight = weight(0.374); 
  if(flavor=="qcdunmatched"&& lepflavor=="Electrons") frweight = weight(0.329); 


  if(flavor=="light" && lepflavor=="Muons") frweight = weight(0.048); 
  if(flavor=="charm"&& lepflavor=="Muons") frweight = weight(0.196); 
  if(flavor=="bottom"&& lepflavor=="Muons") frweight = weight(0.164); 
  if(flavor=="fake"&& lepflavor=="Muons") frweight = weight(0.058); 
  if(flavor=="unmatched"&& lepflavor=="Muons") frweight = weight(0.008); 
  if(flavor=="data"&& lepflavor=="Muons") frweight = weight(0.371); 

  if(flavor=="qcdlight" && lepflavor=="Muons") frweight = weight(0.193); 
  if(flavor=="qcdcharm"&& lepflavor=="Muons") frweight = weight(0.330); 
  if(flavor=="qcdbottom"&& lepflavor=="Muons") frweight = weight(0.390); 
  if(flavor=="qcdfake"&& lepflavor=="Muons") frweight = weight(0.354); 
  if(flavor=="qcdunmatched"&& lepflavor=="Muons") frweight = weight(0.466); 



  if(flavor.find("qcd")==std::string::npos) h_pred->Scale(frweight);
  double obserr=0;
  float obs = h_obs->IntegralAndError(9,51,obserr);
  double prederr=0;
  float pred = h_pred->IntegralAndError(9,51,prederr);
  std::cout<<std::fixed<<setprecision(2)<<lepflavor<<"&"<<flavor<<"&$"<<obs<<"\\pm"<<obserr<<"$&$"<<pred<<"\\pm"<<prederr<<"$\\\\"<<std::endl;

  double totobserr=0;
  float totobs = h_obs->IntegralAndError(1,51,totobserr);
  double totprederr=0;
  float totpred = h_pred->IntegralAndError(1,51,totprederr);
  std::cout<<std::fixed<<setprecision(2)<<lepflavor<<"&"<<flavor<<"&$"<<totobs<<"\\pm"<<totobserr<<"$&$"<<totpred<<"\\pm"<<totprederr<<"$\\\\"<<std::endl;


  h_pred->Draw("hist e");
  h_obs->Draw("pesame");

  TLegend* leg = new TLegend(0.4,0.6,0.9,0.8);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  std::string predleg = "Predicted Events for "+flavor+" "+lepflavor;
  std::string obsleg = "Observed Events for "+flavor+" "+lepflavor;
  leg->AddEntry(h_obs,obsleg.c_str(),"p");
  leg->AddEntry(h_pred,predleg.c_str(),"l");
  leg->Draw("same");

  //now go to pad2 and draw ratio
  pad2->cd();
  //sumw2
  h_pred->Sumw2();
  TH1F* h_ratio = (TH1F*) h_obs->Clone("h_ratio");
  h_ratio->Divide(h_pred);
  h_ratio->GetYaxis()->SetRangeUser(0.5,1.5);
  h_ratio->GetYaxis()->SetTitle("Obs/Pred");
  h_ratio->GetYaxis()->SetTitleOffset(0.4);
  h_ratio->GetYaxis()->SetTitleSize(0.12);
  h_ratio->GetYaxis()->CenterTitle();
  h_ratio->GetYaxis()->SetLabelSize(0.10);
  h_ratio->GetXaxis()->SetTitle("H_{T}^{Lep}");
  h_ratio->GetXaxis()->SetTitleSize(0.12);
  h_ratio->GetXaxis()->CenterTitle();
  h_ratio->GetXaxis()->SetLabelSize(0.10);
  h_ratio->GetYaxis()->SetNdivisions(205);
  h_ratio->Draw("pe");
  TLine* baseline = new TLine(0,1,5000,1);
  baseline->SetLineColor(kBlack);
  baseline->SetLineStyle(2);
  baseline->Draw();

  c->Update();
  c->Print(pdfname.c_str());
  delete c;

}

TH1F* getTotPredHist(TFile* f,std::string chan){


  if(chan=="el"){

    TH1F* h_light = (TH1F*) f->Get("h_el_pred_light");
    TH1F* h_charm = (TH1F*) f->Get("h_el_pred_charm");
    TH1F* h_bottom = (TH1F*) f->Get("h_el_pred_bottom");
    TH1F* h_fake = (TH1F*) f->Get("h_el_pred_fake");
    TH1F* h_unm = (TH1F*) f->Get("h_el_pred_unm");
    TH1F* h_tot = (TH1F*)h_light->Clone();
    h_tot->Add(h_charm);
    h_tot->Add(h_bottom);
    h_tot->Add(h_fake);
    h_tot->Add(h_unm);
    
    return h_tot;
  }

  else{
    TH1F* h_light = (TH1F*) f->Get("h_mu_pred_light");
    TH1F* h_charm = (TH1F*) f->Get("h_mu_pred_charm");
    TH1F* h_bottom = (TH1F*) f->Get("h_mu_pred_bottom");
    TH1F* h_fake = (TH1F*) f->Get("h_mu_pred_fake");
    TH1F* h_unm = (TH1F*) f->Get("h_mu_pred_unm");
    TH1F* h_tot = (TH1F*)h_light->Clone();
    h_tot->Add(h_charm);
    h_tot->Add(h_bottom);
    h_tot->Add(h_fake);
    h_tot->Add(h_unm);
    
    return h_tot;
  }


};


TH1F* getTotObsHist(TFile* f,std::string chan){


  if(chan=="el"){

    TH1F* h_light = (TH1F*) f->Get("h_el_obs_light");
    TH1F* h_charm = (TH1F*) f->Get("h_el_obs_charm");
    TH1F* h_bottom = (TH1F*) f->Get("h_el_obs_bottom");
    TH1F* h_fake = (TH1F*) f->Get("h_el_obs_fake");
    TH1F* h_unm = (TH1F*) f->Get("h_el_obs_unm");
    TH1F* h_tot = (TH1F*)h_light->Clone();
    h_tot->Add(h_charm);
    h_tot->Add(h_bottom);
    h_tot->Add(h_fake);
    h_tot->Add(h_unm);
    
    return h_tot;
  }

  else{
    TH1F* h_light = (TH1F*) f->Get("h_mu_obs_light");
    TH1F* h_charm = (TH1F*) f->Get("h_mu_obs_charm");
    TH1F* h_bottom = (TH1F*) f->Get("h_mu_obs_bottom");
    TH1F* h_fake = (TH1F*) f->Get("h_mu_obs_fake");
    TH1F* h_unm = (TH1F*) f->Get("h_mu_obs_unm");
    TH1F* h_tot = (TH1F*)h_light->Clone();
    h_tot->Add(h_charm);
    h_tot->Add(h_bottom);
    h_tot->Add(h_fake);
    h_tot->Add(h_unm);
    
    return h_tot;
  }


};

void ct_plotter(){

  gStyle->SetOptStat(kFALSE);

  setTDRStyle();
  
  TFile* f = new TFile("SmartClosure_TTbar.root");
  
  DrawAndSave((TH1F*) f->Get("h_el_pred_light"), (TH1F*) f->Get("h_el_obs_light"), "Closure_Light_Electrons.pdf","light","Electrons");
  DrawAndSave((TH1F*) f->Get("h_el_pred_charm"), (TH1F*) f->Get("h_el_obs_charm"), "Closure_Charm_Electrons.pdf","charm","Electrons");
  DrawAndSave((TH1F*) f->Get("h_el_pred_bottom"), (TH1F*) f->Get("h_el_obs_bottom"), "Closure_Bottom_Electrons.pdf","bottom","Electrons");
  DrawAndSave((TH1F*) f->Get("h_el_pred_fake"), (TH1F*) f->Get("h_el_obs_fake"), "Closure_Fake_Electrons.pdf","fake","Electrons");
  DrawAndSave((TH1F*) f->Get("h_el_pred_unm"), (TH1F*) f->Get("h_el_obs_unm"), "Closure_UnMatched_Electrons.pdf","unmatched","Electrons");

  //DrawAndSave((TH1F*) f->Get("h_el_pred_light"), (TH1F*) f->Get("h_el_obs_light"), "Closure_LightQCD_Electrons.pdf","qcdlight","Electrons");
  //DrawAndSave((TH1F*) f->Get("h_el_pred_charm"), (TH1F*) f->Get("h_el_obs_charm"), "Closure_CharmQCD_Electrons.pdf","qcdcharm","Electrons");
  //DrawAndSave((TH1F*) f->Get("h_el_pred_bottom"), (TH1F*) f->Get("h_el_obs_bottom"), "Closure_BottomQCD_Electrons.pdf","qcdbottom","Electrons");
  //DrawAndSave((TH1F*) f->Get("h_el_pred_fake"), (TH1F*) f->Get("h_el_obs_fake"), "Closure_FakeQCD_Electrons.pdf","qcdfake","Electrons");
  //DrawAndSave((TH1F*) f->Get("h_el_pred_unm"), (TH1F*) f->Get("h_el_obs_unm"), "Closure_UnMatchedQCD_Electrons.pdf","qcdunmatched","Electrons");


  DrawAndSave((TH1F*) f->Get("h_mu_pred_light"), (TH1F*) f->Get("h_mu_obs_light"), "Closure_Light_Muons.pdf","light","Muons");
  DrawAndSave((TH1F*) f->Get("h_mu_pred_charm"), (TH1F*) f->Get("h_mu_obs_charm"), "Closure_Charm_Muons.pdf","charm","Muons");
  DrawAndSave((TH1F*) f->Get("h_mu_pred_bottom"), (TH1F*) f->Get("h_mu_obs_bottom"), "Closure_Bottom_Muons.pdf","bottom","Muons");
  DrawAndSave((TH1F*) f->Get("h_mu_pred_fake"), (TH1F*) f->Get("h_mu_obs_fake"), "Closure_Fake_Muons.pdf","fake","Muons");
  DrawAndSave((TH1F*) f->Get("h_mu_pred_unm"), (TH1F*) f->Get("h_mu_obs_unm"), "Closure_UnMatched_Muons.pdf","unmatched","Muons");

  //DrawAndSave((TH1F*) f->Get("h_mu_pred_light"), (TH1F*) f->Get("h_mu_obs_light"), "Closure_LightQCD_Muons.pdf","qcdlight","Muons");
  //DrawAndSave((TH1F*) f->Get("h_mu_pred_charm"), (TH1F*) f->Get("h_mu_obs_charm"), "Closure_CharmQCD_Muons.pdf","qcdcharm","Muons");
  //DrawAndSave((TH1F*) f->Get("h_mu_pred_bottom"), (TH1F*) f->Get("h_mu_obs_bottom"), "Closure_BottomQCD_Muons.pdf","qcdbottom","Muons");
  //DrawAndSave((TH1F*) f->Get("h_mu_pred_fake"), (TH1F*) f->Get("h_mu_obs_fake"), "Closure_FakeQCD_Muons.pdf","qcdfake","Muons");
  //DrawAndSave((TH1F*) f->Get("h_mu_pred_unm"), (TH1F*) f->Get("h_mu_obs_unm"), "Closure_UnMatchedQCD_Muons.pdf","qcdunmatched","Muons");


  //*************now draw pT*******************


  /*  DrawAndSave((TH1F*) f->Get("h_pt_el_pred_light"), (TH1F*) f->Get("h_pt_el_obs_light"), "Closure_Light_Electrons_LepPT.pdf","light","Electrons");
  DrawAndSave((TH1F*) f->Get("h_pt_el_pred_charm"), (TH1F*) f->Get("h_pt_el_obs_charm"), "Closure_Charm_Electrons_LepPT.pdf","charm","Electrons");
  DrawAndSave((TH1F*) f->Get("h_pt_el_pred_bottom"), (TH1F*) f->Get("h_pt_el_obs_bottom"), "Closure_Bottom_Electrons_LepPT.pdf","bottom","Electrons");
  DrawAndSave((TH1F*) f->Get("h_pt_el_pred_fake"), (TH1F*) f->Get("h_pt_el_obs_fake"), "Closure_Fake_Electrons_LepPT.pdf","fake","Electrons");
  DrawAndSave((TH1F*) f->Get("h_pt_el_pred_unm"), (TH1F*) f->Get("h_pt_el_obs_unm"), "Closure_UnMatched_Electrons_LepPT.pdf","unmatched","Electrons");

  //DrawAndSave((TH1F*) f->Get("h_pt_el_pred_light"), (TH1F*) f->Get("h_pt_el_obs_light"), "Closure_LightQCD_Electrons_LepPT.pdf","qcdlight","Electrons");
  //DrawAndSave((TH1F*) f->Get("h_pt_el_pred_charm"), (TH1F*) f->Get("h_pt_el_obs_charm"), "Closure_CharmQCD_Electrons_LepPT.pdf","qcdcharm","Electrons");
  //DrawAndSave((TH1F*) f->Get("h_pt_el_pred_bottom"), (TH1F*) f->Get("h_pt_el_obs_bottom"), "Closure_BottomQCD_Electrons_LepPT.pdf","qcdbottom","Electrons");
  //DrawAndSave((TH1F*) f->Get("h_pt_el_pred_fake"), (TH1F*) f->Get("h_pt_el_obs_fake"), "Closure_FakeQCD_Electrons_LepPT.pdf","qcdfake","Electrons");
  //DrawAndSave((TH1F*) f->Get("h_pt_el_pred_unm"), (TH1F*) f->Get("h_pt_el_obs_unm"), "Closure_UnMatchedQCD_Electrons_LepPT.pdf","qcdunmatched","Electrons");


  DrawAndSave((TH1F*) f->Get("h_pt_mu_pred_light"), (TH1F*) f->Get("h_pt_mu_obs_light"), "Closure_Light_Muons_LepPT.pdf","light","Muons");
  DrawAndSave((TH1F*) f->Get("h_pt_mu_pred_charm"), (TH1F*) f->Get("h_pt_mu_obs_charm"), "Closure_Charm_Muons_LepPT.pdf","charm","Muons");
  DrawAndSave((TH1F*) f->Get("h_pt_mu_pred_bottom"), (TH1F*) f->Get("h_pt_mu_obs_bottom"), "Closure_Bottom_Muons_LepPT.pdf","bottom","Muons");
  DrawAndSave((TH1F*) f->Get("h_pt_mu_pred_fake"), (TH1F*) f->Get("h_pt_mu_obs_fake"), "Closure_Fake_Muons_LepPT.pdf","fake","Muons");
  DrawAndSave((TH1F*) f->Get("h_pt_mu_pred_unm"), (TH1F*) f->Get("h_pt_mu_obs_unm"), "Closure_UnMatched_Muons_LepPT.pdf","unmatched","Muons");

  //DrawAndSave((TH1F*) f->Get("h_pt_mu_pred_light"), (TH1F*) f->Get("h_pt_mu_obs_light"), "Closure_LightQCD_Muons_LepPT.pdf","qcdlight","Muons");
  //DrawAndSave((TH1F*) f->Get("h_pt_mu_pred_charm"), (TH1F*) f->Get("h_pt_mu_obs_charm"), "Closure_CharmQCD_Muons_LepPT.pdf","qcdcharm","Muons");
  //DrawAndSave((TH1F*) f->Get("h_pt_mu_pred_bottom"), (TH1F*) f->Get("h_pt_mu_obs_bottom"), "Closure_BottomQCD_Muons_LepPT.pdf","qcdbottom","Muons");
  //DrawAndSave((TH1F*) f->Get("h_pt_mu_pred_fake"), (TH1F*) f->Get("h_pt_mu_obs_fake"), "Closure_FakeQCD_Muons_LepPT.pdf","qcdfake","Muons");
  //DrawAndSave((TH1F*) f->Get("h_pt_mu_pred_unm"), (TH1F*) f->Get("h_pt_mu_obs_unm"), "Closure_UnMatchedQCD_Muons_LepPT.pdf","qcdunmatched","Muons");
  */

  f->Close();
  
  TFile* f1 = new TFile("SmartClosure_TTbar.root");

  TH1F* h_pred_tot_el = getTotPredHist(f1,"el");
  TH1F* h_obs_tot_el = getTotObsHist(f1,"el");

  TH1F* h_pred_tot_mu = getTotPredHist(f1,"mu");
  TH1F* h_obs_tot_mu = getTotObsHist(f1,"mu");

  DrawAndSave(h_pred_tot_el,h_obs_tot_el,"Closure_Data_Electrons.pdf","data","Electrons");
  DrawAndSave(h_pred_tot_mu,h_obs_tot_mu,"Closure_Data_Muons.pdf","data","Muons");





}



