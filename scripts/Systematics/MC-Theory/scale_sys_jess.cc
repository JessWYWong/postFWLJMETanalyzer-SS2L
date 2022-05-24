#include "TFile.h"
#include "TH1.h"
#include <vector>
#include "../../../plugins/Macros.cc" //added by rizki
#include "TLegend.h"
#include "TLatex.h"
#include "../../../plugins/SetTDRStyle.cc"
int TTmass[] = {900,1000,1100,1200,1300,1400,1500,1600,1700,1800};
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

std::map<std::string,std::vector<float>> uncSig = {};
std::map<std::string, std::vector<float>>::iterator it;
void initSigMap(){
  for(int m : TTmass){
    uncSig.insert(std::pair<std::string,std::vector<float>>("TTM"+std::to_string(m),{0.,0.,0.,0.,99.,99.}));//SUMSQUp_SUMSQDn_maxUp_maxDn_minUp_minDn 
  }
  for(int m : BBmass){
    uncSig.insert(std::pair<std::string,std::vector<float>>("BBM"+std::to_string(m),{0.,0.,0.,0.,99.,99.}));
  }
}
float maxShift = 0.;
float meanShift = 0.;
float totalShifts = 0;

float roundME(float num){
	float rounded = roundf(num * 1000) / 1000;
	return rounded;
}

