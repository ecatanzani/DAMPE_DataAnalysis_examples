#include "myHeader.h"

void readConfigFile(
                        const std::string configFile,
                        bool &verbosity,
                        std::vector<bool> &analysis
                   )
{
    std::ifstream input_file(configFile.c_str());
    if(!input_file.is_open()) 
    {
        std::cerr << "\nERROR 100! File not open. The path is:\n" << configFile << "\n\n";
        exit(100);
    }
    std::string input_string((std::istreambuf_iterator< char >(input_file)), (std::istreambuf_iterator< char >()));
    std::string tmp_str;
    input_file.close();
    std::istringstream input_stream(input_string);
    while(input_stream>>tmp_str)
    {
        if(strcmp(tmp_str.c_str(),"verbose"))
        {
            input_stream>>tmp_str;
            verbosity = tmp_str == "True" ? true : false;
        }
        if(strcmp(tmp_str.c_str(),"trackerAnalysis"))
        {
            input_stream>>tmp_str;
            analysis[0] = tmp_str == "True" ? true : false;
        }        
        if(strcmp(tmp_str.c_str(),"BGOAnalysis"))
        {
            input_stream>>tmp_str;
            analysis[1] = tmp_str == "True" ? true : false;
        }
    }
}