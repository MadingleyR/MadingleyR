#include "Parameters.h"
#include "Constants.h"
#include "Convertor.h"
#include "Maths.h"
#include "Processor.h"
#include "DataCoords.h"
#include "DataIndices.h"
#include "InputParameters.h"

Types::ParametersPointer Parameters::mThis = NULL;

Types::ParametersPointer Parameters::Get( ) {
    if( mThis == NULL ) {
        mThis = new Parameters( );
    }
    return mThis;
}

Parameters::~Parameters( ) {

    delete[ ] mMonthlyTimeStepArray;
    delete[ ] mAnnualTimeStepArray;
    delete[ ] mDataLongitudeArray;
    delete[ ] mDataLatitudeArray;
    delete[ ] mUserLongitudeArray;
    delete[ ] mUserLatitudeArray;

    if( mThis != NULL ) {
        delete mThis;
    }
}

Parameters::Parameters( ) {
}

bool Parameters::Initialise( const Types::StringMatrix& rawInputParameterData ) {
    bool success = false;

    //SetRootDataDirectory( "none" );
    SetTimeStepUnits( "months" );
    //SetRestartedFromTimeStep( 1 );
    SetPlanktonSizeThreshold( 0.01 );
    SetApplyModelSpinup( 10 );
    SetLengthOfSimulationInMonths( InputParameters::Get( )->GetNumberOfSimulationYears() );
    //######################
    
    // setup spatial domain
    SetSpatialSelection( "block" );
    SetUserMinimumLongitude( InputParameters::Get( )->GetXmin() );
    SetUserMaximumLongitude( InputParameters::Get( )->GetXmax() );
    SetUserMinimumLatitude( InputParameters::Get( )->GetYmin() );
    SetUserMaximumLatitude( InputParameters::Get( )->GetYmax() );


	//std::cout << "selected gridcell size = " << InputParameters::Get( )->GetGridcellSize() << std::endl;
    SetGridCellSize( InputParameters::Get( )->GetGridcellSize()  );
    SetExtinctionThreshold( 1 );
    SetMaximumNumberOfCohorts( InputParameters::Get( )->GetMaxCohortNumber() );
    SetDrawRandomly( "yes" );
    
    // HNPP scenario settings
    SetHumanNPPScenarioType( "none" );
    SetHumanNPPExtractionScale( 0 );
    SetHumanNPPScenarioDuration( 0 );
    SetBurninSteps( 0 );
    SetImpactSteps( 0 );
    SetRecoverySteps( 0 );
    //######################

    SetRunParallel( 1 );
    SetThreadNumber( 1 );

    //#####################

    CalculateParameters( );

    success = true;
    return success;
}

