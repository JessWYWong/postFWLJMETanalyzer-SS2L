#include <iostream>
#include <iomanip>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include <vector>
#include "TLorentzVector.h"
#include "TChain.h"
#include "../interface/TreeReader.h"
#include <stdio.h>
#include <assert.h>
#include <map>
#include <string>
#include <sstream> 
#include <fstream>
#include "../plugins/Macros.cc"
#include <math.h>  //added by rizki
#include "TRegexp.h"

/* the point of this script is to produce a card file suitable for use with higgs combine tool or theta 
   It needs to take in three arguments: leading lepton pT shift, subleading lepton pT shift, HT shift
   where the default values are 30, 30, and 900 GeV (i.e. those of 2012 analysis) */

TH1* getPlot(Sample* s, TString hname, std::string var, int nxbins, double xbins[], std::string cut, int nMu, bool scale_, std::string addWeight = "", int shift = 0, std::string shiftWeight = "dummy"){

  bool scale = scale_;
  TTree* t = s->tree;

  TH1F* hdummy = new TH1F("hdummy","hdummy",nxbins,xbins);
  hdummy->Sumw2();

  //make cut string based on channel, should always be outside of parantheses for other cuts so a simply && should work also apply chargeMisIDWeight
  std::stringstream channel;
  if(nMu>=0)  channel<<"&& (Channel =="<<nMu<<")";
  else  channel<<"";

  std::string cutstring = addWeight+" 1.0 ";

  if( (s->name).find("Data")==std::string::npos){
      if(((s->name).find("NonPrompt")!=std::string::npos && ((s->name).find("TTJets")==std::string::npos) ) ||
         ( (s->name).find("ChargeMisID")!=std::string::npos && ((s->name).find("MC")==std::string::npos) )){
          cutstring = cutstring + " * ChargeMisIDWeight * NPWeight";
      }
      else {
          cutstring = cutstring + " * PUWeight * IDSF * IsoSF * trigSF * GsfSF * MCWeight * ChargeMisIDWeight * NPWeight* prefireWeight";
          if( (s->name).find("Tprime")!=std::string::npos || (s->name).find("Bprime")!=std::string::npos  ) cutstring = "pdfWeights4LHC[0] * " + cutstring;
          else  cutstring = " 1/abs(MCWeight) * " + cutstring;
      }
  }
  if (shift !=0){
      if(cutstring.find(shiftWeight)==std::string::npos)std::cout<<"WARNING! shiftWeiht name "<< shiftWeight <<" not found in cutstring!"<<std::endl;
      if(shift>0){
          if(shiftWeight.substr(shiftWeight.length()-2)=="SF")cutstring.replace(cutstring.find(shiftWeight), shiftWeight.length(), shiftWeight+"up");
          else cutstring.replace(cutstring.find(shiftWeight), shiftWeight.length(), shiftWeight+"Up");
      }
      if(shift<0){
          if(shiftWeight.substr(shiftWeight.length()-2)=="SF")cutstring.replace(cutstring.find(shiftWeight), shiftWeight.length(), shiftWeight+"dn");
          else cutstring.replace(cutstring.find(shiftWeight), shiftWeight.length(), shiftWeight+"Down");
      }
  }

  cutstring = cutstring+" * ( "+cut+channel.str()+")";
  if(nMu==0 && (s->name).find("BprimeBprime_M-1500")!=std::string::npos) std::cout<< "BBM1500 cutstring is "<< cutstring<<std::endl;
  if((s->name).find("NonPrompt")!=std::string::npos)std::cout<< hname<<" cutstring is "<<cutstring<< std::endl;
  t->Project("hdummy",var.c_str(),cutstring.c_str());

  //now weight properly - for Data don't apply
  if( ( (s->name).find("NonPrompt")!=std::string::npos) && ((s->name).find("TTJets")==std::string::npos) ) { scale=false; }
  else if( ( (s->name).find("ChargeMisID")!=std::string::npos) && ((s->name).find("MC")==std::string::npos) ) { scale=false; }

  if(scale){hdummy->Scale(s->weight);}
  if(nMu==0 && (s->name).find("BprimeBprime_M-1500")!=std::string::npos) std::cout<< "BBM1500 weight is "<< s->weight<<std::endl;

  //TRegexp re("__Trig");
  //if(hname.Contains("__Trig")){
  //    if(nMu == 0) hname(re) = "__eeTrig";
  //    else if(nMu == 1) hname(re) = "__emTrig";
  //    else if(nMu == 2) hname(re) = "__mmTrig";
  //    else std::cout<<"channel not identifiable!!"<<std::endl;
  //}
  hdummy->SetName(hname);
  hdummy->SetTitle("");
  hdummy->SetDirectory(0);

  return hdummy;
};

void overflow(TH1 *hdummy){
  int nBinsX = hdummy->GetNbinsX();
  double content = hdummy->GetBinContent(nBinsX) + hdummy->GetBinContent(nBinsX+1);
  double error = sqrtf(pow(hdummy->GetBinError(nBinsX),2.0)+pow(hdummy->GetBinError(nBinsX+1),2.0));
  hdummy->SetBinContent(nBinsX,content);
  hdummy->SetBinError(nBinsX,error);
  hdummy->SetBinContent(nBinsX+1,0.0);
  hdummy->SetBinError(nBinsX+1,0.0);
}

void checkBinContent(TH1 *hdummy){
  double norm0=hdummy->Integral();
  for(int iBin=0; iBin< hdummy->GetNbinsX()+2; iBin++){
      if(hdummy->GetBinContent(iBin)<0.0) hdummy->SetBinContent(iBin,0.0);
  }
  if(hdummy->Integral()!=0.0 && norm0>0.0) hdummy->Scale(norm0/hdummy->Integral());

  for(int i=1; i<hdummy->GetNbinsX()+1; i++){
      if(hdummy->GetBinContent(i)==0.0){
          hdummy->SetBinContent(i,1e-6);
          hdummy->SetBinError(i,sqrtf(1e-6));
      }
  }
  return;
}

