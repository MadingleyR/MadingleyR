#include "InputParameters.h"

Types::InputParametersPointer InputParameters::mThis = NULL;

Types::InputParametersPointer InputParameters::Get( ) {
    if( mThis == NULL ) {
        mThis = new InputParameters( );
    }
    return mThis;
}
InputParameters::~InputParameters( ) {
}
InputParameters::InputParameters( ) {
}

// mTestParam
std::string InputParameters::GetTestParam( ) const {
    return mTestParam;
}
void InputParameters::SetTestParam(const std::string& string){
    mTestParam=string;
}

// mConfigurationDirectory
std::string InputParameters::GetConfigurationDirectory( ) const {
    return mConfigurationDirectory;
}
void InputParameters::SetConfigurationDirectory(const std::string& string){
    mConfigurationDirectory=string;
}

// mOutputFolderName
std::string InputParameters::GetOutputFolderName( ) const {
    return mOutputFolderName;
}
void InputParameters::SetOutputFolderName(const std::string& string){
    mOutputFolderName=string;
}

// mTypeOfRun
std::string InputParameters::GetTypeOfRun( ) const {
    return mTypeOfRun;
}
void InputParameters::SetTypeOfRun(const std::string& string){
    mTypeOfRun=string;
}

// mNumberOfSimulationYears
int InputParameters::GetNumberOfSimulationYears( ) const {
    return mNumberOfSimulationYears;
}
void InputParameters::SetNumberOfSimulationYears( const int& val ) {
    mNumberOfSimulationYears = val;
}

// gridcell size
double InputParameters::GetGridcellSize( ) const {return mGridcellSize;}
void InputParameters::SetGridcellSize( const double& val ) {mGridcellSize = val;}


// mXmin
int InputParameters::GetXmin( ) const {return mXmin;}
void InputParameters::SetXmin( const int& val ) {mXmin = val;}
// mXmax
int InputParameters::GetXmax( ) const {return mXmax;}
void InputParameters::SetXmax( const int& val ) {mXmax = val;}
// mYmin
int InputParameters::GetYmin( ) const {return mYmin;}
void InputParameters::SetYmin( const int& val ) {mYmin = val;}
// mYmax
int InputParameters::GetYmax( ) const {return mYmax;}
void InputParameters::SetYmax( const int& val ) {mYmax = val;}

// mTimestepWritingBinnedCohortStatistics;
int InputParameters::GetTimestepWritingBinnedCohortStatistics( ) const {return mTimestepWritingBinnedCohortStatistics;}
void InputParameters::SetTimestepWritingBinnedCohortStatistics( const int& val ) {mTimestepWritingBinnedCohortStatistics = val;}
// mTimestepWritingFullCohortProperties;
int InputParameters::GetTimestepWritingFullCohortProperties( ) const {return mTimestepWritingFullCohortProperties;}
void InputParameters::SetTimestepWritingFullCohortProperties( const int& val ) {mTimestepWritingFullCohortProperties = val;}
// mTimestepWritingPreyBinnedFoodwebConnections;
int InputParameters::GetTimestepWritingPreyBinnedFoodwebConnections( ) const {return mTimestepWritingPreyBinnedFoodwebConnections;}
void InputParameters::SetTimestepWritingPreyBinnedFoodwebConnections( const int& val ) {mTimestepWritingPreyBinnedFoodwebConnections = val;}
// mTimestepWritingStockProperties;
int InputParameters::GetTimestepWritingStockProperties( ) const {return mTimestepWritingStockProperties;}
void InputParameters::SetTimestepWritingStockProperties( const int& val ) {mTimestepWritingStockProperties = val;}
// mWriteGridProperties;
int InputParameters::GetWriteGridProperties( ) const {return mWriteGridProperties;}
void InputParameters::SetWriteGridProperties( const int& val ) {mWriteGridProperties = val;}
    
// mMaxCohortNumber
int InputParameters::GetMaxCohortNumber( ) const {
    return mMaxCohortNumber;
}
void InputParameters::SetMaxCohortNumber( const int& val ) {
    mMaxCohortNumber = val;
}