void Parameters::CalculateParameters( ) {
	
		std::cout << "Spatial resolution: " << mGridCellSize << " degree" << std::endl;

    // Calculate temporal parameters
    mLengthOfSimulationInMonths = mLengthOfSimulationInYears * 12;

    mMonthlyTimeStepArray = new unsigned[ mLengthOfSimulationInMonths ];
    for( unsigned monthIndex = 0; monthIndex < mLengthOfSimulationInMonths; ++monthIndex ) {
        mMonthlyTimeStepArray[ monthIndex ] = monthIndex;
    }

    mAnnualTimeStepArray = new unsigned[ mLengthOfSimulationInYears ];
    for( unsigned yearIndex = 0; yearIndex < mLengthOfSimulationInYears; ++yearIndex ) {
        mAnnualTimeStepArray[ yearIndex ] = yearIndex;
    }

    // Calculate spatial parameters
    mLengthDataLongitudeArray = 360 / mGridCellSize;
    mDataLongitudeArray = new float[ mLengthDataLongitudeArray ];
    for( unsigned longitudeIndex = 0; longitudeIndex < mLengthDataLongitudeArray; ++longitudeIndex ) {
        mDataLongitudeArray[ longitudeIndex ] = ( -180 + ( ( float )mGridCellSize / 2 ) ) + ( longitudeIndex * ( float )mGridCellSize );
    }

    mLengthDataLatitudeArray = 180 / mGridCellSize;
    mDataLatitudeArray = new float[ mLengthDataLatitudeArray ];
    for( unsigned latitudeIndex = 0; latitudeIndex < mLengthDataLatitudeArray; ++latitudeIndex ) {
        mDataLatitudeArray[ latitudeIndex ] = ( -90 + ( ( float )mGridCellSize / 2 ) ) + ( latitudeIndex * ( float )mGridCellSize );
    }

    mDataIndexOfUserMinimumLongitude = Processor::Get( )->CalculateArrayIndexOfValue( mDataLongitudeArray, mLengthDataLongitudeArray, mUserMinimumLongitude );
    
		if(mGridCellSize==1){
			mDataIndexOfUserMaximumLongitude = abs(InputParameters::Get( )->GetXmin() -InputParameters::Get( )->GetXmax() ) + mDataIndexOfUserMinimumLongitude;
		}else{
			mDataIndexOfUserMaximumLongitude = Processor::Get( )->CalculateArrayIndexOfValue( mDataLongitudeArray, mLengthDataLongitudeArray, mUserMaximumLongitude );
		}
		
		mLengthUserLongitudeArray = ( mDataIndexOfUserMaximumLongitude - mDataIndexOfUserMinimumLongitude ); //+ 1;

    mUserLongitudeArray = new float[ mLengthUserLongitudeArray ];
    for( unsigned userLongitudeIndex = 0; userLongitudeIndex < mLengthUserLongitudeArray; ++userLongitudeIndex ) {
        mUserLongitudeArray[ userLongitudeIndex ] = mDataLongitudeArray[ userLongitudeIndex + mDataIndexOfUserMinimumLongitude ];
    }

    mDataIndexOfUserMinimumLatitude = Processor::Get( )->CalculateArrayIndexOfValue( mDataLatitudeArray, mLengthDataLatitudeArray, mUserMinimumLatitude );

		if(mGridCellSize==1){    
			mDataIndexOfUserMaximumLatitude = abs( InputParameters::Get( )->GetYmin() - InputParameters::Get( )->GetYmax() ) + mDataIndexOfUserMinimumLatitude;
		}else{
			mDataIndexOfUserMaximumLatitude = Processor::Get( )->CalculateArrayIndexOfValue( mDataLatitudeArray, mLengthDataLatitudeArray, mUserMaximumLatitude );
    }
		
		mLengthUserLatitudeArray = ( mDataIndexOfUserMaximumLatitude - mDataIndexOfUserMinimumLatitude ); //+ 1;

    mUserLatitudeArray = new float[ mLengthUserLatitudeArray ];
    for( unsigned userLatitudeIndex = 0; userLatitudeIndex < mLengthUserLatitudeArray; ++userLatitudeIndex ) {
        mUserLatitudeArray[ userLatitudeIndex ] = mDataLatitudeArray[ userLatitudeIndex + mDataIndexOfUserMinimumLatitude ];
    }

    mNumberOfGridCells = mLengthUserLongitudeArray * mLengthUserLatitudeArray;
    mSizeOfMonthlyGridDatum = mNumberOfGridCells * mLengthOfSimulationInMonths;
    mSizeOfAnnualGridDatum = mNumberOfGridCells * mLengthOfSimulationInYears;

	///*
    std::cout << "Number of grid cells: " << mNumberOfGridCells << std::endl;
	//*/

    unsigned cellIndex = 0;
    mCoordsIndicesLookup.resize( mNumberOfGridCells );
    for( unsigned latitudeIndex = 0; latitudeIndex < mLengthUserLatitudeArray; ++latitudeIndex ) {
        for( unsigned longitudeIndex = 0; longitudeIndex < mLengthUserLongitudeArray; ++longitudeIndex ) {

            float longitude = mUserLongitudeArray[ longitudeIndex ];
            float latitude = mUserLatitudeArray[ latitudeIndex ];

            Types::DataCoordsPointer coords = new DataCoords( longitude, latitude );
            Types::DataIndicesPointer indices = new DataIndices( longitudeIndex, latitudeIndex );

            mCoordsIndicesLookup[ cellIndex ] = std::make_pair( coords, indices );

            cellIndex += 1;
        }
    }
}

std::string Parameters::GetRootDataDirectory( ) const {
    return mRootDataDirectory;
}

