#include "ClimateVariablesCalculator.h"

ClimateVariablesCalculator::ClimateVariablesCalculator( ) {

}

std::tuple< std::vector<double>, double, double > ClimateVariablesCalculator::MonthlyActualEvapotranspirationSoilMoisture( double AvailableWaterCapacity, std::vector<double> Precipitation, std::vector<double> MonthlyTemperatures ) {
    // Vector to hold potential evapotranspiration data
    std::vector<double> PotentialEvapotranspiration( 12 );

    // Loop over months and calculate actual evapotranspiration
    for( int i = 0; i < 12; i++ ) {
        PotentialEvapotranspiration[ i ] = CalculatePotentialEvapotranspiration( MonthlyTemperatures[ i ] );
    }

    // This algorithm sets the soil water content at field capacity and simulates 10 years of
    // soil water dynamics, which in testing has been long enough for the annual soil water dynamics to 
    // settle on an equilibrium cycle.
    int RunYears = 10; //Number of years to simulate
    double SoilWaterPast = AvailableWaterCapacity; // Initialise the past soil water content to be field capacity
    std::vector<double> ActualEvapotranspiration( 12 ); // Will store monthly actual avapotranspiration (mm)
    std::vector<double> SoilWater( 12 ); // Will store montly soil water
    std::vector<double> DailyAET( 30 ); // Temporary store for daily actual evapotranspiration
    std::vector<double> DailySWC( 30 ); // Temporary store for daily soil water content
    std::vector<double> MidMonthDailyPET( 12 ); // The daily PET at the middle of each month
    std::vector<double> MidMonthDailyPPT( 12 ); // The daily PPT at the middle of each month
    double SoilMoistureFireThreshold = 0.3;

    for( int jj = 0; jj < 12; jj++ ) // for each month work out the mid point values for PPT and PET
    {
        MidMonthDailyPET[ jj ] = PotentialEvapotranspiration[ jj ] / 30; // Approximating 30 days per month
        MidMonthDailyPPT[ jj ] = Precipitation[ jj ] / 30; // Approximating 30 days per month
    }

    int PrevMonth = 11; // used to store the index of the previous month
    int NextMonth = 1; // used to store the index of the next month
    double PET = 0; // tracks the daily potential evapotranspiration rate (mm day-1)
    double PPT = 0; // tracks the dail predicipitation rate (mm day-1)
    double TMP = 0;

    double LengthOfFireSeason = 0; // The length of the fire season is the fraction of the year that the soil moisture status is below a critical value: indicative of fire risk

    for( int ii = 0; ii < RunYears; ii++ ) // for each of the simulated years
    {
        for( int jj = 0; jj < 12; jj++ ) // for each month
        {
            PrevMonth = ( jj == 0 ) ? 11 : jj - 1; // work out the index of the previous month
            NextMonth = ( jj == 11 ) ? 0 : jj + 1; // work out the index of the next month  

            for( int kk = 0; kk < 30; kk++ ) // for each day in the month
            {
                if( kk < 15 ) // if we are less than half way through the month then linearly interpolate from the previous month
                {
                    PET = MidMonthDailyPET[ PrevMonth ] + ( ( MidMonthDailyPET[ jj ] - MidMonthDailyPET[ PrevMonth ] ) / ( double )15 ) * ( double )kk; //basically divide the difference by fifteen steps and multiply by the number of steps forward
                    PPT = MidMonthDailyPPT[ PrevMonth ] + ( ( MidMonthDailyPPT[ jj ] - MidMonthDailyPPT[ PrevMonth ] ) / ( double )15 ) * ( double )kk;
                } else // if we are more than half way through the month then linearly interpolate forwards
                {
                    PET = MidMonthDailyPET[ jj ] + ( ( MidMonthDailyPET[ NextMonth ] - MidMonthDailyPET[ jj ] ) / ( double )15 ) * ( double )( kk - 15 );
                    PPT = MidMonthDailyPPT[ jj ] + ( ( MidMonthDailyPPT[ NextMonth ] - MidMonthDailyPPT[ jj ] ) / ( double )15 ) * ( double )( kk - 15 );
                }
                if( AvailableWaterCapacity == 0 )DailyAET[ kk ] = 0;
                else
                    DailyAET[ kk ] = PET * ( SoilWaterPast / AvailableWaterCapacity ); // this is the potential evapotranspiration rates scaled by how dry the soil is. The further the soil water is from field capacity the less the evapotranspiration rate is.
                
                DailySWC[ kk ] = std::min( std::max( ( SoilWaterPast + PPT - DailyAET[ kk ] ), ( double )0 ), AvailableWaterCapacity ); //Soil water content is then updated
                SoilWaterPast = DailySWC[ kk ]; // update the previous soil water content
            } // end of day loop
            if( ii == ( RunYears - 1 ) ) { // if we are in the last year of simulation then we also want to record the monthly values
                for( int kk = 0; kk < 30; kk++ ) // for each day in the month
                {
                    if( kk < 15 ) // if we are less than half way through the month then linearly interpolate from the previous month
                    {
                        TMP = MonthlyTemperatures[ PrevMonth ] + ( ( MonthlyTemperatures[ jj ] - MonthlyTemperatures[ PrevMonth ] ) / ( double )15 ) * ( double )kk;
                    } else // if we are more than half way through the month then linearly interpolate forwards
                    {
                        TMP = MonthlyTemperatures[ jj ] + ( ( MonthlyTemperatures[ NextMonth ] - MonthlyTemperatures[ jj ] ) / ( double )15 ) * ( double )( kk - 15 );
                    }
                    ActualEvapotranspiration[ jj ] += DailyAET[ kk ]; // Add up the actual evapotranspiration
                    SoilWater[ jj ] += DailySWC[ kk ]; // Add up the soil water contents (we'll take an average)
                    double SoilMoistureContent = 0;
                    if( AvailableWaterCapacity > 0 ) {
                        SoilMoistureContent = DailySWC[ kk ] / AvailableWaterCapacity;
                        if( TMP > 0 && SoilMoistureContent < SoilMoistureFireThreshold ) {
                            LengthOfFireSeason += ( exp( ( -acos( -1. ) ) * pow( ( ( DailySWC[ kk ] / AvailableWaterCapacity ) / 0.3 ), 2 ) ) ); // work out the length of the fire season
                        }
                    }
                }
            }
        }
    }
    return std::tuple< std::vector< double >, double, double >( ActualEvapotranspiration, SoilWaterPast, LengthOfFireSeason ); // return the collection of results
}

