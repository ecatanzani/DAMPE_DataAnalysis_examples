#include "myHeader.h"

int main(int argc,char* argv[])
{
    std::string dataFile;
    std::string outPrefix;
    std::string configFile = "config.txt";
    ULong64_t sDFactor = 1;

    std::vector<bool> analysis;

    /*
        Analysis vector

        [0] "True" --> STK analysis
        [1] "True" --> BGO analysis
        [2] "True" --> Ancillary analysis

    */


    ////// Data Analysis Software Parameters

    bool verbosity = false;

    ////// 

    if(argc<3)
    {
        std::cout << "\n\nUsage: ./exmplAnalysis (path_to_ext_config_file) path_to_data_file prefix_out_file" << std::endl << std::endl;
        exit(100);
    }
    else if(argc==3)
    {
        dataFile = argv[1];
        outPrefix = argv[2];
    }
    else
    {
        configFile = argv[1];
        dataFile = argv[2];
        outPrefix = argv[3];
    }

    readConfigFile(
                    configFile,
                    verbosity,
                    analysis,
                    sDFactor
                  );

    if(verbosity)
    {   
        std::cout << "\n ************************ Data Analysis params ************************\n\n";
        std::cout << "Selected data file: " << dataFile << std::endl;
        std::cout << "Selected config file: " << configFile;
        std:: cout << "\n\n *************************************************************************** \n\n";
    }
    if(sDFactor!=1)
        std::cout << "\nData scaling factor applied: " << sDFactor;

    for(int aIdx=0; aIdx<analysis.size(); ++aIdx)
    {
        if(analysis.at(aIdx) == true && aIdx == 0)
        {
            std::cout << "\n\n ***** STK Analysis ***** \n\n";
            getTracks(
                        dataFile,
                        outPrefix,
                        verbosity,
                        sDFactor
                    );
        }
        else if(analysis.at(aIdx) == true && aIdx == 1)
        {
            std::cout << "\n\n ***** BGO Analysis ***** \n\n";
            getBGOdata(
                        dataFile,
                        outPrefix,
                        verbosity,
                        sDFactor
                    );
        }
        else if(analysis.at(aIdx) == true && aIdx == 2)
        {
            std::cout << "\n\n ***** ancillary Analysis ***** \n\n";
            getAncillarydata(
                                dataFile,
                                outPrefix,
                                verbosity,
                                sDFactor
                            );
        }
    }

    return 0;

}