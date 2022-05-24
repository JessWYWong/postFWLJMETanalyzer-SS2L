#ifndef UNC_H
#define UNC_H
//Created by Jess on Dec 4 2020
//store bkg unc (MC+NP+CMID) for makePlots_forPAS and makeTables in test/

static const float uncLumi  = 0.025;
static const float uncIsoSF = 0.03;

static const float unc_NP = 0.115; //FR only
static const float unc_CMID = 0.589;

static const float uncPU_TTZ  = 0.00961;
static const float uncPU_TTW  = 0.00862;
static const float uncPU_TTH  = 0.000702;
static const float uncPU_TTTT = 0.00125;
static const float uncPU_WZ   = 0.0425;
static const float uncPU_ZZ   = 0.402;
static const float uncPU_WpWp = 0.00487;
static const float uncPU_WWZ  = 0.0508;
static const float uncPU_WZZ  = 0.131;
static const float uncPU_ZZZ  = 0.0861;
static const float uncPU_sig  = 0.0032; // max(RMSup,RMSdn) TTM1000

static const float uncTrigSF_TTZ  = 0.0137;
static const float uncTrigSF_TTW  = 0.0139;
static const float uncTrigSF_TTH  = 0.0140;
static const float uncTrigSF_TTTT = 0.0138;
static const float uncTrigSF_WZ   = 0.0137;
static const float uncTrigSF_ZZ   = 0.0146;
static const float uncTrigSF_WpWp = 0.0138;
static const float uncTrigSF_WWZ  = 0.0132;
static const float uncTrigSF_WZZ  = 0.0128;
static const float uncTrigSF_ZZZ  = 0.0146;
static const float uncTrigSF_sig  = 0.014; // max(RMSup,RMSdn) TTM1000

static const float uncIDSF_TTZ  = 0.0161;
static const float uncIDSF_TTW  = 0.0161;
static const float uncIDSF_TTH  = 0.0102;
static const float uncIDSF_TTTT = 0.0131;
static const float uncIDSF_WZ   = 0.0193;
static const float uncIDSF_ZZ   = 0.0;
static const float uncIDSF_WpWp = 0.0195;
static const float uncIDSF_WWZ  = 0.0314;
static const float uncIDSF_WZZ  = 0.0267;
static const float uncIDSF_ZZZ  = 0.0303;
static const float uncIDSF_sig  = 0.0179; // max(RMSup,RMSdn) TTM1000

static const float uncPDF_TTZ  = 0.01337;
static const float uncPDF_TTW  = 0.01736;
static const float uncPDF_TTH  = 0.01228;
static const float uncPDF_TTTT = 0.02621;
static const float uncPDF_WZ   = 0.01083;
static const float uncPDF_ZZ   = 0.01048;
static const float uncPDF_WpWp = 0.7525;
static const float uncPDF_WWZ  = 0.8469;
static const float uncPDF_WZZ  = 0.1116;
static const float uncPDF_ZZZ  = 0.2122;
static const float uncPDF_sig  = 0.00349; // max(RMSup,RMSdn) TTM1000

static const float uncSCALE_TTZ  = 0.116;
static const float uncSCALE_TTW  = 0.180;
static const float uncSCALE_TTH  = 0.542;
static const float uncSCALE_TTTT = 0.274;
static const float uncSCALE_WZ   = 0.207;
static const float uncSCALE_ZZ   = 0.267;
static const float uncSCALE_WpWp = 0.160;
static const float uncSCALE_WWZ  = 0.194;
static const float uncSCALE_WZZ  = 0.218;
static const float uncSCALE_ZZZ  = 0.0555;
static const float uncSCALE_sig  = 0.00112; // max(RMSup,RMSdn) TTM1000

static const float uncJES_TTZ  = 0.0551;
static const float uncJES_TTW  = 0.0456;
static const float uncJES_TTH  = 0.0593;
static const float uncJES_TTTT = 0.0414;
static const float uncJES_WZ   = 0.0588;
static const float uncJES_ZZ   = 0.0;
static const float uncJES_WpWp = 0.0657;
static const float uncJES_WWZ  = 0.1;
static const float uncJES_WZZ  = 0.0625;
static const float uncJES_ZZZ  = 0.0;
static const float uncJES_sig  = 0.0172; // max(RMSup,RMSdn) TTM1000

static const float uncJER_TTZ  = 0.00641;
static const float uncJER_TTW  = 0.00301;
static const float uncJER_TTH  = 0.00367;
static const float uncJER_TTTT = 0.00103;
static const float uncJER_WZ   = 0.0;
static const float uncJER_ZZ   = 0.0;
static const float uncJER_WpWp = 0.0146;
static const float uncJER_WWZ  = 0.0;
static const float uncJER_WZZ  = 0.0;
static const float uncJER_ZZZ  = 0.0;
static const float uncJER_sig  = 0.00091; // max(RMSup,RMSdn) TTM1000

#endif

