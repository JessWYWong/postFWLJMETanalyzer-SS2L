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

TGraphAsymmErrors* getEtaGraph(TTree* t, float looseMiniIsoCut,float tightMiniIsoCut,bool mu, bool allPt){

  string cutstring = Form("nConst>=2 && MET < 25 && mT < 25 && LepIsTight == 1 && LepMiniIso < %f",tightMiniIsoCut);
  string loosecut = Form("nConst>=2 && MET < 25 && mT < 25 && LepMiniIso < %f",looseMiniIsoCut);
  if(!allPt){
      cutstring += " && LepPt > 25 && LepPt < 35";
      loosecut  += " && LepPt > 25 && LepPt < 35";
  }
//   float muEtabins[11]{-2.4,-2.1,-1.2,-0.9,-0.4,0.0,0.4,0.9,1.2,2.1,2.4};
//   float elEtabins[11]={-2.4,-1.566,-1.4442,-0.8,-0.4,0.0,0.4,0.8,1.4442,1.566,2.4};
  float muEtabins[10]{-2.4,-2.1,-1.2,-0.9,-0.4,0.4,0.9,1.2,2.1,2.4};
//   float elEtabins[10]={-2.4,-1.57,-1.44,-0.8,-0.4,0.4,0.8,1.44,1.57,2.4};
  float elEtabins[8]={-2.4,-1.57,-0.8,-0.4,0.4,0.8,1.57,2.4};
  TH1D* num;
  TH1D* den;
//   if(mu) {num = new TH1D("num","",10,muEtabins);den = new TH1D("den","",10,muEtabins);}
//   else {num = new TH1D("num","",10,elEtabins);den = new TH1D("den","",10,elEtabins);}
  if(mu) {num = new TH1D("num","",9,muEtabins);den = new TH1D("den","",9,muEtabins);}
  else {num = new TH1D("num","",7,elEtabins);den = new TH1D("den","",7,elEtabins);}
  std::cout<<"num : "<<cutstring<<std::endl;
  std::cout<<"den : "<<loosecut<<std::endl;
  t->Project("num","LepEta",cutstring.c_str());
  t->Project("den","LepEta",loosecut.c_str());

  TGraphAsymmErrors* g = new TGraphAsymmErrors(num,den);


  for(int i=0; i< 9;i++){
  	if(mu){
  	std::cout << "eta bin: " << muEtabins[i] << " to " << muEtabins[i+1]  << ", FR: "<< g->GetY()[i] << " +/- " << g->GetErrorY(i) <<std::endl;  	
  	}
  	else{
	if(i >= sizeof(elEtabins)/sizeof(*elEtabins)-1) break;
  	std::cout << "eta bin: " << elEtabins[i] << " to " << elEtabins[i+1]  << ", FR: "<< g->GetY()[i] << " +/- " << g->GetErrorY(i) <<std::endl;
  	}
  }  

  delete num; delete den;
  return g;
} 

TGraphAsymmErrors* getPtGraph(TTree* t, float looseMiniIsoCut,float tightMiniIsoCut,bool mu){

  string cutstring = Form("MET < 25 && mT < 25 && LepIsTight == 1 && LepMiniIso < %f",tightMiniIsoCut);
  string loosecut = Form("MET < 25 && mT < 25  && LepMiniIso < %f",looseMiniIsoCut);
  TH1D* num= new TH1D("num","",20,0,200);
  TH1D* den= new TH1D("den","",20,0,200);
  t->Project("num","LepPt",cutstring.c_str());
  t->Project("den","LepPt",loosecut.c_str());

  TGraphAsymmErrors* g = new TGraphAsymmErrors(num,den);


  for(int i=0; i< 20;i++){
  	std::cout << "pt bin: " << g->GetX()[i] << ", FR: "<< g->GetY()[i] << " +/- " << g->GetErrorY(i) <<std::endl;
  }  

  delete num; delete den;
  return g;
}

