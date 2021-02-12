#ifndef MORTALITYSENESCENCE
#define MORTALITYSENESCENCE

#include "Mortality.h"
#include "UtilityFunctions.h"
#include "Parameters.h"

/** \brief A formulation of the process of senescence mortality*/
class MortalitySenescence : public Mortality {
public:
    /** \brief Constructor for senscence mortality: assigns all parameter values */
    MortalitySenescence( std::string globalModelTimeStepUnit );

    /** \brief Calculate the rate of individuals in a cohort that die from senescence mortality in a model time step 
    @param actingCohort The position of the acting cohort in the jagged array of grid cell cohorts 
    @param currentTimestep The current model time step 
    @return The rate of individuals in the cohort that die from senescence mortality*/
    double CalculateMortalityRate( Cohort*, double, unsigned );

    int UseNonDefaultModelParameters;
    std::vector<double> Mortality_Parameters;

private:
    /** \brief Include Utility class */
    UtilityFunctions mUtilities;
    /** \brief The time units associated with this implementation of dispersal*/
    std::string mTimeUnitImplementation;
    /** \brief Cohort senescence mortality rate scalar: the rate of individuals dying in a time step when they reach maturity */
    double mMortalityRate;
    /** \brief Scalar to convert from the time step units used by this formulation of dispersal to global model time step units */
    double mDeltaT;
};
#endif
