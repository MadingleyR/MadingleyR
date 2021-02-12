#ifndef REPRODUCTIONSET
#define REPRODUCTIONSET

#include "EcologicalProcessWithinGridCell.h"
#include "ReproductionBasic.h"

/** \brief Performs reproduction */
class ReproductionSet : public EcologicalProcessWithinGridCell {
public:
    /** \brief The available implementations of the reproduction process */
    std::map< std::string, Reproduction* > mImplementations;

    /**  \brief Constructor for Reproduction: fills the list of available implementations of reproduction */
    ReproductionSet( std::string, bool );

    /** Destructor ensure we tidy everything up */
    ~ReproductionSet( );

    /** \brief Initialize an implementation of reproduction. This is only in here to satisfy the requirements of IEcologicalProcessWithinGridCells
    @param gcl The current grid cell
    @param params The definitions for functional groups in the model, among other things
    @param implementationKey The name of the reproduction implementation to initialize */
    void InitializeEcologicalProcess( GridCell& gcl, MadingleyInitialisation& params, std::string implementationKey ) ;

    /** \brief Run reproduction
    @param gcl The current grid cell
    @param actingCohort The acting cohort
    @param currentTimeStep The current model time step
    @param partial Thread-locked variables for the parallelised version
    @param currentMonth The current model month
    @params Parameters defined here */
    void RunEcologicalProcess( GridCell&, Cohort*, unsigned, ThreadVariables&, unsigned, MadingleyInitialisation& );
};
#endif
