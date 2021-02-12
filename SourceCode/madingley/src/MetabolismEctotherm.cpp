#include "MetabolismEctotherm.h"
#include "InputParameters.h"
#include <algorithm>

MetabolismEctotherm::MetabolismEctotherm( std::string globalModelTimeStepUnit ) {
    mTimeUnitImplementation = "day";
    
    // Parameters from fitting to Nagy 1999 Field Metabolic Rates for reptiles - assumes that reptile FMR was measured with animals at their optimal temp of 30degC
    mMetabolismMassExponent = 0.88; // MetabolismEctotherm_Parameters[0]
    mNormalizationConstant = 148984000000; // MetabolismEctotherm_Parameters[1]
    mActivationEnergy = 0.69; // includes endotherms in hibernation and torpor // MetabolismEctotherm_Parameters[2]
    mBoltzmannConstant = 8.617e-5; // MetabolismEctotherm_Parameters[3]
    // BMR normalisation constant from Brown et al 2004 - original units of J/s so scale to kJ/d 
    mNormalizationConstantBMR = 41918272883; //exp( 20 )*60 * 60 * 24 / 1000; // MetabolismEctotherm_Parameters[4]
    mBasalMetabolismMassExponent = 0.69; // MetabolismEctotherm_Parameters[5]
    // Currently a very rough estimate based on calorific values of fat, protein and carbohydrate - assumes organism is metabolising mass of 1/4 protein, 1/4 carbohydrate and 1/2 fat 
    mEnergyScalar = 0.036697248; //1 / 27.25; // MetabolismEctotherm_Parameters[6]

    UseNonDefaultModelParameters = InputParameters::Get( )->GetUseNonDefaultModelParameters();

    if(UseNonDefaultModelParameters==1){
        MetabolismEctotherm_Parameters = InputParameters::Get( )->Get_MetabolismEctotherm_Parameters();
        mMetabolismMassExponent = MetabolismEctotherm_Parameters[0];
        mNormalizationConstant = MetabolismEctotherm_Parameters[1];
        mActivationEnergy = MetabolismEctotherm_Parameters[2];
        mBoltzmannConstant = MetabolismEctotherm_Parameters[3];
        mNormalizationConstantBMR = MetabolismEctotherm_Parameters[4];
        mBasalMetabolismMassExponent = MetabolismEctotherm_Parameters[5];
        mEnergyScalar = MetabolismEctotherm_Parameters[6];
    }

    // Set the constant to convert temperature in degrees Celsius to Kelvin
    mTemperatureUnitsConvert = 273.0;    
    // Calculate the scalar to convert from the time step units used by this implementation of metabolism to the global  model time step units
    mDeltaT = mUtilities.ConvertTimeUnits( globalModelTimeStepUnit, mTimeUnitImplementation );
    mProportionTimeActiveCalculatedThisTimestep = false;
}

void MetabolismEctotherm::Run( Cohort* actingCohort, unsigned currentTimestep, unsigned currentMonth ) {
    // Calculate metabolic loss for an individual and add the value to the delta biomass for metabolism
    //Cohort::mMassAccounting["biomass"]["metabolism"] = -CalculateIndividualMetabolicRate( actingCohort->mIndividualBodyMass, Environment::Get( "Temperature", actingCohort->GetCurrentCell( ) ) + mTemperatureUnitsConvert, actingCohort->mProportionTimeActive ) * mDeltaT;
    Cohort::mMassAccounting["biomass"]["metabolism"] = -CalculateIndividualMetabolicRate( actingCohort->mIndividualBodyMass, 
                                                                                          actingCohort->GetGridcellTemperature( currentMonth ) + mTemperatureUnitsConvert, 
                                                                                          actingCohort->mProportionTimeActive ) * mDeltaT;

    // If metabolic loss is greater than individual body mass after herbivory and predation, then set equal to individual body mass
    Cohort::mMassAccounting["biomass"]["metabolism"] = std::max( Cohort::mMassAccounting["biomass"]["metabolism"], -( actingCohort->mIndividualBodyMass + Cohort::mMassAccounting["biomass"]["predation"] + Cohort::mMassAccounting["biomass"]["herbivory"] ) );

    // Add total metabolic loss for all individuals in the cohort to delta biomass for metabolism in the respiratory CO2 pool
    Cohort::mMassAccounting["respiratoryCO2pool"]["metabolism"] = -Cohort::mMassAccounting["biomass"]["metabolism"] * actingCohort->mCohortAbundance;
}


double MetabolismEctotherm::CalculateIndividualMetabolicRate( double individualBodyMass, double temperature, double proportionTimeActive ) {
    // Calculate field metabolic loss in kJ
    double FieldMetabolicLosskJ = mNormalizationConstant * pow( individualBodyMass, mMetabolismMassExponent ) * exp( -( mActivationEnergy / ( mBoltzmannConstant * temperature ) ) );
    double BasalMetabolicLosskJ = mNormalizationConstantBMR * pow( individualBodyMass, mBasalMetabolismMassExponent ) * exp( -( mActivationEnergy / ( mBoltzmannConstant * temperature ) ) );

    // Return metabolic loss in grams
    return (( proportionTimeActive * FieldMetabolicLosskJ ) + ( ( 1 - proportionTimeActive ) * ( BasalMetabolicLosskJ ) ) ) * mEnergyScalar;
}
