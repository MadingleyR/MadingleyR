#ifndef ECOLOGYCOHORT
#define ECOLOGYCOHORT

#include "EcologicalProcessWithinGridCell.h"
#include "Eating.h"
#include "EatingSet.h"
#include "ReproductionSet.h"
#include "MortalitySet.h"
#include "MetabolismSet.h"
#include "EcologyApply.h"

/** \brief A class to specify, initalise and run ecological processes pertaining to cohorts */
class EcologyCohort {
public:

    /** \brief Initalise the ecological processes */
    EcologyCohort( );

    void InitialiseEating( GridCell&, MadingleyInitialisation& );

    ~EcologyCohort( );

    /** \brief Run ecological processes that operate on cohorts within a single grid cell
    @param gcl The current grid cell
    @param actingCohort The acting cohort
    @param currentTimestep The current model time step
    @param partial Thread-locked local variables
    @param currentMonth The current model month
    @param params Things that may be needed */
    void RunWithinCellEcology( GridCell&, Cohort*, unsigned, ThreadVariables&, unsigned, MadingleyInitialisation& );

    /** \brief Update the properties of the acting cohort and of the environmental biomass pools after running the ecological processes for a cohort
    @param gridCell The current grid cell
    @param actingCohort The acting cohort
    @param currentTimestep The current model time step */
    void UpdateEcology( GridCell&, Cohort*, unsigned );

    /** \brief  A vector of stopwatch objects for timing the ecological processes*/
    std::vector< Stopwatch > mStopWatches;
    /** \brief A sorted list of formulations of metabolism */
    std::map< std::string, EcologicalProcessWithinGridCell* > mMetabolismFormulations;
    /** \brief A sorted list of formulations of eating */
    std::map< std::string, EcologicalProcessWithinGridCell* > mEatingFormulations;
    /** \brief A sorted list of formulations of mortality */
    std::map< std::string, EcologicalProcessWithinGridCell* > mMortalityFormulations;
    /** \brief A sorted list of formulations of reproduction */
    std::map< std::string, EcologicalProcessWithinGridCell* > mReproductionFormulations;
    /** \brief An instance of apply ecology */
    EcologyApply mApplyEcologicalProcessResults;
};
#endif
