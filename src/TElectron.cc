#include "../interface/TElectron.h"


TElectron::TElectron(
    double pttemp,
    double etatemp,
    double phitemp,
    double energytemp, 
    int chargetemp, 
    int gsfCharget, 
    int ctfCharget, 
    int scpixCharget, 
    double dEtatemp, 
    double dPhitemp, 
    double dZtemp, 
    double siptemp,
    double d0temp,
    double hOverEtemp, 
    double mHitstemp, 
    double ooEmooPtemp, 
    double sigmaIetaIetatemp,
    double chIsotemp,
    double puIsotemp, 
    double neuIsotemp, 
    double photIsotemp,
    double rhoIsotemp,
    double AEfftemp, 
    int passConv, 
    int chargeconsistencytemp, 
    double mvatemp, 
    double mva80Xtemp, 
    double miniIsotemp, 
    double susyisotemp
    ):
  TLepton(
      pttemp,
      etatemp,
      phitemp,
      energytemp,
      chargetemp,
      miniIsotemp,
      miniIsotemp,
      susyisotemp
      ),
  gsfCharge(gsfCharget),
  ctfCharge(ctfCharget),
  scpixCharge(scpixCharget),
  dEta(dEtatemp),
  dPhi(dPhitemp),
  dZ(dZtemp),
  SIP3d(siptemp),
  d0(d0temp),
  hOverE(hOverEtemp),
  mHits(mHitstemp),
  ooEmooP(ooEmooPtemp),
  sigmaIetaIeta(sigmaIetaIetatemp),
  chIso(chIsotemp),
  puIso(puIsotemp),
  neutIso(neuIsotemp),
  photIso(photIsotemp),
  rhoIso(rhoIsotemp),
  AEff(AEfftemp),
  passConversion(passConv),
  chargeConsistency(chargeconsistencytemp),
  mvaValue(mvatemp),
  mvaValue80X(mva80Xtemp),
  miniIso(miniIsotemp),
  susyIso(susyisotemp)
{
  setLV();

  //redo effective area
  if( fabs(eta)<1.0) AEff=0.1752;
  else if(fabs(eta)<1.479) AEff=0.1862;
  else if(fabs(eta)<2.0) AEff=0.1411;
  else if(fabs(eta)<2.2) AEff=0.1534;
  else if(fabs(eta)<2.3) AEff=0.1903;
  else if(fabs(eta)<2.4) AEff=0.2243;
  else if(fabs(eta)<2.5) AEff=0.2687;


  relIsoDB = (chIso + std::max(0.0, neutIso + photIso - 0.5*puIso) ) / pt;
  relIsoEA = (chIso + std::max(0.0, neutIso + photIso - AEff*rhoIso) ) / pt; 
}




/// *********************** MVA IDs 94X 2017data************************ - https://twiki.cern.ch/twiki/bin/viewauth/CMS/MultivariateElectronIdentificationRun2#Recommended_MVA_Recipe_V2_for_re
// Avoiding adding more arguments in the constructer.
void TElectron::set_mva94XTightV2_80(bool value){
   ISmva94XTightV2_80 = value;
}
void TElectron::set_mva94XTightV2_90(bool value){
   ISmva94XTightV2_90 = value;
}
void TElectron::set_mva94XLooseV2(bool value){
   ISmva94XLooseV2 = value;
}

bool TElectron::mva94XTightV2_80(){return ISmva94XTightV2_80;}
bool TElectron::mva94XTightV2_80_Iso(){
  bool pass = mva94XTightV2_80() && miniIso<0.1;
  return pass;
}
bool TElectron::mva94XTightV2_80_Iso_RC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva94XTightV2_80_Iso() && cc;
  return pass;
}
bool TElectron::mva94XTightV2_80_RC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva94XTightV2_80() && cc;
  return pass;
}

bool TElectron::mva94XTightV2_90(){return ISmva94XTightV2_90;}
bool TElectron::mva94XTightV2_90_Iso(){
  bool pass = mva94XTightV2_90() && miniIso<0.1;
  return pass;
}
bool TElectron::mva94XTightV2_90_Iso_RC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva94XTightV2_90_Iso() && cc;
  return pass;
}
bool TElectron::mva94XTightV2_90_RC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva94XTightV2_90() && cc;
  return pass;
}

bool TElectron::mva94XLooseV2(){return ISmva94XLooseV2;}
bool TElectron::mva94XLooseV2_Iso(){
  bool pass = mva94XLooseV2() && miniIso<0.4;
  return pass;
}
bool TElectron::mva94XLooseV2_Iso_RC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva94XLooseV2_Iso() && cc;
  return pass;
}
bool TElectron::mva94XLooseV2_RC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva94XLooseV2() && cc;
  return pass;
}

/// *********************** MVA IDs 94X 2017data (with Iso from POG)************************ - https://twiki.cern.ch/twiki/bin/viewauth/CMS/MultivariateElectronIdentificationRun2#Recommended_MVA_Recipe_V2_for_re

