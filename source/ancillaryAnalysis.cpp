#include "myHeader.h"

#include "DmpStkEventMetadata.h"

#include "DmpStkEventMetadata.h"
#include "AncillaryEvent.hh"
#include "AncillaryEventIons.hh"

void getAncillarydata(const std::string inputFilePath,const bool verbosity)
{
    gSystem->Load(dampeEvtLib.c_str());

    TFile* dataFile = TFile::Open(inputFilePath.c_str());
    if(dataFile->IsZombie())
    {
        std::cout << "\n\nError reading input data file: " << inputFilePath << std::endl << std::endl;
        exit(100);
    }

    TTree* dataTree = (TTree*) dataFile->Get("CollectionTree");

    // Register STK collections
    DmpStkEventMetadata* stkMetadata = new DmpStkEventMetadata();
    TClonesArray* stkladderadc = new TClonesArray("DmpStkLadderAdc");

    // Register ANCILLARY container
    AncillaryEventIons* ancevent = new AncillaryEventIons();
    dataTree->SetBranchAddress("AncillaryEventIons",&ancevent);

    // Event LOOP
   for(int entry=0; entry<dataTree->GetEntries(); entry++)
   {
        dataTree->GetEntry(entry);
      
        // STK metadata
        if(verbosity)
            printf("\nSTK data mode = %d\n", stkMetadata->fRunMode );
        
        // Ancillary detector
        if(verbosity)
        {
            printf("\nAnclillary detector information:\n");
            printf("   ADC s1      = %f\n",ancevent->s1);
            printf("   ADC s1_6db  = %f\n",ancevent->s1_6db);
            printf("   ADC s1_12db = %f\n",ancevent->s1_12db);
            printf("   ADC s2      = %f\n",ancevent->s2);
            printf("   ADC s2_6db  = %f\n",ancevent->s2_6db);
            printf("   ADC s2_12db = %f\n",ancevent->s2_12db);
            printf("   ADC si0_sg = %f\n",ancevent->si0_sg);
            printf("   ADC si0_lg = %f\n",ancevent->si0_lg);
            printf("   ADC si1_sg = %f\n",ancevent->si1_sg);
            printf("   ADC si1_lg = %f\n",ancevent->si1_lg);
            // etc.
            // For more information see http://dpnc.unige.ch/SVNDAMPE/DAMPE/DmpSoftware/trunk/Event/Ams/include/AncillaryEventIons.hh
        }
   }

}