#include <iostream>
#include <iomanip> 
#include <vector>
#include <string>
#include "TString.h"
#include "TRegexp.h"

bool DEBUG = false;

float getEvents(TTree* t,TString cut){
  float nEvents = t->Draw("Lep1Pt",cut);
  return nEvents;
}
float getYields(TTree* t,TString cut){
  TH1F* h = new TH1F("h","h",1,0,1000000);
  t->Project("h","Lep1Pt",cut);
  float nEvents = h->GetBinContent(1); //h->Integral();
  delete h;
  return nEvents;
}


void combinedSignal(TString mass, int BRtype){

  TString workArea = "/eos/uscms/store/user/wywong/FWLJMET102X_2lep2018_wywong_082020_hadds_postFWLJMETanalyzer_IsoTrig_IDSFunc/";
  TString fname = workArea+"TprimeTprime_M-"+mass;

  TString elID = "MVA2017TightV2IsoTightRC";
  TString muID = "CBTightMiniIsoTight";
  TString era = "2018A-D";

  //--------------------open root files to combine : all the decays
  if(DEBUG)std::cout << "Opening Files ..." << std::endl;
  TFile* f_BWBW = TFile::Open(fname+"_BWBW_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* f_TZTZ = TFile::Open(fname+"_TZTZ_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* f_THTH = TFile::Open(fname+"_THTH_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* f_THBW = TFile::Open(fname+"_THBW_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* f_TZBW = TFile::Open(fname+"_TZBW_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* f_TZTH = TFile::Open(fname+"_TZTH_Mu"+muID+"_El"+elID+"_"+era+".root");
  
  //--------------------create new root file
  TString decay;
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
  std::cout << "Creating new file: " << "TprimeTprime_M-"+mass+"_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root" <<std::endl;
  TFile* fsig = new TFile("TprimeTprime_M-"+mass+"_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root","RECREATE");

  //--------------------which BR
  float BR=0.;
  float BR_bW=0.;
  float BR_tZ=0.;
  float BR_tH=0.;  
  float BR_BWBW = 0;
  float BR_TZTZ = 0;
  float BR_THTH = 0;
  float BR_THBW = 0;
  float BR_TZBW = 0;
  float BR_TZTH = 0;

  if(BRtype==0){BR_bW=0.5	;BR_tZ=0.25	;BR_tH=0.25	;} //nominal 
  if(BRtype==1){BR_bW=0.	;BR_tZ=1.0	;BR_tH=0.0	;} // 
  if(BRtype==2){BR_bW=0.	;BR_tZ=0.8	;BR_tH=0.2	;} // 
  if(BRtype==3){BR_bW=0.	;BR_tZ=0.6	;BR_tH=0.4	;} // 
  if(BRtype==4){BR_bW=0.	;BR_tZ=0.4	;BR_tH=0.6	;} // 
  if(BRtype==5){BR_bW=0.	;BR_tZ=0.2	;BR_tH=0.8	;} // 
  if(BRtype==6){BR_bW=0.	;BR_tZ=0.0	;BR_tH=1.0	;} // 
  if(BRtype==7){BR_bW=0.2	;BR_tZ=0.8	;BR_tH=0.0	;} // 
  if(BRtype==8){BR_bW=0.2	;BR_tZ=0.6	;BR_tH=0.2	;} // 
  if(BRtype==9){BR_bW=0.2	;BR_tZ=0.4	;BR_tH=0.4	;} // 
  if(BRtype==10){BR_bW=0.2	;BR_tZ=0.2	;BR_tH=0.6	;} // 
  if(BRtype==11){BR_bW=0.2	;BR_tZ=0.0	;BR_tH=0.8	;} // 
  if(BRtype==12){BR_bW=0.4	;BR_tZ=0.6	;BR_tH=0.0	;} // 
  if(BRtype==13){BR_bW=0.4	;BR_tZ=0.4	;BR_tH=0.2	;} // 
  if(BRtype==14){BR_bW=0.4	;BR_tZ=0.2	;BR_tH=0.4	;} // 
  if(BRtype==15){BR_bW=0.4	;BR_tZ=0.0	;BR_tH=0.6	;} // 
  if(BRtype==16){BR_bW=0.6	;BR_tZ=0.4	;BR_tH=0.0	;} // 
  if(BRtype==17){BR_bW=0.6	;BR_tZ=0.2	;BR_tH=0.2	;} // 
  if(BRtype==18){BR_bW=0.6	;BR_tZ=0.0	;BR_tH=0.4	;} // 
  if(BRtype==19){BR_bW=0.8	;BR_tZ=0.2	;BR_tH=0.0	;} // 
  if(BRtype==20){BR_bW=0.8	;BR_tZ=0.0	;BR_tH=0.2	;} // 
  if(BRtype==21){BR_bW=1.0	;BR_tZ=0.0	;BR_tH=0.0	;} // 
  if(BRtype==22){BR_bW=0.0	;BR_tZ=0.5	;BR_tH=0.5	;} // 
  BR_BWBW = BR_bW*BR_bW;
  BR_TZTZ = BR_tZ*BR_tZ;
  BR_THTH = BR_tH*BR_tH;
  BR_THBW = 2*BR_tH*BR_bW;
  BR_TZBW = 2*BR_tZ*BR_bW;
  BR_TZTH = 2*BR_tZ*BR_tH;
    
  std::cout << "Processing signal with BR : bW=" << BR_bW <<" tZ=" << BR_tZ <<" tH=" << BR_tH <<std::endl;


  float initBR; //MC sample initially has equal BR's for T -> bW, tZ,tH
  //TT
  float initBR_BWBW=0.333*0.333;
  float initBR_TZTZ=0.333*0.333;
  float initBR_THTH=0.333*0.333;
  float initBR_THBW=0.333*0.333*2;
  float initBR_TZBW=0.333*0.333*2;
  float initBR_TZTH=0.333*0.333*2;
  
  float Xsec = 0.;
  float nEvnts = 0.;

  float NEvts;
  //if (mass=="800"){ Xsec = 0.196; NEvts = 795000.;}
  if (mass=="900"){ Xsec = 0.0903; NEvts = 983788.10;}
  if (mass=="1000"){ Xsec = 0.0440; NEvts = 988591.585;}//802686.;}
  if (mass=="1100"){ Xsec = 0.0224; NEvts = 1006338.443;}//795116.;}
  if (mass=="1200"){ Xsec = 0.0118; NEvts = 1015599.881;}//778462.;}
  if (mass=="1300"){ Xsec = 0.00639; NEvts = 1027236.813;}//760116.;}
  if (mass=="1400"){ Xsec = 0.00354; NEvts = 1003266.430;}//710902.;}
  if (mass=="1500"){ Xsec = 0.00200; NEvts = 963700.784;}//648060.;}
  if (mass=="1600"){ Xsec = 0.001148; NEvts = 988854.229;}//622720.;}
  if (mass=="1700"){ Xsec = 0.000666; NEvts = 953395.981;}//551326.;}
  if (mass=="1800"){ Xsec = 0.000391; NEvts = 907874.112;}//467962.;}

  //float lumi = 17.68; //fb^-1  
  float lumi = 59.74; //fb^-1  

  //vector to hold weights
  float Weights_BWBW;
  float Weights_TZTZ;
  float Weights_THTH;
  float Weights_THBW;
  float Weights_TZBW;
  float Weights_TZTH;
  Weights_BWBW = lumi * 1000 * ( (Xsec*BR_BWBW) / (NEvts * initBR_BWBW) ) ; //factor of 1000 to convert lumi to pb^-1
  Weights_TZTZ = lumi * 1000 * ( (Xsec*BR_TZTZ) / (NEvts * initBR_TZTZ) ) ; //factor of 1000 to convert lumi to pb^-1
  Weights_THTH = lumi * 1000 * ( (Xsec*BR_THTH) / (NEvts * initBR_THTH) ) ; //factor of 1000 to convert lumi to pb^-1
  Weights_THBW = lumi * 1000 * ( (Xsec*BR_THBW) / (NEvts * initBR_THBW) ) ; //factor of 1000 to convert lumi to pb^-1
  Weights_TZBW = lumi * 1000 * ( (Xsec*BR_TZBW) / (NEvts * initBR_TZBW) ) ; //factor of 1000 to convert lumi to pb^-1
  Weights_TZTH = lumi * 1000 * ( (Xsec*BR_TZTH) / (NEvts * initBR_TZTH) ) ; //factor of 1000 to convert lumi to pb^-1

  //--------------------create new combined histograms
  if(DEBUG)std::cout << "create new combined histograms ..." << std::endl;

  //histogram for pdf uncertainties
  TH2F* hist_pdfHT = new TH2F("hist_pdfHT","PDF Weights",500,0,5,30,0,3000);
  TH2F* hist_pdf4LHCHT_nom = new TH2F("hist_pdf4LHCHT_nom","PDF4LHC nominal Weights",500,0,5,30,0,3000);
  TH2F* hist_pdf4LHCHT = new TH2F("hist_pdf4LHCHT","PDF4LHC Weights",500,0,5,30,0,3000);
  TH2F* pdf_hist = new TH2F("pdf_hist","PDFnew Weights",100,0,100,30,0,3000); //julie added this in Analyzer
  TH2F* pdf4LHC_hist = new TH2F("pdf4LHC_hist","PDFnew Weights",31,0,31,30,0,3000);
  //histogram for scale uncertainties - one for all and then separate ones
  TH2F* hist_scaleHT = new TH2F("hist_scaleHT","MC Scale Uncertainties Combined",500,0,5,30,0,3000);//total
  TH1F* hist_scaleHT_nom = new TH1F("hist_scaleHT_nom","MC Scale Uncertainties ID:Nominal",30,0,3000);//1002
  TH1F* hist_scaleHT_1002 = new TH1F("hist_scaleHT_1002","MC Scale Uncertainties ID:1002",30,0,3000);//1002
  TH1F* hist_scaleHT_1003 = new TH1F("hist_scaleHT_1003","MC Scale Uncertainties ID:1003",30,0,3000);//1003
  TH1F* hist_scaleHT_1004 = new TH1F("hist_scaleHT_1004","MC Scale Uncertainties ID:1004",30,0,3000);//1004
  TH1F* hist_scaleHT_1005 = new TH1F("hist_scaleHT_1005","MC Scale Uncertainties ID:1005",30,0,3000);//1005
  TH1F* hist_scaleHT_1007 = new TH1F("hist_scaleHT_1007","MC Scale Uncertainties ID:1007",30,0,3000);//1007
  TH1F* hist_scaleHT_1009 = new TH1F("hist_scaleHT_1009","MC Scale Uncertainties ID:1009",30,0,3000);//1009
  TH1F* hist_scaleHT_ssdl_nom = new TH1F("hist_scaleHT_ssdl_nom","MC Scale Uncertainties ID:Nominal",30,0,3000);//1002
  TH1F* hist_scaleHT_ssdl_1002 = new TH1F("hist_scaleHT_ssdl_1002","MC Scale Uncertainties ID:1002",30,0,3000);//1002
  TH1F* hist_scaleHT_ssdl_1003 = new TH1F("hist_scaleHT_ssdl_1003","MC Scale Uncertainties ID:1003",30,0,3000);//1003
  TH1F* hist_scaleHT_ssdl_1004 = new TH1F("hist_scaleHT_ssdl_1004","MC Scale Uncertainties ID:1004",30,0,3000);//1004
  TH1F* hist_scaleHT_ssdl_1005 = new TH1F("hist_scaleHT_ssdl_1005","MC Scale Uncertainties ID:1005",30,0,3000);//1005
  TH1F* hist_scaleHT_ssdl_1007 = new TH1F("hist_scaleHT_ssdl_1007","MC Scale Uncertainties ID:1007",30,0,3000);//1007
  TH1F* hist_scaleHT_ssdl_1009 = new TH1F("hist_scaleHT_ssdl_1009","MC Scale Uncertainties ID:1009",30,0,3000);//1009
  //jes JER
  TH1F* hist_nom = new TH1F("hist_nom","nom Uncertainties",1,0,1);
  TH1F* hist_jesup = new TH1F("hist_jesup","jes up Uncertainties",1,0,1);
  TH1F* hist_jesdown = new TH1F("hist_jesdown","jes down Uncertainties",1,0,1);
  TH1F* hist_jerup = new TH1F("hist_jerup","JER up Uncertainties",1,0,1);
  TH1F* hist_jerdown = new TH1F("hist_jerdown","JER down Uncertainties",1,0,1);
  //pileup
  TH1F* hist_punom = new TH1F("hist_punom","PU nom Uncertainties",1,0,1);
  TH1F* hist_puup = new TH1F("hist_puup","PU up Uncertainties",1,0,1);
  TH1F* hist_pudown = new TH1F("hist_pudown","PU down Uncertainties",1,0,1);
  //IDSF
  TH1F* hist_idup = new TH1F("hist_idup","IDSF up Uncertainties",1,0,1);
  TH1F* hist_iddown = new TH1F("hist_iddown","IDSF down Uncertainties",1,0,1);
  //trig
  TH1F* hist_trigup = new TH1F("hist_trigup","trigSF up Uncertainties",1,0,1);
  TH1F* hist_trigdown = new TH1F("hist_trigdown","trigSF down Uncertainties",1,0,1);
  //prefire
  TH1F* hist_prefireup = new TH1F("hist_prefireup","prefire up Uncertainties",1,0,1);
  TH1F* hist_prefiredown = new TH1F("hist_prefiredown","prefire down Uncertainties",1,0,1); 
 
  //--------------------fetch histograms/trees from individual decay root files, get event numbers maybe create a vector
  if(DEBUG)std::cout << "fetch histograms/trees from individual decay root files ..." << std::endl;
  
  //BWBW
  TH2F* hist_pdfHT_BWBW = (TH2F*) f_BWBW->Get("hist_pdfHT");
  TH2F* pdf_hist_BWBW = (TH2F*) f_BWBW->Get("pdf_hist");
  TH2F* hist_pdf4LHCHT_nom_BWBW = (TH2F*) f_BWBW->Get("hist_pdf4LHCHT_nom");
  TH2F* hist_pdf4LHCHT_BWBW = (TH2F*) f_BWBW->Get("hist_pdf4LHCHT");
  TH2F* pdf4LHC_hist_BWBW = (TH2F*) f_BWBW->Get("pdf4LHC_hist");
  TH1F* hist_scaleHT_BWBW = (TH1F*)f_BWBW->Get("hist_scaleHT");
  TH1F* hist_scaleHT_nom_BWBW = (TH1F*)f_BWBW->Get("hist_scaleHT_nom");
  TH1F* hist_scaleHT_1002_BWBW = (TH1F*)f_BWBW->Get("hist_scaleHT_1002");
  TH1F* hist_scaleHT_1003_BWBW = (TH1F*)f_BWBW->Get("hist_scaleHT_1003");
  TH1F* hist_scaleHT_1004_BWBW = (TH1F*)f_BWBW->Get("hist_scaleHT_1004");
  TH1F* hist_scaleHT_1005_BWBW = (TH1F*)f_BWBW->Get("hist_scaleHT_1005");
  TH1F* hist_scaleHT_1007_BWBW = (TH1F*)f_BWBW->Get("hist_scaleHT_1007");
  TH1F* hist_scaleHT_1009_BWBW = (TH1F*)f_BWBW->Get("hist_scaleHT_1009");
  TH1F* hist_scaleHT_ssdl_nom_BWBW = (TH1F*)f_BWBW->Get("hist_scaleHT_ssdl_nom");
  TH1F* hist_scaleHT_ssdl_1002_BWBW = (TH1F*)f_BWBW->Get("hist_scaleHT_ssdl_1002");
  TH1F* hist_scaleHT_ssdl_1003_BWBW = (TH1F*)f_BWBW->Get("hist_scaleHT_ssdl_1003");
  TH1F* hist_scaleHT_ssdl_1004_BWBW = (TH1F*)f_BWBW->Get("hist_scaleHT_ssdl_1004");
  TH1F* hist_scaleHT_ssdl_1005_BWBW = (TH1F*)f_BWBW->Get("hist_scaleHT_ssdl_1005");
  TH1F* hist_scaleHT_ssdl_1007_BWBW = (TH1F*)f_BWBW->Get("hist_scaleHT_ssdl_1007");
  TH1F* hist_scaleHT_ssdl_1009_BWBW = (TH1F*)f_BWBW->Get("hist_scaleHT_ssdl_1009");
  TTree* t_BWBW = (TTree*) f_BWBW->Get("tEvts_sZVeto");
  TTree* t_BWBW_JECup = (TTree*) f_BWBW->Get("tEvts_sZVeto_JECup");
  TTree* t_BWBW_JECdn = (TTree*) f_BWBW->Get("tEvts_sZVeto_JECdn");
  TTree* t_BWBW_JERup = (TTree*) f_BWBW->Get("tEvts_sZVeto_JERup");
  TTree* t_BWBW_JERdn = (TTree*) f_BWBW->Get("tEvts_sZVeto_JERdn");

  //TZTZ
  TH2F* hist_pdfHT_TZTZ = (TH2F*) f_TZTZ->Get("hist_pdfHT");
  TH2F* pdf_hist_TZTZ = (TH2F*) f_TZTZ->Get("pdf_hist");
  TH2F* hist_pdf4LHCHT_nom_TZTZ = (TH2F*) f_TZTZ->Get("hist_pdf4LHCHT_nom");
  TH2F* hist_pdf4LHCHT_TZTZ = (TH2F*) f_TZTZ->Get("hist_pdf4LHCHT");
  TH2F* pdf4LHC_hist_TZTZ = (TH2F*) f_TZTZ->Get("pdf4LHC_hist");
  TH1F* hist_scaleHT_TZTZ = (TH1F*)f_TZTZ->Get("hist_scaleHT");
  TH1F* hist_scaleHT_nom_TZTZ = (TH1F*)f_TZTZ->Get("hist_scaleHT_nom");
  TH1F* hist_scaleHT_1002_TZTZ = (TH1F*)f_TZTZ->Get("hist_scaleHT_1002");
  TH1F* hist_scaleHT_1003_TZTZ = (TH1F*)f_TZTZ->Get("hist_scaleHT_1003");
  TH1F* hist_scaleHT_1004_TZTZ = (TH1F*)f_TZTZ->Get("hist_scaleHT_1004");
  TH1F* hist_scaleHT_1005_TZTZ = (TH1F*)f_TZTZ->Get("hist_scaleHT_1005");
  TH1F* hist_scaleHT_1007_TZTZ = (TH1F*)f_TZTZ->Get("hist_scaleHT_1007");
  TH1F* hist_scaleHT_1009_TZTZ = (TH1F*)f_TZTZ->Get("hist_scaleHT_1009");
  TH1F* hist_scaleHT_ssdl_nom_TZTZ = (TH1F*)f_TZTZ->Get("hist_scaleHT_ssdl_nom");
  TH1F* hist_scaleHT_ssdl_1002_TZTZ = (TH1F*)f_TZTZ->Get("hist_scaleHT_ssdl_1002");
  TH1F* hist_scaleHT_ssdl_1003_TZTZ = (TH1F*)f_TZTZ->Get("hist_scaleHT_ssdl_1003");
  TH1F* hist_scaleHT_ssdl_1004_TZTZ = (TH1F*)f_TZTZ->Get("hist_scaleHT_ssdl_1004");
  TH1F* hist_scaleHT_ssdl_1005_TZTZ = (TH1F*)f_TZTZ->Get("hist_scaleHT_ssdl_1005");
  TH1F* hist_scaleHT_ssdl_1007_TZTZ = (TH1F*)f_TZTZ->Get("hist_scaleHT_ssdl_1007");
  TH1F* hist_scaleHT_ssdl_1009_TZTZ = (TH1F*)f_TZTZ->Get("hist_scaleHT_ssdl_1009");
  TTree* t_TZTZ = (TTree*) f_TZTZ->Get("tEvts_sZVeto");
  TTree* t_TZTZ_JECup = (TTree*) f_TZTZ->Get("tEvts_sZVeto_JECup");
  TTree* t_TZTZ_JECdn = (TTree*) f_TZTZ->Get("tEvts_sZVeto_JECdn");
  TTree* t_TZTZ_JERup = (TTree*) f_TZTZ->Get("tEvts_sZVeto_JERup");
  TTree* t_TZTZ_JERdn = (TTree*) f_TZTZ->Get("tEvts_sZVeto_JERdn");


  //THTH
  TH2F* hist_pdfHT_THTH = (TH2F*) f_THTH->Get("hist_pdfHT");
  TH2F* pdf_hist_THTH = (TH2F*) f_THTH->Get("pdf_hist");
  TH2F* hist_pdf4LHCHT_nom_THTH = (TH2F*) f_THTH->Get("hist_pdf4LHCHT_nom");
  TH2F* hist_pdf4LHCHT_THTH = (TH2F*) f_THTH->Get("hist_pdf4LHCHT");
  TH2F* pdf4LHC_hist_THTH = (TH2F*) f_THTH->Get("pdf4LHC_hist");
  TH1F* hist_scaleHT_THTH = (TH1F*)f_THTH->Get("hist_scaleHT");
  TH1F* hist_scaleHT_nom_THTH = (TH1F*)f_THTH->Get("hist_scaleHT_nom");
  TH1F* hist_scaleHT_1002_THTH = (TH1F*)f_THTH->Get("hist_scaleHT_1002");
  TH1F* hist_scaleHT_1003_THTH = (TH1F*)f_THTH->Get("hist_scaleHT_1003");
  TH1F* hist_scaleHT_1004_THTH = (TH1F*)f_THTH->Get("hist_scaleHT_1004");
  TH1F* hist_scaleHT_1005_THTH = (TH1F*)f_THTH->Get("hist_scaleHT_1005");
  TH1F* hist_scaleHT_1007_THTH = (TH1F*)f_THTH->Get("hist_scaleHT_1007");
  TH1F* hist_scaleHT_1009_THTH = (TH1F*)f_THTH->Get("hist_scaleHT_1009");
  TH1F* hist_scaleHT_ssdl_nom_THTH = (TH1F*)f_THTH->Get("hist_scaleHT_ssdl_nom");
  TH1F* hist_scaleHT_ssdl_1002_THTH = (TH1F*)f_THTH->Get("hist_scaleHT_ssdl_1002");
  TH1F* hist_scaleHT_ssdl_1003_THTH = (TH1F*)f_THTH->Get("hist_scaleHT_ssdl_1003");
  TH1F* hist_scaleHT_ssdl_1004_THTH = (TH1F*)f_THTH->Get("hist_scaleHT_ssdl_1004");
  TH1F* hist_scaleHT_ssdl_1005_THTH = (TH1F*)f_THTH->Get("hist_scaleHT_ssdl_1005");
  TH1F* hist_scaleHT_ssdl_1007_THTH = (TH1F*)f_THTH->Get("hist_scaleHT_ssdl_1007");
  TH1F* hist_scaleHT_ssdl_1009_THTH = (TH1F*)f_THTH->Get("hist_scaleHT_ssdl_1009");
  TTree* t_THTH = (TTree*) f_THTH->Get("tEvts_sZVeto");
  TTree* t_THTH_JECup = (TTree*) f_THTH->Get("tEvts_sZVeto_JECup");
  TTree* t_THTH_JECdn = (TTree*) f_THTH->Get("tEvts_sZVeto_JECdn");
  TTree* t_THTH_JERup = (TTree*) f_THTH->Get("tEvts_sZVeto_JERup");
  TTree* t_THTH_JERdn = (TTree*) f_THTH->Get("tEvts_sZVeto_JERdn");

  //THBW
  TH2F* hist_pdfHT_THBW = (TH2F*) f_THBW->Get("hist_pdfHT");
  TH2F* pdf_hist_THBW = (TH2F*) f_THBW->Get("pdf_hist");
  TH2F* hist_pdf4LHCHT_nom_THBW = (TH2F*) f_THBW->Get("hist_pdf4LHCHT_nom");
  TH2F* hist_pdf4LHCHT_THBW = (TH2F*) f_THBW->Get("hist_pdf4LHCHT");
  TH2F* pdf4LHC_hist_THBW = (TH2F*) f_THBW->Get("pdf4LHC_hist");
  TH1F* hist_scaleHT_THBW = (TH1F*)f_THBW->Get("hist_scaleHT");
  TH1F* hist_scaleHT_nom_THBW = (TH1F*)f_THBW->Get("hist_scaleHT_nom");
  TH1F* hist_scaleHT_1002_THBW = (TH1F*)f_THBW->Get("hist_scaleHT_1002");
  TH1F* hist_scaleHT_1003_THBW = (TH1F*)f_THBW->Get("hist_scaleHT_1003");
  TH1F* hist_scaleHT_1004_THBW = (TH1F*)f_THBW->Get("hist_scaleHT_1004");
  TH1F* hist_scaleHT_1005_THBW = (TH1F*)f_THBW->Get("hist_scaleHT_1005");
  TH1F* hist_scaleHT_1007_THBW = (TH1F*)f_THBW->Get("hist_scaleHT_1007");
  TH1F* hist_scaleHT_1009_THBW = (TH1F*)f_THBW->Get("hist_scaleHT_1009");
  TH1F* hist_scaleHT_ssdl_nom_THBW = (TH1F*)f_THBW->Get("hist_scaleHT_ssdl_nom");
  TH1F* hist_scaleHT_ssdl_1002_THBW = (TH1F*)f_THBW->Get("hist_scaleHT_ssdl_1002");
  TH1F* hist_scaleHT_ssdl_1003_THBW = (TH1F*)f_THBW->Get("hist_scaleHT_ssdl_1003");
  TH1F* hist_scaleHT_ssdl_1004_THBW = (TH1F*)f_THBW->Get("hist_scaleHT_ssdl_1004");
  TH1F* hist_scaleHT_ssdl_1005_THBW = (TH1F*)f_THBW->Get("hist_scaleHT_ssdl_1005");
  TH1F* hist_scaleHT_ssdl_1007_THBW = (TH1F*)f_THBW->Get("hist_scaleHT_ssdl_1007");
  TH1F* hist_scaleHT_ssdl_1009_THBW = (TH1F*)f_THBW->Get("hist_scaleHT_ssdl_1009");
  TTree* t_THBW = (TTree*) f_THBW->Get("tEvts_sZVeto");
  TTree* t_THBW_JECup = (TTree*) f_THBW->Get("tEvts_sZVeto_JECup");
  TTree* t_THBW_JECdn = (TTree*) f_THBW->Get("tEvts_sZVeto_JECdn");
  TTree* t_THBW_JERup = (TTree*) f_THBW->Get("tEvts_sZVeto_JERup");
  TTree* t_THBW_JERdn = (TTree*) f_THBW->Get("tEvts_sZVeto_JERdn");

  //TZBW
  TH2F* hist_pdfHT_TZBW = (TH2F*) f_TZBW->Get("hist_pdfHT");
  TH2F* pdf_hist_TZBW = (TH2F*) f_TZBW->Get("pdf_hist");
  TH2F* hist_pdf4LHCHT_nom_TZBW = (TH2F*) f_TZBW->Get("hist_pdf4LHCHT_nom");
  TH2F* hist_pdf4LHCHT_TZBW = (TH2F*) f_TZBW->Get("hist_pdf4LHCHT");
  TH2F* pdf4LHC_hist_TZBW = (TH2F*) f_TZBW->Get("pdf4LHC_hist");
  TH1F* hist_scaleHT_TZBW = (TH1F*)f_TZBW->Get("hist_scaleHT");
  TH1F* hist_scaleHT_nom_TZBW = (TH1F*)f_TZBW->Get("hist_scaleHT_nom");
  TH1F* hist_scaleHT_1002_TZBW = (TH1F*)f_TZBW->Get("hist_scaleHT_1002");
  TH1F* hist_scaleHT_1003_TZBW = (TH1F*)f_TZBW->Get("hist_scaleHT_1003");
  TH1F* hist_scaleHT_1004_TZBW = (TH1F*)f_TZBW->Get("hist_scaleHT_1004");
  TH1F* hist_scaleHT_1005_TZBW = (TH1F*)f_TZBW->Get("hist_scaleHT_1005");
  TH1F* hist_scaleHT_1007_TZBW = (TH1F*)f_TZBW->Get("hist_scaleHT_1007");
  TH1F* hist_scaleHT_1009_TZBW = (TH1F*)f_TZBW->Get("hist_scaleHT_1009");
  TH1F* hist_scaleHT_ssdl_nom_TZBW = (TH1F*)f_TZBW->Get("hist_scaleHT_ssdl_nom");
  TH1F* hist_scaleHT_ssdl_1002_TZBW = (TH1F*)f_TZBW->Get("hist_scaleHT_ssdl_1002");
  TH1F* hist_scaleHT_ssdl_1003_TZBW = (TH1F*)f_TZBW->Get("hist_scaleHT_ssdl_1003");
  TH1F* hist_scaleHT_ssdl_1004_TZBW = (TH1F*)f_TZBW->Get("hist_scaleHT_ssdl_1004");
  TH1F* hist_scaleHT_ssdl_1005_TZBW = (TH1F*)f_TZBW->Get("hist_scaleHT_ssdl_1005");
  TH1F* hist_scaleHT_ssdl_1007_TZBW = (TH1F*)f_TZBW->Get("hist_scaleHT_ssdl_1007");
  TH1F* hist_scaleHT_ssdl_1009_TZBW = (TH1F*)f_TZBW->Get("hist_scaleHT_ssdl_1009");
  TTree* t_TZBW = (TTree*) f_TZBW->Get("tEvts_sZVeto");
  TTree* t_TZBW_JECup = (TTree*) f_TZBW->Get("tEvts_sZVeto_JECup");
  TTree* t_TZBW_JECdn = (TTree*) f_TZBW->Get("tEvts_sZVeto_JECdn");
  TTree* t_TZBW_JERup = (TTree*) f_TZBW->Get("tEvts_sZVeto_JERup");
  TTree* t_TZBW_JERdn = (TTree*) f_TZBW->Get("tEvts_sZVeto_JERdn");

  //TZTH
  TH2F* hist_pdfHT_TZTH = (TH2F*) f_TZTH->Get("hist_pdfHT");
  TH2F* pdf_hist_TZTH = (TH2F*) f_TZTH->Get("pdf_hist");
  TH2F* hist_pdf4LHCHT_nom_TZTH = (TH2F*) f_TZTH->Get("hist_pdf4LHCHT_nom");
  TH2F* hist_pdf4LHCHT_TZTH = (TH2F*) f_TZTH->Get("hist_pdf4LHCHT");
  TH2F* pdf4LHC_hist_TZTH = (TH2F*) f_TZTH->Get("pdf4LHC_hist");
  TH1F* hist_scaleHT_TZTH = (TH1F*)f_TZTH->Get("hist_scaleHT");
  TH1F* hist_scaleHT_nom_TZTH = (TH1F*)f_TZTH->Get("hist_scaleHT_nom");
  TH1F* hist_scaleHT_1002_TZTH = (TH1F*)f_TZTH->Get("hist_scaleHT_1002");
  TH1F* hist_scaleHT_1003_TZTH = (TH1F*)f_TZTH->Get("hist_scaleHT_1003");
  TH1F* hist_scaleHT_1004_TZTH = (TH1F*)f_TZTH->Get("hist_scaleHT_1004");
  TH1F* hist_scaleHT_1005_TZTH = (TH1F*)f_TZTH->Get("hist_scaleHT_1005");
  TH1F* hist_scaleHT_1007_TZTH = (TH1F*)f_TZTH->Get("hist_scaleHT_1007");
  TH1F* hist_scaleHT_1009_TZTH = (TH1F*)f_TZTH->Get("hist_scaleHT_1009");
  TH1F* hist_scaleHT_ssdl_nom_TZTH = (TH1F*)f_TZTH->Get("hist_scaleHT_ssdl_nom");
  TH1F* hist_scaleHT_ssdl_1002_TZTH = (TH1F*)f_TZTH->Get("hist_scaleHT_ssdl_1002");
  TH1F* hist_scaleHT_ssdl_1003_TZTH = (TH1F*)f_TZTH->Get("hist_scaleHT_ssdl_1003");
  TH1F* hist_scaleHT_ssdl_1004_TZTH = (TH1F*)f_TZTH->Get("hist_scaleHT_ssdl_1004");
  TH1F* hist_scaleHT_ssdl_1005_TZTH = (TH1F*)f_TZTH->Get("hist_scaleHT_ssdl_1005");
  TH1F* hist_scaleHT_ssdl_1007_TZTH = (TH1F*)f_TZTH->Get("hist_scaleHT_ssdl_1007");
  TH1F* hist_scaleHT_ssdl_1009_TZTH = (TH1F*)f_TZTH->Get("hist_scaleHT_ssdl_1009");    
  TTree* t_TZTH = (TTree*) f_TZTH->Get("tEvts_sZVeto");
  TTree* t_TZTH_JECup = (TTree*) f_TZTH->Get("tEvts_sZVeto_JECup");
  TTree* t_TZTH_JECdn = (TTree*) f_TZTH->Get("tEvts_sZVeto_JECdn");
  TTree* t_TZTH_JERup = (TTree*) f_TZTH->Get("tEvts_sZVeto_JERup");
  TTree* t_TZTH_JERdn = (TTree*) f_TZTH->Get("tEvts_sZVeto_JERdn");


  //--------------------scale histograms
  if(DEBUG)std::cout << "scale histograms ..." << std::endl;

  TString nomcut = "ChargeMisIDWeight * NPWeight * IsoSF * trigSF * GsfSF * PUWeight * IDSF * MCWeight * (Lep1Pt>40 && Lep2Pt > 30 && nConst >=4 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200) * pdfWeights4LHC[0]";
 
  //for jes,jer
  //TRegexp reNconst("nConst");
  //TString jesUpcut = nomcut; jesUpcut(reNconst) = "nConstJESup";
  //TString jesDowncut = nomcut; jesDowncut(reNconst) = "nConstJESdn";
  //TString jerUpcut = nomcut; jerUpcut(reNconst) = "nConstJERup";
  //TString jerDowncut = nomcut; jerDowncut(reNconst) = "nConstJERdn";
  
  //for pu
  TRegexp rePU("PUWeight");
  TString puUpcut = nomcut; puUpcut(rePU) = "PUWeightUp";
  TString puDowncut = nomcut; puDowncut(rePU) = "PUWeightDown";

  //for IDSF
  TRegexp reIDSF("IDSF");
  TString idUpcut = nomcut; idUpcut(reIDSF) = "IDSFup";
  TString idDowncut = nomcut; idDowncut(reIDSF) = "IDSFdn";

  //for trigSF
  TRegexp retrigSF("trigSF");
  TString trigUpcut = nomcut; trigUpcut(retrigSF) = "trigSFup";
  TString trigDowncut = nomcut; trigDowncut(retrigSF) = "trigSFdn";

  //for prefire
  TRegexp reprefire("prefireWeight");
  TString prefireUpcut = nomcut; prefireUpcut(reprefire) = "prefireWeightUp";
  TString prefireDowncut = nomcut; prefireDowncut(reprefire) = "prefireWeightDown";


  //BWBW
  hist_pdfHT_BWBW->Scale(Weights_BWBW);
  pdf_hist_BWBW->Scale(Weights_BWBW);
  hist_pdf4LHCHT_nom_BWBW->Scale(Weights_BWBW);
  hist_pdf4LHCHT_BWBW->Scale(Weights_BWBW);
  pdf4LHC_hist_BWBW->Scale(Weights_BWBW);
  hist_scaleHT_BWBW->Scale(Weights_BWBW);
  hist_scaleHT_nom_BWBW->Scale(Weights_BWBW);
  hist_scaleHT_1002_BWBW->Scale(Weights_BWBW);
  hist_scaleHT_1003_BWBW->Scale(Weights_BWBW);
  hist_scaleHT_1004_BWBW->Scale(Weights_BWBW);
  hist_scaleHT_1005_BWBW->Scale(Weights_BWBW);
  hist_scaleHT_1007_BWBW->Scale(Weights_BWBW);
  hist_scaleHT_1009_BWBW->Scale(Weights_BWBW);
  hist_scaleHT_ssdl_nom_BWBW->Scale(Weights_BWBW);
  hist_scaleHT_ssdl_1002_BWBW->Scale(Weights_BWBW);
  hist_scaleHT_ssdl_1003_BWBW->Scale(Weights_BWBW);
  hist_scaleHT_ssdl_1004_BWBW->Scale(Weights_BWBW);
  hist_scaleHT_ssdl_1005_BWBW->Scale(Weights_BWBW);
  hist_scaleHT_ssdl_1007_BWBW->Scale(Weights_BWBW);
  hist_scaleHT_ssdl_1009_BWBW->Scale(Weights_BWBW);
  TString wStr_BWBW = std::to_string(Weights_BWBW);
  if(DEBUG)std::cout << "wStr_BWBW: "<< wStr_BWBW <<std::endl;
//   TString nomcut_BWBW = wStr_BWBW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";
//   TString jesUpcut_BWBW = wStr_BWBW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleUp > 1200)";
//   TString jesDowncut_BWBW = wStr_BWBW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleDown > 1200)";
//   TString jerUpcut_BWBW = wStr_BWBW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearUp > 1200)";
//   TString jerDowncut_BWBW = wStr_BWBW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearDown > 1200)";
  float nom_BWBW = getEvents(t_BWBW,nomcut)*Weights_BWBW;
  float jesup_BWBW = getEvents(t_BWBW_JECup,nomcut)*Weights_BWBW;
  float jesdown_BWBW = getEvents(t_BWBW_JECdn,nomcut)*Weights_BWBW;
  float jerup_BWBW = getEvents(t_BWBW_JERup,nomcut)*Weights_BWBW;
  float jerdown_BWBW = getEvents(t_BWBW_JERdn,nomcut)*Weights_BWBW;
  if(DEBUG)std::cout << "	nom_BWBW: "<< nom_BWBW <<std::endl;
  if(DEBUG)std::cout << "		jesup_BWBW: "<< jesup_BWBW <<std::endl;
  if(DEBUG)std::cout << "		jesdown_BWBW: "<< jesdown_BWBW <<std::endl;
  if(DEBUG)std::cout << "		jerup_BWBW: "<< jerup_BWBW <<std::endl;
  if(DEBUG)std::cout << "		jerdown_BWBW: "<< jerdown_BWBW <<std::endl;
  float punom_BWBW = getYields(t_BWBW,nomcut)*Weights_BWBW;
  float puup_BWBW = getYields(t_BWBW,puUpcut)*Weights_BWBW;
  float pudown_BWBW = getYields(t_BWBW,puDowncut)*Weights_BWBW;
  if(DEBUG)std::cout << "	punom_BWBW: "<< punom_BWBW/Weights_BWBW << " "<<punom_BWBW <<std::endl;
  if(DEBUG)std::cout << "		puup_BWBW: "<< puup_BWBW/Weights_BWBW <<" "<< puup_BWBW <<std::endl;
  if(DEBUG)std::cout << "		pudown_BWBW: "<< pudown_BWBW/Weights_BWBW<<" " <<pudown_BWBW <<std::endl;
  float idup_BWBW = getYields(t_BWBW,idUpcut)*Weights_BWBW;
  float iddown_BWBW = getYields(t_BWBW,idDowncut)*Weights_BWBW;
  if(DEBUG)std::cout << "               idup_BWBW: "<< idup_BWBW <<std::endl;
  if(DEBUG)std::cout << "               iddown_BWBW: "<< iddown_BWBW <<std::endl;
  float trigup_BWBW = getYields(t_BWBW,trigUpcut)*Weights_BWBW;
  float trigdown_BWBW = getYields(t_BWBW,trigDowncut)*Weights_BWBW;
  if(DEBUG)std::cout << "               trigup_BWBW: "<< trigup_BWBW <<std::endl;
  if(DEBUG)std::cout << "               trigdown_BWBW: "<< trigdown_BWBW <<std::endl;
  float prefireup_BWBW = getYields(t_BWBW,prefireUpcut)*Weights_BWBW;
  float prefiredown_BWBW = getYields(t_BWBW,prefireDowncut)*Weights_BWBW;
  if(DEBUG)std::cout << "               prefireup_BWBW: "<< prefireup_BWBW <<std::endl;
  if(DEBUG)std::cout << "               prefiredown_BWBW: "<< prefiredown_BWBW <<std::endl;

  //TZTZ
  hist_pdfHT_TZTZ->Scale(Weights_TZTZ);
  pdf_hist_TZTZ->Scale(Weights_TZTZ);
  hist_pdf4LHCHT_nom_TZTZ->Scale(Weights_TZTZ);
  hist_pdf4LHCHT_TZTZ->Scale(Weights_TZTZ);
  pdf4LHC_hist_TZTZ->Scale(Weights_TZTZ);
  hist_scaleHT_TZTZ->Scale(Weights_TZTZ);
  hist_scaleHT_nom_TZTZ->Scale(Weights_TZTZ);
  hist_scaleHT_1002_TZTZ->Scale(Weights_TZTZ);
  hist_scaleHT_1003_TZTZ->Scale(Weights_TZTZ);
  hist_scaleHT_1004_TZTZ->Scale(Weights_TZTZ);
  hist_scaleHT_1005_TZTZ->Scale(Weights_TZTZ);
  hist_scaleHT_1007_TZTZ->Scale(Weights_TZTZ);
  hist_scaleHT_1009_TZTZ->Scale(Weights_TZTZ);
  hist_scaleHT_ssdl_nom_TZTZ->Scale(Weights_TZTZ);
  hist_scaleHT_ssdl_1002_TZTZ->Scale(Weights_TZTZ);
  hist_scaleHT_ssdl_1003_TZTZ->Scale(Weights_TZTZ);
  hist_scaleHT_ssdl_1004_TZTZ->Scale(Weights_TZTZ);
  hist_scaleHT_ssdl_1005_TZTZ->Scale(Weights_TZTZ);
  hist_scaleHT_ssdl_1007_TZTZ->Scale(Weights_TZTZ);
  hist_scaleHT_ssdl_1009_TZTZ->Scale(Weights_TZTZ);
  TString wStr_TZTZ = std::to_string(Weights_TZTZ);
  if(DEBUG)std::cout << "wStr_TZTZ: "<< wStr_TZTZ <<std::endl;
//   TString nomcut_TZTZ = wStr_TZTZ+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";
//   TString jesUpcut_TZTZ = wStr_TZTZ+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleUp > 1200)";
//   TString jesDowncut_TZTZ = wStr_TZTZ+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleDown > 1200)";
//   TString jerUpcut_TZTZ = wStr_TZTZ+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearUp > 1200)";
//   TString jerDowncut_TZTZ = wStr_TZTZ+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearDown > 1200)";
  float nom_TZTZ = getEvents(t_TZTZ,nomcut)*Weights_TZTZ;
  float jesup_TZTZ = getEvents(t_TZTZ_JECup,nomcut)*Weights_TZTZ;
  float jesdown_TZTZ = getEvents(t_TZTZ_JECdn,nomcut)*Weights_TZTZ;
  float jerup_TZTZ = getEvents(t_TZTZ_JERup,nomcut)*Weights_TZTZ;
  float jerdown_TZTZ = getEvents(t_TZTZ_JERdn,nomcut)*Weights_TZTZ;
  if(DEBUG)std::cout << "	nom_TZTZ: "<< nom_TZTZ <<std::endl;
  if(DEBUG)std::cout << "		jesup_TZTZ: "<< jesup_TZTZ <<std::endl;
  if(DEBUG)std::cout << "		jesdown_TZTZ: "<< jesdown_TZTZ <<std::endl;
  if(DEBUG)std::cout << "		jerup_TZTZ: "<< jerup_TZTZ <<std::endl;
  if(DEBUG)std::cout << "		jerdown_TZTZ: "<< jerdown_TZTZ <<std::endl;
  float punom_TZTZ = getYields(t_TZTZ,nomcut)*Weights_TZTZ;
  float puup_TZTZ = getYields(t_TZTZ,puUpcut)*Weights_TZTZ;
  float pudown_TZTZ = getYields(t_TZTZ,puDowncut)*Weights_TZTZ;
  if(DEBUG)std::cout << "	punom_TZTZ: "<< punom_TZTZ <<std::endl;
  if(DEBUG)std::cout << "		puup_TZTZ: "<< puup_TZTZ <<std::endl;
  if(DEBUG)std::cout << "		pudown_TZTZ: "<< pudown_TZTZ <<std::endl;
  float idup_TZTZ = getYields(t_TZTZ,idUpcut)*Weights_TZTZ;
  float iddown_TZTZ = getYields(t_TZTZ,idDowncut)*Weights_TZTZ;
  if(DEBUG)std::cout << "               idup_TZTZ: "<< idup_TZTZ <<std::endl;
  if(DEBUG)std::cout << "               iddown_TZTZ: "<< iddown_TZTZ <<std::endl;
  float trigup_TZTZ = getYields(t_TZTZ,trigUpcut)*Weights_TZTZ;
  float trigdown_TZTZ = getYields(t_TZTZ,trigDowncut)*Weights_TZTZ;
  if(DEBUG)std::cout << "               trigup_TZTZ: "<< trigup_TZTZ <<std::endl;
  if(DEBUG)std::cout << "               trigdown_TZTZ: "<< trigdown_TZTZ <<std::endl;
  float prefireup_TZTZ = getYields(t_TZTZ,prefireUpcut)*Weights_TZTZ;
  float prefiredown_TZTZ = getYields(t_TZTZ,prefireDowncut)*Weights_TZTZ;
  if(DEBUG)std::cout << "               prefireup_TZTZ: "<< prefireup_TZTZ <<std::endl;
  if(DEBUG)std::cout << "               prefiredown_TZTZ: "<< prefiredown_TZTZ <<std::endl;

  //THTH
  hist_pdfHT_THTH->Scale(Weights_THTH);
  pdf_hist_THTH->Scale(Weights_THTH);
  hist_pdf4LHCHT_nom_THTH->Scale(Weights_THTH);
  hist_pdf4LHCHT_THTH->Scale(Weights_THTH);
  pdf4LHC_hist_THTH->Scale(Weights_THTH);
  hist_scaleHT_THTH->Scale(Weights_THTH);
  hist_scaleHT_nom_THTH->Scale(Weights_THTH);
  hist_scaleHT_1002_THTH->Scale(Weights_THTH);
  hist_scaleHT_1003_THTH->Scale(Weights_THTH);
  hist_scaleHT_1004_THTH->Scale(Weights_THTH);
  hist_scaleHT_1005_THTH->Scale(Weights_THTH);
  hist_scaleHT_1007_THTH->Scale(Weights_THTH);
  hist_scaleHT_1009_THTH->Scale(Weights_THTH);
  hist_scaleHT_ssdl_nom_THTH->Scale(Weights_THTH);
  hist_scaleHT_ssdl_1002_THTH->Scale(Weights_THTH);
  hist_scaleHT_ssdl_1003_THTH->Scale(Weights_THTH);
  hist_scaleHT_ssdl_1004_THTH->Scale(Weights_THTH);
  hist_scaleHT_ssdl_1005_THTH->Scale(Weights_THTH);
  hist_scaleHT_ssdl_1007_THTH->Scale(Weights_THTH);
  hist_scaleHT_ssdl_1009_THTH->Scale(Weights_THTH);
  TString wStr_THTH = std::to_string(Weights_THTH);
  if(DEBUG)std::cout << "wStr_THTH: "<< wStr_THTH <<std::endl;
//   TString nomcut_THTH = wStr_THTH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";
//   TString jesUpcut_THTH = wStr_THTH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleUp > 1200)";
//   TString jesDowncut_THTH = wStr_THTH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleDown > 1200)";
//   TString jerUpcut_THTH = wStr_THTH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearUp > 1200)";
//   TString jerDowncut_THTH = wStr_THTH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearDown > 1200)";
  float nom_THTH = getEvents(t_THTH,nomcut)*Weights_THTH;
  float jesup_THTH = getEvents(t_THTH_JECup,nomcut)*Weights_THTH;
  float jesdown_THTH = getEvents(t_THTH_JECdn,nomcut)*Weights_THTH;
  float jerup_THTH = getEvents(t_THTH_JERup,nomcut)*Weights_THTH;
  float jerdown_THTH = getEvents(t_THTH_JERdn,nomcut)*Weights_THTH;
  if(DEBUG)std::cout << "	nom_THTH: "<< nom_THTH <<std::endl;
  if(DEBUG)std::cout << "		jesup_THTH: "<< jesup_THTH <<std::endl;
  if(DEBUG)std::cout << "		jesdown_THTH: "<< jesdown_THTH <<std::endl;
  if(DEBUG)std::cout << "		jerup_THTH: "<< jerup_THTH <<std::endl;
  if(DEBUG)std::cout << "		jerdown_THTH: "<< jerdown_THTH <<std::endl;
  float punom_THTH = getYields(t_THTH,nomcut)*Weights_THTH;
  float puup_THTH = getYields(t_THTH,puUpcut)*Weights_THTH;
  float pudown_THTH = getYields(t_THTH,puDowncut)*Weights_THTH;
  if(DEBUG)std::cout << "	punom_THTH: "<< punom_THTH <<std::endl;
  if(DEBUG)std::cout << "		puup_THTH: "<< puup_THTH <<std::endl;
  if(DEBUG)std::cout << "		pudown_THTH: "<< pudown_THTH <<std::endl;
  float idup_THTH = getYields(t_THTH,idUpcut)*Weights_THTH;
  float iddown_THTH = getYields(t_THTH,idDowncut)*Weights_THTH;
  if(DEBUG)std::cout << "               idup_THTH: "<< idup_THTH <<std::endl;
  if(DEBUG)std::cout << "               iddown_THTH: "<< iddown_THTH <<std::endl;
  float trigup_THTH = getYields(t_THTH,trigUpcut)*Weights_THTH;
  float trigdown_THTH = getYields(t_THTH,trigDowncut)*Weights_THTH;
  if(DEBUG)std::cout << "               trigup_THTH: "<< trigup_THTH <<std::endl;
  if(DEBUG)std::cout << "               trigdown_THTH: "<< trigdown_THTH <<std::endl;
  float prefireup_THTH = getYields(t_THTH,prefireUpcut)*Weights_THTH;
  float prefiredown_THTH = getYields(t_THTH,prefireDowncut)*Weights_THTH;
  if(DEBUG)std::cout << "               prefireup_THTH: "<< prefireup_THTH <<std::endl;
  if(DEBUG)std::cout << "               prefiredown_THTH: "<< prefiredown_THTH <<std::endl;


  //THBW
  hist_pdfHT_THBW->Scale(Weights_THBW);
  pdf_hist_THBW->Scale(Weights_THBW);
  hist_pdf4LHCHT_nom_THBW->Scale(Weights_THBW);
  hist_pdf4LHCHT_THBW->Scale(Weights_THBW);
  pdf4LHC_hist_THBW->Scale(Weights_THBW);
  hist_scaleHT_THBW->Scale(Weights_THBW);
  hist_scaleHT_nom_THBW->Scale(Weights_THBW);
  hist_scaleHT_1002_THBW->Scale(Weights_THBW);
  hist_scaleHT_1003_THBW->Scale(Weights_THBW);
  hist_scaleHT_1004_THBW->Scale(Weights_THBW);
  hist_scaleHT_1005_THBW->Scale(Weights_THBW);
  hist_scaleHT_1007_THBW->Scale(Weights_THBW);
  hist_scaleHT_1009_THBW->Scale(Weights_THBW);
  hist_scaleHT_ssdl_nom_THBW->Scale(Weights_THBW);
  hist_scaleHT_ssdl_1002_THBW->Scale(Weights_THBW);
  hist_scaleHT_ssdl_1003_THBW->Scale(Weights_THBW);
  hist_scaleHT_ssdl_1004_THBW->Scale(Weights_THBW);
  hist_scaleHT_ssdl_1005_THBW->Scale(Weights_THBW);
  hist_scaleHT_ssdl_1007_THBW->Scale(Weights_THBW);
  hist_scaleHT_ssdl_1009_THBW->Scale(Weights_THBW);
  TString wStr_THBW = std::to_string(Weights_THBW);
  if(DEBUG)std::cout << "wStr_THBW: "<< wStr_THBW <<std::endl;
//   TString nomcut_THBW = wStr_THBW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";
//   TString jesUpcut_THBW = wStr_THBW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleUp > 1200)";
//   TString jesDowncut_THBW = wStr_THBW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleDown > 1200)";
//   TString jerUpcut_THBW = wStr_THBW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearUp > 1200)";
//   TString jerDowncut_THBW = wStr_THBW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearDown > 1200)";
  float nom_THBW = getEvents(t_THBW,nomcut)*Weights_THBW;
  float jesup_THBW = getEvents(t_THBW_JECup,nomcut)*Weights_THBW;
  float jesdown_THBW = getEvents(t_THBW_JECdn,nomcut)*Weights_THBW;
  float jerup_THBW = getEvents(t_THBW_JERup,nomcut)*Weights_THBW;
  float jerdown_THBW = getEvents(t_THBW_JERdn,nomcut)*Weights_THBW;
  if(DEBUG)std::cout << "	nom_THBW: "<< nom_THBW <<std::endl;
  if(DEBUG)std::cout << "		jesup_THBW: "<< jesup_THBW <<std::endl;
  if(DEBUG)std::cout << "		jesdown_THBW: "<< jesdown_THBW <<std::endl;
  if(DEBUG)std::cout << "		jerup_THBW: "<< jerup_THBW <<std::endl;
  if(DEBUG)std::cout << "		jerdown_THBW: "<< jerdown_THBW <<std::endl;
  float punom_THBW = getYields(t_THBW,nomcut)*Weights_THBW;
  float puup_THBW = getYields(t_THBW,puUpcut)*Weights_THBW;
  float pudown_THBW = getYields(t_THBW,puDowncut)*Weights_THBW;
  if(DEBUG)std::cout << "	punom_THBW: "<< punom_THBW <<std::endl;
  if(DEBUG)std::cout << "		puup_THBW: "<< puup_THBW <<std::endl;
  if(DEBUG)std::cout << "		pudown_THBW: "<< pudown_THBW <<std::endl;
  float idup_THBW = getYields(t_THBW,idUpcut)*Weights_THBW;
  float iddown_THBW = getYields(t_THBW,idDowncut)*Weights_THBW;
  if(DEBUG)std::cout << "               idup_THBW: "<< idup_THBW <<std::endl;
  if(DEBUG)std::cout << "               iddown_THBW: "<< iddown_THBW <<std::endl;
  float trigup_THBW = getYields(t_THBW,trigUpcut)*Weights_THBW;
  float trigdown_THBW = getYields(t_THBW,trigDowncut)*Weights_THBW;
  if(DEBUG)std::cout << "               trigup_THBW: "<< trigup_THBW <<std::endl;
  if(DEBUG)std::cout << "               trigdown_THBW: "<< trigdown_THBW <<std::endl;
  float prefireup_THBW = getYields(t_THBW,prefireUpcut)*Weights_THBW;
  float prefiredown_THBW = getYields(t_THBW,prefireDowncut)*Weights_THBW;
  if(DEBUG)std::cout << "               prefireup_THBW: "<< prefireup_THBW <<std::endl;
  if(DEBUG)std::cout << "               prefiredown_THBW: "<< prefiredown_THBW <<std::endl;

  //TZBW
  hist_pdfHT_TZBW->Scale(Weights_TZBW);
  pdf_hist_TZBW->Scale(Weights_TZBW);
  hist_pdf4LHCHT_nom_TZBW->Scale(Weights_TZBW);
  hist_pdf4LHCHT_TZBW->Scale(Weights_TZBW);
  pdf4LHC_hist_TZBW->Scale(Weights_TZBW);
  hist_scaleHT_TZBW->Scale(Weights_TZBW);
  hist_scaleHT_nom_TZBW->Scale(Weights_TZBW);
  hist_scaleHT_1002_TZBW->Scale(Weights_TZBW);
  hist_scaleHT_1003_TZBW->Scale(Weights_TZBW);
  hist_scaleHT_1004_TZBW->Scale(Weights_TZBW);
  hist_scaleHT_1005_TZBW->Scale(Weights_TZBW);
  hist_scaleHT_1007_TZBW->Scale(Weights_TZBW);
  hist_scaleHT_1009_TZBW->Scale(Weights_TZBW);
  hist_scaleHT_ssdl_nom_TZBW->Scale(Weights_TZBW);
  hist_scaleHT_ssdl_1002_TZBW->Scale(Weights_TZBW);
  hist_scaleHT_ssdl_1003_TZBW->Scale(Weights_TZBW);
  hist_scaleHT_ssdl_1004_TZBW->Scale(Weights_TZBW);
  hist_scaleHT_ssdl_1005_TZBW->Scale(Weights_TZBW);
  hist_scaleHT_ssdl_1007_TZBW->Scale(Weights_TZBW);
  hist_scaleHT_ssdl_1009_TZBW->Scale(Weights_TZBW);
  TString wStr_TZBW = std::to_string(Weights_TZBW);
  if(DEBUG)std::cout << "wStr_TZBW: "<< wStr_TZBW <<std::endl;
//   TString nomcut_TZBW = wStr_TZBW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";
//   TString jesUpcut_TZBW = wStr_TZBW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleUp > 1200)";
//   TString jesDowncut_TZBW = wStr_TZBW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleDown > 1200)";
//   TString jerUpcut_TZBW = wStr_TZBW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearUp > 1200)";
//   TString jerDowncut_TZBW = wStr_TZBW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearDown > 1200)";
  float nom_TZBW = getEvents(t_TZBW,nomcut)*Weights_TZBW;
  float jesup_TZBW = getEvents(t_TZBW_JECup,nomcut)*Weights_TZBW;
  float jesdown_TZBW = getEvents(t_TZBW_JECdn,nomcut)*Weights_TZBW;
  float jerup_TZBW = getEvents(t_TZBW_JERup,nomcut)*Weights_TZBW;
  float jerdown_TZBW = getEvents(t_TZBW_JERdn,nomcut)*Weights_TZBW;
  if(DEBUG)std::cout << "	nom_TZBW: "<< nom_TZBW <<std::endl;
  if(DEBUG)std::cout << "		jesup_TZBW: "<< jesup_TZBW <<std::endl;
  if(DEBUG)std::cout << "		jesdown_TZBW: "<< jesdown_TZBW <<std::endl;
  if(DEBUG)std::cout << "		jerup_TZBW: "<< jerup_TZBW <<std::endl;
  if(DEBUG)std::cout << "		jerdown_TZBW: "<< jerdown_TZBW <<std::endl;
  float punom_TZBW = getYields(t_TZBW,nomcut)*Weights_TZBW;
  float puup_TZBW = getYields(t_TZBW,puUpcut)*Weights_TZBW;
  float pudown_TZBW = getYields(t_TZBW,puDowncut)*Weights_TZBW;
  if(DEBUG)std::cout << "	punom_TZBW: "<< punom_TZBW <<std::endl;
  if(DEBUG)std::cout << "		puup_TZBW: "<< puup_TZBW <<std::endl;
  if(DEBUG)std::cout << "		pudown_TZBW: "<< pudown_TZBW <<std::endl;
  float idup_TZBW = getYields(t_TZBW,idUpcut)*Weights_TZBW;
  float iddown_TZBW = getYields(t_TZBW,idDowncut)*Weights_TZBW;
  if(DEBUG)std::cout << "               idup_TZBW: "<< idup_TZBW <<std::endl;
  if(DEBUG)std::cout << "               iddown_TZBW: "<< iddown_TZBW <<std::endl;
  float trigup_TZBW = getYields(t_TZBW,trigUpcut)*Weights_TZBW;
  float trigdown_TZBW = getYields(t_TZBW,trigDowncut)*Weights_TZBW;
  if(DEBUG)std::cout << "               trigup_TZBW: "<< trigup_TZBW <<std::endl;
  if(DEBUG)std::cout << "               trigdown_TZBW: "<< trigdown_TZBW <<std::endl;
  float prefireup_TZBW = getYields(t_TZBW,prefireUpcut)*Weights_TZBW;
  float prefiredown_TZBW = getYields(t_TZBW,prefireDowncut)*Weights_TZBW;
  if(DEBUG)std::cout << "               prefireup_TZBW: "<< prefireup_TZBW <<std::endl;
  if(DEBUG)std::cout << "               prefiredown_TZBW: "<< prefiredown_TZBW <<std::endl;

  //TZTH
  hist_pdfHT_TZTH->Scale(Weights_TZTH);
  pdf_hist_TZTH->Scale(Weights_TZTH);
  hist_pdf4LHCHT_nom_TZTH->Scale(Weights_TZTH);
  hist_pdf4LHCHT_TZTH->Scale(Weights_TZTH);
  pdf4LHC_hist_TZTH->Scale(Weights_TZTH);
  hist_scaleHT_TZTH->Scale(Weights_TZTH);
  hist_scaleHT_nom_TZTH->Scale(Weights_TZTH);
  hist_scaleHT_1002_TZTH->Scale(Weights_TZTH);
  hist_scaleHT_1003_TZTH->Scale(Weights_TZTH);
  hist_scaleHT_1004_TZTH->Scale(Weights_TZTH);
  hist_scaleHT_1005_TZTH->Scale(Weights_TZTH);
  hist_scaleHT_1007_TZTH->Scale(Weights_TZTH);
  hist_scaleHT_1009_TZTH->Scale(Weights_TZTH);
  hist_scaleHT_ssdl_nom_TZTH->Scale(Weights_TZTH);
  hist_scaleHT_ssdl_1002_TZTH->Scale(Weights_TZTH);
  hist_scaleHT_ssdl_1003_TZTH->Scale(Weights_TZTH);
  hist_scaleHT_ssdl_1004_TZTH->Scale(Weights_TZTH);
  hist_scaleHT_ssdl_1005_TZTH->Scale(Weights_TZTH);
  hist_scaleHT_ssdl_1007_TZTH->Scale(Weights_TZTH);
  hist_scaleHT_ssdl_1009_TZTH->Scale(Weights_TZTH);
  TString wStr_TZTH = std::to_string(Weights_TZTH);
  if(DEBUG)std::cout << "wStr_TZTH: "<< wStr_TZTH <<std::endl;
//   TString nomcut_TZTH = wStr_TZTH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";
//   TString jesUpcut_TZTH = wStr_TZTH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleUp > 1200)";
//   TString jesDowncut_TZTH = wStr_TZTH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleDown > 1200)";
//   TString jerUpcut_TZTH = wStr_TZTH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearUp > 1200)";
//   TString jerDowncut_TZTH = wStr_TZTH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearDown > 1200)";
  float nom_TZTH = getEvents(t_TZTH,nomcut)*Weights_TZTH;
  float jesup_TZTH = getEvents(t_TZTH_JECup,nomcut)*Weights_TZTH;
  float jesdown_TZTH = getEvents(t_TZTH_JECdn,nomcut)*Weights_TZTH;
  float jerup_TZTH = getEvents(t_TZTH_JERup,nomcut)*Weights_TZTH;
  float jerdown_TZTH = getEvents(t_TZTH_JERdn,nomcut)*Weights_TZTH;
  if(DEBUG)std::cout << "	nom_TZTH: "<< nom_TZTH <<std::endl;
  if(DEBUG)std::cout << "		jesup_TZTH: "<< jesup_TZTH <<std::endl;
  if(DEBUG)std::cout << "		jesdown_TZTH: "<< jesdown_TZTH <<std::endl;
  if(DEBUG)std::cout << "		jerup_TZTH: "<< jerup_TZTH <<std::endl;
  if(DEBUG)std::cout << "		jerdown_TZTH: "<< jerdown_TZTH <<std::endl;
  float punom_TZTH = getYields(t_TZTH,nomcut)*Weights_TZTH;
  float puup_TZTH = getYields(t_TZTH,puUpcut)*Weights_TZTH;
  float pudown_TZTH = getYields(t_TZTH,puDowncut)*Weights_TZTH;
  if(DEBUG)std::cout << "	punom_TZTH: "<< punom_TZTH <<std::endl;
  if(DEBUG)std::cout << "		puup_TZTH: "<< puup_TZTH <<std::endl;
  if(DEBUG)std::cout << "		pudown_TZTH: "<< pudown_TZTH <<std::endl;
  float idup_TZTH = getYields(t_TZTH,idUpcut)*Weights_TZTH;
  float iddown_TZTH = getYields(t_TZTH,idDowncut)*Weights_TZTH;
  if(DEBUG)std::cout << "               idup_TZTH: "<< idup_TZTH <<std::endl;
  if(DEBUG)std::cout << "               iddown_TZTH: "<< iddown_TZTH <<std::endl;
  float trigup_TZTH = getYields(t_TZTH,trigUpcut)*Weights_TZTH;
  float trigdown_TZTH = getYields(t_TZTH,trigDowncut)*Weights_TZTH;
  if(DEBUG)std::cout << "               trigup_TZTH: "<< trigup_TZTH <<std::endl;
  if(DEBUG)std::cout << "               trigdown_TZTH: "<< trigdown_TZTH <<std::endl;
  float prefireup_TZTH = getYields(t_TZTH,prefireUpcut)*Weights_TZTH;
  float prefiredown_TZTH = getYields(t_TZTH,prefireDowncut)*Weights_TZTH;
  if(DEBUG)std::cout << "               prefireup_TZTH: "<< prefireup_TZTH <<std::endl;
  if(DEBUG)std::cout << "               prefiredown_TZTH: "<< prefiredown_TZTH <<std::endl;

  //--------------------combine and save
  if(DEBUG)std::cout << "combine and save ..." << std::endl;

  //BWBW
  hist_pdfHT->Add(hist_pdfHT_BWBW);
  pdf_hist->Add(pdf_hist_BWBW);
  hist_pdf4LHCHT_nom->Add(hist_pdf4LHCHT_nom_BWBW);
  hist_pdf4LHCHT->Add(hist_pdf4LHCHT_BWBW);
  pdf4LHC_hist->Add(pdf4LHC_hist_BWBW);
  hist_scaleHT->Add(hist_scaleHT_BWBW);
  hist_scaleHT_nom->Add(hist_scaleHT_nom_BWBW);
  hist_scaleHT_1002->Add(hist_scaleHT_1002_BWBW);
  hist_scaleHT_1003->Add(hist_scaleHT_1003_BWBW);
  hist_scaleHT_1004->Add(hist_scaleHT_1004_BWBW);
  hist_scaleHT_1005->Add(hist_scaleHT_1005_BWBW);
  hist_scaleHT_1007->Add(hist_scaleHT_1007_BWBW);
  hist_scaleHT_1009->Add(hist_scaleHT_1009_BWBW);
  hist_scaleHT_ssdl_nom->Add(hist_scaleHT_ssdl_nom_BWBW);
  hist_scaleHT_ssdl_1002->Add(hist_scaleHT_ssdl_1002_BWBW);
  hist_scaleHT_ssdl_1003->Add(hist_scaleHT_ssdl_1003_BWBW);
  hist_scaleHT_ssdl_1004->Add(hist_scaleHT_ssdl_1004_BWBW);
  hist_scaleHT_ssdl_1005->Add(hist_scaleHT_ssdl_1005_BWBW);
  hist_scaleHT_ssdl_1007->Add(hist_scaleHT_ssdl_1007_BWBW);
  hist_scaleHT_ssdl_1009->Add(hist_scaleHT_ssdl_1009_BWBW);
  hist_nom->SetBinContent(1,hist_nom->GetBinContent(1)+nom_BWBW);
  hist_jesup->SetBinContent(1,hist_jesup->GetBinContent(1)+jesup_BWBW);
  hist_jesdown->SetBinContent(1,hist_jesdown->GetBinContent(1)+jesdown_BWBW);
  hist_jerup->SetBinContent(1,hist_jerup->GetBinContent(1)+jerup_BWBW);
  hist_jerdown->SetBinContent(1,hist_jerdown->GetBinContent(1)+jerdown_BWBW);
  hist_punom->SetBinContent(1,hist_punom->GetBinContent(1)+punom_BWBW);
  hist_puup->SetBinContent(1,hist_puup->GetBinContent(1)+puup_BWBW);
  hist_pudown->SetBinContent(1,hist_pudown->GetBinContent(1)+pudown_BWBW);
  hist_idup->SetBinContent(1,hist_idup->GetBinContent(1)+idup_BWBW);
  hist_iddown->SetBinContent(1,hist_iddown->GetBinContent(1)+iddown_BWBW);
  hist_trigup->SetBinContent(1,hist_trigup->GetBinContent(1)+trigup_BWBW);
  hist_trigdown->SetBinContent(1,hist_trigdown->GetBinContent(1)+trigdown_BWBW);
  hist_prefireup->SetBinContent(1,hist_prefireup->GetBinContent(1)+prefireup_BWBW);
  hist_prefiredown->SetBinContent(1,hist_prefiredown->GetBinContent(1)+prefiredown_BWBW);

  //TZTZ
  hist_pdfHT->Add(hist_pdfHT_TZTZ);
  pdf_hist->Add(pdf_hist_TZTZ);
  hist_pdf4LHCHT_nom->Add(hist_pdf4LHCHT_nom_TZTZ);
  hist_pdf4LHCHT->Add(hist_pdf4LHCHT_TZTZ);
  pdf4LHC_hist->Add(pdf4LHC_hist_TZTZ);
  hist_scaleHT->Add(hist_scaleHT_TZTZ);
  hist_scaleHT_nom->Add(hist_scaleHT_nom_TZTZ);
  hist_scaleHT_1002->Add(hist_scaleHT_1002_TZTZ);
  hist_scaleHT_1003->Add(hist_scaleHT_1003_TZTZ);
  hist_scaleHT_1004->Add(hist_scaleHT_1004_TZTZ);
  hist_scaleHT_1005->Add(hist_scaleHT_1005_TZTZ);
  hist_scaleHT_1007->Add(hist_scaleHT_1007_TZTZ);
  hist_scaleHT_1009->Add(hist_scaleHT_1009_TZTZ);
  hist_scaleHT_ssdl_nom->Add(hist_scaleHT_ssdl_nom_TZTZ);
  hist_scaleHT_ssdl_1002->Add(hist_scaleHT_ssdl_1002_TZTZ);
  hist_scaleHT_ssdl_1003->Add(hist_scaleHT_ssdl_1003_TZTZ);
  hist_scaleHT_ssdl_1004->Add(hist_scaleHT_ssdl_1004_TZTZ);
  hist_scaleHT_ssdl_1005->Add(hist_scaleHT_ssdl_1005_TZTZ);
  hist_scaleHT_ssdl_1007->Add(hist_scaleHT_ssdl_1007_TZTZ);
  hist_scaleHT_ssdl_1009->Add(hist_scaleHT_ssdl_1009_TZTZ);
  hist_nom->SetBinContent(1,hist_nom->GetBinContent(1)+nom_TZTZ);
  hist_jesup->SetBinContent(1,hist_jesup->GetBinContent(1)+jesup_TZTZ);
  hist_jesdown->SetBinContent(1,hist_jesdown->GetBinContent(1)+jesdown_TZTZ);
  hist_jerup->SetBinContent(1,hist_jerup->GetBinContent(1)+jerup_TZTZ);
  hist_jerdown->SetBinContent(1,hist_jerdown->GetBinContent(1)+jerdown_TZTZ);
  hist_punom->SetBinContent(1,hist_punom->GetBinContent(1)+punom_TZTZ);
  hist_puup->SetBinContent(1,hist_puup->GetBinContent(1)+puup_TZTZ);
  hist_pudown->SetBinContent(1,hist_pudown->GetBinContent(1)+pudown_TZTZ);
  hist_idup->SetBinContent(1,hist_idup->GetBinContent(1)+idup_TZTZ);
  hist_iddown->SetBinContent(1,hist_iddown->GetBinContent(1)+iddown_TZTZ);
  hist_trigup->SetBinContent(1,hist_trigup->GetBinContent(1)+trigup_TZTZ);
  hist_trigdown->SetBinContent(1,hist_trigdown->GetBinContent(1)+trigdown_TZTZ);
  hist_prefireup->SetBinContent(1,hist_prefireup->GetBinContent(1)+prefireup_TZTZ);
  hist_prefiredown->SetBinContent(1,hist_prefiredown->GetBinContent(1)+prefiredown_TZTZ);

  //THTH
  hist_pdfHT->Add(hist_pdfHT_THTH);
  pdf_hist->Add(pdf_hist_THTH);
  hist_pdf4LHCHT_nom->Add(hist_pdf4LHCHT_nom_THTH);
  hist_pdf4LHCHT->Add(hist_pdf4LHCHT_THTH);
  pdf4LHC_hist->Add(pdf4LHC_hist_THTH);
  hist_scaleHT->Add(hist_scaleHT_THTH);
  hist_scaleHT_nom->Add(hist_scaleHT_nom_THTH);
  hist_scaleHT_1002->Add(hist_scaleHT_1002_THTH);
  hist_scaleHT_1003->Add(hist_scaleHT_1003_THTH);
  hist_scaleHT_1004->Add(hist_scaleHT_1004_THTH);
  hist_scaleHT_1005->Add(hist_scaleHT_1005_THTH);
  hist_scaleHT_1007->Add(hist_scaleHT_1007_THTH);
  hist_scaleHT_1009->Add(hist_scaleHT_1009_THTH);
  hist_scaleHT_ssdl_nom->Add(hist_scaleHT_ssdl_nom_THTH);
  hist_scaleHT_ssdl_1002->Add(hist_scaleHT_ssdl_1002_THTH);
  hist_scaleHT_ssdl_1003->Add(hist_scaleHT_ssdl_1003_THTH);
  hist_scaleHT_ssdl_1004->Add(hist_scaleHT_ssdl_1004_THTH);
  hist_scaleHT_ssdl_1005->Add(hist_scaleHT_ssdl_1005_THTH);
  hist_scaleHT_ssdl_1007->Add(hist_scaleHT_ssdl_1007_THTH);
  hist_scaleHT_ssdl_1009->Add(hist_scaleHT_ssdl_1009_THTH);
  hist_nom->SetBinContent(1,hist_nom->GetBinContent(1)+nom_THTH);
  hist_jesup->SetBinContent(1,hist_jesup->GetBinContent(1)+jesup_THTH);
  hist_jesdown->SetBinContent(1,hist_jesdown->GetBinContent(1)+jesdown_THTH);
  hist_jerup->SetBinContent(1,hist_jerup->GetBinContent(1)+jerup_THTH);
  hist_jerdown->SetBinContent(1,hist_jerdown->GetBinContent(1)+jerdown_THTH);
  hist_punom->SetBinContent(1,hist_punom->GetBinContent(1)+punom_THTH);
  hist_puup->SetBinContent(1,hist_puup->GetBinContent(1)+puup_THTH);
  hist_pudown->SetBinContent(1,hist_pudown->GetBinContent(1)+pudown_THTH);
  hist_idup->SetBinContent(1,hist_idup->GetBinContent(1)+idup_THTH);
  hist_iddown->SetBinContent(1,hist_iddown->GetBinContent(1)+iddown_THTH);
  hist_trigup->SetBinContent(1,hist_trigup->GetBinContent(1)+trigup_THTH);
  hist_trigdown->SetBinContent(1,hist_trigdown->GetBinContent(1)+trigdown_THTH);
  hist_prefireup->SetBinContent(1,hist_prefireup->GetBinContent(1)+prefireup_THTH);
  hist_prefiredown->SetBinContent(1,hist_prefiredown->GetBinContent(1)+prefiredown_THTH);

  //THBW
  hist_pdfHT->Add(hist_pdfHT_THBW);
  pdf_hist->Add(pdf_hist_THBW);
  hist_pdf4LHCHT_nom->Add(hist_pdf4LHCHT_nom_THBW);
  hist_pdf4LHCHT->Add(hist_pdf4LHCHT_THBW);
  pdf4LHC_hist->Add(pdf4LHC_hist_THBW);
  hist_scaleHT->Add(hist_scaleHT_THBW);
  hist_scaleHT_nom->Add(hist_scaleHT_nom_THBW);
  hist_scaleHT_1002->Add(hist_scaleHT_1002_THBW);
  hist_scaleHT_1003->Add(hist_scaleHT_1003_THBW);
  hist_scaleHT_1004->Add(hist_scaleHT_1004_THBW);
  hist_scaleHT_1005->Add(hist_scaleHT_1005_THBW);
  hist_scaleHT_1007->Add(hist_scaleHT_1007_THBW);
  hist_scaleHT_1009->Add(hist_scaleHT_1009_THBW);
  hist_scaleHT_ssdl_nom->Add(hist_scaleHT_ssdl_nom_THBW);
  hist_scaleHT_ssdl_1002->Add(hist_scaleHT_ssdl_1002_THBW);
  hist_scaleHT_ssdl_1003->Add(hist_scaleHT_ssdl_1003_THBW);
  hist_scaleHT_ssdl_1004->Add(hist_scaleHT_ssdl_1004_THBW);
  hist_scaleHT_ssdl_1005->Add(hist_scaleHT_ssdl_1005_THBW);
  hist_scaleHT_ssdl_1007->Add(hist_scaleHT_ssdl_1007_THBW);
  hist_scaleHT_ssdl_1009->Add(hist_scaleHT_ssdl_1009_THBW);
  hist_nom->SetBinContent(1,hist_nom->GetBinContent(1)+nom_THBW);
  hist_jesup->SetBinContent(1,hist_jesup->GetBinContent(1)+jesup_THBW);
  hist_jesdown->SetBinContent(1,hist_jesdown->GetBinContent(1)+jesdown_THBW);
  hist_jerup->SetBinContent(1,hist_jerup->GetBinContent(1)+jerup_THBW);
  hist_jerdown->SetBinContent(1,hist_jerdown->GetBinContent(1)+jerdown_THBW);
  hist_punom->SetBinContent(1,hist_punom->GetBinContent(1)+punom_THBW);
  hist_puup->SetBinContent(1,hist_puup->GetBinContent(1)+puup_THBW);
  hist_pudown->SetBinContent(1,hist_pudown->GetBinContent(1)+pudown_THBW);
  hist_idup->SetBinContent(1,hist_idup->GetBinContent(1)+idup_THBW);
  hist_iddown->SetBinContent(1,hist_iddown->GetBinContent(1)+iddown_THBW);
  hist_trigup->SetBinContent(1,hist_trigup->GetBinContent(1)+trigup_THBW);
  hist_trigdown->SetBinContent(1,hist_trigdown->GetBinContent(1)+trigdown_THBW);
  hist_prefireup->SetBinContent(1,hist_prefireup->GetBinContent(1)+prefireup_THBW);
  hist_prefiredown->SetBinContent(1,hist_prefiredown->GetBinContent(1)+prefiredown_THBW);

  //TZBW
  hist_pdfHT->Add(hist_pdfHT_TZBW);
  pdf_hist->Add(pdf_hist_TZBW);
  hist_pdf4LHCHT_nom->Add(hist_pdf4LHCHT_nom_TZBW);
  hist_pdf4LHCHT->Add(hist_pdf4LHCHT_TZBW);
  pdf4LHC_hist->Add(pdf4LHC_hist_TZBW);
  hist_scaleHT->Add(hist_scaleHT_TZBW);
  hist_scaleHT_nom->Add(hist_scaleHT_nom_TZBW);
  hist_scaleHT_1002->Add(hist_scaleHT_1002_TZBW);
  hist_scaleHT_1003->Add(hist_scaleHT_1003_TZBW);
  hist_scaleHT_1004->Add(hist_scaleHT_1004_TZBW);
  hist_scaleHT_1005->Add(hist_scaleHT_1005_TZBW);
  hist_scaleHT_1007->Add(hist_scaleHT_1007_TZBW);
  hist_scaleHT_1009->Add(hist_scaleHT_1009_TZBW);
  hist_scaleHT_ssdl_nom->Add(hist_scaleHT_ssdl_nom_TZBW);
  hist_scaleHT_ssdl_1002->Add(hist_scaleHT_ssdl_1002_TZBW);
  hist_scaleHT_ssdl_1003->Add(hist_scaleHT_ssdl_1003_TZBW);
  hist_scaleHT_ssdl_1004->Add(hist_scaleHT_ssdl_1004_TZBW);
  hist_scaleHT_ssdl_1005->Add(hist_scaleHT_ssdl_1005_TZBW);
  hist_scaleHT_ssdl_1007->Add(hist_scaleHT_ssdl_1007_TZBW);
  hist_scaleHT_ssdl_1009->Add(hist_scaleHT_ssdl_1009_TZBW);
  hist_nom->SetBinContent(1,hist_nom->GetBinContent(1)+nom_TZBW);
  hist_jesup->SetBinContent(1,hist_jesup->GetBinContent(1)+jesup_TZBW);
  hist_jesdown->SetBinContent(1,hist_jesdown->GetBinContent(1)+jesdown_TZBW);
  hist_jerup->SetBinContent(1,hist_jerup->GetBinContent(1)+jerup_TZBW);
  hist_jerdown->SetBinContent(1,hist_jerdown->GetBinContent(1)+jerdown_TZBW);
  hist_punom->SetBinContent(1,hist_punom->GetBinContent(1)+punom_TZBW);
  hist_puup->SetBinContent(1,hist_puup->GetBinContent(1)+puup_TZBW);
  hist_pudown->SetBinContent(1,hist_pudown->GetBinContent(1)+pudown_TZBW);
  hist_idup->SetBinContent(1,hist_idup->GetBinContent(1)+idup_TZBW);
  hist_iddown->SetBinContent(1,hist_iddown->GetBinContent(1)+iddown_TZBW);
  hist_trigup->SetBinContent(1,hist_trigup->GetBinContent(1)+trigup_TZBW);
  hist_trigdown->SetBinContent(1,hist_trigdown->GetBinContent(1)+trigdown_TZBW);
  hist_prefireup->SetBinContent(1,hist_prefireup->GetBinContent(1)+prefireup_TZBW);
  hist_prefiredown->SetBinContent(1,hist_prefiredown->GetBinContent(1)+prefiredown_TZBW);

  //TZTH
  hist_pdfHT->Add(hist_pdfHT_TZTH);
  pdf_hist->Add(pdf_hist_TZTH);
  hist_pdf4LHCHT_nom->Add(hist_pdf4LHCHT_nom_TZTH);
  hist_pdf4LHCHT->Add(hist_pdf4LHCHT_TZTH);
  pdf4LHC_hist->Add(pdf4LHC_hist_TZTH);
  hist_scaleHT->Add(hist_scaleHT_TZTH);
  hist_scaleHT_nom->Add(hist_scaleHT_nom_TZTH);
  hist_scaleHT_1002->Add(hist_scaleHT_1002_TZTH);
  hist_scaleHT_1003->Add(hist_scaleHT_1003_TZTH);
  hist_scaleHT_1004->Add(hist_scaleHT_1004_TZTH);
  hist_scaleHT_1005->Add(hist_scaleHT_1005_TZTH);
  hist_scaleHT_1007->Add(hist_scaleHT_1007_TZTH);
  hist_scaleHT_1009->Add(hist_scaleHT_1009_TZTH);
  hist_scaleHT_ssdl_nom->Add(hist_scaleHT_ssdl_nom_TZTH);
  hist_scaleHT_ssdl_1002->Add(hist_scaleHT_ssdl_1002_TZTH);
  hist_scaleHT_ssdl_1003->Add(hist_scaleHT_ssdl_1003_TZTH);
  hist_scaleHT_ssdl_1004->Add(hist_scaleHT_ssdl_1004_TZTH);
  hist_scaleHT_ssdl_1005->Add(hist_scaleHT_ssdl_1005_TZTH);
  hist_scaleHT_ssdl_1007->Add(hist_scaleHT_ssdl_1007_TZTH);
  hist_scaleHT_ssdl_1009->Add(hist_scaleHT_ssdl_1009_TZTH);
  hist_nom->SetBinContent(1,hist_nom->GetBinContent(1)+nom_TZTH);
  hist_jesup->SetBinContent(1,hist_jesup->GetBinContent(1)+jesup_TZTH);
  hist_jesdown->SetBinContent(1,hist_jesdown->GetBinContent(1)+jesdown_TZTH);
  hist_jerup->SetBinContent(1,hist_jerup->GetBinContent(1)+jerup_TZTH);
  hist_jerdown->SetBinContent(1,hist_jerdown->GetBinContent(1)+jerdown_TZTH);
  hist_punom->SetBinContent(1,hist_punom->GetBinContent(1)+punom_TZTH);
  hist_puup->SetBinContent(1,hist_puup->GetBinContent(1)+puup_TZTH);
  hist_pudown->SetBinContent(1,hist_pudown->GetBinContent(1)+pudown_TZTH);
  hist_idup->SetBinContent(1,hist_idup->GetBinContent(1)+idup_TZTH);
  hist_iddown->SetBinContent(1,hist_iddown->GetBinContent(1)+iddown_TZTH);
  hist_trigup->SetBinContent(1,hist_trigup->GetBinContent(1)+trigup_TZTH);
  hist_trigdown->SetBinContent(1,hist_trigdown->GetBinContent(1)+trigdown_TZTH);
  hist_prefireup->SetBinContent(1,hist_prefireup->GetBinContent(1)+prefireup_TZTH);
  hist_prefiredown->SetBinContent(1,hist_prefiredown->GetBinContent(1)+prefiredown_TZTH);

  if(DEBUG)std::cout << "==================================="<< std::endl;

  if(DEBUG)std::cout << "nom: "<< hist_nom->GetBinContent(1) <<std::endl;
  if(DEBUG)std::cout << "	jesup: "<< hist_jesup->GetBinContent(1) <<std::endl;
  if(DEBUG)std::cout << "	jesdown: "<< hist_jesdown->GetBinContent(1) <<std::endl;
  if(DEBUG)std::cout << "	jerup: "<< hist_jerup->GetBinContent(1) <<std::endl;
  if(DEBUG)std::cout << "	jerdown: "<< hist_jerdown->GetBinContent(1) <<std::endl;
  if(DEBUG)std::cout << "punom: "<< hist_punom->GetBinContent(1) <<std::endl;
  if(DEBUG)std::cout << "	puup: "<< hist_puup->GetBinContent(1) <<std::endl;
  if(DEBUG)std::cout << "	pudown: "<< hist_pudown->GetBinContent(1) <<std::endl;
  if(DEBUG)std::cout << "       idup: "<< hist_idup->GetBinContent(1) <<std::endl;
  if(DEBUG)std::cout << "       iddown: "<< hist_iddown->GetBinContent(1) <<std::endl;
  if(DEBUG)std::cout << "       trigup: "<< hist_trigup->GetBinContent(1) <<std::endl;
  if(DEBUG)std::cout << "       trigdown: "<< hist_trigdown->GetBinContent(1) <<std::endl;
  if(DEBUG)std::cout << "       prefireup: "<< hist_prefireup->GetBinContent(1) <<std::endl;
  if(DEBUG)std::cout << "       prefiredown: "<< hist_prefiredown->GetBinContent(1) <<std::endl;
  
  fsig->Write();
  fsig->Close();
  
  f_BWBW->Close();
  f_TZTZ->Close();
  f_THTH->Close();
  f_THBW->Close();
  f_TZBW->Close();
  f_TZTH->Close();

}

void CombineTTsignal_integral_jess(){
	for(int i=0;i<23;i++){
		//combinedSignal("800",i);
		combinedSignal("900",i);
		combinedSignal("1000",i);
		combinedSignal("1100",i);
		combinedSignal("1200",i);
		combinedSignal("1300",i);
		combinedSignal("1400",i);
		combinedSignal("1500",i);
		combinedSignal("1600",i);
		combinedSignal("1700",i);
		combinedSignal("1800",i);
	}
	gApplication->Terminate();
}