void printSys(TFile* f,std::string name,std::stringstream& filecontent){

  TH1F* h1 = (TH1F*)f->Get("hist_scaleHT_1002");
  TH1F* h2 = (TH1F*)f->Get("hist_scaleHT_1003");
  TH1F* h3 = (TH1F*)f->Get("hist_scaleHT_1004");
  TH1F* h4 = (TH1F*)f->Get("hist_scaleHT_1005");
  TH1F* h5 = (TH1F*)f->Get("hist_scaleHT_1007");
  TH1F* h6 = (TH1F*)f->Get("hist_scaleHT_1009");

  TH1F* h_nom = (TH1F*) f->Get("hist_scaleHT_nom");
  float nom = h_nom->Integral(1,h_nom->GetNbinsX()+1);
  //std::cout<<"nominal number is: "<<nom<<std::endl;

  for(unsigned int j=0; j<h1->GetNbinsX();j++){
  //std::cout<<"bin: "<<j+1<<" lower edge is: "<<h1->GetXaxis()->GetBinLowEdge(j+1)<<std::endl;
  }

  TH1F* h1_ssdl = (TH1F*)f->Get("hist_scaleHT_ssdl_1002");
  TH1F* h2_ssdl = (TH1F*)f->Get("hist_scaleHT_ssdl_1003");
  TH1F* h3_ssdl = (TH1F*)f->Get("hist_scaleHT_ssdl_1004");
  TH1F* h4_ssdl = (TH1F*)f->Get("hist_scaleHT_ssdl_1005");
  TH1F* h5_ssdl = (TH1F*)f->Get("hist_scaleHT_ssdl_1007");
  TH1F* h6_ssdl = (TH1F*)f->Get("hist_scaleHT_ssdl_1009");

  TH1F* h_nom_ssdl = (TH1F*) f->Get("hist_scaleHT_ssdl_nom");
  float nom_ssdl = h_nom_ssdl->Integral(1,h_nom_ssdl->GetNbinsX()+1);
  //std::cout<<"nominal ssdl number is: "<<nom_ssdl<<std::endl;
  

  std::vector<TH1F*> hists; hists.push_back(h1);hists.push_back(h2);hists.push_back(h3);hists.push_back(h4);hists.push_back(h5);hists.push_back(h6);
  std::vector<TH1F*> hists_ssdl; hists_ssdl.push_back(h1_ssdl);hists_ssdl.push_back(h2_ssdl);hists_ssdl.push_back(h3_ssdl);hists_ssdl.push_back(h4_ssdl);hists_ssdl.push_back(h5_ssdl);hists_ssdl.push_back(h6_ssdl);

  //SF from Julie (updated by Jess 30 Nov 2020) https://github.com/jmhogan/singleLepAnalyzer/blob/tptp_2017/makeTemplates/modifyBinning_byyear.py
  float scaleSFsUp = 1.0;
  if(name=="TTM900") scaleSFsUp = 0.745;
  if(name=="TTM1000") scaleSFsUp = 0.744;
  if(name=="TTM1100") scaleSFsUp = 0.747;
  if(name=="TTM1200") scaleSFsUp = 0.742;
  if(name=="TTM1300") scaleSFsUp = 0.741;
  if(name=="TTM1400") scaleSFsUp = 0.738;
  if(name=="TTM1500") scaleSFsUp = 0.740;
  if(name=="TTM1600") scaleSFsUp = 0.735;
  if(name=="TTM1700") scaleSFsUp = 0.721;
  if(name=="TTM1800") scaleSFsUp = 0.746;
  float scaleSFsDn = 1.0;
  if(name=="TTM900") scaleSFsDn = 1.311;
  if(name=="TTM1000") scaleSFsDn = 1.312;
  if(name=="TTM1100") scaleSFsDn = 1.306;
  if(name=="TTM1200") scaleSFsDn = 1.315;
  if(name=="TTM1300") scaleSFsDn = 1.316;
  if(name=="TTM1400") scaleSFsDn = 1.322;
  if(name=="TTM1500") scaleSFsDn = 1.319;
  if(name=="TTM1600") scaleSFsDn = 1.329;
  if(name=="TTM1700") scaleSFsDn = 1.354;
  if(name=="TTM1800") scaleSFsDn = 1.311;

  if(name=="BBM900") scaleSFsUp = 0.744;
  if(name=="BBM1000") scaleSFsUp = 0.742;
  if(name=="BBM1100") scaleSFsUp = 0.743;
  if(name=="BBM1200") scaleSFsUp = 0.742;
  if(name=="BBM1300") scaleSFsUp = 0.741;
  if(name=="BBM1400") scaleSFsUp = 0.739;
  if(name=="BBM1500") scaleSFsUp = 0.735;
  if(name=="BBM1600") scaleSFsUp = 0.735;
  if(name=="BBM1700") scaleSFsUp = 0.733;
  if(name=="BBM1800") scaleSFsUp = 0.731;

  if(name=="BBM900") scaleSFsDn = 1.312;
  if(name=="BBM1000") scaleSFsDn = 1.315;
  if(name=="BBM1100") scaleSFsDn = 1.314;
  if(name=="BBM1200") scaleSFsDn = 1.316;
  if(name=="BBM1300") scaleSFsDn = 1.318;
  if(name=="BBM1400") scaleSFsDn = 1.321;
  if(name=="BBM1500") scaleSFsDn = 1.329;
  if(name=="BBM1600") scaleSFsDn = 1.329;
  if(name=="BBM1700") scaleSFsDn = 1.331;
  if(name=="BBM1800") scaleSFsDn = 1.337;

  float nom_full = h_nom->Integral(13,h_nom->GetNbinsX()+1);
  //std::cout<< "nom_full : " << nom_full <<std::endl;
  float maxdiff=0;
  float mindiff=0;
  for(unsigned int i=0; i<hists.size();i++){
    float tot = hists.at(i)->Integral(13,hists.at(i)->GetNbinsX()+1);
  	//std::cout<< "tot : " << tot <<std::endl;
    float percentdiff = 1*( (tot-nom_full) / nom_full);
    if( percentdiff > maxdiff) {maxdiff=percentdiff;}
    if( percentdiff < mindiff) {mindiff=percentdiff;}
    //std::cout<<percentdiff<<std::endl;
  }
//   std::cout<<"**************Running uncertainty for: ****************"<<name<<std::endl;
//   std::cout<<"for full analysis cut: "<<maxdiff<<std::endl;
//   std::cout<< name <<" | "<< maxdiff <<" | " << mindiff << std::endl;
//   std::cout.precision(3);
//   std::cout<< name <<" | "<<  roundME(1+maxdiff) <<" | " << roundME(1+mindiff)  << std::endl;

  float scaleUp = (1+maxdiff)*scaleSFsUp;
  float scaleDn = (1+mindiff)*scaleSFsDn;
  std::cout<< name <<" | "<<  roundME( scaleUp ) <<" | " << roundME( scaleDn )  << " ----> "<< fabs(1-scaleUp) << " | "<< fabs(1-scaleDn)  << std::endl;

  //swap Up-Dn if necessary
//   float temp;
//   if(scaleUp < scaleDn){
//   	temp = scaleUp;
//   	scaleUp = scaleDn;
//   	scaleDn = temp;
//   }
  filecontent<<"uncDict['scaleUp']['"<< name << "']="<<scaleUp<< "\n";
  filecontent<<"uncDict['scaleDn']['"<< name << "']="<<scaleDn<< "\n";

  /*
  maxdiff=0;
  for(unsigned int i=0; i<hists.size();i++){
    float tot = hists.at(i)->Integral(1,hists.at(i)->GetNbinsX()+1);
    float percentdiff = 100*( (tot-nom) / nom);
    if(fabs(percentdiff) > maxdiff) maxdiff=percentdiff;
  }
  //std::cout<<"for nconst  cut: "<<maxdiff<<std::endl; 
  maxdiff=0;
  for(unsigned int i=0; i<hists_ssdl.size();i++){
    float tot = hists_ssdl.at(i)->Integral(1,hists_ssdl.at(i)->GetNbinsX()+1);
    float percentdiff = 100*( (tot-nom_ssdl) / nom_ssdl);
    if(fabs(percentdiff) > maxdiff) maxdiff=percentdiff;
  }
  //std::cout<<"Now for just ssdl cut: "<<maxdiff<<"\n";  
  //std::cout<<"\n\n";  
  */

  float sysUp = scaleUp;
  float sysDn = scaleDn;
  std::string sample = (std::string) name;

  if(sample.find("TTM")!=std::string::npos || sample.find("BBM")!=std::string::npos){
    if(maxShift < fabs(1-scaleUp) ) maxShift = fabs(1-scaleUp);
    if(maxShift < fabs(1-scaleDn) ) maxShift = fabs(1-scaleDn);
    meanShift = meanShift + fabs(1-scaleUp) + fabs(1-scaleDn);
    totalShifts += 2;

    uncSig.at(sample).at(0) += pow(sysUp,2.0);
    uncSig.at(sample).at(1) += pow(sysDn,2.0);
    if(sysUp > uncSig.at(sample).at(2)) uncSig.at(sample).at(2) = sysUp;
    if(sysDn > uncSig.at(sample).at(3)) uncSig.at(sample).at(3) = sysDn;
    if(sysUp < uncSig.at(sample).at(4)) uncSig.at(sample).at(4) = sysUp;
    if(sysDn < uncSig.at(sample).at(5)) uncSig.at(sample).at(5) = sysDn;

    double m_sig = std::stod(sample.substr(3,sample.length()-3));
    if(sample.find("TTM")!=std::string::npos){hTTuncUp->Fill(m_sig,sysUp); hTTuncDn->Fill(m_sig,sysDn);}
    else if(sample.find("BBM")!=std::string::npos){hBBuncUp->Fill(m_sig,sysUp); hBBuncDn->Fill(m_sig,sysDn);}    
  }
}