void TElectron::set_mva94XTightIsoV2_80(bool value){
   ISmva94XTightIsoV2_80 = value;
}
void TElectron::set_mva94XTightIsoV2_90(bool value){
   ISmva94XTightIsoV2_90 = value;
}
void TElectron::set_mva94XLooseIsoV2(bool value){
   ISmva94XLooseIsoV2 = value;
}

bool TElectron::mva94XTightIsoV2_80(){return ISmva94XTightIsoV2_80;}
bool TElectron::mva94XTightIsoV2_80_RC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva94XTightIsoV2_80() && cc;
  return pass;
}

bool TElectron::mva94XTightIsoV2_90(){return ISmva94XTightIsoV2_90;}
bool TElectron::mva94XTightIsoV2_90_RC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva94XTightIsoV2_90() && cc;
  return pass;
}

bool TElectron::mva94XLooseIsoV2(){return ISmva94XLooseIsoV2;}
bool TElectron::mva94XLooseIsoV2_RC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva94XLooseIsoV2() && cc;
  return pass;
}


/// *********************** MVA IDs 94X 2017data************************ - https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentificationRun2#MVA_Recipe_V1_for_regular_users

bool TElectron::mva94XTightV1(){

  //first veto on gap
  if(fabs(eta)>1.442 && fabs(eta)<1.556) return false;

  if(fabs(eta)<0.8){
  	float C=0.96165;
  	float tau=8.75794;
  	float A=3.13902;
    if(mvaValue> ( C - exp( -pt / tau ) *A ) ) return true;
  }
  else if(fabs(eta)<1.479){
  	float C=0.93193;
  	float tau=8.84606;
  	float A=3.59851;
    if(mvaValue> ( C - exp( -pt / tau ) *A ) ) return true;
  }
  else if(fabs(eta)<2.4){
  	float C=0.88993;
  	float tau=10.12423;
  	float A=4.35279;
    if(mvaValue> ( C - exp( -pt / tau ) *A ) ) return true;
  }
  //if none of the above conditions is met return false
  return false;
}
bool TElectron::mva94XTightV1_Iso(){
  bool pass = mva94XTightV1() && miniIso<0.1;
  return pass;
}
bool TElectron::mva94XTightV1_Iso_RC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva94XTightV1_Iso() && cc;
  return pass;
}
bool TElectron::mva94XTightV1_RC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva94XTightV1() && cc;
  return pass;
}


bool TElectron::mva94XLooseV1(){

  //first veto on gap
  if(fabs(eta)>1.442 && fabs(eta)<1.556) return false;

  if(fabs(eta)<0.8){
    if(mvaValue>-0.86) return true;
  }
  else if(fabs(eta)<1.479){
    if(mvaValue>-0.81) return true;
  }
  else if(fabs(eta)<2.4){
    if(mvaValue>-0.72) return true;
  }
  //if none of the above conditions is met return false
  return false;
}
bool TElectron::mva94XLooseV1_Iso(){
  bool pass = mva94XLooseV1() && miniIso<0.4;
  return pass;
}
bool TElectron::mva94XLooseV1_Iso_RC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva94XLooseV1_Iso() && cc;
  return pass;
}
bool TElectron::mva94XLooseV1_RC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva94XLooseV1() && cc;
  return pass;
}

/// *********************** MVA IDs ************************
bool TElectron::mvaLoose(){

  //first veto on gap
  if(fabs(eta)>1.442 && fabs(eta)<1.556) return false;

  if(fabs(eta)<0.8){
    if(mvaValue>0.913286) return true;
  }
  else if(fabs(eta)<1.479){
    if(mvaValue>0.805013) return true;
  }
  else if(fabs(eta)<2.4){
    if(mvaValue>0.358969) return true;
  }
  //if none of the above conditions is met return false
  return false;
}

bool TElectron::mva80XLoose(){

  //first veto on gap
  if(fabs(eta)>1.442 && fabs(eta)<1.556) return false;

  if(fabs(eta)<0.8){
    if(mvaValue80X>0.837) return true;
  }
  else if(fabs(eta)<1.479){
    if(mvaValue80X>0.715) return true;
  }
  else if(fabs(eta)<2.4){
    if(mvaValue80X>0.357) return true;
  }
  //if none of the above conditions is met return false
  return false;
}

bool TElectron::mvaJulieLoose(){

  //first veto on gap
  if(fabs(eta)>1.442 && fabs(eta)<1.556) return false;

  if(fabs(eta)<0.8){
    if(mvaValue80X>-0.041) return true;
  }
  else if(fabs(eta)<1.479){
    if(mvaValue80X>0.383) return true;
  }
  else if(fabs(eta)<2.4){
    if(mvaValue80X>-0.515) return true;
  }
  //if none of the above conditions is met return false
  return false;
}

bool TElectron::mvaTight(){

  //first veto on gap
  if(fabs(eta)>1.442 && fabs(eta)<1.556) return false;

  if(fabs(eta)<0.8){
    if(mvaValue>0.967083) return true;
  }
  else if(fabs(eta)<1.479){
    if(mvaValue>0.929117) return true;
  }
  else if(fabs(eta)<2.4){
    if(mvaValue>0.726311) return true;
  }
  //if none of the above conditions is met return false
  return false;
}