std::string Parameters::GetTimeStepUnits( ) const {
    return "month";
}

unsigned Parameters::GetLengthOfSimulationInYears( ) const {
    return mLengthOfSimulationInYears;
}

int Parameters::GetUserMinimumLongitude( ) const {
    return mUserMinimumLongitude;
}

int Parameters::GetUserMaximumLongitude( ) const {
    return mUserMaximumLongitude;
}

int Parameters::GetUserMinimumLatitude( ) const {
    return mUserMinimumLatitude;
}

int Parameters::GetUserMaximumLatitude( ) const {
    return mUserMaximumLatitude;
}

float Parameters::GetGridCellSize( ) const {
    return mGridCellSize;
}

float Parameters::GetExtinctionThreshold( ) const {
    return mExtinctionThreshold;
}

unsigned Parameters::GetMaximumNumberOfCohorts( ) const {
    return mMaximumNumberOfCohorts;
}

float Parameters::GetPlanktonSizeThreshold( ) const {
    return mPlanktonSizeThreshold;
}

bool Parameters::GetDrawRandomly( ) const {
    return mDrawRandomly;
}


std::string Parameters::GetHumanNPPScenarioType( ) const {
    return mHumanNPPScenarioType;
}
double Parameters::GetHumanNPPExtractionScale( ) const{
    return mHumanNPPExtractionScale;
}
double Parameters::GetHumanNPPScenarioDuration( ) const{
    return mHumanNPPScenarioDuration;
}
unsigned Parameters::GetBurninSteps( ) const{
    return mBurninSteps;
}
unsigned Parameters::GetImpactSteps( ) const{
    return mImpactSteps;
}
unsigned Parameters::GetRecoverySteps( ) const{
    return mRecoverySteps;
}

//############################################
unsigned Parameters::GetRunParallel( ) const{
    return mRunParallel;
}
unsigned Parameters::GetThreadNumber( ) const{
    return mThreadNumber;
}

std::string Parameters::GetSpatialSelection( ) const{return mSpatialSelection;}
//###########################################

unsigned Parameters::GetApplyModelSpinup( ) const{
    return mApplyModelSpinup;
}
std::string Parameters::GetCohortCSVLocation( ) const{
    return mCohortCSVLocation;
}
std::string Parameters::GetStockCSVLocation( ) const{
    return mStockCSVLocation;
}
std::string Parameters::GetPoolCSVLocation( ) const{
    return mPoolCSVLocation;
}

//# csv outputs
unsigned Parameters::GetWriteCohortSpecifics( ) const{return mWriteCohortSpecifics;}
unsigned Parameters::GetWriteCohortConsumption( ) const{return mWriteCohortConsumption;}
unsigned Parameters::GetWriteConsumptionSummary( ) const{return mWriteConsumptionSummary;}

unsigned Parameters::GetWriteStateInModelTime( ) const{return mWriteStateInModelTime;}
unsigned Parameters::GetWriteStateOverwrite( ) const{return mWriteStateOverwrite;}
double Parameters::GetWriteStateInterval( ) const{return mWriteStateInterval;}
std::string Parameters::GetWriteStateIntervalUnit( ) const{return mWriteStateIntervalUnit;}

//############################################

void Parameters::SetRootDataDirectory( const std::string& rootDataDirectory ) {
    mRootDataDirectory = rootDataDirectory;
}

void Parameters::SetTimeStepUnits( const std::string& timeStepUnits ) {
    mTimeStepUnits = timeStepUnits;
}


void Parameters::SetLengthOfSimulationInMonths( const unsigned& lengthOfSimulationInYears ) {
    mLengthOfSimulationInYears = lengthOfSimulationInYears;
}

void Parameters::SetRestartedFromTimeStep( const unsigned& restart_from  ) {
    mRestartedFromTimeStep = restart_from;
}

void Parameters::SetUserMinimumLongitude( const int& userMinimumLongitude ) {
    mUserMinimumLongitude = userMinimumLongitude;
}

void Parameters::SetUserMaximumLongitude( const int& userMaximumLongitude ) {
    mUserMaximumLongitude = userMaximumLongitude;
}

