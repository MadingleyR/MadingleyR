#ifndef MORTALITYBACKGROUND
#define MORTALITYBACKGROUND

#include "UtilityFunctions.h"
#include "Mortality.h"

/** \brief A formulation of the process of background mortality, i.e. mortality from disease, accidents and other random events*/
class MortalityBackground : public Mortality {

public:
    /** \brief Constructor for background mortality: assigns all parameter values*/
    MortalityBackground( std::string );

    /** \brief Calculate the rate of individuals in a cohort that die from background mortality in a model time step
    @param actingCohort The position of the acting cohort in the jagged array of grid cell cohorts 
    @param currentTimestep The current model time step 
    @return The rate of individuals in the cohort that die from background mortality */
    double CalculateMortalityRate( Cohort*, double, unsigned );


    int UseNonDefaultModelParameters;
    std::vector<double> Mortality_Parameters;

private:
    /** \brief Include Utility class */
    UtilityFunctions mUtilities;
    /** \brief The time units associated with this implementation of dispersal*/
    std::string mTimeUnitImplementation;
    /** \brief Cohort background mortality rate - the proportion of individuals dying in a time step */
    double mMortailtyRate;
    /** \brief Scalar to convert from the time step units used by this formulation of dispersal to global model time step units */
    double mDeltaT;
};
#endif
