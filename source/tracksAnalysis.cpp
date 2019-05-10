#include "myHeader.h"

#include "DmpStkEventMetadata.h"
#include "DmpStkSiCluster.h"
#include "DmpStkLadderAdc.hh"
#include "DmpStkTrack.h"
#include "DmpEvtHeader.h"
#include "DmpStkHkeepHeader.h"

void getTracks(
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

    //// Histos 

    TH1D totalADCcounts("totalADCcounts","total ADC counts",1000,0,100);
    TH1D clusterStrip("clusterStrip","cluster Strip",100,0,10);
    TH1D fStripSignal("fStripSignal","first Strip Signal",1000,0,100);
    TH1D lStripSignal("lStripSignal","last Strip Signal",1000,0,100);
    TH1D noisefStripSignal("noisefStripSignal","noise first Strip Signal",1000,0,100);
    TH1D noiselStripSignal("noiselStripSignal","noise last Strip Signal",1000,0,100);
    TH1D cog("cog","Center of Gravity",1000,0,800);
    TH1D ladderID("ladderID","ladder ID",1000,0,1000);

    //// Single TRB

    TH1D sTRBtotalADCcounts("sTRBtotalADCcounts","sTRB total ADC counts",1000,0,100);
    TH1D sTRBclusterStrip("sTRBclusterStrip","sTRB cluster Strip",100,0,10);
    TH1D sTRBfStripSignal("sTRBfStripSignal","sTRB first Strip Signal",1000,0,100);
    TH1D sTRBlStripSignal("sTRBlStripSignal","sTRB last Strip Signal",1000,0,100);
    TH1D sTRBnoisefStripSignal("sTRBnoisefStripSignal","sTRB noise first Strip Signal",1000,0,100);
    TH1D sTRBnoiselStripSignal("sTRBnoiselStripSignal","sTRB noise last Strip Signal",1000,0,100);
    TH1D sTRBcog("sTRBcog","sTRB Center of Gravity",1000,0,800);
    TH1D sTRBladderID("sTRBladderID","sTRB ladder ID",1000,0,1000);

    //// Tracks

    TH1D trackImpactPointX("trackImpactPointX","track Impact Point X",1000,-100,100);
    TH1D trackImpactPointY("trackImpactPointY","track Impact Point Y",1000,-100,100);
    TH1D trackImpactPointZ("trackImpactPointZ","track Impact Point Z",1000,0,1000);

    TH1D slopeX("slopeX","slope X",1000,0,100);
    TH1D slopeY("slopeY","slope Y",1000,0,100);

    TH1D trackNpoints("trackNpoints","track N points",100,0,10);

    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////

    // Event LOOP
    for(int entry=0; entry<totEvents; ++entry)
    {
        dataTree->GetEntry(entry);
        pctgEvt = ((Double_t)(entry+1)/totEvents)*100;
        pBar(pctgEvt);

        // STK metadata
        if(verbosity)
            printf("\nSTK data mode = %d\n", stkMetadata->fRunMode );   
        
        // Loop over STK clusters in event
        for(int i=0; i<stkclusters->GetLast()+1; ++i)
        {
            DmpStkSiCluster* cluster = (DmpStkSiCluster*)stkclusters->ConstructedAt(i);
            if(verbosity)
            {
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

            totalADCcounts.Fill(cluster->getEnergy());
            clusterStrip.Fill(cluster->getNstrip());
            fStripSignal.Fill(cluster->GetSignal(0));
            lStripSignal.Fill(cluster->GetSignal(cluster->getNstrip()-1));
            noisefStripSignal.Fill(cluster->GetNoise(0));
            noiselStripSignal.Fill(cluster->GetNoise(cluster->getNstrip()-1));
            cog.Fill(cluster->getLadderCentroid());
            ladderID.Fill(cluster->getLadderHardware());

        }
        
        // Loop over STK adc 
        for(int idx = 0; idx < stkladderadc->GetLast()+1; ++idx)
        {   
            DmpStkLadderAdc* laddderadc = (DmpStkLadderAdc*)stkladderadc->ConstructedAt(idx);
            int ladderid = laddderadc->GetLadderID();
            double adc = -999;
            for(int j=0; j<384; j++)
            {
                adc = laddderadc->GetChannelAdc(j);
                if(verbosity)
                    std::cout << "\nladder ID: " << ladderid << "ADC channel " << idx << adc; 
            }
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

                    trackImpactPointX.Fill(track_x);
                    trackImpactPointY.Fill(track_y);
                    trackImpactPointZ.Fill(track_z);

                    slopeX.Fill(track_incl_x);
                    slopeY.Fill(track_incl_y);

                    // GET CLUSTERS FOR THE TRACK
                    int ntrackpoints = track->GetNPoints();

                    trackNpoints.Fill(ntrackpoints);

                    for(int p=0; p<ntrackpoints; p++)
                    {
                        DmpStkSiCluster* clx = track->GetClusterX(p,stkclusters);
                        DmpStkSiCluster* cly = track->GetClusterY(p,stkclusters);

                        if(clx)
                            if(verbosity) 
                                printf("Cluster x exists for the point \n");
                        else
                            if(verbosity) 
                                printf("No x cluter assigned to the track point in plane \n");
                        if(cly)
                            if(verbosity) 
                                printf("Cluster y exists for the point \n");
                        else
                            if(verbosity) 
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
            if(verbosity)
                std::cout << "Human readable time stamp: "<< timestamp << std::endl;
            int msec = evtheader->GetMillisecond(); 
            int timestamp_msec = sec*1000 + msec;

            //
            // Check STK trigger (internal VS external)
            //
            if(evtheader->GeneratedPeriodTrigger())
                if(verbosity) 
                    std::cout<<"STK trigger is internal! (Noise run)"<<std::endl;
            else
                if(verbosity)     
                    std::cout<<"STK trigger is NOT internal! (Cosmics run)"<<std::endl;
            // 
            //  .... the following is equivalent to the previous one
            if(evtheader->EnabledPeriodTrigger())    
                if(verbosity)
                    std::cout<<"STK trigger is internal! (Noise run)"<<std::endl;
            else   
                if(verbosity)
                    std::cout<<"STK trigger is NOT internal! (Cosmics run)"<<std::endl;


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
                if(verbosity)
                {
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

                sTRBtotalADCcounts.Fill(cluster->getEnergy());
                sTRBclusterStrip.Fill(cluster->getNstrip());
                sTRBfStripSignal.Fill(cluster->GetSignal(0));
                sTRBlStripSignal.Fill(cluster->GetSignal(cluster->getNstrip()-1));
                sTRBnoisefStripSignal.Fill(cluster->GetNoise(0));
                sTRBnoiselStripSignal.Fill(cluster->GetNoise(cluster->getNstrip()-1));
                sTRBcog.Fill(cluster->getLadderCentroid());
                sTRBladderID.Fill(cluster->getLadderHardware());
            }

        }
   
    //// Closing data file

    dataFile->Close();

    //// Opening input file

    TString outPathFile(outPrefix);
    outPathFile += "/STKAnalysis.root";

    TFile outFile(outPathFile.Data(),"RECREATE");
    if(outFile.IsZombie())
    {
        std::cerr << "\n\nError writing output file in STK analysis. outPath is " << outPathFile << "\n\n";
        exit(100);
    }

    //// Writing to file

    totalADCcounts.Write();
    clusterStrip.Write();
    fStripSignal.Write();
    lStripSignal.Write();
    noisefStripSignal.Write();
    noiselStripSignal.Write();
    cog.Write();
    ladderID.Write();

    sTRBtotalADCcounts.Write();
    sTRBclusterStrip.Write();
    sTRBfStripSignal.Write();
    sTRBlStripSignal.Write();
    sTRBnoisefStripSignal.Write();
    sTRBnoiselStripSignal.Write();
    sTRBcog.Write();
    sTRBladderID.Write();

    trackImpactPointX.Write();
    trackImpactPointY.Write();
    trackImpactPointZ.Write();

    slopeX.Write();
    slopeY.Write();

    trackNpoints.Write();

    //// Cleaning pointers

    stkclusters->Delete();
    stkMetadata->Delete();
    stkladderadc->Delete();
    stktracks->Delete(); 
    stkclusters_stktrb->Delete();
    evtheader->Delete();

}