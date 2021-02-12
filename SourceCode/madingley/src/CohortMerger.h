#ifndef COHORTMERGE
#define COHORTMERGE

#include "Cohort.h"
#include "CohortPair.h"
#include "MadingleyInitialisation.h"
#include "NonStaticSimpleRNG.h"
#include "Parameters.h"
#include <set>

/** \brief Merges cohorts with similar properties */
class CohortMerger {
public:
    CohortMerger( );
    
    void ResetRandom( );

    /** \brief Calculate the distance between two cohorts in multi-dimensional trait space (body mass, adult mass, juvenile mass)
    @param Cohort1 The first cohort to calculate distance to 
    @param Cohort2 The cohort to compare to 
    @returns The relative distance in trait space */
    static double CalculateDistance( Cohort*, Cohort* );

    /** \brief Merge cohorts until below a specified threshold number of cohorts in each grid cell
    @param gridCellCohorts The cohorts within this grid cell 
    @param TotalNumberOfCohorts The total number of cohorts in this grid cell 
    @param TargetCohortThreshold The target threshold to reduce the number of cohorts to 
    @return The number of cohorts that have been merged */
    int MergeToReachThresholdFast( GridCell&, MadingleyInitialisation& );
    
    /** \brief An instance of simple random number generator class */
    NonStaticSimpleRNG mRandomNumber;
};

#endif
