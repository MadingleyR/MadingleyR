#ifndef EATINGSET
#define EATINGSET

#include "Eating.h"
#include "EcologicalProcessWithinGridCell.h"
#include "EatingCarnivory.h"
#include "EatingHerbivory.h"

/** \brief Performs eating */
class EatingSet : public EcologicalProcessWithinGridCell {
public:

    /** \brief Constructor for Eating: fills the list of available implementations of eating */
    EatingSet( std::string );

    /** \brief Destructor to remove pointer storage */
    ~EatingSet( );

    /** \briefInitializes an implementation of eating
    @param gcl The current grid cell
    @param params The model parameter set
    @param implementationKey The name of the implementation of eating to initialize
    \remarks Eating needs to be initialized every time step */

    void InitializeEcologicalProcess( GridCell&, MadingleyInitialisation&, std::string );
    /** \brief Run eating
    @param gcl The current grid cell
    @param actingCohort The position of the acting cohort in the jagged array of grid cell cohorts
    @param currentTimestep The current model time step
    @param partial Thread-locked variables
    @param currentMonth The current model month
    @params params The Params */
    void RunEcologicalProcess( GridCell&, Cohort*, unsigned, ThreadVariables&, unsigned, MadingleyInitialisation& );

    /** \brief The available implementations of the eating process */
    std::map< std::string, Eating* > mImplementations;
    /** \brief Tracks the total time to handle all potential food for omnivores */
    double mTotalTimeToEatForOmnivores;
};
#endif
