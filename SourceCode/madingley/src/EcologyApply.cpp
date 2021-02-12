#include "EcologyApply.h"
#include "Cohort.h"

#include <assert.h>

void EcologyApply::UpdateAllEcology( GridCell& gcl, Cohort* actingCohort, unsigned currentTimestep ) {
    // Apply cohort abundance changes
    UpdateAbundance( gcl, actingCohort );
    // Apply cohort biomass changes
    UpdateBiomass( gcl, actingCohort, currentTimestep );
    // Apply changes to the environmental biomass pools
    UpdatePools( gcl );
}

void EcologyApply::UpdateAbundance( GridCell& gcl, Cohort* actingCohort ) {
    // Variable to calculate net abundance change to check that cohort abundance will not become negative
    double NetAbundanceChange = 0.0;
    // Loop over all abundance deltas
    for( auto& d: Cohort::mMassAccounting["abundance"] ) {
        // Update net abundance change
        NetAbundanceChange += d.second;
    }
    //std::cout << "ab change = "<< NetAbundanceChange << std::endl;
    
    // Check that cohort abundance will not become negative
    assert( NetAbundanceChange >= 0 && "Cohort abundance < 0" );
    //this is a variation from the original below -
    //the abundance here only changes in Mortality.h where the change was mortalityTotal=(1-exp(-sum-of-Mortalities))*abundance
    //this is now change to mortalityTotal=exp(-sum-of-mortalities) - avoiding possibilities of negatives arising.
    
    // test
    // if(NetAbundanceChange!=0.0){
    //   actingCohort->mCohortAbundance *= NetAbundanceChange;
    // }
    
    actingCohort->mCohortAbundance *= NetAbundanceChange;
    //original code
    //Loop over all keys in the abundance deltas sorted list
    //for( auto& d: Cohort::mMassFluxes["abundance"] ) {
    //    // Update the abundance of the acting cohort
    //  actingCohort->mCohortAbundance += d.second;
    //}
}

void EcologyApply::UpdateBiomass( GridCell& gcl, Cohort* actingCohort, unsigned currentTimestep ) {
    // Variable to calculate net biomass change to check that cohort individual body mass will not become negative
    double NetBiomass = 0.0;

    // Loop over all biomass deltas
    for( auto& d: Cohort::mMassAccounting["biomass"] ) {
        // Update net biomass change
        NetBiomass += d.second;
    }
    double BiomassCheck = 0.0;
    bool NetToBeApplied = true;
    // If cohort abundance is greater than zero, then check that the calculated net biomass will not make individual body mass become negative
    if( actingCohort->mCohortAbundance > 0 ) {

        BiomassCheck = actingCohort->mIndividualBodyMass + NetBiomass;
        if( BiomassCheck < 0 ) {
            std::cout << "Biomass going negative, acting cohort: " << actingCohort->mFunctionalGroupIndex << ", " << actingCohort->mID << std::endl;
            exit( 1 );
        }
    }

    //Loop over all keys in the abundance deltas sorted list
    for( auto& d: Cohort::mMassAccounting["biomass"] ) {
        // If cohort abundance is zero, then set cohort individual body mass to zero and reset the biomass delta to zero,
        // otherwise update cohort individual body mass and reset the biomass delta to zero
        if( actingCohort->mCohortAbundance == 0 ) {
            actingCohort->mIndividualBodyMass = 0.0;
        } else {
            if( NetToBeApplied ) {
                actingCohort->mIndividualBodyMass = actingCohort->mIndividualBodyMass + NetBiomass;
                NetToBeApplied = false;
            }
        }
    }
    // Check that individual body mass is still greater than zero
    assert( actingCohort->mIndividualBodyMass >= 0 && "biomass < 0" );

    // If the current individual body mass is the largest that has been achieved by this cohort, then update the maximum achieved
    // body mass tracking variable for the cohort
    if( actingCohort->mIndividualBodyMass > actingCohort->mMaximumAchievedBodyMass )
        actingCohort->mMaximumAchievedBodyMass = actingCohort->mIndividualBodyMass;

    // Variable to calculate net reproductive biomass change to check that cohort individual body mass will not become negative
    double NetReproductiveBiomass = 0.0;

    // Loop over all reproductive biomass deltas
    for( auto& d: Cohort::mMassAccounting["reproductivebiomass"] ) {
        // Update net reproductive biomass change
        NetReproductiveBiomass += d.second;
    }

    //Loop over all keys in the abundance deltas sorted list
    for( auto& d: Cohort::mMassAccounting["reproductivebiomass"] ) {
        // If cohort abundance is zero, then set cohort reproductive body mass to zero and reset the biomass delta to zero,
        // otherwise update cohort reproductive body mass and reset the biomass delta to zero
        if( actingCohort->mCohortAbundance == 0 ) {
            actingCohort->mIndividualReproductivePotentialMass = 0.0;
        } else {

            actingCohort->mIndividualReproductivePotentialMass += d.second;
        }
    }
    //Note that maturity time step is set in TReproductionBasic
}

void EcologyApply::UpdatePools( GridCell& gcl ) {
    // Loop over all keys in the organic pool deltas sorted list
    for( auto &D: Cohort::mMassAccounting["organicpool"] ) {
        // Check that the delta value is not negative
        //if( D.second < 0 ) std::cout << "organic pool " << D.first << " " << D.second << std::endl;

        //assert(D.second >= 0.0 && "A delta value for the organic pool is negative " );
        // Update the organic pool biomass
        gcl.mGridcellOrganicPool += D.second; //Environment::Get( "Organic Pool", gcl ) += D.second;
        //Reset the delta value to zero
    }
    // Loop over all keys in the respiratory pool deltas sorted list
    for( auto &D: Cohort::mMassAccounting["respiratoryCO2pool"] ) {
        // Check that the delta value is not negative
        assert( D.second >= 0.0 && "A delta value for the respiratory CO2 pool is negative" );
        // Update the respiratory CO2 pool
        gcl.mGridcellRespiratoryCO2Pool += D.second; //Environment::Get( "Respiratory CO2 Pool", gcl ) 
        // Reset the delta value to zero // FIX - Should there be a declaration here?
    }
}