void Parameters::SetUserMinimumLatitude( const int& userMinimumLatitude ) {
    mUserMinimumLatitude = userMinimumLatitude;
}



void Parameters::SetUserMaximumLatitude( const int& userMaximumLatitude ) {
    mUserMaximumLatitude = userMaximumLatitude;
}

void Parameters::SetGridCellSize( const float& gridCellSize ) {
    mGridCellSize = gridCellSize;
}

void Parameters::SetExtinctionThreshold( const float& extinctionThreshold ) {
    mExtinctionThreshold = extinctionThreshold;
}

void Parameters::SetMaximumNumberOfCohorts( const unsigned& maximumNumberOfCohorts ) {
    mMaximumNumberOfCohorts = maximumNumberOfCohorts;
}

void Parameters::SetPlanktonSizeThreshold( const float& planktonSizeThreshold ) {
    mPlanktonSizeThreshold = planktonSizeThreshold;
}

void Parameters::SetDrawRandomly( const std::string& drawRandomlyString ) {
    if( drawRandomlyString == "yes" )
        mDrawRandomly = true;
    else
        mDrawRandomly = false;
}

void Parameters::SetHumanNPPScenarioType(const std::string& humanNPPScenarioType){
    mHumanNPPScenarioType=humanNPPScenarioType;
}
void Parameters::SetHumanNPPExtractionScale(const double& humanNPPExtractionScale ){
    mHumanNPPExtractionScale=humanNPPExtractionScale;
}
void Parameters::SetHumanNPPScenarioDuration(const double & humanNPPScenarioDuration){
    mHumanNPPScenarioDuration=humanNPPScenarioDuration;
}
void Parameters::SetBurninSteps(const unsigned& burninSteps){
    mBurninSteps=burninSteps;
}
void Parameters::SetImpactSteps(const unsigned& impactSteps){
    mImpactSteps=impactSteps;
}
void Parameters::SetRecoverySteps(const unsigned& recoverySteps){
    mRecoverySteps=recoverySteps;
}

//############################################
void Parameters::SetRunParallel(const unsigned& runParallel){
    mRunParallel=runParallel;
}
void Parameters::SetThreadNumber(const unsigned& threadNumber){
    mThreadNumber=threadNumber;
}

//std::string Parameters::GetSpatialSelection( ) const{return mSpatialSelection;}
void Parameters::SetSpatialSelection( const std::string& spatialSelection ) {
    mSpatialSelection = spatialSelection;
}

//############################################
void Parameters::SetApplyModelSpinup(const unsigned& applyModelSpinup){
    mApplyModelSpinup=applyModelSpinup;
}
void Parameters::SetCohortCSVLocation(const std::string& cohortCSVLocation){
    mCohortCSVLocation=cohortCSVLocation;
}
void Parameters::SetStockCSVLocation(const std::string& stockCSVLocation){
    mStockCSVLocation=stockCSVLocation;
}
void Parameters::SetPoolCSVLocation(const std::string& poolCSVLocation){
    mPoolCSVLocation=poolCSVLocation;
}

//# csv outputs
void Parameters::SetWriteCohortSpecifics( const unsigned& writeCohortSpecifics ) {mWriteCohortSpecifics=writeCohortSpecifics;}
void Parameters::SetWriteCohortConsumption( const unsigned& writeCohortConsumption) {mWriteCohortConsumption=writeCohortConsumption;}
void Parameters::SetWriteConsumptionSummary( const unsigned& writeConsumptionSummary ) {mWriteConsumptionSummary=writeConsumptionSummary;}

void Parameters::SetWriteStateInModelTime( const unsigned& writeStateInModelTime ) {mWriteStateInModelTime=writeStateInModelTime;}
void Parameters::SetWriteStateOverwrite( const unsigned& writeStateOverwrite) {mWriteStateOverwrite=writeStateOverwrite;}
void Parameters::SetWriteStateInterval( const std::string& writeStateInterval ) {
    mWriteStateInterval = std::stoul(writeStateInterval.substr(0, writeStateInterval.size()-1));
}
void Parameters::SetWriteStateIntervalUnit( const std::string& writeStateIntervalUnit ) {
    mWriteStateIntervalUnit = writeStateIntervalUnit.substr(writeStateIntervalUnit.size()-1, writeStateIntervalUnit.size());
}
//############################################

