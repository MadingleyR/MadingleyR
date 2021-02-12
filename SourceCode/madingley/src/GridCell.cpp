#include "GridCell.h"

std::vector<Cohort*> GridCell::mNewCohorts;

GridCell::GridCell( ) {
    mGridcellOrganicPool = 0.0;
    mGridcellRespiratoryCO2Pool = 0.0;

}

void GridCell::SetCellCoords( unsigned index ) {
    // set values for this grid cell
    // Also standardise missing values
    mIndex = index;
    Types::DataIndicesPointer indices = Parameters::Get( )->GetDataIndicesFromCellIndex( mIndex );
    // Add the latitude and longitude indices
    mLatitudeIndex = indices->GetY( );
    mLongitudeIndex = indices->GetX( );
    // Add the grid cell area (in km2) to the cell environment with an initial value of 0
    // Calculate the area of this grid cell
    // Add it to the cell environment- latitude value is cell lower left corner
    // Is this really consistent with below? The C# code also has a half cell shift, but in the height and width
    // whereas the utilities code says it wants lower left corner for both areas and lengths
    mCellArea = mUtilities.CalculateGridCellArea( Parameters::Get( )->GetUserLatitudeAtIndex( indices->GetY( ) ), Parameters::Get( )->GetGridCellSize( ) );
    // Calculate the lengths of widths of grid cells in each latitudinal strip
    // Assume that we are at the midpoint of each cell when calculating lengths
    mCellHeightKm = mUtilities.CalculateLengthOfDegreeLatitude( Parameters::Get( )->GetUserLatitudeAtIndex( indices->GetY( ) ) + 
                                                                Parameters::Get( )->GetGridCellSize( ) / 2 ) * Parameters::Get( )->GetGridCellSize( );
    mCellWidthKm = mUtilities.CalculateLengthOfDegreeLongitude( Parameters::Get( )->GetUserLatitudeAtIndex( indices->GetY( ) ) + 
                                                                Parameters::Get( )->GetGridCellSize( ) / 2 ) * Parameters::Get( )->GetGridCellSize( );
}

void GridCell::InsertCohort( Cohort* c ) {
    //std::cout <<  mCohorts.size( ) << std::endl;
    mCohorts[ c->mFunctionalGroupIndex ].push_back( c );
}

void GridCell::RemoveCohort( Cohort* c ) {
    std::vector<Cohort*>& z = mCohorts[ c->mFunctionalGroupIndex ];
    z.erase(std::remove(z.begin(),z.end(),c),z.end());
}

void GridCell::MoveCohort( Cohort* c ) {
    RemoveCohort( c );
    c->mCurrentCell= c->mDestinationCell;
    c->mCurrentCell->InsertCohort(c);
}

void GridCell::RandomizeCohorts( ) {
    for( int index = 0; index < mCohorts.size( ); index++ ) {
        random_shuffle( mCohorts[ index ].begin( ), mCohorts[ index ].end( ) );
    }
}

double GridCell::GetRealm( ) {
    return mGridcellRealmClassification;
    //return Environment::Get( "Realm", *this );
}

bool GridCell::IsMarine( ) {
  return (mGridcellRealmClassification == 1.0);
    //return ( Environment::Get( "Realm", *this ) == 2.0 );
}

unsigned GridCell::GetIndex( ) const {
    return mIndex;
}

unsigned GridCell::GetLatitudeIndex( ) const {
    return mLatitudeIndex;
}

unsigned GridCell::GetLongitudeIndex( ) const {
    return mLongitudeIndex;
}

double GridCell::GetCellArea( ) const {
    return mCellArea;
}

double GridCell::GetCellHeight( ) const {
    return mCellHeightKm;
}

double GridCell::GetCellWidth( ) const {
    return mCellWidthKm;
}

void GridCell::SetCohortSize( unsigned size ) {
    mCohorts.resize( size );
}

