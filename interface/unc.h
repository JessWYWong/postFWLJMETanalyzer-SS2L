#ifndef UNC_H
#define UNC_H
//Created by Jess on Dec 4 2020
//store bkg unc (MC+NP+CMID) for makePlots_forPAS and makeTables in test/

static const float uncLumi  = 0.025;
static const float uncIsoSF = 0.03;

static const float unc_NP = 0.115; //FR only
static const float unc_CMID = 0.589;

static const float uncPU_TTZ  = 0.00835;
static const float uncPU_TTW  = 0.00763;
static const float uncPU_TTH  = 0.000779;
static const float uncPU_TTTT = 0.0017;
static const float uncPU_WZ   = 0.0425;
static const float uncPU_ZZ   = 0.402;
static const float uncPU_WpWp = 0.00487;
static const float uncPU_WWZ  = 0.0508;
static const float uncPU_WZZ  = 0.131;
static const float uncPU_ZZZ  = 0.0861;
static const float uncPU_sig  = 0.013; // RMS (average over all signal samples)

static const float uncTrigSF_TTZ  = 0.0190;
static const float uncTrigSF_TTW  = 0.0193;
static const float uncTrigSF_TTH  = 0.0196;
static const float uncTrigSF_TTTT = 0.0192;
static const float uncTrigSF_WZ   = 0.0189;
static const float uncTrigSF_ZZ   = 0.0206;
static const float uncTrigSF_WpWp = 0.0192;
static const float uncTrigSF_WWZ  = 0.0181;
static const float uncTrigSF_WZZ  = 0.0173;
static const float uncTrigSF_ZZZ  = 0.0206;
static const float uncTrigSF_sig  = 0.0193; // RMS (average over all signal samples)

static const float uncIDSF_TTZ  = 0.0362;
static const float uncIDSF_TTW  = 0.0397;
static const float uncIDSF_TTH  = 0.0337;
static const float uncIDSF_TTTT = 0.0365;
static const float uncIDSF_WZ   = 0.0410;
static const float uncIDSF_ZZ   = 0.0408;
static const float uncIDSF_WpWp = 0.0412;
static const float uncIDSF_WWZ  = 0.0395;
static const float uncIDSF_WZZ  = 0.0392;
static const float uncIDSF_ZZZ  = 0.0508;
static const float uncIDSF_sig  = 0.04; // RMS (average over all signal samples)

static const float uncPDF_TTZ  = 0.01335;
static const float uncPDF_TTW  = 0.01734;
static const float uncPDF_TTH  = 0.01228;
static const float uncPDF_TTTT = 0.0262;
static const float uncPDF_WZ   = 0.01076;
static const float uncPDF_ZZ   = 0.01052;
static const float uncPDF_WpWp = 0.7513;
static const float uncPDF_WWZ  = 0.8279;
static const float uncPDF_WZZ  = 0.1106;
static const float uncPDF_ZZZ  = 0.2032;
static const float uncPDF_sig  = 0.09342; // mean Shift

static const float uncSCALE_TTZ  = 0.115981;
static const float uncSCALE_TTW  = 0.180169;
static const float uncSCALE_TTH  = 0.542189;
static const float uncSCALE_TTTT = 0.274188;
static const float uncSCALE_WZ   = 0.206809;
static const float uncSCALE_ZZ   = 0.266587;
static const float uncSCALE_WpWp = 0.160368;
static const float uncSCALE_WWZ  = 0.194237;
static const float uncSCALE_WZZ  = 0.218229;
static const float uncSCALE_ZZZ  = 0.0554842;
static const float uncSCALE_sig  = 0.00946849; // mean Shift

static const float uncJES_TTZ  = 0.0547;
static const float uncJES_TTW  = 0.0456;
static const float uncJES_TTH  = 0.0589;
static const float uncJES_TTTT = 0.0413;
static const float uncJES_WZ   = 0.0588;
static const float uncJES_ZZ   = 0.0;
static const float uncJES_WpWp = 0.0657;
static const float uncJES_WWZ  = 0.1;
static const float uncJES_WZZ  = 0.0625;
static const float uncJES_ZZZ  = 0.0;
static const float uncJES_sig  = 0.0114403; //mean JES

static const float uncJER_TTZ  = 0.00637;
static const float uncJER_TTW  = 0.00298;
static const float uncJER_TTH  = 0.00365;
static const float uncJER_TTTT = 0.000948;
static const float uncJER_WZ   = 0.0;
static const float uncJER_ZZ   = 0.0;
static const float uncJER_WpWp = 0.0146;
static const float uncJER_WWZ  = 0.0;
static const float uncJER_WZZ  = 0.0;
static const float uncJER_ZZZ  = 0.0;
static const float uncJER_sig  = 0.001937; //mean JER 

#endif

