#include <fenv.h>
#include "Madingley.h"
#include "FileReader.h"
#include "Date.h"
#include "LoadCSVEnvironment.h"
#include "InputParameters.h"

std::string CPP_VERSION = "2.01";

// flags
int NumberFlags;
std::string TypeOfRun;
int NumberOfSimulationYears;
int Xmin, Xmax, Ymin, Ymax;
std::string OutputFolderName = "none";
int TimestepWritingBinnedCohortStatistics; // in months
int TimestepWritingFullCohortProperties; // in months
int TimestepWritingPreyBinnedFoodwebConnections; // slows down code, tracking in EatingCarnivore.cpp
int TimestepWritingStockProperties; // in months
int WriteGridProperties; //(yes=1/no=0)
int MaxCohortNumber; //(yes=1/no=0)
std::string ConfigurationDirectory; // input dir, keep?
int RestartedFromTimeStepMonth;
std::string SpatialInputLocation;
int UseNonDefaultModelParameters;

// old flags
bool OverwriteApplySpinUp = false;
std::string CohortCSVLocation;
std::string StockCSVLocation;
std::string PoolOrgCSV;
std::string PoolCO2CSV;

// grid env vectors[cellIndex], loaded csv data
std::vector<double> eGV_awc;
std::vector<double> eGV_land_mask;
std::vector<double> eGV_hanpp;
std::vector<double> eGV_realm_class;
std::vector<double> eGV_Ecto_max; // Ecto_max
std::vector<double> eGV_Endo_C_max; // Endo_C_max
std::vector<double> eGV_Endo_H_max; // Endo_H_max
std::vector<double> eGV_Endo_O_max; // Endo_O_max
int eGV_NumberOfGridCells;

// 3d env grid vectors[timeIndex][cellIndex], loaded csv data 
std::vector <std::vector<double>> eGV_dtr; // diurnal_temperature_range
std::vector <std::vector<double>> eGV_gff; // ground_frost_frequency
std::vector <std::vector<double>> eGV_nst; // near-surface_temperature
std::vector <std::vector<double>> eGV_prec; // precipitation
std::vector <std::vector<double>> eGV_tnpp; // terrestrial_net_primary_productivity

//!!!! start of food web calculations
int TimeStep_Months_Calc_FoodWeb; // slows down code, tracking in EatingCarnivore.cpp

//################################## Get model input parameters
std::vector<double> Activity_Parameters(4,0.0);
std::vector<double> Dispersal_Parameters(6,0.0);
std::vector<double> EatingCarnivory_Parameters(9,0.0);
std::vector<double> EatingHerbivory_Parameters(9,0.0);
std::vector<double> MetabolismEctotherm_Parameters(7,0.0);
std::vector<double> MetabolismEndotherm_Parameters(6,0.0);
std::vector<double> MetabolismHeterotroph_Parameters(3,0.0);
std::vector<double> Mortality_Parameters(5,0.0);
std::vector<double> Reproduction_Parameters(4,0.0);
std::vector<double> VegetationModel_Parameters(33,0.0);
//################################## Get model input parameters