//---Environmental set layers--------------------
void GridCell::SetGridcellRealmClassification( double input ){
  mGridcellRealmClassification = input;
}
void GridCell::SetGridcellAvailableWaterCapacity( double input ){
  mGridcellAvailableWaterCapacity = input;

}
void GridCell::SetGridcellLandMask( int input ){
  mGridcellLandMask = input;
}
void GridCell::SetGridcellHanpp( double input ){
  mGridcellHanpp = input;
}
void GridCell::SetGridcellDiurnalTemperatureRange( std::vector<double> input ){
  for( unsigned i = 0; i < 12; i++){
    mGridcellDiurnalTemperatureRange.push_back(input[i]);
  }
}
void GridCell::SetGridcellNearSurfaceTemp( std::vector<double> input ){
  for( unsigned i = 0; i < 12; i++){
    mGridcellNearSurfaceTemp.push_back(input[i]);
  }
}
void GridCell::SetGridcellGroundFrostFrequency( std::vector<double> input ){
  for( unsigned i = 0; i < 12; i++){
    mGridcellGroundFrostFrequency.push_back(input[i]);
  }
}
void GridCell::SetGridcellPrecipitation( std::vector<double> input ){
  double TotalPrecip = 0.0;
  for( unsigned i = 0; i < 12; i++){
    mGridcellPrecipitation.push_back(input[i]);
    TotalPrecip += input[i];
  }
  mGridcellTotalPrecip = TotalPrecip;
}
void GridCell::SetGridcellTerrestrialNPP( std::vector<double> input ){
  for( unsigned i = 0; i < 12; i++){
    mGridcellTerrestrialNPP.push_back(input[i]);
  }
}
void GridCell::Set_Ecto_max( double input ){
  m_Ecto_max = input;
}
void GridCell::Set_Endo_C_max( double input ){
  m_Endo_C_max = input;
}
void GridCell::Set_Endo_H_max( double input ){
  m_Endo_H_max = input;
}
void GridCell::Set_Endo_O_max( double input ){
  m_Endo_O_max = input;
}
void GridCell::SetNPPSeasonalityAndBreeding( std::vector<double> NPP ) {

    // Loop over months and calculate total annual NPP
    double totalNPP = 0.0;
    for( int timeIndex = 0; timeIndex < 12; timeIndex++ ) {
      double N = NPP[timeIndex];
      if( N != Constants::cMissingValue && N > 0 ) totalNPP += N;
    }
    
    if( totalNPP == 0 ) {
      // Loop over months and calculate seasonality
      // If there is no NPP value then assign a uniform flat seasonality
      for( int timeIndex = 0; timeIndex < 12; timeIndex++ ) {
        mGridcellSeasonality.push_back(1.0 / 12.0);
      }
      
    } else {
      // Some NPP data exists for this grid cell so use that to infer the NPP seasonality
      // Loop over months and calculate seasonality
      for( int timeIndex = 0; timeIndex < 12; timeIndex++ ) {
        double N = NPP[timeIndex];
        if( N != Constants::cMissingValue && N > 0 ) {
          mGridcellSeasonality.push_back(N / totalNPP);
        } else {
          mGridcellSeasonality.push_back(0.0);
        }
      }
    }
    
    // Designate a breeding season for this grid cell, where a month is considered to be part of the breeding season if its NPP is at
    // least 80% of the maximum NPP throughout the whole year
    double maxSeason = -1;
    for( int i = 0; i < 12; i++ ) {
      maxSeason = std::max( maxSeason, mGridcellSeasonality[i] );
    }
    
    for( int i = 0; i < 12; i++ ) {
      if( mGridcellSeasonality[i] / maxSeason > 0.5 ) {
        mGridcellBreedingSeason.push_back(1.0);
      } else {
        mGridcellBreedingSeason.push_back(0.0);
      }
    }
}
void GridCell::SetAVGSDTemp( int realm, std::vector<double> nst ) {
    
    double avg = 0;
    for( int timeIndex = 0; timeIndex < 12; timeIndex++ ) {
  
      double d = Constants::cMissingValue;
      if( realm == 1 ) {
        d = Constants::cMissingValue;
      } else if( realm == 2 ) {
        d = nst[timeIndex];
      }
      
      if( d == Constants::cMissingValue ) d = 0;
      avg += d;
    }
    
    avg = avg / 12;
    double sota = 0, sumExp = 0;
    
    std::vector< double > exptdev( 12 );
    for( int timeIndex = 0; timeIndex < 12; timeIndex++ ) {
      
      double d = Constants::cMissingValue;
      if( realm == 1 ) {
        d = Constants::cMissingValue;
      } else if( realm == 2 ) {
        d = nst[timeIndex];
      }
      if( d == Constants::cMissingValue ) d = 0;
      sota += ( d - avg )*( d - avg );
      exptdev[ timeIndex ] = exp( -( d - avg ) / 3 );
      sumExp += exptdev[ timeIndex ];
    }
    
    for( int tm = 0; tm < 12; tm++ ) mGridcellExpTDevWeight.push_back(exptdev[tm] / sumExp);
    
    mGridcellAnnualTemperature = avg;
    mGridcellSDTemperature = sqrt( sota / 12 );
}
void GridCell::SetFrostandFire( int realm, double awc, std::vector<double> nst, std::vector<double> gff, std::vector<double> prec ) {
  // Calculate other climate variables from temperature and precipitation
  // Declare an instance of the climate variables calculator
  ClimateVariablesCalculator CVC;
  
  // Calculate the fraction of the year that experiences frost
  std::vector< double > FrostDays( 12 ), Temperature( 12 ), Precipitation( 12 );
  for( int i = 0; i < 12; i++ ) {
    if( realm == 2 ) {
      FrostDays[i] = gff[i];
      Precipitation[i] = prec[i];
      Temperature[i] = nst[i];
    }
  }
  
  mGridcellFractionYearFrost = CVC.GetNDF( FrostDays, Temperature, Constants::cMissingValue );
  std::vector< double > MonthDays = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  
  for( int i = 0; i < 12; i++ ) mGridcellFractionMonthFrost.push_back(std::min( FrostDays[i] / MonthDays[i], ( double )1.0 ));
  
  std::tuple< std::vector< double >, double, double > TempTuple = CVC.MonthlyActualEvapotranspirationSoilMoisture( awc, Precipitation, Temperature );
  
  double TotalAET = 0;
  for( int i = 0; i < 12; i++ ) {
      mGridcellAET.push_back( std::get< 0 >( TempTuple )[ i ] );
      TotalAET += std::get< 0 >( TempTuple )[ i ];
  }
  mGridcellTotalAET = TotalAET;
  
  mGridcellFractionYearFire = ( std::get< 2 > ( TempTuple ) / 360 );

}
//-----------------------------------------------

