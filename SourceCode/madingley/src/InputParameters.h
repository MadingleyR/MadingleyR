#ifndef INPUTPARAMETERS
#define INPUTPARAMETERS

#include "Types.h"

class InputParameters {
public:

    ~InputParameters( );
    static Types::InputParametersPointer Get( );

    // mTestParam
    std::string GetTestParam( ) const;
    void SetTestParam( const std::string& string); 

    // mConfigurationDirectory
    std::string GetConfigurationDirectory( ) const;
    void SetConfigurationDirectory( const std::string& string); 

    // mOutputFolderName
    std::string GetOutputFolderName( ) const;
    void SetOutputFolderName( const std::string& string); 

    // mTypeOfRun
    std::string GetTypeOfRun( ) const;
    void SetTypeOfRun( const std::string& string); 

    // mNumberOfSimulationYears
    int GetNumberOfSimulationYears( ) const;
    void SetNumberOfSimulationYears( const int& ); 
	
	
	// gridcell size
	double GetGridcellSize( ) const;
	void SetGridcellSize( const double& );

    // mXmin;
    int GetXmin( ) const;
    void SetXmin( const int& ); 
    // mXmax;
    int GetXmax( ) const;
    void SetXmax( const int& ); 
    // mYmin;
    int GetYmin( ) const;
    void SetYmin( const int& ); 
    // mYmax;
    int GetYmax( ) const;
    void SetYmax( const int& ); 

    // mTimestepWritingBinnedCohortStatistics;
    int GetTimestepWritingBinnedCohortStatistics( ) const;
    void SetTimestepWritingBinnedCohortStatistics( const int& ); 
    // mTimestepWritingFullCohortProperties;
    int GetTimestepWritingFullCohortProperties( ) const;
    void SetTimestepWritingFullCohortProperties( const int& ); 
    // mTimestepWritingPreyBinnedFoodwebConnections;
    int GetTimestepWritingPreyBinnedFoodwebConnections( ) const;
    void SetTimestepWritingPreyBinnedFoodwebConnections( const int& ); 
    // mTimestepWritingStockProperties;
    int GetTimestepWritingStockProperties( ) const;
    void SetTimestepWritingStockProperties( const int& ); 
    // mWriteGridProperties;
    int GetWriteGridProperties( ) const;
    void SetWriteGridProperties( const int& ); 

    // mMaxCohortNumber;
    int GetMaxCohortNumber( ) const;
    void SetMaxCohortNumber( const int& ); 

    // mCohortCSVLocation
    std::string GetCohortCSVLocation( ) const;
    void SetCohortCSVLocation( const std::string& string); 
    // mStockCSVLocation
    std::string GetStockCSVLocation( ) const;
    void SetStockCSVLocation( const std::string& string); 
    // mSpatialInputLocation
    std::string GetSpatialInputLocation( ) const;
    void SetSpatialInputLocation( const std::string& string); 

    // mRestartedFromTimeStepMonth;
    int GetRestartedFromTimeStepMonth( ) const;
    void SetRestartedFromTimeStepMonth( const int& ); 
    // mTimeStep_Months_Calc_FoodWeb;
    int GetTimeStep_Months_Calc_FoodWeb( ) const;
    void SetTimeStep_Months_Calc_FoodWeb( const int& ); 
    // mUseNonDefaultModelParameters;
    int GetUseNonDefaultModelParameters( ) const;
    void SetUseNonDefaultModelParameters( const int& ); 

    // mOverwriteApplySpinUp
    bool GetOverwriteApplySpinUp( ) const;
    void SetOverwriteApplySpinUp( const bool& ); 

    // Spatial inputs
    void SetSpatialLayers_awc( const std::vector<double>& ); 
    std::vector<double> GetSpatialLayers_awc( ) const;
    void SetSpatialLayers_land_mask( const std::vector<double>& ); 
    std::vector<double> GetSpatialLayers_land_mask( ) const;
    void SetSpatialLayers_hanpp( const std::vector<double>& ); 
    std::vector<double> GetSpatialLayers_hanpp( ) const;
    void SetSpatialLayers_realm_class( const std::vector<double>& ); 
    std::vector<double> GetSpatialLayers_realm_class( ) const;

    void SetSpatialLayers_Ecto_max( const std::vector<double>& ); 
    std::vector<double> GetSpatialLayers_Ecto_max( ) const;
    void SetSpatialLayers_Endo_C_max( const std::vector<double>& ); 
    std::vector<double> GetSpatialLayers_Endo_C_max( ) const;
    void SetSpatialLayers_Endo_O_max( const std::vector<double>& ); 
    std::vector<double> GetSpatialLayers_Endo_O_max( ) const;
    void SetSpatialLayers_Endo_H_max( const std::vector<double>& ); 
    std::vector<double> GetSpatialLayers_Endo_H_max( ) const;

    void SetSpatialLayers_NumberOfGridCells( const int& ); 
    int GetSpatialLayers_NumberOfGridCells( ) const;

