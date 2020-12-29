#include "TFile.h"
#include "TH2F.h"
#include "TH1.h"
#include "TLegend.h"
#include "TLatex.h"
#include "../../../plugins/SetTDRStyle.cc"
int TTmass[] = {1000,1100,1200,1300,1400,1500,1600,1700,1800};
int BBmass[] = {900,1000,1100,1200,1300,1400,1500,1600,1700,1800};

int nTT = sizeof(TTmass)/sizeof(TTmass[0]);
int nBB = sizeof(BBmass)/sizeof(BBmass[0]);

TH2D* hTTuncUp = new TH2D("uncUp_TT","uncUp_TT",(TTmass[nTT-1]-TTmass[0])+100,TTmass[0]-50,TTmass[nTT-1]+50,2000,0.0,2.0);
TH2D* hTTuncDn = new TH2D("uncDn_TT","uncDn_TT",(TTmass[nTT-1]-TTmass[0])+100,TTmass[0]-50,TTmass[nTT-1]+50,2000,0.0,2.0);
TH2D* hTTRMSUp = new TH2D("RMSUp_TT","RMSUp_TT",(TTmass[nTT-1]-TTmass[0])+100,TTmass[0]-50,TTmass[nTT-1]+50,2000,0.0,2.0);
TH2D* hTTRMSDn = new TH2D("RMSDn_TT","RMSDn_TT",(TTmass[nTT-1]-TTmass[0])+100,TTmass[0]-50,TTmass[nTT-1]+50,2000,0.0,2.0);

TH2D* hBBuncUp = new TH2D("uncUp_BB","uncUp_BB",(nBB+1)*(BBmass[1]-BBmass[0]),BBmass[0]-((BBmass[1]-BBmass[0])/2),BBmass[nBB-1]+((BBmass[1]-BBmass[0])/2),2000,0.0,2.0);
TH2D* hBBuncDn = new TH2D("uncDn_BB","uncDn_BB",(nBB+1)*(BBmass[1]-BBmass[0]),BBmass[0]-((BBmass[1]-BBmass[0])/2),BBmass[nBB-1]+((BBmass[1]-BBmass[0])/2),2000,0.0,2.0);
TH2D* hBBRMSUp = new TH2D("RMSUp_BB","RMSUp_BB",(nBB+1)*(BBmass[1]-BBmass[0]),BBmass[0]-((BBmass[1]-BBmass[0])/2),BBmass[nBB-1]+((BBmass[1]-BBmass[0])/2),2000,0.0,2.0);
TH2D* hBBRMSDn = new TH2D("RMSDn_BB","RMSDn_BB",(nBB+1)*(BBmass[1]-BBmass[0]),BBmass[0]-((BBmass[1]-BBmass[0])/2),BBmass[nBB-1]+((BBmass[1]-BBmass[0])/2),2000,0.0,2.0);

bool debug = false;
bool usepdfSFsSym = false; //default is false


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

