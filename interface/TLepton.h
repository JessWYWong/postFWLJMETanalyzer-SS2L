#ifndef TLepton_h
#define TLepton_h

#include <TLorentzVector.h>
#include <cmath>
#include <iostream>

class TLepton {
public:
  TLepton(double pttemp, 
          double etatemp, 
          double phitemp, 
          double energytemp, 
          int chargetemp, 
          float relIsotemp, 
          float miniIsotemp, 
          float susyisotemp 
          );
  //Four vector
  double pt;
  double eta;
  double phi;
  double energy;

  int charge;
  float relIso;
  float miniIso;
  float susyIso;

  bool isEl;
  bool isMu;

  bool Tight;
  bool Loose;
  bool isPrompt; // only valid for MC
  TLorentzVector lv;

  void setLV() {
    lv.SetPxPyPzE(pt*cos(phi), pt*sin(phi), pt*TMath::SinH(eta), energy);
  }

  enum flavor { Prompt, Light, Charm, Bottom, Fake, Unmatched} SourceFlavor;

};

#endif