unsigned Parameters::GetNumberOfGridCells( ) const {
    return mNumberOfGridCells;
}

unsigned Parameters::GetLengthOfSimulationInMonths( ) const {
    return mLengthOfSimulationInMonths;
}

unsigned Parameters::GetRestartedFromTimeStep( ) const {
    return mRestartedFromTimeStep;
}

unsigned Parameters::GetLengthDataLongitudeArray( ) const {
    return mLengthDataLongitudeArray;
}

unsigned Parameters::GetLengthDataLatitudeArray( ) const {
    return mLengthDataLatitudeArray;
}

unsigned Parameters::GetDataIndexOfUserMinimumLongitude( ) const {
    return mDataIndexOfUserMinimumLongitude;
}

unsigned Parameters::GetDataIndexOfUserMaximumLongitude( ) const {
    return mDataIndexOfUserMaximumLongitude;
}

unsigned Parameters::GetDataIndexOfUserMinimumLatitude( ) const {
    return mDataIndexOfUserMinimumLatitude;
}

unsigned Parameters::GetDataIndexOfUserMaximumLatitude( ) const {
    return mDataIndexOfUserMaximumLatitude;
}

unsigned Parameters::GetLengthUserLongitudeArray( ) const {
    return mLengthUserLongitudeArray;
}

unsigned Parameters::GetLengthUserLatitudeArray( ) const {
    return mLengthUserLatitudeArray;
}

unsigned Parameters::GetSizeOfMonthlyGridDatum( ) const {
    return mSizeOfMonthlyGridDatum;
}

unsigned Parameters::GetSizeOfAnnualGridDatum( ) const {
    return mSizeOfAnnualGridDatum;
}

float Parameters::GetDataLongitudeAtIndex( const unsigned& index ) const {
    return mDataLongitudeArray[ index ];
}

float Parameters::GetDataLatitudeAtIndex( const unsigned& index ) const {
    return mDataLatitudeArray[ index ];
}

float Parameters::GetUserLongitudeAtIndex( const unsigned& index ) const {
    return mUserLongitudeArray[ index ];
}

float Parameters::GetUserLatitudeAtIndex( const unsigned& index ) const {
    return mUserLatitudeArray[ index ];
}

//float* Parameters::GetDataLongitudeArray( ) const {
//    return mDataLongitudeArray;
//}
//
//float* Parameters::GetDataLatitudeArray( ) const {
//    return mDataLatitudeArray;
//}

unsigned* Parameters::GetMonthlyTimeStepArray( ) const {
    return mMonthlyTimeStepArray;
}

unsigned* Parameters::GetAnnualTimeStepArray( ) const {
    return mAnnualTimeStepArray;
}

float* Parameters::GetUserLongitudeArray( ) const {
    return mUserLongitudeArray;
}

float* Parameters::GetUserLatitudeArray( ) const {
    return mUserLatitudeArray;
}

int Parameters::GetBin( std::vector<double> Bins, double bodymass ) {
    int bin_number = 0;
    for( int i = 0; i < Bins.size(); i++){
        if(bodymass>Bins[i]){
            bin_number++;
        }else{
            break;
        }
    }
    return bin_number;
}


int Parameters::GetCellIndexFromDataIndices( const unsigned& longitudeIndex, const unsigned& latitudeIndex ) const {

    int cellIndex = Constants::cMissingValue;
    for( unsigned index = 0; index < mNumberOfGridCells; ++index ) {
        Types::DataIndicesPointer indices = mCoordsIndicesLookup[ index ].second;

        if( indices->GetX( ) == longitudeIndex && indices->GetY( ) == latitudeIndex ) {
            cellIndex = index;
            break;
        }
    }
    return cellIndex;
}

Types::DataCoordsPointer Parameters::GetDataCoordsFromCellIndex( const unsigned& cellIndex ) const {
    return mCoordsIndicesLookup[ cellIndex ].first;
}

Types::DataIndicesPointer Parameters::GetDataIndicesFromCellIndex( const unsigned& cellIndex ) const {
    return mCoordsIndicesLookup[ cellIndex ].second;
}