bool TElectron::mva80XTight(){

  //first veto on gap
  if(fabs(eta)>1.442 && fabs(eta)<1.556) return false;

  if(fabs(eta)<0.8){
    if(mvaValue80X>0.941) return true;
  }
  else if(fabs(eta)<1.479){
    if(mvaValue80X>0.899) return true;
  }
  else if(fabs(eta)<2.4){
    if(mvaValue80X>0.758) return true;
  }
  //if none of the above conditions is met return false
  return false;
}

bool TElectron::mva2016Tight(){

  //first veto on gap
  if(fabs(eta)>1.442 && fabs(eta)<1.556) return false;

  if(fabs(eta)<0.8){
    if(mvaValue80X>0.674) return true;
  }
  else if(fabs(eta)<1.479){
    if(mvaValue80X>0.744) return true;
  }
  else if(fabs(eta)<2.4){
    if(mvaValue80X>0.17) return true;
  }
  //if none of the above conditions is met return false
  return false;
}
bool TElectron::mvaJulieTight(){

  //first veto on gap
  if(fabs(eta)>1.442 && fabs(eta)<1.556) return false;

  if(fabs(eta)<0.8){
    if(mvaValue80X>0.674) return true;
  }
  else if(fabs(eta)<1.479){
    if(mvaValue80X>0.795) return true;
  }
  else if(fabs(eta)<2.4){
    if(mvaValue80X>0.17) return true;
  }
  //if none of the above conditions is met return false
  return false;
}

bool TElectron::mvaJulieNewTight(){

  //first veto on gap
  if(fabs(eta)>1.442 && fabs(eta)<1.556) return false;

  if(fabs(eta)<0.8){
    if(mvaValue80X>0.788) return true;
  }
  else if(fabs(eta)<1.479){
    if(mvaValue80X>0.744) return true;
  }
  else if(fabs(eta)<2.4){
    if(mvaValue80X>0.508) return true;
  }
  //if none of the above conditions is met return false
  return false;
}

//standard plus ISO
bool TElectron::mvaTightIso(){
  bool pass = mvaTight() && miniIso<0.1;
  return pass;
}

bool TElectron::mva80XTightIso(){
  bool pass = mva80XTight() && miniIso<0.1;
  return pass;
}

bool TElectron::mva2016TightIso(){
  bool pass = mva2016Tight() && miniIso<0.1;
  return pass;
}

bool TElectron::mvaJulieTightIso(){
  bool pass = mvaJulieTight() && miniIso<0.1;
  return pass;
}

bool TElectron::mvaJulieNewTightIso(){
  bool pass = mvaJulieNewTight() && miniIso<0.1;
  return pass;
}

//standard plus medium ISO
bool TElectron::mvaTightMedIso(){
  bool pass = mvaTight() && miniIso<0.2;
  return pass;
}

bool TElectron::mvaLooseIso(){
  bool pass = mvaLoose() && miniIso<0.4;
  return pass;
}
bool TElectron::mva80XLooseIso(){
  bool pass = mva80XLoose() && miniIso<0.4;
  return pass;
}
bool TElectron::mvaJulieLooseIso(){
  bool pass = mvaJulieLoose() && miniIso<0.4;
  return pass;
}

bool TElectron::mvaLooseCC(){
  bool cc = chargeConsistency< 1 ? false : true;
  bool pass = mvaLoose() && cc;
  return pass;
}

bool TElectron::mvaTightCC(){
  bool cc = chargeConsistency< 1 ? false : true;
  bool pass = mvaTight() && cc;
  return pass;
}


//**********New IDS defined by me and Aram
bool TElectron::mvaTightNew(){
  //check basic mva value and miniIso first
  bool pass = mvaTight() && miniIso< 0.1;
  if(!pass) return false;

  //add track cuts from cutbasedID
  if(fabs(eta) <= 1.479){
    if(fabs(d0) >= 0.0111)      return false;
    if(fabs(dZ) >= 0.0466)      return false;
    if(mHits > 2)              return false;
    if(!passConversion)        return false;
    //    if(pt <30)                 return false;
  }
  //Endcap
  else{
    if(fabs(eta)>2.4)           return false;
    if(fabs(d0) >= 0.0351)      return false;
    if(fabs(dZ) >= 0.417)      return false;
    if(mHits > 1)              return false;
    if(!passConversion)        return false;
  }

  //charge consistency cut
  if(chargeConsistency <1) return false;
  return true;
}

