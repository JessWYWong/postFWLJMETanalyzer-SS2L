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
#include <math.h>
#include <map>
#include <string>
#include <sstream> 
#include <iomanip>
#include <iostream>
//#include "../plugins/Macros.cc"
#include "../plugins/SetTDRStyle.cc"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "makeTables.cc"

void DrawAndSave(Variable* Var, std::vector<Sample*> vSamples, TFile* outfile, std::string elID, std::string muID, int nMu=-1, int cutIndex=0);
TH1F* getNPErrorHisto(std::vector<Sample*> vS, Variable* var, int nMu, std::string elID, std::string muID, int cutIndex);
std::stringstream& printRegEvtsAndError(std::stringstream& tablestring,std::vector<CutClass*> vCC, std::vector<std::string> vCS);
std::stringstream& printClosureEvtsAndError(std::stringstream& tablestring,std::vector<Sample*> vS, std::string FRflavor, int nmu);

int main(int argc, char* argv[]){

  if(argc!=3) {
    std::cout<<"Need to specify electron and moun IDs! Exiting..."<<std::endl;
    return 0;
  }

  TFile* fout = new TFile("ClosureTest_Plots.root","RECREATE");
  setTDRStyle();

  std::string elID = argv[1];
  std::string muID = argv[2];
  float lumi = 2.0; //estimate for end of year

  std::vector<Sample*> vSamples = getSamplesForClosureTest("sZVeto",lumi,elID, muID);

  std::vector<Variable*> vVariables = getVariablesForClosureTest();

  for(unsigned int i=0; i< vVariables.size(); i++){
    for(unsigned int j =0; j<5; j++){
      DrawAndSave(vVariables.at(i),vSamples,fout,elID,muID,-1,j);
      DrawAndSave(vVariables.at(i),vSamples,fout,elID,muID,0,j);
      DrawAndSave(vVariables.at(i),vSamples,fout,elID,muID,1,j);
      DrawAndSave(vVariables.at(i),vSamples,fout,elID,muID,2,j);
      
    }
  }

  //check closure in terms of number of events
  std::ofstream outfile;
  outfile.open("table_closureTest_ttbar.txt");

  //now get vector of cuts
  std::vector<std::string> vCutString = getCutString();

  for(size_t i =0; i<vCutString.size();i++){
    std::cout<<vCutString.at(i)<<std::endl;
  }
  std::stringstream tables;
  tables<<std::fixed<<std::setprecision(2);
  //bool Bkg=false;
  std::vector<std::string> flavors; 
  flavors.push_back("data"); flavors.push_back("light"); flavors.push_back("charm");flavors.push_back("bottom");flavors.push_back("fakes");flavors.push_back("average");
  flavors.push_back("light_qcd"); flavors.push_back("charm_qcd");flavors.push_back("bottom_qcd");flavors.push_back("fakes_qcd");flavors.push_back("average_qcd");
  flavors.push_back("light_ttb"); flavors.push_back("charm_ttb");flavors.push_back("bottom_ttb");flavors.push_back("fakes_ttb");flavors.push_back("unmatched_ttb");flavors.push_back("average_ttb");

  for(int nmu=-1; nmu<3; nmu++){
    //channel header
    std::string chan;
    if(nmu==-1) chan = "\\multicolumn{3}{|c|}{All Channels Combined}\\\\\n";
    if(nmu==0) chan = "\\multicolumn{3}{|c|}{Di-electron Channel}\\\\\n";
    if(nmu==1) chan = "\\multicolumn{3}{|c|}{Electron-muon Channel}\\\\\n";
    if(nmu==2) chan = "\\multicolumn{3}{|c|}{Di-muon Channel}\\\\\n";
 
    std::string header = "Sample & FakeRate Source & Events\\\\\n\hline\n";
    tables<<chan<<header;
    //now make a vector of cutClass for bkg
    std::vector<CutClass*> vCutBkg = getCutClassVector(vSamples,vCutString,nmu);
    //print line for regular method
    printRegEvtsAndError(tables,vCutBkg,vCutString);
    //print lines for nonprompt with different fake rates
    for(unsigned int f=0; f<flavors.size();f++){
      std::string flav=flavors.at(f);
      printClosureEvtsAndError(tables,vSamples,flav,nmu);
    }
    tables<<"\n\\hline\n";

  }
  
  

  outfile<<tables.str();

  outfile.close();


  fout->Write();
  return 0;

}

