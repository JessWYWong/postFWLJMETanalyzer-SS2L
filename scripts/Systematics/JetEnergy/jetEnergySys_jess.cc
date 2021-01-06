#include <iostream>
#include <iomanip> 
#include <vector>
#include <string>
#include "TString.h"
#include "TRegexp.h"
#include "TLegend.h"
#include "TLatex.h"
#include "../../../plugins/SetTDRStyle.cc"

bool DEBUG = true;

int TTmass[] = {900,1000,1100,1200,1300,1400,1500,1600,1700,1800};
int BBmass[] = {900,1000,1100,1200,1300,1400,1500,1600,1700,1800};

int nTT = sizeof(TTmass)/sizeof(TTmass[0]);
int nBB = sizeof(BBmass)/sizeof(BBmass[0]);

TH2D* hTTuncUp_jec = new TH2D("uncUp_jec_TT","uncUp_jec_TT",(TTmass[nTT-1]-TTmass[0])+100,TTmass[0]-50,TTmass[nTT-1]+50,200,0.0,2.0);
TH2D* hTTuncDn_jec = new TH2D("uncDn_jec_TT","uncDn_jec_TT",(TTmass[nTT-1]-TTmass[0])+100,TTmass[0]-50,TTmass[nTT-1]+50,200,0.0,2.0);
TH2D* hTTRMSUp_jec = new TH2D("RMSUp_jec_TT","RMSUp_jec_TT",(TTmass[nTT-1]-TTmass[0])+100,TTmass[0]-50,TTmass[nTT-1]+50,200,0.0,2.0);
TH2D* hTTRMSDn_jec = new TH2D("RMSDn_jec_TT","RMSDn_jec_TT",(TTmass[nTT-1]-TTmass[0])+100,TTmass[0]-50,TTmass[nTT-1]+50,200,0.0,2.0);
TH2D* hBBuncUp_jec = new TH2D("uncUp_jec_BB","uncUp_jec_BB",(nBB+1)*(BBmass[1]-BBmass[0]),BBmass[0]-((BBmass[1]-BBmass[0])/2),BBmass[nBB-1]+((BBmass[1]-BBmass[0])/2),200,0.0,2.0);
TH2D* hBBuncDn_jec = new TH2D("uncDn_jec_BB","uncDn_jec_BB",(nBB+1)*(BBmass[1]-BBmass[0]),BBmass[0]-((BBmass[1]-BBmass[0])/2),BBmass[nBB-1]+((BBmass[1]-BBmass[0])/2),200,0.0,2.0);
TH2D* hBBRMSUp_jec = new TH2D("RMSUp_jec_BB","RMSUp_jec_BB",(nBB+1)*(BBmass[1]-BBmass[0]),BBmass[0]-((BBmass[1]-BBmass[0])/2),BBmass[nBB-1]+((BBmass[1]-BBmass[0])/2),200,0.0,2.0);
TH2D* hBBRMSDn_jec = new TH2D("RMSDn_jec_BB","RMSDn_jec_BB",(nBB+1)*(BBmass[1]-BBmass[0]),BBmass[0]-((BBmass[1]-BBmass[0])/2),BBmass[nBB-1]+((BBmass[1]-BBmass[0])/2),200,0.0,2.0);

TH2D* hTTuncUp_jer = new TH2D("uncUp_jer_TT","uncUp_jer_TT",(TTmass[nTT-1]-TTmass[0])+100,TTmass[0]-50,TTmass[nTT-1]+50,200,0.0,2.0);
TH2D* hTTuncDn_jer = new TH2D("uncDn_jer_TT","uncDn_jer_TT",(TTmass[nTT-1]-TTmass[0])+100,TTmass[0]-50,TTmass[nTT-1]+50,200,0.0,2.0);
TH2D* hTTRMSUp_jer = new TH2D("RMSUp_jer_TT","RMSUp_jer_TT",(TTmass[nTT-1]-TTmass[0])+100,TTmass[0]-50,TTmass[nTT-1]+50,200,0.0,2.0);
TH2D* hTTRMSDn_jer = new TH2D("RMSDn_jer_TT","RMSDn_jer_TT",(TTmass[nTT-1]-TTmass[0])+100,TTmass[0]-50,TTmass[nTT-1]+50,200,0.0,2.0);
TH2D* hBBuncUp_jer = new TH2D("uncUp_jer_BB","uncUp_jer_BB",(nBB+1)*(BBmass[1]-BBmass[0]),BBmass[0]-((BBmass[1]-BBmass[0])/2),BBmass[nBB-1]+((BBmass[1]-BBmass[0])/2),200,0.0,2.0);
TH2D* hBBuncDn_jer = new TH2D("uncDn_jer_BB","uncDn_jer_BB",(nBB+1)*(BBmass[1]-BBmass[0]),BBmass[0]-((BBmass[1]-BBmass[0])/2),BBmass[nBB-1]+((BBmass[1]-BBmass[0])/2),200,0.0,2.0);
TH2D* hBBRMSUp_jer = new TH2D("RMSUp_jer_BB","RMSUp_jer_BB",(nBB+1)*(BBmass[1]-BBmass[0]),BBmass[0]-((BBmass[1]-BBmass[0])/2),BBmass[nBB-1]+((BBmass[1]-BBmass[0])/2),200,0.0,2.0);
TH2D* hBBRMSDn_jer = new TH2D("RMSDn_jer_BB","RMSDn_jer_BB",(nBB+1)*(BBmass[1]-BBmass[0]),BBmass[0]-((BBmass[1]-BBmass[0])/2),BBmass[nBB-1]+((BBmass[1]-BBmass[0])/2),200,0.0,2.0);

