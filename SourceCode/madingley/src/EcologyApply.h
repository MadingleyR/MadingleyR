#ifndef ECOLOGYAPPLY
#define ECOLOGYAPPLY

#include "GridCell.h"

/** \brief Class for applying changes from the ecological processes to the properties of the acting cohort and to the environment */
class EcologyApply {
public:
    /** \brief  Apply all updates from the ecological processes to the properties of the acting cohort and to the environment
    @param gcl The gridCell 
    @param actingCohort The acting cohort 
    @param currentTimestep The current model time step */
    void UpdateAllEcology( GridCell&, Cohort*, unsigned );

    /** \brief  Update the abundance of the acting cohort according to the delta abundances from the ecological processes
    @param gcl The current grid cell 
    @param actingCohort The acting cohort */
    void UpdateAbundance( GridCell&, Cohort* );
    
    /** \brief  Update the individual and reproductive body masses of the acting cohort according to the delta biomasses from the ecological processes
    @param gridCell The current grid cell 
    @param actingCohort The acting cohort  
    @param currentTimestep The current model time step */
    void UpdateBiomass( GridCell&, Cohort*, unsigned );
    
    /** \brief Update the organic and respiratory biomass pools according to the relevant deltas from the ecological processes
    @param gcl The current grid cell */
    void UpdatePools( GridCell& );
};
#endif
