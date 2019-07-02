#include <stdio.h>
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TLatex.h"
#include "TGraphAsymmErrors.h"
using namespace std;

TGraphAsymmErrors* getEtaGraph(TTree* t, float looseMiniIsoCut,float tightMiniIsoCut,bool mu){

  string cutstring = Form("MET < 25 && mT < 25 && LepPt > 25 && LepPt < 35 && LepIsTight == 1 && LepMiniIso < %f",tightMiniIsoCut);
  string loosecut = Form("MET < 25 && mT < 25 && LepPt > 25 && LepPt < 35 && LepMiniIso < %f",looseMiniIsoCut);
//   float muEtabins[11]{-2.4,-2.1,-1.2,-0.9,-0.4,0.0,0.4,0.9,1.2,2.1,2.4};
//   float elEtabins[11]={-2.4,-1.566,-1.4442,-0.8,-0.4,0.0,0.4,0.8,1.4442,1.566,2.4};
  float muEtabins[10]{-2.4,-2.1,-1.2,-0.9,-0.4,0.4,0.9,1.2,2.1,2.4};
//   float elEtabins[10]={-2.4,-1.57,-1.44,-0.8,-0.4,0.4,0.8,1.44,1.57,2.4};
  float elEtabins[8]={-2.4,-1.57,-0.8,-0.4,0.4,0.8,1.57,2.4};
  TH1F* num;
  TH1F* den;
//   if(mu) {num = new TH1F("num","",10,muEtabins);den = new TH1F("den","",10,muEtabins);}
//   else {num = new TH1F("num","",10,elEtabins);den = new TH1F("den","",10,elEtabins);}
  if(mu) {num = new TH1F("num","",9,muEtabins);den = new TH1F("den","",9,muEtabins);}
  else {num = new TH1F("num","",7,elEtabins);den = new TH1F("den","",7,elEtabins);}
  t->Project("num","LepEta",cutstring.c_str());
  t->Project("den","LepEta",loosecut.c_str());

  TGraphAsymmErrors* g = new TGraphAsymmErrors(num,den);


  for(int i=0; i< 9;i++){
  	if(mu){
  	std::cout << "eta bin: " << muEtabins[i] << " to " << muEtabins[i+1]  << ", FR: "<< g->GetY()[i] << " +/- " << g->GetErrorY(i) <<std::endl;  	
  	}
  	else{
  	std::cout << "eta bin: " << elEtabins[i] << " to " << elEtabins[i+1]  << ", FR: "<< g->GetY()[i] << " +/- " << g->GetErrorY(i) <<std::endl;
  	}
  }  

  delete num; delete den;
  return g;
}

TGraphAsymmErrors* getPtGraph(TTree* t, float looseMiniIsoCut,float tightMiniIsoCut,bool mu){

  string cutstring = Form("MET < 25 && mT < 25 && LepPt > 25 && LepPt < 35 && LepIsTight == 1 && LepMiniIso < %f",tightMiniIsoCut);
  string loosecut = Form("MET < 25 && mT < 25 && LepPt > 25 && LepPt < 35 && LepMiniIso < %f",looseMiniIsoCut);
  TH1F* num= new TH1F("num","",20,0,200);
  TH1F* den= new TH1F("den","",20,0,200);
  t->Project("num","LepPt",cutstring.c_str());
  t->Project("den","LepPt",loosecut.c_str());

  TGraphAsymmErrors* g = new TGraphAsymmErrors(num,den);


  for(int i=0; i< 20;i++){
  	std::cout << "pt bin: " << g->GetX()[i] << ", FR: "<< g->GetY()[i] << " +/- " << g->GetErrorY(i) <<std::endl;
  }  

  delete num; delete den;
  return g;
}

