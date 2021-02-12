#ifndef DISPERSALDIFFUSIVE
#define DISPERSALDIFFUSIVE

#include "Dispersal.h"
#include "UtilityFunctions.h"

/** \brief A formulation of the process of dispersal */
class DispersalDiffusive : public Dispersal {
public:


    int UseNonDefaultModelParameters;
    std::vector<double> Dispersal_Parameters;



    /** \brief Constructor for dispersal: assigns all parameter values */
    DispersalDiffusive( );

    /** \brief Run diffusive dispersal
    @param cellIndices List of indices of active cells in the model grid 
    @param gridForDispersal The model grid to run dispersal for 
    @param cohortToDisperse The cohort for which to run the dispersal process for 
    @param actingCohortFunctionalGroup The functional group index of the acting cohort 
    @param actingCohortNumber The position of the cohort within the functional group in the array of grid cell cohorts 
    @param currentMonth The current model month */
    void Run( Grid& gridForDispersal, Cohort* cohortToDisperse, const unsigned& currentMonth );

private:
    
    /** \brief Calculates the average diffusive dispersal speed of individuals in a cohort given their body mass
    @param bodyMass The current body mass of individuals in the cohort 
    @return The average dispersal speed, in km per month*/
    double CalculateDispersalSpeed( double );

    /** \brief Calculates the probability of diffusive dispersal given average individual dispersal speed
    @param madingleyGrid The model grid 
    @param C a cohort
    @param dispersalSpeed The average speed at which individuals in this cohort move around their environment, in km per month */
    void CalculateDispersalProbability( Grid&, Cohort*, double );
    
    /** \brief The time units associated with this implementation of dispersal */
    std::string mTimeUnitImplementation;
    /** \brief Scalar relating dispersal speed to individual body mass */
    double mDispersalSpeedBodyMassScalar;
    /** \brief Body-mass exponent of the relationship between disperal speed and individual body mass */
    double mDispersalSpeedBodyMassExponent;
    /** \brief Scalar to convert from the time step units used by this formulation of dispersal to global model time step units */
    double mDeltaT;
};
#endif