// mCohortCSVLocation
std::string InputParameters::GetCohortCSVLocation( ) const {
    return mCohortCSVLocation;
}
void InputParameters::SetCohortCSVLocation(const std::string& string){
    mCohortCSVLocation=string;
}
// mStockCSVLocation
std::string InputParameters::GetStockCSVLocation( ) const {
    return mStockCSVLocation;
}
void InputParameters::SetStockCSVLocation(const std::string& string){
    mStockCSVLocation=string;
}
// mSpatialInputLocation
std::string InputParameters::GetSpatialInputLocation( ) const {
    return mSpatialInputLocation;
}
void InputParameters::SetSpatialInputLocation(const std::string& string){
    mSpatialInputLocation=string;
}

// mRestartedFromTimeStepMonth;
int InputParameters::GetRestartedFromTimeStepMonth( ) const {return mRestartedFromTimeStepMonth;}
void InputParameters::SetRestartedFromTimeStepMonth( const int& val ) {mRestartedFromTimeStepMonth = val;}
// mTimeStep_Months_Calc_FoodWeb;
int InputParameters::GetTimeStep_Months_Calc_FoodWeb( ) const {return mTimeStep_Months_Calc_FoodWeb;}
void InputParameters::SetTimeStep_Months_Calc_FoodWeb( const int& val ) {mTimeStep_Months_Calc_FoodWeb = val;}
// mUseNonDefaultModelParameters;
int InputParameters::GetUseNonDefaultModelParameters( ) const {return mUseNonDefaultModelParameters;}
void InputParameters::SetUseNonDefaultModelParameters( const int& val ) {mUseNonDefaultModelParameters = val;}

// mOverwriteApplySpinUp
bool InputParameters::GetOverwriteApplySpinUp( ) const {
    return mOverwriteApplySpinUp;
}
void InputParameters::SetOverwriteApplySpinUp(const bool& bool_val){
    mOverwriteApplySpinUp=bool_val;
}

// Model parameters
void InputParameters::SetSpatialLayers_awc( const std::vector<double>& Activity_Parameters,
                                            const std::vector<double>& Dispersal_Parameters,
                                            const std::vector<double>& EatingCarnivory_Parameters,
                                            const std::vector<double>& EatingHerbivory_Parameters,
                                            const std::vector<double>& MetabolismEctotherm_Parameters,
                                            const std::vector<double>& MetabolismEndotherm_Parameters,
                                            const std::vector<double>& MetabolismHeterotroph_Parameters,
                                            const std::vector<double>& Mortality_Parameters,
                                            const std::vector<double>& Reproduction_Parameters,
                                            const std::vector<double>& VegetationModel_Parameters ){
    m_Activity_Parameters = Activity_Parameters;
    m_Dispersal_Parameters = Dispersal_Parameters;
    m_EatingCarnivory_Parameters = EatingCarnivory_Parameters;
    m_EatingHerbivory_Parameters = EatingHerbivory_Parameters;
    m_MetabolismEctotherm_Parameters = MetabolismEctotherm_Parameters;
    m_MetabolismEndotherm_Parameters = MetabolismEndotherm_Parameters;
    m_MetabolismHeterotroph_Parameters = MetabolismHeterotroph_Parameters;
    m_Mortality_Parameters = Mortality_Parameters;
    m_Reproduction_Parameters = Reproduction_Parameters;
    m_VegetationModel_Parameters = VegetationModel_Parameters;
}

std::vector<double> InputParameters::Get_Activity_Parameters( ) const {
    return m_Activity_Parameters;
}
std::vector<double> InputParameters::Get_Dispersal_Parameters( ) const {
    return m_Dispersal_Parameters;
}
std::vector<double> InputParameters::Get_EatingCarnivory_Parameters( ) const {
    return m_EatingCarnivory_Parameters;
}
std::vector<double> InputParameters::Get_EatingHerbivory_Parameters( ) const {
    return m_EatingHerbivory_Parameters;
}

double InputParameters::Get_USE_HANPP( ) const {
    return mUSE_HANPP;
}
void InputParameters::Set_USE_HANPP(const double& val ) {
    mUSE_HANPP = val;
}

int InputParameters::GetRunWithoutDispersal( ) const {
    return mRunWithoutDispersal;
}
void InputParameters::SetRunWithoutDispersal(const int& val ) {
    mRunWithoutDispersal = val;
}

int InputParameters::GetRunInParallel( ) const {
    return mRunInParallel;
}
void InputParameters::SetRunInParallel(const int& val ) {
    mRunInParallel = val;
}



