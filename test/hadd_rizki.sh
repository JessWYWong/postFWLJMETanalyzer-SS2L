#!/bin/bash

#folder=root://cmseos.fnal.gov//store/user/lpcljm/LJMet94x_2lepTT_2017datasets_2018_11_18_rizki_hadds_Analyzer
#folder=/store/user/wywong/FWLJMET102X_2lep2018_wywong_082020_hadds_postFWLJMETanalyzer_IsoTrig_IDSFunc
#folder=/store/user/wywong/FWLJMET102X_2lep2018_wywong_082020_hadds_postFWLJMETanalyzer_IsoTrig_HEMveto_ANv8
#folder=/store/user/wywong/FWLJMET102X_2lep2018_wywong_082020_hadds_postFWLJMETanalyzer_nonIsoTrig_HEMveto_Aug21_njet3_3LFR_dFRMu2El8_uPR
#folder=/store/user/wywong/FWLJMET102X_2lep2018_wywong_082020_hadds_postFWLJMETanalyzer_nonIsoTrig_HEMveto_Aug21_njet3_3LFR_dFRMu2El8_bTag
#folder=/store/user/wywong/FWLJMET102X_2lep2018_wywong_082020_hadds_postFWLJMETanalyzer_nonIsoTrig_HEMveto_Aug21_njet3_3LFR_dFRMu2El8_uPR_CMIDnConst3
folder=/store/user/wywong/FWLJMET102X_2lep2018_wywong_082020_hadds_postFWLJMETanalyzer_nonIsoTrig_HEMveto_Aug21_njet3_3LFRminDR_dFRMu8El10
era=2018A-D_HEMveto
elID=MVA2017TightV2IsoTightRC
muID=CBTightMiniIsoTight

#hadd -f root://cmseos.fnal.gov/$folder/Data_Mu${muID}_El${elID}_${era}.root `(xrdfs root://cmseos.fnal.gov ls -u $folder | grep "/Data") | grep -v "Data_Mu${muID}_El${elID}_${era}.root"`
hadd -f root://cmseos.fnal.gov/$folder/NonPromptData_Mu${muID}_El${elID}_${era}.root `(xrdfs root://cmseos.fnal.gov ls -u $folder | grep "/NonPromptData") | grep -v "NonPromptData_Mu${muID}_El${elID}_${era}.root" | grep -v "Unity"`
#hadd -f root://cmseos.fnal.gov/$folder/ChargeMisID_Mu${muID}_El${elID}_${era}.root `(xrdfs root://cmseos.fnal.gov ls -u $folder | grep "/ChargeMisID") | grep -v "ChargeMisID_" | grep -v "EvtOverlap.root"`
#hadd -f root://cmseos.fnal.gov/$folder/NonPromptData_Mu${muID}_El${elID}_${era}_UnityElPR.root `(xrdfs root://cmseos.fnal.gov ls -u $folder | grep "UnityElPR.root") | grep -v "NonPromptData_Mu${muID}_El${elID}_${era}_UnityElPR.root"`
#hadd -f root://cmseos.fnal.gov/$folder/NonPromptData_Mu${muID}_El${elID}_${era}_UnityMuPR.root `(xrdfs root://cmseos.fnal.gov ls -u $folder | grep "UnityMuPR.root") | grep -v "NonPromptData_Mu${muID}_El${elID}_${era}_UnityMuPR.root"`


#hadd -f $folder/Data_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root `xrdfs root://cmseos.fnal.gov ls -u /store/user/lpcljm/LJMet94x_2lepTT_2017datasets_2018_11_18_rizki_hadds_Analyzer | grep "/Data"`

# hadd -f $folder/NonPromptData_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root `xrdfs root://cmseos.fnal.gov ls -u /store/user/lpcljm/LJMet94x_2lepTT_2017datasets_2018_11_18_rizki_hadds_Analyzer | grep "NonPrompt"`
# 
# 
# hadd -f $folder/ChargeMisID_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root `xrdfs root://cmseos.fnal.gov ls -u /store/user/lpcljm/LJMet94x_2lepTT_2017datasets_2018_11_18_rizki_hadds_Analyzer | grep "Charge"`
# 

# hadd -f $folder/NonPromptData_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# 
# $folder/NonPromptDataElEl2017B_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/NonPromptDataElEl2017C_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/NonPromptDataElEl2017D_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/NonPromptDataElEl2017E_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/NonPromptDataElEl2017F_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/NonPromptDataElMu2017B_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/NonPromptDataElMu2017C_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/NonPromptDataElMu2017D_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/NonPromptDataElMu2017E_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/NonPromptDataElMu2017F_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/NonPromptDataMuMu2017B_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/NonPromptDataMuMu2017C_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/NonPromptDataMuMu2017D_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/NonPromptDataMuMu2017E_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/NonPromptDataMuMu2017F_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root



# hadd -f $folder/ChargeMisID_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# 
# $folder/ChargeMisIDElEl2017B_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/ChargeMisIDElEl2017C_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/ChargeMisIDElEl2017D_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/ChargeMisIDElEl2017E_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/ChargeMisIDElEl2017F_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/ChargeMisIDElMu2017B_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/ChargeMisIDElMu2017C_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/ChargeMisIDElMu2017D_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/ChargeMisIDElMu2017E_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root \
# $folder/ChargeMisIDElMu2017F_MuCBTightMiniIsoTight_ElMVA2016TightRC_2017B-F.root



# Missing:
