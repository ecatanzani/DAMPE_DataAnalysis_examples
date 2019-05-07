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
        dataFile = argv[2];
    else
    {
        configFile = argv[2];
        dataFile = argv[3];
    }
    
    readConfigFile(
                    configFile,
                    verbosity,
                    analysis
                  );

    return 0;

}