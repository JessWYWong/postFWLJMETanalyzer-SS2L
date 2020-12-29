#ifndef UNC_H
#define UNC_H
//Created by Jess on Dec 2 2020
//store bkg unc (MC+NP+CMID) for makePlots_forPAS and makeTables in test/

static const float uncLumi  = 0.023;
static const float uncIsoSF = 0.03;

static const float unc_NP = 0.106; //FR only
static const float unc_CMID = 0.314;

static const float uncPU_TTZ  = 0.0091;
static const float uncPU_TTW  = 0.0033;
static const float uncPU_TTH  = 0.0052;
static const float uncPU_TTTT = 0.0013;
static const float uncPU_WZ   = 0.0122;
static const float uncPU_ZZ   = 0.0084;
static const float uncPU_WpWp = 0.0024;
static const float uncPU_WWZ  = 0.0418;
static const float uncPU_WZZ  = 0.0546;
static const float uncPU_ZZZ  = 0.0961;
static const float uncPU_sig  = 0.01; // RMS (average over all signal samples)

static const float uncPrefire_TTZ  = 0.0104;
static const float uncPrefire_TTW  = 0.00933;
static const float uncPrefire_TTH  = 0.00918;
static const float uncPrefire_TTTT = 0.0132;
static const float uncPrefire_WZ   = 0.00834;
static const float uncPrefire_ZZ   = 0.0241;
static const float uncPrefire_WpWp = 0.0095;
static const float uncPrefire_WWZ  = 0.00746;
static const float uncPrefire_WZZ  = 0.0143;
static const float uncPrefire_ZZZ  = 0.;
static const float uncPrefire_sig  = 0.0078; // RMS (average over all signal samples)

static const float uncTrigSF_TTZ  = 0.0192;
static const float uncTrigSF_TTW  = 0.0199;
static const float uncTrigSF_TTH  = 0.0201;
static const float uncTrigSF_TTTT = 0.0198;
static const float uncTrigSF_WZ   = 0.0197;
static const float uncTrigSF_ZZ   = 0.0173;
static const float uncTrigSF_WpWp = 0.0199;
static const float uncTrigSF_WWZ  = 0.0214;
static const float uncTrigSF_WZZ  = 0.0169;
static const float uncTrigSF_ZZZ  = 0.0213;
static const float uncTrigSF_sig  = 0.02; // RMS (average over all signal samples)

static const float uncIDSF_TTZ  = 0.0541;
static const float uncIDSF_TTW  = 0.0463;
static const float uncIDSF_TTH  = 0.0397;
static const float uncIDSF_TTTT = 0.0419;
static const float uncIDSF_WZ   = 0.0783;
static const float uncIDSF_ZZ   = 0.128;
static const float uncIDSF_WpWp = 0.0496;
static const float uncIDSF_WWZ  = 0.0349;
static const float uncIDSF_WZZ  = 0.0537;
static const float uncIDSF_ZZZ  = 0.09;
static const float uncIDSF_sig  = 0.058; // RMS (average over all signal samples)

static const float uncPDF_TTZ  = 0.01407;
static const float uncPDF_TTW  = 0.01539;
static const float uncPDF_TTH  = 0.1876;
static const float uncPDF_TTTT = 0.03016;
static const float uncPDF_WZ   = 0.01922;
static const float uncPDF_ZZ   = 0.01091;
static const float uncPDF_WpWp = 0.7348;
static const float uncPDF_WWZ  = 0.2114;
static const float uncPDF_WZZ  = 0.2283;
static const float uncPDF_ZZZ  = 0.09059;
static const float uncPDF_sig  = 0.168; // mean Shift

static const float uncSCALE_TTZ  = 0.109376;
static const float uncSCALE_TTW  = 0.181078;
static const float uncSCALE_TTH  = 0.257886;
static const float uncSCALE_TTTT = 0.24518;
static const float uncSCALE_WZ   = 0.194237;
static const float uncSCALE_ZZ   = 0.0582732;
static const float uncSCALE_WpWp = 0.160057;
static const float uncSCALE_WWZ  = 0.199757;
static const float uncSCALE_WZZ  = 0.19327;
static const float uncSCALE_ZZZ  = 0.216287;
static const float uncSCALE_sig  = 0.00800039; // mean Shift

static const float uncJES_TTZ  = 0.0388;
static const float uncJES_TTW  = 0.0317;
static const float uncJES_TTH  = 0.0384;
static const float uncJES_TTTT = 0.0254;
static const float uncJES_WZ   = 0.0524;
static const float uncJES_ZZ   = 0.143;
static const float uncJES_WpWp = 0.0517;
static const float uncJES_WWZ  = 0.0;
static const float uncJES_WZZ  = 0.0714;
static const float uncJES_ZZZ  = 0.0;
static const float uncJES_sig  = 0.0085; //mean JES

static const float uncJER_TTZ  = 0.00935;
static const float uncJER_TTW  = 0.00673;
static const float uncJER_TTH  = 0.00726;
static const float uncJER_TTTT = 0.00393;
static const float uncJER_WZ   = 0.0419;
static const float uncJER_ZZ   = 0.0714;
static const float uncJER_WpWp = 0.00345;
static const float uncJER_WWZ  = 0.0;
static const float uncJER_WZZ  = 0.0;
static const float uncJER_ZZZ  = 0.0;
static const float uncJER_sig  = 0.00327; //mean JER 
#endif
