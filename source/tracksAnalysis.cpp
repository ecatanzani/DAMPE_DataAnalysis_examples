#include "myHeader.h"
#include "DmpStkEventMetadata.h"

void getTracks(const std::string inputFilePath)
{
    gSystem->Load(dampeEvtLib.c_str());
    
    TFile dataFile(inputFilePath.c_str(),"READ");
    if(dataFile.IsZombie())
    {
        std::cout << "\n\nError reading input data file: " << inputFilePath << std::endl << std::endl;
        exit(100);
    }

    TTree* dataTree = (TTree*) dataFile.Get("CollectionTree");

    // Register STK collections
    TClonesArray* stkclusters = new TClonesArray("DmpStkSiCluster");
    DmpStkEventMetadata* stkMetadata = new DmpStkEventMetadata();
    TClonesArray* stkladderadc = new TClonesArray("DmpStkLadderAdc");
   
    dataTree->SetBranchAddress("StkClusterCollection",&stkclusters);
    dataTree->SetBranchAddress("DmpStkEventMetadata", &stkMetadata);
    dataTree->SetBranchAddress("DmpStkLadderAdcCollection", &stkladderadc);

    // Check if STK tracks collection exists 
    bool fStkKalmanTracksFound = false;
    for(int i=0; i<dataTree->GetListOfBranches()->GetEntries(); ++i)
        if(std::string(dataTree->GetListOfBranches()->At(i)->GetName()) == std::string("StkKalmanTracks"))
        {
            fStkKalmanTracksFound = true;
            break;
        }
    
}