std::vector<double> InputParameters::Get_MetabolismEctotherm_Parameters( ) const {
    return m_MetabolismEctotherm_Parameters;
}
std::vector<double> InputParameters::Get_MetabolismEndotherm_Parameters( ) const {
    return m_MetabolismEndotherm_Parameters;
}
std::vector<double> InputParameters::Get_MetabolismHeterotroph_Parameters( ) const {
    return m_MetabolismHeterotroph_Parameters;
}
std::vector<double> InputParameters::Get_Mortality_Parameters( ) const {
    return m_Mortality_Parameters;
}
std::vector<double> InputParameters::Get_Reproduction_Parameters( ) const {
    return m_Reproduction_Parameters;
}
std::vector<double> InputParameters::Get_VegetationModel_Parameters( ) const {
    return m_VegetationModel_Parameters;
}


// -------------------------------------------------
// Spatial inputs
void InputParameters::SetSpatialLayers_awc( const std::vector<double>& input_vector ){
    m_awc = input_vector;
}
std::vector<double> InputParameters::GetSpatialLayers_awc( ) const {
    return m_awc;
}
void InputParameters::SetSpatialLayers_land_mask( const std::vector<double>& input_vector ){
    m_land_mask = input_vector;
} 
std::vector<double> InputParameters::GetSpatialLayers_land_mask( ) const {
    return m_land_mask;
}
void InputParameters::SetSpatialLayers_hanpp( const std::vector<double>& input_vector ){
    m_hanpp = input_vector;
}
std::vector<double> InputParameters::GetSpatialLayers_hanpp( ) const{
    return m_hanpp;
}
void InputParameters::SetSpatialLayers_realm_class( const std::vector<double>& input_vector ){
    m_realm_class = input_vector;
} 
std::vector<double> InputParameters::GetSpatialLayers_realm_class( ) const{
    return m_realm_class;
}
void InputParameters::SetSpatialLayers_Ecto_max( const std::vector<double>& input_vector ){
    m_Ecto_max = input_vector;
}
std::vector<double> InputParameters::GetSpatialLayers_Ecto_max( ) const{
    return m_Ecto_max;
}
void InputParameters::SetSpatialLayers_Endo_C_max( const std::vector<double>& input_vector ){
    m_Endo_C_max = input_vector;
}
std::vector<double> InputParameters::GetSpatialLayers_Endo_C_max( ) const{
    return m_Endo_C_max;
}
void InputParameters::SetSpatialLayers_Endo_O_max( const std::vector<double>& input_vector ){
    m_Endo_O_max = input_vector;
}
std::vector<double> InputParameters::GetSpatialLayers_Endo_O_max( ) const{
    return m_Endo_O_max;
}
void InputParameters::SetSpatialLayers_Endo_H_max( const std::vector<double>& input_vector ){
    m_Endo_H_max = input_vector;
}
std::vector<double> InputParameters::GetSpatialLayers_Endo_H_max( ) const{
    return m_Endo_H_max;
}
void InputParameters::SetSpatialLayers_NumberOfGridCells( const int& n_cells ){
    m_NumberOfGridCells = n_cells;
}
int InputParameters::GetSpatialLayers_NumberOfGridCells( ) const{
    return m_NumberOfGridCells;
}
void InputParameters::SetSpatialLayers_dtr( std::vector <std::vector<double>>& input_vector ){
    m_dtr = input_vector;
}
std::vector <std::vector<double>> InputParameters::GetSpatialLayers_dtr( ) const{
    return m_dtr;
}
void InputParameters::SetSpatialLayers_gff( std::vector <std::vector<double>>& input_vector ){
    m_gff = input_vector;
} 
std::vector <std::vector<double>> InputParameters::GetSpatialLayers_gff( ) const{
    return m_gff;
}
void InputParameters::SetSpatialLayers_nst( std::vector <std::vector<double>>& input_vector ){
    m_nst = input_vector;
} 
std::vector <std::vector<double>> InputParameters::GetSpatialLayers_nst( ) const{
    return m_nst;
}
void InputParameters::SetSpatialLayers_prec( std::vector <std::vector<double>>& input_vector ){
    m_prec = input_vector;
} 
std::vector <std::vector<double>> InputParameters::GetSpatialLayers_prec( ) const{
    return m_prec;
}
void InputParameters::SetSpatialLayers_tnpp( std::vector <std::vector<double>>& input_vector ){
    m_tnpp = input_vector;
} 
std::vector <std::vector<double>> InputParameters::GetSpatialLayers_tnpp( ) const{
    return m_tnpp;
}
// -------------------------------------------------