TGraphAsymmErrors* getnConstGraph(TTree* t, float looseMiniIsoCut,float tightMiniIsoCut,bool mu){

  string cutstring = Form("MET < 25 && mT < 25 && LepIsTight == 1 && LepMiniIso < %f",tightMiniIsoCut);
  string loosecut = Form("MET < 25 && mT < 25  && LepMiniIso < %f",looseMiniIsoCut);
  float nConstbins[5]{1,2,3,4,10};
  TH1D* num= new TH1D("num","",4,nConstbins);
  TH1D* den= new TH1D("den","",4,nConstbins);
  t->Project("num","nConst",cutstring.c_str());
  t->Project("den","nConst",loosecut.c_str());

  TGraphAsymmErrors* g = new TGraphAsymmErrors(num,den);

  for(int i=0; i< 4;i++){
        std::cout << "nConst bin: " << g->GetX()[i] << ", FR: "<< g->GetY()[i] << " +/- " << g->GetErrorY(i) <<std::endl;
  }

  g->GetXaxis()->SetTitle("nConst");
  g->GetYaxis()->SetTitle("Fake Rates");

  delete num; delete den;
  return g;
}

TGraphAsymmErrors* getHTGraph(TTree* t, float looseMiniIsoCut,float tightMiniIsoCut,bool mu){

  string addCut = "";
  if (mu) {addCut = "nConst>=2 && ";}
  string cutstring = Form("MET < 25 && mT < 25 && LepIsTight == 1 && LepMiniIso < %f",tightMiniIsoCut);
  string loosecut = Form("MET < 25 && mT < 25  && LepMiniIso < %f",looseMiniIsoCut);
  float HTbins[6]{30,120,220,320,420,2000};
  TH1D* num= new TH1D("num","",5,HTbins);
  TH1D* den= new TH1D("den","",5,HTbins);
  t->Project("num","HT",cutstring.c_str());
  t->Project("den","HT",loosecut.c_str());

  TGraphAsymmErrors* g = new TGraphAsymmErrors(num,den);

  for(int i=0; i< 5;i++){
        std::cout << "HT bin: " << g->GetX()[i] << ", FR: "<< g->GetY()[i] << " +/- " << g->GetErrorY(i) <<std::endl;
  }

  g->GetXaxis()->SetTitle("H_T");
  g->GetYaxis()->SetTitle("Fake Rates");

  delete num; delete den;
  return g;
}

