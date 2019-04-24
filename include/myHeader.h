#ifndef MYHEADER_H
#define MYHEADER_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "TFile.h"
#include "TTree.h"

const string dampeEvtLib="/storage/gpfs_data/dampe/users/ecatanzani/myRepos/DAMPE/DAMPE_Analysis/Event/libDmpEvent.so";
//const string dampeEvtLib="/storage/gpfs_data/dampe/users/ecatanzani/myRepos/DAMPE/DAMPE_Analysis/Event/libDmpEvent.so";

//// Data Analysis functions

void getTracks(const std::string inputFilePath);



#endif