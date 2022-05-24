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
  float nEvents = h->GetBinContent(1);//Integral();
  delete h;
  return nEvents;
}


void combinedSignal(TString mass, int BRtype){

  TString workArea = "/eos/uscms/store/user/wywong/FWLJMET102X_2lep2018_wywong_082020_hadds_postFWLJMETanalyzer_IsoTrig_HEMveto_ANv8/";
  //"/store/user/wywong/FWLJMET102X_2lep2018_wywong_082020_hadds_postFWLJMETanalyzer_IsoTrig_IDSFunc_HEMveto/";
  TString fname = workArea+"BprimeBprime_M-"+mass;

  TString elID = "MVA2017TightV2IsoTightRC";
  TString muID = "CBTightMiniIsoTight";
  TString era = "2018A-D_HEMveto";

  //--------------------open root files to combine : all the decays
  if(DEBUG)std::cout << "Opening Files ..." << std::endl;
  TFile* f_TWTW = TFile::Open(fname+"_TWTW_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* f_BZBZ = TFile::Open(fname+"_BZBZ_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* f_BHBH = TFile::Open(fname+"_BHBH_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* f_BHTW = TFile::Open(fname+"_BHTW_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* f_BZTW = TFile::Open(fname+"_BZTW_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* f_BZBH = TFile::Open(fname+"_BZBH_Mu"+muID+"_El"+elID+"_"+era+".root");
  
  //--------------------create new root file
  TString decay;
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
  std::cout << "Creating new file: " << "BprimeBprime_M-"+mass+"_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root" <<std::endl;
  TFile* fsig = new TFile("BprimeBprime_M-"+mass+"_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root","RECREATE");

  //--------------------which BR
  float BR=0.;
  float BR_tW=0.;
  float BR_bZ=0.;
  float BR_bH=0.;  
  float BR_TWTW = 0;
  float BR_BZBZ = 0;
  float BR_BHBH = 0;
  float BR_BHTW = 0;
  float BR_BZTW = 0;
  float BR_BZBH = 0;

  if(BRtype==0){BR_tW=0.5	;BR_bZ=0.25	;BR_bH=0.25	;} //nominal 
  if(BRtype==1){BR_tW=0.	;BR_bZ=1.0	;BR_bH=0.0	;} // 
  if(BRtype==2){BR_tW=0.	;BR_bZ=0.8	;BR_bH=0.2	;} // 
  if(BRtype==3){BR_tW=0.	;BR_bZ=0.6	;BR_bH=0.4	;} // 
  if(BRtype==4){BR_tW=0.	;BR_bZ=0.4	;BR_bH=0.6	;} // 
  if(BRtype==5){BR_tW=0.	;BR_bZ=0.2	;BR_bH=0.8	;} // 
  if(BRtype==6){BR_tW=0.	;BR_bZ=0.0	;BR_bH=1.0	;} // 
  if(BRtype==7){BR_tW=0.2	;BR_bZ=0.8	;BR_bH=0.0	;} // 
  if(BRtype==8){BR_tW=0.2	;BR_bZ=0.6	;BR_bH=0.2	;} // 
  if(BRtype==9){BR_tW=0.2	;BR_bZ=0.4	;BR_bH=0.4	;} // 
  if(BRtype==10){BR_tW=0.2	;BR_bZ=0.2	;BR_bH=0.6	;} // 
  if(BRtype==11){BR_tW=0.2	;BR_bZ=0.0	;BR_bH=0.8	;} // 
  if(BRtype==12){BR_tW=0.4	;BR_bZ=0.6	;BR_bH=0.0	;} // 
  if(BRtype==13){BR_tW=0.4	;BR_bZ=0.4	;BR_bH=0.2	;} // 
  if(BRtype==14){BR_tW=0.4	;BR_bZ=0.2	;BR_bH=0.4	;} // 
  if(BRtype==15){BR_tW=0.4	;BR_bZ=0.0	;BR_bH=0.6	;} // 
  if(BRtype==16){BR_tW=0.6	;BR_bZ=0.4	;BR_bH=0.0	;} // 
  if(BRtype==17){BR_tW=0.6	;BR_bZ=0.2	;BR_bH=0.2	;} // 
  if(BRtype==18){BR_tW=0.6	;BR_bZ=0.0	;BR_bH=0.4	;} // 
  if(BRtype==19){BR_tW=0.8	;BR_bZ=0.2	;BR_bH=0.0	;} // 
  if(BRtype==20){BR_tW=0.8	;BR_bZ=0.0	;BR_bH=0.2	;} // 
  if(BRtype==21){BR_tW=1.0	;BR_bZ=0.0	;BR_bH=0.0	;} // 
  if(BRtype==22){BR_tW=0.0	;BR_bZ=0.5	;BR_bH=0.5	;} // 
  BR_TWTW = BR_tW*BR_tW;
  BR_BZBZ = BR_bZ*BR_bZ;
  BR_BHBH = BR_bH*BR_bH;
  BR_BHTW = 2*BR_bH*BR_tW;
  BR_BZTW = 2*BR_bZ*BR_tW;
  BR_BZBH = 2*BR_bZ*BR_bH;
    
  std::cout << "Processing signal with BR : bW=" << BR_tW <<" tZ=" << BR_bZ <<" tH=" << BR_bH <<std::endl;


  float initBR; //MC sample initially has equal BR's for T -> bW, tZ,tH
  //TT
  float initBR_TWTW=0.333*0.333;
  float initBR_BZBZ=0.333*0.333;
  float initBR_BHBH=0.333*0.333;
  float initBR_BHTW=0.333*0.333*2;
  float initBR_BZTW=0.333*0.333*2;
  float initBR_BZBH=0.333*0.333*2;
  
  float Xsec = 0.;
  float nEvnts = 0.;

  float NEvts;
  //if (mass=="800"){ Xsec = 0.196; NEvts = 795000.;}
  if (mass=="900"){ Xsec = 0.0903; NEvts = 951074.915;}
  if (mass=="1000"){ Xsec = 0.0440; NEvts = 1003421.665;}
  if (mass=="1100"){ Xsec = 0.0224; NEvts = 1021828.790;}
  if (mass=="1200"){ Xsec = 0.0118; NEvts = 1020164.846;}
  if (mass=="1300"){ Xsec = 0.00639; NEvts = 1027983.479;}
  if (mass=="1400"){ Xsec = 0.00354; NEvts = 975159.796;}
  if (mass=="1500"){ Xsec = 0.00200; NEvts = 1010764.210;}
  if (mass=="1600"){ Xsec = 0.001148; NEvts = 943449.549;}
  if (mass=="1700"){ Xsec = 0.000666; NEvts = 932757.943;}
  if (mass=="1800"){ Xsec = 0.000391; NEvts = 882458.436;}
  
  //float lumi = 17.68; //fb^-1  
  float lumi = 59.74; //fb^-1  

  //vector to hold weights
  float Weights_TWTW;
  float Weights_BZBZ;
  float Weights_BHBH;
  float Weights_BHTW;
  float Weights_BZTW;
  float Weights_BZBH;
  Weights_TWTW = lumi * 1000 * ( (Xsec*BR_TWTW) / (NEvts * initBR_TWTW) ) ; //factor of 1000 to convert lumi to pb^-1
  Weights_BZBZ = lumi * 1000 * ( (Xsec*BR_BZBZ) / (NEvts * initBR_BZBZ) ) ; //factor of 1000 to convert lumi to pb^-1
  Weights_BHBH = lumi * 1000 * ( (Xsec*BR_BHBH) / (NEvts * initBR_BHBH) ) ; //factor of 1000 to convert lumi to pb^-1
  Weights_BHTW = lumi * 1000 * ( (Xsec*BR_BHTW) / (NEvts * initBR_BHTW) ) ; //factor of 1000 to convert lumi to pb^-1
  Weights_BZTW = lumi * 1000 * ( (Xsec*BR_BZTW) / (NEvts * initBR_BZTW) ) ; //factor of 1000 to convert lumi to pb^-1
  Weights_BZBH = lumi * 1000 * ( (Xsec*BR_BZBH) / (NEvts * initBR_BZBH) ) ; //factor of 1000 to convert lumi to pb^-1

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
  
  //TWTW
  TH2F* hist_pdfHT_TWTW = (TH2F*) f_TWTW->Get("hist_pdfHT");
  TH2F* pdf_hist_TWTW = (TH2F*) f_TWTW->Get("pdf_hist");
  TH2F* hist_pdf4LHCHT_nom_TWTW = (TH2F*) f_TWTW->Get("hist_pdf4LHCHT_nom");
  TH2F* hist_pdf4LHCHT_TWTW = (TH2F*) f_TWTW->Get("hist_pdf4LHCHT");
  TH2F* pdf4LHC_hist_TWTW = (TH2F*) f_TWTW->Get("pdf4LHC_hist");
  TH1F* hist_scaleHT_TWTW = (TH1F*)f_TWTW->Get("hist_scaleHT");
  TH1F* hist_scaleHT_nom_TWTW = (TH1F*)f_TWTW->Get("hist_scaleHT_nom");
  TH1F* hist_scaleHT_1002_TWTW = (TH1F*)f_TWTW->Get("hist_scaleHT_1002");
  TH1F* hist_scaleHT_1003_TWTW = (TH1F*)f_TWTW->Get("hist_scaleHT_1003");
  TH1F* hist_scaleHT_1004_TWTW = (TH1F*)f_TWTW->Get("hist_scaleHT_1004");
  TH1F* hist_scaleHT_1005_TWTW = (TH1F*)f_TWTW->Get("hist_scaleHT_1005");
  TH1F* hist_scaleHT_1007_TWTW = (TH1F*)f_TWTW->Get("hist_scaleHT_1007");
  TH1F* hist_scaleHT_1009_TWTW = (TH1F*)f_TWTW->Get("hist_scaleHT_1009");
  TH1F* hist_scaleHT_ssdl_nom_TWTW = (TH1F*)f_TWTW->Get("hist_scaleHT_ssdl_nom");
  TH1F* hist_scaleHT_ssdl_1002_TWTW = (TH1F*)f_TWTW->Get("hist_scaleHT_ssdl_1002");
  TH1F* hist_scaleHT_ssdl_1003_TWTW = (TH1F*)f_TWTW->Get("hist_scaleHT_ssdl_1003");
  TH1F* hist_scaleHT_ssdl_1004_TWTW = (TH1F*)f_TWTW->Get("hist_scaleHT_ssdl_1004");
  TH1F* hist_scaleHT_ssdl_1005_TWTW = (TH1F*)f_TWTW->Get("hist_scaleHT_ssdl_1005");
  TH1F* hist_scaleHT_ssdl_1007_TWTW = (TH1F*)f_TWTW->Get("hist_scaleHT_ssdl_1007");
  TH1F* hist_scaleHT_ssdl_1009_TWTW = (TH1F*)f_TWTW->Get("hist_scaleHT_ssdl_1009");
  TTree* t_TWTW = (TTree*) f_TWTW->Get("tEvts_sZVeto");
  TTree* t_TWTW_JECup = (TTree*) f_TWTW->Get("tEvts_sZVeto_JECup");
  TTree* t_TWTW_JECdn = (TTree*) f_TWTW->Get("tEvts_sZVeto_JECdn");
  TTree* t_TWTW_JERup = (TTree*) f_TWTW->Get("tEvts_sZVeto_JERup");
  TTree* t_TWTW_JERdn = (TTree*) f_TWTW->Get("tEvts_sZVeto_JERdn");

  //BZBZ
  TH2F* hist_pdfHT_BZBZ = (TH2F*) f_BZBZ->Get("hist_pdfHT");
  TH2F* pdf_hist_BZBZ = (TH2F*) f_BZBZ->Get("pdf_hist");
  TH2F* hist_pdf4LHCHT_nom_BZBZ = (TH2F*) f_BZBZ->Get("hist_pdf4LHCHT_nom");
  TH2F* hist_pdf4LHCHT_BZBZ = (TH2F*) f_BZBZ->Get("hist_pdf4LHCHT");
  TH2F* pdf4LHC_hist_BZBZ = (TH2F*) f_BZBZ->Get("pdf4LHC_hist");
  TH1F* hist_scaleHT_BZBZ = (TH1F*)f_BZBZ->Get("hist_scaleHT");
  TH1F* hist_scaleHT_nom_BZBZ = (TH1F*)f_BZBZ->Get("hist_scaleHT_nom");
  TH1F* hist_scaleHT_1002_BZBZ = (TH1F*)f_BZBZ->Get("hist_scaleHT_1002");
  TH1F* hist_scaleHT_1003_BZBZ = (TH1F*)f_BZBZ->Get("hist_scaleHT_1003");
  TH1F* hist_scaleHT_1004_BZBZ = (TH1F*)f_BZBZ->Get("hist_scaleHT_1004");
  TH1F* hist_scaleHT_1005_BZBZ = (TH1F*)f_BZBZ->Get("hist_scaleHT_1005");
  TH1F* hist_scaleHT_1007_BZBZ = (TH1F*)f_BZBZ->Get("hist_scaleHT_1007");
  TH1F* hist_scaleHT_1009_BZBZ = (TH1F*)f_BZBZ->Get("hist_scaleHT_1009");
  TH1F* hist_scaleHT_ssdl_nom_BZBZ = (TH1F*)f_BZBZ->Get("hist_scaleHT_ssdl_nom");
  TH1F* hist_scaleHT_ssdl_1002_BZBZ = (TH1F*)f_BZBZ->Get("hist_scaleHT_ssdl_1002");
  TH1F* hist_scaleHT_ssdl_1003_BZBZ = (TH1F*)f_BZBZ->Get("hist_scaleHT_ssdl_1003");
  TH1F* hist_scaleHT_ssdl_1004_BZBZ = (TH1F*)f_BZBZ->Get("hist_scaleHT_ssdl_1004");
  TH1F* hist_scaleHT_ssdl_1005_BZBZ = (TH1F*)f_BZBZ->Get("hist_scaleHT_ssdl_1005");
  TH1F* hist_scaleHT_ssdl_1007_BZBZ = (TH1F*)f_BZBZ->Get("hist_scaleHT_ssdl_1007");
  TH1F* hist_scaleHT_ssdl_1009_BZBZ = (TH1F*)f_BZBZ->Get("hist_scaleHT_ssdl_1009");
  TTree* t_BZBZ = (TTree*) f_BZBZ->Get("tEvts_sZVeto");
  TTree* t_BZBZ_JECup = (TTree*) f_BZBZ->Get("tEvts_sZVeto_JECup");
  TTree* t_BZBZ_JECdn = (TTree*) f_BZBZ->Get("tEvts_sZVeto_JECdn");
  TTree* t_BZBZ_JERup = (TTree*) f_BZBZ->Get("tEvts_sZVeto_JERup");
  TTree* t_BZBZ_JERdn = (TTree*) f_BZBZ->Get("tEvts_sZVeto_JERdn");

  //BHBH
  TH2F* hist_pdfHT_BHBH = (TH2F*) f_BHBH->Get("hist_pdfHT");
  TH2F* pdf_hist_BHBH = (TH2F*) f_BHBH->Get("pdf_hist");
  TH2F* hist_pdf4LHCHT_nom_BHBH = (TH2F*) f_BHBH->Get("hist_pdf4LHCHT_nom");
  TH2F* hist_pdf4LHCHT_BHBH = (TH2F*) f_BHBH->Get("hist_pdf4LHCHT");
  TH2F* pdf4LHC_hist_BHBH = (TH2F*) f_BHBH->Get("pdf4LHC_hist");
  TH1F* hist_scaleHT_BHBH = (TH1F*)f_BHBH->Get("hist_scaleHT");
  TH1F* hist_scaleHT_nom_BHBH = (TH1F*)f_BHBH->Get("hist_scaleHT_nom");
  TH1F* hist_scaleHT_1002_BHBH = (TH1F*)f_BHBH->Get("hist_scaleHT_1002");
  TH1F* hist_scaleHT_1003_BHBH = (TH1F*)f_BHBH->Get("hist_scaleHT_1003");
  TH1F* hist_scaleHT_1004_BHBH = (TH1F*)f_BHBH->Get("hist_scaleHT_1004");
  TH1F* hist_scaleHT_1005_BHBH = (TH1F*)f_BHBH->Get("hist_scaleHT_1005");
  TH1F* hist_scaleHT_1007_BHBH = (TH1F*)f_BHBH->Get("hist_scaleHT_1007");
  TH1F* hist_scaleHT_1009_BHBH = (TH1F*)f_BHBH->Get("hist_scaleHT_1009");
  TH1F* hist_scaleHT_ssdl_nom_BHBH = (TH1F*)f_BHBH->Get("hist_scaleHT_ssdl_nom");
  TH1F* hist_scaleHT_ssdl_1002_BHBH = (TH1F*)f_BHBH->Get("hist_scaleHT_ssdl_1002");
  TH1F* hist_scaleHT_ssdl_1003_BHBH = (TH1F*)f_BHBH->Get("hist_scaleHT_ssdl_1003");
  TH1F* hist_scaleHT_ssdl_1004_BHBH = (TH1F*)f_BHBH->Get("hist_scaleHT_ssdl_1004");
  TH1F* hist_scaleHT_ssdl_1005_BHBH = (TH1F*)f_BHBH->Get("hist_scaleHT_ssdl_1005");
  TH1F* hist_scaleHT_ssdl_1007_BHBH = (TH1F*)f_BHBH->Get("hist_scaleHT_ssdl_1007");
  TH1F* hist_scaleHT_ssdl_1009_BHBH = (TH1F*)f_BHBH->Get("hist_scaleHT_ssdl_1009");
  TTree* t_BHBH = (TTree*) f_BHBH->Get("tEvts_sZVeto");
  TTree* t_BHBH_JECup = (TTree*) f_BHBH->Get("tEvts_sZVeto_JECup");
  TTree* t_BHBH_JECdn = (TTree*) f_BHBH->Get("tEvts_sZVeto_JECdn");
  TTree* t_BHBH_JERup = (TTree*) f_BHBH->Get("tEvts_sZVeto_JERup");
  TTree* t_BHBH_JERdn = (TTree*) f_BHBH->Get("tEvts_sZVeto_JERdn");

  //BHTW
  TH2F* hist_pdfHT_BHTW = (TH2F*) f_BHTW->Get("hist_pdfHT");
  TH2F* pdf_hist_BHTW = (TH2F*) f_BHTW->Get("pdf_hist");
  TH2F* hist_pdf4LHCHT_nom_BHTW = (TH2F*) f_BHTW->Get("hist_pdf4LHCHT_nom");
  TH2F* hist_pdf4LHCHT_BHTW = (TH2F*) f_BHTW->Get("hist_pdf4LHCHT");
  TH2F* pdf4LHC_hist_BHTW = (TH2F*) f_BHTW->Get("pdf4LHC_hist");
  TH1F* hist_scaleHT_BHTW = (TH1F*)f_BHTW->Get("hist_scaleHT");
  TH1F* hist_scaleHT_nom_BHTW = (TH1F*)f_BHTW->Get("hist_scaleHT_nom");
  TH1F* hist_scaleHT_1002_BHTW = (TH1F*)f_BHTW->Get("hist_scaleHT_1002");
  TH1F* hist_scaleHT_1003_BHTW = (TH1F*)f_BHTW->Get("hist_scaleHT_1003");
  TH1F* hist_scaleHT_1004_BHTW = (TH1F*)f_BHTW->Get("hist_scaleHT_1004");
  TH1F* hist_scaleHT_1005_BHTW = (TH1F*)f_BHTW->Get("hist_scaleHT_1005");
  TH1F* hist_scaleHT_1007_BHTW = (TH1F*)f_BHTW->Get("hist_scaleHT_1007");
  TH1F* hist_scaleHT_1009_BHTW = (TH1F*)f_BHTW->Get("hist_scaleHT_1009");
  TH1F* hist_scaleHT_ssdl_nom_BHTW = (TH1F*)f_BHTW->Get("hist_scaleHT_ssdl_nom");
  TH1F* hist_scaleHT_ssdl_1002_BHTW = (TH1F*)f_BHTW->Get("hist_scaleHT_ssdl_1002");
  TH1F* hist_scaleHT_ssdl_1003_BHTW = (TH1F*)f_BHTW->Get("hist_scaleHT_ssdl_1003");
  TH1F* hist_scaleHT_ssdl_1004_BHTW = (TH1F*)f_BHTW->Get("hist_scaleHT_ssdl_1004");
  TH1F* hist_scaleHT_ssdl_1005_BHTW = (TH1F*)f_BHTW->Get("hist_scaleHT_ssdl_1005");
  TH1F* hist_scaleHT_ssdl_1007_BHTW = (TH1F*)f_BHTW->Get("hist_scaleHT_ssdl_1007");
  TH1F* hist_scaleHT_ssdl_1009_BHTW = (TH1F*)f_BHTW->Get("hist_scaleHT_ssdl_1009");
  TTree* t_BHTW = (TTree*) f_BHTW->Get("tEvts_sZVeto");
  TTree* t_BHTW_JECup = (TTree*) f_BHTW->Get("tEvts_sZVeto_JECup");
  TTree* t_BHTW_JECdn = (TTree*) f_BHTW->Get("tEvts_sZVeto_JECdn");
  TTree* t_BHTW_JERup = (TTree*) f_BHTW->Get("tEvts_sZVeto_JERup");
  TTree* t_BHTW_JERdn = (TTree*) f_BHTW->Get("tEvts_sZVeto_JERdn");

  //BZTW
  TH2F* hist_pdfHT_BZTW = (TH2F*) f_BZTW->Get("hist_pdfHT");
  TH2F* pdf_hist_BZTW = (TH2F*) f_BZTW->Get("pdf_hist");
  TH2F* hist_pdf4LHCHT_nom_BZTW = (TH2F*) f_BZTW->Get("hist_pdf4LHCHT_nom");
  TH2F* hist_pdf4LHCHT_BZTW = (TH2F*) f_BZTW->Get("hist_pdf4LHCHT");
  TH2F* pdf4LHC_hist_BZTW = (TH2F*) f_BZTW->Get("pdf4LHC_hist");
  TH1F* hist_scaleHT_BZTW = (TH1F*)f_BZTW->Get("hist_scaleHT");
  TH1F* hist_scaleHT_nom_BZTW = (TH1F*)f_BZTW->Get("hist_scaleHT_nom");
  TH1F* hist_scaleHT_1002_BZTW = (TH1F*)f_BZTW->Get("hist_scaleHT_1002");
  TH1F* hist_scaleHT_1003_BZTW = (TH1F*)f_BZTW->Get("hist_scaleHT_1003");
  TH1F* hist_scaleHT_1004_BZTW = (TH1F*)f_BZTW->Get("hist_scaleHT_1004");
  TH1F* hist_scaleHT_1005_BZTW = (TH1F*)f_BZTW->Get("hist_scaleHT_1005");
  TH1F* hist_scaleHT_1007_BZTW = (TH1F*)f_BZTW->Get("hist_scaleHT_1007");
  TH1F* hist_scaleHT_1009_BZTW = (TH1F*)f_BZTW->Get("hist_scaleHT_1009");
  TH1F* hist_scaleHT_ssdl_nom_BZTW = (TH1F*)f_BZTW->Get("hist_scaleHT_ssdl_nom");
  TH1F* hist_scaleHT_ssdl_1002_BZTW = (TH1F*)f_BZTW->Get("hist_scaleHT_ssdl_1002");
  TH1F* hist_scaleHT_ssdl_1003_BZTW = (TH1F*)f_BZTW->Get("hist_scaleHT_ssdl_1003");
  TH1F* hist_scaleHT_ssdl_1004_BZTW = (TH1F*)f_BZTW->Get("hist_scaleHT_ssdl_1004");
  TH1F* hist_scaleHT_ssdl_1005_BZTW = (TH1F*)f_BZTW->Get("hist_scaleHT_ssdl_1005");
  TH1F* hist_scaleHT_ssdl_1007_BZTW = (TH1F*)f_BZTW->Get("hist_scaleHT_ssdl_1007");
  TH1F* hist_scaleHT_ssdl_1009_BZTW = (TH1F*)f_BZTW->Get("hist_scaleHT_ssdl_1009");
  TTree* t_BZTW = (TTree*) f_BZTW->Get("tEvts_sZVeto");
  TTree* t_BZTW_JECup = (TTree*) f_BZTW->Get("tEvts_sZVeto_JECup");
  TTree* t_BZTW_JECdn = (TTree*) f_BZTW->Get("tEvts_sZVeto_JECdn");
  TTree* t_BZTW_JERup = (TTree*) f_BZTW->Get("tEvts_sZVeto_JERup");
  TTree* t_BZTW_JERdn = (TTree*) f_BZTW->Get("tEvts_sZVeto_JERdn");

  //BZBH
  TH2F* hist_pdfHT_BZBH = (TH2F*) f_BZBH->Get("hist_pdfHT");
  TH2F* pdf_hist_BZBH = (TH2F*) f_BZBH->Get("pdf_hist");
  TH2F* hist_pdf4LHCHT_nom_BZBH = (TH2F*) f_BZBH->Get("hist_pdf4LHCHT_nom");
  TH2F* hist_pdf4LHCHT_BZBH = (TH2F*) f_BZBH->Get("hist_pdf4LHCHT");
  TH2F* pdf4LHC_hist_BZBH = (TH2F*) f_BZBH->Get("pdf4LHC_hist");
  TH1F* hist_scaleHT_BZBH = (TH1F*)f_BZBH->Get("hist_scaleHT");
  TH1F* hist_scaleHT_nom_BZBH = (TH1F*)f_BZBH->Get("hist_scaleHT_nom");
  TH1F* hist_scaleHT_1002_BZBH = (TH1F*)f_BZBH->Get("hist_scaleHT_1002");
  TH1F* hist_scaleHT_1003_BZBH = (TH1F*)f_BZBH->Get("hist_scaleHT_1003");
  TH1F* hist_scaleHT_1004_BZBH = (TH1F*)f_BZBH->Get("hist_scaleHT_1004");
  TH1F* hist_scaleHT_1005_BZBH = (TH1F*)f_BZBH->Get("hist_scaleHT_1005");
  TH1F* hist_scaleHT_1007_BZBH = (TH1F*)f_BZBH->Get("hist_scaleHT_1007");
  TH1F* hist_scaleHT_1009_BZBH = (TH1F*)f_BZBH->Get("hist_scaleHT_1009");
  TH1F* hist_scaleHT_ssdl_nom_BZBH = (TH1F*)f_BZBH->Get("hist_scaleHT_ssdl_nom");
  TH1F* hist_scaleHT_ssdl_1002_BZBH = (TH1F*)f_BZBH->Get("hist_scaleHT_ssdl_1002");
  TH1F* hist_scaleHT_ssdl_1003_BZBH = (TH1F*)f_BZBH->Get("hist_scaleHT_ssdl_1003");
  TH1F* hist_scaleHT_ssdl_1004_BZBH = (TH1F*)f_BZBH->Get("hist_scaleHT_ssdl_1004");
  TH1F* hist_scaleHT_ssdl_1005_BZBH = (TH1F*)f_BZBH->Get("hist_scaleHT_ssdl_1005");
  TH1F* hist_scaleHT_ssdl_1007_BZBH = (TH1F*)f_BZBH->Get("hist_scaleHT_ssdl_1007");
  TH1F* hist_scaleHT_ssdl_1009_BZBH = (TH1F*)f_BZBH->Get("hist_scaleHT_ssdl_1009");    
  TTree* t_BZBH = (TTree*) f_BZBH->Get("tEvts_sZVeto");
  TTree* t_BZBH_JECup = (TTree*) f_BZBH->Get("tEvts_sZVeto_JECup");
  TTree* t_BZBH_JECdn = (TTree*) f_BZBH->Get("tEvts_sZVeto_JECdn");
  TTree* t_BZBH_JERup = (TTree*) f_BZBH->Get("tEvts_sZVeto_JERup");
  TTree* t_BZBH_JERdn = (TTree*) f_BZBH->Get("tEvts_sZVeto_JERdn");

  //--------------------scale histograms
  if(DEBUG)std::cout << "scale histograms ..." << std::endl;

  TString nomcut = "ChargeMisIDWeight * NPWeight * IsoSF * trigSF * GsfSF * PUWeight * IDSF * MCWeight * (Lep1Pt>40 && Lep2Pt>30 && nConst >=4 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200) * pdfWeights4LHC[0] ";
 
  //for jes,jer
  /*
  TRegexp reNconst("nConst");
  TString jesUpcut = nomcut; jesUpcut(reNconst) = "nConstJESup";
  TString jesDowncut = nomcut; jesDowncut(reNconst) = "nConstJESdn";
  TString jerUpcut = nomcut; jerUpcut(reNconst) = "nConstJERup";
  TString jerDowncut = nomcut; jerDowncut(reNconst) = "nConstJERdn";
  */

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


  //TWTW
  hist_pdfHT_TWTW->Scale(Weights_TWTW);
  pdf_hist_TWTW->Scale(Weights_TWTW);
  hist_pdf4LHCHT_nom_TWTW->Scale(Weights_TWTW);
  hist_pdf4LHCHT_TWTW->Scale(Weights_TWTW);
  pdf4LHC_hist_TWTW->Scale(Weights_TWTW);
  hist_scaleHT_TWTW->Scale(Weights_TWTW);
  hist_scaleHT_nom_TWTW->Scale(Weights_TWTW);
  hist_scaleHT_1002_TWTW->Scale(Weights_TWTW);
  hist_scaleHT_1003_TWTW->Scale(Weights_TWTW);
  hist_scaleHT_1004_TWTW->Scale(Weights_TWTW);
  hist_scaleHT_1005_TWTW->Scale(Weights_TWTW);
  hist_scaleHT_1007_TWTW->Scale(Weights_TWTW);
  hist_scaleHT_1009_TWTW->Scale(Weights_TWTW);
  hist_scaleHT_ssdl_nom_TWTW->Scale(Weights_TWTW);
  hist_scaleHT_ssdl_1002_TWTW->Scale(Weights_TWTW);
  hist_scaleHT_ssdl_1003_TWTW->Scale(Weights_TWTW);
  hist_scaleHT_ssdl_1004_TWTW->Scale(Weights_TWTW);
  hist_scaleHT_ssdl_1005_TWTW->Scale(Weights_TWTW);
  hist_scaleHT_ssdl_1007_TWTW->Scale(Weights_TWTW);
  hist_scaleHT_ssdl_1009_TWTW->Scale(Weights_TWTW);
  TString wStr_TWTW = std::to_string(Weights_TWTW);
  if(DEBUG)std::cout << "wStr_TWTW: "<< wStr_TWTW <<std::endl;
//   TString nomcut_TWTW = wStr_TWTW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";
//   TString jesUpcut_TWTW = wStr_TWTW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleUp > 1200)";
//   TString jesDowncut_TWTW = wStr_TWTW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleDown > 1200)";
//   TString jerUpcut_TWTW = wStr_TWTW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearUp > 1200)";
//   TString jerDowncut_TWTW = wStr_TWTW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearDown > 1200)";
  float nom_TWTW = getEvents(t_TWTW,nomcut)*Weights_TWTW;
  float jesup_TWTW = getEvents(t_TWTW_JECup,nomcut)*Weights_TWTW;
  float jesdown_TWTW = getEvents(t_TWTW_JECdn,nomcut)*Weights_TWTW;
  float jerup_TWTW = getEvents(t_TWTW_JERup,nomcut)*Weights_TWTW;
  float jerdown_TWTW = getEvents(t_TWTW_JERdn,nomcut)*Weights_TWTW;
  if(DEBUG)std::cout << "	nom_TWTW: "<< nom_TWTW <<std::endl;
  if(DEBUG)std::cout << "		jesup_TWTW: "<< jesup_TWTW <<std::endl;
  if(DEBUG)std::cout << "		jesdown_TWTW: "<< jesdown_TWTW <<std::endl;
  if(DEBUG)std::cout << "		jerup_TWTW: "<< jerup_TWTW <<std::endl;
  if(DEBUG)std::cout << "		jerdown_TWTW: "<< jerdown_TWTW <<std::endl;
  float punom_TWTW = getYields(t_TWTW,nomcut)*Weights_TWTW;
  float puup_TWTW = getYields(t_TWTW,puUpcut)*Weights_TWTW;
  float pudown_TWTW = getYields(t_TWTW,puDowncut)*Weights_TWTW;
  if(DEBUG)std::cout << "	punom_TWTW: "<< punom_TWTW/Weights_TWTW << " "<<punom_TWTW <<std::endl;
  if(DEBUG)std::cout << "		puup_TWTW: "<< puup_TWTW/Weights_TWTW <<" "<< puup_TWTW <<std::endl;
  if(DEBUG)std::cout << "		pudown_TWTW: "<< pudown_TWTW/Weights_TWTW<<" " <<pudown_TWTW <<std::endl;
  float idup_TWTW = getYields(t_TWTW,idUpcut)*Weights_TWTW;
  float iddown_TWTW = getYields(t_TWTW,idDowncut)*Weights_TWTW;
  if(DEBUG)std::cout << "               idup_TWTW: "<< idup_TWTW <<std::endl;
  if(DEBUG)std::cout << "               iddown_TWTW: "<< iddown_TWTW <<std::endl;
  float trigup_TWTW = getYields(t_TWTW,trigUpcut)*Weights_TWTW;
  float trigdown_TWTW = getYields(t_TWTW,trigDowncut)*Weights_TWTW;
  if(DEBUG)std::cout << "               trigup_TWTW: "<< trigup_TWTW <<std::endl;
  if(DEBUG)std::cout << "               trigdown_TWTW: "<< trigdown_TWTW <<std::endl;
  float prefireup_TWTW = getYields(t_TWTW,prefireUpcut)*Weights_TWTW;
  float prefiredown_TWTW = getYields(t_TWTW,prefireDowncut)*Weights_TWTW;
  if(DEBUG)std::cout << "               prefireup_TWTW: "<< prefireup_TWTW <<std::endl;
  if(DEBUG)std::cout << "               prefiredown_TWTW: "<< prefiredown_TWTW <<std::endl;

  //BZBZ
  hist_pdfHT_BZBZ->Scale(Weights_BZBZ);
  pdf_hist_BZBZ->Scale(Weights_BZBZ);
  hist_pdf4LHCHT_nom_BZBZ->Scale(Weights_BZBZ);
  hist_pdf4LHCHT_BZBZ->Scale(Weights_BZBZ);
  pdf4LHC_hist_BZBZ->Scale(Weights_BZBZ);
  hist_scaleHT_BZBZ->Scale(Weights_BZBZ);
  hist_scaleHT_nom_BZBZ->Scale(Weights_BZBZ);
  hist_scaleHT_1002_BZBZ->Scale(Weights_BZBZ);
  hist_scaleHT_1003_BZBZ->Scale(Weights_BZBZ);
  hist_scaleHT_1004_BZBZ->Scale(Weights_BZBZ);
  hist_scaleHT_1005_BZBZ->Scale(Weights_BZBZ);
  hist_scaleHT_1007_BZBZ->Scale(Weights_BZBZ);
  hist_scaleHT_1009_BZBZ->Scale(Weights_BZBZ);
  hist_scaleHT_ssdl_nom_BZBZ->Scale(Weights_BZBZ);
  hist_scaleHT_ssdl_1002_BZBZ->Scale(Weights_BZBZ);
  hist_scaleHT_ssdl_1003_BZBZ->Scale(Weights_BZBZ);
  hist_scaleHT_ssdl_1004_BZBZ->Scale(Weights_BZBZ);
  hist_scaleHT_ssdl_1005_BZBZ->Scale(Weights_BZBZ);
  hist_scaleHT_ssdl_1007_BZBZ->Scale(Weights_BZBZ);
  hist_scaleHT_ssdl_1009_BZBZ->Scale(Weights_BZBZ);
  TString wStr_BZBZ = std::to_string(Weights_BZBZ);
  if(DEBUG)std::cout << "wStr_BZBZ: "<< wStr_BZBZ <<std::endl;
//   TString nomcut_BZBZ = wStr_BZBZ+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";
//   TString jesUpcut_BZBZ = wStr_BZBZ+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleUp > 1200)";
//   TString jesDowncut_BZBZ = wStr_BZBZ+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleDown > 1200)";
//   TString jerUpcut_BZBZ = wStr_BZBZ+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearUp > 1200)";
//   TString jerDowncut_BZBZ = wStr_BZBZ+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearDown > 1200)";
  float nom_BZBZ = getEvents(t_BZBZ,nomcut)*Weights_BZBZ;
  float jesup_BZBZ = getEvents(t_BZBZ_JECup,nomcut)*Weights_BZBZ;
  float jesdown_BZBZ = getEvents(t_BZBZ_JECdn,nomcut)*Weights_BZBZ;
  float jerup_BZBZ = getEvents(t_BZBZ_JERup,nomcut)*Weights_BZBZ;
  float jerdown_BZBZ = getEvents(t_BZBZ_JERdn,nomcut)*Weights_BZBZ;
  if(DEBUG)std::cout << "	nom_BZBZ: "<< nom_BZBZ <<std::endl;
  if(DEBUG)std::cout << "		jesup_BZBZ: "<< jesup_BZBZ <<std::endl;
  if(DEBUG)std::cout << "		jesdown_BZBZ: "<< jesdown_BZBZ <<std::endl;
  if(DEBUG)std::cout << "		jerup_BZBZ: "<< jerup_BZBZ <<std::endl;
  if(DEBUG)std::cout << "		jerdown_BZBZ: "<< jerdown_BZBZ <<std::endl;
  float punom_BZBZ = getYields(t_BZBZ,nomcut)*Weights_BZBZ;
  float puup_BZBZ = getYields(t_BZBZ,puUpcut)*Weights_BZBZ;
  float pudown_BZBZ = getYields(t_BZBZ,puDowncut)*Weights_BZBZ;
  if(DEBUG)std::cout << "	punom_BZBZ: "<< punom_BZBZ <<std::endl;
  if(DEBUG)std::cout << "		puup_BZBZ: "<< puup_BZBZ <<std::endl;
  if(DEBUG)std::cout << "		pudown_BZBZ: "<< pudown_BZBZ <<std::endl;
  float idup_BZBZ = getYields(t_BZBZ,idUpcut)*Weights_BZBZ;
  float iddown_BZBZ = getYields(t_BZBZ,idDowncut)*Weights_BZBZ;
  if(DEBUG)std::cout << "               idup_BZBZ: "<< idup_BZBZ <<std::endl;
  if(DEBUG)std::cout << "               iddown_BZBZ: "<< iddown_BZBZ <<std::endl;
  float trigup_BZBZ = getYields(t_BZBZ,trigUpcut)*Weights_BZBZ;
  float trigdown_BZBZ = getYields(t_BZBZ,trigDowncut)*Weights_BZBZ;
  if(DEBUG)std::cout << "               trigup_BZBZ: "<< trigup_BZBZ <<std::endl;
  if(DEBUG)std::cout << "               trigdown_BZBZ: "<< trigdown_BZBZ <<std::endl;
  float prefireup_BZBZ = getYields(t_BZBZ,prefireUpcut)*Weights_BZBZ;
  float prefiredown_BZBZ = getYields(t_BZBZ,prefireDowncut)*Weights_BZBZ;
  if(DEBUG)std::cout << "               prefireup_BZBZ: "<< prefireup_BZBZ <<std::endl;
  if(DEBUG)std::cout << "               prefiredown_BZBZ: "<< prefiredown_BZBZ <<std::endl;

  //BHBH
  hist_pdfHT_BHBH->Scale(Weights_BHBH);
  pdf_hist_BHBH->Scale(Weights_BHBH);
  hist_pdf4LHCHT_nom_BHBH->Scale(Weights_BHBH);
  hist_pdf4LHCHT_BHBH->Scale(Weights_BHBH);
  pdf4LHC_hist_BHBH->Scale(Weights_BHBH);
  hist_scaleHT_BHBH->Scale(Weights_BHBH);
  hist_scaleHT_nom_BHBH->Scale(Weights_BHBH);
  hist_scaleHT_1002_BHBH->Scale(Weights_BHBH);
  hist_scaleHT_1003_BHBH->Scale(Weights_BHBH);
  hist_scaleHT_1004_BHBH->Scale(Weights_BHBH);
  hist_scaleHT_1005_BHBH->Scale(Weights_BHBH);
  hist_scaleHT_1007_BHBH->Scale(Weights_BHBH);
  hist_scaleHT_1009_BHBH->Scale(Weights_BHBH);
  hist_scaleHT_ssdl_nom_BHBH->Scale(Weights_BHBH);
  hist_scaleHT_ssdl_1002_BHBH->Scale(Weights_BHBH);
  hist_scaleHT_ssdl_1003_BHBH->Scale(Weights_BHBH);
  hist_scaleHT_ssdl_1004_BHBH->Scale(Weights_BHBH);
  hist_scaleHT_ssdl_1005_BHBH->Scale(Weights_BHBH);
  hist_scaleHT_ssdl_1007_BHBH->Scale(Weights_BHBH);
  hist_scaleHT_ssdl_1009_BHBH->Scale(Weights_BHBH);
  TString wStr_BHBH = std::to_string(Weights_BHBH);
  if(DEBUG)std::cout << "wStr_BHBH: "<< wStr_BHBH <<std::endl;
//   TString nomcut_BHBH = wStr_BHBH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";
//   TString jesUpcut_BHBH = wStr_BHBH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleUp > 1200)";
//   TString jesDowncut_BHBH = wStr_BHBH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleDown > 1200)";
//   TString jerUpcut_BHBH = wStr_BHBH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearUp > 1200)";
//   TString jerDowncut_BHBH = wStr_BHBH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearDown > 1200)";
  float nom_BHBH = getEvents(t_BHBH,nomcut)*Weights_BHBH;
  float jesup_BHBH = getEvents(t_BHBH_JECup,nomcut)*Weights_BHBH;
  float jesdown_BHBH = getEvents(t_BHBH_JECdn,nomcut)*Weights_BHBH;
  float jerup_BHBH = getEvents(t_BHBH_JERup,nomcut)*Weights_BHBH;
  float jerdown_BHBH = getEvents(t_BHBH_JERdn,nomcut)*Weights_BHBH;
  if(DEBUG)std::cout << "	nom_BHBH: "<< nom_BHBH <<std::endl;
  if(DEBUG)std::cout << "		jesup_BHBH: "<< jesup_BHBH <<std::endl;
  if(DEBUG)std::cout << "		jesdown_BHBH: "<< jesdown_BHBH <<std::endl;
  if(DEBUG)std::cout << "		jerup_BHBH: "<< jerup_BHBH <<std::endl;
  if(DEBUG)std::cout << "		jerdown_BHBH: "<< jerdown_BHBH <<std::endl;
  float punom_BHBH = getYields(t_BHBH,nomcut)*Weights_BHBH;
  float puup_BHBH = getYields(t_BHBH,puUpcut)*Weights_BHBH;
  float pudown_BHBH = getYields(t_BHBH,puDowncut)*Weights_BHBH;
  if(DEBUG)std::cout << "	punom_BHBH: "<< punom_BHBH <<std::endl;
  if(DEBUG)std::cout << "		puup_BHBH: "<< puup_BHBH <<std::endl;
  if(DEBUG)std::cout << "		pudown_BHBH: "<< pudown_BHBH <<std::endl;
  float idup_BHBH = getYields(t_BHBH,idUpcut)*Weights_BHBH;
  float iddown_BHBH = getYields(t_BHBH,idDowncut)*Weights_BHBH;
  if(DEBUG)std::cout << "               idup_BHBH: "<< idup_BHBH <<std::endl;
  if(DEBUG)std::cout << "               iddown_BHBH: "<< iddown_BHBH <<std::endl;
  float trigup_BHBH = getYields(t_BHBH,trigUpcut)*Weights_BHBH;
  float trigdown_BHBH = getYields(t_BHBH,trigDowncut)*Weights_BHBH;
  if(DEBUG)std::cout << "               trigup_BHBH: "<< trigup_BHBH <<std::endl;
  if(DEBUG)std::cout << "               trigdown_BHBH: "<< trigdown_BHBH <<std::endl;
  float prefireup_BHBH = getYields(t_BHBH,prefireUpcut)*Weights_BHBH;
  float prefiredown_BHBH = getYields(t_BHBH,prefireDowncut)*Weights_BHBH;
  if(DEBUG)std::cout << "               prefireup_BHBH: "<< prefireup_BHBH <<std::endl;
  if(DEBUG)std::cout << "               prefiredown_BHBH: "<< prefiredown_BHBH <<std::endl;


  //BHTW
  hist_pdfHT_BHTW->Scale(Weights_BHTW);
  pdf_hist_BHTW->Scale(Weights_BHTW);
  hist_pdf4LHCHT_nom_BHTW->Scale(Weights_BHTW);
  hist_pdf4LHCHT_BHTW->Scale(Weights_BHTW);
  pdf4LHC_hist_BHTW->Scale(Weights_BHTW);
  hist_scaleHT_BHTW->Scale(Weights_BHTW);
  hist_scaleHT_nom_BHTW->Scale(Weights_BHTW);
  hist_scaleHT_1002_BHTW->Scale(Weights_BHTW);
  hist_scaleHT_1003_BHTW->Scale(Weights_BHTW);
  hist_scaleHT_1004_BHTW->Scale(Weights_BHTW);
  hist_scaleHT_1005_BHTW->Scale(Weights_BHTW);
  hist_scaleHT_1007_BHTW->Scale(Weights_BHTW);
  hist_scaleHT_1009_BHTW->Scale(Weights_BHTW);
  hist_scaleHT_ssdl_nom_BHTW->Scale(Weights_BHTW);
  hist_scaleHT_ssdl_1002_BHTW->Scale(Weights_BHTW);
  hist_scaleHT_ssdl_1003_BHTW->Scale(Weights_BHTW);
  hist_scaleHT_ssdl_1004_BHTW->Scale(Weights_BHTW);
  hist_scaleHT_ssdl_1005_BHTW->Scale(Weights_BHTW);
  hist_scaleHT_ssdl_1007_BHTW->Scale(Weights_BHTW);
  hist_scaleHT_ssdl_1009_BHTW->Scale(Weights_BHTW);
  TString wStr_BHTW = std::to_string(Weights_BHTW);
  if(DEBUG)std::cout << "wStr_BHTW: "<< wStr_BHTW <<std::endl;
//   TString nomcut_BHTW = wStr_BHTW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";
//   TString jesUpcut_BHTW = wStr_BHTW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleUp > 1200)";
//   TString jesDowncut_BHTW = wStr_BHTW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleDown > 1200)";
//   TString jerUpcut_BHTW = wStr_BHTW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearUp > 1200)";
//   TString jerDowncut_BHTW = wStr_BHTW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearDown > 1200)";
  float nom_BHTW = getEvents(t_BHTW,nomcut)*Weights_BHTW;
  float jesup_BHTW = getEvents(t_BHTW_JECup,nomcut)*Weights_BHTW;
  float jesdown_BHTW = getEvents(t_BHTW_JECdn,nomcut)*Weights_BHTW;
  float jerup_BHTW = getEvents(t_BHTW_JERup,nomcut)*Weights_BHTW;
  float jerdown_BHTW = getEvents(t_BHTW_JERdn,nomcut)*Weights_BHTW;
  if(DEBUG)std::cout << "	nom_BHTW: "<< nom_BHTW <<std::endl;
  if(DEBUG)std::cout << "		jesup_BHTW: "<< jesup_BHTW <<std::endl;
  if(DEBUG)std::cout << "		jesdown_BHTW: "<< jesdown_BHTW <<std::endl;
  if(DEBUG)std::cout << "		jerup_BHTW: "<< jerup_BHTW <<std::endl;
  if(DEBUG)std::cout << "		jerdown_BHTW: "<< jerdown_BHTW <<std::endl;
  float punom_BHTW = getYields(t_BHTW,nomcut)*Weights_BHTW;
  float puup_BHTW = getYields(t_BHTW,puUpcut)*Weights_BHTW;
  float pudown_BHTW = getYields(t_BHTW,puDowncut)*Weights_BHTW;
  if(DEBUG)std::cout << "	punom_BHTW: "<< punom_BHTW <<std::endl;
  if(DEBUG)std::cout << "		puup_BHTW: "<< puup_BHTW <<std::endl;
  if(DEBUG)std::cout << "		pudown_BHTW: "<< pudown_BHTW <<std::endl;
  float idup_BHTW = getYields(t_BHTW,idUpcut)*Weights_BHTW;
  float iddown_BHTW = getYields(t_BHTW,idDowncut)*Weights_BHTW;
  if(DEBUG)std::cout << "               idup_BHTW: "<< idup_BHTW <<std::endl;
  if(DEBUG)std::cout << "               iddown_BHTW: "<< iddown_BHTW <<std::endl;
  float trigup_BHTW = getYields(t_BHTW,trigUpcut)*Weights_BHTW;
  float trigdown_BHTW = getYields(t_BHTW,trigDowncut)*Weights_BHTW;
  if(DEBUG)std::cout << "               trigup_BHTW: "<< trigup_BHTW <<std::endl;
  if(DEBUG)std::cout << "               trigdown_BHTW: "<< trigdown_BHTW <<std::endl;
  float prefireup_BHTW = getYields(t_BHTW,prefireUpcut)*Weights_BHTW;
  float prefiredown_BHTW = getYields(t_BHTW,prefireDowncut)*Weights_BHTW;
  if(DEBUG)std::cout << "               prefireup_BHTW: "<< prefireup_BHTW <<std::endl;
  if(DEBUG)std::cout << "               prefiredown_BHTW: "<< prefiredown_BHTW <<std::endl;

  //BZTW
  hist_pdfHT_BZTW->Scale(Weights_BZTW);
  pdf_hist_BZTW->Scale(Weights_BZTW);
  hist_pdf4LHCHT_nom_BZTW->Scale(Weights_BZTW);
  hist_pdf4LHCHT_BZTW->Scale(Weights_BZTW);
  pdf4LHC_hist_BZTW->Scale(Weights_BZTW);
  hist_scaleHT_BZTW->Scale(Weights_BZTW);
  hist_scaleHT_nom_BZTW->Scale(Weights_BZTW);
  hist_scaleHT_1002_BZTW->Scale(Weights_BZTW);
  hist_scaleHT_1003_BZTW->Scale(Weights_BZTW);
  hist_scaleHT_1004_BZTW->Scale(Weights_BZTW);
  hist_scaleHT_1005_BZTW->Scale(Weights_BZTW);
  hist_scaleHT_1007_BZTW->Scale(Weights_BZTW);
  hist_scaleHT_1009_BZTW->Scale(Weights_BZTW);
  hist_scaleHT_ssdl_nom_BZTW->Scale(Weights_BZTW);
  hist_scaleHT_ssdl_1002_BZTW->Scale(Weights_BZTW);
  hist_scaleHT_ssdl_1003_BZTW->Scale(Weights_BZTW);
  hist_scaleHT_ssdl_1004_BZTW->Scale(Weights_BZTW);
  hist_scaleHT_ssdl_1005_BZTW->Scale(Weights_BZTW);
  hist_scaleHT_ssdl_1007_BZTW->Scale(Weights_BZTW);
  hist_scaleHT_ssdl_1009_BZTW->Scale(Weights_BZTW);
  TString wStr_BZTW = std::to_string(Weights_BZTW);
  if(DEBUG)std::cout << "wStr_BZTW: "<< wStr_BZTW <<std::endl;
//   TString nomcut_BZTW = wStr_BZTW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";
//   TString jesUpcut_BZTW = wStr_BZTW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleUp > 1200)";
//   TString jesDowncut_BZTW = wStr_BZTW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleDown > 1200)";
//   TString jerUpcut_BZTW = wStr_BZTW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearUp > 1200)";
//   TString jerDowncut_BZTW = wStr_BZTW+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearDown > 1200)";
  float nom_BZTW = getEvents(t_BZTW,nomcut)*Weights_BZTW;
  float jesup_BZTW = getEvents(t_BZTW_JECup,nomcut)*Weights_BZTW;
  float jesdown_BZTW = getEvents(t_BZTW_JECdn,nomcut)*Weights_BZTW;
  float jerup_BZTW = getEvents(t_BZTW_JERup,nomcut)*Weights_BZTW;
  float jerdown_BZTW = getEvents(t_BZTW_JERdn,nomcut)*Weights_BZTW;
  if(DEBUG)std::cout << "	nom_BZTW: "<< nom_BZTW <<std::endl;
  if(DEBUG)std::cout << "		jesup_BZTW: "<< jesup_BZTW <<std::endl;
  if(DEBUG)std::cout << "		jesdown_BZTW: "<< jesdown_BZTW <<std::endl;
  if(DEBUG)std::cout << "		jerup_BZTW: "<< jerup_BZTW <<std::endl;
  if(DEBUG)std::cout << "		jerdown_BZTW: "<< jerdown_BZTW <<std::endl;
  float punom_BZTW = getYields(t_BZTW,nomcut)*Weights_BZTW;
  float puup_BZTW = getYields(t_BZTW,puUpcut)*Weights_BZTW;
  float pudown_BZTW = getYields(t_BZTW,puDowncut)*Weights_BZTW;
  if(DEBUG)std::cout << "	punom_BZTW: "<< punom_BZTW <<std::endl;
  if(DEBUG)std::cout << "		puup_BZTW: "<< puup_BZTW <<std::endl;
  if(DEBUG)std::cout << "		pudown_BZTW: "<< pudown_BZTW <<std::endl;
  float idup_BZTW = getYields(t_BZTW,idUpcut)*Weights_BZTW;
  float iddown_BZTW = getYields(t_BZTW,idDowncut)*Weights_BZTW;
  if(DEBUG)std::cout << "               idup_BZTW: "<< idup_BZTW <<std::endl;
  if(DEBUG)std::cout << "               iddown_BZTW: "<< iddown_BZTW <<std::endl;
  float trigup_BZTW = getYields(t_BZTW,trigUpcut)*Weights_BZTW;
  float trigdown_BZTW = getYields(t_BZTW,trigDowncut)*Weights_BZTW;
  if(DEBUG)std::cout << "               trigup_BZTW: "<< trigup_BZTW <<std::endl;
  if(DEBUG)std::cout << "               trigdown_BZTW: "<< trigdown_BZTW <<std::endl;
  float prefireup_BZTW = getYields(t_BZTW,prefireUpcut)*Weights_BZTW;
  float prefiredown_BZTW = getYields(t_BZTW,prefireDowncut)*Weights_BZTW;
  if(DEBUG)std::cout << "               prefireup_BZTW: "<< prefireup_BZTW <<std::endl;
  if(DEBUG)std::cout << "               prefiredown_BZTW: "<< prefiredown_BZTW <<std::endl;

  //BZBH
  hist_pdfHT_BZBH->Scale(Weights_BZBH);
  pdf_hist_BZBH->Scale(Weights_BZBH);
  hist_pdf4LHCHT_nom_BZBH->Scale(Weights_BZBH);
  hist_pdf4LHCHT_BZBH->Scale(Weights_BZBH);
  pdf4LHC_hist_BZBH->Scale(Weights_BZBH);
  hist_scaleHT_BZBH->Scale(Weights_BZBH);
  hist_scaleHT_nom_BZBH->Scale(Weights_BZBH);
  hist_scaleHT_1002_BZBH->Scale(Weights_BZBH);
  hist_scaleHT_1003_BZBH->Scale(Weights_BZBH);
  hist_scaleHT_1004_BZBH->Scale(Weights_BZBH);
  hist_scaleHT_1005_BZBH->Scale(Weights_BZBH);
  hist_scaleHT_1007_BZBH->Scale(Weights_BZBH);
  hist_scaleHT_1009_BZBH->Scale(Weights_BZBH);
  hist_scaleHT_ssdl_nom_BZBH->Scale(Weights_BZBH);
  hist_scaleHT_ssdl_1002_BZBH->Scale(Weights_BZBH);
  hist_scaleHT_ssdl_1003_BZBH->Scale(Weights_BZBH);
  hist_scaleHT_ssdl_1004_BZBH->Scale(Weights_BZBH);
  hist_scaleHT_ssdl_1005_BZBH->Scale(Weights_BZBH);
  hist_scaleHT_ssdl_1007_BZBH->Scale(Weights_BZBH);
  hist_scaleHT_ssdl_1009_BZBH->Scale(Weights_BZBH);
  TString wStr_BZBH = std::to_string(Weights_BZBH);
  if(DEBUG)std::cout << "wStr_BZBH: "<< wStr_BZBH <<std::endl;
//   TString nomcut_BZBH = wStr_BZBH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HT > 1200)";
//   TString jesUpcut_BZBH = wStr_BZBH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleUp > 1200)";
//   TString jesDowncut_BZBH = wStr_BZBH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTScaleDown > 1200)";
//   TString jerUpcut_BZBH = wStr_BZBH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearUp > 1200)";
//   TString jerDowncut_BZBH = wStr_BZBH+" * MCWeight * (Lep1Pt>40 && nConst >=5 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ) && cleanAK4HTSmearDown > 1200)";
  float nom_BZBH = getEvents(t_BZBH,nomcut)*Weights_BZBH;
  float jesup_BZBH = getEvents(t_BZBH_JECup,nomcut)*Weights_BZBH;
  float jesdown_BZBH = getEvents(t_BZBH_JECdn,nomcut)*Weights_BZBH;
  float jerup_BZBH = getEvents(t_BZBH_JERup,nomcut)*Weights_BZBH;
  float jerdown_BZBH = getEvents(t_BZBH_JERdn,nomcut)*Weights_BZBH;
  if(DEBUG)std::cout << "	nom_BZBH: "<< nom_BZBH <<std::endl;
  if(DEBUG)std::cout << "		jesup_BZBH: "<< jesup_BZBH <<std::endl;
  if(DEBUG)std::cout << "		jesdown_BZBH: "<< jesdown_BZBH <<std::endl;
  if(DEBUG)std::cout << "		jerup_BZBH: "<< jerup_BZBH <<std::endl;
  if(DEBUG)std::cout << "		jerdown_BZBH: "<< jerdown_BZBH <<std::endl;
  float punom_BZBH = getYields(t_BZBH,nomcut)*Weights_BZBH;
  float puup_BZBH = getYields(t_BZBH,puUpcut)*Weights_BZBH;
  float pudown_BZBH = getYields(t_BZBH,puDowncut)*Weights_BZBH;
  if(DEBUG)std::cout << "	punom_BZBH: "<< punom_BZBH <<std::endl;
  if(DEBUG)std::cout << "		puup_BZBH: "<< puup_BZBH <<std::endl;
  if(DEBUG)std::cout << "		pudown_BZBH: "<< pudown_BZBH <<std::endl;
  float idup_BZBH = getYields(t_BZBH,idUpcut)*Weights_BZBH;
  float iddown_BZBH = getYields(t_BZBH,idDowncut)*Weights_BZBH;
  if(DEBUG)std::cout << "               idup_BZBH: "<< idup_BZBH <<std::endl;
  if(DEBUG)std::cout << "               iddown_BZBH: "<< iddown_BZBH <<std::endl;
  float trigup_BZBH = getYields(t_BZBH,trigUpcut)*Weights_BZBH;
  float trigdown_BZBH = getYields(t_BZBH,trigDowncut)*Weights_BZBH;
  if(DEBUG)std::cout << "               trigup_BZBH: "<< trigup_BZBH <<std::endl;
  if(DEBUG)std::cout << "               trigdown_BZBH: "<< trigdown_BZBH <<std::endl;
  float prefireup_BZBH = getYields(t_BZBH,prefireUpcut)*Weights_BZBH;
  float prefiredown_BZBH = getYields(t_BZBH,prefireDowncut)*Weights_BZBH;
  if(DEBUG)std::cout << "               prefireup_BZBH: "<< prefireup_BZBH <<std::endl;
  if(DEBUG)std::cout << "               prefiredown_BZBH: "<< prefiredown_BZBH <<std::endl;

  //--------------------combine and save
  if(DEBUG)std::cout << "combine and save ..." << std::endl;

  //TWTW
  hist_pdfHT->Add(hist_pdfHT_TWTW);
  pdf_hist->Add(pdf_hist_TWTW);
  hist_pdf4LHCHT_nom->Add(hist_pdf4LHCHT_nom_TWTW);
  hist_pdf4LHCHT->Add(hist_pdf4LHCHT_TWTW);
  pdf4LHC_hist->Add(pdf4LHC_hist_TWTW);
  hist_scaleHT->Add(hist_scaleHT_TWTW);
  hist_scaleHT_nom->Add(hist_scaleHT_nom_TWTW);
  hist_scaleHT_1002->Add(hist_scaleHT_1002_TWTW);
  hist_scaleHT_1003->Add(hist_scaleHT_1003_TWTW);
  hist_scaleHT_1004->Add(hist_scaleHT_1004_TWTW);
  hist_scaleHT_1005->Add(hist_scaleHT_1005_TWTW);
  hist_scaleHT_1007->Add(hist_scaleHT_1007_TWTW);
  hist_scaleHT_1009->Add(hist_scaleHT_1009_TWTW);
  hist_scaleHT_ssdl_nom->Add(hist_scaleHT_ssdl_nom_TWTW);
  hist_scaleHT_ssdl_1002->Add(hist_scaleHT_ssdl_1002_TWTW);
  hist_scaleHT_ssdl_1003->Add(hist_scaleHT_ssdl_1003_TWTW);
  hist_scaleHT_ssdl_1004->Add(hist_scaleHT_ssdl_1004_TWTW);
  hist_scaleHT_ssdl_1005->Add(hist_scaleHT_ssdl_1005_TWTW);
  hist_scaleHT_ssdl_1007->Add(hist_scaleHT_ssdl_1007_TWTW);
  hist_scaleHT_ssdl_1009->Add(hist_scaleHT_ssdl_1009_TWTW);
  hist_nom->SetBinContent(1,hist_nom->GetBinContent(1)+nom_TWTW);
  hist_jesup->SetBinContent(1,hist_jesup->GetBinContent(1)+jesup_TWTW);
  hist_jesdown->SetBinContent(1,hist_jesdown->GetBinContent(1)+jesdown_TWTW);
  hist_jerup->SetBinContent(1,hist_jerup->GetBinContent(1)+jerup_TWTW);
  hist_jerdown->SetBinContent(1,hist_jerdown->GetBinContent(1)+jerdown_TWTW);
  hist_punom->SetBinContent(1,hist_punom->GetBinContent(1)+punom_TWTW);
  hist_puup->SetBinContent(1,hist_puup->GetBinContent(1)+puup_TWTW);
  hist_pudown->SetBinContent(1,hist_pudown->GetBinContent(1)+pudown_TWTW);
  hist_idup->SetBinContent(1,hist_idup->GetBinContent(1)+idup_TWTW);
  hist_iddown->SetBinContent(1,hist_iddown->GetBinContent(1)+iddown_TWTW);
  hist_trigup->SetBinContent(1,hist_trigup->GetBinContent(1)+trigup_TWTW);
  hist_trigdown->SetBinContent(1,hist_trigdown->GetBinContent(1)+trigdown_TWTW);
  hist_prefireup->SetBinContent(1,hist_prefireup->GetBinContent(1)+prefireup_TWTW);
  hist_prefiredown->SetBinContent(1,hist_prefiredown->GetBinContent(1)+prefiredown_TWTW);

  //BZBZ
  hist_pdfHT->Add(hist_pdfHT_BZBZ);
  pdf_hist->Add(pdf_hist_BZBZ);
  hist_pdf4LHCHT_nom->Add(hist_pdf4LHCHT_nom_BZBZ);
  hist_pdf4LHCHT->Add(hist_pdf4LHCHT_BZBZ);
  pdf4LHC_hist->Add(pdf4LHC_hist_BZBZ);
  hist_scaleHT->Add(hist_scaleHT_BZBZ);
  hist_scaleHT_nom->Add(hist_scaleHT_nom_BZBZ);
  hist_scaleHT_1002->Add(hist_scaleHT_1002_BZBZ);
  hist_scaleHT_1003->Add(hist_scaleHT_1003_BZBZ);
  hist_scaleHT_1004->Add(hist_scaleHT_1004_BZBZ);
  hist_scaleHT_1005->Add(hist_scaleHT_1005_BZBZ);
  hist_scaleHT_1007->Add(hist_scaleHT_1007_BZBZ);
  hist_scaleHT_1009->Add(hist_scaleHT_1009_BZBZ);
  hist_scaleHT_ssdl_nom->Add(hist_scaleHT_ssdl_nom_BZBZ);
  hist_scaleHT_ssdl_1002->Add(hist_scaleHT_ssdl_1002_BZBZ);
  hist_scaleHT_ssdl_1003->Add(hist_scaleHT_ssdl_1003_BZBZ);
  hist_scaleHT_ssdl_1004->Add(hist_scaleHT_ssdl_1004_BZBZ);
  hist_scaleHT_ssdl_1005->Add(hist_scaleHT_ssdl_1005_BZBZ);
  hist_scaleHT_ssdl_1007->Add(hist_scaleHT_ssdl_1007_BZBZ);
  hist_scaleHT_ssdl_1009->Add(hist_scaleHT_ssdl_1009_BZBZ);
  hist_nom->SetBinContent(1,hist_nom->GetBinContent(1)+nom_BZBZ);
  hist_jesup->SetBinContent(1,hist_jesup->GetBinContent(1)+jesup_BZBZ);
  hist_jesdown->SetBinContent(1,hist_jesdown->GetBinContent(1)+jesdown_BZBZ);
  hist_jerup->SetBinContent(1,hist_jerup->GetBinContent(1)+jerup_BZBZ);
  hist_jerdown->SetBinContent(1,hist_jerdown->GetBinContent(1)+jerdown_BZBZ);
  hist_punom->SetBinContent(1,hist_punom->GetBinContent(1)+punom_BZBZ);
  hist_puup->SetBinContent(1,hist_puup->GetBinContent(1)+puup_BZBZ);
  hist_pudown->SetBinContent(1,hist_pudown->GetBinContent(1)+pudown_BZBZ);
  hist_idup->SetBinContent(1,hist_idup->GetBinContent(1)+idup_BZBZ);
  hist_iddown->SetBinContent(1,hist_iddown->GetBinContent(1)+iddown_BZBZ);
  hist_trigup->SetBinContent(1,hist_trigup->GetBinContent(1)+trigup_BZBZ);
  hist_trigdown->SetBinContent(1,hist_trigdown->GetBinContent(1)+trigdown_BZBZ);
  hist_prefireup->SetBinContent(1,hist_prefireup->GetBinContent(1)+prefireup_BZBZ);
  hist_prefiredown->SetBinContent(1,hist_prefiredown->GetBinContent(1)+prefiredown_BZBZ);

  //BHBH
  hist_pdfHT->Add(hist_pdfHT_BHBH);
  pdf_hist->Add(pdf_hist_BHBH);
  hist_pdf4LHCHT_nom->Add(hist_pdf4LHCHT_nom_BHBH);
  hist_pdf4LHCHT->Add(hist_pdf4LHCHT_BHBH);
  pdf4LHC_hist->Add(pdf4LHC_hist_BHBH);
  hist_scaleHT->Add(hist_scaleHT_BHBH);
  hist_scaleHT_nom->Add(hist_scaleHT_nom_BHBH);
  hist_scaleHT_1002->Add(hist_scaleHT_1002_BHBH);
  hist_scaleHT_1003->Add(hist_scaleHT_1003_BHBH);
  hist_scaleHT_1004->Add(hist_scaleHT_1004_BHBH);
  hist_scaleHT_1005->Add(hist_scaleHT_1005_BHBH);
  hist_scaleHT_1007->Add(hist_scaleHT_1007_BHBH);
  hist_scaleHT_1009->Add(hist_scaleHT_1009_BHBH);
  hist_scaleHT_ssdl_nom->Add(hist_scaleHT_ssdl_nom_BHBH);
  hist_scaleHT_ssdl_1002->Add(hist_scaleHT_ssdl_1002_BHBH);
  hist_scaleHT_ssdl_1003->Add(hist_scaleHT_ssdl_1003_BHBH);
  hist_scaleHT_ssdl_1004->Add(hist_scaleHT_ssdl_1004_BHBH);
  hist_scaleHT_ssdl_1005->Add(hist_scaleHT_ssdl_1005_BHBH);
  hist_scaleHT_ssdl_1007->Add(hist_scaleHT_ssdl_1007_BHBH);
  hist_scaleHT_ssdl_1009->Add(hist_scaleHT_ssdl_1009_BHBH);
  hist_nom->SetBinContent(1,hist_nom->GetBinContent(1)+nom_BHBH);
  hist_jesup->SetBinContent(1,hist_jesup->GetBinContent(1)+jesup_BHBH);
  hist_jesdown->SetBinContent(1,hist_jesdown->GetBinContent(1)+jesdown_BHBH);
  hist_jerup->SetBinContent(1,hist_jerup->GetBinContent(1)+jerup_BHBH);
  hist_jerdown->SetBinContent(1,hist_jerdown->GetBinContent(1)+jerdown_BHBH);
  hist_punom->SetBinContent(1,hist_punom->GetBinContent(1)+punom_BHBH);
  hist_puup->SetBinContent(1,hist_puup->GetBinContent(1)+puup_BHBH);
  hist_pudown->SetBinContent(1,hist_pudown->GetBinContent(1)+pudown_BHBH);
  hist_idup->SetBinContent(1,hist_idup->GetBinContent(1)+idup_BHBH);
  hist_iddown->SetBinContent(1,hist_iddown->GetBinContent(1)+iddown_BHBH);
  hist_trigup->SetBinContent(1,hist_trigup->GetBinContent(1)+trigup_BHBH);
  hist_trigdown->SetBinContent(1,hist_trigdown->GetBinContent(1)+trigdown_BHBH);
  hist_prefireup->SetBinContent(1,hist_prefireup->GetBinContent(1)+prefireup_BHBH);
  hist_prefiredown->SetBinContent(1,hist_prefiredown->GetBinContent(1)+prefiredown_BHBH);

  //BHTW
  hist_pdfHT->Add(hist_pdfHT_BHTW);
  pdf_hist->Add(pdf_hist_BHTW);
  hist_pdf4LHCHT_nom->Add(hist_pdf4LHCHT_nom_BHTW);
  hist_pdf4LHCHT->Add(hist_pdf4LHCHT_BHTW);
  pdf4LHC_hist->Add(pdf4LHC_hist_BHTW);
  hist_scaleHT->Add(hist_scaleHT_BHTW);
  hist_scaleHT_nom->Add(hist_scaleHT_nom_BHTW);
  hist_scaleHT_1002->Add(hist_scaleHT_1002_BHTW);
  hist_scaleHT_1003->Add(hist_scaleHT_1003_BHTW);
  hist_scaleHT_1004->Add(hist_scaleHT_1004_BHTW);
  hist_scaleHT_1005->Add(hist_scaleHT_1005_BHTW);
  hist_scaleHT_1007->Add(hist_scaleHT_1007_BHTW);
  hist_scaleHT_1009->Add(hist_scaleHT_1009_BHTW);
  hist_scaleHT_ssdl_nom->Add(hist_scaleHT_ssdl_nom_BHTW);
  hist_scaleHT_ssdl_1002->Add(hist_scaleHT_ssdl_1002_BHTW);
  hist_scaleHT_ssdl_1003->Add(hist_scaleHT_ssdl_1003_BHTW);
  hist_scaleHT_ssdl_1004->Add(hist_scaleHT_ssdl_1004_BHTW);
  hist_scaleHT_ssdl_1005->Add(hist_scaleHT_ssdl_1005_BHTW);
  hist_scaleHT_ssdl_1007->Add(hist_scaleHT_ssdl_1007_BHTW);
  hist_scaleHT_ssdl_1009->Add(hist_scaleHT_ssdl_1009_BHTW);
  hist_nom->SetBinContent(1,hist_nom->GetBinContent(1)+nom_BHTW);
  hist_jesup->SetBinContent(1,hist_jesup->GetBinContent(1)+jesup_BHTW);
  hist_jesdown->SetBinContent(1,hist_jesdown->GetBinContent(1)+jesdown_BHTW);
  hist_jerup->SetBinContent(1,hist_jerup->GetBinContent(1)+jerup_BHTW);
  hist_jerdown->SetBinContent(1,hist_jerdown->GetBinContent(1)+jerdown_BHTW);
  hist_punom->SetBinContent(1,hist_punom->GetBinContent(1)+punom_BHTW);
  hist_puup->SetBinContent(1,hist_puup->GetBinContent(1)+puup_BHTW);
  hist_pudown->SetBinContent(1,hist_pudown->GetBinContent(1)+pudown_BHTW);
  hist_idup->SetBinContent(1,hist_idup->GetBinContent(1)+idup_BHTW);
  hist_iddown->SetBinContent(1,hist_iddown->GetBinContent(1)+iddown_BHTW);
  hist_trigup->SetBinContent(1,hist_trigup->GetBinContent(1)+trigup_BHTW);
  hist_trigdown->SetBinContent(1,hist_trigdown->GetBinContent(1)+trigdown_BHTW);
  hist_prefireup->SetBinContent(1,hist_prefireup->GetBinContent(1)+prefireup_BHTW);
  hist_prefiredown->SetBinContent(1,hist_prefiredown->GetBinContent(1)+prefiredown_BHTW);

  //BZTW
  hist_pdfHT->Add(hist_pdfHT_BZTW);
  pdf_hist->Add(pdf_hist_BZTW);
  hist_pdf4LHCHT_nom->Add(hist_pdf4LHCHT_nom_BZTW);
  hist_pdf4LHCHT->Add(hist_pdf4LHCHT_BZTW);
  pdf4LHC_hist->Add(pdf4LHC_hist_BZTW);
  hist_scaleHT->Add(hist_scaleHT_BZTW);
  hist_scaleHT_nom->Add(hist_scaleHT_nom_BZTW);
  hist_scaleHT_1002->Add(hist_scaleHT_1002_BZTW);
  hist_scaleHT_1003->Add(hist_scaleHT_1003_BZTW);
  hist_scaleHT_1004->Add(hist_scaleHT_1004_BZTW);
  hist_scaleHT_1005->Add(hist_scaleHT_1005_BZTW);
  hist_scaleHT_1007->Add(hist_scaleHT_1007_BZTW);
  hist_scaleHT_1009->Add(hist_scaleHT_1009_BZTW);
  hist_scaleHT_ssdl_nom->Add(hist_scaleHT_ssdl_nom_BZTW);
  hist_scaleHT_ssdl_1002->Add(hist_scaleHT_ssdl_1002_BZTW);
  hist_scaleHT_ssdl_1003->Add(hist_scaleHT_ssdl_1003_BZTW);
  hist_scaleHT_ssdl_1004->Add(hist_scaleHT_ssdl_1004_BZTW);
  hist_scaleHT_ssdl_1005->Add(hist_scaleHT_ssdl_1005_BZTW);
  hist_scaleHT_ssdl_1007->Add(hist_scaleHT_ssdl_1007_BZTW);
  hist_scaleHT_ssdl_1009->Add(hist_scaleHT_ssdl_1009_BZTW);
  hist_nom->SetBinContent(1,hist_nom->GetBinContent(1)+nom_BZTW);
  hist_jesup->SetBinContent(1,hist_jesup->GetBinContent(1)+jesup_BZTW);
  hist_jesdown->SetBinContent(1,hist_jesdown->GetBinContent(1)+jesdown_BZTW);
  hist_jerup->SetBinContent(1,hist_jerup->GetBinContent(1)+jerup_BZTW);
  hist_jerdown->SetBinContent(1,hist_jerdown->GetBinContent(1)+jerdown_BZTW);
  hist_punom->SetBinContent(1,hist_punom->GetBinContent(1)+punom_BZTW);
  hist_puup->SetBinContent(1,hist_puup->GetBinContent(1)+puup_BZTW);
  hist_pudown->SetBinContent(1,hist_pudown->GetBinContent(1)+pudown_BZTW);
  hist_idup->SetBinContent(1,hist_idup->GetBinContent(1)+idup_BZTW);
  hist_iddown->SetBinContent(1,hist_iddown->GetBinContent(1)+iddown_BZTW);
  hist_trigup->SetBinContent(1,hist_trigup->GetBinContent(1)+trigup_BZTW);
  hist_trigdown->SetBinContent(1,hist_trigdown->GetBinContent(1)+trigdown_BZTW);
  hist_prefireup->SetBinContent(1,hist_prefireup->GetBinContent(1)+prefireup_BZTW);
  hist_prefiredown->SetBinContent(1,hist_prefiredown->GetBinContent(1)+prefiredown_BZTW);

  //BZBH
  hist_pdfHT->Add(hist_pdfHT_BZBH);
  pdf_hist->Add(pdf_hist_BZBH);
  hist_pdf4LHCHT_nom->Add(hist_pdf4LHCHT_nom_BZBH);
  hist_pdf4LHCHT->Add(hist_pdf4LHCHT_BZBH);
  pdf4LHC_hist->Add(pdf4LHC_hist_BZBH);
  hist_scaleHT->Add(hist_scaleHT_BZBH);
  hist_scaleHT_nom->Add(hist_scaleHT_nom_BZBH);
  hist_scaleHT_1002->Add(hist_scaleHT_1002_BZBH);
  hist_scaleHT_1003->Add(hist_scaleHT_1003_BZBH);
  hist_scaleHT_1004->Add(hist_scaleHT_1004_BZBH);
  hist_scaleHT_1005->Add(hist_scaleHT_1005_BZBH);
  hist_scaleHT_1007->Add(hist_scaleHT_1007_BZBH);
  hist_scaleHT_1009->Add(hist_scaleHT_1009_BZBH);
  hist_scaleHT_ssdl_nom->Add(hist_scaleHT_ssdl_nom_BZBH);
  hist_scaleHT_ssdl_1002->Add(hist_scaleHT_ssdl_1002_BZBH);
  hist_scaleHT_ssdl_1003->Add(hist_scaleHT_ssdl_1003_BZBH);
  hist_scaleHT_ssdl_1004->Add(hist_scaleHT_ssdl_1004_BZBH);
  hist_scaleHT_ssdl_1005->Add(hist_scaleHT_ssdl_1005_BZBH);
  hist_scaleHT_ssdl_1007->Add(hist_scaleHT_ssdl_1007_BZBH);
  hist_scaleHT_ssdl_1009->Add(hist_scaleHT_ssdl_1009_BZBH);
  hist_nom->SetBinContent(1,hist_nom->GetBinContent(1)+nom_BZBH);
  hist_jesup->SetBinContent(1,hist_jesup->GetBinContent(1)+jesup_BZBH);
  hist_jesdown->SetBinContent(1,hist_jesdown->GetBinContent(1)+jesdown_BZBH);
  hist_jerup->SetBinContent(1,hist_jerup->GetBinContent(1)+jerup_BZBH);
  hist_jerdown->SetBinContent(1,hist_jerdown->GetBinContent(1)+jerdown_BZBH);
  hist_punom->SetBinContent(1,hist_punom->GetBinContent(1)+punom_BZBH);
  hist_puup->SetBinContent(1,hist_puup->GetBinContent(1)+puup_BZBH);
  hist_pudown->SetBinContent(1,hist_pudown->GetBinContent(1)+pudown_BZBH);
  hist_idup->SetBinContent(1,hist_idup->GetBinContent(1)+idup_BZBH);
  hist_iddown->SetBinContent(1,hist_iddown->GetBinContent(1)+iddown_BZBH);
  hist_trigup->SetBinContent(1,hist_trigup->GetBinContent(1)+trigup_BZBH);
  hist_trigdown->SetBinContent(1,hist_trigdown->GetBinContent(1)+trigdown_BZBH);
  hist_prefireup->SetBinContent(1,hist_prefireup->GetBinContent(1)+prefireup_BZBH);
  hist_prefiredown->SetBinContent(1,hist_prefiredown->GetBinContent(1)+prefiredown_BZBH);

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
  
  f_TWTW->Close();
  f_BZBZ->Close();
  f_BHBH->Close();
  f_BHTW->Close();
  f_BZTW->Close();
  f_BZBH->Close();

}

void CombineBBsignal_integral_jess(){
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
