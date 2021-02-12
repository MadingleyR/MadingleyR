#ifndef ECOLOGICALPROCESSWITHINGRIDCELL
#define ECOLOGICALPROCESSWITHINGRIDCELL

#include "MadingleyInitialisation.h"
#include "ThreadVariables.h"

/** \brief Interface for ecological process code */
class EcologicalProcessWithinGridCell {
public:

    /** \brief Run the ecological process
    @param gridCell The current grid cell
    @param actingCohort The  acting cohort
    @param currentTimestep The current model time step
    @param partial Thread-locked variables
    @param currentMonth The current model month
    @param params The instance of the MadingleyModelInitialisation class for this simulation */
    virtual void RunEcologicalProcess( GridCell&, Cohort*, unsigned, ThreadVariables&, unsigned, MadingleyInitialisation& ) {
        std::cout << "Top level EcologicalWithinGridCell RunEcologicalProcess process called: should be virtual so this is probably not what you want!" << std::endl;
    }

    /** \brief Initialises an implementation of the ecological process
    @param gridCell The current grid cell
    @params params parameters for the model
    @param implementationKey The name of the specific implementation of this process to initialize */
    virtual void InitializeEcologicalProcess( GridCell&, MadingleyInitialisation&, std::string ) {
        ;
    }

    /** \brief Virtual destructor */
    virtual ~EcologicalProcessWithinGridCell( ) {
        ;
    }
};

#endif
