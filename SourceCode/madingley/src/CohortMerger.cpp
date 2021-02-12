#include "CohortMerger.h"
#include "CohortPair.h"

CohortMerger::CohortMerger( ) {
    if( Parameters::Get( )->GetDrawRandomly( ) == true ) {
        unsigned seed = std::chrono::system_clock::now( ).time_since_epoch( ).count( );
        mRandomNumber.SetSeed( seed );
    } else {
        mRandomNumber.SetSeed( 4000 );
    }
}

void CohortMerger::ResetRandom( )
{
    if( Parameters::Get( )->GetDrawRandomly( ) == true )
    {
        unsigned seed = std::chrono::system_clock::now( ).time_since_epoch( ).count( );
        mRandomNumber.SetSeed( seed );
    } else
    {
        mRandomNumber.SetSeed( 4000 );
    }
}

double CohortMerger::CalculateDistance( Cohort* cohortA, Cohort* cohortB ) {
    double AdultMassDistance = ( cohortA->mAdultMass - cohortB->mAdultMass ) / cohortA->mAdultMass;
    double JuvenileMassDistance = ( cohortA->mJuvenileMass - cohortB->mJuvenileMass ) / cohortA->mJuvenileMass;
    double CurrentMassDistance = ( cohortA->mIndividualBodyMass - cohortB->mIndividualBodyMass ) / cohortA->mIndividualBodyMass;

    return ( ( AdultMassDistance * AdultMassDistance ) + ( JuvenileMassDistance * JuvenileMassDistance ) + ( CurrentMassDistance * CurrentMassDistance ) );
}

int CohortMerger::MergeToReachThresholdFast( GridCell& gcl, MadingleyInitialisation& params ) {
    // Set of lists of shortest distances in each functional group
    // set is automatically sorted - multiset allows for elements with the same distance
    std::multiset< CohortPair, CohortPair::Comparator > SortedDistances;
    // How many cohorts to remove to hit the threshold
    unsigned MergeCounter = 0;

    int NumberToRemove = gcl.GetNumberOfCohorts( ) - Parameters::Get( )->GetMaximumNumberOfCohorts( );

    if( NumberToRemove > 0 ) {
        //Loop through functional groups
        for( unsigned ff = 0; ff < gcl.mCohorts.size( ); ff++ ) {
            if( gcl.mCohorts[ff].size( ) > 1 ) {
                // Loop through cohorts within functional groups
                for( int cc = 0; cc < gcl.mCohorts[ff].size( ) - 1; cc++ ) {
                    // Loop through comparison cohorts
                    for( int dd = cc + 1; dd < gcl.mCohorts[ff].size( ); dd++ ) {
                        CohortPair PairwiseDistance( gcl.mCohorts[ff][cc], gcl.mCohorts[ff][dd], mRandomNumber.GetUniform( ) );
                        SortedDistances.insert( PairwiseDistance );
                    }
                }
            }
        }

        auto I = SortedDistances.begin( );
        while( MergeCounter < NumberToRemove && I != SortedDistances.end( ) ) {
            Cohort* CohortToMergeFrom = ( I->mCohortA );
            Cohort* CohortToMergeTo =   ( I->mCohortB );
 
            if( CohortToMergeFrom->mCohortAbundance > 0 && CohortToMergeTo->mCohortAbundance > 0 ) {
                // Add the abundance of the second cohort to that of the first
                CohortToMergeTo->mCohortAbundance += CohortToMergeFrom->mCohortAbundance * CohortToMergeFrom->mIndividualBodyMass / CohortToMergeTo->mIndividualBodyMass;
                // Add the reproductive potential mass of the second cohort to that of the first
                CohortToMergeTo->mIndividualReproductivePotentialMass += CohortToMergeFrom->mIndividualReproductivePotentialMass * CohortToMergeFrom->mCohortAbundance / CohortToMergeTo->mCohortAbundance;
                // Set the abundance of the second cohort to zero
                CohortToMergeFrom->mCohortAbundance = 0.0;
                // Designate both cohorts as having merged
                CohortToMergeTo->mMerged = true;
                CohortToMergeFrom->mMerged = true;
                MergeCounter++;
            }
            ++I;
        
        }
    }
    return MergeCounter;
}