    void SetSpatialLayers_dtr( std::vector <std::vector<double>>& ); 
    std::vector <std::vector<double>> GetSpatialLayers_dtr( ) const;
    void SetSpatialLayers_gff( std::vector <std::vector<double>>& ); 
    std::vector <std::vector<double>> GetSpatialLayers_gff( ) const;
    void SetSpatialLayers_nst( std::vector <std::vector<double>>& ); 
    std::vector <std::vector<double>> GetSpatialLayers_nst( ) const;
    void SetSpatialLayers_prec( std::vector <std::vector<double>>& ); 
    std::vector <std::vector<double>> GetSpatialLayers_prec( ) const;
    void SetSpatialLayers_tnpp( std::vector <std::vector<double>>& ); 
    std::vector <std::vector<double>> GetSpatialLayers_tnpp( ) const;





    void SetRandomCohortOrderNoiseThreshold( const double& ); 
    double GetRandomCohortOrderNoiseThreshold( ) const;

    void SetRunWithoutDispersal( const int& ); 
    int GetRunWithoutDispersal( ) const;
    
    void SetRunInParallel( const int& ); 
    int GetRunInParallel( ) const;


    // Model parameters
    void SetSpatialLayers_awc( const std::vector<double>&, const std::vector<double>&,
                               const std::vector<double>&, const std::vector<double>&,
                               const std::vector<double>&, const std::vector<double>&,
                               const std::vector<double>&, const std::vector<double>&,
                               const std::vector<double>&, const std::vector<double>& );
    std::vector<double> Get_Activity_Parameters( ) const;
    std::vector<double> Get_Dispersal_Parameters( ) const;
    std::vector<double> Get_EatingCarnivory_Parameters( ) const;
    std::vector<double> Get_EatingHerbivory_Parameters( ) const;
    std::vector<double> Get_MetabolismEctotherm_Parameters( ) const; 
    std::vector<double> Get_MetabolismEndotherm_Parameters( ) const; 
    std::vector<double> Get_MetabolismHeterotroph_Parameters( ) const; 
    std::vector<double> Get_Mortality_Parameters( ) const; 
    std::vector<double> Get_Reproduction_Parameters( ) const;
    std::vector<double> Get_VegetationModel_Parameters( ) const; 

private:
    InputParameters( );

    static Types::InputParametersPointer mThis;

    // User defined parameters
    std::string mTestParam;
    std::string mConfigurationDirectory;
    std::string mOutputFolderName;
    std::string mTypeOfRun;
    int mNumberOfSimulationYears;
	
	double mGridcellSize;
	
    int mXmin;
    int mXmax;
    int mYmin;
    int mYmax;
    int mTimestepWritingBinnedCohortStatistics;
    int mTimestepWritingFullCohortProperties;
    int mTimestepWritingPreyBinnedFoodwebConnections;
    int mTimestepWritingStockProperties;
    int mWriteGridProperties;
    int mMaxCohortNumber;
    std::string mCohortCSVLocation;
    std::string mStockCSVLocation;
    std::string mSpatialInputLocation;
    int mRestartedFromTimeStepMonth;
    int mTimeStep_Months_Calc_FoodWeb;
    int mUseNonDefaultModelParameters;
    bool mOverwriteApplySpinUp;

    double mRandomCohortOrderNoiseThreshold;
    int mRunWithoutDispersal;
    int mRunInParallel;

    // grid env vectors[cellIndex], loaded csv data
    std::vector<double> m_awc;
    std::vector<double> m_land_mask;
    std::vector<double> m_hanpp;
    std::vector<double> m_realm_class;
    std::vector<double> m_Ecto_max; // Ecto_max
    std::vector<double> m_Endo_C_max; // Endo_C_max
    std::vector<double> m_Endo_H_max; // Endo_H_max
    std::vector<double> m_Endo_O_max; // Endo_O_max
    int m_NumberOfGridCells;

    // 3d env grid vectors[timeIndex][cellIndex], loaded csv data 
    std::vector <std::vector<double>> m_dtr; // diurnal_temperature_range
    std::vector <std::vector<double>> m_gff; // ground_frost_frequency
    std::vector <std::vector<double>> m_nst; // near-surface_temperature
    std::vector <std::vector<double>> m_prec; // precipitation
    std::vector <std::vector<double>> m_tnpp; // terrestrial_net_primary_productivity

    //################################## Model specific parameters
    std::vector<double> m_Activity_Parameters;
    std::vector<double> m_Dispersal_Parameters;
    std::vector<double> m_EatingCarnivory_Parameters;
    std::vector<double> m_EatingHerbivory_Parameters;
    std::vector<double> m_MetabolismEctotherm_Parameters;
    std::vector<double> m_MetabolismEndotherm_Parameters;
    std::vector<double> m_MetabolismHeterotroph_Parameters;
    std::vector<double> m_Mortality_Parameters;
    std::vector<double> m_Reproduction_Parameters;
    std::vector<double> m_VegetationModel_Parameters;
    //################################## Model specific parameters

};

#endif

