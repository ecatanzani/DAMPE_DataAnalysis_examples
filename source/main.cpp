#include "myHeader.h"

int main(int argc,char* argv[])
{
    std::string dataFile;
    std::string configFile = "config.txt";
    std::vector<bool> analysis;
    analysis.resize(2);

    ////// Data Analysis Software Parameters

    bool verbosity = false;

    ////// 

    if(argc<2)
    {
        std::cout << "\n\nUsage: ./exmplAnalysis (path_to_ext_config_file) path_to_data_file" << std::endl << std::endl;
        exit(100);
    }
    else if(argc==2)
        dataFile = argv[1];
    else
    {
        configFile = argv[1];
        dataFile = argv[2];
    }

    readConfigFile(
                    configFile,
                    verbosity,
                    analysis
                  );

    if(verbosity)
    {   
        std::cout << "\n ************************ Data Analysis params ************************\n\n";
        std::cout << "Selected data file: " << dataFile << std::endl;
        std::cout << "Selected config file: " << configFile;
        std:: cout << "\n\n *************************************************************************** \n\n";
    }

    if(analysis[0])
        getTracks(dataFile,verbosity);

    return 0;

}