int main( int argc, char* argv[] ) {

    // Write out model details to the console
	ConfigurationDirectory = "./input/Model setup/";

    //for(int i = 0; i < 19; i++) std::cout << argv[i] <<  std::endl;
    InputParameters::Get( )->SetTestParam( "drolleke!" );

    // Get model flags
    NumberFlags = argc;

    TypeOfRun = argv[1];
    if(TypeOfRun=="version"){
        std::cout << CPP_VERSION << std::endl;
        exit(0);
    }
    InputParameters::Get( )->SetTypeOfRun( TypeOfRun );
    
    NumberOfSimulationYears = std::stoi(argv[2]);
    InputParameters::Get( )->SetNumberOfSimulationYears( NumberOfSimulationYears );

    Xmin = std::stoi(argv[3]); 
    InputParameters::Get( )->SetXmin( Xmin );
    Xmax = std::stoi(argv[4]);
    InputParameters::Get( )->SetXmax( Xmax );
    Ymin = std::stoi(argv[5]);
    InputParameters::Get( )->SetYmin( Ymin );
    Ymax = std::stoi(argv[6]);
    InputParameters::Get( )->SetYmax( Ymax );

	//PATH
    OutputFolderName = argv[7];
	OutputFolderName.erase(remove( OutputFolderName.begin(), OutputFolderName.end(), '\"' ),OutputFolderName.end());
    InputParameters::Get( )->SetOutputFolderName( OutputFolderName );

    TimestepWritingBinnedCohortStatistics = std::stoi(argv[8]); 
    InputParameters::Get( )->SetTimestepWritingBinnedCohortStatistics( TimestepWritingBinnedCohortStatistics );

    TimestepWritingFullCohortProperties = std::stoi(argv[9]); 
    InputParameters::Get( )->SetTimestepWritingFullCohortProperties( TimestepWritingFullCohortProperties );

    TimestepWritingPreyBinnedFoodwebConnections = std::stoi(argv[10]);  // slows down code, tracking in EatingCarnivore.cpp 
    InputParameters::Get( )->SetTimestepWritingPreyBinnedFoodwebConnections( TimestepWritingPreyBinnedFoodwebConnections );

    TimestepWritingStockProperties = std::stoi(argv[11]); 
    InputParameters::Get( )->SetTimestepWritingStockProperties( TimestepWritingStockProperties );

    WriteGridProperties = std::stoi(argv[12]); //(yes=1/no=0)
    InputParameters::Get( )->SetWriteGridProperties( WriteGridProperties );

	//PATH
    ConfigurationDirectory = argv[13]; // input dir, keep?
	ConfigurationDirectory.erase(remove( ConfigurationDirectory.begin(), ConfigurationDirectory.end(), '\"' ),ConfigurationDirectory.end());
    InputParameters::Get( )->SetConfigurationDirectory( ConfigurationDirectory );
    
    MaxCohortNumber = std::stoi(argv[14]); 
    InputParameters::Get( )->SetMaxCohortNumber( MaxCohortNumber );

	//PATH
    CohortCSVLocation = argv[15];
	CohortCSVLocation.erase(remove( CohortCSVLocation.begin(), CohortCSVLocation.end(), '\"' ),CohortCSVLocation.end());
    InputParameters::Get( )->SetCohortCSVLocation( CohortCSVLocation );

	//PATH
    StockCSVLocation = argv[16];
	StockCSVLocation.erase(remove( StockCSVLocation.begin(), StockCSVLocation.end(), '\"' ),StockCSVLocation.end());
	StockCSVLocation.substr(1, StockCSVLocation.size() - 2);
    InputParameters::Get( )->SetStockCSVLocation( StockCSVLocation );

    RestartedFromTimeStepMonth = std::stoi(argv[17]);
    InputParameters::Get( )->SetRestartedFromTimeStepMonth( RestartedFromTimeStepMonth );

	//PATH
    SpatialInputLocation = argv[18];
	SpatialInputLocation.erase(remove( SpatialInputLocation.begin(), SpatialInputLocation.end(), '\"' ),SpatialInputLocation.end());
	//std::cout << "q removed: "<< SpatialInputLocation << std::endl;
    InputParameters::Get( )->SetSpatialInputLocation( SpatialInputLocation );

    TimeStep_Months_Calc_FoodWeb = TimestepWritingPreyBinnedFoodwebConnections; // slows down code, tracking in EatingCarnivore.cpp
    InputParameters::Get( )->SetTimeStep_Months_Calc_FoodWeb( TimeStep_Months_Calc_FoodWeb );

	double GridcellSize = std::stod(argv[19]);
	InputParameters::Get( )->SetGridcellSize( GridcellSize );

	double NoiseRandomizationCohortOrder = std::stod(argv[20]);
	InputParameters::Get( )->SetRandomCohortOrderNoiseThreshold( NoiseRandomizationCohortOrder );
    if(NoiseRandomizationCohortOrder>0.0) std::cout << "Ordering cohorts from small to large using " << 1.0 - NoiseRandomizationCohortOrder <<  " randomization" << std::endl;

    int RunWithoutDispersal = std::stoi(argv[21]);
	InputParameters::Get( )->SetRunWithoutDispersal( RunWithoutDispersal );
    if(RunWithoutDispersal==1) std::cout << "Running the model without dispersal between grid cells" << std::endl;

    int RunInParallel = std::stoi(argv[22]);
	InputParameters::Get( )->SetRunInParallel( RunInParallel );
    if(RunInParallel!=1) std::cout << "Parallel computation switched off" << std::endl;

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if(TypeOfRun=="run") OverwriteApplySpinUp=true;

    UseNonDefaultModelParameters = std::stoi(argv[23]);
    InputParameters::Get( )->SetUseNonDefaultModelParameters( UseNonDefaultModelParameters );

    //this line enables the gdb debugger to catch Nan or floating point problems
    feraiseexcept( FE_INVALID | FE_OVERFLOW );
   

    //################################## Get model specific parameters
    if(UseNonDefaultModelParameters==1){
        std::cout << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        std::cout << "Applying user input model parameters:" << std::endl;

        int var_counter = 24;

        Activity_Parameters[0] = std::stod(argv[var_counter]); var_counter++;
        Activity_Parameters[1] = std::stod(argv[var_counter]); var_counter++;
        Activity_Parameters[2] = std::stod(argv[var_counter]); var_counter++;
        Activity_Parameters[3] = std::stod(argv[var_counter]); var_counter++;
        std::cout << "Activity_Parameters ";
        for(auto const& value: Activity_Parameters) std::cout << value << " ";
        
        std::cout << std::endl;
        
        Dispersal_Parameters[0] = std::stod(argv[var_counter]); var_counter++;
        Dispersal_Parameters[1] = std::stod(argv[var_counter]); var_counter++;
        Dispersal_Parameters[2] = std::stod(argv[var_counter]); var_counter++;
        Dispersal_Parameters[3] = std::stod(argv[var_counter]); var_counter++;
        Dispersal_Parameters[4] = std::stod(argv[var_counter]); var_counter++;
        Dispersal_Parameters[5] = std::stod(argv[var_counter]); var_counter++;
        std::cout << "Dispersal_Parameters ";
        for(auto const& value: Dispersal_Parameters) std::cout << value << " ";

        std::cout << std::endl;

        EatingCarnivory_Parameters[0] = std::stod(argv[var_counter]); var_counter++;
        EatingCarnivory_Parameters[1] = std::stod(argv[var_counter]); var_counter++;
        EatingCarnivory_Parameters[2] = std::stod(argv[var_counter]); var_counter++;
        EatingCarnivory_Parameters[3] = std::stod(argv[var_counter]); var_counter++;
        EatingCarnivory_Parameters[4] = std::stod(argv[var_counter]); var_counter++;
        EatingCarnivory_Parameters[5] = std::stod(argv[var_counter]); var_counter++;
        EatingCarnivory_Parameters[6] = std::stod(argv[var_counter]); var_counter++;
        EatingCarnivory_Parameters[7] = std::stod(argv[var_counter]); var_counter++;
        EatingCarnivory_Parameters[8] = std::stod(argv[var_counter]); var_counter++;
        std::cout << "EatingCarnivory_Parameters ";
        for(auto const& value: EatingCarnivory_Parameters) std::cout << value << " ";

        std::cout << std::endl;

        EatingHerbivory_Parameters[0] = std::stod(argv[var_counter]); var_counter++;
        EatingHerbivory_Parameters[1] = std::stod(argv[var_counter]); var_counter++;
        EatingHerbivory_Parameters[2] = std::stod(argv[var_counter]); var_counter++;
        EatingHerbivory_Parameters[3] = std::stod(argv[var_counter]); var_counter++;
        EatingHerbivory_Parameters[4] = std::stod(argv[var_counter]); var_counter++;
        EatingHerbivory_Parameters[5] = std::stod(argv[var_counter]); var_counter++;
        EatingHerbivory_Parameters[6] = std::stod(argv[var_counter]); var_counter++;
        EatingHerbivory_Parameters[7] = std::stod(argv[var_counter]); var_counter++;
        EatingHerbivory_Parameters[8] = std::stod(argv[var_counter]); var_counter++;
        std::cout << "EatingHerbivory_Parameters ";
        for(auto const& value: EatingHerbivory_Parameters) std::cout << value << " ";

        std::cout << std::endl;

        MetabolismEctotherm_Parameters[0] = std::stod(argv[var_counter]); var_counter++;
        MetabolismEctotherm_Parameters[1] = std::stod(argv[var_counter]); var_counter++;
        MetabolismEctotherm_Parameters[2] = std::stod(argv[var_counter]); var_counter++;
        MetabolismEctotherm_Parameters[3] = std::stod(argv[var_counter]); var_counter++;
        MetabolismEctotherm_Parameters[4] = std::stod(argv[var_counter]); var_counter++;
        MetabolismEctotherm_Parameters[5] = std::stod(argv[var_counter]); var_counter++;
        MetabolismEctotherm_Parameters[6] = std::stod(argv[var_counter]); var_counter++;
        std::cout << "MetabolismEctotherm_Parameters ";
        for(auto const& value: MetabolismEctotherm_Parameters) std::cout << value << " ";

        std::cout << std::endl;

        MetabolismEndotherm_Parameters[0] = std::stod(argv[var_counter]); var_counter++;
        MetabolismEndotherm_Parameters[1] = std::stod(argv[var_counter]); var_counter++;
        MetabolismEndotherm_Parameters[2] = std::stod(argv[var_counter]); var_counter++;
        MetabolismEndotherm_Parameters[3] = std::stod(argv[var_counter]); var_counter++;
        MetabolismEndotherm_Parameters[4] = std::stod(argv[var_counter]); var_counter++;
        MetabolismEndotherm_Parameters[5] = std::stod(argv[var_counter]); var_counter++;
        std::cout << "MetabolismEndotherm_Parameters ";
        for(auto const& value: MetabolismEndotherm_Parameters) std::cout << value << " ";

        std::cout << std::endl;

        MetabolismHeterotroph_Parameters[0] = std::stod(argv[var_counter]); var_counter++;
        MetabolismHeterotroph_Parameters[1] = std::stod(argv[var_counter]); var_counter++;
        MetabolismHeterotroph_Parameters[2] = std::stod(argv[var_counter]); var_counter++;
        std::cout << "MetabolismHeterotroph_Parameters ";
        for(auto const& value: MetabolismHeterotroph_Parameters) std::cout << value << " ";

        std::cout << std::endl;

        Mortality_Parameters[0] = std::stod(argv[var_counter]); var_counter++;
        Mortality_Parameters[1] = std::stod(argv[var_counter]); var_counter++;
        Mortality_Parameters[2] = std::stod(argv[var_counter]); var_counter++;
        Mortality_Parameters[3] = std::stod(argv[var_counter]); var_counter++;
        Mortality_Parameters[4] = std::stod(argv[var_counter]); var_counter++;
        std::cout << "Mortality_Parameters ";
        for(auto const& value: Mortality_Parameters) std::cout << value << " ";

        std::cout << std::endl;

        Reproduction_Parameters[0] = std::stod(argv[var_counter]); var_counter++;
        Reproduction_Parameters[1] = std::stod(argv[var_counter]); var_counter++;
        Reproduction_Parameters[2] = std::stod(argv[var_counter]); var_counter++;
        Reproduction_Parameters[3] = std::stod(argv[var_counter]); var_counter++;
        std::cout << "Reproduction_Parameters ";
        for(auto const& value: Reproduction_Parameters) std::cout << value << " ";

        std::cout << std::endl;
        
        for(int i = 0; i<VegetationModel_Parameters.size(); i++) VegetationModel_Parameters[i] = std::stod(argv[var_counter+i]);
        std::cout << "VegetationModel_Parameters ";
        for(auto const& value: VegetationModel_Parameters) std::cout << value << " ";

        InputParameters::Get( )->SetSpatialLayers_awc(  Activity_Parameters,
                                                        Dispersal_Parameters,
                                                        EatingCarnivory_Parameters,
                                                        EatingHerbivory_Parameters,
                                                        MetabolismEctotherm_Parameters,
                                                        MetabolismEndotherm_Parameters,
                                                        MetabolismHeterotroph_Parameters,
                                                        Mortality_Parameters,
                                                        Reproduction_Parameters,
                                                        VegetationModel_Parameters );
            
        std::cout << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        std::cout << std::endl;
    }else{
        if(TypeOfRun=="run") {
            std::cout << "Using default model parameters" << std::endl;
        }
    }
    //################################## Get model specific parameters

    // Declare and start a timer
    Stopwatch s;
    s.Start( );
    
    //////////////////////////////////////////////////////////////////////
    //# READ SPATIAL INPUTS, load model environment using csvs not netcdfs
    //////////////////////////////////////////////////////////////////////
    LoadCSVEnvironment LNV;
    std::string csv_dir = SpatialInputLocation; // "../MadingleyData-master/CSV/1deg/";

    // load spatial (climate) input data
    int print_env_vectors = 0; // print file content loaded in environment (prints content in table format)
    int print2 = 0; // print which files are being loaded (prints complete file path)
    eGV_realm_class = LNV.ExtractVarGrid(LNV.Read_Env_CSV( csv_dir + "realm_classification.csv", print2 ), Xmin, Xmax, Ymin, Ymax, print_env_vectors);
    eGV_awc = LNV.ExtractVarGrid(LNV.Read_Env_CSV( csv_dir + "available_water_capacity.csv", print2 ), Xmin, Xmax, Ymin, Ymax, print_env_vectors);
    eGV_land_mask = LNV.ExtractVarGrid(LNV.Read_Env_CSV( csv_dir + "land_mask.csv", print2 ), Xmin, Xmax, Ymin, Ymax, print_env_vectors);
    eGV_hanpp = LNV.ExtractVarGrid(LNV.Read_Env_CSV( csv_dir + "hanpp.csv", print2 ), Xmin, Xmax, Ymin, Ymax, print_env_vectors);
    eGV_NumberOfGridCells = eGV_realm_class.size();

    //std::cout << "N gridcells in data: " << eGV_NumberOfGridCells << std::endl;

    // load spatial + temporal (climate) input data
    if(print2==1) std::cout << "Reading spatial csv input: " + csv_dir + "diurnal_temperature_range_*.csv" << std::endl;
    for(int i = 0; i < 12; i++) eGV_dtr.push_back( LNV.ExtractVarGrid(LNV.Read_Env_CSV( csv_dir + "diurnal_temperature_range_" + 
                                                   std::to_string(i+1) + ".csv", 0 ), Xmin, Xmax, Ymin, Ymax, print_env_vectors) );
    if(print2==1) std::cout << "Reading spatial csv input: " + csv_dir + "ground_frost_frequency_*.csv" << std::endl;
    for(int i = 0; i < 12; i++) eGV_gff.push_back( LNV.ExtractVarGrid(LNV.Read_Env_CSV( csv_dir + "ground_frost_frequency_" + 
                                                   std::to_string(i+1) + ".csv", 0 ), Xmin, Xmax, Ymin, Ymax, print_env_vectors) );
    if(print2==1) std::cout << "Reading spatial csv input: " + csv_dir + "near-surface_temperature_*.csv" << std::endl;
    for(int i = 0; i < 12; i++) eGV_nst.push_back( LNV.ExtractVarGrid(LNV.Read_Env_CSV( csv_dir + "near-surface_temperature_" + 
                                                   std::to_string(i+1) + ".csv", 0 ), Xmin, Xmax, Ymin, Ymax, print_env_vectors) );
    if(print2==1) std::cout << "Reading spatial csv input: " + csv_dir + "precipitation_*.csv" << std::endl;
    for(int i = 0; i < 12; i++) eGV_prec.push_back( LNV.ExtractVarGrid(LNV.Read_Env_CSV( csv_dir + "precipitation_" + 
                                                   std::to_string(i+1) + ".csv", 0 ), Xmin, Xmax, Ymin, Ymax, print_env_vectors) );
    if(print2==1) std::cout << "Reading spatial csv input: " + csv_dir + "terrestrial_net_primary_productivity_*.csv" << std::endl;
    for(int i = 0; i < 12; i++) eGV_tnpp.push_back( LNV.ExtractVarGrid(LNV.Read_Env_CSV( csv_dir + "terrestrial_net_primary_productivity_" + 
                                                   std::to_string(i+1) + ".csv", 0 ), Xmin, Xmax, Ymin, Ymax, print_env_vectors) );

    // load spatial body mass masks for cohort initialization
    eGV_Ecto_max   = LNV.ExtractVarGrid(LNV.Read_Env_CSV( csv_dir + "Ecto_max.csv", print2 ), Xmin, Xmax, Ymin, Ymax, print_env_vectors);
    eGV_Endo_C_max = LNV.ExtractVarGrid(LNV.Read_Env_CSV( csv_dir + "Endo_C_max.csv", print2 ), Xmin, Xmax, Ymin, Ymax, print_env_vectors);
    eGV_Endo_H_max = LNV.ExtractVarGrid(LNV.Read_Env_CSV( csv_dir + "Endo_H_max.csv", print2 ), Xmin, Xmax, Ymin, Ymax, print_env_vectors);
    eGV_Endo_O_max = LNV.ExtractVarGrid(LNV.Read_Env_CSV( csv_dir + "Endo_O_max.csv", print2 ), Xmin, Xmax, Ymin, Ymax, print_env_vectors);

    // set spatial input layers in inputparameters
    InputParameters::Get( )->SetSpatialLayers_realm_class( eGV_realm_class );
    InputParameters::Get( )->SetSpatialLayers_awc( eGV_awc );
    InputParameters::Get( )->SetSpatialLayers_land_mask( eGV_land_mask );
    InputParameters::Get( )->SetSpatialLayers_hanpp( eGV_hanpp );

    InputParameters::Get( )->SetSpatialLayers_NumberOfGridCells( eGV_NumberOfGridCells );

    InputParameters::Get( )->SetSpatialLayers_Ecto_max( eGV_Ecto_max );
    InputParameters::Get( )->SetSpatialLayers_Endo_C_max( eGV_Endo_C_max );
    InputParameters::Get( )->SetSpatialLayers_Endo_H_max( eGV_Endo_H_max );
    InputParameters::Get( )->SetSpatialLayers_Endo_O_max( eGV_Endo_O_max );
        
    InputParameters::Get( )->SetSpatialLayers_dtr( eGV_dtr );
    InputParameters::Get( )->SetSpatialLayers_gff( eGV_gff );
    InputParameters::Get( )->SetSpatialLayers_nst( eGV_nst );
    InputParameters::Get( )->SetSpatialLayers_prec( eGV_prec );
    InputParameters::Get( )->SetSpatialLayers_tnpp( eGV_tnpp );

    //////////////////////////////////////////////////////////////////////
    //# END
    //////////////////////////////////////////////////////////////////////


    FileReader fileReader;
    if( fileReader.ReadFiles( ) == true ) { // reading (amongst other things) netcdf files --> netcdf part removed, inputs loaded using LoadCSVEnvironment
            
        // Initialise the model, declare an instance of the class that runs a Madingley model simulation
        Madingley madingleyModel;

        // Run the simulation
        if(TypeOfRun=="run") {
            //std::cout << "Running Madingley!" << std::endl;
            madingleyModel.Run( );
        }

        // Stop the timer and write out the time taken to run this simulation
        s.Stop( );
        std::cout << "Total elapsed time was " + Convertor::Get( )->ToString( s.GetElapsedTimeSecs( ) ) + " (sec)" << std::endl;

    } else {

        std::cout << "ERROR> File reading failed. System exiting..." << std::endl;

    }
    //std::cout << std::endl;
    return 0;
}
