# DAMPE_DataAnalysis_examples
Examples of data taking procedures with DAMPE's framework

Here the usase syntax:
    
    Usage: ./exmplAnalysis (path_to_ext_config_file) path_to_data_file prefix_out_file

Configuration file:

The config file is automatically detected by the software (and so is an optional parameter to provide) but could be also passed the path of a personalized one (the existing syntax should however be respected). 

The configuraton file allows to turn on/off the verbosity of the data taking software, together with the list of the different types of data there are going to be acquired. Up to now just the data of the STK, BGO and ancillary detectors will be read from the raw .root file. Setting "True" to each one of the detector module all the correpsonding data will be taken consequently. Each desidered combination of these analysis could be performed.

Here an example of the config file:

    ################ Data Analysis configuration file ################

    ##### Software parameters

    verbose False

    ##### Analysis functions

    trackerAnalysis True
    BGOAnalysis True
    ancillaryAnalysis True

    ##### Analysis parameters

    inputDataScalingFactor   1

In this example fo cinfig file the verbosity has been turned OFF and all the analysis (STK, BGO and Ancillaty detectors) will be performed. 

"Analysis parameters" is a particular section containg some parameters useful during the data analysis for some of the detectors. Up to now this section just contains a variable named "inputDataScalingFactor" used to scale the number fo events of a certain input data file; setting its value to 1 (default value) all the data into the raw .root file will be used, otherwise just the first total_input_data/inputDataScalingFactor will be considered. This variable us useful in case fo tests on an input dataset, using just a selection of the data.

path_to_data_file:

That's the absolute path of the input .root data file

prefix_out_file:

Prefix  of the output files, containing just a simple statistics (few histos) of the acquired data.

##### Use release executable for better performances

To improve performances use the "Release" executable.

##### Requirements

DAMPESW chould be present in the machine.
C++11 shoule be present in the machine, to comile this code (C++ version could be changed modifying the compiler flasg into the makefile)