float maxJesShift = 0.;
float maxJerShift = 0.;
float meanJesShift = 0.;
float meanJerShift = 0.;
float totalShifts = 0;

float getEvents(TTree* t,TString cut){
  float nEvents = t->Draw("Lep1Pt",cut);
  return nEvents;
}


void printLineMCBkg(std::string sample, TString filename,std::stringstream& filecontent){

/*
//   TString nomcut = "MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";
//   TString jesUpcut = "MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleUp > 1200)";
//   TString jesDowncut = "MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleDown > 1200)";
//   TString jerUpcut = "MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearUp > 1200)";
//   TString jerDowncut = "MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearDown > 1200)";

  TString nomcut = "MCWeight * (Lep1Pt>40 && nConst >=4 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";
  TString jesUpcut = "MCWeight * (Lep1Pt>40 && nConstJESup >=4 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleUp > 1200)";
  TString jesDowncut = "MCWeight * (Lep1Pt>40 && nConstJESdn >=4 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleDown > 1200)";
  TString jerUpcut = "MCWeight * (Lep1Pt>40 && nConstJERup >=4 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearUp > 1200)";
  TString jerDowncut = "MCWeight * (Lep1Pt>40 && nConstJERdn >=4 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearDown > 1200)";
*/

  TString nomcut = "ChargeMisIDWeight * NPWeight * IsoSF * trigSF * GsfSF * PUWeight * IDSF * MCWeight * (Lep1Pt>40 && nConst >=4 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";

  //for jes,jer
  //TRegexp reNconst("nConst");
  //TString jesUpcut = nomcut; jesUpcut(reNconst) = "nConstJESup";
  //TString jesDowncut = nomcut; jesDowncut(reNconst) = "nConstJESdn";
  //TString jerUpcut = nomcut; jerUpcut(reNconst) = "nConstJERup";
  //TString jerDowncut = nomcut; jerDowncut(reNconst) = "nConstJERdn";

  TFile* f = new TFile(filename);
  TTree* t = (TTree*) f->Get("tEvts_sZVeto");
  TTree* t_JECup = (TTree*) f->Get("tEvts_sZVeto_JECup");
  TTree* t_JECdn = (TTree*) f->Get("tEvts_sZVeto_JECdn");
  TTree* t_JERup = (TTree*) f->Get("tEvts_sZVeto_JERup");
  TTree* t_JERdn = (TTree*) f->Get("tEvts_sZVeto_JERdn");

  float nom = getEvents(t,nomcut);
  //std::cout<< "nom : " << nom <<std::endl;

  float jesup = getEvents(t_JECup,nomcut);
  //std::cout<< "jesup : " << jesup <<std::endl;
  float jesupPct = 1*(jesup-nom)/nom;

  float jesdown = getEvents(t_JECdn,nomcut);
  //std::cout<< "jesdown : " << jesdown <<std::endl;
  float jesdownPct = 1*(jesdown-nom)/nom;

  float jerup = getEvents(t_JERup,nomcut);
  //std::cout<< "jerup : " << jerup <<std::endl;
  float jerupPct = 1*(jerup-nom)/nom;

  float jerdown = getEvents(t_JERdn,nomcut);
  //std::cout<< jerDowncut <<std::endl;
  //std::cout<< "jerdown : " << jerdown <<std::endl;
  float jerdownPct = 1*(jerdown-nom)/nom;

  std::cout<<setprecision(5);
  std::cout<<sample<<" |  "<<1+jesupPct<<" |  "<<1+jesdownPct<<" |  "<<1+jerupPct<<" |  "<<1+jerdownPct << " ----> "<< fabs(jesupPct) << " | "<< fabs(jesdownPct)<< " | "<< fabs(jerupPct) << " | "<< fabs(jerdownPct) <<std::endl;
  
  //check in NaN = 0 
  if(jesupPct!=jesupPct)jesupPct=0.;
  if(jesdownPct!=jesdownPct)jesdownPct=0.;
  if(jerupPct!=jerupPct)jerupPct=0.;
  if(jerdownPct!=jerdownPct)jerdownPct=0.;

  filecontent<<"uncDict['jecUp']['"<< sample << "']="<<1+jesupPct<< "\n";
  filecontent<<"uncDict['jecDn']['"<< sample << "']="<<1+jesdownPct<< "\n";
  filecontent<<"uncDict['jerUp']['"<< sample << "']="<<1+jerupPct<< "\n";
  filecontent<<"uncDict['jerDn']['"<< sample << "']="<<1+jerdownPct<< "\n";

  delete t; delete f;

}

