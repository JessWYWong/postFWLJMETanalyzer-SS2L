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
#include <math.h>  //added 

/* the point of this script is to produce a card file suitable for use with higgs combine tool or theta 
   It needs to take in three arguments: leading lepton pT shift, subleading lepton pT shift, HT shift
   where the default values are 30, 30, and 900 GeV (i.e. those of 2012 analysis) */

TH1* getPlot(Sample* s, TString hname, std::string var, int nxbins, double xbins[], std::string cut, int nMu, bool scale_, std::string addWeight = "1.0 * "){

  bool scale = scale_;
  TTree* t = s->tree;

  TH1F* hdummy = new TH1F("hdummy","hdummy",nxbins,xbins);
  hdummy->Sumw2();

  //make cut string based on channel, should always be outside of parantheses for other cuts so a simply && should work also apply chargeMisIDWeight
  std::stringstream channel;
  if(nMu>=0)  channel<<"&& (Channel =="<<nMu<<")";
  else  channel<<"";

  std::string cutstring = addWeight+" PUWeight * IDSF * IsoSF * trigSF * GsfSF * MCWeight * ChargeMisIDWeight * NPWeight * ( "+cut+channel.str()+")";
  if( (s->name).find("Tprime") != std::string::npos || (s->name).find("Bprime") !=std::string::npos ) cutstring = "pdfWeights4LHC[0] * " + cutstring;
  else  cutstring = " 1/abs(MCWeight) * " + cutstring;

  //std::cout<< "cutstring is "<< cutstring<<std::endl;
  t->Project("hdummy",var.c_str(),cutstring.c_str());

  //now weight properly - for Data don't apply
  if( ( (s->name).find("NonPrompt")!=std::string::npos) && ((s->name).find("TTJets")==std::string::npos) ) { scale=false; }
  else if( ( (s->name).find("ChargeMisID")!=std::string::npos) && ((s->name).find("MC")==std::string::npos) ) { scale=false; }

  if(scale){hdummy->Scale(s->weight);}

  hdummy->SetName(hname);
  hdummy->SetTitle("");
  hdummy->SetDirectory(0);
  //hdummy->Print();

  return hdummy;
};

