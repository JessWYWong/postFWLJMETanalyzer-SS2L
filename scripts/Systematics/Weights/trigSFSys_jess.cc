#include <iostream>
#include <iomanip> 
#include <vector>
#include <string>
#include "TString.h"
#include "TRegexp.h"
#include "TLegend.h"
#include "TLatex.h"
#include "../../../plugins/SetTDRStyle.cc"
int TTmass[] = {1000,1100,1200,1300,1400,1500,1600,1700,1800};
int BBmass[] = {900,1000,1100,1200,1300,1400,1500,1600,1700,1800};

int nTT = sizeof(TTmass)/sizeof(TTmass[0]);
int nBB = sizeof(BBmass)/sizeof(BBmass[0]);

TH2D* hTTuncUp = new TH2D("uncUp_TT","uncUp_TT",(TTmass[nTT-1]-TTmass[0])+100,TTmass[0]-50,TTmass[nTT-1]+50,200,0.0,2.0);
TH2D* hTTuncDn = new TH2D("uncDn_TT","uncDn_TT",(TTmass[nTT-1]-TTmass[0])+100,TTmass[0]-50,TTmass[nTT-1]+50,200,0.0,2.0);
TH2D* hTTRMSUp = new TH2D("RMSUp_TT","RMSUp_TT",(TTmass[nTT-1]-TTmass[0])+100,TTmass[0]-50,TTmass[nTT-1]+50,200,0.0,2.0);
TH2D* hTTRMSDn = new TH2D("RMSDn_TT","RMSDn_TT",(TTmass[nTT-1]-TTmass[0])+100,TTmass[0]-50,TTmass[nTT-1]+50,200,0.0,2.0);

TH2D* hBBuncUp = new TH2D("uncUp_BB","uncUp_BB",(nBB+1)*(BBmass[1]-BBmass[0]),BBmass[0]-((BBmass[1]-BBmass[0])/2),BBmass[nBB-1]+((BBmass[1]-BBmass[0])/2),200,0.0,2.0);
TH2D* hBBuncDn = new TH2D("uncDn_BB","uncDn_BB",(nBB+1)*(BBmass[1]-BBmass[0]),BBmass[0]-((BBmass[1]-BBmass[0])/2),BBmass[nBB-1]+((BBmass[1]-BBmass[0])/2),200,0.0,2.0);
TH2D* hBBRMSUp = new TH2D("RMSUp_BB","RMSUp_BB",(nBB+1)*(BBmass[1]-BBmass[0]),BBmass[0]-((BBmass[1]-BBmass[0])/2),BBmass[nBB-1]+((BBmass[1]-BBmass[0])/2),200,0.0,2.0);
TH2D* hBBRMSDn = new TH2D("RMSDn_BB","RMSDn_BB",(nBB+1)*(BBmass[1]-BBmass[0]),BBmass[0]-((BBmass[1]-BBmass[0])/2),BBmass[nBB-1]+((BBmass[1]-BBmass[0])/2),200,0.0,2.0);

float getEvents(TTree* t,TString cut){
  TH1F* h = new TH1F("h","h",1,0,1000000);
  t->Project("h","Lep1Pt",cut);  
  float nEvents = h->Integral();
  delete h;

  return nEvents;
}


