#ifndef UTILITYFUNCTIONS
#define UTILITYFUNCTIONS

#include "NonStaticSimpleRNG.h"
#include "Parameters.h"
#include "Convertor.h"

#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <assert.h>
#include <iostream>

using namespace std;

class UtilityFunctions {
public:

    /** \brief If longitudinal cell coordinates run from 0 to 360, the convert to -180 to 180 values
    @param lons The longitudinal coorindates of the cells in the model grid */
    void ConvertToM180To180( std::vector<double>& lons );

    /** \brief Generate a random order in which cohorts will be subjected to ecological processes
    @param cohortNumber The number of cohorts in the current grid cell
    @return A vector of randomly ordered integers corresponding to the cohorts in the grid cell - will change with every run */
    std::vector<unsigned> RandomlyOrderedCohorts( unsigned cohortNumber );

    /** \brief Generate a non-random order in which cohorts will be subjected to ecological processes
    @param cohortNumber The number of cohorts in the current grid cell
    @param currentTimeStep The current time step of the model
    @return A vector of non-randomly ordered integers corresponding to the cohorts in the grid cell -will be systematic in some way... */
    std::vector<unsigned> NonRandomlyOrderedCohorts( unsigned cohortNumber, unsigned currentTimeStep );

    unsigned GetCurrentMonth( unsigned currentTimestep );

    /** \brief Calculates factors to convert between different time units
    @param fromUnit Time unit to convert from
    @param toUnit Time unit to convert to
    @return Factor to convert between time units */
    double ConvertTimeUnits( std::string fromUnit, std::string toUnit );

    /** \brief For a given cohort index, return a vector pair of values corresponding to the cohort's location in the jagged array of grid cell cohorts
    @param valueToFind The index of the cohort (values range between zero and the number of cohorts in the jagged arrray)
    @param arrayToSearch The jaggged array of cohorts, where rows correspond to functional groups, and columns to cohorts within functional groups
    @param totalNumberOfCohorts The total number of cohorts in the grid cell
    @return The position of the specified cohort in the jagged array of grid cell cohorts, where the first value is the row index (functional group) and the second value is the column index (position within functional group)</returns> */
    std::vector<int> FindJaggedArrayIndex( unsigned valueToFind, std::vector< std::vector < unsigned> > arrayToSearch, unsigned totalNumberOfCohorts );

    /** \brief Converts values per square km to per square degree, given cell latitude
    @param valueToConvert The value per square km
    @param latitude The latitude of the grid cell
    @return The specified value converted to per square degree */
    double ConvertSqMToSqDegrees( double valueToConvert, double latitude );

    /** \brief Calculates the probability of a particular value under a log-normal distribution with specified mean and standard deviation
    @param xValue The value to return the probability of under the log-normal distribtuion, in identity space
    @param meanIdentity The mean of the log-normal distribution, in identity space
    @param standardDeviation The standard deviation of the log-normal distribution, in log space
    @return The probability of the specified value under the specified log-normal distribution
     */
    double LogNormalPDF( double xValue, double meanIdentity, double standardDeviation );

    /** \brief Calculates the probability of a particular value under a normal distribution with specified mean and standard deviation
    @param xValue The value to return the probability of under the normal distribtuion
    @param meanValue The mean of the normal distribution
    @param standardDeviation The standard deviation of the normal distribution
    @return The probability of the specified value under the specified normal distribution */
    double NormalPDF( double xValue, double meanValue, double standardDeviation );

    /** \brief Calculate the area of a grid cell in square km, given its dimensions and geographical position
    @param latitude The latitude of the bottom-left corner of the grid cell
    @param lonCellSize The longitudinal dimension of the grid cell
    @param latCellSize The latitudinal dimension of the grid cell
    @return The area in square km of the grid cell */
    double CalculateGridCellArea( double latitude, double cellSize );

    /** \brief Calculate the length of a degree of latitude at a particular latitude /
    @param latitude The latitude of the bottom-left corner of the grid cell MB ?? This is not consistent with cellarea in the original code
    @return The length of a degree of latitude in kilometres */
    double CalculateLengthOfDegreeLatitude( float latitude );

    /** \brief Calculate the length of a degree of longitude at a particular latitude
    @param latitude The latitude of the bottom-left corner of the grid cell
    @return The length of a degree of longitude in kilometres MB ?? This is not consistent with cellarea in the original code */
    double CalculateLengthOfDegreeLongitude( float latitude );

    /** \brief Convert from degrees to radians
    @param degrees The value in degrees to convert
    @return The value converted to radians</returns> */
    double DegreesToRadians( double degrees );
};
#endif