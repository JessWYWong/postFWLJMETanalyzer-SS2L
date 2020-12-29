#include <stdio.h>
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TLatex.h"
#include "TGraphAsymmErrors.h"
#include <string>
using namespace std;

TGraphAsymmErrors* getEtaGraph(TTree* t, float looseMiniIsoCut,float tightMiniIsoCut,bool mu){

  string cutstring = Form("LepIsTight == 1 && LepMiniIso < %f",tightMiniIsoCut);
  string loosecut = Form("LepMiniIso < %f",looseMiniIsoCut);
  //float muEtabins[11]={-2.4,-2.1,-1.2,-0.9,-0.4,0.0,0.4,0.9,1.2,2.1,2.4}; //2016 binning
  //float elEtabins[11]={-2.4,-1.566,-1.4442,-0.8,-0.4,0.0,0.4,0.8,1.4442,1.566,2.4}; //2016 binning
  float muEtabins[10]={-2.4,-2.1,-1.2,-0.9,-0.4,0.4,0.9,1.2,2.1,2.4};
  float elEtabins[10]={-2.4,-1.566,-1.4442,-0.8,-0.4,0.4,0.8,1.4442,1.566,2.4};
  
  TH1D* num;
  TH1D* den;
  //if(mu) {num = new TH1D("num","",10,muEtabins);den = new TH1D("den","",10,muEtabins);} //2016 binning
  //else {num = new TH1D("num","",10,elEtabins);den = new TH1D("den","",10,elEtabins);} //2016 binning
  if(mu) {num = new TH1D("num","Muon Prompt Rate v.s. #eta",9,muEtabins);den = new TH1D("den","",9,muEtabins);}
  else {num = new TH1D("num","Electron Prompt Rate v.s. #eta",9,elEtabins);den = new TH1D("den","",9,elEtabins);}
  t->Project("num","LepEta",cutstring.c_str());
  t->Project("den","LepEta",loosecut.c_str());

  TGraphAsymmErrors* g = new TGraphAsymmErrors(num,den);
  g->GetYaxis()->SetRangeUser(0.,1.1);
  g->GetYaxis()->SetTitle("Prompt Rate");
  g->GetXaxis()->SetTitle("#eta");
  g->GetYaxis()->SetTitleSize(0.035);
  g->GetXaxis()->SetTitleSize(0.035);

  double x,y;
  for(int i=0; i< g->GetN();i++){
  	if(!mu){
  	std::cout << "eta bin: " << elEtabins[i] << " to " << elEtabins[i+1]  << ", PR: "<< g->GetY()[i] << "+/-" << g->GetErrorY(i) <<std::endl;
  	}
  	else{
  	std::cout << "eta bin: " << muEtabins[i] << " to " << muEtabins[i+1]  << ", PR: "<< g->GetY()[i] << "+/-" << g->GetErrorY(i) <<std::endl;  	
  	}
        g->GetPoint(i,x,y);
        //std::cout << "check point "<<i<<" : x="<<x<<" , y="<<y<<std::endl;
  }  
  std::cout << "mean = " << g->GetMean(2)<<  " ; RMS = " << g->GetRMS(2) <<std::endl;
  delete num; delete den;
  return g;
}

TGraphAsymmErrors* getPtGraph(TTree* t, float looseMiniIsoCut,float tightMiniIsoCut, bool mu){

  string cutstring = Form("LepIsTight == 1 && LepMiniIso < %f",tightMiniIsoCut);
  string loosecut = Form("LepMiniIso < %f",looseMiniIsoCut);
//   float ptbins[15] = {30,40,50,60,70,80,90,100,125,150,200,300,400,500,1000};
//   TH1D* num = new TH1D("num","PromptRate",14,ptbins);  
//   TH1D* den = new TH1D("den","den",14,ptbins);
  //float ptbins[16] = {20,30,40,50,60,70,80,90,100,125,150,200,300,400,500,1000};
  //TH1D* num = new TH1D("num","PromptRate",15,ptbins);
  //TH1D* den = new TH1D("den","den",15,ptbins);
  float ptbins[15] = {30,40,50,60,70,80,90,100,125,150,200,300,400,500,1000};
  
  TH1D* num;
  if(mu) num = new TH1D("num","Muon Prompt Rate v.s. P_{T}",14,ptbins);  
  else num = new TH1D("num","Electron Prompt Rate v.s. P_{T}",14,ptbins);
  TH1D* den = new TH1D("den","den",14,ptbins);
  t->Project("num","LepPt",cutstring.c_str());
  //std::cout << "check num "<<num->GetXaxis()->GetNbins()<<std::endl;
  t->Project("den","LepPt",loosecut.c_str());
  //std::cout << "check den "<<den->GetXaxis()->GetNbins()<<std::endl;
  //for(int i=0; i< den->GetXaxis()->GetNbins();i++){
  //     std::cout << "den (loose) pt bin: "<< ptbins[i]<<"-" << ptbins[i+1] << " , bin content: "<<den->GetBinContent(i+1)<<std::endl;
  //}

  TGraphAsymmErrors* g = new TGraphAsymmErrors(num,den);
  g->GetYaxis()->SetRangeUser(0.,1.1);
  g->GetYaxis()->SetTitle("Prompt Rate");
  g->GetXaxis()->SetTitle("p_{T} (GeV)");
  g->GetYaxis()->SetTitleSize(0.035);
  g->GetXaxis()->SetTitleSize(0.035);
  double x,y;
  for(int i=0; i< g->GetN();i++){
  	std::cout << "pt bin: " << ptbins[i] << "-" << ptbins[i+1] << ", PR: "<< g->GetY()[i] << "+/-" << g->GetErrorY(i) <<std::endl;
        g->GetPoint(i,x,y);
        //std::cout << "check point "<<i<<" : x="<<x<<" , y="<<y<<std::endl;
  }  
  std::cout << "mean = " << g->GetMean(2)<<  " ; RMS = " << g->GetRMS(2) <<std::endl;

  delete num; delete den;
  return g;
}

