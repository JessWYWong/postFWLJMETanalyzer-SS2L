#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "TChain.h"

#include "../interface/TreeReader.h"
//#include "../interface/TreeReader_multipleTree.h"
#include "JetAnalyzer.cc"
#include "GenAnalyzer.cc"
#include "../interface/TreeMaker.h"
#include "../plugins/AnalyzerMacros.cc"
//#include "../plugins/EventFilterFromFile_MuonEG.cc"
//#include "../plugins/EventFilterFromFile_DoubleMu.cc"
//#include "../plugins/EventFilterFromFile_DoubleEG.cc"
//#include "../plugins/EventFilterFromFile_CSC.cc"
//#include "../plugins/EventFilterFromFile_ECALSC.cc"
//#include "../plugins/EventFilterFromVector.cc"
//#include "../plugins/ReadEventFilterFromFile.cc"

bool DEBUG_COUT = false;
bool printCutFlow = true;

std::vector<TLepton*> makeLeptons(std::vector<TMuon*>, std::vector<TElectron*>, float, std::string, std::string, bool);
std::vector<TLepton*> makeSSLeptons(std::vector<TLepton*>);
std::vector<TLepton*> makeOSLeptonsForDY(std::vector<TLepton*>);
bool checkSameSignLeptons(std::vector<TLepton*>);
bool checkOppositeSignLeptonsForDY(std::vector<TLepton*>);
int getNSSDLGen(std::vector<TGenParticle*>, int);
void printParticle(TGenParticle*);
void printParticle(TLepton*);
std::pair<bool,float> checkSecondaryZVeto(std::vector<TLepton*> leps, std::vector<TMuon*> muons, std::vector<TElectron*> electrons);
std::vector<TLepton*> pruneSSLep(std::vector<TLepton*> allLeps, std::vector<TLepton*> ssLeps);
bool sortByPt(TLepton* lep1, TLepton* lep2){return lep1->pt > lep2->pt;};

