## Post FWLJMET analysis code for same-sign dilepton analysis

Originally forked from https://github.com/clint-richardson/X53ThirteenTeVAnalysisCode --> https://github.com/rsyarif/X53ThirteenTeVAnalysisCode

### Intructions:

git clone -b 2017data git@github.com:rsyarif/postFWLJMETanalyzer-SS2L.git postFWLJMETanalyzer

cd postFWLJMETanalyzer

mkdir lib

make (compiles plugins, etc)

cd test/

source setup.csh (this set up the library path)

make (compiles the analyzer on default but can be configured via makefile)

Needed to update:

* TreeReader.cc/h
* X53Analyzer.cc
* Macro.cc
* (and possibly others, say of you need to update s, TElectron.h/cc if you need to update electron ID, say.)

test:

* ./X53Analyzer.o <some argument needed check existing sh files for examples>

run jobs on condor:
* python condor_submitargs.py

check error in the logs just outside the CMSSW dir:
      ID