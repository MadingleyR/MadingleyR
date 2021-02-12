#ifndef DISPERSALADVECTIVE
#define DISPERSALADVECTIVE

#include "Dispersal.h"
#include "UtilityFunctions.h"
//#include "Environment.h"
#include "Parameters.h"
#include "Types.h"

#include <assert.h>

/** \brief A formulation of the process of dispersal */
class DispersalAdvective : public Dispersal {
public:
    /** \brief Constructor for dispersal: assigns all parameter values */
    DispersalAdvective( );

    /** \brief Run advective dispersal
    @param cellIndex The longitudinal and latitudinal indices of the focal grid cell 
    @param gridForDispersal The model grid to run dispersal for 
    @param cohortToDisperse The cohort to run dispersal for 
    @param actingCohortFunctionalGroup The functional group index of the acting cohort 
    @param actingCohortNumber The position of the acting cohort within the functional group in the array of grid cell cohorts 
    @param currentMonth The current model month */
    void Run( Grid&, Cohort*, const unsigned& );
    
private:
    /** \brief    Convert dispersal speed from m per second to km per dispersal time step (currently 18h)
    @param dispersalSpeed The dispersal speed in m per second 
    @return The dispersal speed in kilometres per time step*/
    inline const double RescaleDispersalSpeed( const double& ) const;
    
    /** \brief    Calculates the probability of advective dispersal given the grid cell
    @param madingleyGrid The model grid 
    @param c a cohort */
    void CalculateDispersalProbability( Grid&, Cohort*, const unsigned& );
    
    /** \brief    Get a randomly directed diffusion vector. This is derived from the LTRANS model formulation, which itself is derived from Visser 1997 (MEPS)
    We assume that the standard deviation of the random draw is 1.0
    @return A two element array, where the first element is the diffusion component in the u direction, and the second component is the
    diffusion component in the v direction*/
    Types::DoubleVector CalculateDiffusion( );
    
    /** \brief Include Utility class */
    //UtilityFunctions mUtilities;
    /** \brief The time units associated with this implementation of dispersal*/
    std::string mTimeUnitImplementation;
    /** \brief The horizontal diffusivity parameter (m^2/s) */
    double mHorizontalDiffusivity;
    /** \brief The length of the time-step for advective dispersal, in hours*/
    unsigned mAdvectiveModelTimeStepLengthHours;
    /** \brief Horizontal diffusivity in km^2/advective-dispersal-time-step*/
    double mHorizontalDiffusivityKmSqPerADTimeStep;
    /** \brief Time unit scalar to apply to advective dispersal*/
    double mAdvectionTimeStepsPerModelTimeStep;
    /** \brief Factor to convert velocity from m/s to km/month*/
    double mVelocityUnitConversion;
    /** \brief Scalar to convert from the time step units used by this formulation of dispersal to global model time step units */
    double mDeltaT;
};

#endif
