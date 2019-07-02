#ifndef TMuon_h
#define TMuon_h

#include "TLepton.h"

class TMuon : public TLepton{
public:

  //constructor
  TMuon(
        double ptTemp,
        double etatemp,
        double phitemp, 
        double energytemp, 
        int chargetemp, 
        int isLtemp, 
        int isTtemp, 
        bool globaltemp,
        bool pftemp,
        bool trackertemp,
        double chi2temp, 
        int nMuHtemp, 
        int nMatchStattemp, 
        double dxytemp, 
        double dztemp, 
        double siptemp, 
        int ValPixtemp,
        int nTracktemp, 
        double relIsotemp, 
        double miniIsotemp, 
        double susyisotemp
        );

  //built-in ID flags
  int isLoose;
  int isTight;
  bool Global;
  bool PFMuon;
  bool Tracker;

  //int    global;
  double chi2;
  int    nValMuHits;
  int    nMatchedStations;
  double dxy;
  double dz;
  double SIP3d;
  int    nValPixelHits;
  int    nTrackerLayers;
  double relIso;
  double miniIso;
  double susyIso;

  bool cutBasedLoose();
  bool cutBasedLoose_NoIso();
  bool cutBasedLooseMiniIso();
  bool cutBasedTight();
  bool cutBasedTight_NoIso();
  bool cutBasedTightMiniIso();
  bool cutBasedTightMiniIsoTight();

  void init();
};

#endif