bool TElectron::mvaLooseNew(){
  bool mvaloose = mvaLoose() && miniIso<0.4;
  if(!mvaloose) return false;

  //since it passed mva cuts apply loose cuts on track from cutbasedID
  if(fabs(eta) <= 1.479){
    if(fabs(d0) >= 0.0261)      return false;
    if(fabs(dZ) >= 0.41)     return false;
    if(mHits > 2)              return false;
    if(!passConversion)        return false;
  }
  //Endcap
  else{
    if(fabs(eta)>2.4)           return false;
    if(fabs(d0) >= 0.118)       return false;
    if(fabs(dZ) >= 0.822)      return false;
    if(mHits > 1)              return false;
    if(!passConversion)        return false;
  }

  //charge consistency
  bool cc =  chargeConsistency< 1 ? false : true;
  if(!cc) return false;

  return true;
}

bool TElectron::mvaTightNewRC(){

  bool pass = mvaTight() && miniIso< 0.1;
  if(!pass) return false;

  //add track cuts from cutbasedID
  if(fabs(eta) <= 1.479){
    if(fabs(d0) >= 0.0111)      return false;
    if(fabs(dZ) >= 0.0466)      return false;
    if(mHits > 2)              return false;
    if(!passConversion)        return false;
    //    if(pt <30)                 return false;
  }
  //Endcap
  else{
    if(fabs(eta)>2.4)           return false;
    if(fabs(d0) >= 0.0351)      return false;
    if(fabs(dZ) >= 0.417)      return false;
    if(mHits > 1)              return false;
    if(!passConversion)        return false;
  }

  //relaxed charge consistency cut
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  if(!cc) return false;

  return true;
}

bool TElectron::mvaLooseNewRC(){
  bool mvaloose = mvaLoose() && miniIso<0.4;
  if(!mvaloose) return false;

  //since it passed mva cuts apply loose cuts on track from cutbasedID
  if(fabs(eta) <= 1.479){
    if(fabs(d0) >= 0.0261)      return false;
    if(fabs(dZ) >= 0.41)     return false;
    if(mHits > 2)              return false;
    if(!passConversion)        return false;
  }
  //Endcap
  else{
    if(fabs(eta)>2.4)           return false;
    if(fabs(d0) >= 0.118)       return false;
    if(fabs(dZ) >= 0.822)      return false;
    if(mHits > 1)              return false;
    if(!passConversion)        return false;
  }

  //relaxed charge consistency cut
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  if(!cc) return false;

  return true;
}

//standard with relaxed charge
bool TElectron::mvaLooseRC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mvaLoose() && cc;
  return pass;
}
bool TElectron::mva80XLooseRC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva80XLoose() && cc;
  return pass;
}

bool TElectron::mvaJulieLooseRC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mvaJulieLoose() && cc;
  return pass;
}

bool TElectron::mvaTightRC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mvaTight() && cc;
  return pass;
}

bool TElectron::mva80XTightRC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva80XTight() && cc;
  return pass;
}

bool TElectron::mva2016TightRC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva2016Tight() && cc;
  return pass;
}

bool TElectron::mvaJulieTightRC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mvaJulieTight() && cc;
  return pass;
}

bool TElectron::mvaJulieNewTightRC(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mvaJulieNewTight() && cc;
  return pass;
}

bool TElectron::mvaLooseRCIso(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mvaLooseIso() && cc;
  return pass;
}
bool TElectron::mva80XLooseRCIso(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva80XLooseIso() && cc;
  return pass;
}
bool TElectron::mvaJulieLooseRCIso(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mvaJulieLooseIso() && cc;
  return pass;
}

bool TElectron::mvaLooseLCIso(){
  bool cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mvaLooseIso() && cc;
  return pass;
}

bool TElectron::mvaTightRCIso(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mvaTightIso() && cc;
  return pass;
}

bool TElectron::mva80XTightRCIso(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva80XTightIso() && cc;
  return pass;
}
bool TElectron::mva2016TightRCIso(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mva2016TightIso() && cc;
  return pass;
}
bool TElectron::mvaJulieTightRCIso(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mvaJulieTightIso() && cc;
  return pass;
}

bool TElectron::mvaJulieNewTightRCIso(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mvaJulieNewTightIso() && cc;
  return pass;
}

bool TElectron::mvaTightRCMedIso(){
  bool cc;
  if(pt<100) cc = chargeConsistency< 1 ? false : true;
  else cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mvaTightMedIso() && cc;
  return pass;
}


bool TElectron::mvaTightLCIso(){
  bool cc = gsfCharge==ctfCharge ? true : false;
  bool pass = mvaTightIso() && cc;
  return pass;
}

//standard with tight charge consistency
bool TElectron::mvaLooseCCIso(){
  bool cc = chargeConsistency< 1 ? false : true;
  bool pass = mvaLooseIso() && cc;
  return pass;
}

bool TElectron::mvaTightCCIso(){
  bool cc = chargeConsistency< 1 ? false : true;
  bool pass = mvaTightIso() && cc;
  return pass;
}