void DrawAndSave(Variable* var, std::vector<Sample*> vSamples, TFile* outfile, std::string elID, std::string muID, int nMu, int cutIndex){


  //rename plots by channel
  std::string channel;
  if(nMu==-1) channel="All";
  if(nMu==0) channel="ElEl";
  if(nMu==1) channel="ElMu";
  if(nMu==2) channel="MuMu";


  TCanvas* c1 = new TCanvas("c1","c1");

  c1->SetLogy();

  std::stringstream cutstring;
  if(nMu>=0){
    if(cutIndex==0){cutstring<<"( ChargeMisIDWeight*(Channel=="<<nMu<<") )";}
    else if(cutIndex==1){cutstring<<"( ChargeMisIDWeight *(Channel=="<<nMu<<" && DilepMass >20 &&  ((Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) )) )";}
    else if(cutIndex==2){cutstring<<"( ChargeMisIDWeight *(Channel=="<<nMu<<" && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) )) )";}
    else if(cutIndex==3){cutstring<<"( ChargeMisIDWeight *(Channel=="<<nMu<<" && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nConst >=5 ) )";}
    else if(cutIndex==4){cutstring<<"( ChargeMisIDWeight *(Channel=="<<nMu<<" && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nConst >=5  && Lep1Pt > 100) )";}
  }
  else {
    if(cutIndex==0){cutstring<<"( ChargeMisIDWeight* (Channel>=0 && ((Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) )) )";}
    else if(cutIndex==1){cutstring<<"( ChargeMisIDWeight *(Channel>=0 && DilepMass >20 && ((Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) )) )";}
    else if(cutIndex==2){cutstring<<"( ChargeMisIDWeight *(Channel>=0 && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) )) )";}
    else if(cutIndex==3){cutstring<<"( ChargeMisIDWeight *(Channel>=0 && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nConst >=5 ) )";}
    else if(cutIndex==4){cutstring<<"( ChargeMisIDWeight *(Channel>=0 && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nConst >=5 && Lep1Pt > 100 ) )";}
  }

  TLegend* leg = new TLegend(0.65,0.6,0.9,0.9);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  //histogram for weights
  TH1F* h_err = getNPErrorHisto(vSamples,var,nMu,elID,muID, cutIndex);
  for(unsigned int i=2; i< vSamples.size();i++){
    Sample* s = vSamples.at(i);
    
    TH1F* h = new TH1F("h",(var->name).c_str(), var->nbins, var->xmin, var->xmax);    
    TTree* t = s->tree;
    
    t->Project("h",(var->name).c_str(),(cutstring.str()).c_str());
    //scale by weight
    h->Scale(s->weight);
    //aesthetics
    h->SetTitle("");
    h->SetFillStyle(0);
    h->SetLineColor(s->color);
    h->SetLineWidth(2);
    h->SetLineStyle(s->linestyle);
    h->GetXaxis()->SetTitle(var->Xaxis.c_str());
    h->GetYaxis()->SetTitle(var->Yaxis.c_str());
    std::cout<<"Sample: "<<s->name<<" and bin 3 content: "<<h->GetBinContent(3)<<" h_Err bin 3 content: "<<h_err->GetBinContent(3)<<std::endl;
    //add to legend
    if(i==2) leg->AddEntry(h,"Observed TTbar","l");
    else leg->AddEntry(h,"Predicted TTbar","l");
    if(i==2) h->Draw("hist e");
    else { h->Draw("hist same");}
    
  }
    
    
  c1->RedrawAxis();
  c1->Update();
  //draw errors
  h_err->SetFillStyle(3344);
  h_err->SetFillColor(1);
  //h_err->Draw("SAME E2");
  
  //latex
  TLatex* cmstex = new TLatex();
  cmstex->SetNDC();
  cmstex->SetTextSize(0.04);
  TLatex* lumitex = new TLatex();
  lumitex->SetNDC();
  lumitex->SetTextSize(0.04);
  
  //draw legend, cms and lumi
  leg->Draw("same");
  
  //draw latex
  cmstex->DrawLatex(0.15,0.96,"CMS Simulation Preliminary");
  lumitex->DrawLatex(0.65,0.96,"2.0 fb^{-1} (13 TeV)");

  //draw latex for channels
  TLatex* chantex = new TLatex();
  chantex->SetNDC();
  chantex->SetTextSize(0.04);
  if(nMu==-1) chantex->DrawLatex(0.25,0.8,"All Channels");
  if(nMu==0) chantex->DrawLatex(0.25,0.8,"#bf{ee}");
  if(nMu==1) chantex->DrawLatex(0.25,0.8,"#bf{e#mu}");
  if(nMu==2) chantex->DrawLatex(0.25,0.8,"#bf{#mu#mu}");

  std::string cutname;
  if(cutIndex==0) cutname =  "sZVeto";
  if(cutIndex==1) cutname =  "QuarkoniaAndElElZVeto";
  if(cutIndex==2) cutname =  "TwoJets";
  if(cutIndex==3) cutname =  "nConstituents";
  if(cutIndex==4) cutname =  "LeadingLepPt100";
  std::string pdfname = "./plots/ClosureTest_"+(var->name)+"_"+channel+"_"+cutname+"_Mu"+muID+"_El"+elID+".pdf";
  std::string pngname = "./plots/ClosureTest_"+(var->name)+"_"+channel+"_"+cutname+"_Mu"+muID+"_El"+elID+".png";
  
  c1->Print(pdfname.c_str());
  c1->Print(pngname.c_str());
  outfile->WriteTObject(c1);
  delete c1;
  delete cmstex;
  delete lumitex;
  delete h_err;
}

