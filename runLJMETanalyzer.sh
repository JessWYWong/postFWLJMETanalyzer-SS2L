#!/bin/bash

hostname

sample=${1}
era=${2}
decay=${3}
outputdir=${4}
PRUNCmode=${5}

source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc7_amd64_gcc630


scramv1 project CMSSW CMSSW_10_2_10
cd CMSSW_10_2_10
eval `scramv1 runtime -sh`
cd -

tar -xf postFWLJMETanalyzer.tar
cd postFWLJMETanalyzer/test/

#source setup.csh
export LD_LIBRARY_PATH=../lib/:$LD_LIBRARY_PATH 

echo "Executing: ./LJMETanalyzer.o ${sample} MVA2017TightV2IsoTightRC CBTightMiniIsoTight ${era} ${decay} ${PRUNCmode}"

./LJMETanalyzer.o $sample MVA2017TightV2IsoTightRC CBTightMiniIsoTight $era $decay $PRUNCmode
#./LJMETanalyzer.o TprimeTprime_M-1500 MVA2017TightV2IsoRC CBTightMiniIsoTight dummyEra TZTZ

#rm Pileup*.root

echo "ROOT Files to transfer:"
ls -l *.root

echo "xrdcp output for condor"
for FILE in *.root
do
  echo "xrdcp -f ${FILE} root://cmseos.fnal.gov/${outputdir}/${FILE}"
  xrdcp -f ${FILE} root://cmseos.fnal.gov/${outputdir}/${FILE} 2>&1
  XRDEXIT=$?
  if [[ $XRDEXIT -ne 0 ]]; then
    rm *.root
    echo "exit code $XRDEXIT, failure in xrdcp"
    exit $XRDEXIT
  fi
  rm ${FILE}
done