std::map<std::string,std::vector<float>> uncSig_jec = {};
std::map<std::string,std::vector<float>> uncSig_jer = {};
std::map<std::string, std::vector<float>>::iterator it;
void initSigMap(){
  for(int m : TTmass){
    uncSig_jec.insert(std::pair<std::string,std::vector<float>>("TTM"+std::to_string(m),{0.,0.,0.,0.,99.,99.}));//SUMSQUp_SUMSQDn_maxUp_maxDn_minUp_minDn 
    uncSig_jer.insert(std::pair<std::string,std::vector<float>>("TTM"+std::to_string(m),{0.,0.,0.,0.,99.,99.}));
  }
  for(int m : BBmass){
    uncSig_jec.insert(std::pair<std::string,std::vector<float>>("BBM"+std::to_string(m),{0.,0.,0.,0.,99.,99.}));
    uncSig_jer.insert(std::pair<std::string,std::vector<float>>("BBM"+std::to_string(m),{0.,0.,0.,0.,99.,99.}));
  }
}

void printLine(std::string sample, TString filename,std::stringstream& filecontent){


  TFile* f = new TFile(filename);
  if(DEBUG)std::cout << "Opening :"<< filename<< std::endl;

  TH1D* hnom = (TH1D*)f->Get("hist_nom");
  float nom = hnom->GetBinContent(1);//Integral();
  if(DEBUG) std::cout<< "nom : " << nom <<std::endl;
 
  TH1D* hjesup = (TH1D*)f->Get("hist_jesup");
  float jesup = hjesup->GetBinContent(1);//Integral();
  if(DEBUG) std::cout<< "jesup : " << jesup <<std::endl;
  float jesupPct = 1*(jesup-nom)/nom;
 
  TH1D* hjesdown = (TH1D*)f->Get("hist_jesdown");
  float jesdown = hjesdown->GetBinContent(1);//Integral();
  if(DEBUG) std::cout<< "jesdown : " << jesdown <<std::endl;
  float jesdownPct = 1*(jesdown-nom)/nom;
 
  TH1D* hjerup = (TH1D*)f->Get("hist_jerup");
  float jerup = hjerup->GetBinContent(1);//Integral();
  if(DEBUG) std::cout<< "jerup : " << jerup <<std::endl;
  float jerupPct = 1*(jerup-nom)/nom;
 
  TH1D* hjerdown = (TH1D*)f->Get("hist_jerdown");
  float jerdown = hjerdown->GetBinContent(1);//Integral();
  if(DEBUG) std::cout<< "jerdown : " << jerdown <<std::endl;
  float jerdownPct = 1*(jerdown-nom)/nom;
  std::cout<<setprecision(5);
//   std::cout<<sample<<" |  "<<jesupPct<<" |  "<<jesdownPct<<" |  "<<jerupPct<<" |  "<<jerdownPct<<std::endl;
  std::cout<<sample<<" |  "<<1+jesupPct<<" |  "<<1+jesdownPct<<" |  "<<1+jerupPct<<" |  "<<1+jerdownPct<<std::endl;
  filecontent<<"uncDict['jecUp']['"<< sample << "']="<<1+jesupPct<< "\n";
  filecontent<<"uncDict['jecDn']['"<< sample << "']="<<1+jesdownPct<< "\n";
  filecontent<<"uncDict['jerUp']['"<< sample << "']="<<1+jerupPct<< "\n";
  filecontent<<"uncDict['jerDn']['"<< sample << "']="<<1+jerdownPct<< "\n";


  if(maxJesShift < 1+fabs(jesupPct) ) maxJesShift = 1+fabs(jesupPct);
  if(maxJesShift < 1+fabs(jesdownPct) ) maxJesShift = 1+fabs(jesdownPct);
  if(maxJerShift < 1+fabs(jesupPct) ) maxJerShift = 1+fabs(jerupPct);
  if(maxJerShift < 1+fabs(jerdownPct) ) maxJerShift = 1+fabs(jerdownPct); 
  meanJesShift = meanJesShift+ (1+fabs(jesupPct));
  meanJesShift = meanJesShift+ (1+fabs(jesdownPct));
  meanJerShift = meanJerShift+ (1+fabs(jerupPct));
  meanJerShift = meanJerShift+ (1+fabs(jerdownPct));
  totalShifts += 2; 

  float sysUp_jec = 1+jesupPct;
  float sysDn_jec = 1+jesdownPct;
  float sysUp_jer = 1+jerupPct;
  float sysDn_jer = 1+jerdownPct;

  uncSig_jec.at(sample).at(0) += pow(sysUp_jec,2.0);
  uncSig_jec.at(sample).at(1) += pow(sysDn_jec,2.0);
  if(sysUp_jec > uncSig_jec.at(sample).at(2)) uncSig_jec.at(sample).at(2) = sysUp_jec;
  if(sysDn_jec > uncSig_jec.at(sample).at(3)) uncSig_jec.at(sample).at(3) = sysDn_jec;
  if(sysUp_jec < uncSig_jec.at(sample).at(4)) uncSig_jec.at(sample).at(4) = sysUp_jec;
  if(sysDn_jec < uncSig_jec.at(sample).at(5)) uncSig_jec.at(sample).at(5) = sysDn_jec;

  uncSig_jer.at(sample).at(0) += pow(sysUp_jer,2.0);
  uncSig_jer.at(sample).at(1) += pow(sysDn_jer,2.0);
  if(sysUp_jer > uncSig_jer.at(sample).at(2)) uncSig_jer.at(sample).at(2) = sysUp_jer;
  if(sysDn_jer > uncSig_jer.at(sample).at(3)) uncSig_jer.at(sample).at(3) = sysDn_jer;
  if(sysUp_jer < uncSig_jer.at(sample).at(4)) uncSig_jer.at(sample).at(4) = sysUp_jer;
  if(sysDn_jer < uncSig_jer.at(sample).at(5)) uncSig_jer.at(sample).at(5) = sysDn_jer;

  double m_sig = std::stod(sample.substr(3,sample.length()-3));
  if(sample.find("TTM")!=std::string::npos){
    hTTuncUp_jec->Fill(m_sig,sysUp_jec); hTTuncDn_jec->Fill(m_sig,sysDn_jec);
    hTTuncUp_jer->Fill(m_sig,sysUp_jer); hTTuncDn_jer->Fill(m_sig,sysDn_jer);
  }
  else if(sample.find("BBM")!=std::string::npos){
    hBBuncUp_jec->Fill(m_sig,sysUp_jec); hBBuncDn_jec->Fill(m_sig,sysDn_jec);
    hBBuncUp_jer->Fill(m_sig,sysUp_jer); hBBuncDn_jer->Fill(m_sig,sysDn_jer);
  }

  delete hnom;
  delete hjesup;
  delete hjesdown;
  delete hjerup;
  delete hjerdown;
  f->Close();
  delete f;

}


