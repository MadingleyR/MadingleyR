#ifndef CLIMATEVARIABLESCALCULATOR
#define CLIMATEVARIABLESCALCULATOR

#include <vector>
#include <cmath>
#include <tuple>

/** \brief Calculates derived climate variables for which no input data exist */
class ClimateVariablesCalculator {
public:

    /** \brief Constructor the climate variables calculator */
    ClimateVariablesCalculator( );

    /** \brief Calculates monthly water balance variables: actual evapotranspiration, soil water deficit,
    and the an approximation for the length of the fire season.
    Actual Evapotranspiration and soil moisture was calculated by following Prentice et al (1993)
    "A simulation model for the transient effects of climate change on forest landscapes",
    Ecological Modelling, 65, 51-70, but using potential evapotranspiration rates calculated elsewhere
    (normally the Penman Monteith equation).
    The approximate length of the fire season was calculated using equations (2) and (4) of 
    Thonicke et al. (2001). "The role of fire disturbance for global vegetation dynamics: coupling
    fire into a Dynamic Global Vegetation Model". Global Ecology and Biogeography, 10, 661-677.
    @param AvailableWaterCapacity The available water capacity of the soil (mm) 
    @param Precipitation Mean monthly precipitation (mm) 
    @param MonthlyTemperatures Mean monthly temperatures, degrees celcius 
    @return A Tuple containing i) monthly actual evapotranspiration (mm), ii) soil water deficit (mm) and, iii) fire season length (between 0 and 360 days) */
    std::tuple< std::vector<double>, double, double> MonthlyActualEvapotranspirationSoilMoisture( double, std::vector<double>, std::vector<double> );

    /** \brief Calculates the monthly potential evapotranspiration according to
    Malmstrom VH (1969) A new approach to the classification of climate. J Geog 68:351–357.
    @param Temperature Mean monthly temperature, degrees Celsius 
    @return Potential Monthly Evapotranspiration, mm */
    double CalculatePotentialEvapotranspiration( double Temperature );

    /** \brief Estimates the fraction of the year in which the temperature drops below zero at some time in the day
    according to the the CRU CL 2.0 gridded climate dataset (For details of this dataset see CRU2p0Dataset.txt)
    @param monthlyFrostDays A vector containing the number of frost days each month 
    @param monthlyTemperature A vector containing average temperatures for each month 
    @param missingValue The missing value used in the the environmental datasets 
    @return The fraction of the year in which temperature drops below zero at some point in the day */
    double GetNDF( std::vector<double>&, std::vector<double>&, double );

    /** \brief Calculates the number of days frost using an alternative algorithm to that in ClimateLookup
    that is based on mean annual temperature data alone. This will probably be a coarse representation
    of the number of frost days but will do for now.
    @param MATData Mean monthly temperatures, degrees celcius 
    @return Fraction of the year that experiences frost */
    double ApproximateNDF( std::vector<double> MATData );
};
#endif