TH1F* getNPErrorHisto(std::vector<Sample*> vS, Variable* var, int nMu,std::string elID, std::string muID, int cutIndex){

  TH1F* h_err = new TH1F("h_err",(var->name).c_str(), var->nbins, var->xmin, var->xmax);

  
  //now go through pain of getting non-prompt background error correct;
  TH1F* h_t00 = new TH1F("h_t00",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
  TH1F* h_t01 = new TH1F("h_t01",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
  TH1F* h_t10 = new TH1F("h_t10",(var->name).c_str(), var->nbins, var->xmin, var->xmax);
  TH1F* h_t11 = new TH1F("h_t11",(var->name).c_str(), var->nbins, var->xmin, var->xmax);

  //get version without npweight
  std::stringstream NTcutstring;
  if(nMu>=0){
    if(cutIndex==0){NTcutstring<<"( trigSF * IDSF * IsoSF * ChargeMisIDWeight * MCWeight * prefireWeigh * (Channel=="<<nMu<<") )";}
    else if(cutIndex==1){NTcutstring<<"( trigSF * IDSF * IsoSF * ChargeMisIDWeight * MCWeight * prefireWeigh * (Channel=="<<nMu<<" && DilepMass >20 &&  ((Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) )) )";}
    else if(cutIndex==2){NTcutstring<<"( trigSF * IDSF * IsoSF * ChargeMisIDWeight * MCWeight * prefireWeigh * (Channel=="<<nMu<<" && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) )) )";}
    else if(cutIndex==3){NTcutstring<<"( trigSF * IDSF * IsoSF * ChargeMisIDWeight * MCWeight * prefireWeigh * (Channel=="<<nMu<<" && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nConst >=5) )";}
    else if(cutIndex==4){NTcutstring<<"( trigSF * IDSF * IsoSF * ChargeMisIDWeight * MCWeight * (Channel=="<<nMu<<" && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nConst >=5 && Lep1Pt > 100) )";}
  }
  else {
    if(cutIndex==0){NTcutstring<<"( trigSF * IDSF * IsoSF * ChargeMisIDWeight * MCWeight * prefireWeigh * (Channel>=0 && ((Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) )) )";}
    else if(cutIndex==1){NTcutstring<<"( trigSF * IDSF * IsoSF * ChargeMisIDWeight * MCWeight * prefireWeigh * (Channel>=0 && DilepMass >20 && ((Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) )) )";}
    else if(cutIndex==2){NTcutstring<<"( trigSF * IDSF * IsoSF * ChargeMisIDWeight * MCWeight * prefireWeigh * (Channel>=0 && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) )) )";}
    else if(cutIndex==3){NTcutstring<<"( trigSF * IDSF * IsoSF * ChargeMisIDWeight * MCWeight * prefireWeigh * (Channel>=0 && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nConst >=5) )";}
    else if(cutIndex==4){NTcutstring<<"( trigSF * IDSF * IsoSF * ChargeMisIDWeight * MCWeight * prefireWeigh * (Channel>=0 && DilepMass >20 && nCleanAK4Jets > 1 && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && nConst >=5 && Lep1Pt >100) )";}
  }
  //now add ntl requirement
  std::string nt00cut= "("+NTcutstring.str()+" && nTL==0)";;
  std::string nt01cut= "("+NTcutstring.str()+" && nTL==1)";;
  std::string nt10cut= "("+NTcutstring.str()+" && nTL==2)";;
  std::string nt11cut= "("+NTcutstring.str()+" && nTL==3)";;

  //now fill the needed histograms
  for(std::vector<Sample*>::size_type nt=0;nt<vS.size();nt++){
    //load sample
    Sample* s= vS.at(nt);
    //skip if not non prompt
    if( (s->name).find("NonPromptTTJets")==std::string::npos) continue;
    std::cout<<"adding errors for sample: "<<s->name<<std::endl;
    //get tree
    TTree* tdummy = s->tree;
    //fill histograms
    tdummy->Project("h_t00",(var->name).c_str(),nt00cut.c_str());
    tdummy->Project("h_t01",(var->name).c_str(),nt01cut.c_str());
    tdummy->Project("h_t10",(var->name).c_str(),nt10cut.c_str());
    tdummy->Project("h_t11",(var->name).c_str(),nt11cut.c_str());
  

    //now scale histograms and then add them to the total error but only do non-empty ones
    if(h_t00->Integral()!=0){ 
      if(nMu==0 || nMu==2) h_t00->Scale(WeightSF_T0(elID,muID,nMu));
      else h_t00->Scale(WeightOF_T00(elID,muID));
      h_err->Add(h_t00);
    }
    if(h_t01->Integral()!=0){ 
      if(nMu==0 || nMu==2) h_t01->Scale(WeightSF_T1(elID,muID,nMu));
      else h_t01->Scale(WeightOF_T01(elID,muID));
      h_err->Add(h_t01);
    }
    if(h_t10->Integral()!=0){ 
      if(nMu==0 || nMu==2) h_t10->Scale(WeightSF_T1(elID,muID,nMu));
      else h_t10->Scale(WeightOF_T10(elID,muID));
      h_err->Add(h_t10);
    }
    if(h_t11->Integral()!=0){ 
      if(nMu==0 || nMu==2) h_t11->Scale(WeightSF_T2(elID,muID,nMu));
      else h_t11->Scale(WeightOF_T11(elID,muID));
      h_err->Add(h_t11);
    }
    h_err->Scale(s->weight);
  }//end loop over samples

  return h_err;

}


std::stringstream& printRegEvtsAndError(std::stringstream& tablestring,std::vector<CutClass*> vCC, std::vector<std::string> vCS){

  int index = vCC.at(2)->nEvents.size();
  CutClass* cc = vCC.at(2);
  tablestring<<"Observed Events (MC) & N/A &"<< cc->nEvents.at(index-1)<<"$\\pm$"<<cc->vErr.at(index-1)<<"\\\\"<<std::endl;


}

std::stringstream& printClosureEvtsAndError(std::stringstream& tablestring,std::vector<Sample*> vS, std::string FRflavor, int nmu){

  for(unsigned int i=0; i < vS.size(); i++){
    Sample* s = vS.at(i);    
    if( (s->name).find("NonPromptTTbar")==std::string::npos) continue;
    TH1F* h; 
    if( nmu==0) {h = s->SetNPCombinedHisto(s->hists_elel,weights_elel(FRflavor));h->Sumw2();}
    if( nmu==1) {h = s->SetNPCombinedHisto(s->hists_elmu,weights_elmu(FRflavor));h->Sumw2();}
    if( nmu==2) {h = s->SetNPCombinedHisto(s->hists_mumu,weights_mumu(FRflavor));h->Sumw2();}
    if(nmu==-1){
      h = s->SetNPCombinedHisto(s->hists_elel,weights_elel(FRflavor));
      h->Sumw2();
      h->Add(s->SetNPCombinedHisto(s->hists_elmu,weights_elmu(FRflavor)));
      h->Add(s->SetNPCombinedHisto(s->hists_mumu,weights_mumu(FRflavor)));
    }

    //scale by weight
    h->Scale(s->weight);

    double_t err;
    h->IntegralAndError(101,1001,err);
    //std::cout<<"itegrating from: "<<h->GetBinCenter(100)<<" to "<<h->GetBinCenter(1000)<<std::endl;
    float nEvts = h->Integral(101,1001);
    tablestring<<"Predicted Events & "<<FRflavor<<" & "<<nEvts<<"$\\pm$"<<err<<"\\\\"<<std::endl;

  }


}
