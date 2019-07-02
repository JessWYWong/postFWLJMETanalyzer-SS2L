#include "../interface/TMuon.h"


TMuon::TMuon(
    double pttemp,
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
    ):
    TLepton(
        pttemp,
        etatemp,
        phitemp,
        energytemp,
        chargetemp,
        relIsotemp,
        miniIsotemp,
        susyisotemp
        ),
    isLoose(isLtemp),
    isTight(isTtemp),
    Global(globaltemp),
    PFMuon(pftemp),
    Tracker(trackertemp),
    chi2(chi2temp),
    nValMuHits(nMuHtemp), 
    nMatchedStations(nMatchStattemp),
    dxy(dxytemp),dz(dztemp),
    SIP3d(siptemp),
    nValPixelHits(ValPixtemp),
    nTrackerLayers(nTracktemp),
    relIso(relIsotemp),
    miniIso(miniIsotemp), 
    susyIso(susyisotemp)
{
  setLV();
}
bool TMuon::cutBasedLoose(){
  //  if (pt < 20)              return false;
  if(fabs(eta)>2.4)          return false;
  if(!PFMuon)                return false;
  if(!(Tracker || Global))   return false;
  //added iso cut
  if(relIso > 0.4)           return false;
  return true;
}

bool TMuon::cutBasedLoose_NoIso(){
  //  if (pt < 20)              return false;
  if(fabs(eta)>2.4)          return false;
  if(!PFMuon)                return false;
  if(!(Tracker || Global))   return false;
  //added iso cut
  //  if(relIso > 0.4)           return false;
  return true;
}

bool TMuon::cutBasedLooseMiniIso(){
  //  if (pt < 20)              return false;
  if(fabs(eta)>2.4)          return false;
  if(!PFMuon)                return false;
  if(!(Tracker || Global))   return false;
  //added iso cut
  if(miniIso > 0.4)           return false;
  return true;
}

bool TMuon::cutBasedTight_NoIso(){
  if(fabs(eta)>2.4)         return false;
  if(!Global)               return false;
  if(!PFMuon)               return false;
  //  if (pt < 30)              return false;
  //  if (global < 5)           return false; //Global muon and tracker muon
  if (chi2 > 10)            return false;
  if (fabs(dz) > 0.5)             return false;
  if (fabs(dxy) > 0.2)            return false;
  if (nValMuHits < 1)       return false;
  if (nMatchedStations < 2) return false;
  if (nValPixelHits < 1)    return false;
  if (nTrackerLayers < 6)   return false;
  //if (relIso > 0.2)        return false;
  
  return true;
}


bool TMuon::cutBasedTight(){
  if(fabs(eta)>2.4)         return false;
  if(!Global)               return false;
  if(!PFMuon)               return false;
  //  if (pt < 30)              return false;
  //  if (global < 5)           return false; //Global muon and tracker muon
  if (chi2 > 10)            return false;
  if (fabs(dz) > 0.5)             return false;
  if (fabs(dxy) > 0.2)            return false;
  if (nValMuHits < 1)       return false;
  if (nMatchedStations < 2) return false;
  if (nValPixelHits < 1)    return false;
  if (nTrackerLayers < 6)   return false;
  if (relIso > 0.2)        return false;
  
  return true;
}

bool TMuon::cutBasedTightMiniIso(){
  if(fabs(eta)>2.4)         return false;
  if(!Global)               return false;
  if(!PFMuon)               return false;
  //  if (pt < 30)              return false;
  //  if (global < 5)           return false; //Global muon and tracker muon
  if (chi2 > 10)            return false;
  if (fabs(dz) > 0.5)             return false;
  if (fabs(dxy) > 0.2)            return false;
  if (nValMuHits < 1)       return false;
  if (nMatchedStations < 2) return false;
  if (nValPixelHits < 1)    return false;
  if (nTrackerLayers < 6)   return false;
  if (miniIso > 0.2)        return false;
  
  return true;
}


bool TMuon::cutBasedTightMiniIsoTight(){
  if(fabs(eta)>2.4)         return false;
  if(!Global)               return false;
  if(!PFMuon)               return false;
  //  if (pt < 30)              return false;
  //  if (global < 5)           return false; //Global muon and tracker muon
  if (chi2 > 10)            return false;
  if (fabs(dz) > 0.5)             return false;
  if (fabs(dxy) > 0.2)            return false;
  if (nValMuHits < 1)       return false;
  if (nMatchedStations < 2) return false;
  if (nValPixelHits < 1)    return false;
  if (nTrackerLayers < 6)   return false;
  if (miniIso > 0.1)        return false;
  
  return true;
}