int main(int argc, char* argv[]){

//   std::string eosarea="root://cmsxrootd.fnal.gov//store/group/lpcljm/LJMet94x_2lepTT_2017datasets_2018_11_18_rizki_hadds/";
//   std::string eosdataarea="root://cmsxrootd.fnal.gov//store/group/lpcljm/LJMet94x_2lepTT_2017datasets_2018_11_18_rizki_hadds/";
//   std::string eosarea_signal="root://cmsxrootd.fnal.gov//store/group/lpcljm/LJMet94x_2lepTT_2017datasets_2018_11_18_rizki_hadds/";

//   std::string eosarea="root://cmsxrootd.fnal.gov//store/group/lpcljm/LJMet94x_2lepTT_2017datasets_2019_1_13_rizki_hadds/";
//  std::string eosarea="root://cmsxrootd.fnal.gov//store/group/lpcljm/LJMet94x_2lepTT_2017datasets_2019_2_18_rizki_hadds/"; // nonIso HT triggers live here.
//   std::string eosarea="root://cmsxrootd.fnal.gov//store/group/lpcljm/LJMet94x_2lepTT_2017datasets_2019_3_15_rizki_hadds/"; // nonIso HT triggers live here with el ID V2.
  //std::string eosarea="root://cmseos.fnal.gov//store/user/lpcljm/FWLJMET102X_2lep2017_062719_hadds/"; // FWLJMET era.
  std::string eosarea="root://cmseos.fnal.gov//store/user/lpcljm/FWLJMET102X_2lep2018_wywong_082020_hadds/";

  std::string eosdataarea=eosarea;
  std::string eosarea_signal=eosarea;

  if(argc<5) return 0; // edited by rizki
  std::string argv1=argv[1];
  std::string elID = argv[2];
  std::string muID = argv[3];
  std::string era = argv[4];
  std::string sigDecay = argv[5]; //added by rizki

  //CHOOSE TRIGGERS:
  bool isoTrig = true; // added by rizki
  bool nonIso_HTtrig =false; // added by rizki

  //CHOOSE UNC:
  bool elPRunc = false;
  bool muPRunc = false;
  if (argc>6){
    int PRUncMode = std::atoi(argv[6]);
    if (PRUncMode > 0){
      std::cout << "UNCERTAINTY!!!! Unity PR "<<std::endl;
      if(PRUncMode == 1) {elPRunc=true; std::cout << "for electrons \n" <<std::endl;}
      else if (PRUncMode == 2){muPRunc = true; std::cout << "for muons \n" <<std::endl;}
    }
  }

  std::cout << "\nWhich triggers: " << std::endl;
  std::cout <<  "\tisoTrig "<< isoTrig<< "\n"<<std::endl;
  std::cout <<  "\tnonIso_HTtrig "<< nonIso_HTtrig<< "\n"<<std::endl;

  typedef std::map<std::string,std::string> StringMap;

  StringMap bg_samples, sig_samples,data_samples;
  std::vector<std::string> powheg_samples{"WZ","ZZ","TTH"};
  std::vector<std::string> madgraph_samples{"WpWp","WJets"};
  bg_samples["TTW"]=eosarea+"TTW.root";
  bg_samples["TTZ"]=eosarea+"TTZ.root";
  bg_samples["TTH"]=eosarea+"TTH.root";
  bg_samples["TTTT"]=eosarea+"TTTT.root";
  bg_samples["WWW"]=eosarea+"WWW.root";
  bg_samples["WWZ"]=eosarea+"WWZ.root";
  bg_samples["WZZ"]=eosarea+"WZZ.root";
  bg_samples["ZZZ"]=eosarea+"ZZZ.root";
  bg_samples["WZ"]=eosarea+"WZ.root";
  bg_samples["ZZ"]=eosarea+"ZZ.root";
  bg_samples["WpWp"]=eosarea+"WpWp.root";

  //TT signal - rizki
  //sig_samples["TprimeTprime_M-800"]=eosarea_signal+"TprimeTprime_M-800.root";
  sig_samples["TprimeTprime_M-900"]=eosarea_signal+"TpTp900.root";
  sig_samples["TprimeTprime_M-1000"]=eosarea_signal+"TpTp1000.root";
  sig_samples["TprimeTprime_M-1100"]=eosarea_signal+"TpTp1100.root";
  sig_samples["TprimeTprime_M-1200"]=eosarea_signal+"TpTp1200.root";
  sig_samples["TprimeTprime_M-1300"]=eosarea_signal+"TpTp1300.root";
  sig_samples["TprimeTprime_M-1400"]=eosarea_signal+"TpTp1400.root";
  sig_samples["TprimeTprime_M-1500"]=eosarea_signal+"TpTp1500.root";
  sig_samples["TprimeTprime_M-1600"]=eosarea_signal+"TpTp1600.root";
  sig_samples["TprimeTprime_M-1700"]=eosarea_signal+"TpTp1700.root";
  sig_samples["TprimeTprime_M-1800"]=eosarea_signal+"TpTp1800.root";

  //BB signal - rizki
  //sig_samples["BprimeBprime_M-800"]=eosarea_signal+"BprimeBprime_M-800.root";
  sig_samples["BprimeBprime_M-900"]=eosarea_signal+"BpBp900.root";
  sig_samples["BprimeBprime_M-1000"]=eosarea_signal+"BpBp1000.root";
  sig_samples["BprimeBprime_M-1100"]=eosarea_signal+"BpBp1100.root";
  sig_samples["BprimeBprime_M-1200"]=eosarea_signal+"BpBp1200.root";
  sig_samples["BprimeBprime_M-1300"]=eosarea_signal+"BpBp1300.root";
  sig_samples["BprimeBprime_M-1400"]=eosarea_signal+"BpBp1400.root";
  sig_samples["BprimeBprime_M-1500"]=eosarea_signal+"BpBp1500.root";
  sig_samples["BprimeBprime_M-1600"]=eosarea_signal+"BpBp1600.root";
  sig_samples["BprimeBprime_M-1700"]=eosarea_signal+"BpBp1700.root";
  sig_samples["BprimeBprime_M-1800"]=eosarea_signal+"BpBp1800.root";


  //data samples
  data_samples["DataElEl2018A"]=eosdataarea+"EGammaRun2018A.root";
  data_samples["DataElMu2018A"]=eosdataarea+"MuonEGRun2018A.root";
  data_samples["DataMuMu2018A"]=eosdataarea+"DoubleMuonRun2018A.root";
  data_samples["DataElEl2018B"]=eosdataarea+"EGammaRun2018B.root";
  data_samples["DataElMu2018B"]=eosdataarea+"MuonEGRun2018B.root";
  data_samples["DataMuMu2018B"]=eosdataarea+"DoubleMuonRun2018B.root";
  data_samples["DataElEl2018C"]=eosdataarea+"EGammaRun2018C.root";
  data_samples["DataElMu2018C"]=eosdataarea+"MuonEGRun2018C.root";
  data_samples["DataMuMu2018C"]=eosdataarea+"DoubleMuonRun2018C.root";
  data_samples["DataElEl2018D"]=eosdataarea+"EGammaRun2018D.root";
  data_samples["DataElMu2018D"]=eosdataarea+"MuonEGRun2018D.root";
  data_samples["DataMuMu2018D"]=eosdataarea+"DoubleMuonRun2018D.root";
//   data_samples["NonPromptData"]=eosdataarea+"All_2017.root";
  data_samples["NonPromptDataElEl2018A"]=eosdataarea+"EGammaRun2018A.root";
  data_samples["NonPromptDataElMu2018A"]=eosdataarea+"MuonEGRun2018A.root";
  data_samples["NonPromptDataMuMu2018A"]=eosdataarea+"DoubleMuonRun2018A.root";
  data_samples["NonPromptDataElEl2018B"]=eosdataarea+"EGammaRun2018B.root";
  data_samples["NonPromptDataElMu2018B"]=eosdataarea+"MuonEGRun2018B.root";
  data_samples["NonPromptDataMuMu2018B"]=eosdataarea+"DoubleMuonRun2018B.root";
  data_samples["NonPromptDataElEl2018C"]=eosdataarea+"EGammaRun2018C.root";
  data_samples["NonPromptDataElMu2018C"]=eosdataarea+"MuonEGRun2018C.root";
  data_samples["NonPromptDataMuMu2018C"]=eosdataarea+"DoubleMuonRun2018C.root";
  data_samples["NonPromptDataElEl2018D"]=eosdataarea+"EGammaRun2018D.root";
  data_samples["NonPromptDataElMu2018D"]=eosdataarea+"MuonEGRun2018D.root";
  data_samples["NonPromptDataMuMu2018D"]=eosdataarea+"DoubleMuonRun2018D.root";

//   data_samples["ChargeMisID"]=eosdataarea+"All_2017.root";
  data_samples["ChargeMisIDElEl2018A"]=eosdataarea+"EGammaRun2018A.root";
  data_samples["ChargeMisIDElMu2018A"]=eosdataarea+"MuonEGRun2018A.root";
  data_samples["ChargeMisIDElEl2018B"]=eosdataarea+"EGammaRun2018B.root";
  data_samples["ChargeMisIDElMu2018B"]=eosdataarea+"MuonEGRun2018B.root";
  data_samples["ChargeMisIDElEl2018C"]=eosdataarea+"EGammaRun2018C.root";
  data_samples["ChargeMisIDElMu2018C"]=eosdataarea+"MuonEGRun2018C.root";
  data_samples["ChargeMisIDElEl2018D"]=eosdataarea+"EGammaRun2018D.root";
  data_samples["ChargeMisIDElMu2018D"]=eosdataarea+"MuonEGRun2018D.root";


  bool signal=false;
  bool bg_mc=false;
  bool bg_np = false; //bool to run  using tight loose method to get non prompt background
  //bool bg_dd=false;
  bool data=false;

  //check usage
  bool correctusage=true;
  if(argc!=7 || ( bg_samples.find(argv[1])==bg_samples.end() && sig_samples.find(argv[1])==sig_samples.end() && data_samples.find(argv[1])==data_samples.end() && argv1!="NonPromptMC") ) correctusage=false; //edited by rizki
  if(!correctusage){
    std::cout<<"Need to specify electron and muon ID as well as supply argument for sample name of one of the following"<<std::endl;
    std::cout<<std::endl<<"********** Background *********"<<std::endl;
    for(std::map<std::string,std::string>::iterator iter=bg_samples.begin(); iter!= bg_samples.end(); iter++){
      std::cout<<iter->first<<std::endl;
    }
    std::cout<<std::endl<<"********** Signal *********"<<std::endl;
    for(std::map<std::string,std::string>::iterator iter=sig_samples.begin(); iter!= sig_samples.end(); iter++){
      std::cout<<iter->first<<std::endl;
    }
    std::cout<<"********* OR Specify \'NonPromptMC\' to run over MC using Data-Driven Method"<<std::endl;
    std::cout<<" Examples:\n       ./LJMETanalyzer.o TTJets CBTight CBMed 2016B-D \n       ./LJMETanalyzer.o NonPromptMC MVATight CBTight 2016E-H"<<std::endl;
    return 0;
  }

  if(bg_samples.find(argv[1])!=bg_samples.end()) bg_mc=true;
  if(sig_samples.find(argv[1])!=sig_samples.end()) signal=true;
  if(data_samples.find(argv[1])!=data_samples.end()) data=true;
  std::string sample = argv[1];
  if(sample.find("NonPrompt")!=std::string::npos) bg_np = true;

  //check BR for signal - added by rizki
  if(signal && (argc!=7 ||
                sigDecay=="" ||
                !(sigDecay=="BWBW" || sigDecay=="THBW" || sigDecay=="TZBW"|| sigDecay=="TZTZ"|| sigDecay=="TZTH"|| sigDecay=="THTH" ||
                  sigDecay=="TWTW" || sigDecay=="BHTW" || sigDecay=="BZTW"|| sigDecay=="BZBZ"|| sigDecay=="BZBH"|| sigDecay=="BHBH")
                )
     ){
    std::cout << "argc =  "<< argc << std::endl;
    std::cout << "Entered sigDecay: "<< sigDecay << std::endl;
    std::cout << "Need to enter TT/BB decay: BWBW, THBW, TZBW, TZTZ, TZTH, or THTH, etc. "<< std::endl;
    return 0;
  }

  //make TreeReader
  std::string filename;
  if(bg_mc) filename = bg_samples.find(argv[1])->second;
  if(signal) filename = sig_samples.find(argv[1])->second;
  if(data) filename = data_samples.find(argv[1])->second;
  if(bg_np)  std::cout<<"running NonPrompt on  file: "<<filename<<std::endl;
  else{  //edited by rizki
        std::cout<<"running RegularMethod  on  file: "<<filename;
        if(signal) std::cout << " -- signal Decay : " << sigDecay << std::endl;
        std::cout << std::endl;
  }
  std::string sampleName = argv[1];
  if(signal){
        std::string massStr = sampleName.substr(sampleName.find("M-")+2,sampleName.length()-sampleName.find("M-")-2);
        if(sampleName.find("Tprime") != std::string::npos) sampleName = "TpTp"+massStr;
        else if (sampleName.find("Bprime") != std::string::npos) sampleName = "BpBp"+massStr;
        else std::cout<<"WARNING!!! Incorrect signal sample name." <<std::endl;
  }
  if(DEBUG_COUT) std::cout<<"sample name : " << sampleName << std::endl;

  //make output file
  std::stringstream outnamestream;
  if(signal) {
    if (elPRunc) outnamestream<<argv[1]<<"_"<<sigDecay<<"_Mu"<<muID<<"_El"<<elID<<"_"<<era<<"_UnityElPR.root"; //added by Jess
    else if (muPRunc) outnamestream<<argv[1]<<"_"<<sigDecay<<"_Mu"<<muID<<"_El"<<elID<<"_"<<era<<"_UnityMuPR.root"; //added by Jess
    else outnamestream<<argv[1]<<"_"<<sigDecay<<"_Mu"<<muID<<"_El"<<elID<<"_"<<era<<".root"; //edited by rizki
  }
  else {
    if (elPRunc) outnamestream<<argv[1]<<"_Mu"<<muID<<"_El"<<elID<<"_"<<era<<"_UnityElPR.root"; //added by Jess
    else if (muPRunc) outnamestream<<argv[1]<<"_Mu"<<muID<<"_El"<<elID<<"_"<<era<<"_UnityMuPR.root"; //added by Jess
    else outnamestream<<argv[1]<<"_Mu"<<muID<<"_El"<<elID<<"_"<<era<<".root";
  }

  std::string outname = outnamestream.str();
  TFile* fsig = new TFile(outname.c_str(),"RECREATE");

  TreeMaker* tm_ss = new TreeMaker();
  tm_ss->InitTree("tEvts_ss");
  TreeMaker* tm_ssdl = new TreeMaker();
  tm_ssdl->InitTree("tEvts_ssdl");
  TreeMaker* tm_sZVeto = new TreeMaker();
  tm_sZVeto->InitTree("tEvts_sZVeto");
  TreeMaker* tm_DilepMassCut = new TreeMaker();
  tm_DilepMassCut->InitTree("tEvts_DilepMassCut");

  TreeMaker* tm_sZVeto_JECup = new TreeMaker();
  tm_sZVeto_JECup->InitTree("tEvts_sZVeto_JECup");
  TreeMaker* tm_sZVeto_JECdn  = new TreeMaker();
  tm_sZVeto_JECdn->InitTree("tEvts_sZVeto_JECdn");
  TreeMaker* tm_sZVeto_JERup = new TreeMaker();
  tm_sZVeto_JERup->InitTree("tEvts_sZVeto_JERup");
  TreeMaker* tm_sZVeto_JERdn = new TreeMaker();
  tm_sZVeto_JERdn->InitTree("tEvts_sZVeto_JERdn");

  TreeReader* tr;
  tr = new TreeReader(filename.c_str(),"ljmet/ljmet",!data,true);
  /*
  TreeReader_multipleTree* tr;
  TString treename_JECup = "ljmet_JECup/ljmet_JECup";
  TString treename_JECdown = "ljmet_JECdown/ljmet_JECdown";
  TString treename_JERup = "ljmet_JERup/ljmet_JERup";
  TString treename_JERdown = "ljmet_JERdown/ljmet_JERdown";
  if(data) {
    treename_JECup = "ljmet/ljmet";
    treename_JECdown = "ljmet/ljmet";
    treename_JERup = "ljmet/ljmet";
    treename_JERdown = "ljmet/ljmet";
  }
  tr = new TreeReader_multipleTree(filename.c_str(),"ljmet/ljmet",treename_JECup, treename_JECdown, treename_JERup, treename_JERdown,!data,true);
  */
  TTree* t=tr->tree;

  //histogram for cutflow
  //TH1F* h_MuCutFlow = new TH1F("h_MuCutFlow","Cut Flow For Muons",13,0,13);

  //histogram for trigger studies -- NEEDS UPDATING
  TH1F* h_DoubleEle33Num = new TH1F("h_DoubleEle33Num","",60,0,600);
  TH1F* h_DoubleEle33_MWNum = new TH1F("h_DoubleEle33_MWNum","",60,0,600);
  TH1F* h_Ele27WP85Num = new TH1F("h_Ele27WP85Num","",60,0,600);
  TH1F* h_Mu30TkMu11Num = new TH1F("h_Mu30TkMu11Num","",60,0,600);
  TH1F* h_Mu40Num = new TH1F("h_Mu40Num","",60,0,600);
  TH1F* h_IsoTkMu24Num = new TH1F("h_IsoTkMu24Num","",60,0,600);
  TH1F* h_DoubleMu33NoFiltersNoVtxNum = new TH1F("h_DoubleMu33NoFiltersNoVtxNum","",60,0,600);
  TH1F* h_Mu23Ele12Num = new TH1F("h_Mu23Ele12Num","",60,0,600);
  TH1F* h_Mu8Ele23Num = new TH1F("h_Mu8Ele23Num","",60,0,600);
  TH1F* h_PFHT900Num = new TH1F("h_PFHT900Num","",60,0,600);


  TH1F* h_DoubleEle33Den = new TH1F("h_DoubleEle33Den","",60,0,600);
  TH1F* h_DoubleEle33_MWDen = new TH1F("h_DoubleEle33_MWDen","",60,0,600);
  TH1F* h_Ele27WP85Den = new TH1F("h_Ele27WP85Den","",60,0,600);
  TH1F* h_Mu30TkMu11Den = new TH1F("h_Mu30TkMu11Den","",60,0,600);
  TH1F* h_Mu40Den = new TH1F("h_Mu40Den","",60,0,600);
  TH1F* h_IsoTkMu24Den = new TH1F("h_IsoTkMu24Den","",60,0,600);
  TH1F* h_DoubleMu33NoFiltersNoVtxDen = new TH1F("h_DoubleMu33NoFiltersNoVtxDen","",60,0,600);
  TH1F* h_Mu23Ele12Den = new TH1F("h_Mu23Ele12Den","",60,0,600);
  TH1F* h_Mu8Ele23Den = new TH1F("h_Mu8Ele23Den","",60,0,600);
  TH1F* h_PFHT900Den = new TH1F("h_PFHT900Den","",60,0,600);


  //get vector of histograms for nonprompt
  std::vector<TH1F*> npHistos_all  = getNPHistos(-1);
  std::vector<TH1F*> npHistos_ee   = getNPHistos(0);
  std::vector<TH1F*> npHistos_emu  = getNPHistos(1);
  std::vector<TH1F*> npHistos_mumu = getNPHistos(2);

  //histogram for pdf uncertainties
  TH2F* hist_pdfHT = new TH2F("hist_pdfHT","PDF Weights",500,0,5,30,0,3000);
  TH2F* hist_pdf4LHCHT_nom = new TH2F("hist_pdf4LHCHT_nom","PDF4LHC nominal Weights",500,0,5,30,0,3000);
  TH2F* hist_pdf4LHCHT = new TH2F("hist_pdf4LHCHT","PDF4LHC Weights",500,0,5,30,0,3000);
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

  //added by rizki
  TH2F* pdf_hist = new TH2F("pdf_hist","PDF Weights",100,0,100,30,0,3000);  // just need pdf vars with yields
  TH2F* pdf4LHC_hist = new TH2F("pdf4LHC_hist","PDF Weights",31,0,31,30,0,3000);

  // Load Charge MisID eta weights in:
  std::cout << "Loading CMID file ..." << std::endl;
  std::string cmidFilename = "../scripts/ChargeMisID/ChargeMisID_Data_All_Electrons_"+elID+"_corrected.root";
  //for the moment there is no new measurement for MVA2017 so use 2016.
  //if(elID=="MVA2017TightV2RC")cmidFilename = "../scripts/ChargeMisID/ChargeMisID_Data_All_Electrons_MVA2017TightV2IsoRC_corrected.root";
  //if(argv1=="DataElEl2017F_TEST" && elID=="MVA2017TightV2RC") cmidFilename = "../scripts/ChargeMisID/ChargeMisID_Data_2017F_Electrons_MVA2017TightV2IsoRC_corrected.root";
  if(elID=="MVA2017TightV2IsoTightRC")cmidFilename = "../scripts/ChargeMisID/ChargeMisID_likelihood_Data_2018All_Electrons_MVA2017TightV2IsoTightRC.root";
  TFile* eWfile = new TFile(cmidFilename.c_str());
  std::vector<float> etaWeights_lpt = getEtaWeights_lpt(eWfile);
  std::vector<float> etaWeights_hpt = getEtaWeights_hpt(eWfile);
  std::vector<float> etaWeights_hhpt = getEtaWeights_hhpt(eWfile);
  std::cout << "Done: Loading CMID file." << std::endl;



  //load pileup hist
  //std::cout << "Loading PU files ..." << std::endl;
  //TFile* fpu = new TFile("PileupWeights.root");
  //TH1F* hpu = (TH1F*) fpu->Get("h_weights");
  //load pileup hist - down
  //TFile* fpuDown = new TFile("PileupWeights_Down.root");
  //TH1F* hpuDown = (TH1F*) fpuDown->Get("h_weights");
  //load pileup hist
  //TFile* fpuUp = new TFile("PileupWeights_Up.root");
  //TH1F* hpuUp = (TH1F*) fpuUp->Get("h_weights");
  //std::cout << "Done: Loading PU files." << std::endl;


  //get prompt rate according to ID * constant for muons ****
  float muPromptRate;
  std::vector<float> muPromptRates;
  if (muPRunc){
    for(int i=0;i<14;i++) muPromptRates.push_back(1.0);
  }
  else if(muID=="CBTight") muPromptRate=0.940;
  else if(muID=="CBTightMiniIso") muPromptRate=0.956;
//   else if(muID=="CBTightMiniIsoTight") muPromptRate=0.943;
  else if(muID=="CBTightMiniIsoTight"){
  //Rizki's measurement for 2017 data: (April 30, 2019)  
	// MUON
	// 
	// Trigger: nonIsoHTTrigs
	// working on era: All
	// 
	// Opening file:/PromptRate_Data_All_Muons_CBTightMiniIso_SortByPhi.root
	// eta bin: -2.4 to -1.566, PR: 0.946022+/-0.000261778
	// eta bin: -1.566 to -1.4442, PR: 0.952213+/-0.000121532
	// eta bin: -1.4442 to -0.8, PR: 0.925424+/-0.00023006
	// eta bin: -0.8 to -0.4, PR: 0.935103+/-0.000159053
	// eta bin: -0.4 to 0.4, PR: 0.931663+/-0.000125854
	// eta bin: 0.4 to 0.8, PR: 0.934086+/-0.000160126
	// eta bin: 0.8 to 1.4442, PR: 0.921252+/-0.000235986
	// eta bin: 1.4442 to 1.566, PR: 0.952797+/-0.000120283
	// eta bin: 1.566 to 2.4, PR: 0.952706+/-0.00024421
	// 
	// pt bin: 20-30, PR: 0.894022+/-0.000183225
	// pt bin: 30-40, PR: 0.930789+/-0.000102873
	// pt bin: 40-50, PR: 0.954045+/-7.76882e-05
	// pt bin: 50-60, PR: 0.962158+/-0.000143523
	// pt bin: 60-70, PR: 0.967671+/-0.000233195
	// pt bin: 70-80, PR: 0.96884+/-0.00033994
	// pt bin: 80-90, PR: 0.969207+/-0.000469262
	// pt bin: 90-100, PR: 0.969129+/-0.000632858
	// pt bin: 100-125, PR: 0.967569+/-0.000638519
	// pt bin: 125-150, PR: 0.964728+/-0.00112312
	// pt bin: 150-200, PR: 0.957498+/-0.00147216
	// pt bin: 200-300, PR: 0.958634+/-0.00228617
	// pt bin: 300-400, PR: 0.951185+/-0.00605169
	// pt bin: 400-500, PR: 0.932773+/-0.0147285
	// pt bin: 500-1000, PR: 0.929078+/-0.025443
/*
    muPromptRates.push_back(0.894);
    muPromptRates.push_back(0.930);
    muPromptRates.push_back(0.954);
    muPromptRates.push_back(0.962);
    muPromptRates.push_back(0.968);
    muPromptRates.push_back(0.969);
    muPromptRates.push_back(0.969);
    muPromptRates.push_back(0.969);
    muPromptRates.push_back(0.968);
    muPromptRates.push_back(0.965);
    muPromptRates.push_back(0.957);
    muPromptRates.push_back(0.959);
    muPromptRates.push_back(0.951);
    muPromptRates.push_back(0.932);
    muPromptRates.push_back(0.929);
*/
    //added by Jess 11-27-2020
    //Opening file: ../scripts/PromptRate/Outputs_2018data_isoTrig_forTrilep/PromptRate_Graph_All_MVA2017TightV2IsoTightRC_isoTrig_forTrilep.root
    //pt bin: 30-40, PR: 0.901128+/-0.000106491
    //pt bin: 40-50, PR: 0.941808+/-7.60221e-05
    //pt bin: 50-60, PR: 0.959551+/-0.000127237
    //pt bin: 60-70, PR: 0.969682+/-0.000207045
    //pt bin: 70-80, PR: 0.972126+/-0.000342536
    //pt bin: 80-90, PR: 0.972367+/-0.000495816
    //pt bin: 90-100, PR: 0.971629+/-0.000654343
    //pt bin: 100-125, PR: 0.972171+/-0.000582344
    //pt bin: 125-150, PR: 0.972471+/-0.000886017
    //pt bin: 150-200, PR: 0.967743+/-0.00110169
    //pt bin: 200-300, PR: 0.964977+/-0.00172115
    //pt bin: 300-400, PR: 0.951493+/-0.00487995
    //pt bin: 400-500, PR: 0.961382+/-0.00978131
    //pt bin: 500-1000, PR: 0.935622+/-0.0183726
    //mean = 0.958154 ; RMS = 0.0195215
    muPromptRates.push_back(0.901);
    muPromptRates.push_back(0.942);
    muPromptRates.push_back(0.960);
    muPromptRates.push_back(0.970);
    muPromptRates.push_back(0.972);
    muPromptRates.push_back(0.972);
    muPromptRates.push_back(0.972);
    muPromptRates.push_back(0.972);
    muPromptRates.push_back(0.972);
    muPromptRates.push_back(0.968);
    muPromptRates.push_back(0.965);
    muPromptRates.push_back(0.951);
    muPromptRates.push_back(0.961);
    muPromptRates.push_back(0.936);
  }
  else{ std::cout<<"Didn't pick a valid muon ID. Exiting..."<<std::endl; return 0;}

  //get electron prompt rate
  std::vector<float> elPromptRates;
  if (elPRunc){
    for(int i=0;i<14;i++) elPromptRates.push_back(1.0);
  }
  else if(elID=="CBTight" || elID=="CBTightRC") elPromptRates.push_back(0.7259);
  else if(elID=="MVATightCC" || elID=="MVATightRC") elPromptRates.push_back(0.839);
  else if(elID=="MVA2016TightCC" || elID=="MVA2016TightRC"){
    elPromptRates.push_back(0.904);
    elPromptRates.push_back(0.928);
    elPromptRates.push_back(0.934);
    elPromptRates.push_back(0.942);
    elPromptRates.push_back(0.947);
    elPromptRates.push_back(0.953);
    elPromptRates.push_back(0.955);
    elPromptRates.push_back(0.948);
    elPromptRates.push_back(0.951);
    elPromptRates.push_back(0.946);
    elPromptRates.push_back(0.935);
    elPromptRates.push_back(0.920);
    elPromptRates.push_back(0.902);
    elPromptRates.push_back(0.800);
  }
  else if(elID=="MVA2017TightRC"){ // NOTE: this is a copy of PR from MVA2016TightRC !!! Need to update! Nov25,2018.
    elPromptRates.push_back(0.904);
    elPromptRates.push_back(0.928);
    elPromptRates.push_back(0.934);
    elPromptRates.push_back(0.942);
    elPromptRates.push_back(0.947);
    elPromptRates.push_back(0.953);
    elPromptRates.push_back(0.955);
    elPromptRates.push_back(0.948);
    elPromptRates.push_back(0.951);
    elPromptRates.push_back(0.946);
    elPromptRates.push_back(0.935);
    elPromptRates.push_back(0.920);
    elPromptRates.push_back(0.902);
    elPromptRates.push_back(0.800);
  }
  else if(elID=="MVA2017TightV2RC"){
  //Rizki's measurement for 2017 data: (April 30, 2019)
	// pt bin: 20-30, PR: 0.807545+/-0.000186806
	// pt bin: 30-40, PR: 0.851842+/-0.000150336
	// pt bin: 40-50, PR: 0.872654+/-0.000292136
	// pt bin: 50-60, PR: 0.879749+/-0.000558864
	// pt bin: 60-70, PR: 0.882003+/-0.000963929
	// pt bin: 70-80, PR: 0.882304+/-0.00141193
	// pt bin: 80-90, PR: 0.883859+/-0.00183804
	// pt bin: 90-100, PR: 0.868217+/-0.00169931
	// pt bin: 100-125, PR: 0.85802+/-0.00267284
	// pt bin: 125-150, PR: 0.854862+/-0.00310203
	// pt bin: 150-200, PR: 0.860461+/-0.00445062
	// pt bin: 200-300, PR: 0.831768+/-0.0113596
	// pt bin: 300-400, PR: 0.850153+/-0.0212822
	// pt bin: 400-500, PR: 0.791139+/-0.0354723
	// pt bin: 500-1000, PR: 1.18034e-316+/--1

    //elPromptRates.push_back(0.808);
    elPromptRates.push_back(0.852);
    elPromptRates.push_back(0.873);
    elPromptRates.push_back(0.880);
    elPromptRates.push_back(0.882);
    elPromptRates.push_back(0.882);
    elPromptRates.push_back(0.884);
    elPromptRates.push_back(0.868);
    elPromptRates.push_back(0.858);
    elPromptRates.push_back(0.855);
    elPromptRates.push_back(0.860);
    elPromptRates.push_back(0.832);
    elPromptRates.push_back(0.850);
    elPromptRates.push_back(0.80);
    elPromptRates.push_back(0.80);

  }
  else if(elID=="MVA2017TightV2IsoTightRC"){
    //added by Jess 10-13-2020
    //Opening file: ../scripts/PromptRate/Outputs_2018data_isoTrig_forTrilep/PromptRate_Graph_All_MVA2017TightV2IsoTightRC_isoTrig_forTrilep.root
    //pt bin: 30-40, PR: 0.785024+/-0.00018126
    //pt bin: 40-50, PR: 0.840857+/-0.000150694
    //pt bin: 50-60, PR: 0.870809+/-0.000273156
    //pt bin: 60-70, PR: 0.883434+/-0.000488534
    //pt bin: 70-80, PR: 0.888326+/-0.000813609
    //pt bin: 80-90, PR: 0.886918+/-0.00117244
    //pt bin: 90-100, PR: 0.889993+/-0.00149505
    //pt bin: 100-125, PR: 0.872105+/-0.00138833
    //pt bin: 125-150, PR: 0.868276+/-0.00212473
    //pt bin: 150-200, PR: 0.862877+/-0.00242515
    //pt bin: 200-300, PR: 0.852351+/-0.0037382
    //pt bin: 300-400, PR: 0.836686+/-0.00928862
    //pt bin: 400-500, PR: 0.815851+/-0.0198796
    //pt bin: 500-1000, PR: 0.826733+/-0.029112
    //mean = 0.855731 ; RMS = 0.0301265
    
    elPromptRates.push_back(0.785);
    elPromptRates.push_back(0.841);
    elPromptRates.push_back(0.871);
    elPromptRates.push_back(0.883);
    elPromptRates.push_back(0.888);
    elPromptRates.push_back(0.887);
    elPromptRates.push_back(0.890);
    elPromptRates.push_back(0.872);
    elPromptRates.push_back(0.868);
    elPromptRates.push_back(0.863);
    elPromptRates.push_back(0.852);
    elPromptRates.push_back(0.837);
    elPromptRates.push_back(0.816);
    elPromptRates.push_back(0.827);
  }
  else if(elID=="MVATightMedIsoRC") elPromptRates.push_back(0.859);
  else if(elID=="MVATightNew" || elID=="MVATightNewRC") elPromptRates.push_back(0.8618);
  else if(elID=="SUSYTight" || elID=="SUSYTightRC") elPromptRates.push_back(0.7956);
  else{std::cout<<"Didn't pick a valid electron ID. Exiting..."<<std::endl; return 0;}

  //get fake rate according to ID
  std::vector<float> muFakeRates;
  if(muID=="CBTight") muFakeRates.push_back(0.346);
  else if(muID=="CBTightMiniIso") muFakeRates.push_back(0.429);
  else if(muID=="CBTightMiniIsoTight"){
//     muFakeRates.push_back(0.312);
//     muFakeRates.push_back(0.257);
//     muFakeRates.push_back(0.214);
//     muFakeRates.push_back(0.190);
//     muFakeRates.push_back(0.187);
//     muFakeRates.push_back(0.177);
//     muFakeRates.push_back(0.185);
//     muFakeRates.push_back(0.199);
//     muFakeRates.push_back(0.256);
//     muFakeRates.push_back(0.319);
/*
    //Rizki's measurement for 2017 data: (April 30, 2019) from plots_April17-2019_isoTrig_forTrilep
	// Mu:
	// eta bin: -2.4 to -2.1, FR: 0.641026 +/- 0.0884624
	// eta bin: -2.1 to -1.2, FR: 0.518519 +/- 0.042203
	// eta bin: -1.2 to -0.9, FR: 0.603448 +/- 0.0721912
	// eta bin: -0.9 to -0.4, FR: 0.552632 +/- 0.0507157
	// eta bin: -0.4 to 0.4, FR: 0.519231 +/- 0.0430588
	// eta bin: 0.4 to 0.9, FR: 0.544554 +/- 0.0542126
	// eta bin: 0.9 to 1.2, FR: 0.508772 +/- 0.0742506
	// eta bin: 1.2 to 2.1, FR: 0.522581 +/- 0.0431957
	// eta bin: 2.1 to 2.4, FR: 0.527778 +/- 0.0955496    
    muFakeRates.push_back(0.641);
    muFakeRates.push_back(0.519);
    muFakeRates.push_back(0.603);
    muFakeRates.push_back(0.552);
    muFakeRates.push_back(0.519);
    muFakeRates.push_back(0.545);
    muFakeRates.push_back(0.508);
    muFakeRates.push_back(0.523);
    muFakeRates.push_back(0.528);
*/
    //Using singleLep data. Mu17+Mu20 triggers. Opening file "../scripts/FakeRate/plots2018_082020_FakeRate_isoTrig_forTrilep_Mu20/MuonFakeRate-vs-Pt_2018All_CBTightMiniIsoTight_isoTrig_forTrilep.root"
    //eta bin: -2.4 to -2.1, FR: 0.405021 +/- 0.0146131
    //eta bin: -2.1 to -1.2, FR: 0.334534 +/- 0.00659194
    //eta bin: -1.2 to -0.9, FR: 0.269733 +/- 0.0108583
    //eta bin: -0.9 to -0.4, FR: 0.257283 +/- 0.00836451
    //eta bin: -0.4 to 0.4, FR: 0.266817 +/- 0.00675769
    //eta bin: 0.4 to 0.9, FR: 0.275318 +/- 0.00844677
    //eta bin: 0.9 to 1.2, FR: 0.288361 +/- 0.011188
    //eta bin: 1.2 to 2.1, FR: 0.340944 +/- 0.00663306
    //eta bin: 2.1 to 2.4, FR: 0.399832 +/- 0.0146273
    //muFakeRates.push_back(0.405);
    //muFakeRates.push_back(0.335);
    //muFakeRates.push_back(0.270);
    //muFakeRates.push_back(0.257);
    //muFakeRates.push_back(0.267);
    //muFakeRates.push_back(0.275);
    //muFakeRates.push_back(0.288);
    //muFakeRates.push_back(0.341);
    //muFakeRates.push_back(0.400);

    //Using singleLep data. Mu17 triggers. Opening file "../scripts/FakeRate/plots2018_082020_FakeRate_isoTrig_forTrilep_AllPt_OneTrig/MuonFakeRate-vs-Pt_2018All_CBTightMiniIsoTight_isoTrig_forTrilep.root"
    //eta bin: -2.4 to -2.1, FR: 0.361646 +/- 0.0146501
    //eta bin: -2.1 to -1.2, FR: 0.305113 +/- 0.00676984
    //eta bin: -1.2 to -0.9, FR: 0.247365 +/- 0.0110519
    //eta bin: -0.9 to -0.4, FR: 0.220794 +/- 0.00825365
    //eta bin: -0.4 to 0.4, FR: 0.234553 +/- 0.00671655
    //eta bin: 0.4 to 0.9, FR: 0.236455 +/- 0.00855168
    //eta bin: 0.9 to 1.2, FR: 0.244416 +/- 0.0111735
    //eta bin: 1.2 to 2.1, FR: 0.305631 +/- 0.00676941
    //eta bin: 2.1 to 2.4, FR: 0.376316 +/- 0.0147811
    muFakeRates.push_back(0.362);
    muFakeRates.push_back(0.305);
    muFakeRates.push_back(0.247);
    muFakeRates.push_back(0.221);
    muFakeRates.push_back(0.235);
    muFakeRates.push_back(0.236);
    muFakeRates.push_back(0.244);
    muFakeRates.push_back(0.306);
    muFakeRates.push_back(0.376);

  }
  else{ std::cout<<"Didn't pick a valid muon ID. Exiting..."<<std::endl; return 0;}

  //get electron fake rate
  std::vector<float> elFakeRates;
  if(elID=="CBTight" || elID=="CBTightRC") elFakeRates.push_back(0.43);
  else if(elID=="MVATightCC" || elID=="MVATightRC") elFakeRates.push_back(0.204);
  else if(elID=="MVA2016TightCC" || elID=="MVA2016TightRC"){
    elFakeRates.push_back(0.232);
    elFakeRates.push_back(0.0);//ee-eb gap
    elFakeRates.push_back(0.202);
    elFakeRates.push_back(0.185);
    elFakeRates.push_back(0.177);
    elFakeRates.push_back(0.182);
    elFakeRates.push_back(0.186);
    elFakeRates.push_back(0.205);
    elFakeRates.push_back(0.0);//ee-eb gap
    elFakeRates.push_back(0.251);
  }
  else if(elID=="MVA2017TightRC"){ // NOTE: this is a copy of FR from MVA2016TightRC !!! Need to update! Nov25,2018.
    elFakeRates.push_back(0.232);
    elFakeRates.push_back(0.0);//ee-eb gap
    elFakeRates.push_back(0.202);
    elFakeRates.push_back(0.185);
    elFakeRates.push_back(0.177);
    elFakeRates.push_back(0.182);
    elFakeRates.push_back(0.186);
    elFakeRates.push_back(0.205);
    elFakeRates.push_back(0.0);//ee-eb gap
    elFakeRates.push_back(0.251);
  }
  else if(elID=="MVA2017TightV2RC"){ 
// Rizki's measurement for 2017 data: (April 30, 2019) from plots_April17-2019_isoTrig_forTrilep
	// El:
	// eta bin: -2.4 to -1.57, FR: 0.487805 +/- 0.0890036
	// eta bin: -1.57 to -0.8, FR: 0.604167 +/- 0.0801033
	// eta bin: -0.8 to -0.4, FR: 0.540541 +/- 0.0939745
	// eta bin: -0.4 to 0.4, FR: 0.661017 +/- 0.0695383
	// eta bin: 0.4 to 0.8, FR: 0.35 +/- 0.128173
	// eta bin: 0.8 to 1.57, FR: 0.55814 +/- 0.086224
	// eta bin: 1.57 to 2.4, FR: 0.512821 +/- 0.0914971
	// eta bin: 2.4 to -2.4, FR: 4.79244e-322 +/- -1
	// eta bin: -2.4 to -2.1, FR: 4.05461e-316 +/- -1

	// Electron Fake Rate for miniIso cut < 0.01 is: 0.250871 +/- 0.0273064
	// Electron Fake Rate for miniIso cut < 0.1 is: 0.554007 +/- 0.0310273
    elFakeRates.push_back(0.488);
    elFakeRates.push_back(0.604);
    elFakeRates.push_back(0.541);
    elFakeRates.push_back(0.661);
    elFakeRates.push_back(0.35);
    elFakeRates.push_back(0.558);
    elFakeRates.push_back(0.513);
  }
  else if(elID=="MVA2017TightV2IsoTightRC"){
    // added by Jess 11-27-2020
    // Opening file: "../scripts/FakeRate/plots2018_082020_isoTrig_forTrilep/FakeRate_Graph_2018All_MVA2017TightV2IsoTightRC.root";
    // eta bin: -2.4 to -1.57, FR: 0.170594 +/- 0.00459516
    // eta bin: -1.57 to -0.8, FR: 0.156282 +/- 0.00683312
    // eta bin: -0.8 to -0.4, FR: 0.143367 +/- 0.00884668
    // eta bin: -0.4 to 0.4, FR: 0.133116 +/- 0.00629998
    // eta bin: 0.4 to 0.8, FR: 0.153121 +/- 0.00903473
    // eta bin: 0.8 to 1.57, FR: 0.152293 +/- 0.00681867
    // eta bin: 1.57 to 2.4, FR: 0.180682 +/- 0.00498344
    //elFakeRates.push_back(0.171);
    //elFakeRates.push_back(0.156);
    //elFakeRates.push_back(0.143);
    //elFakeRates.push_back(0.133);
    //elFakeRates.push_back(0.153);
    //elFakeRates.push_back(0.152);
    //elFakeRates.push_back(0.181);
    // added by Jess 12-07-2020
    // Opening file: "../scripts/FakeRate/plots2018_082020_FakeRate_isoTrig_forTrilep_AllPt_OneTrig/FakeRate_Graph_2018All_MVA2017TightV2IsoTightRC.root";
    // eta bin: -2.4 to -1.57, FR: 0.131985 +/- 0.00565513
    // eta bin: -1.57 to -0.8, FR: 0.117433 +/- 0.00906349
    // eta bin: -0.8 to -0.4, FR: 0.116995 +/- 0.0119044
    // eta bin: -0.4 to 0.4, FR: 0.101449 +/- 0.00789849
    // eta bin: 0.4 to 0.8, FR: 0.13728 +/- 0.012849
    // eta bin: 0.8 to 1.57, FR: 0.127232 +/- 0.00946525
    // eta bin: 1.57 to 2.4, FR: 0.136189 +/- 0.00629332
    elFakeRates.push_back(0.132);
    elFakeRates.push_back(0.117);
    elFakeRates.push_back(0.117);
    elFakeRates.push_back(0.101);
    elFakeRates.push_back(0.137);
    elFakeRates.push_back(0.127);
    elFakeRates.push_back(0.136);
  }
  else if(elID=="MVATightMedIsoRC") elFakeRates.push_back(0.354);
  else if(elID=="MVATightNew" || elID=="MVATightNewRC") elFakeRates.push_back(0.28);
  else if(elID=="SUSYTight" || elID=="SUSYTightRC") elFakeRates.push_back(0.20);
  else{std::cout<<"Didn't pick a valid electron ID. Exiting..."<<std::endl; return 0;}


  //doGenPlots(fsig,t,tr);
  //cd back to main directory after making gen plots
  fsig->cd();

  int nEntries = t->GetEntries();

  int nMuMu=0;
  int nElMu=0;
  int nElEl=0;
  //int nGenMuMu=0;
  //int nGenElMu=0;
  //int nGenElEl=0;

  //floats for trigger eff
  float nMu40=0;
  float nIsoTkMu24=0;
  float nMu30TkMu11=0;
  float nMu27TkMu8=0;
  float nEle27=0;
  float nDoubleEle33=0;
  float nEle17Ele12=0;
  float nEle17Ele12ORDoubleEle8HT300=0;
  float nEle17Ele12_highHT=0;
  float nEle17Ele12ORDoubleEle8HT300_highHT=0;
  float nMu30Ele30=0;
  float nMu17Ele12=0;
  float nMu8Ele17=0;
  float nMu23Ele12=0;
  float nMu8Ele23=0;
  float nMu30Ele30ORMu17Ele12ORMu8Ele17=0;

  //get variable vector
  std::vector<Variable*> vVar = getVariableVec();
  //now make histograms for various levels of cuts;
  std::vector<TH1F*> hists_ssdl_all = initHistos(vVar,-1,"ssdl");
  std::vector<TH1F*> hists_ssdl_elel = initHistos(vVar,0,"ssdl");
  std::vector<TH1F*> hists_ssdl_elmu = initHistos(vVar,1,"ssdl");
  std::vector<TH1F*> hists_ssdl_mumu = initHistos(vVar,2,"ssdl");
  //associated z-veto
  std::vector<TH1F*> hists_sZVeto_all = initHistos(vVar,-1,"sZVeto");
  std::vector<TH1F*> hists_sZVeto_elel = initHistos(vVar,0,"sZVeto");
  std::vector<TH1F*> hists_sZVeto_elmu = initHistos(vVar,1,"sZVeto");
  std::vector<TH1F*> hists_sZVeto_mumu = initHistos(vVar,2,"sZVeto");
  //two jets
  std::vector<TH1F*> hists_TwoJets_all = initHistos(vVar,-1,"TwoJets");
  std::vector<TH1F*> hists_TwoJets_elel = initHistos(vVar,0,"TwoJets");
  std::vector<TH1F*> hists_TwoJets_elmu = initHistos(vVar,1,"TwoJets");
  std::vector<TH1F*> hists_TwoJets_mumu = initHistos(vVar,2,"TwoJets");
  //nConst
  std::vector<TH1F*> hists_nConst_all = initHistos(vVar,-1,"nConst");
  std::vector<TH1F*> hists_nConst_elel = initHistos(vVar,0,"nConst");
  std::vector<TH1F*> hists_nConst_elmu = initHistos(vVar,1,"nConst");
  std::vector<TH1F*> hists_nConst_mumu = initHistos(vVar,2,"nConst");


  for(int ient=0; ient<nEntries; ient++){

    if(ient % 500 ==0) std::cout<<"Completed "<<ient<<" out of "<<nEntries<<" events"<<std::endl;

    tr->GetEntry(ient);

    //weight for non prompt method
    float NPweight=0;
    float NPAltWeight=0;
    float NPSUSYWeight=0;
    int TL;


    //make vector of good Leptons change based on era --> No need anymore --rizki
    std::vector<TLepton*> goodLeptons;
    if(isoTrig || nonIso_HTtrig){
      goodLeptons = makeLeptons(tr->allMuons, tr->allElectrons,30.0,elID,muID,bg_np);
    }
    else{
      goodLeptons = makeLeptons(tr->allMuons, tr->allElectrons,35.0,elID,muID,bg_np);
    }
//     else{//era not configured correctly
//       std::cout<<"Era not set correctly. You picked: "<<era<<", but it should be either 2016B-D or 2016E-H"<<std::endl;
//       return 0;
//     }

    //check TT/BB sig Decay - added by rizki
    if(signal){
            //std::cout << "Signal Decay Check -- chosen decay : " << sigDecay <<std::endl;
            //std::cout << "        isTZTZ = " << tr->isTZTZ <<std::endl;
            //std::cout << "        isTZTH = " << tr->isTZTH <<std::endl;
            //std::cout << "        isTZBW = " << tr->isTZBW <<std::endl;
            //std::cout << "        isTHTH = " << tr->isTHTH <<std::endl;
            //std::cout << "        isTHBW = " << tr->isTHBW <<std::endl;
            //std::cout << "        isBWBW = " << tr->isBWBW <<std::endl;
            if(sigDecay=="TZTZ" && tr->isTZTZ==false){continue ;}//std::cout << "chosen decay and sigDecay info DONT match. Skipping! " <<std::endl;}
            if(sigDecay=="TZTH" && tr->isTZTH==false){continue ;}//std::cout << "chosen decay and sigDecay info DONT match. Skipping! " <<std::endl;}
            if(sigDecay=="TZBW" && tr->isTZBW==false){continue ;}//std::cout << "chosen decay and sigDecay info DONT match. Skipping! " <<std::endl;}
            if(sigDecay=="THTH" && tr->isTHTH==false){continue ;}//std::cout << "chosen decay and sigDecay info DONT match. Skipping! " <<std::endl;}
            if(sigDecay=="THBW" && tr->isTHBW==false){continue ;}//std::cout << "chosen decay and sigDecay info DONT match. Skipping! " <<std::endl;}
            if(sigDecay=="BWBW" && tr->isBWBW==false){continue ;}//std::cout << "chosen decay and sigDecay info DONT match. Skipping! " <<std::endl;}

            if(sigDecay=="BZBZ" && tr->isBZBZ==false){continue ;}//std::cout << "chosen decay and sigDecay info DONB match. Skipping! " <<std::endl;}
            if(sigDecay=="BZBH" && tr->isBZBH==false){continue ;}//std::cout << "chosen decay and sigDecay info DONB match. Skipping! " <<std::endl;}
            if(sigDecay=="BZTW" && tr->isBZTW==false){continue ;}//std::cout << "chosen decay and sigDecay info DONB match. Skipping! " <<std::endl;}
            if(sigDecay=="BHBH" && tr->isBHBH==false){continue ;}//std::cout << "chosen decay and sigDecay info DONB match. Skipping! " <<std::endl;}
            if(sigDecay=="BHTW" && tr->isBHTW==false){continue ;}//std::cout << "chosen decay and sigDecay info DONB match. Skipping! " <<std::endl;}
            if(sigDecay=="TWTW" && tr->isTWTW==false){continue ;}//std::cout << "chosen decay and sigDecay info DONB match. Skipping! " <<std::endl;}
            //std::cout << "chosen decay and sigDecay info matches! " <<std::endl;
    }

    //reorder the leptons by pt to remove flavor ordering
    std::sort(goodLeptons.begin(),goodLeptons.end(),sortByPt);
    bool samesign;

    //get chargeMisID rate for DY and save DY events outside of Z-peak (71-111 GeV) with weights for chargeMisID
    float weight=0;
    if(outname.find("DYJets")!=std::string::npos || outname.find("ChargeMisID")!=std::string::npos){

      samesign = checkOppositeSignLeptonsForDY(goodLeptons); //returns true if find opposite sign leptons not in mu-mu channel

    }
    //now that we have good leptons, if it's not DY sample just check for two with same-sign charge and assign weight of 1
    else{
      samesign = checkSameSignLeptons(goodLeptons);
      weight=1;
    }

    if(!samesign) continue;

    //now make vector of same-sign leptons, for DY make vector containing opposite sign leptons closest to Z mass
    std::vector<TLepton*> vSSLep;
    if(outname.find("DYJets")!=std::string::npos || outname.find("ChargeMisID")!=std::string::npos){
      vSSLep = makeOSLeptonsForDY(goodLeptons);
    }
    else vSSLep = makeSSLeptons(goodLeptons);

    //dummy check to make sure the vector got filled properly
    assert(vSSLep.size() > 1);
    //make sure both are tight if not doing fakes background
    if(!bg_np){
      if( !(vSSLep.at(0)->Tight && vSSLep.at(1)->Tight)) continue;
    }

    //now prune the goodleptons of the ssleptons
    std::vector<TLepton*> vNonSSLep = pruneSSLep(goodLeptons,vSSLep);

  // move to after tm_ss->FillTree
    //added by rizki - require exactly 2 leptons, only the SSdilep, means 0 extra leptons
    //if(vNonSSLep.size()!=0) continue;

    float ew1,ew2=0.0;
    //with vector now get weight for DY Events
    if(outname.find("DYJets")!=std::string::npos || outname.find("ChargeMisID")!=std::string::npos) {
      //first set to one if muon
      if(vSSLep.at(0)->isMu){ew1=0.0;}//zero since it's cmid probability
      else{
        if(vSSLep.at(0)->pt>200){
          ew1=getEtaWeight_hhpt(fabs(vSSLep.at(0)->eta),etaWeights_hhpt);
        }
        else if(vSSLep.at(0)->pt>100){
          ew1=getEtaWeight_hpt(fabs(vSSLep.at(0)->eta),etaWeights_hpt);
        }
        else{
          ew1=getEtaWeight_lpt(fabs(vSSLep.at(0)->eta),etaWeights_lpt);
        }
      }
      if(vSSLep.at(1)->isMu){ew2=0.0;} //zero since it's cmid probability
      else{
        if(vSSLep.at(1)->pt>200){
          ew2 = getEtaWeight_hhpt(fabs(vSSLep.at(1)->eta),etaWeights_hhpt);
        }
        else if(vSSLep.at(1)->pt>100){
          ew2 = getEtaWeight_hpt(fabs(vSSLep.at(1)->eta),etaWeights_hpt);
        }
        else{
          ew2 = getEtaWeight_lpt(fabs(vSSLep.at(1)->eta),etaWeights_lpt);
        }
      }
      //now add since these are probabilities
      weight=ew1 + ew2 - ew1*ew2;
      //std::cout<<"lep1 abseta: "<<fabs(vSSLep.at(0)->eta)<<" lep1 pt: "<<vSSLep.at(0)->pt<<" ew1: "<<ew1<<std::endl;
      //std::cout<<"lep2 abseta: "<<fabs(vSSLep.at(1)->eta)<<" lep2 pt: "<<vSSLep.at(1)->pt<<" ew2: "<<ew2<<std::endl;
    }

    //now get dilepton mass
    float dilepMass = (vSSLep.at(0)->lv + vSSLep.at(1)->lv).M();


    float HT=0;
    HT+=vSSLep.at(0)->pt+vSSLep.at(1)->pt;
    for(unsigned int uijet=0; uijet<tr->allAK4Jets.size();uijet++){
      HT+=tr->allAK4Jets.at(uijet)->pt;
    }

    //get channel
    int nMu=0;
    if(vSSLep.at(0)->isMu) nMu++;
    if(vSSLep.at(1)->isMu) nMu++;

    //bools for channels
    bool mumu=false;
    bool elmu=false;
    bool elel=false;



    //ok at this point only have events with same-sign leptons, now let's do simple check to see how many of each channel we have:
    if(vSSLep.at(0)->isMu && vSSLep.at(1)->isMu){ nMuMu+=1; mumu=true;}
    else if( ( vSSLep.at(0)->isEl && vSSLep.at(1)->isMu) || (vSSLep.at(0)->isMu && vSSLep.at(1)->isEl)){ nElMu+=1; elmu=true;}
    else {nElEl+=1; elel=true;}


    //require OR of triggers but make each channel consistent
    bool skip = true;

        if(isoTrig){
                //isoTrig
          if(mumu && tr->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v) skip =false;
          if(elmu && (tr->HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v || tr->HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v)) skip = false;
          if(elel && (tr->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v)) skip = false;
        }
        else{
                if(nonIso_HTtrig){
                //NonIso_HTtrig
                  if(mumu && tr->HLT_DoubleMu4_Mass8_DZ_PFHT350) skip =false;
                  if(elmu && (tr->HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT350_DZ_v )) skip = false;
                  if(elel && tr->HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_DZ_PFHT350_v) skip = false;
                }
                else{
                //NoIsoTrig --> the corressponding single lepton trigger for one of these double triggers below dont exist for 2017/2018 !!
                  if(mumu && tr->HLT_Mu37_TkMu27) skip =false;
                  if(elmu && (tr->HLT_Mu37Ele27 || tr->HLT_Mu27Ele37)) skip = false;
                  if(elel && tr->HLT_Ele27_Ele37) skip = false;
                }
        }

    if(skip) continue;

        //HT requirement if using HT triggers - added by rizki
    if(nonIso_HTtrig){
        if(HT < 400) continue ;
    }

        //Not needed anymore -- rizki
    //now make sure we are not double counting 2016F
    bool overlap2016F = false;
//     if(data){
//       if(era=="2016B-D" && tr->run >= 278273) overlap2016F = true;
//       else if(era=="2016E-H" && tr->run <278273) overlap2016F = true;
//     }
//     else{};
    if(overlap2016F) continue;


    //now skip if not the channel from the corresponding dataset
    if((argv1.find("DataMuMu")!=std::string::npos || argv1.find("NonPromptDataMuMu")!=std::string::npos) && !mumu) continue;
    if((argv1.find("DataElMu")!=std::string::npos || argv1.find("NonPromptDataElMu")!=std::string::npos || argv1.find("ChargeMisIDElMu")!=std::string::npos) && !elmu) continue;
    if((argv1.find("DataElEl")!=std::string::npos || argv1.find("NonPromptDataElEl")!=std::string::npos || argv1.find("ChargeMisIDElEl")!=std::string::npos) && !elel) continue;

    //now veto on bad events from met scanners - already done at ljmet for 80X
    bool badEvent = false;
    if(data){
      badEvent=false;
      /*
      if(tr->run >= 260536) badEvent = EventFilterFromVector(tr->run, tr->lumi, tr->event, runs260536,lumis260536,evts260536)|| EventFilterFromFile_ECALSC(tr->run,tr->lumi,tr->event);
      else if(tr->run >= 259891) badEvent = EventFilterFromVector(tr->run, tr->lumi, tr->event, runs259891,lumis259891,evts259891)|| EventFilterFromFile_ECALSC(tr->run,tr->lumi,tr->event);
      else if(tr->run >= 259686) badEvent = EventFilterFromVector(tr->run, tr->lumi, tr->event, runs259686,lumis259686,evts259686)|| EventFilterFromFile_ECALSC(tr->run,tr->lumi,tr->event);
      else if(tr->run >= 258713) badEvent = EventFilterFromVector(tr->run, tr->lumi, tr->event, runs258713,lumis258713,evts258713)|| EventFilterFromFile_ECALSC(tr->run,tr->lumi,tr->event);
      else if(tr->run >= 258244) badEvent = EventFilterFromVector(tr->run, tr->lumi, tr->event, runs258244,lumis258244,evts258244)|| EventFilterFromFile_ECALSC(tr->run,tr->lumi,tr->event);
      else if(tr->run >= 258211) badEvent = EventFilterFromVector(tr->run, tr->lumi, tr->event, runs258211,lumis258211,evts258211)|| EventFilterFromFile_ECALSC(tr->run,tr->lumi,tr->event);
      else if(tr->run >= 258136) badEvent = EventFilterFromVector(tr->run, tr->lumi, tr->event, runs258136,lumis258136,evts258136)|| EventFilterFromFile_ECALSC(tr->run,tr->lumi,tr->event);
      else if(tr->run >= 257751) badEvent = EventFilterFromVector(tr->run, tr->lumi, tr->event, runs257751,lumis257751,evts257751)|| EventFilterFromFile_ECALSC(tr->run,tr->lumi,tr->event);
      else if(tr->run >= 257490) badEvent = EventFilterFromVector(tr->run, tr->lumi, tr->event, runs257490,lumis257490,evts257490)|| EventFilterFromFile_ECALSC(tr->run,tr->lumi,tr->event);
      else if(tr->run >= 256866) badEvent = EventFilterFromVector(tr->run, tr->lumi, tr->event, runs256866,lumis256866,evts256866)|| EventFilterFromFile_ECALSC(tr->run,tr->lumi,tr->event);
      else if(tr->run >= 256734) badEvent = EventFilterFromVector(tr->run, tr->lumi, tr->event, runs256734,lumis256734,evts256734)|| EventFilterFromFile_ECALSC(tr->run,tr->lumi,tr->event);
      else if(tr->run >= 256677) badEvent = EventFilterFromVector(tr->run, tr->lumi, tr->event, runs256677,lumis256677,evts256677)|| EventFilterFromFile_ECALSC(tr->run,tr->lumi,tr->event);
      else if(tr->run >= 256676) badEvent = EventFilterFromVector(tr->run, tr->lumi, tr->event, runs256676,lumis256676,evts256676)|| EventFilterFromFile_ECALSC(tr->run,tr->lumi,tr->event);
      else if(tr->run >= 254227) badEvent = EventFilterFromVector(tr->run, tr->lumi, tr->event, runs254227,lumis254227,evts254227)|| EventFilterFromFile_ECALSC(tr->run,tr->lumi,tr->event);
      //std::cout<<"checking at beginning for sample "<<sample<<std::endl;
      //badEvent = EventFilterFromFile_CSC(tr->run,tr->lumi,tr->event) || EventFilterFromFile_ECALSC(tr->run,tr->lumi,tr->event);
      //if(elel) badEvent = EventFilterFromFile_DoubleEG(tr->run,tr->lumi,tr->event);
      ///else if(mumu) badEvent = EventFilterFromFile_DoubleMu(tr->run,tr->lumi,tr->event);
      //else if(elmu) badEvent = EventFilterFromFile_MuonEG(tr->run,tr->lumi,tr->event);*/
    }
    if(badEvent) {std::cout<<"filtering bad event - run: "<<tr->run<<" lumi: "<<tr->lumi<<" event: "<<tr->event<<std::endl;continue;}


// Temporarily comment, all namings histos need to be updated. --rizki
//     fill muon trigger histograms
//     if(mumu){
//       //HLTMu40 - check efficiency of leading lepton
//       h_Mu40Den->Fill(vSSLep.at(0)->pt);
//       if(tr->HLT_Mu40){
//              h_Mu40Num->Fill(vSSLep.at(0)->pt);
//              nMu40+=1;
//       }
//       //HLT_IsoTkMu24 - also check for leading lepton
//       h_IsoTkMu24Den->Fill(vSSLep.at(0)->pt);
//       if(tr->HLT_IsoTkMu24){
//              h_IsoTkMu24Num->Fill(vSSLep.at(0)->pt);
//              nIsoTkMu24+=1;
//       }
//       //HLT_Mu30TkMu11 - check for subleading lepton
//       h_Mu30TkMu11Den->Fill(vSSLep.at(1)->pt);
//       if(tr->HLT_Mu30TkMu11){
//              h_Mu30TkMu11Num->Fill(vSSLep.at(1)->pt);
//              nMu30TkMu11+=1;
//       }
//       if(tr->HLT_Mu27TkMu8) nMu27TkMu8+=1;
//       //HLT_DoubleMu33NoFiltersNoVtx - check fo subleading lepton
//       h_DoubleMu33NoFiltersNoVtxDen->Fill(vSSLep.at(1)->pt);
//       if(tr->HLT_DoubleMu33NoFiltersNoVtx) h_DoubleMu33NoFiltersNoVtxNum->Fill(vSSLep.at(1)->pt);
//     }
//     //now electron trigger
//     if(elel){
//       //HLT_DoubleEle33 - check vs subleading lepton
//       h_DoubleEle33Den->Fill(vSSLep.at(1)->pt);
//       if(tr->HLT_DoubleEle33){
//              h_DoubleEle33Num->Fill(vSSLep.at(1)->pt);
//              nDoubleEle33+=1;
//       }
//       //HLT_DoubleEle33_MW - check vs subleading lepton
//       h_DoubleEle33_MWDen->Fill(vSSLep.at(1)->pt);
//       if(tr->HLT_DoubleEle33_MW) h_DoubleEle33_MWNum->Fill(vSSLep.at(1)->pt);
//       if(tr->HLT_Ele17Ele12) nEle17Ele12+=1;
//       if(tr->HLT_Ele17Ele12 || tr->HLT_DoubleEle8_HT300) nEle17Ele12ORDoubleEle8HT300+=1;
//     }
//
//     //if cross channel
//     if(elmu){
//       if(tr->HLT_Mu30Ele30) nMu30Ele30+=1;
//       if(tr->HLT_Mu17Ele12) nMu17Ele12+=1;
//       if(tr->HLT_Mu8Ele17) nMu8Ele17+=1;
//       if(tr->HLT_Mu23Ele12) nMu23Ele12+=1;
//       if(tr->HLT_Mu8Ele23) nMu8Ele23+=1;
//       //also get OR
//       if( (tr->HLT_Mu17Ele12) || (tr->HLT_Mu8Ele17) )nMu30Ele30ORMu17Ele12ORMu8Ele17+=1;
//
//     }


    //get trigger scale factors
    std::vector<float> vTrigSF;
    if(!data) vTrigSF = getTrigSF(vSSLep,era);
    else {vTrigSF.push_back(1.0); vTrigSF.push_back(1.0); vTrigSF.push_back(1.0);}
    float trigSF = vTrigSF.at(0);
    float trigSFup = vTrigSF.at(1);
    float trigSFdn = vTrigSF.at(2);

    //now get lepton ID Scale Factors
    std::vector<float> lepIDSF_vec = getLepIDSF(vSSLep);
    float lepIDSF, lepIDSFup, lepIDSFdn;
    if(data) {lepIDSF = 1.0; lepIDSFup = 1.0; lepIDSFdn = 1.0;}
    else {lepIDSF = lepIDSF_vec.at(0); lepIDSFup = lepIDSF_vec.at(1); lepIDSFdn = lepIDSF_vec.at(2);}

    float lepIsoSF;
    if(data) lepIsoSF = 1.0;
    else lepIsoSF = getLepIsoSF(vSSLep);
    //get gsf tracking sf
    float gsfSF = 1.0;
    if(data) gsfSF=1.0;
    else{
      gsfSF=1.0;//set to one currently
      if(vSSLep.at(0)->isEl) gsfSF*= getGsfSF(vSSLep.at(0));
      if(vSSLep.at(1)->isEl) gsfSF*= getGsfSF(vSSLep.at(1));
    }

    //now need to calculate nonPromptWeight - first get prompt rate for each lepton
    float lep1PromptRate=0.0;
    float lep2PromptRate=0.0;
//     if(vSSLep.at(0)->isMu) lep1PromptRate=muPromptRate;
    if(vSSLep.at(0)->isMu){
      //if(vSSLep.at(0)->pt<30) lep1PromptRate=muPromptRates.at(0);
      if(vSSLep.at(0)->pt<40) lep1PromptRate=muPromptRates.at(0);
      else if(vSSLep.at(0)->pt<50) lep1PromptRate=muPromptRates.at(1);
      else if(vSSLep.at(0)->pt<60) lep1PromptRate=muPromptRates.at(2);
      else if(vSSLep.at(0)->pt<70) lep1PromptRate=muPromptRates.at(3);
      else if(vSSLep.at(0)->pt<80) lep1PromptRate=muPromptRates.at(4);
      else if(vSSLep.at(0)->pt<90) lep1PromptRate=muPromptRates.at(5);
      else if(vSSLep.at(0)->pt<100) lep1PromptRate=muPromptRates.at(6);
      else if(vSSLep.at(0)->pt<125) lep1PromptRate=muPromptRates.at(7);
      else if(vSSLep.at(0)->pt<150) lep1PromptRate=muPromptRates.at(8);
      else if(vSSLep.at(0)->pt<200) lep1PromptRate=muPromptRates.at(9);
      else if(vSSLep.at(0)->pt<300) lep1PromptRate=muPromptRates.at(10);
      else if(vSSLep.at(0)->pt<400) lep1PromptRate=muPromptRates.at(11);
      else if(vSSLep.at(0)->pt<500) lep1PromptRate=muPromptRates.at(12);
      else lep1PromptRate = muPromptRates.at(13);    
    }
    else{
      //if(vSSLep.at(0)->pt<30) lep1PromptRate=elPromptRates.at(0);
      if(vSSLep.at(0)->pt<40) lep1PromptRate=elPromptRates.at(0);
      else if(vSSLep.at(0)->pt<50) lep1PromptRate=elPromptRates.at(1);
      else if(vSSLep.at(0)->pt<60) lep1PromptRate=elPromptRates.at(2);
      else if(vSSLep.at(0)->pt<70) lep1PromptRate=elPromptRates.at(3);
      else if(vSSLep.at(0)->pt<80) lep1PromptRate=elPromptRates.at(4);
      else if(vSSLep.at(0)->pt<90) lep1PromptRate=elPromptRates.at(5);
      else if(vSSLep.at(0)->pt<100) lep1PromptRate=elPromptRates.at(6);
      else if(vSSLep.at(0)->pt<125) lep1PromptRate=elPromptRates.at(7);
      else if(vSSLep.at(0)->pt<150) lep1PromptRate=elPromptRates.at(8);
      else if(vSSLep.at(0)->pt<200) lep1PromptRate=elPromptRates.at(9);
      else if(vSSLep.at(0)->pt<300) lep1PromptRate=elPromptRates.at(10);
      else if(vSSLep.at(0)->pt<400) lep1PromptRate=elPromptRates.at(11);
      else if(vSSLep.at(0)->pt<500) lep1PromptRate=elPromptRates.at(12);
      else lep1PromptRate = elPromptRates.at(13);
    }

    if(vSSLep.at(1)->isMu) lep2PromptRate=muPromptRate;
    if(vSSLep.at(1)->isMu){
      //if(vSSLep.at(1)->pt<30) lep2PromptRate=muPromptRates.at(0);
      if(vSSLep.at(1)->pt<40) lep2PromptRate=muPromptRates.at(0);
      else if(vSSLep.at(1)->pt<50) lep2PromptRate=muPromptRates.at(1);
      else if(vSSLep.at(1)->pt<60) lep2PromptRate=muPromptRates.at(2);
      else if(vSSLep.at(1)->pt<70) lep2PromptRate=muPromptRates.at(3);
      else if(vSSLep.at(1)->pt<80) lep2PromptRate=muPromptRates.at(4);
      else if(vSSLep.at(1)->pt<90) lep2PromptRate=muPromptRates.at(5);
      else if(vSSLep.at(1)->pt<100) lep2PromptRate=muPromptRates.at(6);
      else if(vSSLep.at(1)->pt<125) lep2PromptRate=muPromptRates.at(7);
      else if(vSSLep.at(1)->pt<150) lep2PromptRate=muPromptRates.at(8);
      else if(vSSLep.at(1)->pt<200) lep2PromptRate=muPromptRates.at(9);
      else if(vSSLep.at(1)->pt<300) lep2PromptRate=muPromptRates.at(10);
      else if(vSSLep.at(1)->pt<400) lep2PromptRate=muPromptRates.at(11);
      else if(vSSLep.at(1)->pt<500) lep2PromptRate=muPromptRates.at(12);
      else lep2PromptRate = muPromptRates.at(13);    
    }
    else {
      //if(vSSLep.at(1)->pt<30) lep2PromptRate=elPromptRates.at(0);
      if(vSSLep.at(1)->pt<40) lep2PromptRate=elPromptRates.at(0);
      else if(vSSLep.at(1)->pt<50) lep2PromptRate=elPromptRates.at(1);
      else if(vSSLep.at(1)->pt<60) lep2PromptRate=elPromptRates.at(2);
      else if(vSSLep.at(1)->pt<70) lep2PromptRate=elPromptRates.at(3);
      else if(vSSLep.at(1)->pt<80) lep2PromptRate=elPromptRates.at(4);
      else if(vSSLep.at(1)->pt<90) lep2PromptRate=elPromptRates.at(5);
      else if(vSSLep.at(1)->pt<100) lep2PromptRate=elPromptRates.at(6);
      else if(vSSLep.at(1)->pt<125) lep2PromptRate=elPromptRates.at(7);
      else if(vSSLep.at(1)->pt<150) lep2PromptRate=elPromptRates.at(8);
      else if(vSSLep.at(1)->pt<200) lep2PromptRate=elPromptRates.at(9);
      else if(vSSLep.at(1)->pt<300) lep2PromptRate=elPromptRates.at(10);
      else if(vSSLep.at(1)->pt<400) lep2PromptRate=elPromptRates.at(11);
      else if(vSSLep.at(1)->pt<500) lep2PromptRate=elPromptRates.at(12);
      else lep2PromptRate = elPromptRates.at(13);
    }
    //now get fake rate for each lepton
    float lep1FakeRate = 0.0;
    float lep2FakeRate = 0.0;
    if(vSSLep.at(0)->isMu){
      if(vSSLep.at(0)->eta < -2.1) lep1FakeRate=muFakeRates.at(0);
      else if(vSSLep.at(0)->eta < -1.2) lep1FakeRate=muFakeRates.at(1);
      else if(vSSLep.at(0)->eta < -0.9) lep1FakeRate=muFakeRates.at(2);
      else if(vSSLep.at(0)->eta < -0.4) lep1FakeRate=muFakeRates.at(3);
//       else if(vSSLep.at(0)->eta < 0.0) lep1FakeRate=muFakeRates.at(4);
      else if(vSSLep.at(0)->eta < 0.4) lep1FakeRate=muFakeRates.at(4);
      else if(vSSLep.at(0)->eta < 0.9) lep1FakeRate=muFakeRates.at(5);
      else if(vSSLep.at(0)->eta < 1.2) lep1FakeRate=muFakeRates.at(6);
      else if(vSSLep.at(0)->eta < 2.1) lep1FakeRate=muFakeRates.at(7);
      else lep1FakeRate=muFakeRates.at(8);
    }
    else{ //leading is electron
      if(vSSLep.at(0)->eta < -1.566) lep1FakeRate=elFakeRates.at(0);
//       else if(vSSLep.at(0)->eta < -1.4442) lep1FakeRate=elFakeRates.at(1);
      else if(vSSLep.at(0)->eta < -0.8) lep1FakeRate=elFakeRates.at(1);
      else if(vSSLep.at(0)->eta < -0.4) lep1FakeRate=elFakeRates.at(2);
//       else if(vSSLep.at(0)->eta < 0.0) lep1FakeRate=elFakeRates.at(4);
      else if(vSSLep.at(0)->eta < 0.4) lep1FakeRate=elFakeRates.at(3);
      else if(vSSLep.at(0)->eta < 0.8) lep1FakeRate=elFakeRates.at(4);
//       else if(vSSLep.at(0)->eta < 1.4442) lep1FakeRate=elFakeRates.at(7);
      else if(vSSLep.at(0)->eta < 1.566) lep1FakeRate=elFakeRates.at(5);
      else lep1FakeRate=elFakeRates.at(6);
    }
    //now for subleading
    if(vSSLep.at(1)->isMu){
      if(vSSLep.at(1)->eta < -2.1) lep2FakeRate=muFakeRates.at(0);
      else if(vSSLep.at(1)->eta < -1.2) lep2FakeRate=muFakeRates.at(1);
      else if(vSSLep.at(1)->eta < -0.9) lep2FakeRate=muFakeRates.at(2);
      else if(vSSLep.at(1)->eta < -0.4) lep2FakeRate=muFakeRates.at(3);
//       else if(vSSLep.at(1)->eta < 0.0) lep2FakeRate=muFakeRates.at(4);
      else if(vSSLep.at(1)->eta < 0.4) lep2FakeRate=muFakeRates.at(4);
      else if(vSSLep.at(1)->eta < 0.9) lep2FakeRate=muFakeRates.at(5);
      else if(vSSLep.at(1)->eta < 1.2) lep2FakeRate=muFakeRates.at(6);
      else if(vSSLep.at(1)->eta < 2.1) lep2FakeRate=muFakeRates.at(7);
      else lep2FakeRate=muFakeRates.at(8);
    }
    else{ //leading is electron
      if(vSSLep.at(1)->eta < -1.566) lep2FakeRate=elFakeRates.at(0);
//       else if(vSSLep.at(1)->eta < -1.4442) lep2FakeRate=elFakeRates.at(1);
      else if(vSSLep.at(1)->eta < -0.8) lep2FakeRate=elFakeRates.at(1);
      else if(vSSLep.at(1)->eta < -0.4) lep2FakeRate=elFakeRates.at(2);
//       else if(vSSLep.at(1)->eta < 0.0) lep2FakeRate=elFakeRates.at(4);
      else if(vSSLep.at(1)->eta < 0.4) lep2FakeRate=elFakeRates.at(3);
      else if(vSSLep.at(1)->eta < 0.8) lep2FakeRate=elFakeRates.at(4);
//       else if(vSSLep.at(1)->eta < 1.4442) lep2FakeRate=elFakeRates.at(7);
      else if(vSSLep.at(1)->eta < 1.566) lep2FakeRate=elFakeRates.at(5);
      else lep2FakeRate=elFakeRates.at(6);
    }

    if(!bg_np) {NPweight=1.0;NPAltWeight=1.0;NPSUSYWeight=1.0;TL=-1;}
    else{

      //**********get susy alternative weight
      if(vSSLep.at(0)->Tight && vSSLep.at(1)->Tight) NPSUSYWeight=0;
      else if(vSSLep.at(0)->Tight && !(vSSLep.at(1)->Tight)) NPSUSYWeight = lep2FakeRate / (1.0 - lep2FakeRate);
      else if(!(vSSLep.at(0)->Tight) && vSSLep.at(1)->Tight) NPSUSYWeight = lep1FakeRate / (1.0 - lep1FakeRate);
      else NPSUSYWeight = (lep1FakeRate / (1 - lep1FakeRate)) * ( lep2FakeRate/ (1 - lep2FakeRate));

      if(mumu || elel){//same flavor channel
        //updating to kinematic dependent fake/prompt rates for same flavor channels can treat as 'different' flavors where flavor is now leading/subleading
        if(vSSLep.at(0)->Tight && vSSLep.at(1)->Tight) {NPweight = WeightOF_T11(lep1PromptRate,lep1FakeRate,lep2PromptRate,lep2FakeRate); TL = 3;}//both tight
        else if(vSSLep.at(0)->Tight && !(vSSLep.at(1)->Tight)) {NPweight = WeightOF_T10(lep1PromptRate,lep1FakeRate,lep2PromptRate,lep2FakeRate); TL=2;}//leading tight
        else if(!(vSSLep.at(0)->Tight) && vSSLep.at(1)->Tight) {NPweight = WeightOF_T01(lep1PromptRate,lep1FakeRate,lep2PromptRate,lep2FakeRate); TL=1;}//subleading tight
        else {NPweight = WeightOF_T00(lep1PromptRate,lep1FakeRate,lep2PromptRate,lep2FakeRate); TL=0;}//both loose
        //make alt weight the same as above
        NPAltWeight=NPweight;
        /*      if(vSSLep.at(0)->Tight && vSSLep.at(1)->Tight) {NPweight = WeightSF_T2(muPromptRate,muFakeRate); TL = 3;}//both tight
        else if(vSSLep.at(0)->Tight || vSSLep.at(1)->Tight) {NPweight = WeightSF_T1(muPromptRate,muFakeRate); TL=2;}//one tight
        else {NPweight = WeightSF_T0(muPromptRate,muFakeRate); TL=0;}//both loose*/
      }
      /*else if(elel){//electron channel

        if(vSSLep.at(0)->Tight && vSSLep.at(1)->Tight) {NPweight = WeightSF_T2(elPromptRate,elFakeRate); TL=3;}//both tight
        else if(vSSLep.at(0)->Tight || vSSLep.at(1)->Tight) {NPweight = WeightSF_T1(elPromptRate,elFakeRate); TL=1;}//one tight
        else {NPweight = WeightSF_T0(elPromptRate,elFakeRate); TL=0;}//both loose
        }*/
      else{ //cross channel - still important to save which flavor is which, kinematic effects happen 'in background'
        //******** Making alternative weight***********
        //updating to kinematic dependent fake/prompt rates for same flavor channels can treat as 'different' flavors where flavor is now leading/subleading
        if(vSSLep.at(0)->Tight && vSSLep.at(1)->Tight) {NPAltWeight = WeightOF_T11(lep1PromptRate,lep1FakeRate,lep2PromptRate,lep2FakeRate); TL = 3;}//both tight
        else if(vSSLep.at(0)->Tight && !(vSSLep.at(1)->Tight)) {NPAltWeight = WeightOF_T10(lep1PromptRate,lep1FakeRate,lep2PromptRate,lep2FakeRate); TL=2;}//leading tight
        else if(!(vSSLep.at(0)->Tight) && vSSLep.at(1)->Tight) {NPAltWeight = WeightOF_T01(lep1PromptRate,lep1FakeRate,lep2PromptRate,lep2FakeRate); TL=1;}//subleading tight
        else {NPAltWeight = WeightOF_T00(lep1PromptRate,lep1FakeRate,lep2PromptRate,lep2FakeRate); TL=0;}//both loose
        //******** Making origin np weight ************
        float muPR,muFR,elPR,elFR;
        if(vSSLep.at(0)->isMu){muPR=lep1PromptRate;muFR=lep1FakeRate;elPR=lep2PromptRate;elFR=lep2FakeRate;} //for cross channel finding flavor of leading determines flavor of subleading
        else {muPR=lep2PromptRate;muFR=lep2FakeRate;elPR=lep1PromptRate;elFR=lep1FakeRate;}
        if(vSSLep.at(0)->Tight && vSSLep.at(1)->Tight) {NPweight = WeightOF_T11(elPR,elFR,muPR,muFR); TL=3;} //both tight
        else if ( (vSSLep.at(0)->isEl && vSSLep.at(0)->Tight) || (vSSLep.at(1)->isEl && vSSLep.at(1)->Tight) )  {NPweight = WeightOF_T10(elPR,elFR,muPR,muFR);TL=2;} //if electron is tight, muon must be loose or we would be on line above so just see if either lepton is a tight electron
        else if ( (vSSLep.at(0)->isMu && vSSLep.at(0)->Tight) || (vSSLep.at(1)->isMu && vSSLep.at(1)->Tight) ) {NPweight = WeightOF_T01(elPR,elFR,muPR,muFR); TL=1;}//if muon is tight, el must be loose or we would be on tight-tight line so just check for tight muon
        else {NPweight = WeightOF_T00(elPR,elFR,muPR,muFR); TL=0;}//otherwise both are loose
      }
    }//end np check


    //DEBUGGIN STATEMENTS TO UNDERSTAND TL ASSIGNMENT
    //if(HT>900){ //just to lessen the number of events

    //if(bg_np) std::cout<<"Check for NonPrompt -run:"<<tr->run<<" lumi: "<<tr->lumi<<" event: "<<tr->event<<std::endl;
    //else std::cout<<"Check for signal -run:"<<tr->run<<" lumi: "<<tr->lumi<<" event: "<<tr->event<<std::endl;
    //std::cout<<"Lep1 tight: "<<vSSLep.at(0)->Tight<<" Lep2Tight"<<vSSLep.at(1)->Tight<<" TL: "<<TL<<std::endl;

    //}

    //get pileup weight;
    float puweight=-11;
    float puweightUp=-11;
    float puweightDown=-11;
    if(data) puweight=1;
    else{
      int nTrueInteractions = tr->nPU;
      if (tr->nPU > 99) nTrueInteractions = 99;
      if (signal && tr->nPU > 79) nTrueInteractions = 79;
      if (tr->nPU < 0 ) nTrueInteractions = 0;
      puweight = getPUWeight(sampleName, nTrueInteractions, 0);
      puweightUp = getPUWeight(sampleName, nTrueInteractions, 1);
      puweightDown = getPUWeight(sampleName, nTrueInteractions, -1);
      //puweight=1;
    }

    if(puweight >20) std::cout<<"Pileup weight is: "<<puweight<<std::endl;

    float mcweight;
    if(data) mcweight = 1;
    else{
      if(tr->MCWeight>=0) mcweight=1.0;
      else mcweight=-1.0;
    }

    //make boolean here so we can fill the closets associated mass
    bool secondaryZVeto = (checkSecondaryZVeto(vSSLep,tr->looseMuons,tr->looseElectrons)).first;
    float assocMass =  (checkSecondaryZVeto(vSSLep,tr->looseMuons,tr->looseElectrons)).second;

    std::vector<double> pdfweights_ss(100,1.0); 
    std::vector<double> renormWeights_ss(6,1.0);
    std::vector<double> pdfNewWeights_ss(100,1.0);
    std::vector<double> pdfWeights4LHC_ss(31,1.0);
    std::vector<double> pdfWeightsMSTW_ss(41,1.0);
    double pdfNewNominalWeight_ss = 1.0;

    //fill tree for ss cut only (for table)
    tm_ss->FillTree(vSSLep, tr->allAK4Jets, tr->cleanedAK4Jets, tr->simpleCleanedAK4Jets, HT, tr->MET, dilepMass, nMu, weight, vNonSSLep, tr->MCWeight, NPweight, NPAltWeight, NPSUSYWeight, TL, trigSF, trigSFup, trigSFdn, lepIDSF, lepIDSFup, lepIDSFdn, lepIsoSF, gsfSF, puweight, puweightUp, puweightDown, assocMass, tr->allAK8Jets, tr->hadronicGenJets, !data, tr->run, tr->lumi, tr->event, tr->nPrimaryVert, pdfweights_ss, renormWeights_ss, pdfNewWeights_ss, pdfWeights4LHC_ss, pdfWeightsMSTW_ss, pdfNewNominalWeight_ss);

    if(vNonSSLep.size()!=0) continue;

    //fill tree for post ssdl cut since that is all that we've applied so far
    tm_ssdl->FillTree(vSSLep, 
                      tr->allAK4Jets, 
                      tr->cleanedAK4Jets,
                      tr->simpleCleanedAK4Jets, 
                      HT, 
                      tr->MET, 
                      dilepMass,
                      nMu,
                      weight,
                      vNonSSLep,
                      tr->MCWeight,
                      NPweight,
                      NPAltWeight,
                      NPSUSYWeight,
                      TL,
                      trigSF,
                      trigSFup,
                      trigSFdn,
                      lepIDSF,
                      lepIDSFup,
                      lepIDSFdn,
                      lepIsoSF,
                      gsfSF,
                      puweight,
                      puweightUp,
                      puweightDown,
                      assocMass,
                      tr->allAK8Jets,
                      tr->hadronicGenJets,
                      !data,
                      tr->run,
                      tr->lumi,
                      tr->event,
                      tr->nPrimaryVert,
                      pdfweights_ss,
                      renormWeights_ss,
                      pdfNewWeights_ss,
                      pdfWeights4LHC_ss,
                      pdfWeightsMSTW_ss, 
                      pdfNewNominalWeight_ss);
    //fill histos for same cut level
    float totalweight = weight * NPweight * trigSF * lepIDSF * lepIsoSF* puweight * mcweight * gsfSF;
    fillHistos(hists_ssdl_all, vSSLep, vNonSSLep, tr->cleanedAK4Jets, tr->MET, dilepMass, totalweight);
    if(elel) fillHistos(hists_ssdl_elel, vSSLep, vNonSSLep, tr->cleanedAK4Jets, tr->MET, dilepMass, totalweight);
    if(elmu) fillHistos(hists_ssdl_elmu, vSSLep, vNonSSLep, tr->cleanedAK4Jets, tr->MET, dilepMass, totalweight);
    if(mumu) fillHistos(hists_ssdl_mumu, vSSLep, vNonSSLep, tr->cleanedAK4Jets, tr->MET, dilepMass, totalweight);


    float st = vSSLep.at(0)->pt + vSSLep.at(1)->pt;
    for(unsigned int j=0; j < vNonSSLep.size(); j++){
      st = st + vNonSSLep.at(j)->pt;
    }
    for(unsigned int j=0; j <tr->cleanedAK4Jets.size();j++){
      st = st + tr->cleanedAK4Jets.at(j)->pt;
    }

    std::vector<double> pdfweights_ssdl{}; 
    std::vector<double> renormWeights_ssdl{};
    std::vector<double> pdfNewWeights_ssdl{};
    std::vector<double> pdfWeights4LHC_ssdl{};
    std::vector<double> pdfWeightsMSTW_ssdl{};
    double pdfNewNominalWeight_ssdl = 1.0;

    //now get pdf weights at early stage
    if(!data){
      //now fill ppdf weight histogram
      //std::vector<double> pdfweight_ssdl = (*tr->LHEWeights);
      //std::vector<int> pdfweightIDs_ssdl = (*tr->LHEWeightIDs);
      hist_scaleHT_ssdl_nom->Fill(st,mcweight);

      if(signal) pdfNewNominalWeight_ssdl = (*tr->NewPDFweights).at(0);
      //for(unsigned int i=0; i< pdfweightIDs_ssdl.size(); i++){
      //  int ID = pdfweightIDs_ssdl.at(i);
      for(unsigned int i=0; i< (*tr->LHEWeightIDs).size(); i++){
        int ID = (*tr->LHEWeightIDs).at(i);
        // select pdf by ID
        if(signal){
          if(i > 0 && i < 101) pdfNewWeights_ssdl.push_back((*tr->NewPDFweights).at(i));
          if((ID > 1 && ID < 10) && !(ID==6 || ID==8)) renormWeights_ssdl.push_back((*tr->LHEWeights).at(i));
          if(ID > 474 && ID < 575) pdfweights_ssdl.push_back((*tr->LHEWeights).at(i));
          if(ID > 442 && ID < 474) pdfWeights4LHC_ssdl.push_back((*tr->LHEWeights).at(i));
          if(ID > 205 && ID < 247) pdfWeightsMSTW_ssdl.push_back((*tr->LHEWeights).at(i));
        }
        else if(std::find(madgraph_samples.begin(), madgraph_samples.end(), sample)!=madgraph_samples.end() ){
          if((ID > 1 && ID < 10) && !(ID==6 || ID==8)) renormWeights_ssdl.push_back((*tr->LHEWeights).at(i));
          if(ID > 714 && ID < 746) pdfWeights4LHC_ssdl.push_back((*tr->LHEWeights).at(i));
          if(ID > 10 && ID < 111) pdfweights_ssdl.push_back((*tr->LHEWeights).at(i));
        }
        else if(std::find(powheg_samples.begin(), powheg_samples.end(), sample)!=powheg_samples.end() ){
          if((ID > 1001 && ID < 1010) && !(ID==1006 || ID==1008)) renormWeights_ssdl.push_back((*tr->LHEWeights).at(i));
          if(ID > 10999 && ID < 11031) pdfWeights4LHC_ssdl.push_back((*tr->LHEWeights).at(i));
          if(ID > 2000 && ID < 2101) pdfweights_ssdl.push_back((*tr->LHEWeights).at(i));
        }
        else {
          if((ID > 1001 && ID < 1010) && !(ID==1006 || ID==1008)) renormWeights_ssdl.push_back((*tr->LHEWeights).at(i));
          if(ID > 1714 && ID < 1745) pdfWeights4LHC_ssdl.push_back((*tr->LHEWeights).at(i));
          if(ID > 1010 && ID < 1111) pdfweights_ssdl.push_back((*tr->LHEWeights).at(i));
        }

        if(ID==1002 || ID==1003 || ID==1004 || ID==1005 || ID==1007 || ID==1009 || ID==2 || ID==3 || ID==4 || ID==5 || ID==7 || ID==9){
          //now fill individual
          if(ID==1002 || (outname.find("prime")!=std::string::npos && ID==2) )hist_scaleHT_ssdl_1002->Fill(st,(*tr->LHEWeights).at(i)*mcweight); //edited by rizki
          if(ID==1003 || (outname.find("prime")!=std::string::npos && ID==3) )hist_scaleHT_ssdl_1003->Fill(st,(*tr->LHEWeights).at(i)*mcweight); //edited by rizki
          if(ID==1004 || (outname.find("prime")!=std::string::npos && ID==4) )hist_scaleHT_ssdl_1004->Fill(st,(*tr->LHEWeights).at(i)*mcweight); //edited by rizki
          if(ID==1005 || (outname.find("prime")!=std::string::npos && ID==5) )hist_scaleHT_ssdl_1005->Fill(st,(*tr->LHEWeights).at(i)*mcweight); //edited by rizki
          if(ID==1007|| (outname.find("prime")!=std::string::npos && ID==7) )hist_scaleHT_ssdl_1007->Fill(st,(*tr->LHEWeights).at(i)*mcweight); //edited by rizki
          if(ID==1009|| (outname.find("prime")!=std::string::npos && ID==2) )hist_scaleHT_ssdl_1009->Fill(st,(*tr->LHEWeights).at(i)*mcweight); //edited by rizki
        }
        //if(!(ID>2000 && i<2101)) continue;
      }
    }
    else{ // dummy pdf weights for data
      for(unsigned int i=0; i< 100; i++){pdfweights_ssdl.push_back(1.0); pdfNewWeights_ssdl.push_back(1.0);}
      for(unsigned int i=0; i< 6  ; i++){renormWeights_ssdl.push_back(1.0);}
      for(unsigned int i=0; i< 31 ; i++){pdfWeights4LHC_ssdl.push_back(1.0);}
      for(unsigned int i=0; i< 41 ; i++){pdfWeightsMSTW_ssdl.push_back(1.0);}
    }

    if(DEBUG_COUT) std::cout<< "pdf weights size (ssdl) ... " << pdfweights_ssdl.size() << " " << pdfNewWeights_ssdl.size() << " " << renormWeights_ssdl.size() << " " << pdfWeights4LHC_ssdl.size() << " " << pdfWeightsMSTW_ssdl.size() << std::endl;

    //since we have the two same-sign leptons, now make sure neither of them reconstructs with any other tight lepton in the event to form a Z
    if(secondaryZVeto) continue;
    //fill tree for post secondary z veto
    tm_sZVeto->FillTree(vSSLep, tr->allAK4Jets, tr->cleanedAK4Jets, tr->simpleCleanedAK4Jets, HT, tr->MET, dilepMass,nMu,weight,vNonSSLep,tr->MCWeight,NPweight,NPAltWeight,NPSUSYWeight,TL,trigSF,trigSFup,trigSFdn,lepIDSF,lepIDSFup,lepIDSFdn,lepIsoSF,gsfSF,puweight,puweightUp,puweightDown,assocMass,tr->allAK8Jets,tr->hadronicGenJets,!data,tr->run,tr->lumi,tr->event,tr->nPrimaryVert, pdfweights_ssdl, renormWeights_ssdl, pdfNewWeights_ssdl, pdfWeights4LHC_ssdl, pdfWeightsMSTW_ssdl, pdfNewNominalWeight_ssdl);
    //now fill corresponding histos
    fillHistos(hists_sZVeto_all, vSSLep, vNonSSLep, tr->cleanedAK4Jets, tr->MET, dilepMass, totalweight);
    if(elel) fillHistos(hists_sZVeto_elel, vSSLep, vNonSSLep, tr->cleanedAK4Jets, tr->MET, dilepMass, totalweight);
    if(elmu) fillHistos(hists_sZVeto_elmu, vSSLep, vNonSSLep, tr->cleanedAK4Jets, tr->MET, dilepMass, totalweight);
    if(mumu) fillHistos(hists_sZVeto_mumu, vSSLep, vNonSSLep, tr->cleanedAK4Jets, tr->MET, dilepMass, totalweight);

    //skip events in dielectron which reconstruct to z
    if(elel){
      if(dilepMass < 106 && dilepMass>76) continue;
    }

    //quarkonia veto
    if(!(dilepMass>20)) continue;

    //if(data && sample=="ChargeMisIDElEl"){     //do charge misID after vetoing on primary Z
    //if(elel) badEvent = EventFilterFromFile_DoubleEG(tr->run,tr->lumi,tr->event);
    //else if(mumu) badEvent = EventFilterFromFile_DoubleMu(tr->run,tr->lumi,tr->event);
    //else if(elmu) badEvent = EventFilterFromFile_MuonEG(tr->run,tr->lumi,tr->event);
    //}
    if(badEvent) {std::cout<<"filtering bad event"<<std::endl;continue;}
    tm_DilepMassCut->FillTree(vSSLep, tr->allAK4Jets, tr->cleanedAK4Jets, tr->simpleCleanedAK4Jets, HT, tr->MET, dilepMass,nMu,weight,vNonSSLep,tr->MCWeight,NPweight,NPAltWeight,NPSUSYWeight,TL,trigSF,trigSFup,trigSFdn,lepIDSF,lepIDSFup,lepIDSFdn,lepIsoSF,gsfSF,puweight,puweightUp,puweightDown,assocMass,tr->allAK8Jets,tr->hadronicGenJets,!data,tr->run,tr->lumi,tr->event,tr->nPrimaryVert, pdfweights_ssdl, renormWeights_ssdl, pdfNewWeights_ssdl, pdfWeights4LHC_ssdl, pdfWeightsMSTW_ssdl, pdfNewNominalWeight_ssdl);

    if(tr->cleanedAK4Jets.size()>1){
      //now fill corresponding histos
      fillHistos(hists_TwoJets_all, vSSLep, vNonSSLep, tr->cleanedAK4Jets, tr->MET, dilepMass, totalweight);
      if(elel) fillHistos(hists_TwoJets_elel, vSSLep, vNonSSLep, tr->cleanedAK4Jets, tr->MET, dilepMass, totalweight);
      if(elmu) fillHistos(hists_TwoJets_elmu, vSSLep, vNonSSLep, tr->cleanedAK4Jets, tr->MET, dilepMass, totalweight);
      if(mumu) fillHistos(hists_TwoJets_mumu, vSSLep, vNonSSLep, tr->cleanedAK4Jets, tr->MET, dilepMass, totalweight);
    }

    if(isoTrig || nonIso_HTtrig){
        if(vSSLep.at(0)->pt<40) continue; //skip events with leading lepton pt less than 40
    }
    else{
        if(vSSLep.at(0)->pt<40) continue; //skip events with leading lepton pt less than 40
    }

    int nconst = tr->cleanedAK4Jets.size() + vNonSSLep.size();
//     if(nconst<5) continue; //nconst cuts
    if(nconst<4) continue; //nconst cuts //edited by rizki

    //now fill corresponding histos
    fillHistos(hists_nConst_all, vSSLep, vNonSSLep, tr->cleanedAK4Jets, tr->MET, dilepMass, totalweight);
    if(elel) fillHistos(hists_nConst_elel, vSSLep, vNonSSLep, tr->cleanedAK4Jets, tr->MET, dilepMass, totalweight);
    if(elmu) fillHistos(hists_nConst_elmu, vSSLep, vNonSSLep, tr->cleanedAK4Jets, tr->MET, dilepMass, totalweight);
    if(mumu) fillHistos(hists_nConst_mumu, vSSLep, vNonSSLep, tr->cleanedAK4Jets, tr->MET, dilepMass, totalweight);

// Temporarily comment, all namings histos need to be updated. --rizki
//     if(elel && st>1000){
//       if(tr->HLT_Ele17Ele12) nEle17Ele12_highHT+=1;
//       if(tr->HLT_Ele17Ele12 || tr->HLT_DoubleEle8_HT300) nEle17Ele12ORDoubleEle8HT300_highHT+=1;
//     }

    if(!data){
      //now fill ppdf weight histogram
      std::vector<double> pdfweights{}; //= (*tr->LHEWeights);
      //std::vector<int> pdfweightIDs = (*tr->LHEWeightIDs);
      std::vector<double> renormWeights{};
      std::vector<double> pdfNewWeights{};
      std::vector<double> pdfWeights4LHC{};
      std::vector<double> pdfWeightsMSTW{};
      //now fill ppdf weight histogram
      hist_scaleHT_nom->Fill(st,mcweight);

      double pdfNewNominalWeight = 1.0;
      if(signal) pdfNewNominalWeight = (*tr->NewPDFweights).at(0);
      for(unsigned int i=0; i< (*tr->LHEWeightIDs).size(); i++){
                int ID = (*tr->LHEWeightIDs).at(i);
                // select pdf by ID
                  if(signal){
                        if(i > 0 && i < 101) pdfNewWeights.push_back((*tr->NewPDFweights).at(i));
                        if((ID > 1 && ID < 10) && !(ID==6 || ID==8)) renormWeights.push_back((*tr->LHEWeights).at(i));
                        if(ID > 474 && ID < 575) pdfweights.push_back((*tr->LHEWeights).at(i));
                        if(ID > 442 && ID < 474) pdfWeights4LHC.push_back((*tr->LHEWeights).at(i));
                        if(ID > 205 && ID < 247) pdfWeightsMSTW.push_back((*tr->LHEWeights).at(i));
                  }
                  else if(std::find(madgraph_samples.begin(), madgraph_samples.end(), sample)!=madgraph_samples.end() ){
                        if((ID > 1 && ID < 10) && !(ID==6 || ID==8)) renormWeights.push_back((*tr->LHEWeights).at(i));
                        if(ID > 714 && ID < 746) pdfWeights4LHC.push_back((*tr->LHEWeights).at(i));
                        if(ID > 10 && ID < 111) pdfweights.push_back((*tr->LHEWeights).at(i));
                  }
                  else if(std::find(powheg_samples.begin(), powheg_samples.end(), sample)!=powheg_samples.end() ){
                        if((ID > 1001 && ID < 1010) && !(ID==1006 || ID==1008)) renormWeights.push_back((*tr->LHEWeights).at(i));
                        if(ID > 10999 && ID < 11031) pdfWeights4LHC.push_back((*tr->LHEWeights).at(i));
                        if(ID > 2000 && ID < 2101) pdfweights.push_back((*tr->LHEWeights).at(i));
                  }
                  else {
                        if((ID > 1001 && ID < 1010) && !(ID==1006 || ID==1008)) renormWeights.push_back((*tr->LHEWeights).at(i));
                        if(ID > 1714 && ID < 1745) pdfWeights4LHC.push_back((*tr->LHEWeights).at(i));
                        if(ID > 1010 && ID < 1111) pdfweights.push_back((*tr->LHEWeights).at(i));
                  }

                // original code (fill hist)
                if(ID==1002 || ID==1003 || ID==1004 || ID==1005 || ID==1007 || ID==1009 || ID==2 || ID==3 || ID==4 || ID==5 || ID==7 || ID==9){
                  hist_scaleHT->Fill((*tr->LHEWeights).at(i),st);//fill combined
                  //now fill individual
                  if(ID==1002 || (outname.find("prime")!=std::string::npos && ID==2) )hist_scaleHT_1002->Fill(st,(*tr->LHEWeights).at(i)*mcweight); //edited by rizki
                  if(ID==1003 || (outname.find("prime")!=std::string::npos && ID==3) )hist_scaleHT_1003->Fill(st,(*tr->LHEWeights).at(i)*mcweight); //edited by rizki
                  if(ID==1004 || (outname.find("prime")!=std::string::npos && ID==4) )hist_scaleHT_1004->Fill(st,(*tr->LHEWeights).at(i)*mcweight); //edited by rizki
                  if(ID==1005 || (outname.find("prime")!=std::string::npos && ID==5) )hist_scaleHT_1005->Fill(st,(*tr->LHEWeights).at(i)*mcweight); //edited by rizki
                  if(ID==1007 || (outname.find("prime")!=std::string::npos && ID==7) )hist_scaleHT_1007->Fill(st,(*tr->LHEWeights).at(i)*mcweight); //edited by rizki
                  if(ID==1009 || (outname.find("prime")!=std::string::npos && ID==9) )hist_scaleHT_1009->Fill(st,(*tr->LHEWeights).at(i)*mcweight); //edited by rizki
                }
                //if(!( (ID>2000 && i<2101 && outname.find("prime")==std::string::npos ) || (outname.find("prime")!=std::string::npos && ( ID> 111 && ID <212)) ) ) continue; //edited by rizki
                //hist_pdfHT->Fill((*tr->LHEWeights).at(i),st);

                //added by rizki - start
                //if (ID > 111 && ID < 212){
                //        pdf_hist->Fill(ID-111.5, st, totalweight*(*tr->LHEWeights).at(i));
                //        // counting up weighted yields for each pdf variation
                //}
                // If you need nominal in this hist, make it 101,0,101 and do:
                // pdf_hist->Fill(100.5,totalweight);
                //added by rizki - end

      }
      for (unsigned int i=0; i<pdfweights.size(); i++){
           //hist_pdfHT->Fill((*tr->LHEWeights).at(i),st);
           hist_pdfHT->Fill(pdfweights.at(i),st);
           pdf_hist->Fill(i+0.5, st, totalweight*pdfweights.at(i) );
      }

      if(pdfWeights4LHC.size()> 0){
           hist_pdf4LHCHT_nom->Fill(pdfWeights4LHC.at(0),st);
           pdf4LHC_hist->Fill(0.5, st, totalweight*pdfWeights4LHC.at(0));
      }
      for (unsigned int i=1; i<pdfWeights4LHC.size(); i++){
           hist_pdf4LHCHT->Fill(pdfWeights4LHC.at(i),st);
           pdf4LHC_hist->Fill(i+0.5, st, totalweight*pdfWeights4LHC.at(i));
      }

    }

    //now fill nonprompt histos for later use
    if(bg_np){

        if(TL==0) npHistos_all.at(0)->Fill(st,mcweight);
        if(TL==1) npHistos_all.at(1)->Fill(st,mcweight);
        if(TL==2) npHistos_all.at(2)->Fill(st,mcweight);
        if(TL==3) npHistos_all.at(3)->Fill(st,mcweight);

      if(elel){
        if(TL==0) npHistos_ee.at(0)->Fill(st,mcweight);
        if(TL==1) npHistos_ee.at(1)->Fill(st,mcweight);
        if(TL==2) npHistos_ee.at(2)->Fill(st,mcweight);
        if(TL==3) npHistos_ee.at(3)->Fill(st,mcweight);
      }
      if(elmu){
        if(TL==0) npHistos_emu.at(0)->Fill(st,mcweight);
        if(TL==1) npHistos_emu.at(1)->Fill(st,mcweight);
        if(TL==2) npHistos_emu.at(2)->Fill(st,mcweight);
        if(TL==3) npHistos_emu.at(3)->Fill(st,mcweight);
      }
      if(mumu){
        if(TL==0) npHistos_mumu.at(0)->Fill(st,mcweight);
        if(TL==1) npHistos_mumu.at(1)->Fill(st,mcweight);
        if(TL==2) npHistos_mumu.at(2)->Fill(st,mcweight);
        if(TL==3) npHistos_mumu.at(3)->Fill(st,mcweight);
      }
    }//end filling of nphistos

  }//end event loop

  //write the trees
  tm_ss->tree->Write();
  tm_ssdl->tree->Write();
  tm_sZVeto->tree->Write();
  tm_DilepMassCut->tree->Write();

  //write the nonprompt histograms
  for(unsigned int j=0; j<4; j++){
    fsig->WriteTObject(npHistos_all.at(j));
    fsig->WriteTObject(npHistos_ee.at(j));
    fsig->WriteTObject(npHistos_emu.at(j));
    fsig->WriteTObject(npHistos_mumu.at(j));
  }

  //write histograms
  writeHistos(fsig, hists_ssdl_all);
  writeHistos(fsig, hists_ssdl_elel);
  writeHistos(fsig, hists_ssdl_elmu);
  writeHistos(fsig, hists_ssdl_mumu);

  writeHistos(fsig, hists_sZVeto_all);
  writeHistos(fsig, hists_sZVeto_elel);
  writeHistos(fsig, hists_sZVeto_elmu);
  writeHistos(fsig, hists_sZVeto_mumu);

  writeHistos(fsig, hists_TwoJets_all);
  writeHistos(fsig, hists_TwoJets_elel);
  writeHistos(fsig, hists_TwoJets_elmu);
  writeHistos(fsig, hists_TwoJets_mumu);

  writeHistos(fsig, hists_nConst_all);
  writeHistos(fsig, hists_nConst_elel);
  writeHistos(fsig, hists_nConst_elmu);
  writeHistos(fsig, hists_nConst_mumu);

  fsig->WriteTObject(h_DoubleEle33Num);
  fsig->WriteTObject(h_DoubleEle33_MWNum);
  fsig->WriteTObject(h_Ele27WP85Num);
  fsig->WriteTObject(h_Mu30TkMu11Num);
  fsig->WriteTObject(h_Mu40Num);
  fsig->WriteTObject(h_IsoTkMu24Num);
  fsig->WriteTObject(h_DoubleMu33NoFiltersNoVtxNum);
  //fsig->WriteTObject(h_Mu23Ele12Num);
  //fsig->WriteTObject(h_Mu8Ele23Num);
  //fsig->WriteTObject(h_PFHT900Num);


  fsig->WriteTObject(h_DoubleEle33Den);
  fsig->WriteTObject(h_DoubleEle33_MWDen);
  fsig->WriteTObject(h_Ele27WP85Den);
  fsig->WriteTObject(h_Mu30TkMu11Den);
  fsig->WriteTObject(h_Mu40Den);
  fsig->WriteTObject(h_IsoTkMu24Den);
  fsig->WriteTObject(h_DoubleMu33NoFiltersNoVtxDen);
  //fsig->WriteTObject(h_Mu23Ele12Den);
  //fsig->WriteTObject(h_Mu8Ele23Den);
  //fsig->WriteTObject(h_PFHT900Den);


  fsig->WriteTObject(hist_pdfHT);
  fsig->WriteTObject(hist_pdf4LHCHT_nom);
  fsig->WriteTObject(hist_pdf4LHCHT);
  fsig->WriteTObject(pdf_hist);
  fsig->WriteTObject(pdf4LHC_hist);
  fsig->WriteTObject(hist_scaleHT);
  fsig->WriteTObject(hist_scaleHT_nom);
  fsig->WriteTObject(hist_scaleHT_1002);
  fsig->WriteTObject(hist_scaleHT_1003);
  fsig->WriteTObject(hist_scaleHT_1004);
  fsig->WriteTObject(hist_scaleHT_1005);
  fsig->WriteTObject(hist_scaleHT_1007);
  fsig->WriteTObject(hist_scaleHT_1009);

  fsig->WriteTObject(hist_scaleHT_ssdl_nom);
  fsig->WriteTObject(hist_scaleHT_ssdl_1002);
  fsig->WriteTObject(hist_scaleHT_ssdl_1003);
  fsig->WriteTObject(hist_scaleHT_ssdl_1004);
  fsig->WriteTObject(hist_scaleHT_ssdl_1005);
  fsig->WriteTObject(hist_scaleHT_ssdl_1007);
  fsig->WriteTObject(hist_scaleHT_ssdl_1009);

  delete tr;

  /////////////////////////////////////////////// JEC/JECR Trees ///////////////////////////////////////////////
  if(bg_mc || signal){
    std::vector<TString> TreesPostFix{"_JECup","_JECdown","_JERup","_JERdown"};
    for (auto & postFix : TreesPostFix){
      TreeReader* tr_shift;
      tr_shift = new TreeReader(filename.c_str(),"ljmet"+postFix+"/ljmet"+postFix,true,true);

      TTree* t_shift = tr_shift->tree;

      fsig->cd();

      nEntries = t_shift->GetEntries();

      nMuMu=0;
      nElMu=0;
      nElEl=0;

      for(int ient=0; ient<nEntries; ient++){

        if(ient % 500 ==0) std::cout<<"Completed "<<ient<<" out of "<<nEntries<<" "<< postFix <<" events"<<std::endl;

        tr_shift->GetEntry(ient);

        // lumi mask for 2017 ONLY
        if(data && ((tr_shift->run==299480 && tr_shift->lumi==7) || (tr_shift->run==301397 && tr_shift->lumi==518) || (tr_shift->run==305366 && tr_shift->lumi==395))) continue;

        //weight for non prompt method
        float NPweight=0;
        float NPAltWeight=0;
        float NPSUSYWeight=0;
        int TL;

        std::vector<TLepton*> goodLeptons;
        goodLeptons = makeLeptons(tr_shift->allMuons, tr_shift->allElectrons,30.0,elID,muID,bg_np);

        if(signal){ //check TT/BB sig Decay - added by rizki
                if(sigDecay=="TZTZ" && tr_shift->isTZTZ==false){continue ;}
                if(sigDecay=="TZTH" && tr_shift->isTZTH==false){continue ;}
                if(sigDecay=="TZBW" && tr_shift->isTZBW==false){continue ;}
                if(sigDecay=="THTH" && tr_shift->isTHTH==false){continue ;}
                if(sigDecay=="THBW" && tr_shift->isTHBW==false){continue ;}
                if(sigDecay=="BWBW" && tr_shift->isBWBW==false){continue ;}

                if(sigDecay=="BZBZ" && tr_shift->isBZBZ==false){continue ;}
                if(sigDecay=="BZBH" && tr_shift->isBZBH==false){continue ;}
                if(sigDecay=="BZTW" && tr_shift->isBZTW==false){continue ;}
                if(sigDecay=="BHBH" && tr_shift->isBHBH==false){continue ;}
                if(sigDecay=="BHTW" && tr_shift->isBHTW==false){continue ;}
                if(sigDecay=="TWTW" && tr_shift->isTWTW==false){continue ;}
        }

        //reorder the leptons by pt to remove flavor ordering
        std::sort(goodLeptons.begin(),goodLeptons.end(),sortByPt);
        bool samesign;

        //get chargeMisID rate for DY and save DY events outside of Z-peak (71-111 GeV) with weights for chargeMisID
        if(outname.find("DYJets")!=std::string::npos || outname.find("ChargeMisID")!=std::string::npos){

            samesign = checkOppositeSignLeptonsForDY(goodLeptons); //returns true if find opposite sign leptons not in mu-mu channel
        }      
        else{ //now that we have good leptons, if it's not DY sample just check for two with same-sign charge and assign weight of 1
            samesign = checkSameSignLeptons(goodLeptons);
        }

        if(!samesign) continue;

        //now make vector of same-sign leptons, for DY make vector containing opposite sign leptons closest to Z mass
        std::vector<TLepton*> vSSLep;
        if(outname.find("DYJets")!=std::string::npos || outname.find("ChargeMisID")!=std::string::npos){
            vSSLep = makeOSLeptonsForDY(goodLeptons);
        }
        else vSSLep = makeSSLeptons(goodLeptons);

        //dummy check to make sure the vector got filled properly
        assert(vSSLep.size() > 1);
        //make sure both are tight if not doing fakes background
        if(!bg_np){
            if( !(vSSLep.at(0)->Tight && vSSLep.at(1)->Tight)) continue;
        }

        //now prune the goodleptons of the ssleptons
        std::vector<TLepton*> vNonSSLep = pruneSSLep(goodLeptons,vSSLep); 
        if(vNonSSLep.size()!=0) continue;

        //get channel
        int nMu=0;
        if(vSSLep.at(0)->isMu) nMu++;
        if(vSSLep.at(1)->isMu) nMu++;

        //bools for channels
        bool mumu=false;
        bool elmu=false;
        bool elel=false;

        //ok at this point only have events with same-sign leptons, now let's do simple check to see how many of each channel we have:
        if(vSSLep.at(0)->isMu && vSSLep.at(1)->isMu){ nMuMu+=1; mumu=true;}
        else if( ( vSSLep.at(0)->isEl && vSSLep.at(1)->isMu) || (vSSLep.at(0)->isMu && vSSLep.at(1)->isEl)){ nElMu+=1; elmu=true;}
        else {nElEl+=1; elel=true;}

        //require OR of triggers but make each channel consistent
        bool skip = true;

        if(isoTrig){
            if(mumu && (tr_shift->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v)) skip =false; 
            if(elmu && (tr_shift->HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v || tr_shift->HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v)) skip = false;
            if(elel && (tr_shift->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v)) skip = false; 
        }
        else{
            if(nonIso_HTtrig){ //NonIso_HTtrig
                if(mumu && tr_shift->HLT_DoubleMu4_Mass8_DZ_PFHT350) skip =false;
                if(elmu && (tr_shift->HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT350_DZ_v )) skip = false;
                if(elel && tr_shift->HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_DZ_PFHT350_v) skip = false;
            }
            else{ //NoIsoTrig --> the corressponding single lepton trigger for one of these double triggers below dont exist for 2017/2018 !!
                if(mumu && tr_shift->HLT_Mu37_TkMu27) skip =false;
                if(elmu && (tr_shift->HLT_Mu37Ele27 || tr_shift->HLT_Mu27Ele37)) skip = false;
                if(elel && tr_shift->HLT_Ele27_Ele37) skip = false;
            }
        }

        if(skip) continue;

        //HT requirement if using HT triggers - added by rizki
        float HT=0;
        HT+=vSSLep.at(0)->pt+vSSLep.at(1)->pt;
        for(unsigned int uijet=0; uijet<tr_shift->allAK4Jets.size();uijet++){
          HT+=tr_shift->allAK4Jets.at(uijet)->pt;
        }
        if(nonIso_HTtrig){ if(HT < 400) continue ;}

        //now skip if not the channel from the corresponding dataset
        if((argv1.find("DataMuMu")!=std::string::npos || argv1.find("NonPromptDataMuMu")!=std::string::npos) && !mumu) continue;
        if((argv1.find("DataElMu")!=std::string::npos || argv1.find("NonPromptDataElMu")!=std::string::npos || argv1.find("ChargeMisIDElMu")!=std::string::npos) && !elmu) continue;
        if((argv1.find("DataElEl")!=std::string::npos || argv1.find("NonPromptDataElEl")!=std::string::npos || argv1.find("ChargeMisIDElEl")!=std::string::npos) && !elel) continue;

        //make boolean here so we can fill the closets associated mass
        bool secondaryZVeto = (checkSecondaryZVeto(vSSLep,tr_shift->looseMuons,tr_shift->looseElectrons)).first;
        float assocMass =  (checkSecondaryZVeto(vSSLep,tr_shift->looseMuons,tr_shift->looseElectrons)).second;

        //since we have the two same-sign leptons, now make sure neither of them reconstructs with any other tight lepton in the event to form a Z
        if(secondaryZVeto) continue;

        /////// now get CMID weight
        float weight=1.0;
        float ew1,ew2=0.0;
        //with vector now get weight for DY Events
        if(outname.find("DYJets")!=std::string::npos || outname.find("ChargeMisID")!=std::string::npos) {
          //first set to one if muon
          if(vSSLep.at(0)->isMu){ew1=0.0;}//zero since it's cmid probability
          else{
            if(vSSLep.at(0)->pt>200){
              ew1=getEtaWeight_hhpt(fabs(vSSLep.at(0)->eta),etaWeights_hhpt);
            }
            else if(vSSLep.at(0)->pt>100){
              ew1=getEtaWeight_hpt(fabs(vSSLep.at(0)->eta),etaWeights_hpt);
            }
            else{
              ew1=getEtaWeight_lpt(fabs(vSSLep.at(0)->eta),etaWeights_lpt);
            }
          }
          if(vSSLep.at(1)->isMu){ew2=0.0;} //zero since it's cmid probability
          else{
            if(vSSLep.at(1)->pt>200){
              ew2 = getEtaWeight_hhpt(fabs(vSSLep.at(1)->eta),etaWeights_hhpt);
            }
            else if(vSSLep.at(1)->pt>100){
              ew2 = getEtaWeight_hpt(fabs(vSSLep.at(1)->eta),etaWeights_hpt);
            }
            else{
              ew2 = getEtaWeight_lpt(fabs(vSSLep.at(1)->eta),etaWeights_lpt);
            }
          }
          //now add since these are probabilities
          weight=ew1 + ew2 - ew1*ew2;
        }
        /////// end CMID weight

        /////// now get dilepton mass 
        float dilepMass = (vSSLep.at(0)->lv + vSSLep.at(1)->lv).M();

        /////// get trigger scale factors
        std::vector<float> vTrigSF;
        if(!data) vTrigSF = getTrigSF(vSSLep,era);
        else {vTrigSF.push_back(1.0); vTrigSF.push_back(1.0); vTrigSF.push_back(1.0);}
        float trigSF = vTrigSF.at(0);
        float trigSFup = vTrigSF.at(1);
        float trigSFdn = vTrigSF.at(2);
        /////// end trigger scale factors

        /////// lepton Scale Factors
        std::vector<float> lepIDSF_vec;
        lepIDSF_vec = getLepIDSF(vSSLep);
        float lepIDSF, lepIDSFup, lepIDSFdn;
        if(data) {lepIDSF = 1.0; lepIDSFup = 1.0; lepIDSFdn = 1.0;}
        else {lepIDSF = lepIDSF_vec.at(0); lepIDSFup = lepIDSF_vec.at(1); lepIDSFdn = lepIDSF_vec.at(2);}

        float lepIsoSF;
        if(data) lepIsoSF = 1.0;
        else lepIsoSF = getLepIsoSF(vSSLep);

        float gsfSF = 1.0;
        if(data) gsfSF=1.0;
        else{
          gsfSF=1.0;//set to one currently
          if(vSSLep.at(0)->isEl) gsfSF*= getGsfSF(vSSLep.at(0));
          if(vSSLep.at(1)->isEl) gsfSF*= getGsfSF(vSSLep.at(1));
        }
        /////// end lepton Scale Factors
        
        /////// now need to calculate nonPromptWeight - first get prompt rate for each lepton
        float lep1PromptRate=0.0;
        float lep2PromptRate=0.0;
        if(vSSLep.at(0)->isMu){
          if(vSSLep.at(0)->pt<40) lep1PromptRate=muPromptRates.at(0);
          else if(vSSLep.at(0)->pt<50) lep1PromptRate=muPromptRates.at(1);
          else if(vSSLep.at(0)->pt<60) lep1PromptRate=muPromptRates.at(2);
          else if(vSSLep.at(0)->pt<70) lep1PromptRate=muPromptRates.at(3);
          else if(vSSLep.at(0)->pt<80) lep1PromptRate=muPromptRates.at(4);
          else if(vSSLep.at(0)->pt<90) lep1PromptRate=muPromptRates.at(5);
          else if(vSSLep.at(0)->pt<100) lep1PromptRate=muPromptRates.at(6);
          else if(vSSLep.at(0)->pt<125) lep1PromptRate=muPromptRates.at(7);
          else if(vSSLep.at(0)->pt<150) lep1PromptRate=muPromptRates.at(8);
          else if(vSSLep.at(0)->pt<200) lep1PromptRate=muPromptRates.at(9);
          else if(vSSLep.at(0)->pt<300) lep1PromptRate=muPromptRates.at(10);
          else if(vSSLep.at(0)->pt<400) lep1PromptRate=muPromptRates.at(11);
          else if(vSSLep.at(0)->pt<500) lep1PromptRate=muPromptRates.at(12);
          else lep1PromptRate = muPromptRates.at(13);    
        }
        else{
          if(vSSLep.at(0)->pt<40) lep1PromptRate=elPromptRates.at(0);
          else if(vSSLep.at(0)->pt<50) lep1PromptRate=elPromptRates.at(1);
          else if(vSSLep.at(0)->pt<60) lep1PromptRate=elPromptRates.at(2);
          else if(vSSLep.at(0)->pt<70) lep1PromptRate=elPromptRates.at(3);
          else if(vSSLep.at(0)->pt<80) lep1PromptRate=elPromptRates.at(4);
          else if(vSSLep.at(0)->pt<90) lep1PromptRate=elPromptRates.at(5);
          else if(vSSLep.at(0)->pt<100) lep1PromptRate=elPromptRates.at(6);
          else if(vSSLep.at(0)->pt<125) lep1PromptRate=elPromptRates.at(7);
          else if(vSSLep.at(0)->pt<150) lep1PromptRate=elPromptRates.at(8);
          else if(vSSLep.at(0)->pt<200) lep1PromptRate=elPromptRates.at(9);
          else if(vSSLep.at(0)->pt<300) lep1PromptRate=elPromptRates.at(10);
          else if(vSSLep.at(0)->pt<400) lep1PromptRate=elPromptRates.at(11);
          else if(vSSLep.at(0)->pt<500) lep1PromptRate=elPromptRates.at(12);
          else lep1PromptRate = elPromptRates.at(13);
        }
        if(vSSLep.at(1)->isMu){
          if(vSSLep.at(1)->pt<40) lep2PromptRate=muPromptRates.at(0);
          else if(vSSLep.at(1)->pt<50) lep2PromptRate=muPromptRates.at(1);
          else if(vSSLep.at(1)->pt<60) lep2PromptRate=muPromptRates.at(2);
          else if(vSSLep.at(1)->pt<70) lep2PromptRate=muPromptRates.at(3);
          else if(vSSLep.at(1)->pt<80) lep2PromptRate=muPromptRates.at(4);
          else if(vSSLep.at(1)->pt<90) lep2PromptRate=muPromptRates.at(5);
          else if(vSSLep.at(1)->pt<100) lep2PromptRate=muPromptRates.at(6);
          else if(vSSLep.at(1)->pt<125) lep2PromptRate=muPromptRates.at(7);
          else if(vSSLep.at(1)->pt<150) lep2PromptRate=muPromptRates.at(8);
          else if(vSSLep.at(1)->pt<200) lep2PromptRate=muPromptRates.at(9);
          else if(vSSLep.at(1)->pt<300) lep2PromptRate=muPromptRates.at(10);
          else if(vSSLep.at(1)->pt<400) lep2PromptRate=muPromptRates.at(11);
          else if(vSSLep.at(1)->pt<500) lep2PromptRate=muPromptRates.at(12);
          else lep2PromptRate = muPromptRates.at(13);    
        }
        else {
          if(vSSLep.at(1)->pt<40) lep2PromptRate=elPromptRates.at(0);
          else if(vSSLep.at(1)->pt<50) lep2PromptRate=elPromptRates.at(1);
          else if(vSSLep.at(1)->pt<60) lep2PromptRate=elPromptRates.at(2);
          else if(vSSLep.at(1)->pt<70) lep2PromptRate=elPromptRates.at(3);
          else if(vSSLep.at(1)->pt<80) lep2PromptRate=elPromptRates.at(4);
          else if(vSSLep.at(1)->pt<90) lep2PromptRate=elPromptRates.at(5);
          else if(vSSLep.at(1)->pt<100) lep2PromptRate=elPromptRates.at(6);
          else if(vSSLep.at(1)->pt<125) lep2PromptRate=elPromptRates.at(7);
          else if(vSSLep.at(1)->pt<150) lep2PromptRate=elPromptRates.at(8);
          else if(vSSLep.at(1)->pt<200) lep2PromptRate=elPromptRates.at(9);
          else if(vSSLep.at(1)->pt<300) lep2PromptRate=elPromptRates.at(10);
          else if(vSSLep.at(1)->pt<400) lep2PromptRate=elPromptRates.at(11);
          else if(vSSLep.at(1)->pt<500) lep2PromptRate=elPromptRates.at(12);
          else lep2PromptRate = elPromptRates.at(13);
        }

        //now get fake rate for each lepton
        float lep1FakeRate = 0.0;
        float lep2FakeRate = 0.0;
        if(vSSLep.at(0)->isMu){
          if(vSSLep.at(0)->eta < -2.1) lep1FakeRate=muFakeRates.at(0);
          else if(vSSLep.at(0)->eta < -1.2) lep1FakeRate=muFakeRates.at(1);
          else if(vSSLep.at(0)->eta < -0.9) lep1FakeRate=muFakeRates.at(2);
          else if(vSSLep.at(0)->eta < -0.4) lep1FakeRate=muFakeRates.at(3);
          else if(vSSLep.at(0)->eta < 0.4) lep1FakeRate=muFakeRates.at(4);
          else if(vSSLep.at(0)->eta < 0.9) lep1FakeRate=muFakeRates.at(5);
          else if(vSSLep.at(0)->eta < 1.2) lep1FakeRate=muFakeRates.at(6);
          else if(vSSLep.at(0)->eta < 2.1) lep1FakeRate=muFakeRates.at(7);
          else lep1FakeRate=muFakeRates.at(8);
        }
        else{ //leading is electron
          if(vSSLep.at(0)->eta < -1.566) lep1FakeRate=elFakeRates.at(0);
          else if(vSSLep.at(0)->eta < -0.8) lep1FakeRate=elFakeRates.at(1);
          else if(vSSLep.at(0)->eta < -0.4) lep1FakeRate=elFakeRates.at(2);
          else if(vSSLep.at(0)->eta < 0.4) lep1FakeRate=elFakeRates.at(3);
          else if(vSSLep.at(0)->eta < 0.8) lep1FakeRate=elFakeRates.at(4);
          else if(vSSLep.at(0)->eta < 1.566) lep1FakeRate=elFakeRates.at(5);
          else lep1FakeRate=elFakeRates.at(6);
        }
        //now for subleading
        if(vSSLep.at(1)->isMu){
          if(vSSLep.at(1)->eta < -2.1) lep2FakeRate=muFakeRates.at(0);
          else if(vSSLep.at(1)->eta < -1.2) lep2FakeRate=muFakeRates.at(1);
          else if(vSSLep.at(1)->eta < -0.9) lep2FakeRate=muFakeRates.at(2);
          else if(vSSLep.at(1)->eta < -0.4) lep2FakeRate=muFakeRates.at(3);
          else if(vSSLep.at(1)->eta < 0.4) lep2FakeRate=muFakeRates.at(4);
          else if(vSSLep.at(1)->eta < 0.9) lep2FakeRate=muFakeRates.at(5);
          else if(vSSLep.at(1)->eta < 1.2) lep2FakeRate=muFakeRates.at(6);
          else if(vSSLep.at(1)->eta < 2.1) lep2FakeRate=muFakeRates.at(7);
          else lep2FakeRate=muFakeRates.at(8);
        }
        else{ //leading is electron
          if(vSSLep.at(1)->eta < -1.566) lep2FakeRate=elFakeRates.at(0);
          else if(vSSLep.at(1)->eta < -0.8) lep2FakeRate=elFakeRates.at(1);
          else if(vSSLep.at(1)->eta < -0.4) lep2FakeRate=elFakeRates.at(2);
          else if(vSSLep.at(1)->eta < 0.4) lep2FakeRate=elFakeRates.at(3);
          else if(vSSLep.at(1)->eta < 0.8) lep2FakeRate=elFakeRates.at(4);
          else if(vSSLep.at(1)->eta < 1.566) lep2FakeRate=elFakeRates.at(5);
          else lep2FakeRate=elFakeRates.at(6);
        }

        if(!bg_np) {NPweight=1.0;NPAltWeight=1.0;NPSUSYWeight=1.0;TL=-1;}
        else{ //**********get susy alternative weight
          if(vSSLep.at(0)->Tight && vSSLep.at(1)->Tight) NPSUSYWeight=0;
          else if(vSSLep.at(0)->Tight && !(vSSLep.at(1)->Tight)) NPSUSYWeight = lep2FakeRate / (1.0 - lep2FakeRate);
          else if(!(vSSLep.at(0)->Tight) && vSSLep.at(1)->Tight) NPSUSYWeight = lep1FakeRate / (1.0 - lep1FakeRate);
          else NPSUSYWeight = (lep1FakeRate / (1 - lep1FakeRate)) * ( lep2FakeRate/ (1 - lep2FakeRate));

          if(mumu || elel){ // kinematic dependent fake/prompt rates for same flavor channels can treat as 'different' flavors where flavor is now leading/subleading
            if(vSSLep.at(0)->Tight && vSSLep.at(1)->Tight) {NPweight = WeightOF_T11(lep1PromptRate,lep1FakeRate,lep2PromptRate,lep2FakeRate); TL = 3;}//both tight
            else if(vSSLep.at(0)->Tight && !(vSSLep.at(1)->Tight)) {NPweight = WeightOF_T10(lep1PromptRate,lep1FakeRate,lep2PromptRate,lep2FakeRate); TL=2;}//leading tight
            else if(!(vSSLep.at(0)->Tight) && vSSLep.at(1)->Tight) {NPweight = WeightOF_T01(lep1PromptRate,lep1FakeRate,lep2PromptRate,lep2FakeRate); TL=1;}//subleading tight
            else {NPweight = WeightOF_T00(lep1PromptRate,lep1FakeRate,lep2PromptRate,lep2FakeRate); TL=0;}//both loose
            NPAltWeight=NPweight; //make alt weight the same as above
          }
          else{ //cross channel - still important to save which flavor is which, kinematic effects happen 'in background'
            //******** Making alternative weight***********
            if(vSSLep.at(0)->Tight && vSSLep.at(1)->Tight) {NPAltWeight = WeightOF_T11(lep1PromptRate,lep1FakeRate,lep2PromptRate,lep2FakeRate); TL = 3;}//both tight
            else if(vSSLep.at(0)->Tight && !(vSSLep.at(1)->Tight)) {NPAltWeight = WeightOF_T10(lep1PromptRate,lep1FakeRate,lep2PromptRate,lep2FakeRate); TL=2;}//leading tight
            else if(!(vSSLep.at(0)->Tight) && vSSLep.at(1)->Tight) {NPAltWeight = WeightOF_T01(lep1PromptRate,lep1FakeRate,lep2PromptRate,lep2FakeRate); TL=1;}//subleading tight
            else {NPAltWeight = WeightOF_T00(lep1PromptRate,lep1FakeRate,lep2PromptRate,lep2FakeRate); TL=0;}//both loose
            //******** Making origin np weight ************
            float muPR,muFR,elPR,elFR;
            if(vSSLep.at(0)->isMu){muPR=lep1PromptRate;muFR=lep1FakeRate;elPR=lep2PromptRate;elFR=lep2FakeRate;} //for cross channel finding flavor of leading determines flavor of subleading
            else {muPR=lep2PromptRate;muFR=lep2FakeRate;elPR=lep1PromptRate;elFR=lep1FakeRate;}
            if(vSSLep.at(0)->Tight && vSSLep.at(1)->Tight) {NPweight = WeightOF_T11(elPR,elFR,muPR,muFR); TL=3;} //both tight
            else if ( (vSSLep.at(0)->isEl && vSSLep.at(0)->Tight) || (vSSLep.at(1)->isEl && vSSLep.at(1)->Tight) )  {NPweight = WeightOF_T10(elPR,elFR,muPR,muFR);TL=2;} //if electron is tight, muon must be loose or we would be on line above so just see if either lepton is a tight electron
            else if ( (vSSLep.at(0)->isMu && vSSLep.at(0)->Tight) || (vSSLep.at(1)->isMu && vSSLep.at(1)->Tight) ) {NPweight = WeightOF_T01(elPR,elFR,muPR,muFR); TL=1;}//if muon is tight, el must be loose or we would be on tight-tight line so just check for tight muon
            else {NPweight = WeightOF_T00(elPR,elFR,muPR,muFR); TL=0;}//otherwise both are loose
          }
        }
        ///////  end np weight

        ///////get pileup weight;
        float puweight=-11;
        float puweightUp=-11;
        float puweightDown=-11;
        if(data) puweight=1;
        else{
          int nTrueInteractions = tr_shift->nPU;
          if (tr_shift->nPU > 99) nTrueInteractions = 99;
          if (signal && tr_shift->nPU > 79) nTrueInteractions = 79;
          if (tr_shift->nPU < 0 ) nTrueInteractions = 0;
          puweight = getPUWeight(sampleName, nTrueInteractions, 0);
          puweightUp = getPUWeight(sampleName, nTrueInteractions, 1);
          puweightDown = getPUWeight(sampleName, nTrueInteractions, -1);
        }
        /////// end pileup weight

        /////// make dummy pdfweights vector for FillTree
        std::vector<double> pdfweights_ss(100,1.0); 
        std::vector<double> renormWeights_ss(6,1.0);
        std::vector<double> pdfNewWeights_ss(100,1.0);
        std::vector<double> pdfWeights4LHC_ss(31,1.0);
        std::vector<double> pdfWeightsMSTW_ss(41,1.0);
        double pdfNewNominalWeight_ss = 1.0;
        /////// end dummy pdf weight

        //fill tree for post secondary z veto
        if(postFix == "_JECup") tm_sZVeto_JECup->FillTree(vSSLep, tr_shift->allAK4Jets, tr_shift->cleanedAK4Jets, tr_shift->simpleCleanedAK4Jets, HT, tr_shift->MET, dilepMass,nMu,weight,vNonSSLep,tr_shift->MCWeight,NPweight,NPAltWeight,NPSUSYWeight,TL,trigSF,trigSFup,trigSFdn,lepIDSF,lepIDSFup,lepIDSFdn,lepIsoSF,gsfSF,puweight,puweightUp,puweightDown,assocMass,tr_shift->allAK8Jets,tr_shift->hadronicGenJets,!data,tr_shift->run,tr_shift->lumi,tr_shift->event,tr_shift->nPrimaryVert, pdfweights_ss, renormWeights_ss, pdfNewWeights_ss, pdfWeights4LHC_ss, pdfWeightsMSTW_ss, pdfNewNominalWeight_ss);
        if(postFix == "_JECdown") tm_sZVeto_JECdn->FillTree(vSSLep, tr_shift->allAK4Jets, tr_shift->cleanedAK4Jets, tr_shift->simpleCleanedAK4Jets, HT, tr_shift->MET, dilepMass,nMu,weight,vNonSSLep,tr_shift->MCWeight,NPweight,NPAltWeight,NPSUSYWeight,TL,trigSF,trigSFup,trigSFdn,lepIDSF,lepIDSFup,lepIDSFdn,lepIsoSF,gsfSF,puweight,puweightUp,puweightDown,assocMass,tr_shift->allAK8Jets,tr_shift->hadronicGenJets,!data,tr_shift->run,tr_shift->lumi,tr_shift->event,tr_shift->nPrimaryVert, pdfweights_ss, renormWeights_ss, pdfNewWeights_ss, pdfWeights4LHC_ss, pdfWeightsMSTW_ss, pdfNewNominalWeight_ss);
        if(postFix == "_JERup") tm_sZVeto_JERup->FillTree(vSSLep, tr_shift->allAK4Jets, tr_shift->cleanedAK4Jets, tr_shift->simpleCleanedAK4Jets, HT, tr_shift->MET, dilepMass,nMu,weight,vNonSSLep,tr_shift->MCWeight,NPweight,NPAltWeight,NPSUSYWeight,TL,trigSF,trigSFup,trigSFdn,lepIDSF,lepIDSFup,lepIDSFdn,lepIsoSF,gsfSF,puweight,puweightUp,puweightDown,assocMass,tr_shift->allAK8Jets,tr_shift->hadronicGenJets,!data,tr_shift->run,tr_shift->lumi,tr_shift->event,tr_shift->nPrimaryVert, pdfweights_ss, renormWeights_ss, pdfNewWeights_ss, pdfWeights4LHC_ss, pdfWeightsMSTW_ss, pdfNewNominalWeight_ss);
        if(postFix == "_JERdown") tm_sZVeto_JERdn->FillTree(vSSLep, tr_shift->allAK4Jets, tr_shift->cleanedAK4Jets, tr_shift->simpleCleanedAK4Jets, HT, tr_shift->MET, dilepMass,nMu,weight,vNonSSLep,tr_shift->MCWeight,NPweight,NPAltWeight,NPSUSYWeight,TL,trigSF,trigSFup,trigSFdn,lepIDSF,lepIDSFup,lepIDSFdn,lepIsoSF,gsfSF,puweight,puweightUp,puweightDown,assocMass,tr_shift->allAK8Jets,tr_shift->hadronicGenJets,!data,tr_shift->run,tr_shift->lumi,tr_shift->event,tr_shift->nPrimaryVert, pdfweights_ss, renormWeights_ss, pdfNewWeights_ss, pdfWeights4LHC_ss, pdfWeightsMSTW_ss, pdfNewNominalWeight_ss);
      }//end JEC/JER event loop
      delete tr_shift;
    }//end postFix loop 
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////

  tm_sZVeto_JECup->tree->Write();
  tm_sZVeto_JECdn->tree->Write();
  tm_sZVeto_JERup->tree->Write();
  tm_sZVeto_JERdn->tree->Write();  

  fsig->Write();
  fsig->Close();

  //std::cout<<"Number of Gen MuMu: "<<nGenMuMu<<std::endl;
  std::cout<<"Number of MuMu: "<<nMuMu<<std::endl;
  std::cout<<"Number of ElMu: "<<nElMu<<std::endl;
  std::cout<<"Number of ElEl: "<<nElEl<<std::endl;


  std::cout<<"Efficiency of Mu40 for MuMu: "<<nMu40/(nMuMu)<<std::endl;
  std::cout<<"Efficiency of Mu40 for MuMu+ElMu: "<<nMu40/(nMuMu + nElMu)<<std::endl;
  std::cout<<"Efficiency of IsoTkmu24 for MuMu: "<<nIsoTkMu24/(nMuMu)<<std::endl;
  std::cout<<"Efficiency of IsoTkmu24 for MuMu+ElMu: "<<nIsoTkMu24/(nMuMu+nElMu)<<std::endl;

  TH1F* hNMu30TkMu11Num = new TH1F("hNMu30TkMu11Num","",1,0,10000);
  TH1F* hNMu30TkMu11Den = new TH1F("hNMu30TkMu11Den","",1,0,10000);
  for(int j =0; j<nMu30TkMu11;j++) hNMu30TkMu11Num->Fill(j);
  for(int j =0; j<nMuMu;j++) hNMu30TkMu11Den->Fill(j);
  TGraphAsymmErrors* Mu30TkMu11Graph = new TGraphAsymmErrors(hNMu30TkMu11Num,hNMu30TkMu11Den);
  std::cout<<"Efficiency of Mu30TkMu11: "<<nMu30TkMu11/nMuMu<<" +"<<Mu30TkMu11Graph->GetErrorYhigh(0)<<" -"<<Mu30TkMu11Graph->GetErrorYlow(0)<<std::endl<<"\n";
  std::cout<<"Efficiency of Ele27 for ElEl: "<<nEle27/nElEl<<std::endl;
  std::cout<<"Efficiency of Ele27 for ElEl + ElMu: "<<nEle27/(nElMu +nElEl)<<std::endl;

  TH1F* hNMu27TkMu8Num = new TH1F("hNMu27TkMu8Num","",1,0,10000);
  TH1F* hNMu27TkMu8Den = new TH1F("hNMu27TkMu8Den","",1,0,10000);
  for(int j =0; j<nMu27TkMu8;j++) hNMu27TkMu8Num->Fill(j);
  for(int j =0; j<nMuMu;j++) hNMu27TkMu8Den->Fill(j);
  TGraphAsymmErrors* Mu27TkMu8Graph = new TGraphAsymmErrors(hNMu27TkMu8Num,hNMu27TkMu8Den);
  std::cout<<"Efficiency of Mu27TkMu8: "<<nMu27TkMu8/nMuMu<<" +"<<Mu27TkMu8Graph->GetErrorYhigh(0)<<" -"<<Mu27TkMu8Graph->GetErrorYlow(0)<<std::endl<<"\n";

  TH1F* hNDoubleEle33Num = new TH1F("hNDoubleEle33Num","",1,0,10000);
  TH1F* hNDoubleEle33Den = new TH1F("hNDoubleEle33Den","",1,0,10000);
  for(int j =0; j<nDoubleEle33;j++) hNDoubleEle33Num->Fill(j);
  for(int j =0; j<nElEl;j++) hNDoubleEle33Den->Fill(j);
  TGraphAsymmErrors* DoubleEle33Graph = new TGraphAsymmErrors(hNDoubleEle33Num,hNDoubleEle33Den);
  std::cout<<"Efficiency of DoubleEle33: "<<nDoubleEle33/nElEl<<" +"<<DoubleEle33Graph->GetErrorYhigh(0)<<" -"<<DoubleEle33Graph->GetErrorYlow(0)<<std::endl<<"\n";

  TH1F* hNEle17Ele12Num = new TH1F("hNEle17Ele12Num","",1,0,10000);
  TH1F* hNEle17Ele12Den = new TH1F("hNEle17Ele12Den","",1,0,10000);
  for(int j =0; j<nEle17Ele12;j++) hNEle17Ele12Num->Fill(j);
  for(int j =0; j<nElEl;j++) hNEle17Ele12Den->Fill(j);
  TGraphAsymmErrors* Ele17Ele12Graph = new TGraphAsymmErrors(hNEle17Ele12Num,hNEle17Ele12Den);
  std::cout<<"Efficiency of Ele17Ele12: "<<nEle17Ele12/nElEl<<" +"<<Ele17Ele12Graph->GetErrorYhigh(0)<<" -"<<Ele17Ele12Graph->GetErrorYlow(0)<<std::endl;
  std::cout<<"Efficiency of Ele17Ele12 OR DoubleEle8HT300: "<<nEle17Ele12ORDoubleEle8HT300/nElEl<<"\n";
  std::cout<<"Efficiency of Ele17Ele12 at HT > 1000: "<<nEle17Ele12_highHT/nElEl<<"\n\n";
  std::cout<<"Efficiency of Ele17Ele12 OR DoubleEle8HT300 at HT >1000: "<<nEle17Ele12ORDoubleEle8HT300_highHT/nElEl<<"\n\n";

  TH1F* hNMu30Ele30Num = new TH1F("hNMu30Ele30Num","",1,0,10000);
  TH1F* hNMu30Ele30Den = new TH1F("hNMu30Ele30Den","",1,0,10000);
  for(int j =0; j<nMu30Ele30;j++) hNMu30Ele30Num->Fill(j);
  for(int j =0; j<nElMu;j++) hNMu30Ele30Den->Fill(j);
  TGraphAsymmErrors* Mu30Ele30Graph = new TGraphAsymmErrors(hNMu30Ele30Num,hNMu30Ele30Den);
  std::cout<<"Efficiency of Mu30Ele30: "<<nMu30Ele30/nElMu<<" +"<<Mu30Ele30Graph->GetErrorYhigh(0)<<" -"<<Mu30Ele30Graph->GetErrorYlow(0)<<std::endl<<"\n";

  TH1F* hNMu8Ele17Num = new TH1F("hNMu8Ele17Num","",1,0,10000);
  TH1F* hNMu8Ele17Den = new TH1F("hNMu8Ele17Den","",1,0,10000);
  for(int j =0; j<nMu8Ele17;j++) hNMu8Ele17Num->Fill(j);
  for(int j =0; j<nElMu;j++) hNMu8Ele17Den->Fill(j);
  TGraphAsymmErrors* Mu8Ele17Graph = new TGraphAsymmErrors(hNMu8Ele17Num,hNMu8Ele17Den);
  std::cout<<"Efficiency of Mu8Ele17: "<<nMu8Ele17/nElMu<<" +"<<Mu8Ele17Graph->GetErrorYhigh(0)<<" -"<<Mu8Ele17Graph->GetErrorYlow(0)<<std::endl<<"\n";

  TH1F* hNMu8Ele23Num = new TH1F("hNMu8Ele23Num","",1,0,10000);
  TH1F* hNMu8Ele23Den = new TH1F("hNMu8Ele23Den","",1,0,10000);
  for(int j =0; j<nMu8Ele23;j++) hNMu8Ele23Num->Fill(j);
  for(int j =0; j<nElMu;j++) hNMu8Ele23Den->Fill(j);
  TGraphAsymmErrors* Mu8Ele23Graph = new TGraphAsymmErrors(hNMu8Ele23Num,hNMu8Ele23Den);
  std::cout<<"Efficiency of Mu8Ele23: "<<nMu8Ele23/nElMu<<" +"<<Mu8Ele23Graph->GetErrorYhigh(0)<<" -"<<Mu8Ele23Graph->GetErrorYlow(0)<<std::endl<<"\n";

  TH1F* hNMu17Ele12Num = new TH1F("hNMu17Ele12Num","",1,0,10000);
  TH1F* hNMu17Ele12Den = new TH1F("hNMu17Ele12Den","",1,0,10000);
  for(int j =0; j<nMu17Ele12;j++) hNMu17Ele12Num->Fill(j);
  for(int j =0; j<nElMu;j++) hNMu17Ele12Den->Fill(j);
  TGraphAsymmErrors* Mu17Ele12Graph = new TGraphAsymmErrors(hNMu17Ele12Num,hNMu17Ele12Den);
  std::cout<<"Efficiency of Mu17Ele12: "<<nMu17Ele12/nElMu<<" +"<<Mu17Ele12Graph->GetErrorYhigh(0)<<" -"<<Mu17Ele12Graph->GetErrorYlow(0)<<std::endl<<"\n";

  TH1F* hNMu23Ele12Num = new TH1F("hNMu23Ele12Num","",1,0,10000);
  TH1F* hNMu23Ele12Den = new TH1F("hNMu23Ele12Den","",1,0,10000);
  for(int j =0; j<nMu23Ele12;j++) hNMu23Ele12Num->Fill(j);
  for(int j =0; j<nElMu;j++) hNMu23Ele12Den->Fill(j);
  TGraphAsymmErrors* Mu23Ele12Graph = new TGraphAsymmErrors(hNMu23Ele12Num,hNMu23Ele12Den);
  std::cout<<"Efficiency of Mu23Ele12: "<<nMu23Ele12/nElMu<<" +"<<Mu23Ele12Graph->GetErrorYhigh(0)<<" -"<<Mu23Ele12Graph->GetErrorYlow(0)<<std::endl<<"\n";

  std::cout<<"Efficiency of Mu17Ele12 OR Mu8Ele17: "<<nMu30Ele30ORMu17Ele12ORMu8Ele17/nElMu<<std::endl;
}


std::vector<TLepton*> makeLeptons(std::vector<TMuon*> muons, std::vector<TElectron*> electrons, float ptCut, std::string elID, std::string muID, bool doFakes){

  std::vector<TLepton*> Leptons;

  //fill with  muons
  for(unsigned int uimu=0; uimu<muons.size(); uimu++){
    TMuon* imu = muons.at(uimu);
    TLepton* iLep = new TLepton(
                             imu->pt,
                             imu->eta,
                             imu->phi,
                             imu->energy,
                             imu->charge,
                             imu->relIso,
                             imu->miniIso,
                             imu->susyIso
                             );

    if(muID=="CBTight"){
      iLep->Tight=imu->cutBasedTight();
      iLep->Loose=imu->cutBasedLoose();
    }
    else if(muID=="CBTightMiniIso"){
      iLep->Tight=imu->cutBasedTightMiniIso();
      iLep->Loose=imu->cutBasedLooseMiniIso();
    }
    else if(muID=="CBTightMiniIsoTight"){
      iLep->Tight=imu->cutBasedTightMiniIsoTight();
      iLep->Loose=imu->cutBasedLooseMiniIso();
    }
    else if(muID=="CBLoose"){
      iLep->Tight=imu->cutBasedLoose();
      iLep->Loose=true; //in case 'loose ID' is specified as 'tight', take any muon as loose ID
    }
    iLep->isMu = true;
    iLep->isEl = false;
    //skip for smaller than pT cut
    if(iLep->pt<ptCut) continue;
    //now save based on ID requirments if 'normal' running then require tight, else save if loose
    if(!doFakes){
      if(iLep->Tight) Leptons.push_back(iLep);
    }
    else{ //if doing fake estimate save if loose
      if(iLep->Loose) Leptons.push_back(iLep);
    }
  }


  //fill with  electrons
  for(unsigned int uiel=0; uiel<electrons.size(); uiel++){
    TElectron* iel = electrons.at(uiel);
    TLepton* iLep = new TLepton(
                                iel->pt,
                                iel->eta,
                                iel->phi,
                                iel->energy,
                                iel->charge,
                                iel->relIsoEA,
                                iel->miniIso,
                                iel->susyIso
                                );
    iLep->isMu = false;
    iLep->isEl = true;
    if(elID=="CBTight"){
      iLep->Tight=iel->cutBasedTight25nsSpring15MC();
      iLep->Loose=iel->cutBasedLoose25nsSpring15MC();
    }
    else if(elID=="CBLoose"){
      iLep->Tight=iel->cutBasedLoose25nsSpring15MC();
      iLep->Loose=true;
    }
    if(elID=="CBTightRC"){
      iLep->Tight=iel->cutBasedTight25nsSpring15MCRC();
      iLep->Loose=iel->cutBasedLoose25nsSpring15MCRC();
    }
    else if(elID=="CBLooseRC"){
      iLep->Tight=iel->cutBasedLoose25nsSpring15MCRC();
      iLep->Loose=true;
    }
    else if(elID=="MVATight"){
      iLep->Tight=iel->mvaTightIso();
      iLep->Loose=iel->mvaLooseIso();
    }
    else if(elID=="MVATightNew"){
      iLep->Tight=iel->mvaTightNew();
      iLep->Loose=iel->mvaLooseNew();
    }
    else if(elID=="MVATightNewRC"){
      iLep->Tight=iel->mvaTightNewRC();
      iLep->Loose=iel->mvaLooseNewRC();
    }
    else if(elID=="MVATightNoIso"){
      iLep->Tight=iel->mvaTight();
      iLep->Loose=iel->mvaLoose();
    }
    else if(elID=="MVALoose"){
      iLep->Tight=iel->mvaLooseIso();
      iLep->Loose=true;
    }
    else if(elID=="MVALooseNoIso"){
      iLep->Tight=iel->mvaLoose();
      iLep->Loose=true;
    }
    else if(elID=="MVATightCC"){
      iLep->Tight=iel->mvaTightCCIso();
      iLep->Loose=iel->mvaLooseCCIso();
    }
    else if(elID=="MVATightCCNoIso"){
      iLep->Tight=iel->mvaTightCC();
      iLep->Loose=iel->mvaLooseCC();
    }
    else if(elID=="MVALooseCC"){
      iLep->Tight=iel->mvaLooseCCIso();
      iLep->Loose=true;
    }
    else if(elID=="MVALooseNoIso"){
      iLep->Tight=iel->mvaLoose();
      iLep->Loose=true;
    }
    else if(elID=="MVATightRC"){
      iLep->Tight=iel->mvaTightRCIso();
      iLep->Loose=iel->mvaLooseRCIso();
    }
    else if(elID=="MVA2016TightRC"){
      iLep->Tight=iel->mva2016TightRCIso();
      iLep->Loose=iel->mvaJulieLooseRCIso();
    }
    else if(elID=="MVATightMedIsoRC"){
      iLep->Tight=iel->mvaTightRCMedIso();
      iLep->Loose=iel->mvaLooseRCIso();
    }
    else if(elID=="MVALooseRC"){
      iLep->Tight=iel->mvaLooseRCIso();
      iLep->Loose=true;
    }
    else if(elID=="SUSYTight"){
      iLep->Tight=iel->susyTight();
      iLep->Loose=iel->susyLoose();
    }
    else if(elID=="SUSYLoose"){
        iLep->Tight=iel->susyLoose();
        iLep->Loose=true;
    }
    else if(elID=="SUSYTightRC"){
      iLep->Tight=iel->susyTightRC();
      iLep->Loose=iel->susyLooseRC();
    }
    else if(elID=="MVA2017TightRC"){
      iLep->Tight=iel->mva94XTightV1_Iso_RC();
      iLep->Loose=iel->mva94XLooseV1_Iso_RC();
    }
    else if(elID=="MVA2017TightV2IsoRC"){
      iLep->Tight=iel->mva94XTightV2_90_Iso_RC();
      iLep->Loose=iel->mva94XLooseV2_Iso_RC();
    }
    else if(elID=="MVA2017TightV2IsoTightRC"){
      iLep->Tight=iel->mva94XTightV2_90_IsoTight_RC();
      iLep->Loose=iel->mva94XLooseV2_Iso_RC();
    }
    if(iLep->pt<ptCut) continue;
    //now save based on elID requirments if 'normal' running then require tight, else save if loose
    if(!doFakes){
      if(iLep->Tight) Leptons.push_back(iLep);
    }
    else{ //if doing fake estimate save if loose
      if(iLep->Loose) Leptons.push_back(iLep);
    }


  }

  return Leptons;

}

bool checkSameSignLeptons(std::vector<TLepton*> leptons){

  bool samesign=false;

  for(unsigned int uilep=0; uilep<leptons.size(); uilep++){
    for(unsigned int ujlep=uilep+1; ujlep<leptons.size(); ujlep++){
      if(leptons.at(uilep)->charge == leptons.at(ujlep)->charge){
        samesign=true;
      }
    }
  }

  return samesign;

}

bool checkOppositeSignLeptonsForDY(std::vector<TLepton*> leptons){


  bool oppositeSign=false;
  bool sameSign=false;
  //float pairMass=-999;
  //float minDiff=99999;

  TLepton* Lep1 =0;
  TLepton* Lep2 =0;

  for(unsigned int uilep=0; uilep<leptons.size(); uilep++){
    TLepton* lep1 = leptons.at(uilep);
    for(unsigned int ujlep=uilep+1; ujlep<leptons.size(); ujlep++){
      TLepton* lep2 = leptons.at(ujlep);
      //check for hardest
      if(lep1->charge==lep2->charge) sameSign=true;
      if(lep1->charge!=lep2->charge) {
        oppositeSign=true;
        Lep1=lep1;
        Lep2=lep2;
      }
    }

  }
  std::string os = oppositeSign ? "true" : "false";


  //now don't allow any events with samesign dileptons to pass
  if(sameSign) return false;
  //else return whether or not there were indeed opposite sign leptons
  else{
    if(oppositeSign){
      if(Lep1->isMu && Lep2->isMu) return false; //skipp mumu events
      else return true;
    }
    else return false;
  }
}


std::vector<TLepton*> makeSSLeptons(std::vector<TLepton*> leptons){

  std::vector<TLepton*> vSSLep;

  for(unsigned int uilep=0; uilep<leptons.size(); uilep++){
    for(unsigned int ujlep=uilep+1; ujlep<leptons.size(); ujlep++){
      if(leptons.at(uilep)->charge == leptons.at(ujlep)->charge){

        vSSLep.push_back(leptons.at(uilep));
        vSSLep.push_back(leptons.at(ujlep));

      }
    }
  }

  return vSSLep;

}

std::vector<TLepton*> makeOSLeptonsForDY(std::vector<TLepton*> leptons){

  std::vector<TLepton*> vSSLep;


  for(unsigned int uilep=0; uilep<leptons.size(); uilep++){
    TLepton* lep1 = leptons.at(uilep);
    for(unsigned int ujlep=uilep+1; ujlep<leptons.size(); ujlep++){
      TLepton* lep2 = leptons.at(ujlep);
      //if both are muons don't save pair
      if(lep1->isMu && lep2->isMu) continue;
      if(lep1->charge != lep2->charge){
        vSSLep.push_back(lep1);
        vSSLep.push_back(lep2);
      }
    }
  }



  return vSSLep;

}

int getNSSDLGen(std::vector<TGenParticle*> genParticles, int nMu){

  bool samesign;

  int flav1=-999;
  int flav2=-999;

  for(unsigned int uigen=0; uigen < genParticles.size(); uigen++){
    //only check particles from the hard scattering
    //if( genParticles.at(uigen)->status!=23) continue;
    //only check electrons and muons
    if( !( abs(genParticles.at(uigen)->id)==11 || abs(genParticles.at(uigen)->id)==13) ) continue;
    for(unsigned int ujgen=0;  ujgen < genParticles.size(); ujgen++){
      //only check particles from the hard scattering
    if( genParticles.at(ujgen)->status!=23) continue;
      if(ujgen==uigen) continue;
      //std::cout<<"flavor 1: "<<genParticles.at(uigen)->id<<" w/ charge: "<<genParticles.at(uigen)->charge<<" flavor 2: "<<genParticles.at(ujgen)->id<<" w/ charge: "<<genParticles.at(ujgen)->charge<<std::endl;
      //only check electrons and muons
      if( !( abs(genParticles.at(ujgen)->id)==11 || abs(genParticles.at(ujgen)->id)==13) ) continue;
      if( genParticles.at(uigen)->charge == genParticles.at(ujgen)->charge){
        samesign = true;
        flav1 = abs(genParticles.at(uigen)->id);
        flav2 = abs(genParticles.at(ujgen)->id);
      }
    }

  }

  //std::cout<<"\n *** End Event *** \n"<<std::endl;

  if(!samesign) return 0;

  if(nMu==2){
    if( flav1 ==13 && flav2==13) return 1;
    else return 0;
  }

  if(nMu==1){
    if( (flav1==11 && flav2==13) || (flav1==13 || flav2==11) ) return 1;
    else return 0;
  }


  if(nMu==0){
    if( flav1==11 && flav2==11) return 1;
    else return 0;
  }

  //in case none above
  return 0;

}


void printParticle(TGenParticle* p){
        std::cout<<"genP id: "<<p->id<<" charge "<<p->charge<<" status: "<<p->status<<" pt: "<<p->pt<<" eta: "<<p->eta<<" phi: "<<p->phi<<std::endl;
}

void printParticle(TLepton* l){
        std::cout<<"charge "<<l->charge<<" pt: "<<l->pt<<" eta: "<<l->eta<<" phi: "<<l->phi<<std::endl;
}

std::pair<bool,float> checkSecondaryZVeto(std::vector<TLepton*> leps, std::vector<TMuon*> muons, std::vector<TElectron*> electrons){

  bool veto=false;
  float assocMass=-999;
  float zmass=91.1;

  for(std::vector<TLepton*>::size_type ilep=0; ilep < leps.size(); ilep++){
    // get lepton
    TLepton* lep = leps.at(ilep);

    //if muon check to find mass w/ other muons
    if(lep->isMu){
      for(std::vector<TMuon*>::size_type imu=0; imu< muons.size(); imu++){
        //skip if loose lepton has pt <= 15 GeV
        if(muons.at(imu)->pt<=15) continue;

        //skip if looking at any of the SS leptons:
        bool skip=false;
        for(std::vector<TLepton*>::size_type jlep =0; jlep<leps.size(); jlep++){
          if(leps.at(jlep)->pt==muons.at(imu)->pt && leps.at(jlep)->phi==muons.at(imu)->phi){skip=true; break;}
        }
        if(skip) continue;

        float diff =  getPairMass(lep,muons.at(imu)) - zmass;
        if(fabs(diff) < 15){ veto=true; assocMass = zmass+diff; break;}
      }
    }
    //else check mass w/ other electrons
    else{
      for(std::vector<TElectron*>::size_type iel=0; iel< electrons.size(); iel++){
        //skip if loose electron pt <= 15 GeV
        if(electrons.at(iel)->pt <= 15) continue;

        //skip if looking at any of the SS leptons:
        bool skip=false;
        for(std::vector<TLepton*>::size_type jlep =0; jlep<leps.size(); jlep++){
          if(leps.at(jlep)->pt==electrons.at(iel)->pt && leps.at(jlep)->phi==electrons.at(iel)->phi){skip=true; break;}
        }
        if(skip) continue;

        float diff =  getPairMass(lep,electrons.at(iel)) - zmass;
        if(fabs(diff) < 15){ veto=true; assocMass = zmass+diff; break;}
      }
    }
  }

  std::pair<bool,float> checkAndMass(veto,assocMass);
  return checkAndMass;

}


std::vector<TLepton*> pruneSSLep(std::vector<TLepton*> allLeps, std::vector<TLepton*> ssLeps){

  std::vector<TLepton*> nonSSLeps;


  for(std::vector<TLepton*>::size_type ilep=0; ilep<allLeps.size();ilep++){
    //std::cout<<"SSLep 1 pt, eta, phi: "<<ssLeps.at(0)->pt<<", "<<ssLeps.at(0)->eta<<", "<<ssLeps.at(0)->phi<<" SSLep 2 pt, eta, phi: "<<ssLeps.at(1)->pt<<", "<<ssLeps.at(1)->eta<<", "<<ssLeps.at(1)->phi<<" current lep pt, eta, phi: "<<allLeps.at(ilep)->pt<<", "<<allLeps.at(ilep)->eta<<", "<<allLeps.at(ilep)->phi<<std::endl;
    if( (allLeps.at(ilep)->pt==ssLeps.at(0)->pt && allLeps.at(ilep)->eta==ssLeps.at(0)->eta && allLeps.at(ilep)->phi==ssLeps.at(0)->phi) || (allLeps.at(ilep)->pt==ssLeps.at(1)->pt && allLeps.at(ilep)->eta==ssLeps.at(1)->eta && allLeps.at(ilep)->phi==ssLeps.at(1)->phi) || (!allLeps.at(ilep)->Tight) ){ //skip non tight since loose leptons can make it here from np background
    }
    else(nonSSLeps.push_back(allLeps.at(ilep)) );

  }
  //std::cout<<" done pruning for this event and found "<<nonSSLeps.size()<<" nonSSleptons \n \n";
  return nonSSLeps;
}
