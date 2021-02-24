#include "EatingHerbivory.h"
#include "InputParameters.h"

EatingHerbivory::EatingHerbivory( std::string globalModelTimeStepUnit ) {
    mTimeUnitImplementation = "Day";
    mHandlingTimeScalarTerrestrial = 0.7; //EatingHerbivory_Parameters[0]
    mHandlingTimeScalarMarine = 0.7; //EatingHerbivory_Parameters[1]
    mHandlingTimeExponentTerrestrial = 0.7; //EatingHerbivory_Parameters[2]
    mHandlingTimeExponentMarine = 0.7; //EatingHerbivory_Parameters[3]
    mReferenceMass = 1.0; //EatingHerbivory_Parameters[4]
    mHerbivoryRateConstant = 1.0E-11; //EatingHerbivory_Parameters[5]
    mHerbivoryRateMassExponent = 1.0; //EatingHerbivory_Parameters[6]
    mAttackRateExponentTerrestrial = 2.1;  //EatingHerbivory_Parameters[7]
    mProportionEdibleStockMass = 0.1;  //EatingHerbivory_Parameters[8]

    UseNonDefaultModelParameters = InputParameters::Get( )->GetUseNonDefaultModelParameters();

    if(UseNonDefaultModelParameters==1){
        EatingHerbivory_Parameters = InputParameters::Get( )->Get_EatingHerbivory_Parameters();
        mHandlingTimeScalarTerrestrial = EatingHerbivory_Parameters[0];
        mHandlingTimeScalarMarine = EatingHerbivory_Parameters[1];
        mHandlingTimeExponentTerrestrial = EatingHerbivory_Parameters[2];
        mHandlingTimeExponentMarine = EatingHerbivory_Parameters[3];
        mReferenceMass = EatingHerbivory_Parameters[4];
        mHerbivoryRateConstant = EatingHerbivory_Parameters[5];
        mHerbivoryRateMassExponent = EatingHerbivory_Parameters[6];
        mAttackRateExponentTerrestrial = EatingHerbivory_Parameters[7];
        mProportionEdibleStockMass = EatingHerbivory_Parameters[8];
    }

    mAttackRateExponentMarine = 2.0;
    // Calculate the scalar to convert from the time step units used by this implementation of herbivory to the global model time step units
    mDeltaT = mUtilities.ConvertTimeUnits( globalModelTimeStepUnit, mTimeUnitImplementation );

    mCellArea = 0;
    mCellAreaHectares = 0;
    mBodyMassHerbivore = 0;
    mEdibleMass = 0;
    mEdibleScaling = 0;
    mInstantFractionEaten = 0;

    // From base class
    mAssimilationEfficiency = 0;
    mProportionTimeEating = 0;
    mTimeUnitsToHandlePotentialFoodItems = 0;
    mTotalBiomassEatenByCohort = 0;
}

EatingHerbivory::~EatingHerbivory( ) {
    for( auto& B: mBiomassesEaten )B.clear( );
    for( auto& P: mPotentialBiomassesEaten )P.clear( );
}

void EatingHerbivory::InitializeEatingPerTimeStep( GridCell& gcl, MadingleyInitialisation& params ) {

    // Store the specified cell area in this instance of this herbivory implementation
    mCellArea = gcl.GetCellArea( );
    mCellAreaHectares = mCellArea * 100;
    // Get the functional group indices of all autotroph stocks
    mFunctionalGroupIndicesToEat = params.mStockFunctionalGroupDefinitions.GetFunctionalGroupIndex( "Heterotroph/Autotroph", "Autotroph", false );
}

