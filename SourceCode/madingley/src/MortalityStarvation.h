#ifndef MORTALITYSTARVATION
#define MORTALITYSTARVATION

#include "Mortality.h"
#include "UtilityFunctions.h"

/** \brief A formulation of the process of starvation mortality */
class MortalityStarvation : public Mortality {
public:
    /** \brief Constructor for starvation mortality: assigns all parameter values */
    MortalityStarvation( std::string globalModelTimeStepUnit );

    /** \brief Calculate the proportion of individuals in a cohort that die from starvation mortality each time step
    @param actingCohort The position of the acting cohort in the jagged array of grid cell cohorts 
    @param bodyMassIncludingChangeThisTimeStep Body mass including change from other ecological functions this time step; should not exceed adult mass 
    @param currentTimestep The current model time step 
    @return The proportion of individuals in the cohort that die from starvation mortality */
    double CalculateMortalityRate( Cohort*, double, unsigned );

    int UseNonDefaultModelParameters;
    std::vector<double> Mortality_Parameters;
    
private:

    /** \brief Calculates the rate of starvation mortality given current body mass and the maximum body mass ever achieved. Note that metabolic costs are already included in the deltas passed in
    the body mass including change this time step, so no change in body mass should mean no starvation (as metabolic costs have already been met)
    @param actingCohort The position of the acting cohort in the jagged array of grid cell cohorts 
    @param bodyMassIncludingChangeThisTimeStep Body mass including change from other ecological functions this time step; should not exceed adult mass 
    @return The starvation mortality rate in mortality formulation time step units */
    double CalculateStarvationRate( Cohort*, double );

    /** \brief Include Utility class */
    UtilityFunctions mUtilities;
    /** \brief The time units associated with this implementation of dispersal */
    std::string mTimeUnitImplementation;;
    /** \brief The inflection point of the curve describing the relationship between body mass and mortality rate */
    double mLogisticInflectionPoint;
    /** \brief The steepness of the curve describing the relationship between body mass and mortality rate */
    double mLogisticScalingParameter;
    /** \brief The asymptote of the curve describing the relationship between body mass and mortality rate */
    double mMaximumStarvationRate;
    /** \brief Scalar to convert from the time step units used by this formulation of dispersal to global model time step units */
    double mDeltaT;
};
#endif