//---Environmental get layers--------------------
double GridCell::GetGridcellRealmClassification( ) const {return mGridcellRealmClassification;}
int GridCell::GetGridcellLandMask( ) const {return mGridcellLandMask;}
double GridCell::GetGridcellAvailableWaterCapacity( ) const {return mGridcellAvailableWaterCapacity;}
double GridCell::GetGridcellHanpp( ) const {return mGridcellHanpp;}

double GridCell::GetGridcellDiurnalTemperatureRange( int tstep ) const {return mGridcellDiurnalTemperatureRange[tstep];}
double GridCell::GetGridcellNearSurfaceTemp( int tstep ) const {return mGridcellNearSurfaceTemp[tstep];}
double GridCell::GetGridcellGroundFrostFrequency( int tstep ) const {return mGridcellGroundFrostFrequency[tstep];}
double GridCell::GetGridcellPrecipitation( int tstep ) const {return mGridcellPrecipitation[tstep];}
double GridCell::GetGridcellTerrestrialNPP( int tstep ) const {return mGridcellTerrestrialNPP[tstep];}

double GridCell::GetGridcellTotalPrecip( ) const {return mGridcellTotalPrecip;}

double GridCell::GetGridcellSeasonality( int tstep ) const {return mGridcellSeasonality[tstep];}
double GridCell::GetGridcellBreedingSeason( int tstep ) const {return mGridcellBreedingSeason[tstep];}

double GridCell::GetGridcellAnnualTemperature( ) const {return mGridcellAnnualTemperature;}
double GridCell::GetGridcellSDTemperature( ) const {return mGridcellSDTemperature;}
double GridCell::GetGridcellExpTDevWeight( int tstep ) const {return mGridcellExpTDevWeight[tstep];}

double GridCell::GetGridcellAET( int tstep ) const {return mGridcellAET[tstep];}
double GridCell::GetGridcellTotalAET( ) const {return mGridcellTotalAET;}
double GridCell::GetGridcellFractionMonthFrost( int tstep ) const {return mGridcellFractionMonthFrost[tstep];}
double GridCell::GetGridcellFractionYearFrost( ) const {return mGridcellFractionYearFrost;}
double GridCell::GetGridcellFractionYearFire( ) const {return mGridcellFractionYearFire;}

double GridCell::Get_Ecto_max( ) const {return m_Ecto_max;}
double GridCell::Get_Endo_C_max( ) const {return m_Endo_C_max;}
double GridCell::Get_Endo_H_max( ) const {return m_Endo_H_max;}
double GridCell::Get_Endo_O_max( ) const {return m_Endo_O_max;}
//-----------------------------------------------


/** \brief Gets the number of cohorts in this grid cell */
unsigned GridCell::GetNumberOfCohorts( ) {
    unsigned sum = 0;
    for( unsigned cohortIndex = 0; cohortIndex < mCohorts.size( ); cohortIndex++ ) {
        sum += mCohorts[ cohortIndex ].size( );
    }
    return sum;
}
