#include "myHeader.h"

int main(int argc,char* argv[])
{
    std::string exmplName;

    if(argc<2)
    {
        std::cout << "\n\nUsage: ./exmplAnalysis data_file_name example_name example_pars" << std::endl << std::endl;
        exit(100);
    }
    else
        exmlpName = argv[2];

    if(strcmp(exmlpName,"getTracks"))
        getTracks(argv[1]);
    
           

    return 0;

}