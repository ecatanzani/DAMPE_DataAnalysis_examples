#include "myHeader.h"

#include "DmpEvtBgoHits.h"
#include "DmpEvtBgoRec.h"

#include "DmpStkEventMetadata.h"

void getBGOdata(
                    const std::string inputFilePath,
                    const std::string outPrefix,
                    const bool verbosity,
                    const ULong64_t sDFactor
                )
{
    ULong64_t totEvents = 0;
    double pctgEvt = 0;

    gSystem->Load(dampeEvtLib.c_str());

    TFile* dataFile = TFile::Open(inputFilePath.c_str());
    if(dataFile->IsZombie())
    {
        std::cout << "\n\nError reading input data file: " << inputFilePath << std::endl << std::endl;
        exit(100);
    }

    TTree* dataTree = (TTree*) dataFile->Get("CollectionTree");

    totEvents = (ULong64_t)dataFile->Get("CollectionTree")/sDFactor;
    if(verbosity)
        std::cout << "\n" << totEvents << " will be read from data input file\n";

    // Register STK collections
    DmpStkEventMetadata* stkMetadata = new DmpStkEventMetadata();
    dataTree->SetBranchAddress("DmpStkEventMetadata", &stkMetadata);

    // Register BGO constainer
    DmpEvtBgoHits* bgohits  = new  DmpEvtBgoHits();
    dataTree->SetBranchAddress("DmpEvtBgoHits",&bgohits);

    // Register BGO REC constainer
    DmpEvtBgoRec* bgorec  = new  DmpEvtBgoRec();
    dataTree->SetBranchAddress("DmpEvtBgoRec",&bgorec);

    //// Histos

    TH1D trajectoryBGOX("trajectoryBGOX","X trajectory BGO",1000,0,100);
    TH1D trajectoryBGOY("trajectoryBGOY","Y trajectory BGO",1000,0,100);
    TH1D trajectoryBGOZ("trajectoryBGOZ","Z trajectory BGO",1000,0,100);

    TH1D BGOslopeXZ("BGOslopeXZ","slope BG XZ",1000,0,100);
    TH1D BGOslopeYZ("BGOslopeYZ","slope BG YZ",1000,0,100);

    TH1D BGOhitsX("BGOhitsX","BGO hits X",1000,0,500);
    TH1D BGOhitsY("BGOhitsY","BGO hits Y",1000,0,500);
    TH1D BGOhitsZ("BGOhitsZ","BGO hits Z",1000,0,500);

    TH1D recEnergy("recEnergy","BGO rec Energy",1000,0,10000);
    TH1D hitsEnergy("hitsEnergy","BGO hits Energy",1000,0,10000);

    // Event LOOP
    for(int entry=0; entry<totEvents; ++entry)
    {
        dataTree->GetEntry(entry);
        pctgEvt = ((Double_t)(entry+1)/totEvents)*100;
        pBar(pctgEvt);

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

        trajectoryBGOX.Fill(x);
        trajectoryBGOY.Fill(y);
        trajectoryBGOZ.Fill(z);

        BGOslopeXZ.Fill(incl_x);
        BGOslopeYZ.Fill(incl_y);

        recEnergy.Fill(bgorec->GetTotalEnergy());

        // Loop over the BGO (PSD) hits
        for(int i=0; i<bgohits->fEnergy.size(); ++i)
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
            
                hitsEnergy.Fill(bgohits->fEnergy[i]);

                BGOhitsX.Fill(bgohits->GetHitX(i));
                BGOhitsY.Fill(bgohits->GetHitY(i));
                BGOhitsZ.Fill(bgohits->GetHitZ(i));
            }
        } 
    }

    //// Closing data file

    dataFile->Close();

    //// Opening input file

    TString outPathFile(outPrefix);
    outPathFile += "/BGOAnalysis.root";

    TFile outFile(outPathFile.Data(),"RECREATE");
    if(outFile.IsZombie())
    {
        std::cerr << "\n\nError writing output file in BGO analysis. outPath is " << outPathFile << "\n\n";
        exit(100);
    }

    //// Writing to file

    trajectoryBGOX.Write();
    trajectoryBGOY.Write();
    trajectoryBGOZ.Write();

    BGOslopeXZ.Write();
    BGOslopeYZ.Write();

    BGOhitsX.Write();
    BGOhitsY.Write();
    BGOhitsZ.Write();

    recEnergy.Write();
    hitsEnergy.Write();

    //// Closing output file

    outFile.Close();

    //// Cleaning pointers

    stkMetadata->Delete();
    bgohits->Delete();
    bgorec->Delete();
    
}