void EatingHerbivory::GetEatingPotentialTerrestrial( GridCell& gcl, Cohort* actingCohort, MadingleyInitialisation& params ) {
    // Set the total biomass eaten by the acting cohort to zero
    mTotalBiomassEatenByCohort = 0.0;

    // Get the individual body mass of the acting cohort
    mBodyMassHerbivore = actingCohort->mIndividualBodyMass;


    // Set the total number of units to handle all potential biomass eaten to zero
    mTimeUnitsToHandlePotentialFoodItems = 0.0;

    // Initialise the jagged arrays to hold the potential and actual biomass eaten in each of the grid cell autotroph stocks
    mBiomassesEaten.resize( gcl.mStocks.size( ) );
    mPotentialBiomassesEaten.resize( gcl.mStocks.size( ) );

    // Loop over rows in the jagged arrays and initialise each vector
    for( int i = 0; i < gcl.mStocks.size( ); i++ ) {
        mBiomassesEaten[i].resize( gcl.mStocks[i].size( ) );
        mPotentialBiomassesEaten[i].resize( gcl.mStocks[i].size( ) );
    }
    

    // Loop over functional groups that can be eaten
    for( int FunctionalGroup: mFunctionalGroupIndicesToEat ) {
        // Loop over stocks within the functional group
        for( int i = 0; i < gcl.mStocks[FunctionalGroup].size( ); i++ ) {
            // Get the mass from this stock that is available for eating (assumes only 10% is edible)
            mEdibleMass = (gcl.mStocks[FunctionalGroup][i].mTotalBiomass * mProportionEdibleStockMass);

            // Calculate the potential biomass eaten from this stock by the acting cohort
            mPotentialBiomassesEaten[FunctionalGroup][i] = CalculatePotentialBiomassEatenTerrestrial( mEdibleMass, mBodyMassHerbivore, actingCohort, params );

            // Add the time required to handle the potential biomass eaten from this stock to the cumulative total for all stocks
            mTimeUnitsToHandlePotentialFoodItems += mPotentialBiomassesEaten[FunctionalGroup][i] * CalculateHandlingTimeTerrestrial( mBodyMassHerbivore );

        }

    }




}

void EatingHerbivory::Run( GridCell& gcl, Cohort* actingCohort, unsigned currentTimestep, MadingleyInitialisation& params ) {
    
    mEdibleScaling = 1.0;
    if( !gcl.IsMarine( ) ) mEdibleScaling = mProportionEdibleStockMass;

    // Loop over autotroph functional groups that can be eaten
    for( int FunctionalGroup: mFunctionalGroupIndicesToEat ) {
        // Loop over stocks within the functional groups
        for( int i = 0; i < gcl.mStocks[FunctionalGroup].size( ); i++ ) {
            // Get the mass from this stock that is available for eating (assumes only 10% is edible in the terrestrial realm)
            mEdibleMass = (gcl.mStocks[FunctionalGroup][i].mTotalBiomass * mEdibleScaling);

            // Calculate the biomass actually eaten from this stock by the acting cohort
            mBiomassesEaten[FunctionalGroup][i] = CalculateBiomassesEaten( mPotentialBiomassesEaten[FunctionalGroup][i], mTimeUnitsToHandlePotentialFoodItems, actingCohort->mCohortAbundance, mEdibleMass );

            // Remove the biomass eaten from the autotroph stock
            gcl.mStocks[FunctionalGroup][i].mTotalBiomass -= mBiomassesEaten[FunctionalGroup][i];

            // Check that the biomass eaten is not a negative value
            if( mBiomassesEaten[FunctionalGroup][i] < 0 ) {
                std::cout << "Herbivory negative for this herbivore cohort " << actingCohort->mFunctionalGroupIndex << " " << actingCohort->mID << std::endl;
                exit( 1 );
            }
            // Add the biomass eaten and assimilated by an individual to the delta biomass for the acting cohort
            //MB should we be able to get here if abundance is zero?
            if( actingCohort->mCohortAbundance > 0 )Cohort::mMassAccounting["biomass"]["herbivory"] += mBiomassesEaten[FunctionalGroup][i] * mAssimilationEfficiency / actingCohort->mCohortAbundance;

            // Move the biomass eaten but not assimilated by an individual into the organic matter pool
            Cohort::mMassAccounting["organicpool"]["herbivory"] += mBiomassesEaten[FunctionalGroup][i] * ( 1 - mAssimilationEfficiency );

        }

        // Check that the delta biomass from eating for the acting cohort is not negative
        assert( Cohort::mMassAccounting["biomass"]["herbivory"] >= 0 && "Delta biomass from herbviory is negative" );

        // Calculate the total biomass eaten by the acting (herbivore) cohort
        mTotalBiomassEatenByCohort = Cohort::mMassAccounting[ "biomass" ][ "herbivory" ] * actingCohort->mCohortAbundance;
        
        // track cohort consumption
        //##############
        actingCohort->mConsumed_Autotroph = mTotalBiomassEatenByCohort;
        //##############
    }
}

