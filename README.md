## Post FWLJMET analysis code for same-sign dilepton analysis

Originally forked from https://github.com/clint-richardson/X53ThirteenTeVAnalysisCode --> https://github.com/rsyarif/X53ThirteenTeVAnalysisCode

### Intructions:

    git clone -b 2017data git@github.com:rsyarif/postFWLJMETanalyzer-SS2L.git postFWLJMETanalyzer

    cd postFWLJMETanalyzer

    mkdir lib

compile plugins, etc:

	make

	cd test/

setup the library path:

      source setup.csh

compile analyzer (by default but can be configured via makefile)

	make


Probably need to update:

* TreeReader.cc/h
* X53Analyzer.cc
* Macro.cc
* (and possibly others, say of you need to update s, TElectron.h/cc if you need to update electron ID, say.)

run interactive test:

    ./X53Analyzer.o <some argument needed check existing sh files for examples>

run jobs on condor:

    python condor_submitargs.py

check error in the logs just outside the CMSSW dir:

      grep ERROR *.out


Data, NP, and CMID need to be hadded after analyzer step before using the plotter/table maker
* eg.  hadd_Data_NP_CMID_afterAnalyzer.sh

Duplicate check:
*   root -l  dupCheck.cc


Prompt rate measurement:
* ./PromptRate_2017data.o <Data/MC> <El/Mu> <ID> <era>