///************** SUSY IDs*****************************
//  THESE ARE INCOMPLETE BECAUSE OF MISSING SIP3D INFO!!!!!!!
bool TElectron::susyTight(){

  if(fabs(eta)<0.8){
    if(mvaValue < 0.73) return false;
  }
  else if(fabs(eta)<1.479){
    if(mvaValue < 0.57) return false;
  }
  else if(fabs(eta)<2.4){
    if(mvaValue < 0.05) return false;
  }
  else if(fabs(eta)>2.4) return false; //return false if electron above 2.4
  
  //now that it has survived mva value cuts apply other susy cuts
  if(fabs(d0) > 0.05) return false;
  if(fabs(dZ) > 0.1) return false;
  if(!passConversion) return false;
  if(mHits>0) return false;
  if(chargeConsistency <1) return false;
  if(miniIso >0.1) return false;
  return true;

}

bool TElectron::susyLoose(){

  if(fabs(eta)<0.8){
    if(mvaValue < -0.11) return false;
  }
  else if(fabs(eta)<1.479){
    if(mvaValue < -0.35) return false;
  }
  else if(fabs(eta)<2.4){
    if(mvaValue < -0.55) return false;
  }
  else if(fabs(eta)>2.4) return false; //return false if electron above 2.4
  //now that it has survived mva value cuts apply other susy cuts
  if(fabs(d0) > 0.05) return false;
  if(fabs(dZ) > 0.1) return false;
  if(!passConversion) return false;
  if(mHits>1) return false;
  if(miniIso >0.4) return false;

  return true;

}

bool TElectron::susyTightRC(){

  if(fabs(eta)<0.8){
    if(mvaValue < 0.73) return false;
  }
  else if(fabs(eta)<1.479){
    if(mvaValue < 0.57) return false;
  }
  else if(fabs(eta)<2.4){
    if(mvaValue < 0.05) return false;
  }
  else if(fabs(eta)>2.4) return false; //return false if electron above 2.4
  
  //now that it has survived mva value cuts apply other susy cuts
  if(fabs(d0) > 0.05) return false;
  if(fabs(dZ) > 0.1) return false;
  if(!passConversion) return false;
  if(mHits>0) return false;
  if(pt<100){
    if(chargeConsistency <1) return false;
  }
  else{
    if(gsfCharge!=ctfCharge) return false;
  }
  if(miniIso >0.1) return false;
  return true;

}

bool TElectron::susyLooseRC(){
  //right now just regular susy loose since it looks like they don't require charge consistency for loos leptons
  return susyLoose();
}

/// ************ Cut Based IDs ************************
bool TElectron::cutBasedTight50ns(){
  //Barrel
  if(fabs(eta) <= 1.479){
    if(sigmaIetaIeta >= 0.0101) return false;
    if(fabs(dEta) >= 0.0095)    return false;
    if(fabs(dPhi) >= 0.0291)     return false;
    if(hOverE >= 0.0372)        return false;
    if(relIsoEA > 0.0468)        return false;
    if(ooEmooP >= 0.0174) return false;
    if(fabs(d0) >= 0.0144)      return false;
    if(fabs(dZ) >= 0.323)      return false;
    if(mHits > 2)              return false;
    if(chargeConsistency < 1)  return false;
    if(!passConversion)        return false;
    //    if(pt <30)                 return false;
  }
  //Endcap
  else{
    if(fabs(eta)>2.4)           return false;
    if(sigmaIetaIeta >= 0.0287) return false;
    if(fabs(dEta) >= 0.00762)    return false;
    if(fabs(dPhi) >= 0.0439)    return false;
    if(hOverE >= 0.0544)        return false;
    if(relIsoEA >= 0.0759)        return false; 
    if(ooEmooP >= 0.01) return false;
    if(fabs(d0) >= 0.0377)      return false;
    if(fabs(dZ) >= 0.571)      return false;
    if(mHits > 1)              return false;
    if(chargeConsistency < 1)  return false;
    if(!passConversion)        return false;
    //    if(pt <30)                 return false;
  }

  return true;
}

bool TElectron::cutBasedLoose50ns(){
  //Barrel
  if(fabs(eta) <= 1.479){
    if(sigmaIetaIeta >= 0.0105) return false;
    if(fabs(dEta) >= 0.00976)    return false;
    if(fabs(dPhi) >= 0.0929)    return false;
    if(hOverE >= 0.0765)         return false;
    if(relIsoEA >= 0.118)          return false;
    if(ooEmooP >= 0.184) return false;
    if(fabs(d0) >= 0.0227)      return false;
    if(fabs(dZ) >= 0.379)     return false;
    if(mHits > 2)              return false;
    if(chargeConsistency < 1)  return false;
    if(!passConversion)        return false;
    //   if(pt <20)                 return false;
  }
  
  //Endcap
  else{
    if(fabs(eta)>2.4)           return false;
    if(sigmaIetaIeta >= 0.0318)  return false;
    if(fabs(dEta) >= 0.00952)    return false;
    if(fabs(dPhi) >= 0.181)    return false;
    if(hOverE >= 0.0824)        return false;
    if(relIsoEA >= 0.118)        return false;
    if(ooEmooP >= 0.125) return false;
    if(fabs(d0) >= 0.242)       return false;
    if(fabs(dZ) >= 0.921)      return false;
    if(mHits > 1)              return false;
    if(chargeConsistency < 1)  return false;
    if(!passConversion)        return false;
    //    if(pt <20)                 return false;
  }
  return true;
}

