#ifndef TElectron_h
#define TElectron_h

#include "TLepton.h"

class TElectron : public TLepton{
public:
  //constructor
  TElectron(
      double ptTemp,
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
      int chargeconsitencytemp, 
      double mvatemp, 
      double mva80xtemp, 
      double miniIsotemp, 
      double susyisotemp
      );

  //varibles for charge
  int gsfCharge;
  int ctfCharge;
  int scpixCharge;
  //variables for tracking cuts
  double dEta; 
  double dPhi;
  //variables for primary vtx cuts
  double dZ;
  double SIP3d;
  double d0;
  // Hadronic energy divided by the Electromagnetic
  double hOverE;
  //Minimum hits
  double mHits;
  double ooEmooP;
  //shower shape
  double sigmaIetaIeta;
  //Isolation
  double chIso;
  double puIso;
  double neutIso;
  double photIso;
  double rhoIso;
  double AEff;
  double relIsoDB;
  double relIsoEA;
  //passed conversion cuts
  int passConversion;
  //charge consistency
  int    chargeConsistency;

  // mvaID value and ID flags
  double mvaValue;
  double mvaValue80X;
  double miniIso;
  double susyIso;


  //94X - el MVA v2 - https://twiki.cern.ch/twiki/bin/viewauth/CMS/MultivariateElectronIdentificationRun2#Recommended_MVA_Recipe_V2_for_re
  bool ISmva94XTightV2_80;
  bool ISmva94XTightV2_90;
  bool ISmva94XLooseV2;

  void set_mva94XTightV2_80(bool value);
  void set_mva94XTightV2_90(bool value);
  void set_mva94XLooseV2(bool value);

  bool mva94XTightV2_80();
  bool mva94XTightV2_80_Iso();
  bool mva94XTightV2_80_Iso_RC();
  bool mva94XTightV2_80_RC();
  bool mva94XTightV2_90();
  bool mva94XTightV2_90_Iso();
  bool mva94XTightV2_90_Iso_RC();
  bool mva94XTightV2_90_RC();
  bool mva94XLooseV2();
  bool mva94XLooseV2_Iso();
  bool mva94XLooseV2_Iso_RC();
  bool mva94XLooseV2_RC();


  bool ISmva94XTightIsoV2_80;
  bool ISmva94XTightIsoV2_90;
  bool ISmva94XLooseIsoV2;

  void set_mva94XTightIsoV2_80(bool value);
  void set_mva94XTightIsoV2_90(bool value);
  void set_mva94XLooseIsoV2(bool value);

  bool mva94XTightIsoV2_80();
  bool mva94XTightIsoV2_80_RC();
  bool mva94XTightIsoV2_90();
  bool mva94XTightIsoV2_90_RC();
  bool mva94XLooseIsoV2();
  bool mva94XLooseIsoV2_RC();


  
  //94X - https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentificationRun2#MVA_Recipe_V1_for_regular_users
  bool mva94XTightV1();
  bool mva94XTightV1_Iso();
  bool mva94XTightV1_Iso_RC();
  bool mva94XTightV1_RC();
  bool mva94XLooseV1();
  bool mva94XLooseV1_Iso();
  bool mva94XLooseV1_Iso_RC();
  bool mva94XLooseV1_RC();

  bool mvaTight();
  bool mva2016Tight();
  bool mva80XTight();
  bool mvaJulieTight();
  bool mvaJulieNewTight();
  bool mvaLoose();
  bool mva80XLoose();
  bool mvaJulieLoose();
  bool mvaTightIso();
  bool mva80XTightIso();
  bool mva2016TightIso();
  bool mvaJulieTightIso();
  bool mvaJulieNewTightIso();
  bool mvaTightMedIso();
  bool mvaLooseIso();
  bool mva80XLooseIso();
  bool mvaJulieLooseIso();

  bool mvaTightNew();
  bool mvaLooseNew();
  bool mvaTightNewRC();
  bool mvaLooseNewRC();


  bool mvaTightRC();
  bool mva80XTightRC();
  bool mva2016TightRC();
  bool mvaJulieTightRC();
  bool mvaJulieNewTightRC();
  bool mvaLooseRC();
  bool mva80XLooseRC();
  bool mvaJulieLooseRC();
  bool mvaTightCC();
  bool mvaLooseCC();
  bool mvaTightCCIso();
  bool mvaLooseCCIso();
  bool mvaTightRCIso();
  bool mva80XTightRCIso();
  bool mva2016TightRCIso();
  bool mvaJulieTightRCIso();
  bool mvaJulieNewTightRCIso();
  bool mvaTightRCMedIso();
  bool mvaTightLCIso();
  bool mvaLooseRCIso();
  bool mva80XLooseRCIso();
  bool mvaJulieLooseRCIso();
  bool mvaLooseLCIso();

  //susy ids
  bool susyTight();
  bool susyLoose();
  bool susyTightRC();
  bool susyLooseRC();

  bool cutBasedTight50ns();
  bool cutBasedLoose50ns();
  bool cutBasedTight25nsSpring15MC();
  bool cutBasedLoose25nsSpring15MC();
  bool cutBasedTight25nsSpring15MCRC();
  bool cutBasedLoose25nsSpring15MCRC();
  bool cutBasedTightMay2015();
  bool cutBasedLooseMay2015();
  bool cutBasedTightApr2015();
  bool cutBasedLooseApr2015();
  bool CMSDASTight();
  bool CMSDASLoose();

  void init(){
    pt   = -100;
    eta  = -100;
    phi  = -100;
    
    dEta = -100;
    dPhi = -100;

    dZ   = -100;
    SIP3d= -100;
    d0   = -100;
    
    hOverE  = -100;
    
    mHits   =  100;
    ooEmooP = -100;
    
    relIsoEA = 100;
    sigmaIetaIeta = -100;
    
    chargeConsistency = 0;

  }

};

#endif
