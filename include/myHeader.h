#ifndef myHeader_H
#define myHeader_H

//// C/C++ includes

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <sstream>

//// ROOT includes

#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TClonesArray.h"
#include "TChainIndex.h"
#include "TH1D.h"
#include "TString.h"

//// DAMPE evtLibrary

const std::string dampeEvtLib="/storage/gpfs_data/dampe/users/ecatanzani/myRepos/DAMPE/DAMPE_Analysis/Event/libDmpEvent.so";
//const string dampeEvtLib="/storage/gpfs_data/dampe/users/ecatanzani/myRepos/DAMPE/DAMPE_Analysis/Event/libDmpEvent.so";

//// pBar library

#include "progHeader.h"

//// Data Analysis functions

extern void getTracks(
                        const std::string inputFilePath,
                        const std::string outPrefix, 
                        const bool verbosity,
                        const ULong64_t sDFactor
                      );

extern void getBGOdata(
                        const std::string inputFilePath,
                        const std::string outPrefix,
                        const bool verbosity,
                        const ULong64_t sDFactor
                      );

extern void getAncillarydata(
                              const std::string inputFilePath,
                              const std::string outPrefix,
                              const bool verbosity,
                              const ULong64_t sDFactor
                            );

extern void readConfigFile(
                            const std::string configFile,
                            bool &verbosity,
                            std::vector<bool> &analysis,
                            ULong64_t &sDFactor
                          );


#endif