void printLineMCBkg(std::string sample, TString filename,std::stringstream& filecontent){

/*
  TString baseWeight = "ChargeMisIDWeight * NPWeight * IDSF * IsoSF * GsfSF * PUWeight * prefireWeight * ";
  //TString baseWeight = "IDSF * IsoSF * trigSF * GsfSF * prefireWeight * ";
  //TString baseWeight = "";

  TString nomcut =    baseWeight+"trigSF * MCWeight * (Lep1Pt>40 && nConst >=4 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";
  TString Upcut =   baseWeight+"trigSFup * MCWeight * (Lep1Pt>40 && nConst >=4 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT> 1200)";
  TString Downcut = baseWeight+"trigSFdn * MCWeight * (Lep1Pt>40 && nConst >=4 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";
*/
  TString nomcut = "ChargeMisIDWeight * NPWeight * IsoSF * trigSF * GsfSF * prefireWeight * PUWeight * IDSF * MCWeight * (Lep1Pt>40 && Lep2Pt>30 && nConst >=4 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";
  TRegexp re("trigSF");
  TString Upcut = nomcut; Upcut(re) = "trigSFup";
  TString Downcut = nomcut; Downcut(re) = "trigSFdn";


  TFile* f = new TFile(filename);
  TTree* t = (TTree*) f->Get("tEvts_sZVeto");
  float nom = getEvents(t,nomcut);
  float up = getEvents(t,Upcut);
  float upPct = 1*(up-nom)/nom;
  float down = getEvents(t,Downcut);
  float downPct = 1*(down-nom)/nom;
  std::cout<<setprecision(5);
    
  //swap up-dn if necessary
//   float temp;
//   if(upPct < downPct){
//   	temp = upPct;
//   	upPct = downPct;
//   	downPct = temp;
//   }
  std::cout<<sample<<" |  "<<1+upPct<<" |  "<<1+downPct << " ----> "<< fabs(upPct) << " | "<< fabs(downPct) <<std::endl;

  //check in NaN = 0 
  if(upPct!=upPct)upPct=0.;
  if(downPct!=downPct)downPct=0.;

  filecontent<<"uncDict['trigSFUp']['"<< sample << "']="<<1+upPct<< "\n";
  filecontent<<"uncDict['trigSFDn']['"<< sample << "']="<<1+downPct<< "\n";

  delete t; delete f;

}

std::map<std::string,std::vector<float>> uncSig = {};
void initSigMap(){
  for(int m : TTmass){
    uncSig.insert(std::pair<std::string,std::vector<float>>("TTM"+std::to_string(m),{0.,0.,0.,0.,99.,99.}));//SUMSQUp_SUMSQDn_maxUp_maxDn_minUp_minDn 
  }
  for(int m : BBmass){
    uncSig.insert(std::pair<std::string,std::vector<float>>("BBM"+std::to_string(m),{0.,0.,0.,0.,99.,99.}));
  }
}
/*
std::map<std::string,std::vector<float>> RMS_maxUp_maxDn ={
  {"TTM1000",{0.,0.,0.}}, {"TTM1100",{0.,0.,0.}}, {"TTM1200",{0.,0.,0.}}, {"TTM1300",{0.,0.,0.}}, {"TTM1400",{0.,0.,0.}},
  {"TTM1500",{0.,0.,0.}}, {"TTM1600",{0.,0.,0.}}, {"TTM1700",{0.,0.,0.}}, {"TTM1800",{0.,0.,0.}}, {"BBM900",{0.,0.,0.}},
  {"BBM1000",{0.,0.,0.}}, {"BBM1100",{0.,0.,0.}}, {"BBM1200",{0.,0.,0.}}, {"BBM1300",{0.,0.,0.}}, {"BBM1400",{0.,0.,0.}},
  {"BBM1500",{0.,0.,0.}}, {"BBM1600",{0.,0.,0.}}, {"BBM1700",{0.,0.,0.}}, {"BBM1800",{0.,0.,0.}}
};
*/
std::map<std::string, std::vector<float>>::iterator it;
void printLine(std::string sample, TString filename){


  TFile* f = new TFile(filename);
  TH1D* hnom = (TH1D*)f->Get("hist_punom");
  float nom = hnom->Integral();

  TH1D* hup = (TH1D*)f->Get("hist_trigup");
  float up = hup->Integral();
  float upPct = 100*(up-nom)/nom;

  TH1D* hdown = (TH1D*)f->Get("hist_trigdown");
  float down = hdown->Integral();
  float downPct = 100*(down-nom)/nom;
  std::cout<<setprecision(5);
  std::cout<<sample<<" |  "<<upPct<<" |  "<<downPct<<std::endl;
  
  float sysUp = 1+upPct/100.;
  float sysDn = 1+downPct/100.;

  uncSig.at(sample).at(0) += pow(sysUp,2.0);
  uncSig.at(sample).at(1) += pow(sysDn,2.0);
  if(sysUp > uncSig.at(sample).at(2)) uncSig.at(sample).at(2) = sysUp;
  if(sysDn > uncSig.at(sample).at(3)) uncSig.at(sample).at(3) = sysDn;
  if(sysUp < uncSig.at(sample).at(4)) uncSig.at(sample).at(4) = sysUp;
  if(sysDn < uncSig.at(sample).at(5)) uncSig.at(sample).at(5) = sysDn;

  double m_sig = std::stod(sample.substr(3,sample.length()-3));
  if(sample.find("TTM")!=std::string::npos){hTTuncUp->Fill(m_sig,sysUp); hTTuncDn->Fill(m_sig,sysDn);}
  else if(sample.find("BBM")!=std::string::npos){hBBuncUp->Fill(m_sig,sysUp); hBBuncDn->Fill(m_sig,sysDn);}

  f->Close();
  delete f;

}


