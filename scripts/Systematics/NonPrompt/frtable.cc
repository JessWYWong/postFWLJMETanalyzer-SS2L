#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include "TFile.h"
#include "TH1.h"

void frtable(){


  TFile* f = new TFile("FakeRate_TTbar.root");
  //TFile* f = new TFile("FakeRate_TTTo2L2Nu.root");
  TH1F* el_all = (TH1F*) f->Get("elNumHist_all");
  TH1F* el_lpt = (TH1F*) f->Get("elNumHist_lpt");
  TH1F* el_hpt = (TH1F*) f->Get("elNumHist_hpt");

  TH1F* mu_all = (TH1F*) f->Get("muNumHist_all");
  TH1F* mu_lpt = (TH1F*) f->Get("muNumHist_lpt");
  TH1F* mu_hpt = (TH1F*) f->Get("muNumHist_hpt");

  TH1F* elden_all = (TH1F*) f->Get("elDenHist_all");
  TH1F* elden_lpt = (TH1F*) f->Get("elDenHist_lpt");
  TH1F* elden_hpt = (TH1F*) f->Get("elDenHist_hpt");

  TH1F* muden_all = (TH1F*) f->Get("muDenHist_all");
  TH1F* muden_lpt = (TH1F*) f->Get("muDenHist_lpt");
  TH1F* muden_hpt = (TH1F*) f->Get("muDenHist_hpt");


  el_all->Divide(elden_all);
  el_lpt->Divide(elden_lpt);
  el_hpt->Divide(elden_hpt);

  mu_all->Divide(muden_all);
  mu_lpt->Divide(muden_lpt);
  mu_hpt->Divide(muden_hpt);

  std::stringstream table;
  table<<std::fixed<<std::setprecision(3);
  table<<"\\multirow{4}{c}{Electrons}\\\\"<<std::endl;
  table<<"Flavor & $25< \\pt <35& $\\pt>35$ & Combined \\\\"<<std::endl;
  table<<"Light Quarks & $"<<el_lpt->GetBinContent(1)<<"\\pm"<<el_lpt->GetBinError(1)<<"$ & $"<<el_hpt->GetBinContent(1)<<"\\pm"<<el_hpt->GetBinError(1)<<"$ & $"<<el_all->GetBinContent(1)<<"\\pm"<<el_all->GetBinError(1)<<std::endl;
  table<<"Charm Quarks & $"<<el_lpt->GetBinContent(2)<<"\\pm"<<el_lpt->GetBinError(2)<<"$ & $"<<el_hpt->GetBinContent(2)<<"\\pm"<<el_hpt->GetBinError(2)<<"$ & $"<<el_all->GetBinContent(2)<<"\\pm"<<el_all->GetBinError(2)<<std::endl;
  table<<"Bottom Quarks & $"<<el_lpt->GetBinContent(3)<<"\\pm"<<el_lpt->GetBinError(3)<<"$ & $"<<el_hpt->GetBinContent(3)<<"\\pm"<<el_hpt->GetBinError(3)<<"$ & $"<<el_all->GetBinContent(3)<<"\\pm"<<el_all->GetBinError(3)<<std::endl;
    table<<"Fakes & $"<<el_lpt->GetBinContent(4)<<"\\pm"<<el_lpt->GetBinError(4)<<"$ & $"<<el_hpt->GetBinContent(4)<<"\\pm"<<el_hpt->GetBinError(4)<<"$ & $"<<el_all->GetBinContent(4)<<"\\pm"<<el_all->GetBinError(4)<<std::endl;
  table<<"Unmatched & $"<<el_lpt->GetBinContent(5)<<"\\pm"<<el_lpt->GetBinError(5)<<"$ & $"<<el_hpt->GetBinContent(5)<<"\\pm"<<el_hpt->GetBinError(5)<<"$ & $"<<el_all->GetBinContent(5)<<"\\pm"<<el_all->GetBinError(5)<<std::endl;
  table<<"Average & $"<<el_lpt->GetBinContent(6)<<"\\pm"<<el_lpt->GetBinError(6)<<"$ & $"<<el_hpt->GetBinContent(6)<<"\\pm"<<el_hpt->GetBinError(6)<<"$ & $"<<el_all->GetBinContent(6)<<"\\pm"<<el_all->GetBinError(6)<<std::endl;

  table<<"\\multirow{4}{c}{Muons}\\\\"<<std::endl;
  table<<"Flavor & $25< \\pt <35& $\\pt>35$ & Combined \\\\"<<std::endl;
  table<<"Light Quarks & $"<<mu_lpt->GetBinContent(1)<<"\\pm"<<mu_lpt->GetBinError(1)<<"$ & $"<<mu_hpt->GetBinContent(1)<<"\\pm"<<mu_hpt->GetBinError(1)<<"$ & $"<<mu_all->GetBinContent(1)<<"\\pm"<<mu_all->GetBinError(1)<<std::endl;
  table<<"Charm Quarks & $"<<mu_lpt->GetBinContent(2)<<"\\pm"<<mu_lpt->GetBinError(2)<<"$ & $"<<mu_hpt->GetBinContent(2)<<"\\pm"<<mu_hpt->GetBinError(2)<<"$ & $"<<mu_all->GetBinContent(2)<<"\\pm"<<mu_all->GetBinError(2)<<std::endl;
  table<<"Bottom Quarks & $"<<mu_lpt->GetBinContent(3)<<"\\pm"<<mu_lpt->GetBinError(3)<<"$ & $"<<mu_hpt->GetBinContent(3)<<"\\pm"<<mu_hpt->GetBinError(3)<<"$ & $"<<mu_all->GetBinContent(3)<<"\\pm"<<mu_all->GetBinError(3)<<std::endl;
    table<<"Fakes & $"<<mu_lpt->GetBinContent(4)<<"\\pm"<<mu_lpt->GetBinError(4)<<"$ & $"<<mu_hpt->GetBinContent(4)<<"\\pm"<<mu_hpt->GetBinError(4)<<"$ & $"<<mu_all->GetBinContent(4)<<"\\pm"<<mu_all->GetBinError(4)<<std::endl;
  table<<"Unmatched & $"<<mu_lpt->GetBinContent(5)<<"\\pm"<<mu_lpt->GetBinError(5)<<"$ & $"<<mu_hpt->GetBinContent(5)<<"\\pm"<<mu_hpt->GetBinError(5)<<"$ & $"<<mu_all->GetBinContent(5)<<"\\pm"<<mu_all->GetBinError(5)<<std::endl;
  table<<"Average & $"<<mu_lpt->GetBinContent(6)<<"\\pm"<<mu_lpt->GetBinError(6)<<"$ & $"<<mu_hpt->GetBinContent(6)<<"\\pm"<<mu_hpt->GetBinError(6)<<"$ & $"<<mu_all->GetBinContent(6)<<"\\pm"<<mu_all->GetBinError(6)<<std::endl;

  std::cout<<table.str()<<std::endl;

}
