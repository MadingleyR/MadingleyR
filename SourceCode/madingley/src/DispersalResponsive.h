#ifndef DISPERSALRESPONSIVE
#define DISPERSALRESPONSIVE

#include "Dispersal.h"
#include "UtilityFunctions.h"

/** \brief A formulation of the process of responsive dispersal */
class DispersalResponsive : public Dispersal {
public:

    int UseNonDefaultModelParameters;
    std::vector<double> Dispersal_Parameters;


    /** \brief Assigns all parameter values for repsonsive dispersal */
    DispersalResponsive( );

    /** \brief Run responsive dispersal
    @param cellIndices The longitudinal and latitudinal indices of the current grid cell 
    @param gridForDispersal The model grid to run dispersal for 
    @param cohortToDisperse The cohort for which to apply the dispersal process 
    @param actingCohortFunctionalGroup The functional group index of the acting cohort 
    @param actingCohortNumber The position of the acting cohort within the functional group in the array of grid cell cohorts 
    @param currentMonth The current model month */
    void Run( Grid&, Cohort*, const unsigned& );

private:
    bool CheckStarvationDispersal( Grid&, Cohort* );
    void CheckDensityDrivenDispersal( Grid&, Cohort* );

    /** \brief Calculate the average diffusive dispersal speed of individuals in a cohort given their body mass
    @param bodyMass The current body mass of an individual in the cohort 
    @return The (average) dispersal speed in kilometres per month */
    double CalculateDispersalSpeed( double );

    /** \brief   Calculates the probability of responsive dispersal given average individual dispersal speed and grid cell
    @param madingleyGrid The model grid
    @param c the dispersing cohort
    @param dispersalSpeed The average dispersal speed of individuals in the acting cohort */
    void CalculateDispersalProbability( Grid&, Cohort*, double );

    /** \brief The time units associated with this implementation of dispersal */
    std::string mTimeUnitImplementation;
    /** \brief Density threshold below which adult individuals may move to look for other adults of the same cohort
    \remarks The density scales with cohort weight via: Min Density = DensityThresholdScaling / MatureMass (g) */
    double mDensityThresholdScaling;
    /** \brief Scalar relating dispersal speed to individual body mass */
    double mDispersalSpeedBodyMassScalar;
    /** \brief Body-mass exponent of the relationship between disperal speed and individual body mass */
    double mDispersalSpeedBodyMassExponent;
    /** \brief The proportion of body mass loss at which the cohort will try to disperse every time during a time step */
    double mStarvationDispersalBodyMassThreshold;
    /** \brief Scalar to convert from the time step units used by this formulation of dispersal to global model time step units */
    double mDeltaT;
};
#endif