void jetEnergySys_jess(){

  TString elID = "MVA2017TightV2IsoTightRC";
  TString muID = "CBTightMiniIsoTight";
  TString era = "2018A-D";

  TString folder ="/uscms_data/d3/wywong/VLQ/CMSSW_10_2_10/src/postFWLJMETanalyzer_2018/scripts/Systematics/CombineTTsig/";
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

	  std::ofstream outfile;
	  outfile.open("unc_jec_jer_TT_"+decay+".py");

	  std::stringstream filecontent;
	  filecontent<<std::fixed<<std::setprecision(5);
	  
          filecontent<< "uncDict = {} \n";
          filecontent<< "uncDict['jecUp'] = {} \n";
          filecontent<< "uncDict['jecDn'] = {} \n";
          filecontent<< "uncDict['jerUp'] = {} \n";
          filecontent<< "uncDict['jerDn'] = {} \n";

	  //print top line
	  std::cout<<" =========="+decay+"==========="<<std::endl;
	  std::cout<<" Sample | JES Up | JES Down | JER Up | JER Down"<<std::endl;
          for(auto m : TTmass) printLine("TTM"+std::to_string(m),folder+"/TprimeTprime_M-"+std::to_string(m)+"_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
          /*  
	  //printLine("TTM800",folder+"/TprimeTprime_M-800_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
	  //printLine("TTM900",folder+"/TprimeTprime_M-900_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
	  printLine("TTM1000",folder+"/TprimeTprime_M-1000_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
	  printLine("TTM1100",folder+"/TprimeTprime_M-1100_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
	  printLine("TTM1200",folder+"/TprimeTprime_M-1200_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
	  printLine("TTM1300",folder+"/TprimeTprime_M-1300_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
	  printLine("TTM1400",folder+"/TprimeTprime_M-1400_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
	  printLine("TTM1500",folder+"/TprimeTprime_M-1500_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
	  printLine("TTM1600",folder+"/TprimeTprime_M-1600_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
	  printLine("TTM1700",folder+"/TprimeTprime_M-1700_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
          printLine("TTM1800",folder+"/TprimeTprime_M-1800_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
          */
	  outfile<<filecontent.str();
	  outfile.close();
  
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

	  std::ofstream outfile;
	  outfile.open("unc_jec_jer_BB_"+decay+".py");

	  std::stringstream filecontent;
	  filecontent<<std::fixed<<std::setprecision(5);
	  
          filecontent<< "uncDict = {} \n";
          filecontent<< "uncDict['jecUp'] = {} \n";
          filecontent<< "uncDict['jecDn'] = {} \n";
          filecontent<< "uncDict['jerUp'] = {} \n";
          filecontent<< "uncDict['jerDn'] = {} \n";

	  //print top line
	  std::cout<<" =========="+decay+"==========="<<std::endl;
	  std::cout<<" Sample | JES Up | JES Down | JER Up | JER Down"<<std::endl;
          for(auto m : BBmass) printLine("BBM"+std::to_string(m),folder+"/BprimeBprime_M-"+std::to_string(m)+"_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
          /*  
	  //printLine("BBM800",folder+"/BprimeBprime_M-800_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
	  printLine("BBM900",folder+"/BprimeBprime_M-900_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
	  printLine("BBM1000",folder+"/BprimeBprime_M-1000_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
	  printLine("BBM1100",folder+"/BprimeBprime_M-1100_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
	  printLine("BBM1200",folder+"/BprimeBprime_M-1200_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
	  printLine("BBM1300",folder+"/BprimeBprime_M-1300_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
	  printLine("BBM1400",folder+"/BprimeBprime_M-1400_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
	  printLine("BBM1500",folder+"/BprimeBprime_M-1500_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
	  printLine("BBM1600",folder+"/BprimeBprime_M-1600_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
	  printLine("BBM1700",folder+"/BprimeBprime_M-1700_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
          printLine("BBM1800",folder+"/BprimeBprime_M-1800_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root",filecontent);
          */
	  outfile<<filecontent.str();
	  outfile.close();

          countBB++;
  }
  
  std::cout<<std::setprecision(8);
  std::cout<<"\n"<<std::endl;

  std::cout<<" ====================="<<std::endl;
  std::cout<<" max JES  | max JER "<<std::endl;
  std::cout<< maxJesShift <<" | " << maxJerShift<<std::endl;
  
  //float totalShifts = (10 * 23 + 10 * 23) * 2 ;// (TT*BR + BB*BR) * up/down
  std::cout<<" ====================="<<std::endl;
  std::cout<<" mean JES  | mean JER "<<std::endl;
  std::cout<< meanJesShift / totalShifts <<" | " << meanJerShift / totalShifts <<std::endl;
  
  // store unc of signal samples by flavour and mass
  std::ofstream outfile_sig;
  outfile_sig.open("unc_jec_jer_sig.py");

  std::stringstream filecontent_sig;
  filecontent_sig<<std::fixed<<std::setprecision(5);

  filecontent_sig<< "uncDict = {} \n";
  filecontent_sig<< "uncDict['jecUp'] = {} \n";
  filecontent_sig<< "uncDict['jecDn'] = {} \n";
  filecontent_sig<< "uncDict['jerUp'] = {} \n";
  filecontent_sig<< "uncDict['jerDn'] = {} \n";

  std::cout<<"=================================================="<<std::endl;
  std::cout<<"===> over all BRtype (jec): " <<std::endl;
  std::cout<<" Sample  |  RMSUp_jec  |  RMSDn_jec  |  maxUp_jec  |  maxDn_jec  |  minUp_jec  |  minDn_jec  "<<std::endl;

  double yMax_TT_jec = 1.1;
  double yMin_TT_jec = 0.9;
  double yMax_BB_jec = 1.1;
  double yMin_BB_jec = 0.9;
  for( it = uncSig_jec.begin(); it != uncSig_jec.end(); it++ ){
    std::cout<< it->first <<" | "<< pow((it->second)[0]/(countTT),0.5) <<" | "<< pow((it->second)[1]/(countTT),0.5) <<" | "<< (it->second)[2]<<" | "<< (it->second)[3] <<" | "<< (it->second)[4] <<" | "<< (it->second)[5] <<std::endl;
    // use RMS values for all BRType
    filecontent_sig<<"uncDict['jecUp']['"<< it->first << "']="<< pow((it->second)[0]/(countTT),0.5) << "\n";
    filecontent_sig<<"uncDict['jecDn']['"<< it->first << "']="<< pow((it->second)[1]/(countTT),0.5) << "\n";

    double m_sig = std::stod((it->first).substr(3,(it->first).length()-3));
    if((it->first).find("TTM")!=std::string::npos){
      hTTRMSUp_jec->Fill(m_sig, pow((it->second)[0]/(countTT),0.5));
      hTTRMSDn_jec->Fill(m_sig, pow((it->second)[1]/(countTT),0.5));
      if((it->second)[2]>yMax_TT_jec) yMax_TT_jec=(it->second)[2];
      if((it->second)[3]>yMax_TT_jec) yMax_TT_jec=(it->second)[3];
      if((it->second)[4]<yMin_TT_jec) yMin_TT_jec=(it->second)[4];
      if((it->second)[5]<yMin_TT_jec) yMin_TT_jec=(it->second)[5];
    }
    else if ((it->first).find("BBM")!=std::string::npos){
      hBBRMSUp_jec->Fill(m_sig, pow((it->second)[0]/(countBB),0.5));
      hBBRMSDn_jec->Fill(m_sig, pow((it->second)[1]/(countBB),0.5));
      if((it->second)[2]>yMax_BB_jec) yMax_BB_jec=(it->second)[2];
      if((it->second)[3]>yMax_BB_jec) yMax_BB_jec=(it->second)[3];
      if((it->second)[4]<yMin_BB_jec) yMin_BB_jec=(it->second)[4];
      if((it->second)[5]<yMin_BB_jec) yMin_BB_jec=(it->second)[5];
    }
  }

  std::cout<<"=================================================="<<std::endl;
  std::cout<<"===> over all BRtype (jer): " <<std::endl;
  std::cout<<" Sample  |  RMSUp_jer  |  RMSDn_jer  |  maxUp_jer  |  maxDn_jer  |  minUp_jer  |  minDn_jer  "<<std::endl;
  double yMax_TT_jer = 1.1;
  double yMin_TT_jer = 0.9;
  double yMax_BB_jer = 1.1;
  double yMin_BB_jer = 0.9;
  for( it = uncSig_jer.begin(); it != uncSig_jer.end(); it++ ){
    std::cout<< it->first <<" | "<< pow((it->second)[0]/(countTT),0.5) <<" | "<< pow((it->second)[1]/(countTT),0.5) <<" | "<< (it->second)[2]<<" | "<< (it->second)[3] <<" | "<< (it->second)[4] <<" | "<< (it->second)[5] <<std::endl;

    // use RMS values for all BRType
    filecontent_sig<<"uncDict['jerUp']['"<< it->first << "']="<< pow((it->second)[0]/(countTT),0.5) << "\n";
    filecontent_sig<<"uncDict['jerDn']['"<< it->first << "']="<< pow((it->second)[1]/(countTT),0.5) << "\n";

    double m_sig = std::stod((it->first).substr(3,(it->first).length()-3));
    if((it->first).find("TTM")!=std::string::npos){
      hTTRMSUp_jer->Fill(m_sig, pow((it->second)[0]/(countTT),0.5));
      hTTRMSDn_jer->Fill(m_sig, pow((it->second)[1]/(countTT),0.5));
      if((it->second)[2]>yMax_TT_jer) yMax_TT_jer=(it->second)[2];
      if((it->second)[3]>yMax_TT_jer) yMax_TT_jer=(it->second)[3];
      if((it->second)[4]<yMin_TT_jer) yMin_TT_jer=(it->second)[4];
      if((it->second)[5]<yMin_TT_jer) yMin_TT_jer=(it->second)[5];
    }
    else if ((it->first).find("BBM")!=std::string::npos){
      hBBRMSUp_jer->Fill(m_sig, pow((it->second)[0]/(countBB),0.5));
      hBBRMSDn_jer->Fill(m_sig, pow((it->second)[1]/(countBB),0.5));
      if((it->second)[2]>yMax_BB_jer) yMax_BB_jer=(it->second)[2];
      if((it->second)[3]>yMax_BB_jer) yMax_BB_jer=(it->second)[3];
      if((it->second)[4]<yMin_BB_jer) yMin_BB_jer=(it->second)[4];
      if((it->second)[5]<yMin_BB_jer) yMin_BB_jer=(it->second)[5];
    }
  }

  outfile_sig<<filecontent_sig.str();
  outfile_sig.close();

  yMax_TT_jec+=0.02;yMin_TT_jec-=0.02;yMax_BB_jec+=0.02;yMin_BB_jec-=0.02;
  yMax_TT_jer+=0.02;yMin_TT_jer-=0.02;yMax_BB_jer+=0.02;yMin_BB_jer-=0.02;

  setTDRStyle();
  TLatex* cmstex = new TLatex();
  cmstex->SetNDC();
  cmstex->SetTextSize(0.04);

  TCanvas *c1_jec = new TCanvas("c1_jec","c1_jec");
  TLegend* leg1_jec = new TLegend(0.5,0.95,0.95,1.0);
  leg1_jec->SetNColumns(4);
  leg1_jec->SetFillStyle(0);
  leg1_jec->SetBorderSize(0);
  hTTuncUp_jec->SetMarkerStyle(20);
  hTTuncUp_jec->SetMarkerSize(0.5);
  hTTuncUp_jec->SetMarkerColorAlpha(kGray+2,0.5);
  hTTuncUp_jec->GetXaxis()->SetTitle("T mass[GeV]");
  hTTuncUp_jec->GetYaxis()->SetTitle("rate unc.");
  hTTuncUp_jec->SetTitle("");
  hTTuncUp_jec->GetYaxis()->SetRangeUser(yMin_TT_jec,yMax_TT_jec);
  hTTuncUp_jec->Draw("scat");
  leg1_jec->AddEntry(hTTuncUp_jec, "jec Up", "p");
  hTTRMSUp_jec->SetMarkerStyle(5);
  hTTRMSUp_jec->SetMarkerSize(1.5);
  hTTRMSUp_jec->SetMarkerColor(kBlack);
  hTTRMSUp_jec->Draw("scat same");
  leg1_jec->AddEntry(hTTRMSUp_jec, "jec Up (RMS)", "p");
  hTTuncDn_jec->SetMarkerColorAlpha(kRed-7,0.5);
  hTTuncDn_jec->SetMarkerStyle(20);
  hTTuncDn_jec->SetMarkerSize(0.5);
  hTTuncDn_jec->Draw("scat same");
  leg1_jec->AddEntry(hTTuncDn_jec, "jec Down", "p");
  hTTRMSDn_jec->SetMarkerStyle(5);
  hTTRMSDn_jec->SetMarkerSize(1.5);
  hTTRMSDn_jec->SetMarkerColor(kMagenta+2);
  hTTRMSDn_jec->Draw("scat same");
  leg1_jec->AddEntry(hTTRMSDn_jec, "jec Down (RMS)", "p");
  leg1_jec->Draw("same");
  cmstex->DrawLatex(0.15,0.96,"CMS Preliminary");
  c1_jec->SaveAs("plot_unc_jec_TT.pdf");

  TCanvas *c2_jec = new TCanvas("c2_jec","c2_jec");
  TLegend* leg2_jec = new TLegend(0.5,0.95,0.95,1.0);
  leg2_jec->SetNColumns(4);
  leg2_jec->SetFillStyle(0);
  leg2_jec->SetBorderSize(0);
  hBBuncUp_jec->SetMarkerStyle(20);
  hBBuncUp_jec->SetMarkerSize(0.5);
  hBBuncUp_jec->SetMarkerColorAlpha(kGray+2,0.5);
  hBBuncUp_jec->GetXaxis()->SetTitle("B mass[GeV]");
  hBBuncUp_jec->GetYaxis()->SetTitle("rate unc.");
  hBBuncUp_jec->SetTitle("");
  hBBuncUp_jec->GetYaxis()->SetRangeUser(yMin_BB_jec,yMax_BB_jec);
  hBBuncUp_jec->Draw("scat");
  leg2_jec->AddEntry(hBBuncUp_jec, "jec Up", "p");
  hBBRMSUp_jec->SetMarkerStyle(5);
  hBBRMSUp_jec->SetMarkerSize(1.5);
  hBBRMSUp_jec->SetMarkerColor(kBlack);
  hBBRMSUp_jec->Draw("scat same");
  leg2_jec->AddEntry(hBBRMSUp_jec, "jec Up (RMS)", "p");
  hBBuncDn_jec->SetMarkerColorAlpha(kRed-7,0.5);
  hBBuncDn_jec->SetMarkerStyle(20);
  hBBuncDn_jec->SetMarkerSize(0.5);
  hBBuncDn_jec->Draw("scat same");
  leg2_jec->AddEntry(hBBuncDn_jec, "jec Down", "p");
  hBBRMSDn_jec->SetMarkerStyle(5);
  hBBRMSDn_jec->SetMarkerSize(1.5);
  hBBRMSDn_jec->SetMarkerColor(kMagenta+2);
  hBBRMSDn_jec->Draw("scat same");
  leg2_jec->AddEntry(hBBRMSDn_jec, "jec Down (RMS)", "p");
  leg2_jec->Draw("same");
  cmstex->DrawLatex(0.15,0.96,"CMS Preliminary");
  c2_jec->SaveAs("plot_unc_jec_BB.pdf");

  TCanvas *c1_jer = new TCanvas("c1_jer","c1_jer");
  TLegend* leg1_jer = new TLegend(0.5,0.95,0.95,1.0);
  leg1_jer->SetNColumns(4);
  leg1_jer->SetFillStyle(0);
  leg1_jer->SetBorderSize(0);
  hTTuncUp_jer->SetMarkerStyle(20);
  hTTuncUp_jer->SetMarkerSize(0.5);
  hTTuncUp_jer->SetMarkerColorAlpha(kGray+2,0.5);
  hTTuncUp_jer->GetXaxis()->SetTitle("T mass[GeV]");
  hTTuncUp_jer->GetYaxis()->SetTitle("rate unc.");
  hTTuncUp_jer->SetTitle("");
  hTTuncUp_jer->GetYaxis()->SetRangeUser(yMin_TT_jer,yMax_TT_jer);
  hTTuncUp_jer->Draw("scat");
  leg1_jer->AddEntry(hTTuncUp_jer, "jer Up", "p");
  hTTRMSUp_jer->SetMarkerStyle(5);
  hTTRMSUp_jer->SetMarkerSize(1.5);
  hTTRMSUp_jer->SetMarkerColor(kBlack);
  hTTRMSUp_jer->Draw("scat same");
  leg1_jer->AddEntry(hTTRMSUp_jer, "jer Up (RMS)", "p");
  hTTuncDn_jer->SetMarkerColorAlpha(kRed-7,0.5);
  hTTuncDn_jer->SetMarkerStyle(20);
  hTTuncDn_jer->SetMarkerSize(0.5);
  hTTuncDn_jer->Draw("scat same");
  leg1_jer->AddEntry(hTTuncDn_jer, "jer Down", "p");
  hTTRMSDn_jer->SetMarkerStyle(5);
  hTTRMSDn_jer->SetMarkerSize(1.5);
  hTTRMSDn_jer->SetMarkerColor(kMagenta+2);
  hTTRMSDn_jer->Draw("scat same");
  leg1_jer->AddEntry(hTTRMSDn_jer, "jer Down (RMS)", "p");
  leg1_jer->Draw("same");
  cmstex->DrawLatex(0.15,0.96,"CMS Preliminary");
  c1_jer->SaveAs("plot_unc_jer_TT.pdf");

  TCanvas *c2_jer = new TCanvas("c2_jer","c2_jer");
  TLegend* leg2_jer = new TLegend(0.5,0.95,0.95,1.0);
  leg2_jer->SetNColumns(4);
  leg2_jer->SetFillStyle(0);
  leg2_jer->SetBorderSize(0);
  hBBuncUp_jer->SetMarkerStyle(20);
  hBBuncUp_jer->SetMarkerSize(0.5);
  hBBuncUp_jer->SetMarkerColorAlpha(kGray+2,0.5);
  hBBuncUp_jer->GetXaxis()->SetTitle("B mass[GeV]");
  hBBuncUp_jer->GetYaxis()->SetTitle("rate unc.");
  hBBuncUp_jer->SetTitle("");
  hBBuncUp_jer->GetYaxis()->SetRangeUser(yMin_BB_jer,yMax_BB_jer);
  hBBuncUp_jer->Draw("scat");
  leg2_jer->AddEntry(hBBuncUp_jer, "jer Up", "p");
  hBBRMSUp_jer->SetMarkerStyle(5);
  hBBRMSUp_jer->SetMarkerSize(1.5);
  hBBRMSUp_jer->SetMarkerColor(kBlack);
  hBBRMSUp_jer->Draw("scat same");
  leg2_jer->AddEntry(hBBRMSUp_jer, "jer Up (RMS)", "p");
  hBBuncDn_jer->SetMarkerColorAlpha(kRed-7,0.5);
  hBBuncDn_jer->SetMarkerStyle(20);
  hBBuncDn_jer->SetMarkerSize(0.5);
  hBBuncDn_jer->Draw("scat same");
  leg2_jer->AddEntry(hBBuncDn_jer, "jer Down", "p");
  hBBRMSDn_jer->SetMarkerStyle(5);
  hBBRMSDn_jer->SetMarkerSize(1.5);
  hBBRMSDn_jer->SetMarkerColor(kMagenta+2);
  hBBRMSDn_jer->Draw("scat same");
  leg2_jer->AddEntry(hBBRMSDn_jer, "jer Down (RMS)", "p");
  leg2_jer->Draw("same");
  cmstex->DrawLatex(0.15,0.96,"CMS Preliminary");
  c2_jer->SaveAs("plot_unc_jer_BB.pdf");
  
  ///----------MC Bkg-------

  TString MCfolder = "/eos/uscms/store/user/wywong/FWLJMET102X_2lep2018_wywong_082020_hadds_postFWLJMETanalyzer_IsoTrig_IDSFunc/";

  std::ofstream outfile;
  outfile.open("unc_jec_jer_MCBkg.py");

  std::stringstream filecontent;
  filecontent<<std::fixed<<std::setprecision(5);
  
  filecontent<< "uncDict = {} \n";
  filecontent<< "uncDict['jecUp'] = {} \n";
  filecontent<< "uncDict['jecDn'] = {} \n";
  filecontent<< "uncDict['jerUp'] = {} \n";
  filecontent<< "uncDict['jerDn'] = {} \n";

  std::cout<<" ==========MC Bkg==========="<<std::endl;
  std::cout<<" Sample | JES Up | JES Down | JER Up | JER Down"<<std::endl;
  
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

  ///----------MC Bkg------- done
  
  system("cat unc_jec_jer_MCBkg.py unc_jec_jer_TT_bW0p5tZ0p25tH0p25.py > unc_jec_jer_MCBkg_TT_bW0p5tZ0p25tH0p25.py");
  system("cat unc_jec_jer_MCBkg.py unc_jec_jer_TT_bW0p0tZ0p5tH0p5.py > unc_jec_jer_MCBkg_TT_bW0p0tZ0p5tH0p5.py");
  system("cat unc_jec_jer_MCBkg.py unc_jec_jer_TT_bW0p0tZ1p0tH0p0.py > unc_jec_jer_MCBkg_TT_bW0p0tZ1p0tH0p0.py");

  
  gApplication->Terminate();

}
