#include "MetabolismHeterotroph.h"
#include "InputParameters.h"
//#include "Environment.h"


MetabolismHeterotroph::MetabolismHeterotroph( std::string globalModelTimeStepUnit ) {
    mTimeUnitImplementation = "second";
    // Paramters from Brown's (2004) Metabolic Theory of Ecology
    mMetabolismMassExponent = 0.71; //MetabolismHeterotroph_Parameters[0]
    mNormalizationConstant = exp( 20 );
    mActivationEnergy = 0.69; //MetabolismHeterotroph_Parameters[1]
    mBoltzmannConstant = 8.617e-5; //MetabolismHeterotroph_Parameters[2]

    UseNonDefaultModelParameters = InputParameters::Get( )->GetUseNonDefaultModelParameters();

    if(UseNonDefaultModelParameters==1){
        MetabolismHeterotroph_Parameters = InputParameters::Get( )->Get_MetabolismHeterotroph_Parameters();
        mMetabolismMassExponent = MetabolismHeterotroph_Parameters[0];
        mActivationEnergy = MetabolismHeterotroph_Parameters[1];
        mBoltzmannConstant = MetabolismHeterotroph_Parameters[2];
    }

    // Currently a very rough estimate based on calorific values of fat, protein and carbohydrate
    mEnergyScalar = 1.0 / 20000.0;
    // Set the constant to convert temperature in degrees Celsius to Kelvin
    mTemperatureUnitsConvert = 273.0;
    // Calculate the scalar to convert from the time step units used by this implementation of metabolism to the global  model time step units
    mDeltaT = mUtilities.ConvertTimeUnits( globalModelTimeStepUnit, mTimeUnitImplementation );
}

void MetabolismHeterotroph::Run( Cohort* actingCohort, unsigned currentTimestep, unsigned currentMonth ) {
    // Calculate metabolic loss for an individual and add the value to the delta biomass for metabolism
    //Cohort::mMassAccounting[ "biomass" ][ "metabolism" ] = -CalculateIndividualMetabolicRate( actingCohort->mIndividualBodyMass, Environment::Get( "Temperature", actingCohort->GetCurrentCell( ) ) + mTemperatureUnitsConvert ) * mDeltaT;
    Cohort::mMassAccounting[ "biomass" ][ "metabolism" ] = -CalculateIndividualMetabolicRate( actingCohort->mIndividualBodyMass, 
                                                                                              actingCohort->GetGridcellTemperature( currentMonth ) + 
                                                                                              mTemperatureUnitsConvert ) * mDeltaT;
    
    // If metabolic loss is greater than individual body mass after herbivory and predation, then set equal to individual body mass

    Cohort::mMassAccounting[ "biomass" ][ "metabolism" ] = std::max( Cohort::mMassAccounting[ "biomass" ][ "metabolism" ], -( actingCohort->mIndividualBodyMass + Cohort::mMassAccounting[ "biomass" ][ "predation" ] + Cohort::mMassAccounting[ "biomass" ][ "herbivory" ] ) );

    // Add total metabolic loss for all individuals in the cohort to delta biomass for metabolism in the respiratory CO2 pool
    Cohort::mMassAccounting[ "respiratoryCO2pool" ][ "metabolism" ] = -Cohort::mMassAccounting[ "biomass" ][ "metabolism" ] * actingCohort->mCohortAbundance;
}

double MetabolismHeterotroph::CalculateIndividualMetabolicRate( double individualBodyMass, double temperature ) {
    // Calculate metabolic loss in kJ
    double MetabolicLosskJ = mNormalizationConstant * pow( individualBodyMass, mMetabolismMassExponent ) * exp( -( mActivationEnergy / ( mBoltzmannConstant * temperature ) ) );

    // Return metabolic loss in grams
    return MetabolicLosskJ * mEnergyScalar;
}