int main(int argc, char* argv[]){

  //debug, set to true by hand until sure script is working
  bool debug_ = false;
  bool doAllSys = true;
  bool doNPSys = true;
  bool doFRScan = false;
  unsigned int scanRange = 10; // check in LJMetAnalyzer!!!!!!
  int uElFR = 1; // +1 for uFRup only & -1 for uFRdown only
  int uMuFR = 1; // +1 for uFRup only & -1 for uFRdown only
  bool normalizeRENORM_PDF = false;
  bool scaleSignalXsecTo1pb = false;
  double sigScale = 0.1; //pb
  if(scaleSignalXsecTo1pb) sigScale = 1.0; // pb
  std::string elID = "MVA2017TightV2IsoTightRC";
  std::string muID = "CBTightMiniIsoTight";
  //double xbins[20] = {190, 265, 340, 415, 490, 565, 640, 715, 790, 865, 940, 1015, 1090, 1165, 1240, 1315, 1390, 1540, 1765, 2965};
  //double xbins[16] = {190, 290, 390, 490, 590, 690, 790, 890, 990, 1090, 1190, 1290, 1390, 1590, 1790, 2990};
  //double xbins[31] = {0,100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000,2100,2200,2300,2400,2500,2600,2700,2800,2900,3000};
  //double xbins[6] = {1200, 1300, 1400, 1600, 1800, 3000};
  //double xbins[2] = {1200,10000};
  //double xbins[10] = {500, 600, 700, 800, 900, 1000, 1200, 1400, 1600, 3000};
  //double xbins[6] = {500, 600, 700, 800, 900, 1000};
  //double xbins[7] = {400, 500, 600, 700, 800, 900, 1000};
  //double xbins[9] = {200, 300, 400, 500, 600, 700, 800, 900, 1000};
  //double xbins[15] = {200.0, 300.0, 400.0, 500.0, 600.0, 700.0, 800.0, 900.0, 1000.0, 1100.0, 1200.0, 1300.0, 1500.0, 1700.0, 3000.0};
  double xbins[13] = {400.0, 500.0, 600.0, 700.0, 800.0, 900.0, 1000.0, 1100.0, 1200.0, 1300.0, 1500.0, 1700.0, 3000.0};
  int nxbins = sizeof(xbins)/sizeof(*xbins) - 1;
  // group backgrounds
  std::map<std::string,std::vector<std::string>> vGrpBkg{
      {"top",{"TTH","TTW","TTZ","TTTT"}},
      {"ewk",{"WpWp","WZ","ZZ","WWZ"}},//,"WZZ"}},
      {"TTV",{"TTH","TTW","TTZ"}},
      //{"VV" ,{"WpWp","WZ","ZZ"}},
      //{"VVV",{"WWZ"}},//,"WZZ"}},
      {"WZpZZ", {"WZ","ZZ"}},
      {"VVVpWW",{"WpWp","WWZ"}},//,"WZZ"}},
      //{"VVVpWZpZZ",{"WZ","ZZ","WWZ"}},//,"WZZ"}}
  };
  // match syst and systName
  std::vector<std::string> systList {"IDSF","trigSF","prefireWeight","PUWeight"};
  std::vector<TString> systNameList {"elIdSF2017","Trig17","prefire","pileup"};

  //check ot make sure enough arguments have been passed
  if(argc<3){
    std::cout<<"Need to supply at least 2 arguments: X53 Mass and Chirality! Exiting....."<<std::endl;
    return 0;
  }

  //get arguments
  std::string whichSignal(argv[1]);
  if( whichSignal.find("TT")==std::string::npos && whichSignal.find("BB")==std::string::npos) {std::cout<<"Invalid signal choice! Choose either \'TT\' or \'BB\'. Exiting...."<<std::endl; return 0;}

  float mass_lowEdge=0;
  std::istringstream arg2(argv[2]);
  if(!(arg2>>mass_lowEdge)){std::cout<<"Invalid number for T/B mass low edge! Exiting..."<<std::endl; return 0; }
  float mass_upEdge=0;
  std::istringstream arg3(argv[3]);
  if(!(arg3>>mass_upEdge)){std::cout<<"Invalid number for T/B mass up edge! Exiting..."<<std::endl; return 0; }

  //set cuts by hand
  float lep1cut = 40.0;  
  float lep2cut = 30.0;
  std::istringstream arg5(argv[5]);
  float lep2shift=0;
  arg5>>lep2shift;
  lep2cut = lep2cut+ lep2shift;
  std::istringstream arg4(argv[4]);
  float HTshift=0;
  if(!(arg4>>HTshift)){ std::cout<<"Invalid number for HT shift! Exiting..."<<std::endl; return 0;}
  else{arg4>>HTshift;}
  float HTcut   = 0+HTshift; //1000.0+HTshift; //edited by rizki

  //get desired lumi
  float lumi = std::atof(argv[6]); // fb^{-1}
  //get era
  std::string era(argv[7]);

  int nConst = 4; // edited by rizki

  //first get our favorite vectors of samples
  std::vector<Sample*> vMCBkg = getMCBkgSampleVec("sZVeto",lumi,elID,muID,era);
  std::vector<Sample*> vDDBkg = getDDBkgSampleVec("sZVeto",lumi,elID,muID,era);
  std::vector<Sample*> vBkg = appendSampleVectors(vMCBkg,vDDBkg);
  std::cout<<vMCBkg.at(0)<<std::endl;
  std::cout<<vDDBkg.at(0)<<std::endl;

  std::vector<Sample*> vMCBkg_JECup = getMCBkgSampleVec("sZVeto_JECup",lumi,elID,muID,era); 
  std::vector<Sample*> vMCBkg_JECdn = getMCBkgSampleVec("sZVeto_JECdn",lumi,elID,muID,era);
  std::vector<Sample*> vMCBkg_JERup = getMCBkgSampleVec("sZVeto_JERup",lumi,elID,muID,era);
  std::vector<Sample*> vMCBkg_JERdn = getMCBkgSampleVec("sZVeto_JERdn",lumi,elID,muID,era);


  //get BR for signal  
  int whichBR = std::stoi(argv[8]);

  // CR treatment
  bool isCR = false;
  double maxHT = 500;
  if(argc>9) { std::string argv9(argv[9]); if (argv9=="CR") isCR = true; maxHT = HTcut; HTcut = 200;}
  if(isCR && !(xbins[nxbins]==maxHT)){std::cout<<"WARNING! HT upper cut not matching the defined bins up edge"<<std::endl; return 1;}

  std::vector<Sample*> vSigSamples1, vSigSamples2, vSigSamples3, vSigSamples4, vSigSamples5, vSigSamples6, vSig;
  std::vector<Sample*> vSigSamples1_JECup, vSigSamples2_JECup, vSigSamples3_JECup, vSigSamples4_JECup, vSigSamples5_JECup, vSigSamples6_JECup, vSig_JECup;
  std::vector<Sample*> vSigSamples1_JECdn, vSigSamples2_JECdn, vSigSamples3_JECdn, vSigSamples4_JECdn, vSigSamples5_JECdn, vSigSamples6_JECdn, vSig_JECdn;
  std::vector<Sample*> vSigSamples1_JERup, vSigSamples2_JERup, vSigSamples3_JERup, vSigSamples4_JERup, vSigSamples5_JERup, vSigSamples6_JERup, vSig_JERup;
  std::vector<Sample*> vSigSamples1_JERdn, vSigSamples2_JERdn, vSigSamples3_JERdn, vSigSamples4_JERdn, vSigSamples5_JERdn, vSigSamples6_JERdn, vSig_JERdn;
  std::string BRstr;

  if(whichSignal == "TT"){
    if(whichBR==0) BRstr = "bW0p5_tZ0p25_tH0p25";
    if(whichBR==1) BRstr = "bW0p0_tZ1p0_tH0p0";
    if(whichBR==2) BRstr = "bW0p0_tZ0p8_tH0p2";
    if(whichBR==3) BRstr = "bW0p0_tZ0p6_tH0p4";
    if(whichBR==4) BRstr = "bW0p0_tZ0p4_tH0p6";
    if(whichBR==5) BRstr = "bW0p0_tZ0p2_tH0p8";
    if(whichBR==6) BRstr = "bW0p0_tZ0p0_tH1p0";
    if(whichBR==7) BRstr = "bW0p2_tZ0p8_tH0p0";
    if(whichBR==8) BRstr = "bW0p2_tZ0p6_tH0p2";
    if(whichBR==9) BRstr = "bW0p2_tZ0p4_tH0p4";
    if(whichBR==10) BRstr = "bW0p2_tZ0p2_tH0p6";
    if(whichBR==11) BRstr = "bW0p2_tZ0p0_tH0p8";
    if(whichBR==12) BRstr = "bW0p4_tZ0p6_tH0p0";
    if(whichBR==13) BRstr = "bW0p4_tZ0p4_tH0p2";
    if(whichBR==14) BRstr = "bW0p4_tZ0p2_tH0p4";
    if(whichBR==15) BRstr = "bW0p4_tZ0p0_tH0p6";
    if(whichBR==16) BRstr = "bW0p6_tZ0p4_tH0p0";
    if(whichBR==17) BRstr = "bW0p6_tZ0p2_tH0p2";
    if(whichBR==18) BRstr = "bW0p6_tZ0p0_tH0p4";
    if(whichBR==19) BRstr = "bW0p8_tZ0p2_tH0p0";
    if(whichBR==20) BRstr = "bW0p8_tZ0p0_tH0p2";
    if(whichBR==21) BRstr = "bW1p0_tZ0p0_tH0p0";
    if(whichBR==22) BRstr = "bW0p0_tZ0p5_tH0p5";
    if(whichBR==23) BRstr = "bW0p0_tZ0p9_tH0p1";
    if(whichBR==24) BRstr = "bW0p0_tZ0p7_tH0p3";
    if(whichBR==25) BRstr = "bW0p0_tZ0p3_tH0p7";
    if(whichBR==26) BRstr = "bW0p0_tZ0p1_tH0p9";
    if(whichBR==27) BRstr = "bW0p1_tZ0p9_tH0p0";
    if(whichBR==28) BRstr = "bW0p1_tZ0p8_tH0p1";
    if(whichBR==29) BRstr = "bW0p1_tZ0p7_tH0p2";
    if(whichBR==30) BRstr = "bW0p1_tZ0p6_tH0p3";
    if(whichBR==31) BRstr = "bW0p1_tZ0p5_tH0p4";
    if(whichBR==32) BRstr = "bW0p1_tZ0p4_tH0p5";
    if(whichBR==33) BRstr = "bW0p1_tZ0p3_tH0p6";
    if(whichBR==34) BRstr = "bW0p1_tZ0p2_tH0p7";
    if(whichBR==35) BRstr = "bW0p1_tZ0p1_tH0p8";
    if(whichBR==36) BRstr = "bW0p1_tZ0p0_tH0p9";
    if(whichBR==37) BRstr = "bW0p2_tZ0p7_tH0p1";
    if(whichBR==38) BRstr = "bW0p2_tZ0p5_tH0p3";
    if(whichBR==39) BRstr = "bW0p2_tZ0p3_tH0p5";
    if(whichBR==40) BRstr = "bW0p2_tZ0p1_tH0p7";
    if(whichBR==41) BRstr = "bW0p3_tZ0p7_tH0p0";
    if(whichBR==42) BRstr = "bW0p3_tZ0p6_tH0p1";
    if(whichBR==43) BRstr = "bW0p3_tZ0p5_tH0p2";
    if(whichBR==44) BRstr = "bW0p3_tZ0p4_tH0p3";
    if(whichBR==45) BRstr = "bW0p3_tZ0p3_tH0p4";
    if(whichBR==46) BRstr = "bW0p3_tZ0p2_tH0p5";
    if(whichBR==47) BRstr = "bW0p3_tZ0p1_tH0p6";
    if(whichBR==48) BRstr = "bW0p3_tZ0p0_tH0p7";
    if(whichBR==49) BRstr = "bW0p4_tZ0p5_tH0p1";
    if(whichBR==50) BRstr = "bW0p4_tZ0p3_tH0p3";
    if(whichBR==51) BRstr = "bW0p4_tZ0p1_tH0p5";
    if(whichBR==52) BRstr = "bW0p5_tZ0p5_tH0p0";
    if(whichBR==53) BRstr = "bW0p5_tZ0p4_tH0p1";
    if(whichBR==54) BRstr = "bW0p5_tZ0p3_tH0p2";
    if(whichBR==55) BRstr = "bW0p5_tZ0p2_tH0p3";
    if(whichBR==56) BRstr = "bW0p5_tZ0p1_tH0p4";
    if(whichBR==57) BRstr = "bW0p5_tZ0p0_tH0p5";
    if(whichBR==58) BRstr = "bW0p6_tZ0p3_tH0p1";
    if(whichBR==59) BRstr = "bW0p6_tZ0p1_tH0p3";
    if(whichBR==60) BRstr = "bW0p7_tZ0p3_tH0p0";
    if(whichBR==61) BRstr = "bW0p7_tZ0p2_tH0p1";
    if(whichBR==62) BRstr = "bW0p7_tZ0p1_tH0p2";
    if(whichBR==63) BRstr = "bW0p7_tZ0p0_tH0p3";
    if(whichBR==64) BRstr = "bW0p8_tZ0p1_tH0p1";
    if(whichBR==65) BRstr = "bW0p9_tZ0p1_tH0p0";
    if(whichBR==66) BRstr = "bW0p9_tZ0p0_tH0p1";

    //TT SIGNAL <BRstr> - RIZKI 
    //int whichBR = 1 ; //0 = singlet, 1 = doublet, 2 = tZ 100%
    vSigSamples1 = getInclusiveSigTTSampleVecForTable("sZVeto", lumi,elID,muID,era,"BWBW",whichBR);
    vSigSamples2 = getInclusiveSigTTSampleVecForTable("sZVeto", lumi,elID,muID,era,"THBW",whichBR);
    vSigSamples3 = getInclusiveSigTTSampleVecForTable("sZVeto", lumi,elID,muID,era,"THTH",whichBR);
    vSigSamples4 = getInclusiveSigTTSampleVecForTable("sZVeto", lumi,elID,muID,era,"TZBW",whichBR);
    vSigSamples5 = getInclusiveSigTTSampleVecForTable("sZVeto", lumi,elID,muID,era,"TZTH",whichBR);
    vSigSamples6 = getInclusiveSigTTSampleVecForTable("sZVeto", lumi,elID,muID,era,"TZTZ",whichBR);
    vSig = appendSampleVectors(vSigSamples1,vSigSamples2,vSigSamples3,vSigSamples4,vSigSamples5,vSigSamples6);

    if(doAllSys){
      vSigSamples1_JECup = getInclusiveSigTTSampleVecForTable("sZVeto_JECup", lumi,elID,muID,era,"BWBW",whichBR);
      vSigSamples2_JECup = getInclusiveSigTTSampleVecForTable("sZVeto_JECup", lumi,elID,muID,era,"THBW",whichBR);
      vSigSamples3_JECup = getInclusiveSigTTSampleVecForTable("sZVeto_JECup", lumi,elID,muID,era,"THTH",whichBR);
      vSigSamples4_JECup = getInclusiveSigTTSampleVecForTable("sZVeto_JECup", lumi,elID,muID,era,"TZBW",whichBR);
      vSigSamples5_JECup = getInclusiveSigTTSampleVecForTable("sZVeto_JECup", lumi,elID,muID,era,"TZTH",whichBR);
      vSigSamples6_JECup = getInclusiveSigTTSampleVecForTable("sZVeto_JECup", lumi,elID,muID,era,"TZTZ",whichBR);
      vSig_JECup = appendSampleVectors(vSigSamples1_JECup,vSigSamples2_JECup,vSigSamples3_JECup,vSigSamples4_JECup,vSigSamples5_JECup,vSigSamples6_JECup);
      vSigSamples1_JECdn = getInclusiveSigTTSampleVecForTable("sZVeto_JECdn", lumi,elID,muID,era,"BWBW",whichBR);
      vSigSamples2_JECdn = getInclusiveSigTTSampleVecForTable("sZVeto_JECdn", lumi,elID,muID,era,"THBW",whichBR);
      vSigSamples3_JECdn = getInclusiveSigTTSampleVecForTable("sZVeto_JECdn", lumi,elID,muID,era,"THTH",whichBR);
      vSigSamples4_JECdn = getInclusiveSigTTSampleVecForTable("sZVeto_JECdn", lumi,elID,muID,era,"TZBW",whichBR);
      vSigSamples5_JECdn = getInclusiveSigTTSampleVecForTable("sZVeto_JECdn", lumi,elID,muID,era,"TZTH",whichBR);
      vSigSamples6_JECdn = getInclusiveSigTTSampleVecForTable("sZVeto_JECdn", lumi,elID,muID,era,"TZTZ",whichBR);
      vSig_JECdn = appendSampleVectors(vSigSamples1_JECdn,vSigSamples2_JECdn,vSigSamples3_JECdn,vSigSamples4_JECdn,vSigSamples5_JECdn,vSigSamples6_JECdn);
      vSigSamples1_JERup = getInclusiveSigTTSampleVecForTable("sZVeto_JERup", lumi,elID,muID,era,"BWBW",whichBR);
      vSigSamples2_JERup = getInclusiveSigTTSampleVecForTable("sZVeto_JERup", lumi,elID,muID,era,"THBW",whichBR);
      vSigSamples3_JERup = getInclusiveSigTTSampleVecForTable("sZVeto_JERup", lumi,elID,muID,era,"THTH",whichBR);
      vSigSamples4_JERup = getInclusiveSigTTSampleVecForTable("sZVeto_JERup", lumi,elID,muID,era,"TZBW",whichBR);
      vSigSamples5_JERup = getInclusiveSigTTSampleVecForTable("sZVeto_JERup", lumi,elID,muID,era,"TZTH",whichBR);
      vSigSamples6_JERup = getInclusiveSigTTSampleVecForTable("sZVeto_JERup", lumi,elID,muID,era,"TZTZ",whichBR);
      vSig_JERup = appendSampleVectors(vSigSamples1_JERup,vSigSamples2_JERup,vSigSamples3_JERup,vSigSamples4_JERup,vSigSamples5_JERup,vSigSamples6_JERup);
      vSigSamples1_JERdn = getInclusiveSigTTSampleVecForTable("sZVeto_JERdn", lumi,elID,muID,era,"BWBW",whichBR);
      vSigSamples2_JERdn = getInclusiveSigTTSampleVecForTable("sZVeto_JERdn", lumi,elID,muID,era,"THBW",whichBR);
      vSigSamples3_JERdn = getInclusiveSigTTSampleVecForTable("sZVeto_JERdn", lumi,elID,muID,era,"THTH",whichBR);
      vSigSamples4_JERdn = getInclusiveSigTTSampleVecForTable("sZVeto_JERdn", lumi,elID,muID,era,"TZBW",whichBR);
      vSigSamples5_JERdn = getInclusiveSigTTSampleVecForTable("sZVeto_JERdn", lumi,elID,muID,era,"TZTH",whichBR);
      vSigSamples6_JERdn = getInclusiveSigTTSampleVecForTable("sZVeto_JERdn", lumi,elID,muID,era,"TZTZ",whichBR);
      vSig_JERdn = appendSampleVectors(vSigSamples1_JERdn,vSigSamples2_JERdn,vSigSamples3_JERdn,vSigSamples4_JERdn,vSigSamples5_JERdn,vSigSamples6_JERdn);
    }
  }
  else{
    if(whichBR==0) BRstr = "tW0p5_bZ0p25_bH0p25";
    if(whichBR==1) BRstr = "tW0p0_bZ1p0_bH0p0";
    if(whichBR==2) BRstr = "tW0p0_bZ0p8_bH0p2";
    if(whichBR==3) BRstr = "tW0p0_bZ0p6_bH0p4";
    if(whichBR==4) BRstr = "tW0p0_bZ0p4_bH0p6";
    if(whichBR==5) BRstr = "tW0p0_bZ0p2_bH0p8";
    if(whichBR==6) BRstr = "tW0p0_bZ0p0_bH1p0";
    if(whichBR==7) BRstr = "tW0p2_bZ0p8_bH0p0";
    if(whichBR==8) BRstr = "tW0p2_bZ0p6_bH0p2";
    if(whichBR==9) BRstr = "tW0p2_bZ0p4_bH0p4";
    if(whichBR==10) BRstr = "tW0p2_bZ0p2_bH0p6";
    if(whichBR==11) BRstr = "tW0p2_bZ0p0_bH0p8";
    if(whichBR==12) BRstr = "tW0p4_bZ0p6_bH0p0";
    if(whichBR==13) BRstr = "tW0p4_bZ0p4_bH0p2";
    if(whichBR==14) BRstr = "tW0p4_bZ0p2_bH0p4";
    if(whichBR==15) BRstr = "tW0p4_bZ0p0_bH0p6";
    if(whichBR==16) BRstr = "tW0p6_bZ0p4_bH0p0";
    if(whichBR==17) BRstr = "tW0p6_bZ0p2_bH0p2";
    if(whichBR==18) BRstr = "tW0p6_bZ0p0_bH0p4";
    if(whichBR==19) BRstr = "tW0p8_bZ0p2_bH0p0";
    if(whichBR==20) BRstr = "tW0p8_bZ0p0_bH0p2";
    if(whichBR==21) BRstr = "tW1p0_bZ0p0_bH0p0";
    if(whichBR==22) BRstr = "tW0p0_bZ0p5_bH0p5";
    if(whichBR==23) BRstr = "tW0p0_bZ0p9_bH0p1";
    if(whichBR==24) BRstr = "tW0p0_bZ0p7_bH0p3";
    if(whichBR==25) BRstr = "tW0p0_bZ0p3_bH0p7";
    if(whichBR==26) BRstr = "tW0p0_bZ0p1_bH0p9";
    if(whichBR==27) BRstr = "tW0p1_bZ0p9_bH0p0";
    if(whichBR==28) BRstr = "tW0p1_bZ0p8_bH0p1";
    if(whichBR==29) BRstr = "tW0p1_bZ0p7_bH0p2";
    if(whichBR==30) BRstr = "tW0p1_bZ0p6_bH0p3";
    if(whichBR==31) BRstr = "tW0p1_bZ0p5_bH0p4";
    if(whichBR==32) BRstr = "tW0p1_bZ0p4_bH0p5";
    if(whichBR==33) BRstr = "tW0p1_bZ0p3_bH0p6";
    if(whichBR==34) BRstr = "tW0p1_bZ0p2_bH0p7";
    if(whichBR==35) BRstr = "tW0p1_bZ0p1_bH0p8";
    if(whichBR==36) BRstr = "tW0p1_bZ0p0_bH0p9";
    if(whichBR==37) BRstr = "tW0p2_bZ0p7_bH0p1";
    if(whichBR==38) BRstr = "tW0p2_bZ0p5_bH0p3";
    if(whichBR==39) BRstr = "tW0p2_bZ0p3_bH0p5";
    if(whichBR==40) BRstr = "tW0p2_bZ0p1_bH0p7";
    if(whichBR==41) BRstr = "tW0p3_bZ0p7_bH0p0";
    if(whichBR==42) BRstr = "tW0p3_bZ0p6_bH0p1";
    if(whichBR==43) BRstr = "tW0p3_bZ0p5_bH0p2";
    if(whichBR==44) BRstr = "tW0p3_bZ0p4_bH0p3";
    if(whichBR==45) BRstr = "tW0p3_bZ0p3_bH0p4";
    if(whichBR==46) BRstr = "tW0p3_bZ0p2_bH0p5";
    if(whichBR==47) BRstr = "tW0p3_bZ0p1_bH0p6";
    if(whichBR==48) BRstr = "tW0p3_bZ0p0_bH0p7";
    if(whichBR==49) BRstr = "tW0p4_bZ0p5_bH0p1";
    if(whichBR==50) BRstr = "tW0p4_bZ0p3_bH0p3";
    if(whichBR==51) BRstr = "tW0p4_bZ0p1_bH0p5";
    if(whichBR==52) BRstr = "tW0p5_bZ0p5_bH0p0";
    if(whichBR==53) BRstr = "tW0p5_bZ0p4_bH0p1";
    if(whichBR==54) BRstr = "tW0p5_bZ0p3_bH0p2";
    if(whichBR==55) BRstr = "tW0p5_bZ0p2_bH0p3";
    if(whichBR==56) BRstr = "tW0p5_bZ0p1_bH0p4";
    if(whichBR==57) BRstr = "tW0p5_bZ0p0_bH0p5";
    if(whichBR==58) BRstr = "tW0p6_bZ0p3_bH0p1";
    if(whichBR==59) BRstr = "tW0p6_bZ0p1_bH0p3";
    if(whichBR==60) BRstr = "tW0p7_bZ0p3_bH0p0";
    if(whichBR==61) BRstr = "tW0p7_bZ0p2_bH0p1";
    if(whichBR==62) BRstr = "tW0p7_bZ0p1_bH0p2";
    if(whichBR==63) BRstr = "tW0p7_bZ0p0_bH0p3";
    if(whichBR==64) BRstr = "tW0p8_bZ0p1_bH0p1";
    if(whichBR==65) BRstr = "tW0p9_bZ0p1_bH0p0";
    if(whichBR==66) BRstr = "tW0p9_bZ0p0_bH0p1";

    //BB SIGNAL <BRstr> - RIZKI 
    //int whichBR = 0 ; //0 = singlet, 1 = doublet, 2 = tW 100%
    vSigSamples1 = getInclusiveSigBBSampleVecForTable("sZVeto", lumi,elID,muID,era,"TWTW",whichBR);
    vSigSamples2 = getInclusiveSigBBSampleVecForTable("sZVeto", lumi,elID,muID,era,"BHTW",whichBR);
    vSigSamples3 = getInclusiveSigBBSampleVecForTable("sZVeto", lumi,elID,muID,era,"BHBH",whichBR);
    vSigSamples4 = getInclusiveSigBBSampleVecForTable("sZVeto", lumi,elID,muID,era,"BZTW",whichBR);
    vSigSamples5 = getInclusiveSigBBSampleVecForTable("sZVeto", lumi,elID,muID,era,"BZBH",whichBR);
    vSigSamples6 = getInclusiveSigBBSampleVecForTable("sZVeto", lumi,elID,muID,era,"BZBZ",whichBR);
    vSig = appendSampleVectors(vSigSamples1,vSigSamples2,vSigSamples3,vSigSamples4,vSigSamples5,vSigSamples6);

    if(doAllSys){
      vSigSamples1_JECup = getInclusiveSigBBSampleVecForTable("sZVeto_JECup", lumi,elID,muID,era,"TWTW",whichBR);
      vSigSamples2_JECup = getInclusiveSigBBSampleVecForTable("sZVeto_JECup", lumi,elID,muID,era,"BHTW",whichBR);
      vSigSamples3_JECup = getInclusiveSigBBSampleVecForTable("sZVeto_JECup", lumi,elID,muID,era,"BHBH",whichBR);
      vSigSamples4_JECup = getInclusiveSigBBSampleVecForTable("sZVeto_JECup", lumi,elID,muID,era,"BZTW",whichBR);
      vSigSamples5_JECup = getInclusiveSigBBSampleVecForTable("sZVeto_JECup", lumi,elID,muID,era,"BZBH",whichBR);
      vSigSamples6_JECup = getInclusiveSigBBSampleVecForTable("sZVeto_JECup", lumi,elID,muID,era,"BZBZ",whichBR);
      vSig_JECup = appendSampleVectors(vSigSamples1_JECup,vSigSamples2_JECup,vSigSamples3_JECup,vSigSamples4_JECup,vSigSamples5_JECup,vSigSamples6_JECup);
      vSigSamples1_JECdn = getInclusiveSigBBSampleVecForTable("sZVeto_JECdn", lumi,elID,muID,era,"TWTW",whichBR);
      vSigSamples2_JECdn = getInclusiveSigBBSampleVecForTable("sZVeto_JECdn", lumi,elID,muID,era,"BHTW",whichBR);
      vSigSamples3_JECdn = getInclusiveSigBBSampleVecForTable("sZVeto_JECdn", lumi,elID,muID,era,"BHBH",whichBR);
      vSigSamples4_JECdn = getInclusiveSigBBSampleVecForTable("sZVeto_JECdn", lumi,elID,muID,era,"BZTW",whichBR);
      vSigSamples5_JECdn = getInclusiveSigBBSampleVecForTable("sZVeto_JECdn", lumi,elID,muID,era,"BZBH",whichBR);
      vSigSamples6_JECdn = getInclusiveSigBBSampleVecForTable("sZVeto_JECdn", lumi,elID,muID,era,"BZBZ",whichBR);
      vSig_JECdn = appendSampleVectors(vSigSamples1_JECdn,vSigSamples2_JECdn,vSigSamples3_JECdn,vSigSamples4_JECdn,vSigSamples5_JECdn,vSigSamples6_JECdn);
      vSigSamples1_JERup = getInclusiveSigBBSampleVecForTable("sZVeto_JERup", lumi,elID,muID,era,"TWTW",whichBR);
      vSigSamples2_JERup = getInclusiveSigBBSampleVecForTable("sZVeto_JERup", lumi,elID,muID,era,"BHTW",whichBR);
      vSigSamples3_JERup = getInclusiveSigBBSampleVecForTable("sZVeto_JERup", lumi,elID,muID,era,"BHBH",whichBR);
      vSigSamples4_JERup = getInclusiveSigBBSampleVecForTable("sZVeto_JERup", lumi,elID,muID,era,"BZTW",whichBR);
      vSigSamples5_JERup = getInclusiveSigBBSampleVecForTable("sZVeto_JERup", lumi,elID,muID,era,"BZBH",whichBR);
      vSigSamples6_JERup = getInclusiveSigBBSampleVecForTable("sZVeto_JERup", lumi,elID,muID,era,"BZBZ",whichBR);
      vSig_JERup = appendSampleVectors(vSigSamples1_JERup,vSigSamples2_JERup,vSigSamples3_JERup,vSigSamples4_JERup,vSigSamples5_JERup,vSigSamples6_JERup);
      vSigSamples1_JERdn = getInclusiveSigBBSampleVecForTable("sZVeto_JERdn", lumi,elID,muID,era,"TWTW",whichBR);
      vSigSamples2_JERdn = getInclusiveSigBBSampleVecForTable("sZVeto_JERdn", lumi,elID,muID,era,"BHTW",whichBR);
      vSigSamples3_JERdn = getInclusiveSigBBSampleVecForTable("sZVeto_JERdn", lumi,elID,muID,era,"BHBH",whichBR);
      vSigSamples4_JERdn = getInclusiveSigBBSampleVecForTable("sZVeto_JERdn", lumi,elID,muID,era,"BZTW",whichBR);
      vSigSamples5_JERdn = getInclusiveSigBBSampleVecForTable("sZVeto_JERdn", lumi,elID,muID,era,"BZBH",whichBR);
      vSigSamples6_JERdn = getInclusiveSigBBSampleVecForTable("sZVeto_JERdn", lumi,elID,muID,era,"BZBZ",whichBR);
      vSig_JERdn = appendSampleVectors(vSigSamples1_JERdn,vSigSamples2_JERdn,vSigSamples3_JERdn,vSigSamples4_JERdn,vSigSamples5_JERdn,vSigSamples6_JERdn);
    }
  }

  Sample* dataSample = getDataSample("sZVeto",elID,muID,era);

  //now make cut string according to cuts specified:
  std::vector<std::string> vCutString;
  std::stringstream cutSStream;
  bool minDRcut = false;
  cutSStream<<" ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1)) && (DilepMass>20) && (nConst>="<<nConst<<" ) && (Lep1Pt >"<<lep1cut<<") && (Lep2Pt > "<<lep2cut<<") && ( cleanAK4HT > "<<HTcut<<") && ( nNonSSLeps == 0)"; //added by rizki, add exactly 2 leptons (SS) only.
  if(isCR) cutSStream<< " && ( cleanAK4HT < "<<maxHT<<")";
  if(minDRcut) cutSStream<< " && Lep1MinDR >0.4 && Lep2MinDR > 0.4";
  vCutString.push_back(cutSStream.str());

  if(debug_) std::cout<<"Cutstring is: "<<cutSStream.str()<<std::endl;

  std::stringstream dirName;
  dirName << "grouped_" ;
  if(isCR) dirName<< "CR_maxHT"<<maxHT<<"_";
  if(minDRcut) dirName<< "minDR0p4_";
  dirName<< "Lep1Pt"<<lep1cut<<"_Lep2Pt"<<lep2cut<<"_HT"<<HTcut<<"_nConst"<<nConst<<"/Templates_rootfiles_Combine";
  if(!scaleSignalXsecTo1pb) dirName<<std::to_string(sigScale).replace(std::to_string(sigScale).find("."),1,"p").erase(std::to_string(sigScale).find_last_not_of('0')+1,std::string::npos)<<"pb";
  dirName<<"/" ;
  system(("mkdir -pv "+dirName.str()).c_str() ); 
  std::stringstream rootfilename;
  rootfilename<<"templates_"+whichSignal<<"_"<<BRstr<<"_Combine.root";
  TFile* fout = new TFile((dirName.str()+rootfilename.str()).c_str(),"RECREATE");

  //write observed
  TString erast="";
  if(era=="2017B-F") erast="2017BF";
  else erast="UnknownEra";

  //////////// Data ////////////
  TH1F* elel__data_obs=(TH1F*)getPlot(dataSample,"elel"+erast+"__data_obs","cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true);
  TH1F* elmu__data_obs=(TH1F*)getPlot(dataSample,"elmu"+erast+"__data_obs","cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true);
  TH1F* mumu__data_obs=(TH1F*)getPlot(dataSample,"mumu"+erast+"__data_obs","cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true);

  fout->WriteTObject(elel__data_obs);
  fout->WriteTObject(elmu__data_obs);
  fout->WriteTObject(mumu__data_obs);

  //////////// Sig ////////////
  for (int mass = mass_lowEdge; mass < mass_upEdge+100; mass+=100){
    if(debug_) std::cout<<"Mass is: "<< mass <<std::endl;
    if(isCR && mass!=1000 && mass !=1500) continue;
    std::stringstream mss; mss<<mass;
    std::string mstring = mss.str();

    std::vector<Sample*> sigSample;
    for(std::vector<Sample*>::size_type i=0; i< vSig.size(); i++){
      if( vSig.at(i)->name.find("M-"+mstring)!=std::string::npos){
        sigSample.push_back(vSig.at(i));
        }
    }

    std::vector<TH1F*> writeHist {};
    //////////// --> Get nominal hist for each mass points (combining all possible decays)
    TString postfix = erast+"__"+whichSignal+"M"+mstring;
    TH1F* elel__sig = new TH1F("elel"+postfix,"",nxbins,xbins); elel__sig->Sumw2();
    TH1F* elmu__sig = new TH1F("elmu"+postfix,"",nxbins,xbins); elmu__sig->Sumw2();
    TH1F* mumu__sig = new TH1F("mumu"+postfix,"",nxbins,xbins); mumu__sig->Sumw2();
    for(std::vector<Sample*>::size_type i=0; i< sigSample.size(); i++){
      TH1F* elel_temp = (TH1F*)getPlot(sigSample.at(i),"elel"+postfix+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true);
      elel_temp->Scale(1.0*sigScale/sigSample.at(i)->xsec);
      TH1F* elmu_temp = (TH1F*)getPlot(sigSample.at(i),"elmu"+postfix+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true);
      elmu_temp->Scale(1.0*sigScale/sigSample.at(i)->xsec);
      TH1F* mumu_temp = (TH1F*)getPlot(sigSample.at(i),"mumu"+postfix+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true);
      mumu_temp->Scale(1.0*sigScale/sigSample.at(i)->xsec);
      elel__sig->Add(elel_temp);
      elmu__sig->Add(elmu_temp);
      mumu__sig->Add(mumu_temp);
    }
    writeHist.push_back(elel__sig);writeHist.push_back(elmu__sig);writeHist.push_back(mumu__sig);

    if(doAllSys){
      std::vector<TH1F*> elel__sig_Up,elel__sig_Dn,elmu__sig_Up,elmu__sig_Dn,mumu__sig_Up,mumu__sig_Dn;
      for(unsigned int isyst = 0; isyst < systList.size(); isyst++){
        std::string shiftWeight = systList.at(isyst);
        TString postfix_weight = erast+"__"+whichSignal+"M"+mstring+"__"+systNameList.at(isyst);
        elel__sig_Up.push_back(new TH1F("elel"+postfix_weight+"Up","",nxbins,xbins));
        elel__sig_Dn.push_back(new TH1F("elel"+postfix_weight+"Down","",nxbins,xbins));
        elmu__sig_Up.push_back(new TH1F("elmu"+postfix_weight+"Up","",nxbins,xbins));
        elmu__sig_Dn.push_back(new TH1F("elmu"+postfix_weight+"Down","",nxbins,xbins));
        mumu__sig_Up.push_back(new TH1F("mumu"+postfix_weight+"Up","",nxbins,xbins));
        mumu__sig_Dn.push_back(new TH1F("mumu"+postfix_weight+"Down","",nxbins,xbins));
        if(!(elel__sig_Up.at(isyst)->Integral() == 0.0 && elel__sig_Dn.at(isyst)->Integral() == 0.0 && elmu__sig_Up.at(isyst)->Integral() == 0.0 && elmu__sig_Dn.at(isyst)->Integral() == 0.0 && mumu__sig_Up.at(isyst)->Integral() == 0.0 && mumu__sig_Dn.at(isyst)->Integral() == 0.0)) std::cout <<"Warning!!!! Incorrect initialization of shifted signal hist"<<std::endl;
	for(std::vector<Sample*>::size_type i=0; i< sigSample.size(); i++){
            TH1F* elel_tempUp = (TH1F*)getPlot(sigSample.at(i),"elel"+postfix_weight+"Up_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true,"",1,shiftWeight);
            elel_tempUp->Scale(1.0*sigScale/sigSample.at(i)->xsec);
            elel__sig_Up.at(isyst)->Add(elel_tempUp);

            TH1F* elel_tempDn = (TH1F*)getPlot(sigSample.at(i),"elel"+postfix_weight+"Down_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true,"",-1,shiftWeight);
            elel_tempDn->Scale(1.0*sigScale/sigSample.at(i)->xsec);
            elel__sig_Dn.at(isyst)->Add(elel_tempDn);

            TH1F* elmu_tempUp = (TH1F*)getPlot(sigSample.at(i),"elmu"+postfix_weight+"Up_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"",1,shiftWeight);
            elmu_tempUp->Scale(1.0*sigScale/sigSample.at(i)->xsec);
            elmu__sig_Up.at(isyst)->Add(elmu_tempUp);

            TH1F* elmu_tempDn = (TH1F*)getPlot(sigSample.at(i),"elmu"+postfix_weight+"Down_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"",-1,shiftWeight);
            elmu_tempDn->Scale(1.0*sigScale/sigSample.at(i)->xsec);
            elmu__sig_Dn.at(isyst)->Add(elmu_tempDn);

            TH1F* mumu_tempUp = (TH1F*)getPlot(sigSample.at(i),"mumu"+postfix_weight+"Up_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true,"",1,shiftWeight);
            mumu_tempUp->Scale(1.0*sigScale/sigSample.at(i)->xsec);
            mumu__sig_Up.at(isyst)->Add(mumu_tempUp);

            TH1F* mumu_tempDn = (TH1F*)getPlot(sigSample.at(i),"mumu"+postfix_weight+"Down_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true,"",-1,shiftWeight);
            mumu_tempDn->Scale(1.0*sigScale/sigSample.at(i)->xsec);
            mumu__sig_Dn.at(isyst)->Add(mumu_tempDn);
        }
        writeHist.push_back(elel__sig_Up.at(isyst)); writeHist.push_back(elel__sig_Dn.at(isyst));
        writeHist.push_back(elmu__sig_Up.at(isyst)); writeHist.push_back(elmu__sig_Dn.at(isyst));
        writeHist.push_back(mumu__sig_Up.at(isyst)); writeHist.push_back(mumu__sig_Dn.at(isyst));
      }

      //////////// --> Get muRF hists : loop through the 6 renormweights and find max and min for each bin -> scale by SFs (by year)
      TString postfix_muRF = erast+"__"+whichSignal+"M"+mstring+"__muRFcorrdNewSig";
      TH1F* elel__sig_muRFUp = new TH1F("elel"+postfix_muRF+"Up","",nxbins,xbins);
      TH1F* elel__sig_muRFDn = new TH1F("elel"+postfix_muRF+"Down","",nxbins,xbins);
      TH1F* elmu__sig_muRFUp = new TH1F("elmu"+postfix_muRF+"Up","",nxbins,xbins);
      TH1F* elmu__sig_muRFDn = new TH1F("elmu"+postfix_muRF+"Down","",nxbins,xbins);
      TH1F* mumu__sig_muRFUp = new TH1F("mumu"+postfix_muRF+"Up","",nxbins,xbins);
      TH1F* mumu__sig_muRFDn = new TH1F("mumu"+postfix_muRF+"Down","",nxbins,xbins);

      std::vector<TH1F*> hists_elel__sig_muRF, hists_elmu__sig_muRF, hists_mumu__sig_muRF;
      for(int ind=0; ind<6; ind++){
        TH1F* elel__sig_muRF = new TH1F("elel"+postfix_muRF+"_"+std::to_string(ind),"",nxbins,xbins); elel__sig_muRF->Sumw2();
        TH1F* elmu__sig_muRF = new TH1F("elmu"+postfix_muRF+"_"+std::to_string(ind),"",nxbins,xbins); elmu__sig_muRF->Sumw2();
        TH1F* mumu__sig_muRF = new TH1F("mumu"+postfix_muRF+"_"+std::to_string(ind),"",nxbins,xbins); mumu__sig_muRF->Sumw2();
        for(std::vector<Sample*>::size_type i=0; i< sigSample.size(); i++){
          TH1F* elel_temp_muRF = (TH1F*)getPlot(sigSample.at(i),"elel"+postfix_muRF+"_"+std::to_string(ind)+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true,"renormWeights["+std::to_string(ind)+"] * ");
          elel_temp_muRF->Scale(1.0*sigScale/sigSample.at(i)->xsec);
          TH1F* elmu_temp_muRF = (TH1F*)getPlot(sigSample.at(i),"elmu"+postfix_muRF+"_"+std::to_string(ind)+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"renormWeights["+std::to_string(ind)+"] * ");
          elmu_temp_muRF->Scale(1.0*sigScale/sigSample.at(i)->xsec);
          TH1F* mumu_temp_muRF = (TH1F*)getPlot(sigSample.at(i),"mumu"+postfix_muRF+"_"+std::to_string(ind)+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true,"renormWeights["+std::to_string(ind)+"] * ");
          mumu_temp_muRF->Scale(1.0*sigScale/sigSample.at(i)->xsec);
          elel__sig_muRF->Add(elel_temp_muRF);
          elmu__sig_muRF->Add(elmu_temp_muRF);
          mumu__sig_muRF->Add(mumu_temp_muRF);
        }
        hists_elel__sig_muRF.push_back(elel__sig_muRF);
        hists_elmu__sig_muRF.push_back(elmu__sig_muRF);
        hists_mumu__sig_muRF.push_back(mumu__sig_muRF);
      }
      for(int ibin=0; ibin<elel__sig->GetNbinsX()+2; ibin++){
        double elel_RFcoord[4] = {999.,0.,-999.,0.};
        double elmu_RFcoord[4] = {999.,0.,-999.,0.};
        double mumu_RFcoord[4] = {999.,0.,-999.,0.};
        double binContent = -1.;
        double binError = 0.;
        for(int ind=0; ind<6; ind++){
            binContent = hists_elel__sig_muRF.at(ind)->GetBinContent(ibin);
            binError   = hists_elel__sig_muRF.at(ind)->GetBinError(ibin);
            if(binContent<elel_RFcoord[0]) {elel_RFcoord[0] = binContent; elel_RFcoord[1] = binError;}
            if(binContent>elel_RFcoord[2]) {elel_RFcoord[2] = binContent; elel_RFcoord[3] = binError;}

            binContent = hists_elmu__sig_muRF.at(ind)->GetBinContent(ibin);
            binError   = hists_elmu__sig_muRF.at(ind)->GetBinError(ibin);
            if(binContent<elmu_RFcoord[0]) {elmu_RFcoord[0] = binContent; elmu_RFcoord[1] = binError;}
            if(binContent>elmu_RFcoord[2]) {elmu_RFcoord[2] = binContent; elmu_RFcoord[3] = binError;}

            binContent = hists_mumu__sig_muRF.at(ind)->GetBinContent(ibin);
            binError   = hists_mumu__sig_muRF.at(ind)->GetBinError(ibin);
            if(binContent<mumu_RFcoord[0]) {mumu_RFcoord[0] = binContent; mumu_RFcoord[1] = binError;}
            if(binContent>mumu_RFcoord[2]) {mumu_RFcoord[2] = binContent; mumu_RFcoord[3] = binError;}
        }
        elel__sig_muRFUp->SetBinContent(ibin,elel_RFcoord[2]); elel__sig_muRFUp->SetBinError(ibin,elel_RFcoord[3]);
        elel__sig_muRFDn->SetBinContent(ibin,elel_RFcoord[0]); elel__sig_muRFDn->SetBinError(ibin,elel_RFcoord[1]);
        elmu__sig_muRFUp->SetBinContent(ibin,elmu_RFcoord[2]); elmu__sig_muRFUp->SetBinError(ibin,elmu_RFcoord[3]);
        elmu__sig_muRFDn->SetBinContent(ibin,elmu_RFcoord[0]); elmu__sig_muRFDn->SetBinError(ibin,elmu_RFcoord[1]);
        mumu__sig_muRFUp->SetBinContent(ibin,mumu_RFcoord[2]); mumu__sig_muRFUp->SetBinError(ibin,mumu_RFcoord[3]);
        mumu__sig_muRFDn->SetBinContent(ibin,mumu_RFcoord[0]); mumu__sig_muRFDn->SetBinError(ibin,mumu_RFcoord[1]);
      }

      float scaleSFsUp = 1.0;
      float scaleSFsDn = 1.0;
      TString name = whichSignal+"M"+mstring;
      if(name=="TTM900")  {scaleSFsUp = 0.745; scaleSFsDn = 1.311;}
      if(name=="TTM1000") {scaleSFsUp = 0.744; scaleSFsDn = 1.312;}
      if(name=="TTM1100") {scaleSFsUp = 0.747; scaleSFsDn = 1.306;}
      if(name=="TTM1200") {scaleSFsUp = 0.742; scaleSFsDn = 1.315;}
      if(name=="TTM1300") {scaleSFsUp = 0.741; scaleSFsDn = 1.316;}
      if(name=="TTM1400") {scaleSFsUp = 0.738; scaleSFsDn = 1.322;}
      if(name=="TTM1500") {scaleSFsUp = 0.740; scaleSFsDn = 1.319;}
      if(name=="TTM1600") {scaleSFsUp = 0.735; scaleSFsDn = 1.329;}
      if(name=="TTM1700") {scaleSFsUp = 0.721; scaleSFsDn = 1.354;}
      if(name=="TTM1800") {scaleSFsUp = 0.746; scaleSFsDn = 1.311;}
      if(name=="BBM900")  {scaleSFsUp = 0.744; scaleSFsDn = 1.312;}
      if(name=="BBM1000") {scaleSFsUp = 0.742; scaleSFsDn = 1.315;}
      if(name=="BBM1100") {scaleSFsUp = 0.743; scaleSFsDn = 1.314;}
      if(name=="BBM1200") {scaleSFsUp = 0.742; scaleSFsDn = 1.316;}
      if(name=="BBM1300") {scaleSFsUp = 0.741; scaleSFsDn = 1.318;}
      if(name=="BBM1400") {scaleSFsUp = 0.739; scaleSFsDn = 1.321;}
      if(name=="BBM1500") {scaleSFsUp = 0.735; scaleSFsDn = 1.329;}
      if(name=="BBM1600") {scaleSFsUp = 0.735; scaleSFsDn = 1.329;}
      if(name=="BBM1700") {scaleSFsUp = 0.733; scaleSFsDn = 1.331;}
      if(name=="BBM1800") {scaleSFsUp = 0.731; scaleSFsDn = 1.337;}
      elel__sig_muRFUp->Scale(scaleSFsUp); elel__sig_muRFDn->Scale(scaleSFsDn);
      elmu__sig_muRFUp->Scale(scaleSFsUp); elmu__sig_muRFDn->Scale(scaleSFsDn);
      mumu__sig_muRFUp->Scale(scaleSFsUp); mumu__sig_muRFDn->Scale(scaleSFsDn);

      if(normalizeRENORM_PDF){
        elel__sig_muRFUp->Scale(elel__sig->Integral()/elel__sig_muRFUp->Integral());
        elel__sig_muRFDn->Scale(elel__sig->Integral()/elel__sig_muRFDn->Integral());
        elmu__sig_muRFUp->Scale(elmu__sig->Integral()/elmu__sig_muRFUp->Integral());
        elmu__sig_muRFDn->Scale(elmu__sig->Integral()/elmu__sig_muRFDn->Integral());
        mumu__sig_muRFUp->Scale(mumu__sig->Integral()/mumu__sig_muRFUp->Integral());
        mumu__sig_muRFDn->Scale(mumu__sig->Integral()/mumu__sig_muRFDn->Integral());
      }
      writeHist.push_back(elel__sig_muRFUp); writeHist.push_back(elel__sig_muRFDn);
      writeHist.push_back(elmu__sig_muRFUp); writeHist.push_back(elmu__sig_muRFDn);
      writeHist.push_back(mumu__sig_muRFUp); writeHist.push_back(mumu__sig_muRFDn);

      //////////// --> Get pdf4LHCweights hists (loop through the 30 indexes)
      TString postfix_pdf = erast+"__"+whichSignal+"M"+mstring+"__pdfSig20172018";
      TH1F* elel__sig_pdfUp = new TH1F("elel"+postfix_pdf+"Up","",nxbins,xbins);
      TH1F* elel__sig_pdfDn = new TH1F("elel"+postfix_pdf+"Down","",nxbins,xbins);
      TH1F* elmu__sig_pdfUp = new TH1F("elmu"+postfix_pdf+"Up","",nxbins,xbins);
      TH1F* elmu__sig_pdfDn = new TH1F("elmu"+postfix_pdf+"Down","",nxbins,xbins);
      TH1F* mumu__sig_pdfUp = new TH1F("mumu"+postfix_pdf+"Up","",nxbins,xbins);
      TH1F* mumu__sig_pdfDn = new TH1F("mumu"+postfix_pdf+"Down","",nxbins,xbins);

      std::vector<TH1F*> hists_elel__sig_pdf, hists_elmu__sig_pdf, hists_mumu__sig_pdf;
      for(int ind=1; ind<31; ind++){
        TH1F* elel__sig_pdf = new TH1F("elel"+postfix_pdf+"_"+std::to_string(ind),"",nxbins,xbins); elel__sig_pdf->Sumw2();
        TH1F* elmu__sig_pdf = new TH1F("elmu"+postfix_pdf+"_"+std::to_string(ind),"",nxbins,xbins); elmu__sig_pdf->Sumw2();
        TH1F* mumu__sig_pdf = new TH1F("mumu"+postfix_pdf+"_"+std::to_string(ind),"",nxbins,xbins); mumu__sig_pdf->Sumw2();
        for(std::vector<Sample*>::size_type i=0; i< sigSample.size(); i++){
          TH1F* elel_temp_pdf = (TH1F*)getPlot(sigSample.at(i),"elel"+postfix_pdf+"_"+std::to_string(ind)+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true,"(pdfWeights4LHC["+std::to_string(ind)+"]/pdfWeights4LHC[0]) * ");
          elel_temp_pdf->Scale(1.0*sigScale/sigSample.at(i)->xsec);
          TH1F* elmu_temp_pdf = (TH1F*)getPlot(sigSample.at(i),"elmu"+postfix_pdf+"_"+std::to_string(ind)+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"(pdfWeights4LHC["+std::to_string(ind)+"]/pdfWeights4LHC[0]) * ");
          elmu_temp_pdf->Scale(1.0*sigScale/sigSample.at(i)->xsec);
          TH1F* mumu_temp_pdf = (TH1F*)getPlot(sigSample.at(i),"mumu"+postfix_pdf+"_"+std::to_string(ind)+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true,"(pdfWeights4LHC["+std::to_string(ind)+"]/pdfWeights4LHC[0]) * ");
          mumu_temp_pdf->Scale(1.0*sigScale/sigSample.at(i)->xsec);
          elel__sig_pdf->Add(elel_temp_pdf);
          elmu__sig_pdf->Add(elmu_temp_pdf);
          mumu__sig_pdf->Add(mumu_temp_pdf);
        }
        hists_elel__sig_pdf.push_back(elel__sig_pdf);
        hists_elmu__sig_pdf.push_back(elmu__sig_pdf);
        hists_mumu__sig_pdf.push_back(mumu__sig_pdf);
      }

      for(int ibin=0; ibin<elel__sig->GetNbinsX()+2; ibin++){
        double elel_errsq = 0.0;
        double elmu_errsq = 0.0;
        double mumu_errsq = 0.0;
        for(unsigned int ind=0; ind<hists_elel__sig_pdf.size(); ind++){
            elel_errsq += pow((hists_elel__sig_pdf.at(ind)->GetBinContent(ibin) - elel__sig->GetBinContent(ibin)), 2.0);
            elmu_errsq += pow((hists_elmu__sig_pdf.at(ind)->GetBinContent(ibin) - elmu__sig->GetBinContent(ibin)), 2.0);
            mumu_errsq += pow((hists_mumu__sig_pdf.at(ind)->GetBinContent(ibin) - mumu__sig->GetBinContent(ibin)), 2.0);
        }
        elel__sig_pdfUp->SetBinContent(ibin,std::max( 0., elel__sig->GetBinContent(ibin)*(1+pow(elel_errsq,0.5)/elel__sig->GetBinContent(ibin)) ));
        elel__sig_pdfDn->SetBinContent(ibin,std::max( 0., elel__sig->GetBinContent(ibin)*(1-pow(elel_errsq,0.5)/elel__sig->GetBinContent(ibin)) ));
        elmu__sig_pdfUp->SetBinContent(ibin,std::max( 0., elmu__sig->GetBinContent(ibin)*(1+pow(elmu_errsq,0.5)/elmu__sig->GetBinContent(ibin)) ));
        elmu__sig_pdfDn->SetBinContent(ibin,std::max( 0., elmu__sig->GetBinContent(ibin)*(1-pow(elmu_errsq,0.5)/elmu__sig->GetBinContent(ibin)) ));
        mumu__sig_pdfUp->SetBinContent(ibin,std::max( 0., mumu__sig->GetBinContent(ibin)*(1+pow(mumu_errsq,0.5)/mumu__sig->GetBinContent(ibin)) ));
        mumu__sig_pdfDn->SetBinContent(ibin,std::max( 0., mumu__sig->GetBinContent(ibin)*(1-pow(mumu_errsq,0.5)/mumu__sig->GetBinContent(ibin)) ));
      }

      float pdfSFsUp = 1.0;
      float pdfSFsDn = 1.0;
      if(name=="TTM900")  {pdfSFsUp = 0.957; pdfSFsDn = 1.047;}
      if(name=="TTM1000") {pdfSFsUp = 0.954; pdfSFsDn = 1.050;}
      if(name=="TTM1100") {pdfSFsUp = 0.951; pdfSFsDn = 1.054;}
      if(name=="TTM1200") {pdfSFsUp = 0.947; pdfSFsDn = 1.060;}
      if(name=="TTM1300") {pdfSFsUp = 0.942; pdfSFsDn = 1.066;}
      if(name=="TTM1400") {pdfSFsUp = 0.936; pdfSFsDn = 1.073;}
      if(name=="TTM1500") {pdfSFsUp = 0.929; pdfSFsDn = 1.082;}
      if(name=="TTM1600") {pdfSFsUp = 0.921; pdfSFsDn = 1.094;}
      if(name=="TTM1700") {pdfSFsUp = 0.911; pdfSFsDn = 1.109;}
      if(name=="TTM1800") {pdfSFsUp = 0.898; pdfSFsDn = 1.128;}
      if(name=="BBM900")  {pdfSFsUp = 0.957; pdfSFsDn = 1.047;}
      if(name=="BBM1000") {pdfSFsUp = 0.954; pdfSFsDn = 1.050;}
      if(name=="BBM1100") {pdfSFsUp = 0.951; pdfSFsDn = 1.055;}
      if(name=="BBM1200") {pdfSFsUp = 0.947; pdfSFsDn = 1.059;}
      if(name=="BBM1300") {pdfSFsUp = 0.942; pdfSFsDn = 1.066;}
      if(name=="BBM1400") {pdfSFsUp = 0.936; pdfSFsDn = 1.073;}
      if(name=="BBM1500") {pdfSFsUp = 0.929; pdfSFsDn = 1.082;}
      if(name=="BBM1600") {pdfSFsUp = 0.921; pdfSFsDn = 1.094;}
      if(name=="BBM1700") {pdfSFsUp = 0.911; pdfSFsDn = 1.108;}
      if(name=="BBM1800") {pdfSFsUp = 0.897; pdfSFsDn = 1.130;}
      elel__sig_pdfUp->Scale(pdfSFsUp); elel__sig_pdfDn->Scale(pdfSFsDn);
      elmu__sig_pdfUp->Scale(pdfSFsUp); elmu__sig_pdfDn->Scale(pdfSFsDn);
      mumu__sig_pdfUp->Scale(pdfSFsUp); mumu__sig_pdfDn->Scale(pdfSFsDn);

      if(normalizeRENORM_PDF){
        elel__sig_pdfUp->Scale(elel__sig->Integral()/elel__sig_pdfUp->Integral()); 
        elel__sig_pdfDn->Scale(elel__sig->Integral()/elel__sig_pdfDn->Integral());
        elmu__sig_pdfUp->Scale(elmu__sig->Integral()/elmu__sig_pdfUp->Integral());
        elmu__sig_pdfDn->Scale(elmu__sig->Integral()/elmu__sig_pdfDn->Integral());
        mumu__sig_pdfUp->Scale(mumu__sig->Integral()/mumu__sig_pdfUp->Integral());
        mumu__sig_pdfDn->Scale(mumu__sig->Integral()/mumu__sig_pdfDn->Integral());
      }

      writeHist.push_back(elel__sig_pdfUp); writeHist.push_back(elel__sig_pdfDn);
      writeHist.push_back(elmu__sig_pdfUp); writeHist.push_back(elmu__sig_pdfDn);
      writeHist.push_back(mumu__sig_pdfUp); writeHist.push_back(mumu__sig_pdfDn);

      //////////// --> Get hist from JEC and JER trees
      std::vector<Sample*> sigSample_JECup;
      for(std::vector<Sample*>::size_type i=0; i< vSig_JECup.size(); i++){
        if( vSig_JECup.at(i)->name.find("M-"+mstring)!=std::string::npos){
          sigSample_JECup.push_back(vSig_JECup.at(i));
        }
      }

      TString postfix_JECup = erast+"__"+whichSignal+"M"+mstring+"__jec"+erast(0,4)+"Up";
      TH1F* elel__sig_JECup = new TH1F("elel"+postfix_JECup,"",nxbins,xbins); elel__sig_JECup->Sumw2();
      TH1F* elmu__sig_JECup = new TH1F("elmu"+postfix_JECup,"",nxbins,xbins); elmu__sig_JECup->Sumw2();
      TH1F* mumu__sig_JECup = new TH1F("mumu"+postfix_JECup,"",nxbins,xbins); mumu__sig_JECup->Sumw2();
      for(std::vector<Sample*>::size_type i=0; i< sigSample_JECup.size(); i++){
        if(debug_) std::cout<<sigSample_JECup.at(i)->name<<std::endl;
        TH1F* elel_temp = (TH1F*)getPlot(sigSample_JECup.at(i),"elel"+postfix_JECup+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true);
        elel_temp->Scale(1.0*sigScale/sigSample_JECup.at(i)->xsec);
        TH1F* elmu_temp = (TH1F*)getPlot(sigSample_JECup.at(i),"elmu"+postfix_JECup+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true);
        elmu_temp->Scale(1.0*sigScale/sigSample_JECup.at(i)->xsec);
        TH1F* mumu_temp = (TH1F*)getPlot(sigSample_JECup.at(i),"mumu"+postfix_JECup+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true);
        mumu_temp->Scale(1.0*sigScale/sigSample_JECup.at(i)->xsec);
        elel__sig_JECup->Add(elel_temp);
        elmu__sig_JECup->Add(elmu_temp);
        mumu__sig_JECup->Add(mumu_temp);
      }
      writeHist.push_back(elel__sig_JECup);writeHist.push_back(elmu__sig_JECup);writeHist.push_back(mumu__sig_JECup);

      TString postfix_JECdn = erast+"__"+whichSignal+"M"+mstring+"__jec"+erast(0,4)+"Down";
      TH1F* elel__sig_JECdn = new TH1F("elel"+postfix_JECdn,"",nxbins,xbins); elel__sig_JECdn->Sumw2();
      TH1F* elmu__sig_JECdn = new TH1F("elmu"+postfix_JECdn,"",nxbins,xbins); elmu__sig_JECdn->Sumw2();
      TH1F* mumu__sig_JECdn = new TH1F("mumu"+postfix_JECdn,"",nxbins,xbins); mumu__sig_JECdn->Sumw2();
      for(std::vector<Sample*>::size_type i=0; i< vSig_JECdn.size(); i++){
        if( vSig_JECdn.at(i)->name.find("M-"+mstring)==std::string::npos) continue;
        TH1F* elel_temp = (TH1F*)getPlot(vSig_JECdn.at(i),"elel"+postfix_JECdn+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true);
        elel_temp->Scale(1.0*sigScale/vSig_JECdn.at(i)->xsec);
        TH1F* elmu_temp = (TH1F*)getPlot(vSig_JECdn.at(i),"elmu"+postfix_JECdn+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true);
        elmu_temp->Scale(1.0*sigScale/vSig_JECdn.at(i)->xsec);
        TH1F* mumu_temp = (TH1F*)getPlot(vSig_JECdn.at(i),"mumu"+postfix_JECdn+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true);
        mumu_temp->Scale(1.0*sigScale/vSig_JECdn.at(i)->xsec);
        elel__sig_JECdn->Add(elel_temp);
        elmu__sig_JECdn->Add(elmu_temp);
        mumu__sig_JECdn->Add(mumu_temp);
      }
      writeHist.push_back(elel__sig_JECdn);writeHist.push_back(elmu__sig_JECdn);writeHist.push_back(mumu__sig_JECdn);

      TString postfix_JERup = erast+"__"+whichSignal+"M"+mstring+"__jer"+erast(0,4)+"Up";
      TH1F* elel__sig_JERup = new TH1F("elel"+postfix_JERup,"",nxbins,xbins); elel__sig_JERup->Sumw2();
      TH1F* elmu__sig_JERup = new TH1F("elmu"+postfix_JERup,"",nxbins,xbins); elmu__sig_JERup->Sumw2();
      TH1F* mumu__sig_JERup = new TH1F("mumu"+postfix_JERup,"",nxbins,xbins); mumu__sig_JERup->Sumw2();
      for(std::vector<Sample*>::size_type i=0; i< vSig_JERup.size(); i++){
        if( vSig_JERup.at(i)->name.find("M-"+mstring)==std::string::npos) continue;
        TH1F* elel_temp = (TH1F*)getPlot(vSig_JERup.at(i),"elel"+postfix_JERup+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true);
        elel_temp->Scale(1.0*sigScale/vSig_JERup.at(i)->xsec);
        TH1F* elmu_temp = (TH1F*)getPlot(vSig_JERup.at(i),"elmu"+postfix_JERup+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true);
        elmu_temp->Scale(1.0*sigScale/vSig_JERup.at(i)->xsec);
        TH1F* mumu_temp = (TH1F*)getPlot(vSig_JERup.at(i),"mumu"+postfix_JERup+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true);
        mumu_temp->Scale(1.0*sigScale/vSig_JERup.at(i)->xsec);
        elel__sig_JERup->Add(elel_temp);
        elmu__sig_JERup->Add(elmu_temp);
        mumu__sig_JERup->Add(mumu_temp);
      }
      writeHist.push_back(elel__sig_JERup);writeHist.push_back(elmu__sig_JERup);writeHist.push_back(mumu__sig_JERup);

      TString postfix_JERdn = erast+"__"+whichSignal+"M"+mstring+"__jer"+erast(0,4)+"Down";
      TH1F* elel__sig_JERdn = new TH1F("elel"+postfix_JERdn,"",nxbins,xbins);
      TH1F* elmu__sig_JERdn = new TH1F("elmu"+postfix_JERdn,"",nxbins,xbins);
      TH1F* mumu__sig_JERdn = new TH1F("mumu"+postfix_JERdn,"",nxbins,xbins);
      for(std::vector<Sample*>::size_type i=0; i< vSig_JERdn.size(); i++){
        if( vSig_JERdn.at(i)->name.find("M-"+mstring)==std::string::npos) continue;
        TH1F* elel_temp = (TH1F*)getPlot(vSig_JERdn.at(i),"elel"+postfix_JERdn+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true);
        elel_temp->Scale(1.0*sigScale/vSig_JERdn.at(i)->xsec);
        TH1F* elmu_temp = (TH1F*)getPlot(vSig_JERdn.at(i),"elmu"+postfix_JERdn+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true);
        elmu_temp->Scale(1.0*sigScale/vSig_JERdn.at(i)->xsec);
        TH1F* mumu_temp = (TH1F*)getPlot(vSig_JERdn.at(i),"mumu"+postfix_JERdn+"_"+std::to_string(i),"cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true);
        mumu_temp->Scale(1.0*sigScale/vSig_JERdn.at(i)->xsec);
        elel__sig_JERdn->Add(elel_temp);
        elmu__sig_JERdn->Add(elmu_temp);
        mumu__sig_JERdn->Add(mumu_temp);
      }
      writeHist.push_back(elel__sig_JERdn);writeHist.push_back(elmu__sig_JERdn);writeHist.push_back(mumu__sig_JERdn);
    }
    for(unsigned int h =0; h<writeHist.size();h++){
      overflow(writeHist.at(h));
      checkBinContent(writeHist.at(h));
      TString hname = (TString) writeHist.at(h)->GetName();
      if(hname.Contains("__Trig")){
        TRegexp re("__Trig");
        if(hname.Contains("elel")) hname(re) = "__eeTrig";
        else if(hname.Contains("elmu")) hname(re) = "__emTrig";
        else if(hname.Contains("mumu")) hname(re) = "__mmTrig";
        else std::cout<<"cannot find channel for Trig syst"<<std::endl;
      }
      writeHist.at(h)->SetName(hname);
      fout->WriteTObject(writeHist.at(h));
    }
    writeHist.clear();
  }


  std::vector<TH1F*> writeHist {};
  //////////// Bkg ////////////
  for(std::vector<Sample*>::size_type i =0; i< vBkg.size(); i++){
    if(debug_) std::cout<< vBkg.at(i)->name <<std::endl;

    //////////// --> Get nominal hist
    TString postfix = erast+"__"+vBkg.at(i)->name;
    if(vBkg.at(i)->name=="NonPrompt"){postfix = erast+"__FakeRate";}
    TH1F* elel_bkg = (TH1F*)getPlot(vBkg.at(i),"elel"+postfix,"cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true);
    TH1F* elmu_bkg = (TH1F*)getPlot(vBkg.at(i),"elmu"+postfix,"cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true);
    TH1F* mumu_bkg = (TH1F*)getPlot(vBkg.at(i),"mumu"+postfix,"cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true);

    writeHist.push_back(elel_bkg);writeHist.push_back(elmu_bkg);writeHist.push_back(mumu_bkg);

    if(doNPSys && vBkg.at(i)->name=="NonPrompt"){
      TString postfix_NP = postfix+"__NPSys2017";
      TH1F* elel_bkgNPUp = (TH1F*)elel_bkg->Clone("elel"+postfix_NP+"Up"); elel_bkgNPUp->Sumw2();
      TH1F* elmu_bkgNPUp = (TH1F*)elmu_bkg->Clone("elmu"+postfix_NP+"Up"); elmu_bkgNPUp->Sumw2();
      TH1F* mumu_bkgNPUp = (TH1F*)mumu_bkg->Clone("mumu"+postfix_NP+"Up"); mumu_bkgNPUp->Sumw2();
      TH1F* elel_bkgNPDown = (TH1F*)elel_bkg->Clone("elel"+postfix_NP+"Down"); elel_bkgNPDown->Sumw2();
      TH1F* elmu_bkgNPDown = (TH1F*)elmu_bkg->Clone("elmu"+postfix_NP+"Down"); elmu_bkgNPDown->Sumw2();
      TH1F* mumu_bkgNPDown = (TH1F*)mumu_bkg->Clone("mumu"+postfix_NP+"Down"); mumu_bkgNPDown->Sumw2();

      float eeFRunc,emFRunc,mmFRunc;
      for(int iBin = 1; iBin< elel_bkg->GetNbinsX()+1; iBin++){
        if(elel_bkg->GetXaxis()->GetBinUpEdge(iBin)<=400){eeFRunc = 0.195; emFRunc = 0.231; mmFRunc = 0.343;}
        else if(elel_bkg->GetXaxis()->GetBinUpEdge(iBin)<=600){eeFRunc = 0.140; emFRunc = 0.169; mmFRunc = 0.386;}
        else{eeFRunc = 0.460; emFRunc = 0.115; mmFRunc = 0.043;}
        elel_bkgNPUp->SetBinContent(iBin,elel_bkg->GetBinContent(iBin)*(1+eeFRunc));
        elmu_bkgNPUp->SetBinContent(iBin,elmu_bkg->GetBinContent(iBin)*(1+emFRunc));
        mumu_bkgNPUp->SetBinContent(iBin,mumu_bkg->GetBinContent(iBin)*(1+mmFRunc));
        elel_bkgNPDown->SetBinContent(iBin,elel_bkg->GetBinContent(iBin)*(1-eeFRunc));
        elmu_bkgNPDown->SetBinContent(iBin,elmu_bkg->GetBinContent(iBin)*(1-emFRunc));
        mumu_bkgNPDown->SetBinContent(iBin,mumu_bkg->GetBinContent(iBin)*(1-mmFRunc));

        elel_bkgNPUp->SetBinError(iBin,elel_bkg->GetBinError(iBin)*(1+eeFRunc)); 
        elmu_bkgNPUp->SetBinError(iBin,elmu_bkg->GetBinError(iBin)*(1+emFRunc));
        mumu_bkgNPUp->SetBinError(iBin,mumu_bkg->GetBinError(iBin)*(1+mmFRunc));
        elel_bkgNPDown->SetBinError(iBin,elel_bkg->GetBinError(iBin)*(1-eeFRunc));
        elmu_bkgNPDown->SetBinError(iBin,elmu_bkg->GetBinError(iBin)*(1-emFRunc));
        mumu_bkgNPDown->SetBinError(iBin,mumu_bkg->GetBinError(iBin)*(1-mmFRunc));
      }

      writeHist.push_back(elel_bkgNPUp);writeHist.push_back(elmu_bkgNPUp);writeHist.push_back(mumu_bkgNPUp);
      writeHist.push_back(elel_bkgNPDown);writeHist.push_back(elmu_bkgNPDown);writeHist.push_back(mumu_bkgNPDown);

      TString postfix_uElFR = postfix+"__NPWeightEl2017"; 
      if(uElFR>0){ //FRscan + uelFR
        writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elel"+postfix_uElFR+"Up","cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true,"(NPWeightElUp/NPWeight) * "));
        writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elmu"+postfix_uElFR+"Up","cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"(NPWeightElUp/NPWeight) * "));
        writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elel"+postfix_uElFR+"Down","cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true,"(NPWeight/NPWeight) * ")); 
        writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elmu"+postfix_uElFR+"Down","cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"(NPWeight/NPWeight) * ")); 
      } else{ //FRscan - uelFR
        writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elel"+postfix_uElFR+"Up","cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true,"(NPWeight/NPWeight) * "));
        writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elmu"+postfix_uElFR+"Up","cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"(NPWeight/NPWeight) * "));
        writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elel"+postfix_uElFR+"Down","cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true,"(NPWeightElDown/NPWeight) * ")); 
        writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elmu"+postfix_uElFR+"Down","cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"(NPWeightElDown/NPWeight) * ")); 
      }    
      TString postfix_uMuFR = postfix+"__NPWeightMu2017";
      if(uMuFR>0){ //FRscan + umuFR
        writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elmu"+postfix_uMuFR+"Up","cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"(NPWeightMuUp/NPWeight) * ")); 
        writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"mumu"+postfix_uMuFR+"Up","cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true,"(NPWeightMuUp/NPWeight) * ")); 
        writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elmu"+postfix_uMuFR+"Down","cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"(NPWeight/NPWeight) * "));
        writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"mumu"+postfix_uMuFR+"Down","cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true,"(NPWeight/NPWeight) * "));
      } else{ //FRscan - umuFR
        writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elmu"+postfix_uMuFR+"Up","cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"(NPWeight/NPWeight) * ")); 
        writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"mumu"+postfix_uMuFR+"Up","cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true,"(NPWeight/NPWeight) * ")); 
        writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elmu"+postfix_uMuFR+"Down","cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"(NPWeightMuDown/NPWeight) * "));
        writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"mumu"+postfix_uMuFR+"Down","cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true,"(NPWeightMuDown/NPWeight) * "));
      }

      TString postfix_uElPR = postfix+"__NPWeightElPR2017";
      writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elel"+postfix_uElPR+"Up","cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true,"(NPWeightElPRUp/NPWeight) * "));
      writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elmu"+postfix_uElPR+"Up","cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"(NPWeightElPRUp/NPWeight) * "));
      writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elel"+postfix_uElPR+"Down","cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true,"(NPWeightElPRDown/NPWeight) * "));
      writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elmu"+postfix_uElPR+"Down","cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"(NPWeightElPRDown/NPWeight) * "));
      TString postfix_uMuPR = postfix+"__NPWeightMuPR2017";
      writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elmu"+postfix_uMuPR+"Up","cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"(NPWeightMuPRUp/NPWeight) * "));
      writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"mumu"+postfix_uMuPR+"Up","cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true,"(NPWeightMuPRUp/NPWeight) * "));
      writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elmu"+postfix_uMuPR+"Down","cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"(NPWeightMuPRDown/NPWeight) * "));
      writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"mumu"+postfix_uMuPR+"Down","cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true,"(NPWeightMuPRDown/NPWeight) * "));
    }

    if(isCR && doFRScan && vBkg.at(i)->name=="NonPrompt"){
      TString postfix_FRscan = postfix+"__NPWeight2017";
      for(unsigned int ind = 0; ind < scanRange*2+1 ;ind++){
        writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"elel"+postfix_FRscan+std::to_string(ind),"cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true,"(NPWeightScan["+std::to_string(ind)+"]/NPWeight) *"));
        writeHist.push_back((TH1F*)getPlot(vBkg.at(i),"mumu"+postfix_FRscan+std::to_string(ind),"cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true,"(NPWeightScan["+std::to_string(ind)+"]/NPWeight) *"));
      }
    }

    //////////// nominal-only for data-driven background
    if(doAllSys && !(vBkg.at(i)->name=="NonPrompt" || vBkg.at(i)->name=="ChargeMisID")){
      //////////// --> Get shifted weight hist
      for(unsigned int isyst = 0; isyst < systList.size(); isyst++){
          std::string shiftWeight = systList.at(isyst);
          if (debug_) std::cout<< "Shift "<< shiftWeight<<std::endl;
          TString postfix_weight = erast+"__"+vBkg.at(i)->name+"__"+systNameList.at(isyst);
          writeHist.push_back( (TH1F*)getPlot(vBkg.at(i),"elel"+postfix_weight+"Up","cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true,"",1,shiftWeight) );
          writeHist.push_back( (TH1F*)getPlot(vBkg.at(i),"elmu"+postfix_weight+"Up","cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"",1,shiftWeight) );
          writeHist.push_back( (TH1F*)getPlot(vBkg.at(i),"mumu"+postfix_weight+"Up","cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true,"",1,shiftWeight) );
          writeHist.push_back( (TH1F*)getPlot(vBkg.at(i),"elel"+postfix_weight+"Down","cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true,"",-1,shiftWeight) );
          writeHist.push_back( (TH1F*)getPlot(vBkg.at(i),"elmu"+postfix_weight+"Down","cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"",-1,shiftWeight) );
          writeHist.push_back( (TH1F*)getPlot(vBkg.at(i),"mumu"+postfix_weight+"Down","cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true,"",-1,shiftWeight) );
      }

      //////////// --> Get muRF hist
      TString postfix_muRF = erast+"__"+vBkg.at(i)->name+"__muRFcorrdNew";
      if(vBkg.at(i)->name.find("W")==0 || vBkg.at(i)->name.find("Z")==0) postfix_muRF += "Ewk";
      else if(!(vBkg.at(i)->name.find("TT")<0)) postfix_muRF += "TTV";

      TH1F* elel_bkg_muRFUp = new TH1F("elel"+postfix_muRF+"Up","",nxbins,xbins);
      TH1F* elel_bkg_muRFDn = new TH1F("elel"+postfix_muRF+"Down","",nxbins,xbins);
      TH1F* elmu_bkg_muRFUp = new TH1F("elmu"+postfix_muRF+"Up","",nxbins,xbins);
      TH1F* elmu_bkg_muRFDn = new TH1F("elmu"+postfix_muRF+"Down","",nxbins,xbins);
      TH1F* mumu_bkg_muRFUp = new TH1F("mumu"+postfix_muRF+"Up","",nxbins,xbins);
      TH1F* mumu_bkg_muRFDn = new TH1F("mumu"+postfix_muRF+"Down","",nxbins,xbins);

      std::vector<TH1F*> hists_elel_bkg_muRF, hists_elmu_bkg_muRF, hists_mumu_bkg_muRF;
      for(int ind=0; ind<6; ind++){
        hists_elel_bkg_muRF.push_back((TH1F*)getPlot(vBkg.at(i),"elel"+postfix_muRF+"_"+std::to_string(ind),"cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true,"renormWeights["+std::to_string(ind)+"] * "));
        hists_elmu_bkg_muRF.push_back((TH1F*)getPlot(vBkg.at(i),"elmu"+postfix_muRF+"_"+std::to_string(ind),"cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"renormWeights["+std::to_string(ind)+"] * "));
        hists_mumu_bkg_muRF.push_back((TH1F*)getPlot(vBkg.at(i),"mumu"+postfix_muRF+"_"+std::to_string(ind),"cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true,"renormWeights["+std::to_string(ind)+"] * "));
      }

      for(int ibin=0; ibin<elel_bkg->GetNbinsX()+2; ibin++){
        double elel_RFcoord[4] = {999.,0.,-999.,0.};
        double elmu_RFcoord[4] = {999.,0.,-999.,0.};
        double mumu_RFcoord[4] = {999.,0.,-999.,0.};
        double binContent = -1.;
        double binError = 0.;
        for(int ind=0; ind<6; ind++){
            binContent = hists_elel_bkg_muRF.at(ind)->GetBinContent(ibin);
            binError   = hists_elel_bkg_muRF.at(ind)->GetBinError(ibin);
            if(binContent<elel_RFcoord[0]) {elel_RFcoord[0] = binContent; elel_RFcoord[1] = binError;}
            if(binContent>elel_RFcoord[2]) {elel_RFcoord[2] = binContent; elel_RFcoord[3] = binError;}

            binContent = hists_elmu_bkg_muRF.at(ind)->GetBinContent(ibin);
            binError   = hists_elmu_bkg_muRF.at(ind)->GetBinError(ibin);
            if(binContent<elmu_RFcoord[0]) {elmu_RFcoord[0] = binContent; elmu_RFcoord[1] = binError;}
            if(binContent>elmu_RFcoord[2]) {elmu_RFcoord[2] = binContent; elmu_RFcoord[3] = binError;}

            binContent = hists_mumu_bkg_muRF.at(ind)->GetBinContent(ibin);
            binError   = hists_mumu_bkg_muRF.at(ind)->GetBinError(ibin);
            if(binContent<mumu_RFcoord[0]) {mumu_RFcoord[0] = binContent; mumu_RFcoord[1] = binError;}
            if(binContent>mumu_RFcoord[2]) {mumu_RFcoord[2] = binContent; mumu_RFcoord[3] = binError;}
        }
        elel_bkg_muRFUp->SetBinContent(ibin,elel_RFcoord[2]); elel_bkg_muRFUp->SetBinError(ibin,elel_RFcoord[3]);
        elel_bkg_muRFDn->SetBinContent(ibin,elel_RFcoord[0]); elel_bkg_muRFDn->SetBinError(ibin,elel_RFcoord[1]);
        elmu_bkg_muRFUp->SetBinContent(ibin,elmu_RFcoord[2]); elmu_bkg_muRFUp->SetBinError(ibin,elmu_RFcoord[3]);
        elmu_bkg_muRFDn->SetBinContent(ibin,elmu_RFcoord[0]); elmu_bkg_muRFDn->SetBinError(ibin,elmu_RFcoord[1]);
        mumu_bkg_muRFUp->SetBinContent(ibin,mumu_RFcoord[2]); mumu_bkg_muRFUp->SetBinError(ibin,mumu_RFcoord[3]);
        mumu_bkg_muRFDn->SetBinContent(ibin,mumu_RFcoord[0]); mumu_bkg_muRFDn->SetBinError(ibin,mumu_RFcoord[1]);
      }
      writeHist.push_back(elel_bkg_muRFUp); writeHist.push_back(elel_bkg_muRFDn);
      writeHist.push_back(elmu_bkg_muRFUp); writeHist.push_back(elmu_bkg_muRFDn);
      writeHist.push_back(mumu_bkg_muRFUp); writeHist.push_back(mumu_bkg_muRFDn);

      //////////// --> Get pdf hist (100)
      TString postfix_pdf = erast+"__"+vBkg.at(i)->name+"__pdfNew20172018";

      TH1F* elel_bkg_pdfUp = new TH1F("elel"+postfix_pdf+"Up","",nxbins,xbins);
      TH1F* elel_bkg_pdfDn = new TH1F("elel"+postfix_pdf+"Down","",nxbins,xbins);
      TH1F* elmu_bkg_pdfUp = new TH1F("elmu"+postfix_pdf+"Up","",nxbins,xbins);
      TH1F* elmu_bkg_pdfDn = new TH1F("elmu"+postfix_pdf+"Down","",nxbins,xbins);
      TH1F* mumu_bkg_pdfUp = new TH1F("mumu"+postfix_pdf+"Up","",nxbins,xbins);
      TH1F* mumu_bkg_pdfDn = new TH1F("mumu"+postfix_pdf+"Down","",nxbins,xbins);

      std::vector<TH1F*> hists_elel_bkg_pdf, hists_elmu_bkg_pdf, hists_mumu_bkg_pdf;
      for(int ind=0; ind<100; ind++){
        hists_elel_bkg_pdf.push_back((TH1F*)getPlot(vBkg.at(i),"elel"+postfix_pdf+"_"+std::to_string(ind),"cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true,"pdfweights["+std::to_string(ind)+"] * "));
        hists_elmu_bkg_pdf.push_back((TH1F*)getPlot(vBkg.at(i),"elmu"+postfix_pdf+"_"+std::to_string(ind),"cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true,"pdfweights["+std::to_string(ind)+"] * "));
        hists_mumu_bkg_pdf.push_back((TH1F*)getPlot(vBkg.at(i),"mumu"+postfix_pdf+"_"+std::to_string(ind),"cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true,"pdfweights["+std::to_string(ind)+"] * "));
      }

      for(int ibin=0; ibin<elel_bkg->GetNbinsX()+2; ibin++){
        double elel_errsq = 0.0;
        double elmu_errsq = 0.0;
        double mumu_errsq = 0.0;
        for(unsigned int ind=0; ind<hists_elel_bkg_pdf.size(); ind++){
            elel_errsq += pow((hists_elel_bkg_pdf.at(ind)->GetBinContent(ibin) - elel_bkg->GetBinContent(ibin)), 2.0);
            elmu_errsq += pow((hists_elmu_bkg_pdf.at(ind)->GetBinContent(ibin) - elmu_bkg->GetBinContent(ibin)), 2.0);
            mumu_errsq += pow((hists_mumu_bkg_pdf.at(ind)->GetBinContent(ibin) - mumu_bkg->GetBinContent(ibin)), 2.0);
        }
        elel_bkg_pdfUp->SetBinContent(ibin,std::max( 0., elel_bkg->GetBinContent(ibin)*(1+pow(elel_errsq,0.5)/elel_bkg->GetBinContent(ibin)) )); 
        elel_bkg_pdfDn->SetBinContent(ibin,std::max( 0., elel_bkg->GetBinContent(ibin)*(1-pow(elel_errsq,0.5)/elel_bkg->GetBinContent(ibin)) ));
        elmu_bkg_pdfUp->SetBinContent(ibin,std::max( 0., elmu_bkg->GetBinContent(ibin)*(1+pow(elmu_errsq,0.5)/elmu_bkg->GetBinContent(ibin)) ));
        elmu_bkg_pdfDn->SetBinContent(ibin,std::max( 0., elmu_bkg->GetBinContent(ibin)*(1-pow(elmu_errsq,0.5)/elmu_bkg->GetBinContent(ibin)) ));
        mumu_bkg_pdfUp->SetBinContent(ibin,std::max( 0., mumu_bkg->GetBinContent(ibin)*(1+pow(mumu_errsq,0.5)/mumu_bkg->GetBinContent(ibin)) ));
        mumu_bkg_pdfDn->SetBinContent(ibin,std::max( 0., mumu_bkg->GetBinContent(ibin)*(1-pow(mumu_errsq,0.5)/mumu_bkg->GetBinContent(ibin)) ));
        elel_bkg_pdfUp->SetBinError(ibin, 1e-6); elel_bkg_pdfDn->SetBinError(ibin, 1e-6);
        elmu_bkg_pdfUp->SetBinError(ibin, 1e-6); elmu_bkg_pdfDn->SetBinError(ibin, 1e-6);
        mumu_bkg_pdfUp->SetBinError(ibin, 1e-6); mumu_bkg_pdfDn->SetBinError(ibin, 1e-6); 
      }
      writeHist.push_back(elel_bkg_pdfUp); writeHist.push_back(elel_bkg_pdfDn);
      writeHist.push_back(elmu_bkg_pdfUp); writeHist.push_back(elmu_bkg_pdfDn);
      writeHist.push_back(mumu_bkg_pdfUp); writeHist.push_back(mumu_bkg_pdfDn);
    }
  }
  char const * split = "__";
  std::vector<TString> histType;
  for(unsigned int h =0; h<writeHist.size();h++){
    TString hname = writeHist.at(h)->GetName();
    TString chn(hname(0,hname.First(split)));
    TString postfix(hname(hname.First(split)+strlen(split),hname.Length()));
    TString tempType;
    if(! (chn.Contains("elel") && postfix.Contains("TTW")) ) continue;
    if(postfix.First(split)<0) tempType = "";
    else tempType = (TString) postfix(postfix.First(split),postfix.Length());
    histType.push_back(tempType);
    if(tempType.Contains("TTV")) histType.push_back(tempType.ReplaceAll("TTV","Ewk"));
  }
  std::map<std::string,TH1F*> grpHist;
  //TRegexp re_cat1("ee");
  //TRegexp re_cat2("em");
  for(auto type : histType){
    for(auto it=vGrpBkg.begin(); it!=vGrpBkg.end(); ++it){
      if( (type.Contains("TTV") && (it->second)[0].find("TT")!=0) || (type.Contains("Ewk") && (it->second)[0].find("TT")!=std::string::npos) ){std::cout<< "Do not create hist for syst "<< type << " and bkg " << it->first <<std::endl; continue;}
      grpHist[(std::string)("elel"+erast+"__"+it->first+type)] = new TH1F("elel"+erast+"__"+it->first+type,"",nxbins,xbins);
      //if(type.Contains("Trig")) type(re_cat1) = "em";
      grpHist[(std::string)("elmu"+erast+"__"+it->first+type)] = new TH1F("elmu"+erast+"__"+it->first+type,"",nxbins,xbins);
      //if(type.Contains("Trig")) type(re_cat2) = "mm";
      grpHist[(std::string)("mumu"+erast+"__"+it->first+type)] = new TH1F("mumu"+erast+"__"+it->first+type,"",nxbins,xbins);
      grpHist[(std::string)("elel"+erast+"__"+it->first+type)]->Sumw2();
      grpHist[(std::string)("elmu"+erast+"__"+it->first+type)]->Sumw2();
      grpHist[(std::string)("mumu"+erast+"__"+it->first+type)]->Sumw2();
    }
  }
  for(unsigned int h =0; h<writeHist.size();h++){
    TString hname = writeHist.at(h)->GetName();
    TString chn(hname(0,hname.First(split)));
    TString postfix(hname(hname.First(split)+strlen(split),hname.Length()));
    TString bkg(postfix(0,(postfix.First(split)>=0) ? postfix.First(split):postfix.Length() ));
    TString type = "";
    if(postfix.First(split)>=0) type = (TString) postfix(postfix.First(split),postfix.Length());
    for(auto it=vGrpBkg.begin(); it!=vGrpBkg.end(); ++it){
      for(auto sample : it->second){
        if(sample==(std::string)bkg) {std::cout<< hname<<" added to "<< it->first <<std::endl; grpHist[(std::string)(chn+"__"+it->first+type)]->Add(writeHist.at(h));}
      }
    }
  }
  for(auto it=grpHist.begin(); it!=grpHist.end(); ++it){
    writeHist.push_back(it->second);
  }
  for(unsigned int h =0; h<writeHist.size();h++){
    overflow(writeHist.at(h));
    checkBinContent(writeHist.at(h));
    TString hname = (TString) writeHist.at(h)->GetName();
    if(hname.Contains("__Trig")){
      TRegexp re("__Trig");
      if(hname.Contains("elel")) hname(re) = "__eeTrig";
      else if(hname.Contains("elmu")) hname(re) = "__emTrig";
      else if(hname.Contains("mumu")) hname(re) = "__mmTrig";
      else std::cout<<"cannot find channel for Trig syst"<<std::endl;
    }
    writeHist.at(h)->SetName(hname);
    fout->WriteTObject(writeHist.at(h));
  }

  if(doAllSys){
    //////////// --> Get hists from JEC/JER trees
    std::map<std::string,TH1F*> grpHist_shift;
    for(auto it=vGrpBkg.begin(); it!=vGrpBkg.end(); ++it){
      for(TString type : {"__jec"+erast(0,4)+"Up","__jec"+erast(0,4)+"Down","__jer"+erast(0,4)+"Up","__jer"+erast(0,4)+"Down"}){
        grpHist_shift[(std::string)("elel"+erast+"__"+it->first+type)] = new TH1F("elel"+erast+"__"+it->first+type,"",nxbins,xbins);
        grpHist_shift[(std::string)("elmu"+erast+"__"+it->first+type)] = new TH1F("elmu"+erast+"__"+it->first+type,"",nxbins,xbins);
        grpHist_shift[(std::string)("mumu"+erast+"__"+it->first+type)] = new TH1F("mumu"+erast+"__"+it->first+type,"",nxbins,xbins);
        grpHist_shift[(std::string)("elel"+erast+"__"+it->first+type)]->Sumw2();
        grpHist_shift[(std::string)("elmu"+erast+"__"+it->first+type)]->Sumw2();
        grpHist_shift[(std::string)("mumu"+erast+"__"+it->first+type)]->Sumw2();
      }
    }
    for(std::vector<Sample*>::size_type i =0; i< vMCBkg_JECup.size(); i++){
      TString type = "__jec"+erast(0,4)+"Up";
      TString postfix = erast+"__"+vMCBkg_JECup.at(i)->name+type;
      TH1F* elel_bkg = (TH1F*)getPlot(vMCBkg_JECup.at(i),"elel"+postfix,"cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true);
      TH1F* elmu_bkg = (TH1F*)getPlot(vMCBkg_JECup.at(i),"elmu"+postfix,"cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true);
      TH1F* mumu_bkg = (TH1F*)getPlot(vMCBkg_JECup.at(i),"mumu"+postfix,"cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true);
      for(auto it=vGrpBkg.begin(); it!=vGrpBkg.end(); ++it){
        for(auto sample : it->second){
          if(sample==vMCBkg_JECup.at(i)->name){
            grpHist_shift[(std::string)("elel"+erast+"__"+it->first+type)]->Add(elel_bkg);
            grpHist_shift[(std::string)("elmu"+erast+"__"+it->first+type)]->Add(elmu_bkg);
            grpHist_shift[(std::string)("mumu"+erast+"__"+it->first+type)]->Add(mumu_bkg);
          }
        }
      }
      overflow(elel_bkg); overflow(elmu_bkg); overflow(mumu_bkg);
      checkBinContent(elel_bkg);checkBinContent(elmu_bkg);checkBinContent(mumu_bkg);
      fout->WriteTObject(elel_bkg);fout->WriteTObject(elmu_bkg);fout->WriteTObject(mumu_bkg);
    }
    for(std::vector<Sample*>::size_type i =0; i< vMCBkg_JECdn.size(); i++){
      TString type = "__jec"+erast(0,4)+"Down";
      TString postfix = erast+"__"+vMCBkg_JECdn.at(i)->name+type;
      TH1F* elel_bkg = (TH1F*)getPlot(vMCBkg_JECdn.at(i),"elel"+postfix,"cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true);
      TH1F* elmu_bkg = (TH1F*)getPlot(vMCBkg_JECdn.at(i),"elmu"+postfix,"cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true);
      TH1F* mumu_bkg = (TH1F*)getPlot(vMCBkg_JECdn.at(i),"mumu"+postfix,"cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true);
      for(auto it=vGrpBkg.begin(); it!=vGrpBkg.end(); ++it){
        for(auto sample : it->second){
          if(sample==vMCBkg_JECup.at(i)->name){
            grpHist_shift[(std::string)("elel"+erast+"__"+it->first+type)]->Add(elel_bkg);
            grpHist_shift[(std::string)("elmu"+erast+"__"+it->first+type)]->Add(elmu_bkg);
            grpHist_shift[(std::string)("mumu"+erast+"__"+it->first+type)]->Add(mumu_bkg);
          }
        }
      }
      overflow(elel_bkg); overflow(elmu_bkg); overflow(mumu_bkg);
      checkBinContent(elel_bkg);checkBinContent(elmu_bkg);checkBinContent(mumu_bkg);
      fout->WriteTObject(elel_bkg);fout->WriteTObject(elmu_bkg);fout->WriteTObject(mumu_bkg);
    }

    for(std::vector<Sample*>::size_type i =0; i< vMCBkg_JERup.size(); i++){
      TString type = +"__jer"+erast(0,4)+"Up";
      TString postfix = erast+"__"+vMCBkg_JERup.at(i)->name+type;
      TH1F* elel_bkg = (TH1F*)getPlot(vMCBkg_JERup.at(i),"elel"+postfix,"cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true);
      TH1F* elmu_bkg = (TH1F*)getPlot(vMCBkg_JERup.at(i),"elmu"+postfix,"cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true);
      TH1F* mumu_bkg = (TH1F*)getPlot(vMCBkg_JERup.at(i),"mumu"+postfix,"cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true);
      for(auto it=vGrpBkg.begin(); it!=vGrpBkg.end(); ++it){
        for(auto sample : it->second){
          if(sample==vMCBkg_JECup.at(i)->name){
            grpHist_shift[(std::string)("elel"+erast+"__"+it->first+type)]->Add(elel_bkg);
            grpHist_shift[(std::string)("elmu"+erast+"__"+it->first+type)]->Add(elmu_bkg);
            grpHist_shift[(std::string)("mumu"+erast+"__"+it->first+type)]->Add(mumu_bkg);
          }
        }
      }
      overflow(elel_bkg); overflow(elmu_bkg); overflow(mumu_bkg);
      checkBinContent(elel_bkg);checkBinContent(elmu_bkg);checkBinContent(mumu_bkg);
      fout->WriteTObject(elel_bkg);fout->WriteTObject(elmu_bkg);fout->WriteTObject(mumu_bkg);
    }

    for(std::vector<Sample*>::size_type i =0; i< vMCBkg_JERdn.size(); i++){
      TString type = "__jer"+erast(0,4)+"Down";
      TString postfix = erast+"__"+vMCBkg_JERdn.at(i)->name+type;
      TH1F* elel_bkg = (TH1F*)getPlot(vMCBkg_JERdn.at(i),"elel"+postfix,"cleanAK4HT",nxbins,xbins,cutSStream.str(),0,true);
      TH1F* elmu_bkg = (TH1F*)getPlot(vMCBkg_JERdn.at(i),"elmu"+postfix,"cleanAK4HT",nxbins,xbins,cutSStream.str(),1,true);
      TH1F* mumu_bkg = (TH1F*)getPlot(vMCBkg_JERdn.at(i),"mumu"+postfix,"cleanAK4HT",nxbins,xbins,cutSStream.str(),2,true);
      for(auto it=vGrpBkg.begin(); it!=vGrpBkg.end(); ++it){
        for(auto sample : it->second){
          if(sample==vMCBkg_JECup.at(i)->name){
            grpHist_shift[(std::string)("elel"+erast+"__"+it->first+type)]->Add(elel_bkg);
            grpHist_shift[(std::string)("elmu"+erast+"__"+it->first+type)]->Add(elmu_bkg);
            grpHist_shift[(std::string)("mumu"+erast+"__"+it->first+type)]->Add(mumu_bkg);
          }
        }
      }
      overflow(elel_bkg); overflow(elmu_bkg); overflow(mumu_bkg);
      checkBinContent(elel_bkg);checkBinContent(elmu_bkg);checkBinContent(mumu_bkg);
      fout->WriteTObject(elel_bkg);fout->WriteTObject(elmu_bkg);fout->WriteTObject(mumu_bkg);
    }
    for(auto it=grpHist_shift.begin(); it!=grpHist_shift.end(); ++it){
      overflow(it->second); checkBinContent(it->second); fout->WriteTObject(it->second);
    }
  }
  fout->Close();

  return 0;
}