void trigSFSys_jess(){

  TString elID = "MVA2017TightV2IsoTightRC";
  TString muID = "CBTightMiniIsoTight";
  TString era = "2017B-F";

  TString folder ="/uscms_data/d3/wywong/VLQ/CMSSW_10_2_10/src/postFWLJMETanalyzer/scripts/Systematics/CombineTTsig";
  TString decay;

  initSigMap();

  int countTT = 0;  
  for(int BRtype=0;BRtype<23;BRtype++){

	  if(BRtype==0) decay = "bW0p5tZ0p25tH0p25";
	  if(BRtype==1) decay = "bW0p0tZ1p0tH0p0";
	  if(BRtype==2) decay = "bW0p0tZ0p8tH0p2";
	  if(BRtype==3) decay = "bW0p0tZ0p6tH0p4";
	  if(BRtype==4) decay = "bW0p0tZ0p4tH0p6";
	  if(BRtype==5) decay = "bW0p0tZ0p2tH0p8";
	  if(BRtype==6) decay = "bW0p0tZ0p0tH0p0";
	  if(BRtype==7) decay = "bW0p2tZ0p8tH0p0";
	  if(BRtype==8) decay = "bW0p2tZ0p6tH0p2";
	  if(BRtype==9) decay = "bW0p2tZ0p4tH0p4";
	  if(BRtype==10) decay = "bW0p2tZ0p2tH0p6";
	  if(BRtype==11) decay = "bW0p2tZ0p0tH0p8";
	  if(BRtype==12) decay = "bW0p4tZ0p6tH0p0";
	  if(BRtype==13) decay = "bW0p4tZ0p4tH0p2";
	  if(BRtype==14) decay = "bW0p4tZ0p2tH0p4";
	  if(BRtype==15) decay = "bW0p4tZ0p0tH0p6";
	  if(BRtype==16) decay = "bW0p6tZ0p4tH0p0";
	  if(BRtype==17) decay = "bW0p6tZ0p2tH0p2";
	  if(BRtype==18) decay = "bW0p6tZ0p0tH0p4";
	  if(BRtype==19) decay = "bW0p8tZ0p2tH0p0";
	  if(BRtype==20) decay = "bW0p8tZ0p0tH0p2";
	  if(BRtype==21) decay = "bW1p0tZ0p0tH0p0";
	  if(BRtype==22) decay = "bW0p0tZ0p5tH0p5";

	  //print top line
	  std::cout<<" Sample | trigSF Up | trigSF Down"<<std::endl;
          for(auto m : TTmass) printLine("TTM"+std::to_string(m),folder+"/TprimeTprime_M-"+std::to_string(m)+"_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          /*
 	  //printLine("800 ",folder+"/TprimeTprime_M-800_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
	  //printLine("900 ",folder+"/TprimeTprime_M-900_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          printLine("TTM1000",folder+"/TprimeTprime_M-1000_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          printLine("TTM1100",folder+"/TprimeTprime_M-1100_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          printLine("TTM1200",folder+"/TprimeTprime_M-1200_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          printLine("TTM1300",folder+"/TprimeTprime_M-1300_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          printLine("TTM1400",folder+"/TprimeTprime_M-1400_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          printLine("TTM1500",folder+"/TprimeTprime_M-1500_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          printLine("TTM1600",folder+"/TprimeTprime_M-1600_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          printLine("TTM1700",folder+"/TprimeTprime_M-1700_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          printLine("TTM1800",folder+"/TprimeTprime_M-1800_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root"); 
          */
          countTT++; 
  }

  int countBB = 0;
  for(int BRtype=0;BRtype<23;BRtype++){

          if(BRtype==0) decay = "tW0p5bZ0p25bH0p25";
          if(BRtype==1) decay = "tW0p0bZ1p0bH0p0";
          if(BRtype==2) decay = "tW0p0bZ0p8bH0p2";
          if(BRtype==3) decay = "tW0p0bZ0p6bH0p4";
          if(BRtype==4) decay = "tW0p0bZ0p4bH0p6";
          if(BRtype==5) decay = "tW0p0bZ0p2bH0p8";
          if(BRtype==6) decay = "tW0p0bZ0p0bH0p0";
          if(BRtype==7) decay = "tW0p2bZ0p8bH0p0";
          if(BRtype==8) decay = "tW0p2bZ0p6bH0p2";
          if(BRtype==9) decay = "tW0p2bZ0p4bH0p4";
          if(BRtype==10) decay = "tW0p2bZ0p2bH0p6";
          if(BRtype==11) decay = "tW0p2bZ0p0bH0p8";
          if(BRtype==12) decay = "tW0p4bZ0p6bH0p0";
          if(BRtype==13) decay = "tW0p4bZ0p4bH0p2";
          if(BRtype==14) decay = "tW0p4bZ0p2bH0p4";
          if(BRtype==15) decay = "tW0p4bZ0p0bH0p6";
          if(BRtype==16) decay = "tW0p6bZ0p4bH0p0";
          if(BRtype==17) decay = "tW0p6bZ0p2bH0p2";
          if(BRtype==18) decay = "tW0p6bZ0p0bH0p4";
          if(BRtype==19) decay = "tW0p8bZ0p2bH0p0";
          if(BRtype==20) decay = "tW0p8bZ0p0bH0p2";
          if(BRtype==21) decay = "tW1p0bZ0p0bH0p0";
          if(BRtype==22) decay = "tW0p0bZ0p5bH0p5";
          //print top line
          std::cout<<" Sample | trigSF Up | trigSF Down"<<std::endl;
          for(auto m : BBmass) printLine("BBM"+std::to_string(m),folder+"/BprimeBprime_M-"+std::to_string(m)+"_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          /*
          printLine("BBM900",folder+"/BprimeBprime_M-900_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          printLine("BBM1000",folder+"/BprimeBprime_M-1000_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          printLine("BBM1100",folder+"/BprimeBprime_M-1100_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          printLine("BBM1200",folder+"/BprimeBprime_M-1200_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          printLine("BBM1300",folder+"/BprimeBprime_M-1300_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          printLine("BBM1400",folder+"/BprimeBprime_M-1400_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          printLine("BBM1500",folder+"/BprimeBprime_M-1500_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          printLine("BBM1600",folder+"/BprimeBprime_M-1600_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          printLine("BBM1700",folder+"/BprimeBprime_M-1700_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          printLine("BBM1800",folder+"/BprimeBprime_M-1800_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          */
          countBB++;
  }

  // store unc of signal samples by flavour and mass
  /*
  std::cout<<"=================================================="<<std::endl;
  std::cout<<"===> over all BRtype : " <<std::endl;
  std::cout<<" Sample |   RMS   |  maxUp  | maxDn  "<<std::endl;
  for( it = RMS_maxUp_maxDn.begin(); it != RMS_maxUp_maxDn.end(); it++ ){
    std::cout<< it->first <<" | "<< pow((it->second)[0]/(23*2),0.5) <<" | "<< (it->second)[1] <<" | "<< (it->second)[2] <<std::endl;
  }
  */
  std::ofstream outfile_sig;
  outfile_sig.open("unc_TrigSys_sig.py");

  std::stringstream filecontent_sig;
  filecontent_sig<<std::fixed<<std::setprecision(5);

  filecontent_sig<< "uncDict = {} \n";
  filecontent_sig<< "uncDict['trigSFUp'] = {} \n";
  filecontent_sig<< "uncDict['trigSFDn'] = {} \n";

  std::cout<<"=================================================="<<std::endl;
  std::cout<<"===> over all BRtype : " <<std::endl;
  std::cout<<" Sample |  RMSUp  |  RMSDn  |  maxUp  |  maxDn  |  minUp  |  minDn  "<<std::endl;

  double yMax_TT = 1.1; //hTTuncUp->GetYaxis()->GetBinUpEdge(hTTuncUp->GetYaxis()->GetNbins());
  double yMin_TT = 0.9; //hTTuncUp->GetYaxis()->GetBinLowEdge(1);
  double yMax_BB = 1.1; //hBBuncUp->GetYaxis()->GetBinUpEdge(hBBuncUp->GetYaxis()->GetNbins());
  double yMin_BB = 0.9; //hBBuncUp->GetYaxis()->GetBinLowEdge(1);
  for( it = uncSig.begin(); it != uncSig.end(); it++ ){
    std::cout<< it->first <<" | "<< pow((it->second)[0]/(countTT),0.5) <<" | "<< pow((it->second)[1]/(countTT),0.5) <<" | "<< (it->second)[2]<<" | "<< (it->second)[3] <<" | "<< (it->second)[4] <<" | "<< (it->second)[5] <<std::endl;

    // use RMS values for all BRType
    filecontent_sig<<"uncDict['trigSFUp']['"<< it->first << "']="<< pow((it->second)[0]/(countTT),0.5) << "\n";
    filecontent_sig<<"uncDict['trigSFDn']['"<< it->first << "']="<< pow((it->second)[1]/(countTT),0.5) << "\n";

    double m_sig = std::stod((it->first).substr(3,(it->first).length()-3));
    if((it->first).find("TTM")!=std::string::npos){
      hTTRMSUp->Fill(m_sig, pow((it->second)[0]/(countTT),0.5));
      hTTRMSDn->Fill(m_sig, pow((it->second)[1]/(countTT),0.5));
      if((it->second)[2]>yMax_TT) yMax_TT=(it->second)[2];
      if((it->second)[3]>yMax_TT) yMax_TT=(it->second)[3];
      if((it->second)[4]<yMin_TT) yMin_TT=(it->second)[4];
      if((it->second)[5]<yMin_TT) yMin_TT=(it->second)[5];
    }
    else if ((it->first).find("BBM")!=std::string::npos){
      hBBRMSUp->Fill(m_sig, pow((it->second)[0]/(countBB),0.5));
      hBBRMSDn->Fill(m_sig, pow((it->second)[1]/(countBB),0.5));
      if((it->second)[2]>yMax_BB) yMax_BB=(it->second)[2];
      if((it->second)[3]>yMax_BB) yMax_BB=(it->second)[3];
      if((it->second)[4]<yMin_BB) yMin_BB=(it->second)[4];
      if((it->second)[5]<yMin_BB) yMin_BB=(it->second)[5];
    }
  }

  outfile_sig<<filecontent_sig.str();
  outfile_sig.close();

  yMax_TT+=0.02;yMin_TT-=0.02;yMax_BB+=0.02;yMin_BB-=0.02;

  setTDRStyle();
  TLatex* cmstex = new TLatex();
  cmstex->SetNDC();
  cmstex->SetTextSize(0.04);

  TCanvas *c1 = new TCanvas("c1","c1");
  TLegend* leg1 = new TLegend(0.5,0.95,0.95,1.0);
  leg1->SetNColumns(4);
  leg1->SetFillStyle(0);
  leg1->SetBorderSize(0);
  hTTuncUp->SetMarkerStyle(20);
  hTTuncUp->SetMarkerSize(0.5);
  hTTuncUp->SetMarkerColorAlpha(kGray+2,0.5);
  hTTuncUp->GetXaxis()->SetTitle("T mass[GeV]");
  hTTuncUp->GetYaxis()->SetTitle("rate unc.");
  hTTuncUp->SetTitle("");
  hTTuncUp->GetYaxis()->SetRangeUser(yMin_TT,yMax_TT);
  hTTuncUp->Draw("scat");
  leg1->AddEntry(hTTuncUp, "trigSF Up", "p");
  hTTRMSUp->SetMarkerStyle(5);
  hTTRMSUp->SetMarkerSize(1.5);
  hTTRMSUp->SetMarkerColor(kBlack);
  hTTRMSUp->Draw("scat same");
  leg1->AddEntry(hTTRMSUp, "trigSF Up (RMS)", "p");
  hTTuncDn->SetMarkerColorAlpha(kRed-7,0.5);
  hTTuncDn->SetMarkerStyle(20);
  hTTuncDn->SetMarkerSize(0.5);
  hTTuncDn->Draw("scat same");
  leg1->AddEntry(hTTuncDn, "trigSF Down", "p");
  hTTRMSDn->SetMarkerStyle(5);
  hTTRMSDn->SetMarkerSize(1.5);
  hTTRMSDn->SetMarkerColor(kMagenta+2);
  hTTRMSDn->Draw("scat same");
  leg1->AddEntry(hTTRMSDn, "trigSF Down (RMS)", "p");
  leg1->Draw("same");
  cmstex->DrawLatex(0.15,0.96,"CMS Preliminary");
  c1->SaveAs("plot_unc_TrigSys_TT.pdf");

  TCanvas *c2 = new TCanvas("c2","c2");
  TLegend* leg2 = new TLegend(0.5,0.95,0.95,1.0);
  leg2->SetNColumns(4);
  leg2->SetFillStyle(0);
  leg2->SetBorderSize(0);
  hBBuncUp->SetMarkerStyle(20);
  hBBuncUp->SetMarkerSize(0.5);
  hBBuncUp->SetMarkerColorAlpha(kGray+2,0.5);
  hBBuncUp->GetXaxis()->SetTitle("B mass[GeV]");
  hBBuncUp->GetYaxis()->SetTitle("rate unc.");
  hBBuncUp->SetTitle("");
  hBBuncUp->GetYaxis()->SetRangeUser(yMin_BB,yMax_BB);
  hBBuncUp->Draw("scat");
  leg2->AddEntry(hBBuncUp, "trigSF Up", "p");
  hBBRMSUp->SetMarkerStyle(5);
  hBBRMSUp->SetMarkerSize(1.5);
  hBBRMSUp->SetMarkerColor(kBlack);
  hBBRMSUp->Draw("scat same");
  leg2->AddEntry(hBBRMSUp, "trigSF Up (RMS)", "p");
  hBBuncDn->SetMarkerColorAlpha(kRed-7,0.5);
  hBBuncDn->SetMarkerStyle(20);
  hBBuncDn->SetMarkerSize(0.5);
  hBBuncDn->Draw("scat same");
  leg2->AddEntry(hBBuncDn, "trigSF Down", "p");
  hBBRMSDn->SetMarkerStyle(5);
  hBBRMSDn->SetMarkerSize(1.5);
  hBBRMSDn->SetMarkerColor(kMagenta+2);
  hBBRMSDn->Draw("scat same");
  leg2->AddEntry(hBBRMSDn, "trigSF Down (RMS)", "p");
  leg2->Draw("same");
  cmstex->DrawLatex(0.15,0.96,"CMS Preliminary");
  c2->SaveAs("plot_unc_TrigSys_BB.pdf");

  ///----------MC Bkg-------
//   TString MCfolder ="/uscms_data/d3/rsyarif/CERN2017/produceLjmetNtuples_TTSSdilep/CMSSW_8_0_26_patch1/src/PostLJMet/X53ThirteenTeVAnalysisCode/test/ProcessedByJulie_Oct18-2017/test/";
  //TString MCfolder ="/uscms_data/d3/rsyarif/CERN2017/produceLjmetNtuples_TTSSdilep/CMSSW_8_0_26_patch1/src/PostLJMet/X53ThirteenTeVAnalysisCode/test/ProcessedByJulie_Oct20-2017/test/";
  TString MCfolder ="/eos/uscms/store/user/wywong/FWLJMET102X_2lep2017_wywong_082020_hadds_postFWLJMETanalyzer_IsoTrig_IDSFunc_nPU";
  
  std::ofstream outfile;
  outfile.open("unc_TrigSys_MCBkg.py");

  std::stringstream filecontent;
  filecontent<<std::fixed<<std::setprecision(5);
 
  filecontent<< "uncDict = {} \n"; 
  filecontent<< "uncDict['trigSFUp'] = {} \n";
  filecontent<< "uncDict['trigSFDn'] = {} \n";

  std::cout<<" ==========MC Bkg==========="<<std::endl;
  std::cout<<" Sample | trigSF Up | trigSF Down "<<std::endl;
  
  printLineMCBkg("TTZ",MCfolder+"/TTZ_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
  printLineMCBkg("TTW",MCfolder+"/TTW_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
  printLineMCBkg("TTH",MCfolder+"/TTH_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
  printLineMCBkg("TTTT",MCfolder+"/TTTT_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
  printLineMCBkg("WZ",MCfolder+"/WZ_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
  printLineMCBkg("ZZ",MCfolder+"/ZZ_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
  printLineMCBkg("WpWp",MCfolder+"/WpWp_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
  printLineMCBkg("WWZ",MCfolder+"/WWZ_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
  printLineMCBkg("WZZ",MCfolder+"/WZZ_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
  printLineMCBkg("ZZZ",MCfolder+"/ZZZ_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);

  outfile<<filecontent.str();
  outfile.close();


  gApplication->Terminate();
	  
}