/*
std::pair<float,float> getPromptRate(TTree* t,float looseMiniIsoCut,float tightMiniIsoCut){
  string cutstring = Form("LepIsTight == 1 && LepMiniIso < %f",tightMiniIsoCut);
  string loosecut = Form("LepMiniIso < %f",looseMiniIsoCut);
  TH1D* num = new TH1D("num","",1,0,10000);
  TH1D* den = new TH1D("den","",1,0,10000);
  t->Project("num","LepPt",cutstring.c_str());
  t->Project("den","LepPt",loosecut.c_str());
  TGraphAsymmErrors* g = new TGraphAsymmErrors(num,den);  
  float pr = g->GetY()[0];
  float err = g->GetErrorY(0);
  std::pair<float,float> pair;
  pair.first = pr;
  pair.second = err;
  delete den; delete num;
  return pair;
}
*/
void pr_2017data(TString trig_, int doEl_=1, int doMu_=1){

  //std::string elID = "MVA2017TightV2IsoRC";
  //std::string elID = "MVA2017TightV2RC";
  std::string elID ="MVA2017TightV2IsoTightRC";
  //std::string muID = "CBTightMiniIso";
  //std::string muID = "CBTight";
  std::string muID = "CBTightMiniIsoTight";
  //std::string trig = "nonIsoHTTrigs";
  //std::string trig = "IsoTrigs_forTrilep";
  TString trig = trig_;
  
    // Fail safe
  if(trig=="")trig = "nonIsoHTTrig";  //default
  if(trig != "nonIsoHTTrig" && trig != "isoTrig_forTrilep"){
      std::cout<<"Trigger selecton not recognized."<<std::endl;
      gApplication->Terminate();      
    }
  
  std::cout << "Trigger: " << trig << std::endl;
  std::cout << "doEl: " << doEl_ << std::endl;
  std::cout << "doMu: " << doMu_ << std::endl;
  
  bool doEl = doEl_;
  bool doMu = doMu_;
  
  std::vector<TString> eras; 
//   eras.push_back("2017B");
//   eras.push_back("2017C");
//   eras.push_back("2017D");
//   eras.push_back("2017E");
//  eras.push_back("2017F");
   eras.push_back("All");

  // Make output directoty
  TString outDir = "Outputs_082020_"+trig+"/"; //all era's
  system("mkdir -vp "+outDir);
  
  //Input folder
  //TString path = "../../test/PromptRatev2_"+trig;
  TString path = "../../test/PromptRate_082020__"+trig+"_passTrig";

  for(unsigned int i=0; i < eras.size(); i++){

    TString era = eras.at(i);
    std::cout<<"working on era: "<<era<<std::endl;

    //make output file to save graph
    TFile* fout = new TFile(outDir+"/"+"PromptRate_Graph_"+era+"_"+elID+"_"+trig+".root","RECREATE");

    /// ELECTRON :
    
    if(doEl){

    //make output file
	TString el_filename;
	el_filename = "/PromptRate_Data_"+era+"_Electrons_"+elID+"_SortByPhi_"+trig+".root";
    TFile* fEle = new TFile(path+el_filename);
    TTree* tEle = (TTree*) fEle->Get("FakeRate");
    //TTree* tEle = (TTree*) fEle->Get("PromptRate");
//     TGraphErrors* gEle = new TGraphErrors(40);
//     gEle->SetName("ElectronPromptRate");
//     for(int i =1; i<41; i++){
//       float cut = 0.01*i;
//       std::pair<float,float> rateAndErr = getPromptRate(tEle,0.4,cut);
//       float fakerate = rateAndErr.first;
//       float err = rateAndErr.second;
//       gEle->SetPoint(i,cut,fakerate);
//       gEle->SetPointError(i,0.,err);
//       std::cout<<"Electron Prompt Rate for miniIso cut < "<<cut<<" is: "<<fakerate<< " +/- "<< err <<std::endl;
//     }
//     
//     gEle->SetMarkerStyle(22);
//     gEle->SetMarkerColor(kBlue);
//     gEle->SetTitle("Electron Prompt Rate vs MiniIsolation Requirement;I_{mini} Requirement;Prompt Rate");
//     TCanvas* c = new TCanvas();
//     gEle->Draw("apl");
//     c->Print(outDir+"ElectronPromptRate-vs-MiniIso_"+era+"_"+elID+"_"+trig+".pdf");

    //getEtagraphs for 0.1 cut
    TGraphAsymmErrors* gEtaEl = getEtaGraph(tEle,0.4,0.1,false);
    TCanvas* cEtaEl = new TCanvas();
    gEtaEl->Draw("apl");
    cEtaEl->Print(outDir+"ElectronPromptRate-vs-Eta_"+era+"_"+elID+"_"+trig+".pdf");
    //get pT graphs for 0.1 cut
    TGraphAsymmErrors* gPtEl = getPtGraph(tEle,0.4,0.1,false);
    TCanvas* cPtEl = new TCanvas();
    gPtEl->Draw("apl");
    cPtEl->Print(outDir+"ElectronPromptRate-vs-Pt_"+era+"_"+elID+"_"+trig+".pdf");
    
    //save graphs
    //fout->WriteTObject(gEle);
    fout->WriteTObject(gEtaEl,"ElectronEtaPromptRate");
    fout->WriteTObject(gPtEl,"ElectronPtPromptRate");

    }
    
    if(doMu){
    /// MUON :
    //make output file
	TString mu_filename;
	mu_filename = "/PromptRate_Data_"+era+"_Muons_"+muID+"_SortByPhi_"+trig+".root";
	std::cout << "Opening file:" << mu_filename << std::endl;	

    TFile* fMu = new TFile(path+mu_filename);
    TTree* tMu = (TTree*) fMu->Get("FakeRate");
    //TTree* tMu = (TTree*) fMu->Get("PromptRate");
//     TGraphErrors* gMu = new TGraphErrors(41);
//     gMu->SetName("MuonPromptRate");
//     for(int i =1; i<41; i++){
//       float cut = 0.01*i;
//       std::pair<float,float> rateAndErr = getPromptRate(tMu,0.4,cut);
//       float fakerate = rateAndErr.first;
//       float err = rateAndErr.second;
//       gMu->SetPoint(i,cut,fakerate);
//       gMu->SetPointError(i,0.,err);
//       std::cout<<"Muon Prompt Rate for miniIso cut < "<<cut<<" is: "<<fakerate << " +/- "<< err <<std::endl;
//     }
//     
//     gMu->SetMarkerStyle(22);
//     gMu->SetMarkerColor(kBlue);
//     gMu->SetTitle("Muon Prompt Rate vs MiniIsolation Requirement;I_{mini} Requirement;Prompt Rate");
//     TCanvas* c1 = new TCanvas();
//     gMu->Draw("apl");
//     c1->Print(outDir+"MuonPromptRate-vs-MiniIso_"+era+"_"+trig+".pdf");
// 
    //getEtagraphs for 0.1 cut
    TGraphAsymmErrors* gEtaMu = getEtaGraph(tMu,0.4,0.1,true);
    TCanvas* cEtaMu = new TCanvas();
    gEtaMu->Draw("apl");
    cEtaMu->Print(outDir+"MuonPromptRate-vs-Eta_"+era+"_"+muID+"_"+trig+".pdf");
    //get pT graphs for 0.1 cut
    TGraphAsymmErrors* gPtMu = getPtGraph(tMu,0.4,0.1,true);
    TCanvas* cPtMu = new TCanvas();
    gPtMu->Draw("apl");
    cPtMu->Print(outDir+"MuonPromptRate-vs-Pt_"+era+"_"+muID+"_"+trig+".pdf");

 
     //save graphs
     //fout->WriteTObject(gMu);
     fout->WriteTObject(gEtaMu,"MuonEtaPromptRate");
     fout->WriteTObject(gPtMu,"MuonPtPromptRate");

    }

     fout->Close();
  }
  
  gApplication->Terminate();

}