double ClimateVariablesCalculator::CalculatePotentialEvapotranspiration( double Temperature ) {
    double ps0 = 610.78; // The saturation vapour pressure at zero degrees C (pascals)
    double psa = 0.0; // The saturation vapour pressure at another temperature

    // This then predicts vapur pressure as a function of temperature,
    // With different functions depending on whether the temperaure is above or below 0 degrees C
    psa = ( Temperature < 0.0 ) ? exp( ( -6140.4 / ( 273 + Temperature ) ) + 28.916 ) : 610.78 * exp( ( Temperature / ( Temperature + 238.3 ) ) * 17.2694 );

    // The Potential Evapotranspiration is then approximated as a linear
    // function of the ratio of the saturation vapour pressure at the given temperature to the pressure at zero.
    return (( psa / ps0 ) * 25 );
}

double ClimateVariablesCalculator::GetNDF( std::vector<double>& monthlyFrostDays, std::vector<double>& monthlyTemperature, double missingValue ) {
    if( monthlyFrostDays[ 0 ] > missingValue ) {
        double NumMonthsFrost = 0; // will monitor the integrated number of frost months (a continuous variable)
        int prevmonth;
        int nextmonth;

        // We classify a complete "frost month" if we have more than 15 days in the month with frost 
        for( int jj = 0; jj < 12; jj++ ) {
            prevmonth = ( jj == 0 ) ? 11 : jj - 1;
            nextmonth = ( jj == 11 ) ? 0 : jj + 1;
            // We classify a complete "frost month" if we have more than 15 days in the month with frost 
            if( monthlyFrostDays[ jj ] > 15 ) {
                NumMonthsFrost++;
            }// However, if there are less than 15 days in the month with frost then we first of all
                // work out if that month came from a previous month with more than 15 frost days
                // If that is the case then we interpolate forwards and have a fraction of a month that is frost
            else if( monthlyFrostDays[ prevmonth ] > 15 ) {
                NumMonthsFrost += ( double )monthlyFrostDays[ jj ] / 15;

            }// Otherwise if there are more than 15 days frost in the next month then we are going into the winter season
                // and we make an interpolation
            else if( monthlyFrostDays[ nextmonth ] > 15 ) {
                NumMonthsFrost += ( double )monthlyFrostDays[ jj ] / 15;
            }
        }

        return NumMonthsFrost / 12; // convert to a fraction of a year
    } else {
        return ApproximateNDF( monthlyTemperature );
    }
}

double ClimateVariablesCalculator::ApproximateNDF( std::vector<double> MATData ) {
    int PrevMonth;
    int NextMonth;
    double NDF = 0;

    for( int jj = 0; jj < 12; jj++ ) {
        PrevMonth = ( jj == 0 ) ? 11 : jj - 1; // work out the index of the previous month
        NextMonth = ( jj == 11 ) ? 0 : jj + 1; // work out the index of the next month 
        double TempStart = ( MATData[ PrevMonth ] + MATData[ jj ] ) / 2.0;
        double TempEnd = ( MATData[ NextMonth ] + MATData[ jj ] ) / 2.0;
        if( TempStart < 0.0 ) {
            if( MATData[ jj ] < 0.0 ) {
                NDF += 15.0;
            } else {
                NDF += ( MATData[ jj ] / ( MATData[ jj ] - TempStart ) ) * 15;
            }
        } else {
            if( MATData[ jj ] < 0.0 ) {
                NDF += ( TempStart / ( TempStart - MATData[ jj ] ) ) * 15;
            }
        }
        if( MATData[ jj ] < 0.0 ) {
            if( TempEnd < 0.0 ) {
                NDF += 15.0;
            } else {
                NDF += ( TempEnd / ( TempEnd - MATData[ jj ] ) ) * 15;
            }
        } else {
            if( TempEnd < 0.0 ) {
                NDF += ( MATData[ jj ] / ( MATData[ jj ] - TempEnd ) ) * 15;
            }
        }
    }
    return NDF / 360;
}