void getPDFUnc(TFile* f, TString name, TString decay,std::stringstream& filecontent, bool isSig){

  std::string h2Name = "pdf_hist";
  if(isSig) {h2Name = "pdf4LHC_hist";}

  TTree* t = (TTree*) f->Get("tEvts_DilepMassCut");
  TString nomcut = "ChargeMisIDWeight * NPWeight * IsoSF * trigSF * GsfSF * PUWeight * IDSF * prefireWeight * MCWeight * (Lep1Pt>40 && nConst >=4 && (DilepMass>20) && ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1) ))";

  TH2F* h2 = (TH2F*) f->Get(h2Name.c_str());//"hist_pdfHT");
  int nSTbin = 30;
  float ST_min = 0.;
  float ST_max = 3000.;
  TH1D* h_nom = new TH1D("h_nom","h_nom", nSTbin, ST_min, ST_max);
  if(!isSig) t->Project("h_nom","cleanAK4HT",nomcut); 
  else{
    for( unsigned int i = 0; i < nSTbin ; i++){h_nom->SetBinContent(i+1,h2->GetBinContent(1,i+1));}
  }

  float pdfSFsUp=1.0;
  float pdfSFsDn=1.0;
  float pdfSFsSym = 0.0;

  if(!usepdfSFsSym){ // SF from Julie (updated by Jess 28 Dec 2020) https://github.com/jmhogan/singleLepAnalyzer/blob/tptp_2017/makeTemplates/modifyBinning_byyear.py

          if(name=="TTM1000") pdfSFsUp = 0.954;
          if(name=="TTM1100") pdfSFsUp = 0.951;
          if(name=="TTM1200") pdfSFsUp = 0.947;
          if(name=="TTM1300") pdfSFsUp = 0.942;
          if(name=="TTM1400") pdfSFsUp = 0.936;
          if(name=="TTM1500") pdfSFsUp = 0.929;
          if(name=="TTM1600") pdfSFsUp = 0.921;
          if(name=="TTM1700") pdfSFsUp = 0.911;
          if(name=="TTM1800") pdfSFsUp = 0.898;

          if(name=="TTM1000") pdfSFsDn = 1.050;
          if(name=="TTM1100") pdfSFsDn = 1.054;
          if(name=="TTM1200") pdfSFsDn = 1.060;
          if(name=="TTM1300") pdfSFsDn = 1.066;
          if(name=="TTM1400") pdfSFsDn = 1.073;
          if(name=="TTM1500") pdfSFsDn = 1.082;
          if(name=="TTM1600") pdfSFsDn = 1.094;
          if(name=="TTM1700") pdfSFsDn = 1.109;
          if(name=="TTM1800") pdfSFsDn = 1.128;

          if(name=="BBM900") pdfSFsUp = 0.954;
          if(name=="BBM1000") pdfSFsUp = 0.954;
          if(name=="BBM1100") pdfSFsUp = 0.951;
          if(name=="BBM1200") pdfSFsUp = 0.947;
          if(name=="BBM1300") pdfSFsUp = 0.942;
          if(name=="BBM1400") pdfSFsUp = 0.936;
          if(name=="BBM1500") pdfSFsUp = 0.929;
          if(name=="BBM1600") pdfSFsUp = 0.921;
          if(name=="BBM1700") pdfSFsUp = 0.911;
          if(name=="BBM1800") pdfSFsUp = 0.897;

          if(name=="BBM900") pdfSFsDn = 1.050;
          if(name=="BBM1000") pdfSFsDn = 1.050;
          if(name=="BBM1100") pdfSFsDn = 1.055;
          if(name=="BBM1200") pdfSFsDn = 1.059;
          if(name=="BBM1300") pdfSFsDn = 1.066;
          if(name=="BBM1400") pdfSFsDn = 1.073;
          if(name=="BBM1500") pdfSFsDn = 1.082;
          if(name=="BBM1600") pdfSFsDn = 1.094;
          if(name=="BBM1700") pdfSFsDn = 1.108;
          if(name=="BBM1800") pdfSFsDn = 1.130;

  }
  else{ //pdfsym
          if(name=="TTM1000") pdfSFsSym = 0.048;
          if(name=="TTM1100") pdfSFsSym = 0.051;
          if(name=="TTM1200") pdfSFsSym = 0.056;
          if(name=="TTM1300") pdfSFsSym = 0.062;
          if(name=="TTM1400") pdfSFsSym = 0.068;
          if(name=="TTM1500") pdfSFsSym = 0.076;
          if(name=="TTM1600") pdfSFsSym = 0.086;
          if(name=="TTM1700") pdfSFsSym = 0.098;
          if(name=="TTM1800") pdfSFsSym = 0.113;

          if(name=="BBM900") pdfSFsSym = 0.050;
          if(name=="BBM1000") pdfSFsSym = 0.048;
          if(name=="BBM1100") pdfSFsSym = 0.052;
          if(name=="BBM1200") pdfSFsSym = 0.056;
          if(name=="BBM1300") pdfSFsSym = 0.061;
          if(name=="BBM1400") pdfSFsSym = 0.068;
          if(name=="BBM1500") pdfSFsSym = 0.076;
          if(name=="BBM1600") pdfSFsSym = 0.086;
          if(name=="BBM1700") pdfSFsSym = 0.098;
          if(name=="BBM1800") pdfSFsSym = 0.115;
	  //h1->GetXaxis()->SetRange(2,200);
  }
  if(debug) std::cout<< "Nom ingtegral "<< h_nom->Integral()<< std::endl;
  /*
  std::vector<TH1D> hists_pdf;
  int nPDF = h2->GetXaxis()->GetNbins();
  TString pdfWeight = "pdfweights";
  for( unsigned int i=0; i<nPDF ; i++){
    TString i_str; i_str.Form("%i",i);
    TH1D* htemp = new TH1D("hpdf"+i_str,"hpdf"+i_str,nSTbin,ST_min,ST_max);
    if(isSig){ for ( unsigned int j=0; j< nSTbin ; j++){htemp->SetBinContent(j+1,h2->GetBinContent(i+1,j+1));}}
    else t->Project("hpdf"+i_str,"cleanAK4HT",nomcut+" * "+pdfWeight+"["+i_str+"]" );
    hists_pdf.push_back(*htemp);
    if(debug) std::cout<< Form("pdf %i ", i )<< hists_pdf[i].Integral() <<std::endl;
    delete htemp;
  }
  */  
  float errsq;
  float shiftpct;
  TH1D* h_up = new TH1D("h_up","h_up", nSTbin,ST_min,ST_max);
  TH1D* h_dn = new TH1D("h_dn","h_dn", nSTbin,ST_min,ST_max);
  for ( unsigned int j=0; j< nSTbin ; j++){
    errsq = 0;
    shiftpct = 0;
    for( unsigned int i=0; i<h2->GetXaxis()->GetNbins(); i++){
      errsq+= pow((h2->GetBinContent(i+1,j+1) - h_nom->GetBinContent(j+1)) , 2.0);
    }
    //std::cout<<"errsq "<< errsq <<std::endl;
    //for( unsigned int i=0; i<nPDF ; i++){
    //  errsq+=  pow((hists_pdf[i].GetBinContent(j+1)-h_nom->GetBinContent(j+1)),2.0);
    //}
    if (!(h_nom->GetBinContent(j+1) == 0)) shiftpct = pow(errsq, 0.5)/ h_nom->GetBinContent(j+1);
    //if(isSig && usepdfSFsSym)  shiftpct = shiftpct - pdfSFsSym;
    h_up->SetBinContent(j+1, max(0., h_nom->GetBinContent(j+1)*(1+shiftpct)));
    h_dn->SetBinContent(j+1, max(0., h_nom->GetBinContent(j+1)*(1-shiftpct)));
  }

  h_up->Scale(pdfSFsUp);
  h_dn->Scale(pdfSFsDn);

  float pdfSysUp = 1 + ((h_up->Integral()-h_nom->Integral()) / h_nom->Integral());
  float pdfSysDn = 1 + ((h_dn->Integral()-h_nom->Integral()) / h_nom->Integral());

  if (fabs(1-pdfSysUp)>1 || fabs(1-pdfSysDn)>1) std::cout<< "nom ; up ; dn; "<<h_nom->Integral()<< " ; "<<h_up->Integral()<<" ; "<<h_dn->Integral()<< std::endl;
  std::cout<<setprecision(5);
  std::cout<<name<<" | "<< pdfSysUp << " |" << pdfSysDn << " ----> "<< fabs(1-pdfSysUp) << " | "<< fabs(1-pdfSysDn) << std::endl;

  filecontent<<"uncDict['pdfUp']['"<< name << "']="<<pdfSysUp<< "\n";
  filecontent<<"uncDict['pdfDn']['"<< name << "']="<<pdfSysDn<< "\n";  

  float sysUp = pdfSysUp;
  float sysDn = pdfSysDn;
  std::string sample = (std::string) name;
  if(isSig){
    if(maxShift < fabs(1-pdfSysUp) ) maxShift = fabs(1-pdfSysUp);
    if(maxShift < fabs(1-pdfSysDn) ) maxShift = fabs(1-pdfSysDn);
    meanShift = meanShift + fabs(1-pdfSysUp) + fabs(1-pdfSysDn);
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

  //if(debug) std::cout<< "RMS "<< h1->GetRMS()<<endl; 

  //float RMSup   = ( 1+h1->GetRMS() ) * pdfSFsUp ;
  //float RMSdown = ( 1-h1->GetRMS() ) * pdfSFsDn ;

//   float RMSup   = ( ( (1+h1->GetRMS() ) * pdfSFsUp) *100;
//   float RMSdown = ( ( (1-h1->GetRMS() ) * pdfSFsDn) *100;
//   float RMSup = (h1->GetRMS())*100;
//   float RMSdown = (h1->GetRMS())*100;


//   std::cout<<"PDF Uncertainty for: "<<name<<" is "<<h1->GetRMS()<<std::endl;
/*
  std::cout<<setprecision(4);
  std::cout<<name<<" | "<< RMSup <<" |" << RMSdown << " "<<std::endl;
    
  //swap Up-Dn if necessary
  float temp;
  if(RMSup < RMSdown){
  	temp = RMSup;
  	RMSup = RMSdown;
  	RMSdown = temp;
  }
  filecontent<<"pdfUp['"<< name << "']="<<RMSup<< "\n";
  filecontent<<"pdfDn['"<< name << "']="<<RMSdown<< "\n";
*/
  
//   TString fname = name+"_"+decay+"_PDFSys.pdf";
// 
//   TCanvas* c = new TCanvas();
//   h1->Draw();
//   c->Print(fname);

  delete h2;
  delete h_nom;
  delete h_up;
  delete h_dn;  

};


/*void getPDFUncYQM(TFile* f, TString name, TString decay,std::stringstream& filecontent){

  float pdfSFsUp=1.0;
  float pdfSFsDn=1.0;

  //SF from Julie (updated by Jess 30 Nov 2020) https://github.com/jmhogan/singleLepAnalyzer/blob/tptp_2017/makeTemplates/modifyBinning_byyear.py 


  if(name=="TTM1000") pdfSFsUp = 0.945;
  if(name=="TTM1100") pdfSFsUp = 0.908;
  if(name=="TTM1200") pdfSFsUp = 0.894;
  if(name=="TTM1300") pdfSFsUp = 0.856;
  if(name=="TTM1400") pdfSFsUp = 0.811;
  if(name=="TTM1500") pdfSFsUp = 0.732;
  if(name=="TTM1600") pdfSFsUp = 0.660;
  if(name=="TTM1700") pdfSFsUp = 0.590;
  if(name=="TTM1800") pdfSFsUp = 0.383;

  if(name=="TTM1000") pdfSFsDn = 1.055;
  if(name=="TTM1100") pdfSFsDn = 1.092;
  if(name=="TTM1200") pdfSFsDn = 1.106;
  if(name=="TTM1300") pdfSFsDn = 1.144;
  if(name=="TTM1400") pdfSFsDn = 1.189;
  if(name=="TTM1500") pdfSFsDn = 1.268;
  if(name=="TTM1600") pdfSFsDn = 1.340;
  if(name=="TTM1700") pdfSFsDn = 1.410;
  if(name=="TTM1800") pdfSFsDn = 1.617;

  if(name=="BBM900") pdfSFsUp = 0.950;
  if(name=="BBM1000") pdfSFsUp = 0.950;
  if(name=="BBM1100") pdfSFsUp = 0.925;
  if(name=="BBM1200") pdfSFsUp = 0.897;
  if(name=="BBM1300") pdfSFsUp = 0.857;
  if(name=="BBM1400") pdfSFsUp = 0.808;
  if(name=="BBM1500") pdfSFsUp = 0.748;
  if(name=="BBM1600") pdfSFsUp = 0.658;
  if(name=="BBM1700") pdfSFsUp = 0.551;
  if(name=="BBM1800") pdfSFsUp = 0.419;

  if(name=="BBM900") pdfSFsDn = 1.050;
  if(name=="BBM1000") pdfSFsDn = 1.050;
  if(name=="BBM1100") pdfSFsDn = 1.075;
  if(name=="BBM1200") pdfSFsDn = 1.103;
  if(name=="BBM1300") pdfSFsDn = 1.143;
  if(name=="BBM1400") pdfSFsDn = 1.192;
  if(name=="BBM1500") pdfSFsDn = 1.252;
  if(name=="BBM1600") pdfSFsDn = 1.342;
  if(name=="BBM1700") pdfSFsDn = 1.449;
  if(name=="BBM1800") pdfSFsDn = 1.581;




  TH2F* h2 = (TH2F*) f->Get("pdf_hist");
  TH1D* h1 = new TH1D("h1","pdf",100,0,100);
  h2->ProjectionX("h1",13,50);

  std::vector<double> pdfvar;

  for( int i = 0; i < 100; i++){
     pdfvar.push_back(h1->GetBinContent(i+1));
  }
  // if you need nominal from this hist:
  float nom = h1->GetBinContent(100);
//   std::cout << "	h1->GetBinContent(101):" << h1->GetBinContent(101) <<std::endl;
  
  std::sort(pdfvar.begin(),pdfvar.end());
  float yieldpdfup = pdfvar[83];
//   std::cout << "	pdfvar[83]:" << pdfvar[83] << std::endl;
  float yieldpdfdn = pdfvar[15];
//   std::cout << "	pdfvar[15]:" << pdfvar[15] <<std::endl;

//   float pctpdfup = 1*(yieldpdfup-nom)/nom;
//   float pctpdfdn = 1*(yieldpdfdn-nom)/nom;
//   float pdfup = (1+pctpdfup) *pdfSFsUp;
//   float pdfdn = (1+pctpdfdn) *pdfSFsDn;

  float pctpdfup = 1*((yieldpdfup*pdfSFsUp)-nom)/nom;
  float pctpdfdn = 1*((yieldpdfdn*pdfSFsDn)-nom)/nom;
  float pdfup = (1+pctpdfup) ;
  float pdfdn = (1+pctpdfdn) ;


//   std::cout<<"PDF Uncertainty for: "<<name<<" is "<<h1->GetRMS()<<std::endl;
  std::cout<<setprecision(5);
  std::cout<<name<<" | "<< pdfup <<" |" << pdfdn << " ----> "<< fabs(1-pdfup) << " | "<< fabs(1-pdfdn) << std::endl;
    
  //swap Up-Dn if necessary
//   float temp;
//   if(pdfup < pdfdn){
//   	temp = pdfup;
//   	pdfup = pdfdn;
//   	pdfdn = temp;
//   }
  filecontent<<"uncDict['pdfUp']['"<< name << "']="<<pdfup<< "\n";
  filecontent<<"uncDict['pdfDn']['"<< name << "']="<<pdfdn<< "\n";

  
//   TString fname = name+"_"+decay+"_PDFSys.pdf";
// 
//   TCanvas* c = new TCanvas();
//   h1->Draw();
//   c->Print(fname);

  delete h2;
  delete h1;
    

};*/

void pdfsys_jess(){

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
	  outfile.open("unc_pdfNew_TT_"+decay+".py");

	  std::stringstream filecontent;
	  filecontent<<std::fixed<<std::setprecision(5);
	  
          filecontent<< "uncDict = {} \n";
 	  filecontent<< "uncDict['pdfUp'] = {} \n";
	  filecontent<< "uncDict['pdfDn'] = {} \n";
  
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
// 	  std::cout<<" Sample | RMSup*SF | RMSdown*SF "<<std::endl;
	  std::cout<<" Sample | up | down "<<std::endl;
	  

	  getPDFUnc(TTM1000,"TTM1000",decay,filecontent,true);
	  getPDFUnc(TTM1100,"TTM1100",decay,filecontent,true);
	  getPDFUnc(TTM1200,"TTM1200",decay,filecontent,true);
	  getPDFUnc(TTM1300,"TTM1300",decay,filecontent,true);
	  getPDFUnc(TTM1400,"TTM1400",decay,filecontent,true);
	  getPDFUnc(TTM1500,"TTM1500",decay,filecontent,true);
	  getPDFUnc(TTM1600,"TTM1600",decay,filecontent,true);
	  getPDFUnc(TTM1700,"TTM1700",decay,filecontent,true);
	  getPDFUnc(TTM1800,"TTM1800",decay,filecontent,true);

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
	  outfile.open("unc_pdfNew_BB_"+decay+".py");

	  std::stringstream filecontent;
	  filecontent<<std::fixed<<std::setprecision(5);
 
          filecontent<< "uncDict = {} \n";
 	  filecontent<< "uncDict['pdfUp'] = {} \n";
 	  filecontent<< "uncDict['pdfDn'] = {} \n";
  
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
// 	  std::cout<<" Sample | RMSup*SF | RMSdown*SF "<<std::endl;
	  std::cout<<" Sample | up | down "<<std::endl;

	  getPDFUnc(BBM900,"BBM900",decay,filecontent,true);
	  getPDFUnc(BBM1000,"BBM1000",decay,filecontent,true);
	  getPDFUnc(BBM1100,"BBM1100",decay,filecontent,true);
	  getPDFUnc(BBM1200,"BBM1200",decay,filecontent,true);
	  getPDFUnc(BBM1300,"BBM1300",decay,filecontent,true);
	  getPDFUnc(BBM1400,"BBM1400",decay,filecontent,true);
	  getPDFUnc(BBM1500,"BBM1500",decay,filecontent,true);
	  getPDFUnc(BBM1600,"BBM1600",decay,filecontent,true);
	  getPDFUnc(BBM1700,"BBM1700",decay,filecontent,true);
	  getPDFUnc(BBM1800,"BBM1800",decay,filecontent,true);

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
  outfile_sig.open("unc_pdfNew_sig.py");

  std::stringstream filecontent_sig;
  filecontent_sig<<std::fixed<<std::setprecision(5);

  filecontent_sig<< "uncDict = {} \n";
  filecontent_sig<< "uncDict['pdfUp'] = {} \n";
  filecontent_sig<< "uncDict['pdfDn'] = {} \n";

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
    filecontent_sig<<"uncDict['pdfUp']['"<< it->first << "']="<< pow((it->second)[0]/(countTT),0.5) << "\n";
    filecontent_sig<<"uncDict['pdfDn']['"<< it->first << "']="<< pow((it->second)[1]/(countTT),0.5) << "\n";

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
  std::cout<< yMax_TT <<yMin_TT<<yMax_BB<<yMin_BB<<std::endl;

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
  leg1->AddEntry(hTTuncUp, "pdf Up", "p");
  hTTRMSUp->SetMarkerStyle(5);
  hTTRMSUp->SetMarkerSize(1.5);
  hTTRMSUp->SetMarkerColor(kBlack);
  hTTRMSUp->Draw("scat same");
  leg1->AddEntry(hTTRMSUp, "pdf Up (RMS)", "p");
  hTTuncDn->SetMarkerColorAlpha(kRed-7,0.5);
  hTTuncDn->SetMarkerStyle(20);
  hTTuncDn->SetMarkerSize(0.5);
  hTTuncDn->Draw("scat same");
  leg1->AddEntry(hTTuncDn, "pdf Down", "p");
  hTTRMSDn->SetMarkerStyle(5);
  hTTRMSDn->SetMarkerSize(1.5);
  hTTRMSDn->SetMarkerColor(kMagenta+2);
  hTTRMSDn->Draw("scat same");
  leg1->AddEntry(hTTRMSDn, "pdf Down (RMS)", "p");
  leg1->Draw("same");
  cmstex->DrawLatex(0.15,0.96,"CMS Preliminary");
  c1->SaveAs("plot_unc_pdfNew_TT.pdf");

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
  leg2->AddEntry(hBBuncUp, "pdf Up", "p");
  hBBRMSUp->SetMarkerStyle(5);
  hBBRMSUp->SetMarkerSize(1.5);
  hBBRMSUp->SetMarkerColor(kBlack);
  hBBRMSUp->Draw("scat same");
  leg2->AddEntry(hBBRMSUp, "pdf Up (RMS)", "p");
  hBBuncDn->SetMarkerColorAlpha(kRed-7,0.5);
  hBBuncDn->SetMarkerStyle(20);
  hBBuncDn->SetMarkerSize(0.5);
  hBBuncDn->Draw("scat same");
  leg2->AddEntry(hBBuncDn, "pdf Down", "p");
  hBBRMSDn->SetMarkerStyle(5);
  hBBRMSDn->SetMarkerSize(1.5);
  hBBRMSDn->SetMarkerColor(kMagenta+2);
  hBBRMSDn->Draw("scat same");
  leg2->AddEntry(hBBRMSDn, "pdf Down (RMS)", "p");
  leg2->Draw("same");
  cmstex->DrawLatex(0.15,0.96,"CMS Preliminary");
  c2->SaveAs("plot_unc_pdfNew_BB.pdf");


  ///----------MC Bkg-------

//   TString MCfolder ="/uscms_data/d3/rsyarif/CERN2017/produceLjmetNtuples_TTSSdilep/CMSSW_8_0_26_patch1/src/PostLJMet/X53ThirteenTeVAnalysisCode/test/ProcessedByJulie_Oct18-2017/test/";
//  TString MCfolder ="/uscms_data/d3/rsyarif/CERN2017/produceLjmetNtuples_TTSSdilep/CMSSW_8_0_26_patch1/src/PostLJMet/X53ThirteenTeVAnalysisCode/test/ProcessedByJulie_Oct20-2017/test/";
  TString MCfolder ="/eos/uscms/store/user/wywong/FWLJMET102X_2lep2017_wywong_082020_hadds_postFWLJMETanalyzer_IsoTrig_IDSFunc_nPU";

  std::ofstream outfile;
  outfile.open("unc_pdfNew_MCBkg.py");

  std::stringstream filecontent;
  filecontent<<std::fixed<<std::setprecision(5);

  filecontent<< "uncDict ={} \n";
  filecontent<< "uncDict['pdfUp'] = {} \n";
  filecontent<< "uncDict['pdfDn'] = {} \n";

  TFile* TTZ = new TFile(MCfolder+"/TTZ_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* TTW = new TFile(MCfolder+"/TTW_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* TTH = new TFile(MCfolder+"/TTH_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* TTTT = new TFile(MCfolder+"/TTTT_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* WZ = new TFile(MCfolder+"/WZ_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* ZZ = new TFile(MCfolder+"/ZZ_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* WpWp = new TFile(MCfolder+"/WpWp_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* WWZ = new TFile(MCfolder+"/WWZ_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* WZZ = new TFile(MCfolder+"/WZZ_Mu"+muID+"_El"+elID+"_"+era+".root");
  TFile* ZZZ = new TFile(MCfolder+"/ZZZ_Mu"+muID+"_El"+elID+"_"+era+".root");

  std::cout<<" ==========MC Bkg==========="<<std::endl;
// 	  std::cout<<" Sample | RMSup*SF | RMSdown*SF "<<std::endl;
  std::cout<<" Sample | up | down "<<std::endl;
  
  getPDFUnc(TTZ,"TTZ","",filecontent,false);
  getPDFUnc(TTW,"TTW","",filecontent,false);
  getPDFUnc(TTH,"TTH","",filecontent,false);
  getPDFUnc(TTTT,"TTTT","",filecontent,false);
  getPDFUnc(WZ,"WZ","",filecontent,false);
  getPDFUnc(ZZ,"ZZ","",filecontent,false);
  getPDFUnc(WpWp,"WpWp","",filecontent,false);
  getPDFUnc(WWZ,"WWZ","",filecontent,false);
  getPDFUnc(WZZ,"WZZ","",filecontent,false);
  getPDFUnc(ZZZ,"ZZZ","",filecontent,false);

  outfile<<filecontent.str();
  outfile.close();

  ///----------MC Bkg------- done

  
  system("cat unc_pdfNew_MCBkg.py unc_pdfNew_TT_bW0p5tZ0p25tH0p25.py > unc_pdfNew_MCBkg_TT_bW0p5tZ0p25tH0p25.py");
  system("cat unc_pdfNew_MCBkg.py unc_pdfNew_TT_bW0p0tZ0p5tH0p5.py > unc_pdfNew_MCBkg_TT_bW0p0tZ0p5tH0p5.py");
  system("cat unc_pdfNew_MCBkg.py unc_pdfNew_TT_bW0p0tZ1p0tH0p0.py > unc_pdfNew_MCBkg_TT_bW0p0tZ1p0tH0p0.py");
  
  gApplication->Terminate();


}