bool TElectron::cutBasedTight25nsSpring15MC(){
  //Barrel
  if(fabs(eta) <= 1.479){
    if(sigmaIetaIeta >= 0.0101) return false;
    if(fabs(dEta) >= 0.00926)    return false;
    if(fabs(dPhi) >= 0.0336)     return false;
    if(hOverE >= 0.0597)        return false;
    if(relIsoEA > 0.0354)        return false;
    if(ooEmooP >= 0.012) return false;
    if(fabs(d0) >= 0.0111)      return false;
    if(fabs(dZ) >= 0.0466)      return false;
    if(mHits > 2)              return false;
    if(chargeConsistency < 1)  return false;
    if(!passConversion)        return false;
    //    if(pt <30)                 return false;
  }
  //Endcap
  else{
    if(fabs(eta)>2.4)           return false;
    if(sigmaIetaIeta >= 0.0279) return false;
    if(fabs(dEta) >= 0.00724)    return false;
    if(fabs(dPhi) >= 0.0918)    return false;
    if(hOverE >= 0.0615)        return false;
    if(relIsoEA >= 0.0646)        return false; 
    if(ooEmooP >= 0.00999) return false;
    if(fabs(d0) >= 0.0351)      return false;
    if(fabs(dZ) >= 0.417)      return false;
    if(mHits > 1)              return false;
    if(chargeConsistency < 1)  return false;
    if(!passConversion)        return false;
    //    if(pt <30)                 return false;
  }
  return true;
}

bool TElectron::cutBasedLoose25nsSpring15MC(){
  //Barrel
  if(fabs(eta) <= 1.479){
    if(sigmaIetaIeta >= 0.0103) return false;
    if(fabs(dEta) >= 0.0105)    return false;
    if(fabs(dPhi) >= 0.115)    return false;
    if(hOverE >= 0.104)         return false;
    if(relIsoEA >= 0.0893)          return false;
    if(ooEmooP >= 0.102) return false;
    if(fabs(d0) >= 0.0261)      return false;
    if(fabs(dZ) >= 0.41)     return false;
    if(mHits > 2)              return false;
    if(chargeConsistency < 1)  return false;
    if(!passConversion)        return false;
    //   if(pt <20)                 return false;
  }
  
  //Endcap
  else{
    if(fabs(eta)>2.4)           return false;
    if(sigmaIetaIeta >= 0.0301)  return false;
    if(fabs(dEta) >= 0.00814)    return false;
    if(fabs(dPhi) >= 0.182)    return false;
    if(hOverE >= 0.0897)        return false;
    if(relIsoEA >= 0.121)        return false;
    if(ooEmooP >= 0.126) return false;
    if(fabs(d0) >= 0.118)       return false;
    if(fabs(dZ) >= 0.822)      return false;
    if(mHits > 1)              return false;
    if(chargeConsistency < 1)  return false;
    if(!passConversion)        return false;
    //    if(pt <20)                 return false;
  }
  return true;
}

///**** Relaxed charge consistency ****************
bool TElectron::cutBasedTight25nsSpring15MCRC(){
  //Barrel
  if(fabs(eta) <= 1.479){
    if(sigmaIetaIeta >= 0.0101) return false;
    if(fabs(dEta) >= 0.00926)    return false;
    if(fabs(dPhi) >= 0.0336)     return false;
    if(hOverE >= 0.0597)        return false;
    if(relIsoEA > 0.0354)        return false;
    if(ooEmooP >= 0.012) return false;
    if(fabs(d0) >= 0.0111)      return false;
    if(fabs(dZ) >= 0.0466)      return false;
    if(mHits > 2)              return false;
    if(!passConversion)        return false;
    if(pt<100){
      if(chargeConsistency < 1)  return false;
    }
    else{
      if(gsfCharge!=ctfCharge) return false;
    }
    //    if(pt <30)                 return false;
  }
  //Endcap
  else{
    if(fabs(eta)>2.4)           return false;
    if(sigmaIetaIeta >= 0.0279) return false;
    if(fabs(dEta) >= 0.00724)    return false;
    if(fabs(dPhi) >= 0.0918)    return false;
    if(hOverE >= 0.0615)        return false;
    if(relIsoEA >= 0.0646)        return false; 
    if(ooEmooP >= 0.00999) return false;
    if(fabs(d0) >= 0.0351)      return false;
    if(fabs(dZ) >= 0.417)      return false;
    if(mHits > 1)              return false;
    if(!passConversion)        return false;
    if(pt<100){
      if(chargeConsistency < 1)  return false;
    }
    else{
      if(gsfCharge!=ctfCharge) return false;
    }
    //    if(pt <30)                 return false;
  }
  return true;
}

