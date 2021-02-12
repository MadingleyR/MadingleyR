#ifndef REPRODUCTIONBASIC
#define REPRODUCTIONBASIC

#include "Reproduction.h"
#include "Cohort.h"
#include "GridCell.h"
#include "NonStaticSimpleRNG.h"

#include <limits>
#include <assert.h>

/** \brief A formulation of the process of reproduction */
class ReproductionBasic : public Reproduction {
public:

    int UseNonDefaultModelParameters;
    std::vector<double> Reproduction_Parameters;


    /** \brief Constructor for reproduction: assigns all parameter values
    @param globalModelTimeStepUnit The time step of the global model 
    @param drawRandomly Indicates whether to draw values randomly */
    ReproductionBasic( std::string, bool );

    /** \brief Generate new cohorts from reproductive potential mass
    @param gridCell The current grid cell 
    @param currentTimestep The current model time step 
    @param partial Thread-locked variables 
    @param iteroparous Whether the acting cohort is iteroparous, as opposed to semelparous 
    @param currentMonth The current model month */
    void Run( GridCell&, Cohort*, unsigned, ThreadVariables&, bool, unsigned, MadingleyInitialisation& );

    /** \brief assigns ingested biomass from other ecological processes to reproductive potential mass
    @param gridCell The current grid cell 
    @param actingCohort The position of the acting cohort in the jagged array of grid cell cohorts 
    @param currentTimestep The current model time step 
    @param params The model parameters */
    void MassAssignment( GridCell&, Cohort*, unsigned, MadingleyInitialisation& );

    /** \brief Assign the juvenile and adult masses of the new cohort to produce
    @param actingCohort The position of the acting cohort in the jagged array of grid cell cohorts 
    @param madingleyCohortDefinitions The definitions of cohort functional groups in the model 
    @return A vector containing the juvenile and adult masses of the cohort to be produced*/
    std::vector< double > GetOffspringCohortProperties( Cohort*, FunctionalGroupDefinitions&, double );

private:
    /** \brief Include Utility class */
    UtilityFunctions mUtilities;
    /** \brief An instance of the simple random number generator class */
    NonStaticSimpleRNG mRandomNumber;
    /** \brief The time units associated with this implementation of dispersal */
    std::string mTimeUnitImplementation;
    /** \brief The per individual ratio of (adult body mass + reproductive potential mass) to adult body mass above which reproduction is possible*/
    double mMassRatioThreshold;
    /** \brief The probability that random draws above which result in offspring cohorts with 
    evolved juvenile and adult masses*/
    double mMassEvolutionProbabilityThreshold;
    /** \brief The standard deviation around the parent cohort's adult and juvenile masses to apply when drawing offspring
    adult and juvenile masses (when mass evolution occurs)*/
    double mMassEvolutionStandardDeviation;
    /** \brief The proportion of adult (non-reproductive) biomass allocated to offspring during a reproductive event by semelparous organisms*/
    double mSemelparityAdultMassAllocation;
    /** \brief Scalar to convert from the time step units used by this formulation of reproduction to global model time step units */
    double mDeltaT;
};
#endif
