#include "myHeader.h"

#include "DmpEvtBgoHits.h"
#include "DmpEvtBgoRec.h"

#include "DmpStkEventMetadata.h"

void getBGOdata(const std::string inputFilePath,const bool verbosity)
{
    gSystem->Load(dampeEvtLib.c_str());

    TFile* dataFile = TFile::Open(inputFilePath.c_str());
    if(dataFile->IsZombie())
    {
        std::cout << "\n\nError reading input data file: " << inputFilePath << std::endl << std::endl;
        exit(100);
    }

    // Register STK collections
    DmpStkEventMetadata* stkMetadata = new DmpStkEventMetadata();

    TTree* dataTree = (TTree*) dataFile->Get("CollectionTree");

    // Register BGO constainer
    DmpEvtBgoHits* bgohits  = new  DmpEvtBgoHits();
    dataTree->SetBranchAddress("DmpEvtBgoHits",&bgohits);

    // Register BGO REC constainer
    DmpEvtBgoRec* bgorec  = new  DmpEvtBgoRec();
    dataTree->SetBranchAddress("DmpEvtBgoRec",&bgorec);

    // Event LOOP
    for(int entry=0; entry<dataTree->GetEntries(); entry++)
    {
        //for(int entry=0; entry<10; entry++){
        dataTree->GetEntry(entry);

        // STK metadata
        if(verbosity)
            printf("STK data mode = %d\n", stkMetadata->fRunMode );   
            // STK modes: 2 (COMPRESSED), 3(RAW), 5(ULD),  6(DLD)
            // ... for more information see: http://dpnc.unige.ch/SVNDAMPE/DAMPE/DmpSoftware/trunk/Event/Stk/include/DmpStkEventMetadata.h

        // Get BGO energy and direction
        if(verbosity)
            printf("BGO total energy = %f\n", bgorec->GetTotalEnergy());
        // BGO trajectory 
        double x = bgorec->GetTrajectoryLocation2D().x();
        double y = bgorec->GetTrajectoryLocation2D().y();
        double z = bgorec->GetTrajectoryLocation2D().z();
        double incl_x = bgorec->GetSlopeXZ();
        double incl_y = bgorec->GetSlopeYZ();

        // Loop over the BGO (PSD) hits
        for(int i=0; i<bgohits->fEnergy.size(); i++)
        {
            if(verbosity)
            {
                printf("\nBGO hit information:\n");
                printf("   hit energy: %f\n",bgohits->fEnergy[i]);
                printf("   hit x=%f\n", bgohits->GetHitX(i));
                printf("   hit y=%f\n", bgohits->GetHitY(i));
                printf("   hit z=%f\n", bgohits->GetHitZ(i));
                // etc.
                // For more details on BGO event class see http://dpnc.unige.ch/SVNDAMPE/DAMPE/DmpSoftware/trunk/Event/Bgo/include/DmpEvtBgoHits.h
            }
        } 
    }
}