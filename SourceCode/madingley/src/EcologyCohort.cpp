#include "EcologyCohort.h"

EcologyCohort::EcologyCohort( ) {
    // Declare and attach eating formulations
    EatingSet *EatingFormulation = new EatingSet( Parameters::Get( )->GetTimeStepUnits( ) );
    mEatingFormulations["Basic eating"] = EatingFormulation;
    // Declare and attach metabolism formulations
    MetabolismSet *MetabolismFormulation = new MetabolismSet( Parameters::Get( )->GetTimeStepUnits( ) );
    mMetabolismFormulations["Basic metabolism"] = MetabolismFormulation;
    // Declare and attach mortality formulations
    ReproductionSet *ReproductionFormulation = new ReproductionSet( Parameters::Get( )->GetTimeStepUnits( ), Parameters::Get( )->GetDrawRandomly( ) );
    mReproductionFormulations["Basic reproduction"] = ReproductionFormulation;
    // Declare and attach mortality formulations
    MortalitySet *MortalityFormulation = new MortalitySet( Parameters::Get( )->GetTimeStepUnits( ) );
    mMortalityFormulations["Basic mortality"] = MortalityFormulation;
}

void EcologyCohort::InitialiseEating( GridCell& gcl, MadingleyInitialisation& params ) {
    // Initialise eating formulations - has to be redone every step?
    mEatingFormulations["Basic eating"]->InitializeEcologicalProcess( gcl, params, "revised predation" );
    mEatingFormulations["Basic eating"]->InitializeEcologicalProcess( gcl, params, "revised herbivory" );
}

EcologyCohort::~EcologyCohort( ) {
    delete mEatingFormulations["Basic eating"];
    delete mMetabolismFormulations["Basic metabolism"];
    delete mMortalityFormulations["Basic mortality"];
    delete mReproductionFormulations["Basic reproduction"];
}

void EcologyCohort::RunWithinCellEcology( GridCell& gcl, Cohort* actingCohort, unsigned currentTimestep, ThreadVariables& partial, unsigned currentMonth,
  MadingleyInitialisation& params ) {
    // RUN EATING
    if( actingCohort->mIndividualBodyMass > 0 ) {


        mEatingFormulations["Basic eating"]->RunEcologicalProcess( gcl, actingCohort, currentTimestep, partial, currentMonth, params );
        // RUN METABOLISM - THIS TIME TAKE THE METABOLIC LOSS TAKING INTO ACCOUNT WHAT HAS BEEN INGESTED THROUGH EATING
        mMetabolismFormulations["Basic metabolism"]->RunEcologicalProcess( gcl, actingCohort, currentTimestep, partial, currentMonth, params );
        // RUN REPRODUCTION - TAKING INTO ACCOUNT NET BIOMASS CHANGES RESULTING FROM EATING AND METABOLISING
        mReproductionFormulations["Basic reproduction"]->RunEcologicalProcess( gcl, actingCohort, currentTimestep, partial, currentMonth, params );
        // RUN MORTALITY - TAKING INTO ACCOUNT NET BIOMASS CHANGES RESULTING FROM EATING, METABOLISM AND REPRODUCTION
        mMortalityFormulations["Basic mortality"]->RunEcologicalProcess( gcl, actingCohort, currentTimestep, partial, currentMonth, params );

        // if(currentTimestep==1){
        //     std::cout << "mass = " << actingCohort->mIndividualBodyMass << " loss (add/subtract) = " << 
        //                 Cohort::mMassAccounting["biomass"]["metabolism"] + 
        //                 Cohort::mMassAccounting["biomass"]["reproduction"] + 
        //                 Cohort::mMassAccounting["biomass"]["predation"] + 
        //                 Cohort::mMassAccounting["biomass"]["herbivory"] << 
        //                 " abundance = " << actingCohort->mCohortAbundance << " loss (ratio) = " <<
        //                 Cohort::mMassAccounting["abundance"]["mortality"] << std::endl;
        // }   

        // if(Cohort::mMassAccounting["abundance"]["mortality"]==0.0 && currentTimestep<10){
        //     std::cout << " Ind mass " << actingCohort->mIndividualBodyMass << " " <<  
        //     Cohort::mMassAccounting["biomass"]["metabolism"]   << " " <<  
        //     Cohort::mMassAccounting["biomass"]["reproduction"] << " " << 
        //     Cohort::mMassAccounting["biomass"]["predation"]    << " " << 
        //     Cohort::mMassAccounting["biomass"]["herbivory"]    << " " << actingCohort->mFunctionalGroupIndex << std::endl;
        // }
    }
}

void EcologyCohort::UpdateEcology( GridCell& gcl, Cohort* actingCohort, unsigned currentTimestep ) {
    // Apply the results of within-cell ecological processes
    mApplyEcologicalProcessResults.UpdateAllEcology( gcl, actingCohort, currentTimestep );
}
