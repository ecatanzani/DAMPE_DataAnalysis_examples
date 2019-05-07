#ifndef myHeader_H
#define myHeader_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <sstream>

#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TClonesArray.h"
#include "TChainIndex.h"

const std::string dampeEvtLib="/storage/gpfs_data/dampe/users/ecatanzani/myRepos/DAMPE/DAMPE_Analysis/Event/libDmpEvent.so";
//const string dampeEvtLib="/storage/gpfs_data/dampe/users/ecatanzani/myRepos/DAMPE/DAMPE_Analysis/Event/libDmpEvent.so";

//// Data Analysis functions

extern void getTracks(const std::string inputFilePath, const bool verbosity);
extern void getBGOdata(const std::string inputFilePath,const bool verbosity);
extern void getAncillarydata(const std::string inputFilePath,const bool verbosity);

extern void readConfigFile(
                            const std::string configFile,
                            bool &verbosity,
                            std::vector<bool> &analysis
                          );


#endif