bool TElectron::cutBasedLoose25nsSpring15MCRC(){
  //Barrel
  if(fabs(eta) <= 1.479){
    if(sigmaIetaIeta >= 0.0103) return false;
    if(fabs(dEta) >= 0.0105)    return false;
    if(fabs(dPhi) >= 0.115)    return false;
    if(hOverE >= 0.104)         return false;
    if(relIsoEA >= 0.0893)          return false;
    if(ooEmooP >= 0.102) return false;
    if(fabs(d0) >= 0.0261)      return false;
    if(fabs(dZ) >= 0.41)     return false;
    if(mHits > 2)              return false;
    if(!passConversion)        return false;
    if(pt<100){
      if(chargeConsistency < 1)  return false;
    }
    else{
      if(gsfCharge!=ctfCharge) return false;
    }
    //   if(pt <20)                 return false;
  }
  
  //Endcap
  else{
    if(fabs(eta)>2.4)           return false;
    if(sigmaIetaIeta >= 0.0301)  return false;
    if(fabs(dEta) >= 0.00814)    return false;
    if(fabs(dPhi) >= 0.182)    return false;
    if(hOverE >= 0.0897)        return false;
    if(relIsoEA >= 0.121)        return false;
    if(ooEmooP >= 0.126) return false;
    if(fabs(d0) >= 0.118)       return false;
    if(fabs(dZ) >= 0.822)      return false;
    if(mHits > 1)              return false;
    if(!passConversion)        return false;
    if(pt<100){
      if(chargeConsistency < 1)  return false;
    }
    else{
      if(gsfCharge!=ctfCharge) return false;
    }
    //    if(pt <20)                 return false;
  }
  return true;
}


bool TElectron::cutBasedTightMay2015(){
  //Barrel
  if(fabs(eta) <= 1.479){
    if(fabs(dEta) >= 0.006046)    return false;
    if(fabs(dPhi) >= 0.028092)     return false;
    if(sigmaIetaIeta >= 0.009947) return false;
    if(hOverE >= 0.045772)        return false;
    if(fabs(d0) >= 0.008790)      return false;
    if(fabs(dZ) >= 0.021226)      return false;
    if(ooEmooP >= 0.020118) return false;
    if(relIsoEA > 0.069537)        return false;
    if(mHits > 1)              return false;
    if(chargeConsistency < 1)  return false;
    if(!passConversion)        return false;
    //    if(pt <30)                 return false;
  }
  //Endcap
  else{
    if(fabs(eta)>2.4)           return false;
    if(fabs(dEta) >= 0.007057)    return false;
    if(fabs(dPhi) >= 0.030159)    return false;
    if(sigmaIetaIeta >= 0.028237) return false;
    if(hOverE >= 0.067778)        return false;
    if(fabs(d0) >= 0.027984)      return false;
    if(fabs(dZ) >= 0.133431)      return false;
    if(ooEmooP >= 0.098919) return false;
    if(relIsoEA >= 0.078265)        return false; 
    if(mHits > 1)              return false;
    if(chargeConsistency < 1)  return false;
    if(!passConversion)        return false;
    //    if(pt <30)                 return false;
  }
  return true;
}

bool TElectron::cutBasedLooseMay2015(){
  //Barrel
  if(fabs(eta) <= 1.479){
    if(fabs(dEta) >= 0.009277)    return false;
    if(fabs(dPhi) >= 0.094739)    return false;
    if(sigmaIetaIeta >= 0.010331) return false;
    if(hOverE >= 0.093068)         return false;
    if(fabs(d0) >= 0.035904)      return false;
    if(fabs(dZ) >= 0.075496)     return false;
    if(ooEmooP >= 0.189968) return false;
    if(relIsoEA >= 0.130136)          return false;
    if(mHits > 1)              return false;
    if(chargeConsistency < 1)  return false;
    if(!passConversion)        return false;
    //   if(pt <20)                 return false;
  }
  
  //Endcap
  else{
    if(fabs(eta)>2.4)           return false;
    if(fabs(dEta) >= 0.009833)    return false;
    if(fabs(dPhi) >= 0.149934)    return false;
    if(sigmaIetaIeta >= 0.031838)  return false;
    if(hOverE >= 0.115754)        return false;
    if(fabs(d0) >= 0.099266)       return false;
    if(fabs(dZ) >= 0.197897)      return false;
    if(ooEmooP >= 0.140662) return false;
    if(relIsoEA >= 0.163368)        return false;
    if(mHits > 1)              return false;
    if(chargeConsistency < 1)  return false;
    if(!passConversion)        return false;
    //    if(pt <20)                 return false;
  }
  return true;
}