std::pair<float,float> getFakeRate(TTree* t,float looseMiniIsoCut,float tightMiniIsoCut){
  string cutstring = Form("MET < 25 && mT < 25 && LepPt > 25 && LepPt < 35 && LepIsTight == 1 && LepMiniIso < %f",tightMiniIsoCut);
  string loosecut = Form("MET < 25 && mT < 25 && LepPt > 25 && LepPt < 35 && LepMiniIso < %f",looseMiniIsoCut);
  TH1F* num = new TH1F("num","",1,0,10000);
  TH1F* den = new TH1F("den","",1,0,10000);
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

void fr_2017data(TString trig_){

  TString elID = "MVA2017TightV2RC";  

  TString trig = trig_;
//   std::string trig = "nonIsoHTTrig";
//   std::string trig = "isoTrig_forTrilep";

  // Fail safe
  if(trig=="")trig = "nonIsoHTTrig";  //default
  if(trig != "nonIsoHTTrig" && trig != "isoTrig_forTrilep"){
      std::cout<<"Trigger selecton not recognized."<<std::endl;
      std::cout<<"Choose between nonIsoHTTrig and isoTrig_forTrilep -- June 29, 2019."<<std::endl;      
      gApplication->Terminate();
      
    }

  std::cout << "Trigger: " << trig << std::endl;

  std::vector<TString> eras; 
//   eras.push_back("2017C");
//   eras.push_back("2017D");
//   eras.push_back("2017E");
  eras.push_back("2017F");
//   eras.push_back("All");

  // Input directory
  TString indir = "FakeRate_"+trig+"/";
  TString workDir = "/uscms_data/d3/rsyarif/Brown2018/TT_BB_SSDL_Analyzer_2017data/CMSSW_9_4_11/src/Analyzer/test/"+indir;
  
  // Make output directoty
  TString outDir = "plots_06282019_"+trig+"/"; //all era's
  system("mkdir -vp "+outDir);

  // Electron
  for(unsigned int i=0; i < eras.size(); i++){
    TString era = eras.at(i);
    std::cout<<"working on era: "<<era<<std::endl;
    TFile* fEle = new TFile(workDir+"FakeRate_Data_"+era+"Electrons_"+elID+"_2017dataset_"+trig+".root");
    TTree* tEle = (TTree*) fEle->Get("FakeRate");
    //make output file to save graph
    TFile* fout = new TFile(outDir+"FakeRate_Graph_"+era+"_"+elID+".root","RECREATE");
    TGraphErrors* gEle = new TGraphErrors(40);
    gEle->SetName("ElectronFakeRate");
    for(int i =1; i<41; i++){
      float cut = 0.01*i;
      std::pair<float,float> rateAndErr = getFakeRate(tEle,0.4,cut);
      float fakerate = rateAndErr.first;
      float err = rateAndErr.second;
      gEle->SetPoint(i,cut,fakerate);
      gEle->SetPointError(i,0.,err);
      std::cout<<"Electron Fake Rate for miniIso cut < "<<cut<<" is: "<<fakerate<< " +/- " << err<<std::endl;
    }
    
    gEle->SetMarkerStyle(22);
    gEle->SetMarkerColor(kBlue);
    gEle->SetTitle("Electron Fake Rate vs MiniIsolation Requirement;I_{mini} Requirement;Fake Rate");
    TCanvas* c = new TCanvas();
    gEle->Draw("apl");
    c->Print(outDir+"ElectronFakeRate-vs-MiniIso_"+era+"_"+elID+"_"+trig+".pdf");
    
    //getEtagraphs for 0.1 cut
    std::cout<< "\nEl:"<<std::endl;
    TGraphAsymmErrors* gEtaEl = getEtaGraph(tEle,0.4,0.1,false);
    TCanvas* cEtaEl = new TCanvas();
    gEtaEl->Draw("ap");
    cEtaEl->Print(outDir+"ElectronFakeRate-vs-Eta_"+era+"_"+elID+"_"+trig+".pdf");

    std::cout<< "\nEl:"<<std::endl;
    TGraphAsymmErrors* gPtEl = getPtGraph(tEle,0.4,0.1,false);
    TCanvas* cPtEl = new TCanvas();
    gPtEl->Draw("ap");
    cPtEl->Print(outDir+"ElectronFakeRate-vs-Pt_"+era+"_"+elID+"_"+trig+".pdf");

    //save graphs
    fout->WriteTObject(gEle);
    fout->WriteTObject(gEtaEl,"ElectronEtaFakeRate");
    fout->WriteTObject(gPtEl,"ElectronPtFakeRate");


    // Muon
    TFile* fMu = new TFile(workDir+"FakeRate_Data_"+era+"Muons_CBTightMiniIso_2017dataset_"+trig+".root");
    TTree* tMu = (TTree*) fMu->Get("FakeRate");
    TGraphErrors* gMu = new TGraphErrors(41);
    gMu->SetName("MuonFakeRate");
    for(int i =1; i<41; i++){
      float cut = 0.01*i;
      std::pair<float,float> rateAndErr = getFakeRate(tMu,0.4,cut);
      float fakerate = rateAndErr.first;
      float err = rateAndErr.second;
      gMu->SetPoint(i,cut,fakerate);
      gMu->SetPointError(i,0.,err);
      std::cout<<"Muon Fake Rate for miniIso cut < "<<cut<<" is: "<<fakerate<<" +/- " << err<<std::endl;
    }
    
    gMu->SetMarkerStyle(22);
    gMu->SetMarkerColor(kBlue);
    gMu->SetTitle("Muon Fake Rate vs MiniIsolation Requirement;I_{mini} Requirement;Fake Rate");
    TCanvas* c1 = new TCanvas();
    gMu->Draw("apl");
    c1->Print(outDir+"MuonFakeRate-vs-MiniIso_"+era+"_"+trig+".pdf");

    std::cout<< "\nMu:"<<std::endl;
    TGraphAsymmErrors* gEtaMu = getEtaGraph(tMu,0.4,0.1,true);
    TCanvas* cEtaMu = new TCanvas();
    gEtaMu->Draw("ap");
    cEtaMu->Print(outDir+"MuonFakeRate-vs-Eta_"+era+"_"+elID+"_"+trig+".pdf");

    std::cout<< "\nMu:"<<std::endl;
    TGraphAsymmErrors* gPtMu = getPtGraph(tMu,0.4,0.1,true);
    TCanvas* cPtMu = new TCanvas();
    gPtMu->Draw("ap");
    cPtMu->Print(outDir+"MuonFakeRate-vs-Pt_"+era+"_"+elID+"_"+trig+".pdf");

    //save graphs
    fout->WriteTObject(gMu);
    fout->WriteTObject(gEtaMu,"MuonEtaFakeRate");
    fout->WriteTObject(gPtMu,"MuonPtFakeRate");


    fout->Close();
  }
  
  gApplication->Terminate();

}