double EatingHerbivory::CalculatePotentialBiomassEatenTerrestrial( double autotrophBiomass, double herbivoreIndividualMass, Cohort* c, MadingleyInitialisation& params ) {
    // Calculate the inidividual herbivory rate per unit autotroph mass-density per hectare
    double IndividualHerbivoryRate = CalculateIndividualHerbivoryRatePerHectare( herbivoreIndividualMass );

    // Calculate autotroph biomass density per hectare
    double AutotrophBiomassDensity = (autotrophBiomass / (mCellAreaHectares * 1)); //## mProportionEdibleStockMass;

    // Return PotentialBiomassEatenTerrestrial     
    
    //## Omnivores lower mAttackRateExponentTerrestrial
    if(params.mCohortFunctionalGroupDefinitions.GetTraitNames( "Nutrition source", c->mFunctionalGroupIndex ) == "omnivore"){
        return IndividualHerbivoryRate * pow( AutotrophBiomassDensity, ( mAttackRateExponentTerrestrial) ); // -0.3
    }else{
        return IndividualHerbivoryRate * pow( AutotrophBiomassDensity, mAttackRateExponentTerrestrial );
    }
}

double EatingHerbivory::CalculatePotentialBiomassEatenMarine( double autotrophBiomass, double herbivoreIndividualMass ) {
    // Calculate the inidividual herbivory rate per unit autotroph mass-density per hectare
    double IndividualHerbivoryRate = CalculateIndividualHerbivoryRatePerHectare( herbivoreIndividualMass );

    // Calculate autotroph biomass density per hectare
    double AutotrophBiomassDensity = (autotrophBiomass / (mCellAreaHectares * 1)); //## mProportionEdibleStockMass;

    // Calculate the expected autotroph biomass eaten
    return IndividualHerbivoryRate * pow( AutotrophBiomassDensity, mAttackRateExponentMarine );
}

/** \brief Calculate the herbivory rate of an individual herbivore per unit autotroph mass-density per hectare
@param herbivoreIndividualMass Herbivore individual body mass
@return The herbivory rate of an individual herbivore per unit autotroph mass-density per hectare*/
double EatingHerbivory::CalculateIndividualHerbivoryRatePerHectare( double herbivoreIndividualMass ) {
    // Calculate the individual herbivory rate
    return mHerbivoryRateConstant * pow( herbivoreIndividualMass, ( mHerbivoryRateMassExponent ) );

}

/** \brief Calculate the time taken for a herbivore in the marine realm to handle unit mass (1 g) of autotroph mass
@param herbivoreIndividualMass The body mass of an individual herbivore
@return The time taken for a herbivore to handle unit mass (1 g) of autotroph mass*/
double EatingHerbivory::CalculateHandlingTimeMarine( double herbivoreIndividualMass ) {
    return mHandlingTimeScalarMarine * pow( ( mReferenceMass / herbivoreIndividualMass ), mHandlingTimeExponentMarine );
}

/** \brief Calculate the time taken for a herbivore in the terrestrial realm to handle unit mass (1 g) of autotroph mass
@param herbivoreIndividualMass The body mass of an individual herbivore
@return The time taken for a herbivore to handle unit mass (1 g) of autotroph mass*/
double EatingHerbivory::CalculateHandlingTimeTerrestrial( double herbivoreIndividualMass ) {
    return mHandlingTimeScalarTerrestrial * pow( ( mReferenceMass / herbivoreIndividualMass ), mHandlingTimeExponentTerrestrial );
}