int main(int argc, char* argv[]){

  //debug, set to true by hand until sure script is working
  bool debug_ = false;
  std::string elID = "MVA2017TightV2IsoTightRC";
  std::string muID = "CBTightMiniIsoTight";
  //double xbins[21] = {0,120,270,420,570,720,870,1020,1170,1320,1470,1620,1770,1920,2070,2220,2370,2520,2670,2820,2970};
  double xbins[13];
  for(int i = 0; i<13; i++){ xbins[i] = -2.4+i*((2.4+2.4)/12);}
  //double xbins[28] = {0,180, 220, 270, 320, 370, 420, 470, 520, 570, 620, 670, 720, 770, 820, 870, 920, 970, 1020, 1070, 1120, 1170, 1270, 1370, 1470, 1620, 1870, 2970};
  //double xbins[43] = {190, 215, 265, 290, 315, 340, 365, 390, 415, 440, 465, 490, 515, 540, 565, 590, 615, 640, 665, 690, 715, 740, 765, 790, 815, 840, 865, 890, 915, 940, 965, 990, 1015, 1065, 1115, 1165, 1215, 1290, 1365, 1465, 1590, 1790, 2990};
  //double xbins[42] = {190, 265, 290, 315, 340, 365, 390, 415, 440, 465, 490, 515, 540, 565, 590, 615, 640, 665, 690, 715, 740, 765, 790, 815, 840, 865, 890, 915, 940, 965, 990, 1015, 1065, 1115, 1165, 1215, 1290, 1365, 1465, 1590, 1790, 2990};
  //double xbins[20] = {190, 265, 340, 415, 490, 565, 640, 715, 790, 865, 940, 1015, 1090, 1165, 1240, 1315, 1390, 1540, 1765, 2965};
  //double xbins[13] = {500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1600, 1800, 3000};
  //double xbins[10] = {500, 600, 700, 800, 900, 1000, 1200, 1400, 1600, 3000}; 
  //double xbins[4] = {200, 300, 400, 500};
  int nxbins = sizeof(xbins)/sizeof(*xbins) - 1;

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
  float HTcut   = 0+HTshift; //1000.0+HTshift; //edited 

  //get desired lumi
  float lumi = std::atof(argv[6]); // fb^{-1}
  //get era
  std::string era(argv[7]);

  int nConst = 4; // edited 
//   int nConst = 5; // edited 

  std::cout<<argv[8]<<std::endl;
  float stat = std::atof(argv[8]);
  std::string iplot = "cleanAK4HT";
  std::istringstream arg9(argv[9]);
  if(argc>9) arg9>>iplot;

  //first get our favorite vectors of samples
  std::vector<Sample*> vMCBkg = getMCBkgSampleVec("sZVeto",lumi,elID,muID,era);
  std::vector<Sample*> vDDBkg = getDDBkgSampleVec("sZVeto",lumi,elID,muID,era);
  std::vector<Sample*> vBkg = appendSampleVectors(vMCBkg,vDDBkg);

  //now make cut string according to cuts specified:
  std::vector<std::string> vCutString;
  std::stringstream cutSStream;
  cutSStream<<" ( (Channel!=0) ||(DilepMass<76.1 || DilepMass >106.1)) && (DilepMass>20) && (nConst>="<<nConst<<" ) && (Lep1Pt >"<<lep1cut<<") && (Lep2Pt > "<<lep2cut<<") && ( cleanAK4HT > "<<HTcut<<") && ( nNonSSLeps == 0)"; //added , add exactly 2 leptons (SS) only.
  vCutString.push_back(cutSStream.str());

  if(debug_) std::cout<<"Cutstring is: "<<cutSStream.str()<<std::endl;

  //write observed
  TString erast="";
  if(era.find("2018A-D")!=std::string::npos) erast="2018AD";
  else erast="UnknownEra";

  //////////// Bkg ////////////
  std::vector<TH1F*> elel_bkg, elmu_bkg, mumu_bkg;
  for(std::vector<Sample*>::size_type i =0; i< vBkg.size(); i++){
    if(debug_) std::cout<< vBkg.at(i)->name <<std::endl;
    //////////// --> Get nominal hist
    TString postfix = erast+"__"+vBkg.at(i)->name;
    if(vBkg.at(i)->name=="NonPrompt"){postfix = erast+"__FakeRate";}
    elel_bkg.push_back( (TH1F*)getPlot(vBkg.at(i),"elel"+postfix,iplot,nxbins,xbins,cutSStream.str(),0,true) );
    elmu_bkg.push_back( (TH1F*)getPlot(vBkg.at(i),"elmu"+postfix,iplot,nxbins,xbins,cutSStream.str(),1,true) );
    mumu_bkg.push_back( (TH1F*)getPlot(vBkg.at(i),"mumu"+postfix,iplot,nxbins,xbins,cutSStream.str(),2,true) );
  }

  std::vector<double> newBins;

  double elel_bkgsum = 0.0;
  double elmu_bkgsum = 0.0;
  double mumu_bkgsum = 0.0;
  double elel_errsq = 0.0;
  double elmu_errsq = 0.0;
  double mumu_errsq = 0.0;
  if(debug_) std::cout<< stat<< std::endl;
  for(int ibin=0; ibin< nxbins; ibin++){
    if(ibin == 0) newBins.push_back(elel_bkg.at(0)->GetXaxis()->GetBinUpEdge(nxbins-ibin));
    for(unsigned int ind=0; ind<elel_bkg.size(); ind++){
      elel_bkgsum+=elel_bkg.at(ind)->GetBinContent(nxbins-ibin);
      elmu_bkgsum+=elmu_bkg.at(ind)->GetBinContent(nxbins-ibin);
      mumu_bkgsum+=mumu_bkg.at(ind)->GetBinContent(nxbins-ibin);
      elel_errsq+=pow(elel_bkg.at(ind)->GetBinError(nxbins-ibin),2.0);
      elmu_errsq+=pow(elmu_bkg.at(ind)->GetBinError(nxbins-ibin),2.0);
      mumu_errsq+=pow(mumu_bkg.at(ind)->GetBinError(nxbins-ibin),2.0);
      if(debug_) std::cout<< elel_bkg.at(ind)->GetBinContent(nxbins-ibin)<<std::endl;
    }
    if(elel_bkgsum>0. && elmu_bkgsum>0. && mumu_bkgsum>0.){
      if (pow(elel_errsq,0.5)/elel_bkgsum <= stat &&
          pow(elmu_errsq,0.5)/elmu_bkgsum <= stat &&
          pow(mumu_errsq,0.5)/mumu_bkgsum <= stat ){
         elel_bkgsum = 0.0;
         elmu_bkgsum = 0.0;
         mumu_bkgsum = 0.0;
         elel_errsq = 0.0;
         elmu_errsq = 0.0;
         mumu_errsq = 0.0;
         newBins.push_back(elel_bkg.at(0)->GetXaxis()->GetBinLowEdge(nxbins-ibin));
      }
      else{
         std::cout<<"bin "<<nxbins-ibin<<" --> " << elel_bkgsum << " ; "<< elel_errsq<< " ; "<< pow(elel_errsq,0.5)/elel_bkgsum <<" < "<< stat <<std::endl;
         std::cout<<"bin "<<nxbins-ibin<<" --> " << elmu_bkgsum << " ; "<< elmu_errsq<< " ; "<< pow(elmu_errsq,0.5)/elmu_bkgsum <<std::endl;
         std::cout<<"bin "<<nxbins-ibin<<" --> " << mumu_bkgsum << " ; "<< mumu_errsq<< " ; "<< pow(mumu_errsq,0.5)/mumu_bkgsum <<std::endl;         
      }
    }
    else std::cout<<"zero bin "<<nxbins-ibin<<std::endl;
    //if((nxbins-ibin)==1) {newBins.push_back(elel_bkg.at(0)->GetXaxis()->GetBinLowEdge(nxbins-ibin));};
  }
  std::cout<<"new binnig is {" ;
  for(unsigned int x =0; x< newBins.size(); x++){
    if (x== 0 ){ std::cout << newBins.at(newBins.size()-x-1) ;}
    else {std::cout << ", " << newBins.at(newBins.size()-x-1);}
  }
  std::cout<<"};"<<std::endl;

  return 0;
}