bool TElectron::cutBasedTightApr2015(){
  //Barrel
  if(fabs(eta) <= 1.479){
    if(fabs(dEta) >= 0.006574)    return false;
    if(fabs(dPhi) >= 0.022868)     return false;
    if(sigmaIetaIeta >= 0.010181) return false;
    if(hOverE >= 0.037553)        return false;
    if(fabs(d0) >= 0.009924)      return false;
    if(fabs(dZ) >= 0.015310)      return false;
    if(ooEmooP >= 0.131191) return false;
    if(relIsoEA > 0.1649)        return false;
    if(mHits > 1)              return false;
    if(chargeConsistency < 1)  return false;
    //    if(pt <30)                 return false;
  }
  //Endcap
  else{
    if(fabs(eta)>2.4)           return false;
    if(fabs(dEta) >= 0.005681)    return false;
    if(fabs(dPhi) >= 0.032046)    return false;
    if(sigmaIetaIeta >= 0.028766) return false;
    if(hOverE >= 0.081902)        return false;
    if(fabs(d0) >= 0.0027261)      return false;
    if(fabs(dZ) >= 0.147154)      return false;
    if(ooEmooP >= 0.106055) return false;
    if(relIsoEA >= 0.090185)        return false;
    if(mHits > 1)              return false;
    if(chargeConsistency < 1)  return false;
    //    if(pt <30)                 return false;
  }
  return true;
}

bool TElectron::cutBasedLooseApr2015(){
  //Barrel
  if(fabs(eta) <= 1.479){
    if(fabs(dEta) >= 0.012442)    return false;
    if(fabs(dPhi) >= 0.072624)    return false;
    if(sigmaIetaIeta >= 0.010557) return false;
    if(hOverE >= 0.121476)         return false;
    if(fabs(d0) >= 0.022664)      return false;
    if(fabs(dZ) >= 0.173670)     return false;
    if(ooEmooP >= 0.221803) return false;
    if(relIsoEA >= 0.120026)          return false;
    if(mHits > 1)              return false;
    if(chargeConsistency < 1)  return false;
    //    if(pt <20)                 return false;
  }
  
  //Endcap
  else{
    if(fabs(eta)>2.4)           return false;
    if(fabs(dEta) >= 0.010654)    return false;
    if(fabs(dPhi) >= 0.145129)    return false;
    if(sigmaIetaIeta >= 0.032602)  return false;
    if(hOverE >= 0.131862)        return false;
    if(fabs(d0) >= 0.097358)       return false;
    if(fabs(dZ) >= 0.198444)      return false;
    if(ooEmooP >= 0.142283) return false;
    if(relIsoEA >= 0.162914)        return false;
    if(mHits > 1)              return false;
    if(chargeConsistency < 1)  return false;
    //    if(pt <20)                 return false;
  }
  return true;
}

bool TElectron::CMSDASTight(){
  //Barrel
  if(fabs(eta) <= 1.479){
    if(fabs(dEta) > 0.0091)    return false;
    if(fabs(dPhi) > 0.031)     return false;
    if(sigmaIetaIeta > 0.0106) return false;
    if(hOverE > 0.0532)        return false;
    if(fabs(d0) > 0.0126)      return false;
    if(fabs(dZ) > 0.0116)      return false;
    if(fabs(ooEmooP) > 0.0609) return false;
    if(relIsoEA > 0.1649)        return false;
    if(mHits > 1)              return false;
    if(chargeConsistency < 1)  return false;
    //    if(pt <20)                 return false;
  }
  //Endcap
  else{
    if(fabs(eta)>2.4)           return false;
    if(fabs(dEta) > 0.0106)    return false;
    if(fabs(dPhi) > 0.0359)    return false;
    if(sigmaIetaIeta > 0.0305) return false;
    if(hOverE > 0.0835)        return false;
    if(fabs(d0) > 0.0163)      return false;
    if(fabs(dZ) > 0.5999)      return false;
    if(fabs(ooEmooP) > 0.1126) return false;
    if(relIsoEA > 0.2075)        return false;
    if(mHits > 1)              return false;
    if(chargeConsistency < 1)  return false;
    //    if(pt <20)                 return false;
  }
  return true;
}

bool TElectron::CMSDASLoose(){
  //Barrel
  if(fabs(eta) <= 1.479){
    if(fabs(dEta) > 0.0181)    return false;
    if(fabs(dPhi) > 0.0936)    return false;
    if(sigmaIetaIeta > 0.0123) return false;
    if(hOverE > 0.141)         return false;
    if(fabs(d0) > 0.0166)      return false;
    if(fabs(dZ) > 0.54342)     return false;
    if(fabs(ooEmooP) > 0.1353) return false;
    if(relIsoEA > 0.24)          return false;
    if(mHits > 1)              return false;
    if(chargeConsistency < 1)  return false;
    //    if(pt <20)                 return false;
  }

  //Endcap
  else{
    if(fabs(eta)>2.4)           return false;
    if(fabs(dEta) > 0.0124)    return false;
    if(fabs(dPhi) > 0.0642)    return false;
    if(sigmaIetaIeta > 0.035)  return false;
    if(hOverE > 0.1115)        return false;
    if(fabs(d0) > 0.098)       return false;
    if(fabs(dZ) > 0.9187)      return false;
    if(fabs(ooEmooP) > 0.1443) return false;
    if(relIsoEA > 0.3529)        return false;
    if(mHits > 1)              return false;
    if(chargeConsistency < 1)  return false;
    //    if(pt <20)                 return false;
  }
  return true;
}