/** \brief Calculate the actual biomass eaten by a herbivore cohort from an autotroph stock
@param potentialBiomassEaten The potential biomass eaten by the herbivore cohort from the autotroph stock given the encounter rate
@param totalHandlingTime The total time that would be taken to handle all encountered autotroph biomass in all autotroph stocks
@param herbivoreAbundance The number of individuals in the acting herbivore cohort
@param autotrophBiomass The total biomass in the autotroph stock
@return The biomass eaten by the herbivore cohort from the autotroph stock*/
double EatingHerbivory::CalculateBiomassesEaten( double potentialBiomassEaten, double totalHandlingTime, double herbivoreAbundance, double autotrophBiomass ) {
    // Check whether there is any biomass in the autotroph stock
    if( autotrophBiomass > 0.0 ) {
        // Calculate the instantaneous fraction of the autotroph stock eaten
        mInstantFractionEaten = herbivoreAbundance * ( ( potentialBiomassEaten / ( 1 + totalHandlingTime ) ) / autotrophBiomass );
    } else {
        // Set the instantaneous fraction of the autotroph stock eaten to zero
        mInstantFractionEaten = 0.0;
    }

    // Return the total  biomass of the autotroph stock eaten
    return autotrophBiomass * ( 1 - exp( -mInstantFractionEaten * mDeltaT * mProportionTimeEating ) );
}




void EatingHerbivory::GetEatingPotentialMarine( GridCell& gcl, Cohort* actingCohort, MadingleyInitialisation& params ) {
    // Set the total biomass eaten by the acting cohort to zero
    mTotalBiomassEatenByCohort = 0.0;

    // Get the individual body mass of the acting cohort
    mBodyMassHerbivore = actingCohort->mIndividualBodyMass;

    // Set the total number of units to handle all potential biomass eaten to zero
    mTimeUnitsToHandlePotentialFoodItems = 0.0;

    // Initialise the jagged arrays to hold the potential and actual biomass eaten in each of the grid cell autotroph stocks
    mBiomassesEaten.resize( gcl.mStocks.size( ) );
    mPotentialBiomassesEaten.resize( gcl.mStocks.size( ) );

    // Loop over rows in the jagged arrays and initialise each vector
    for( int i = 0; i < gcl.mStocks.size( ); i++ ) {
        mBiomassesEaten[i].resize( gcl.mStocks[i].size( ) );
        mPotentialBiomassesEaten[i].resize( gcl.mStocks[i].size( ) );
    }

	if(mBodyMassHerbivore>(1)) mFunctionalGroupIndicesToEat = {1,3};	

    // Loop over functional groups that can be eaten
    for( int FunctionalGroup: mFunctionalGroupIndicesToEat ) {
        // Loop over stocks within the functional group
        for( int i = 0; i < gcl.mStocks[FunctionalGroup].size( ); i++ ) {
            // Get the mass from this stock that is available for eating (assumes all marine autotrophic organisms are edible)
            //EdibleMass = gridCellStocks[FunctionalGroup][i].TotalBiomass * 0.1; //MB weird line
            mEdibleMass = gcl.mStocks[FunctionalGroup][i].mTotalBiomass;
            // Calculate the potential biomass eaten from this stock by the acting cohort
            mPotentialBiomassesEaten[FunctionalGroup][i] = CalculatePotentialBiomassEatenMarine( mEdibleMass, mBodyMassHerbivore );

            // Add the time required to handle the potential biomass eaten from this stock to the cumulative total for all stocks
            mTimeUnitsToHandlePotentialFoodItems += mPotentialBiomassesEaten[FunctionalGroup][i] * CalculateHandlingTimeMarine( mBodyMassHerbivore );
        }
    }
}
