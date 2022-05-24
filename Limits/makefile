CXX=g++

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs)

ROOTIL      = $(ROOTCFLAGS) $(ROOTLIBS)

LIBS        = libObjects.so

PWD         = $(shell pwd)

TOOLDIR     = $(PWD)/tools

makeLimitCards:
	$(CXX) $(ROOTIL) -L$(PWD)/../lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o makeLimitCards.o makeLimitCards.cc

makeLimitCards_Combined:
	$(CXX) $(ROOTIL) -L$(PWD)/../lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o makeLimitCards_Combined.o makeLimitCards_Combined.cc

makeLimitCards_MCVar:
	$(CXX) $(ROOTIL) -L$(PWD)/../lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o makeLimitCards_MCVar.o makeLimitCards_MCVar.cc

makeLimitRootFiles:
	$(CXX) $(ROOTIL) -L$(PWD)/../lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o makeLimitRootFiles.o makeLimitRootFiles.cc

makeLimitRootFiles_Combine:
	$(CXX) $(ROOTIL) -L$(PWD)/../lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o makeLimitRootFiles_Combine.o makeLimitRootFiles_Combine.cc

makeThetaTemplates:
	$(CXX) $(ROOTIL) -L$(PWD)/../lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o makeThetaTemplates.o makeThetaTemplates.cc

makeLimitRootFiles_Boosted:
	$(CXX) $(ROOTIL) -L$(PWD)/../lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o makeLimitRootFiles_Boosted.o makeLimitRootFiles_Boosted.cc

makeLimitRootFiles_miniIso:
	$(CXX) $(ROOTIL) -L$(PWD)/../lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o makeLimitRootFiles_miniIso.o makeLimitRootFiles_miniIso.cc

makeTemplate_Combine:
	$(CXX) $(ROOTIL) -L$(PWD)/../lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o makeTemplate_Combine.o makeTemplate_Combine.cc

makeTemplateCat_Combine:
	$(CXX) $(ROOTIL) -L$(PWD)/../lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o makeTemplateCat_Combine.o makeTemplateCat_Combine.cc

makeTemplateCat_CombineNewSig:
	$(CXX) $(ROOTIL) -L$(PWD)/../lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o makeTemplateCat_CombineNewSig.o makeTemplateCat_CombineNewSig.cc

makeTemplateCat_Combine_nConst3:
	$(CXX) $(ROOTIL) -L$(PWD)/../lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o makeTemplateCat_Combine_nConst3.o makeTemplateCat_Combine_nConst3.cc

makeTemplateCat_CombineVar:
	$(CXX) $(ROOTIL) -L$(PWD)/../lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o makeTemplateCat_CombineVar.o makeTemplateCat_CombineVar.cc

makeTemplateCat_CombineVar_nConst3:
	$(CXX) $(ROOTIL) -L$(PWD)/../lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o makeTemplateCat_CombineVar_nConst3.o makeTemplateCat_CombineVar_nConst3.cc

checkBinning:
	$(CXX) $(ROOTIL) -L$(PWD)/../lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o checkBinning.o checkBinning.cc

checkBinning_nConst3:
	$(CXX) $(ROOTIL) -L$(PWD)/../lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o checkBinning_nConst3.o checkBinning_nConst3.cc
