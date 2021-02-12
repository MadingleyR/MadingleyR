#include "MortalitySet.h"

MortalitySet::MortalitySet( std::string globalModelTimeStepUnit ) {
    // Add the background mortality implementation to the list of implementations
    MortalityBackground* BackgroundMortalityImplementation = new MortalityBackground( globalModelTimeStepUnit );
    mImplementations[ "basic background mortality" ] = BackgroundMortalityImplementation;

    // Add the senescence mortality implementation to the list of implementations
    MortalitySenescence* SenescenceMortalityImplementation = new MortalitySenescence( globalModelTimeStepUnit );
    mImplementations[ "basic senescence mortality" ] = SenescenceMortalityImplementation;

    // Add the starvation mortality implementation to the list of implementations
    MortalityStarvation* StarvationMortalityImplementation = new MortalityStarvation( globalModelTimeStepUnit );
    mImplementations[ "basic starvation mortality" ] = StarvationMortalityImplementation;

}

MortalitySet::~MortalitySet( ) {
    delete mImplementations[ "basic background mortality" ];
    delete mImplementations[ "basic senescence mortality" ];
    delete mImplementations[ "basic starvation mortality" ];
}

void MortalitySet::InitializeEcologicalProcess( GridCell& gcl, MadingleyInitialisation& params, std::string implementationKey ) {

}

void MortalitySet::RunEcologicalProcess( GridCell& gcl, Cohort* actingCohort, unsigned currentTimestep, ThreadVariables& partial,
  unsigned currentMonth, MadingleyInitialisation& params ) {
    // Variables to hold the mortality rates
    double MortalityRateBackground;
    double MortalityRateSenescence;
    double MortalityRateStarvation;

    // Variable to hold the total abundance lost to all forms of mortality
    double MortalityTotal;

    // Individual body mass including change this time step as a result of other ecological processes
    double BodyMassIncludingChangeThisTimeStep;

    // Individual reproductive mass including change this time step as a result of other ecological processes
    double ReproductiveMassIncludingChangeThisTimeStep;

    // Calculate the body mass of individuals in this cohort including mass gained through eating this time step, up to but not exceeding adult body mass for this cohort.
    // Should be fine because these deductions are made in the reproduction implementation, but use Math.Min to double check.

    BodyMassIncludingChangeThisTimeStep = 0.0;
    // Loop over all items in the biomass deltas
    for( auto Biomass: Cohort::mMassAccounting[ "biomass" ] ) {
        // Add the delta biomass to net biomass
        BodyMassIncludingChangeThisTimeStep += Biomass.second;
    }

    BodyMassIncludingChangeThisTimeStep = std::min( actingCohort->mAdultMass, BodyMassIncludingChangeThisTimeStep + actingCohort->mIndividualBodyMass );

    // Temporary variable to hold net reproductive biomass change of individuals in this cohort as a result of other ecological processes
    ReproductiveMassIncludingChangeThisTimeStep = 0.0;

    // Loop over all items in the biomass Cohort::Deltas
    for( auto Biomass: Cohort::mMassAccounting[ "reproductivebiomass" ] ) {
        // Add the delta biomass to net biomass
        ReproductiveMassIncludingChangeThisTimeStep += Biomass.second;
    }

    ReproductiveMassIncludingChangeThisTimeStep += actingCohort->mIndividualReproductivePotentialMass;
    // Check to see if the cohort has already been killed by predation etc
    if( BodyMassIncludingChangeThisTimeStep < 1.e-7 ) {
        //MB a small number ! maybe should be larger? (e.g. min cohort body mass))
        //This causes a difference between C# and c++ versions as there is a rounding error issue - changed in C# code to match
        // If individual body mass is not greater than zero, then all individuals become extinct
        MortalityTotal = 0.0;
        // used to be actingCohort->mCohortAbundance, but this leads to a large cancellation in applyEcology
        //so mortality total is changed here  - now we only multiply by mortality total - so values can never be negative
        //BodyMassIncludingChangeThisTimeStep = 0; //MB would be a kludge to exclude negative values below - need mass checking throughout the code
    } else {
        // Calculate background mortality rate
        MortalityRateBackground = mImplementations[ "basic background mortality" ]->CalculateMortalityRate( actingCohort, BodyMassIncludingChangeThisTimeStep, currentTimestep );

        // If the cohort has matured, then calculate senescence mortality rate, otherwise set rate to zero
        if( actingCohort->mIsAdult == 1 ) {
            MortalityRateSenescence = mImplementations[ "basic senescence mortality" ]->CalculateMortalityRate( actingCohort, BodyMassIncludingChangeThisTimeStep, currentTimestep );
        } else {
            MortalityRateSenescence = 0.0;
        }
        
        // Calculate the starvation mortality rate based on individual body mass and maximum body mass ever
        // achieved by this cohort
        MortalityRateStarvation = mImplementations[ "basic starvation mortality" ]->CalculateMortalityRate( actingCohort, BodyMassIncludingChangeThisTimeStep, currentTimestep );

        // Calculate the number of individuals that suffer mortality this time step from all sources of mortality
        MortalityTotal = exp( -MortalityRateBackground - MortalityRateSenescence - MortalityRateStarvation );
        // std::cout << "ratio dead MortalityRateBackground = " << MortalityRateBackground << std::endl;
        // std::cout << "ratio dead MortalityRateSenescence = " << MortalityRateSenescence << std::endl;
        // std::cout << "ratio dead MortalityRateStarvation = " << MortalityRateStarvation << std::endl;
        // std::cout << "ratio dead MortalityTotal = " << MortalityTotal << std::endl;
    }

    // Remove individuals that have died from the delta abundance for this cohort
    Cohort::mMassAccounting[ "abundance" ][ "mortality" ] = MortalityTotal;

    // Add the biomass of individuals that have died to the delta biomass in the organic pool (including reproductive
    // potential mass, and mass gained through eating, and excluding mass lost through metabolism)
    Cohort::mMassAccounting[ "organicpool" ][ "mortality" ] = ( 1 - MortalityTotal ) * actingCohort->mCohortAbundance * ( BodyMassIncludingChangeThisTimeStep + ReproductiveMassIncludingChangeThisTimeStep );
}
