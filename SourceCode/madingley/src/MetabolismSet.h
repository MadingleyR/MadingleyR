#ifndef METABOLISMSET
#define METABOLISMSET

#include "EcologicalProcessWithinGridCell.h"
#include "Metabolism.h"
#include "MetabolismHeterotroph.h"
#include "MetabolismEndotherm.h"
#include "MetabolismEctotherm.h"
#include "ThreadVariables.h"

/** \brief  Performs metabolism */
class MetabolismSet : public EcologicalProcessWithinGridCell {
public:
    /** \brief Constructor Metabolism: fills the list of available implementations of metabolism*/
    MetabolismSet( std::string globalModelTimeStepUnit );

    /** Destrcutor to tidy up pointers */
    ~MetabolismSet( );

private:
    /** \brief Initializes an implementation of metabolism
    @param gcl The current grid cell
    @param params A bunch of parameters 'n' stuff 'n' things
    @param implementationKey The name of the implementation of metabolism to initialize  */
    void InitializeEcologicalProcess( GridCell&, MadingleyInitialisation&, std::string );

    /** \brief Run metabolism
    @param gcl The current grid cell
    @param actingCohort The acting cohort
    @param currentTimestep The current model time step
    @param partial Thread-locked variables
    @param currentMonth The current model month
    @param params some parameters  */
    void RunEcologicalProcess( GridCell&, Cohort*, unsigned, ThreadVariables&, unsigned, MadingleyInitialisation& );

    std::map< std::string, Metabolism* > Implementations;
};
#endif