void scale_sys_jess(){

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

	  std::ofstream outfile;
	  outfile.open("unc_muRFcorrdNew_TT_"+decay+".py");

	  std::stringstream filecontent;
	  filecontent<<std::fixed<<std::setprecision(5);
	  
          filecontent<< "uncDict = {} \n";
          filecontent<< "uncDict['scaleUp'] = {} \n";
          filecontent<< "uncDict['scaleDn'] = {} \n";
  
          TFile* TTM900 = new TFile(folder+"/TprimeTprime_M-900_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
	  TFile* TTM1000 = new TFile(folder+"/TprimeTprime_M-1000_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
	  TFile* TTM1100 = new TFile(folder+"/TprimeTprime_M-1100_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
	  TFile* TTM1200 = new TFile(folder+"/TprimeTprime_M-1200_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
	  TFile* TTM1300 = new TFile(folder+"/TprimeTprime_M-1300_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
	  TFile* TTM1400 = new TFile(folder+"/TprimeTprime_M-1400_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
	  TFile* TTM1500 = new TFile(folder+"/TprimeTprime_M-1500_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
	  TFile* TTM1600 = new TFile(folder+"/TprimeTprime_M-1600_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
	  TFile* TTM1700 = new TFile(folder+"/TprimeTprime_M-1700_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          TFile* TTM1800 = new TFile(folder+"/TprimeTprime_M-1800_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");

	  std::cout<<" =========="+decay+"==========="<<std::endl;
	  std::cout<<" Sample | up | down "<<std::endl;

          printSys(TTM900,"TTM900",filecontent);
	  printSys(TTM1000,"TTM1000",filecontent);
	  printSys(TTM1100,"TTM1100",filecontent);
	  printSys(TTM1200,"TTM1200",filecontent);
	  printSys(TTM1300,"TTM1300",filecontent);
	  printSys(TTM1400,"TTM1400",filecontent);
	  printSys(TTM1500,"TTM1500",filecontent);
	  printSys(TTM1600,"TTM1600",filecontent);
	  printSys(TTM1700,"TTM1700",filecontent);
          printSys(TTM1800,"TTM1800",filecontent);

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
	  outfile.open("unc_muRFcorrdNew_BB_"+decay+".py");

	  std::stringstream filecontent;
	  filecontent<<std::fixed<<std::setprecision(5);
	  
          filecontent<< "uncDict = {} \n";
          filecontent<< "uncDict['scaleUp'] = {} \n";
          filecontent<< "uncDict['scaleDn'] = {} \n";

	  TFile* BBM900 = new TFile(folder+"/BprimeBprime_M-900_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
	  TFile* BBM1000 = new TFile(folder+"/BprimeBprime_M-1000_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
	  TFile* BBM1100 = new TFile(folder+"/BprimeBprime_M-1100_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
	  TFile* BBM1200 = new TFile(folder+"/BprimeBprime_M-1200_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
	  TFile* BBM1300 = new TFile(folder+"/BprimeBprime_M-1300_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
	  TFile* BBM1400 = new TFile(folder+"/BprimeBprime_M-1400_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
	  TFile* BBM1500 = new TFile(folder+"/BprimeBprime_M-1500_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
	  TFile* BBM1600 = new TFile(folder+"/BprimeBprime_M-1600_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
	  TFile* BBM1700 = new TFile(folder+"/BprimeBprime_M-1700_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");
          TFile* BBM1800 = new TFile(folder+"/BprimeBprime_M-1800_"+decay+"_Mu"+muID+"_El"+elID+"_"+era+".root");

	  std::cout<<" =========="+decay+"==========="<<std::endl;
	  std::cout<<" Sample | up | down "<<std::endl;

	  printSys(BBM900,"BBM900",filecontent);
	  printSys(BBM1000,"BBM1000",filecontent);
	  printSys(BBM1100,"BBM1100",filecontent);
	  printSys(BBM1200,"BBM1200",filecontent);
	  printSys(BBM1300,"BBM1300",filecontent);
	  printSys(BBM1400,"BBM1400",filecontent);
	  printSys(BBM1500,"BBM1500",filecontent);
	  printSys(BBM1600,"BBM1600",filecontent);
	  printSys(BBM1700,"BBM1700",filecontent);
          printSys(BBM1800,"BBM1800",filecontent);

	  outfile<<filecontent.str();
	  outfile.close();

          countBB++;
  }
  
  std::cout<<"\n"<<std::endl;

  std::cout<<" ====================="<<std::endl;
  std::cout<<" meanShift | maxShift "<<std::endl;
  std::cout<< meanShift / totalShifts <<" | "<< maxShift <<std::endl;

  // store unc of signal samples by flavour and mass
  std::ofstream outfile_sig;
  outfile_sig.open("unc_muRFcorrdNew_sig.py");

  std::stringstream filecontent_sig;
  filecontent_sig<<std::fixed<<std::setprecision(5);

  filecontent_sig<< "uncDict = {} \n";
  filecontent_sig<< "uncDict['scaleUp'] = {} \n";
  filecontent_sig<< "uncDict['scaleDn'] = {} \n";

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
    filecontent_sig<<"uncDict['scaleUp']['"<< it->first << "']="<< pow((it->second)[0]/(countTT),0.5) << "\n";
    filecontent_sig<<"uncDict['scaleDn']['"<< it->first << "']="<< pow((it->second)[1]/(countTT),0.5) << "\n";

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
  leg1->AddEntry(hTTuncUp, "scale Up", "p");
  hTTRMSUp->SetMarkerStyle(5);
  hTTRMSUp->SetMarkerSize(1.5);
  hTTRMSUp->SetMarkerColor(kBlack);
  hTTRMSUp->Draw("scat same");
  leg1->AddEntry(hTTRMSUp, "scale Up (RMS)", "p");
  hTTuncDn->SetMarkerColorAlpha(kRed-7,0.5);
  hTTuncDn->SetMarkerStyle(20);
  hTTuncDn->SetMarkerSize(0.5);
  hTTuncDn->Draw("scat same");
  leg1->AddEntry(hTTuncDn, "scale Down", "p");
  hTTRMSDn->SetMarkerStyle(5);
  hTTRMSDn->SetMarkerSize(1.5);
  hTTRMSDn->SetMarkerColor(kMagenta+2);
  hTTRMSDn->Draw("scat same");
  leg1->AddEntry(hTTRMSDn, "scale Down (RMS)", "p");
  leg1->Draw("same");
  cmstex->DrawLatex(0.15,0.96,"CMS Preliminary");
  c1->SaveAs("plot_unc_muRFcorrdNew_TT.pdf");

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
  leg2->AddEntry(hBBuncUp, "scale Up", "p");
  hBBRMSUp->SetMarkerStyle(5);
  hBBRMSUp->SetMarkerSize(1.5);
  hBBRMSUp->SetMarkerColor(kBlack);
  hBBRMSUp->Draw("scat same");
  leg2->AddEntry(hBBRMSUp, "scale Up (RMS)", "p");
  hBBuncDn->SetMarkerColorAlpha(kRed-7,0.5);
  hBBuncDn->SetMarkerStyle(20);
  hBBuncDn->SetMarkerSize(0.5);
  hBBuncDn->Draw("scat same");
  leg2->AddEntry(hBBuncDn, "scale Down", "p");
  hBBRMSDn->SetMarkerStyle(5);
  hBBRMSDn->SetMarkerSize(1.5);
  hBBRMSDn->SetMarkerColor(kMagenta+2);
  hBBRMSDn->Draw("scat same");
  leg2->AddEntry(hBBRMSDn, "scale Down (RMS)", "p");
  leg2->Draw("same");
  cmstex->DrawLatex(0.15,0.96,"CMS Preliminary");
  c2->SaveAs("plot_unc_muRFcorrdNew_BB.pdf");

  ///----------MC Bkg-------
  TString MCfolder ="/eos/uscms/store/user/wywong/FWLJMET102X_2lep2017_wywong_082020_hadds_postFWLJMETanalyzer_IsoTrig_ANv8";

  std::ofstream outfile;
  outfile.open("unc_muRFcorrdNew_MCBkg.py");

  std::stringstream filecontent;
  filecontent<<std::fixed<<std::setprecision(5);
 
  filecontent<< "uncDict = {} \n"; 
  filecontent<< "uncDict['scaleUp'] = {} \n";
  filecontent<< "uncDict['scaleDn'] = {} \n";

  TFile* TTZ = new TFile(MCfolder+"/TTZ_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* TTW = new TFile(MCfolder+"/TTW_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* TTH = new TFile(MCfolder+"/TTH_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* TTTT = new TFile(MCfolder+"/TTTT_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* WZ = new TFile(MCfolder+"/WZ_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* ZZ = new TFile(MCfolder+"/ZZ_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* WpWp = new TFile(MCfolder+"/WpWp_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* WWZ = new TFile(MCfolder+"/WWZ_Mu"+muID+"_El"+elID+"_"+era+".root");
  //TFile* WZZ = new TFile(MCfolder+"/WZZ_Mu"+muID+"_El"+elID+"_"+era+".root");
  //TFile* ZZZ = new TFile(MCfolder+"/ZZZ_Mu"+muID+"_El"+elID+"_"+era+".root");

  std::cout<<" ==========MC Bkg==========="<<std::endl;
// 	  std::cout<<" Sample | RMSup*SF | RMSdown*SF "<<std::endl;
  std::cout<<" Sample | up | down "<<std::endl;
  
  printSys(TTZ,"TTZ",filecontent);
  printSys(TTW,"TTW",filecontent);
  printSys(TTH,"TTH",filecontent);
  printSys(TTTT,"TTTT",filecontent);
  printSys(WZ,"WZ",filecontent);
  printSys(ZZ,"ZZ",filecontent);
  printSys(WpWp,"WpWp",filecontent);
  printSys(WWZ,"WWZ",filecontent);
  //printSys(WZZ,"WZZ",filecontent);
  //printSys(ZZZ,"ZZZ",filecontent);

  outfile<<filecontent.str();
  outfile.close();

  ///----------MC Bkg------- done
  
  system("cat unc_muRFcorrdNew_MCBkg.py unc_muRFcorrdNew_TT_bW0p5tZ0p25tH0p25.py > unc_muRFcorrdNew_MCBkg_TT_bW0p5tZ0p25tH0p25.py");
  system("cat unc_muRFcorrdNew_MCBkg.py unc_muRFcorrdNew_TT_bW0p0tZ0p5tH0p5.py > unc_muRFcorrdNew_MCBkg_TT_bW0p0tZ0p5tH0p5.py");
  system("cat unc_muRFcorrdNew_MCBkg.py unc_muRFcorrdNew_TT_bW0p0tZ1p0tH0p0.py > unc_muRFcorrdNew_MCBkg_TT_bW0p0tZ1p0tH0p0.py");

	
  gApplication->Terminate();

}