std::pair<float,float> getFakeRate(TTree* t,float looseMiniIsoCut,float tightMiniIsoCut,bool allPt){
  string cutstring = Form("MET < 25 && mT < 25 && LepIsTight == 1 && LepMiniIso < %f",tightMiniIsoCut);
  string loosecut = Form("MET < 25 && mT < 25  && LepMiniIso < %f",looseMiniIsoCut);
  if(!allPt){
      cutstring += " && LepPt > 25 && LepPt < 35";
      loosecut  += " && LepPt > 25 && LepPt < 35";
  }
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

std::pair<float,float> getFakeRateMET(TTree* t,float metCut,bool allPt){
  string cutstring = Form("MET < %f && mT < %f && LepIsTight == 1 && LepMiniIso < 0.1", metCut, metCut);
  string loosecut = Form("MET < %f && mT < %f",  metCut,metCut); //  && LepMiniIso < 0.4", metCut,metCut);
  if(!allPt){
      cutstring += " && LepPt > 25 && LepPt < 35";
      loosecut  += " && LepPt > 25 && LepPt < 35";
  }
  TH1D* num = new TH1D("num","",1,0,10000);
  TH1D* den = new TH1D("den","",1,0,10000);
  std::cout<<"num : "<<cutstring<<std::endl;
  std::cout<<"den : "<<loosecut<<std::endl;
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

void fr_2018data(TString trig_, TString postfix_=""){

  TString elID = "MVA2017TightV2IsoTightRC";
  TString muID = "CBTightMiniIsoTight";
  TString trig = trig_;
  TString trig_lower = ToLower(trig+postfix_);
//   std::string trig = "nonIsoHTTrig";
//   std::string trig = "isoTrig_forTrilep";

  bool allPt = false;
  if(trig_lower.Contains("allpt")) {allPt = true; std::cout<<"Using allPt option"<<std::endl;}

  // Fail safe
  if(trig=="")trig = "nonIsoTrig";  //default
  //if(trig != "nonIsoHTTrig" && trig != "isoTrig_forTrilep"){
  if(!(trig.Contains("nonIsoTrig")) && !(trig.Contains("isoTrig_forTrilep"))){
      std::cout<<"Trigger selecton not recognized."<<std::endl;
      std::cout<<"Choose between nonIsoHTTrig and isoTrig_forTrilep -- June 29, 2019."<<std::endl;      
      gApplication->Terminate();
      
  }

  std::cout << "Trigger: " << trig << std::endl;

  std::vector<TString> eras; 
  eras.push_back("2018All");

  // Input directory
  TString indir = "FakeRate_"+trig+postfix_+"/";//+"_OneTrig_HEMveto/";
  TString workDir = "../../test/"+indir;
  
  // Make output directoty
  TString outDir = "plots2018_082020_"+indir+"/"; //all era's
  system("mkdir -vp "+outDir);
  bool doEl = true;
  bool doMu = true;

  for(unsigned int i=0; i < eras.size(); i++){
    TString era = eras.at(i);
    std::cout<<"Reading file from: "<< workDir<<std::endl;
    std::cout<<"working on era: "<<era<<std::endl;
    //make output file to save graph
    TFile* fout = new TFile(outDir+"FakeRate_Graph_"+era+"_"+elID+".root","RECREATE");
    // Electron
    if(doEl){
      TFile* fEle = new TFile(workDir+"FakeRate_Data_"+era+"Electrons_"+elID+"_2018dataset_"+trig+".root");
      TTree* tEle = (TTree*) fEle->Get("FakeRate");
      TGraphErrors* gEle = new TGraphErrors(40);
      gEle->SetName("ElectronFakeRate");
      for(int i =1; i<41; i++){
        float cut = 0.01*i;
        std::pair<float,float> rateAndErr = getFakeRate(tEle,0.4,cut,allPt);
        float fakerate = rateAndErr.first;
        float err = rateAndErr.second;
        gEle->SetPoint(i,cut,fakerate);
        gEle->SetPointError(i,0.,err);
        std::cout<<"Electron Fake Rate for miniIso cut < "<<cut<<" is: "<<fakerate<< " +/- " << err<<std::endl;
      }
      gEle->SetMarkerStyle(22);
      gEle->SetMarkerColor(kBlue);
      gEle->SetMaximum(0.68);
      gEle->SetTitle("Electron Fake Rate vs MiniIsolation Requirement;I_{mini} Requirement;Fake Rate");
      TCanvas* c = new TCanvas();
      gEle->Draw("apl");
      c->Print(outDir+"ElectronFakeRate-vs-MiniIso_"+era+"_"+elID+"_"+trig+".pdf");

      if(trig_lower.Contains("nomet")){
        TGraphErrors* gEle_MET = new TGraphErrors(40);
        gEle_MET->SetName("ElectronFakeRate_MET");
        for(int i =1; i<41; i++){
          float cut = -5.0+10.0*i;
          std::pair<float,float> rateAndErr = getFakeRateMET(tEle,cut,allPt);
          float fakerate = rateAndErr.first;
          float err = rateAndErr.second;
          gEle_MET->SetPoint(i,cut,fakerate);
          gEle_MET->SetPointError(i,0.,err);
          std::cout<<"Electron Fake Rate for MET cut < "<<cut<<" is: "<<fakerate<< " +/- " << err<<std::endl;
        }
        gEle_MET->SetMarkerStyle(22);
        gEle_MET->SetMarkerColor(kBlue);
        gEle_MET->SetMaximum(0.68);
        gEle_MET->SetTitle("Electron Fake Rate vs MET Requirement;MET Requirement;Fake Rate");
        TCanvas* c = new TCanvas();
        gEle_MET->Draw("apl");
        c->Print(outDir+"ElectronFakeRate-vs-MET_"+era+"_"+elID+"_"+trig+".pdf");
      }

      //getEtagraphs for 0.1 cut
      std::cout<< "\nEl:"<<std::endl;
      TGraphAsymmErrors* gEtaEl = getEtaGraph(tEle,0.4,0.1,false,allPt);
      TCanvas* cEtaEl = new TCanvas();
      gEtaEl->SetMaximum(0.6);
      gEtaEl->SetMinimum(0.0);
      gEtaEl->SetTitle("Electron Fake Rate vs #eta; #eta ;Fake Rate");
      gEtaEl->Draw("ap");
      cEtaEl->Print(outDir+"ElectronFakeRate-vs-Eta_"+era+"_"+elID+"_"+trig+".pdf");
 
      std::cout<< "\nEl:"<<std::endl;
      TGraphAsymmErrors* gPtEl = getPtGraph(tEle,0.4,0.1,false);
      TCanvas* cPtEl = new TCanvas();
      gPtEl->Draw("ap");
      cPtEl->Print(outDir+"ElectronFakeRate-vs-Pt_"+era+"_"+elID+"_"+trig+".pdf");

      std::cout<< "\nEl:"<<std::endl;
      TGraphAsymmErrors* gnConstEl = getnConstGraph(tEle,0.4,0.1,true);
      TCanvas* cnConstEl = new TCanvas();
      gnConstEl->SetTitle("ElectronFakeRate-vs-nConst");
      gnConstEl->Draw("ap");
      cnConstEl->Print(outDir+"ElectronFakeRate-vs-nConst_"+era+"_"+muID+"_"+trig+".pdf");

      std::cout<< "\nEl:"<<std::endl;
      TGraphAsymmErrors* gHTEl = getHTGraph(tEle,0.4,0.1,true);
      TCanvas* cHTEl = new TCanvas();
      gHTEl->SetTitle("ElectronFakeRate-vs-HT");
      gHTEl->Draw("ap");
      cHTEl->Print(outDir+"ElectronFakeRate-vs-HT_"+era+"_"+muID+"_"+trig+".pdf");

      //save graphs
      fout->WriteTObject(gEle);
      fout->WriteTObject(gEtaEl,"ElectronEtaFakeRate");
      fout->WriteTObject(gPtEl,"ElectronPtFakeRate");
      fout->WriteTObject(gnConstEl,"ElectronnConstFakeRate");
      fout->WriteTObject(gHTEl,"ElectronHTFakeRate");
    }

    // Muon
    if(doMu){
      TFile* fMu = new TFile(workDir+"FakeRate_Data_"+era+"Muons_"+muID+"_2018dataset_"+trig+".root");
      TTree* tMu = (TTree*) fMu->Get("FakeRate");
      TGraphErrors* gMu = new TGraphErrors(41);
      gMu->SetName("MuonFakeRate");
      for(int i =1; i<41; i++){
        float cut = 0.01*i;
        std::pair<float,float> rateAndErr = getFakeRate(tMu,0.4,cut,allPt);
        float fakerate = rateAndErr.first;
        float err = rateAndErr.second;
        gMu->SetPoint(i,cut,fakerate);
        gMu->SetPointError(i,0.,err);
        std::cout<<"Muon Fake Rate for miniIso cut < "<<cut<<" is: "<<fakerate<<" +/- " << err<<std::endl;
      }
      gMu->SetMarkerStyle(22);
      gMu->SetMarkerColor(kBlue);
      gMu->SetMaximum(1.025);
      gMu->SetTitle("Muon Fake Rate vs MiniIsolation Requirement;I_{mini} Requirement;Fake Rate");
      TCanvas* c1 = new TCanvas();
      gMu->Draw("apl");
      c1->Print(outDir+"MuonFakeRate-vs-MiniIso_"+era+"_"+trig+".pdf");

      if(trig_lower.Contains("nomet")){
        TGraphErrors* gMu_MET = new TGraphErrors(40);
        gMu_MET->SetName("MuonFakeRate_MET");
        for(int i =1; i<41; i++){
          float cut = -5.0+10.0*i;
          std::pair<float,float> rateAndErr = getFakeRateMET(tMu,cut,allPt);
          float fakerate = rateAndErr.first;
          float err = rateAndErr.second;
          gMu_MET->SetPoint(i,cut,fakerate);
          gMu_MET->SetPointError(i,0.,err);
          std::cout<<"Muon Fake Rate for MET cut < "<<cut<<" is: "<<fakerate<< " +/- " << err<<std::endl;
        }
        gMu_MET->SetMarkerStyle(22);
        gMu_MET->SetMarkerColor(kBlue);
        gMu_MET->SetMaximum(0.68);
        gMu_MET->SetTitle("Mu Fake Rate vs MET Requirement;MET Requirement;Fake Rate");
        TCanvas* c = new TCanvas();
        gMu_MET->Draw("apl");
        c->Print(outDir+"MuonFakeRate-vs-MET_"+era+"_"+elID+"_"+trig+".pdf");
      }

      std::cout<< "\nMu:"<<std::endl;
      TGraphAsymmErrors* gEtaMu = getEtaGraph(tMu,0.4,0.1,true,allPt);
      TCanvas* cEtaMu = new TCanvas();
      gEtaMu->SetMaximum(1.0);
      gEtaMu->SetMinimum(0.0);
      gEtaMu->SetTitle("Muon Fake Rate vs #eta; #eta ;Fake Rate");
      gEtaMu->Draw("ap");
      cEtaMu->Print(outDir+"MuonFakeRate-vs-Eta_"+era+"_"+muID+"_"+trig+".pdf");
  
      std::cout<< "\nMu:"<<std::endl;
      TGraphAsymmErrors* gPtMu = getPtGraph(tMu,0.4,0.1,true);
      TCanvas* cPtMu = new TCanvas();
      gPtMu->Draw("ap");
      cPtMu->Print(outDir+"MuonFakeRate-vs-Pt_"+era+"_"+muID+"_"+trig+".pdf");

      std::cout<< "\nMu:"<<std::endl;
      TGraphAsymmErrors* gnConstMu = getnConstGraph(tMu,0.4,0.1,true);
      TCanvas* cnConstMu = new TCanvas();
      gnConstMu->SetTitle("MuonFakeRate-vs-nConst");
      gnConstMu->Draw("ap");
      cnConstMu->Print(outDir+"MuonFakeRate-vs-nConst_"+era+"_"+muID+"_"+trig+".pdf");
  
      std::cout<< "\nMu:"<<std::endl;
      TGraphAsymmErrors* gHTMu = getHTGraph(tMu,0.4,0.1,true);
      TCanvas* cHTMu = new TCanvas();
      gHTMu->SetTitle("MuonFakeRate-vs-HT");
      gHTMu->Draw("ap");
      cHTMu->Print(outDir+"MuonFakeRate-vs-HT_"+era+"_"+muID+"_"+trig+".pdf");

      //save graphs
      fout->WriteTObject(gMu);
      fout->WriteTObject(gEtaMu,"MuonEtaFakeRate");
      fout->WriteTObject(gPtMu,"MuonPtFakeRate"); 
      fout->WriteTObject(gnConstMu,"MuonnConstFakeRate");
      fout->WriteTObject(gHTMu,"MuonHTFakeRate");
    }
    fout->Close();

  }
  
  gApplication->Terminate();

}
