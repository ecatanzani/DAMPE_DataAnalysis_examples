#include "myHeader.h"

#include "DmpStkEventMetadata.h"
#include "DmpStkSiCluster.h"
#include "DmpStkLadderAdc.hh"
#include "DmpStkTrack.h"
#include "DmpEvtHeader.h"
#include "DmpStkHkeepHeader.h"

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

   // Register STK tracks collection
   TClonesArray* stktracks = new TClonesArray("DmpStkTrack", 200);
   if(fStkKalmanTracksFound)
      dataTree->SetBranchAddress("StkKalmanTracks",&stktracks);


   // Register STK Single-TRB collections (FM and EQM ladders)
   TClonesArray* stkclusters_stktrb = new TClonesArray("DmpStkSiCluster");
   dataTree->SetBranchAddress("StkClusterCollection_STKTRB",&stkclusters_stktrb);   

   // Register DAMPE event header (event metadata information)
   DmpEvtHeader* evtheader = new DmpEvtHeader();
   dataTree->SetBranchAddress("EventHeader",&evtheader);


   // Event LOOP
   for(int entry=0; entry<dataTree->GetEntries(); entry++)
   {
        dataTree->GetEntry(entry);
      
        // STK metadata
        printf("STK data mode = %d\n", stkMetadata->fRunMode );   
      
        // Loop over STK clusters in event
        for(int i=0; i<stkclusters->GetLast()+1; i++)
        {
            DmpStkSiCluster* cluster = (DmpStkSiCluster*)stkclusters->ConstructedAt(i);
            printf("\nSTK cluster info:\n");
            printf("   total ADC counts = %f\n",cluster->getEnergy());
            printf("   number of strips = %d\n",cluster->getNstrip());         
            printf("   signal in the first strip = %f\n", cluster->GetSignal(0));
            printf("   signal in the last  strip = %f\n", cluster->GetSignal(cluster->getNstrip()-1));
            printf("   noise  in the first strip = %f\n", cluster->GetNoise(0));
            printf("   noise  in the last  strip = %f\n", cluster->GetNoise(cluster->getNstrip()-1));
            printf("   center of gravity = %f\n",cluster->getLadderCentroid());
            printf("   ladder ID (hardware) = %d\n",cluster->getLadderHardware());
        }
      
        // Loop over STK adc 
        for(int i=0; i<stkladderadc->GetLast()+1; i++)
        {
            DmpStkLadderAdc* laddderadc = (DmpStkLadderAdc*)stkladderadc->ConstructedAt(i);
            int ladderid = laddderadc->GetLadderID();
            for(int j=0; j<384; j++)
                double adc = laddderadc->GetChannelAdc(j); 
        }
   
        // Loop over STK tracks (if present)
        if(fStkKalmanTracksFound)
        {
            for(int i=0; i<stktracks->GetLast()+1; i++)
            {
                DmpStkTrack* track = (DmpStkTrack* ) stktracks-> ConstructedAt(i);
                double track_x = track->getImpactPoint().x();
                double track_y = track->getImpactPoint().y();
                double track_z = track->getImpactPoint().z();
                double track_incl_x = track->getTrackParams().getSlopeX();
                double track_incl_y = track->getTrackParams().getSlopeY();

                // GET CLUSTERS FOR THE TRACK
                int ntrackpoints = track->GetNPoints();
                for(int p=0; p<ntrackpoints; p++)
                {
                    DmpStkSiCluster* clx = track->GetClusterX(p,stkclusters);
                    DmpStkSiCluster* cly = track->GetClusterY(p,stkclusters);

                    if(clx) 
                        printf("Cluster x exists for the point \n");
                    else 
                        printf("No x cluter assigned to the track point in plane \n");
                    if(cly) 
                        printf("Cluster y exists for the point \n");
                    else 
                        printf("No y cluter assigned to the track point in plane \n");
                }
            }
        }

        // Get event time stamp
        //
        //   NOTE: due to the absence of information on the time zone where the file was recorded,
        //                the returned value of the time stamp can be one-hour different w.r.t. 
        //                original time stamp. Please cross-check the results of this funcntion 
        //                with the relevant run log file  
        //
      
        int sec = evtheader->GetSecond();                                        
        string timestamp = DmpStkHkeepHeader::TCtoString(sec);   
        std::cout<<"Human readable time stamp: "<<timestamp<<std::endl;
        int msec = evtheader->GetMillisecond(); 
        int timestamp_msec = sec*1000 + msec;

        //
        // Check STK trigger (internal VS external)
        //
        if(evtheader->GeneratedPeriodTrigger()) 
            std::cout<<"STK trigger is internal! (Noise run)"<<std::endl;
        else     
            std::cout<<"STK trigger is NOT internal! (Cosmics run)"<<std::endl;
        // 
        //  .... the following is equivalent to the previous one
        if(evtheader->EnabledPeriodTrigger())    
            std::cout<<"STK trigger is internal! (Noise run)"<<std::endl;
        else   
            std::cout<<"STK trigger is NOT internal! (Cosmics run)"<<std::endl;
        // 
        //  .... the following is equivalent to the previous one
        if(evtheader->GeneratedTrigger(0))  
            std::cout<<"STK trigger is NOT internal! (Cosmics run)"<<std::endl;      
        else     
            std::cout<<"STK trigger is internal! (Noise run)"<<std::endl;      


        // Loop over STK single-trb clusters in event
        //
        //    WARNING:  The single-TRB data contains information of all 24-ladders, while only up to 6-real ladders (out of 24) are connected
        //                        Usually, only clusters that have   cluster->getLadderHardware() = 26 , 27 , 30, 31,   34, 35  come from the real ladders
        //
        //                        Please don't use GetX, GetY, GetZ methods for these clusters, the results will be wrong, since x,y,z are calculated 
        //                        assuming the STK ladders are integrated into DAMPE, while it's not the case for the considered single TRB  
        //
        for(int i=0; i<stkclusters_stktrb->GetLast()+1; i++)
        {
            DmpStkSiCluster* cluster = (DmpStkSiCluster*)stkclusters_stktrb->ConstructedAt(i);
            printf("\nSTK cluster info:\n");
            printf("   total ADC counts = %f\n",cluster->getEnergy());
            printf("   number of strips = %d\n",cluster->getNstrip());         
            printf("   signal in the first strip = %f\n", cluster->GetSignal(0));
            printf("   signal in the last  strip = %f\n", cluster->GetSignal(cluster->getNstrip()-1));
            printf("   noise  in the first strip = %f\n", cluster->GetNoise(0));
            printf("   noise  in the last  strip = %f\n", cluster->GetNoise(cluster->getNstrip()-1));
            printf("   center of gravity = %f\n",cluster->getLadderCentroid());
            printf("   ladder ID (hardware) = %d\n",cluster->getLadderHardware());
            //etc.
            // For more details on STK clusters see http://dpnc.unige.ch/SVNDAMPE/DAMPE/DmpSoftware/trunk/Event/Stk/include/DmpStkSiCluster.h
      }

   }
   
   
}