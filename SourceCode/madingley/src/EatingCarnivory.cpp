#include "EatingCarnivory.h"
#include "InputParameters.h"

EatingCarnivory::EatingCarnivory( std::string globalModelTimeStepUnit ) {
    mTimeUnitImplementation = "Day";
    mHandlingTimeScalarTerrestrial = 0.5; //EatingCarnivory_Parameters[0]
    mHandlingTimeExponentTerrestrial = 0.7; //EatingCarnivory_Parameters[1]
    mHandlingTimeScalarMarine = 0.5; //EatingCarnivory_Parameters[2]
    mHandlingTimeExponentMarine = 0.7; //EatingCarnivory_Parameters[3]
    mReferenceMass = 1.0; //EatingCarnivory_Parameters[4]
    mKillRateConstant = 1E-6; //EatingCarnivory_Parameters[5]
    mKillRateConstantMassExponent = 1.0; //EatingCarnivory_Parameters[6]
    mFeedingPreferenceStandardDeviation = 0.7; //EatingCarnivory_Parameters[7]
    mRelativeMaxPreyRatioOmnivores = 0.1; //EatingCarnivory_Parameters[8]

    UseNonDefaultModelParameters = InputParameters::Get( )->GetUseNonDefaultModelParameters();
    
    if(UseNonDefaultModelParameters==1){
        EatingCarnivory_Parameters = InputParameters::Get( )->Get_EatingCarnivory_Parameters();
        mHandlingTimeScalarTerrestrial = EatingCarnivory_Parameters[0];
        mHandlingTimeExponentTerrestrial = EatingCarnivory_Parameters[1];
        mHandlingTimeScalarMarine = EatingCarnivory_Parameters[2];
        mHandlingTimeExponentMarine = EatingCarnivory_Parameters[3];
        mReferenceMass = EatingCarnivory_Parameters[4];
        mKillRateConstant = EatingCarnivory_Parameters[5];
        mKillRateConstantMassExponent = EatingCarnivory_Parameters[6];
        mFeedingPreferenceStandardDeviation = EatingCarnivory_Parameters[7];
        mRelativeMaxPreyRatioOmnivores = EatingCarnivory_Parameters[8];
    }
    
    mNumberOfBins = 12;

    // Calculate the scalar to convert from the time step units used by this implementation of predation to the global model time step units
    mDeltaT = mUtilities.ConvertTimeUnits( globalModelTimeStepUnit, mTimeUnitImplementation );

    mSpecificPredatorHandlingTimeScaling = 0;
    mSpecificPredatorKillRateConstant = 0;
    mOptimalPreyPredatorMassRatioTerrestrial = 0;
    mOptimalPreyPredatorMassRatioMarine = 0;
    mRelativeFeedingPreference = 0;
    mSpecificPredatorTimeUnitsEatingPerGlobalTimeStep = 0;
    mPreyDensityPerHectare = 0;
    mAlphaIJ = 0;
    mCellArea = 0;
    mCellAreaHectares = 0;
    mPredatorAssimilationEfficiency = 0;
    mPredatorNonAssimilation = 0;
    mDietIsAllSpecial = 0;
    mPredatorLogOptimalPreyBodySizeRatio = 0;
    mBodyMassPrey = 0;
	mAdultBodyMassPrey = 0;
    mBodyMassPredator = 0;
    mAbundancePredator = 0;
    mReferenceMassRatioScalingTerrestrial = 0;
    mReferenceMassRatioScalingMarine = 0;
    mPredatorAbundanceMultipliedByTimeEating = 0;
    mTemporaryValue = 0;
    mPreyMassBinNumber = 0;
    mTrackIndividualCohorts = false;

    // From base class
    mAssimilationEfficiency = 0;
    mProportionTimeEating = 0;
    mTimeUnitsToHandlePotentialFoodItems = 0;
    mTotalBiomassEatenByCohort = 0;
}

EatingCarnivory::~EatingCarnivory( ) {
    for( auto& A: mAbundancesEaten )A.clear( );
    for( auto& P: mPotentialAbundanceEaten )P.clear( );
    for( auto& b: mBinnedPreyDensities )b.clear( );
}

void EatingCarnivory::InitializeEatingPerTimeStep( GridCell& gcl, MadingleyInitialisation& params ) {
    // Store the specified cell area in this instance of this herbivory implementation
    mCellArea = gcl.GetCellArea( );
    mCellAreaHectares = mCellArea * 100;
    //Get the functional group indices of all heterotroph cohorts (i.e. potential prey)
    mFunctionalGroupIndicesToEat = params.mCohortFunctionalGroupDefinitions.GetFunctionalGroupIndex( "Heterotroph/Autotroph", "heterotroph", false );

    // Initialise the vector to hold the number of cohorts in each functional group at the start of the time step
    mNumberCohortsPerFunctionalGroupNoNewCohorts.resize( gcl.mCohorts.size( ) );

    // Initialise the jagged arrays to hold the potential and actual numbers of prey eaten in each of the grid cell cohorts
    mAbundancesEaten.resize( gcl.mCohorts.size( ) );
    mPotentialAbundanceEaten.resize( gcl.mCohorts.size( ) );
    // Initialise the vector to identify carnivore cohorts
    mCarnivoreFunctionalGroups.resize( mFunctionalGroupIndicesToEat.size( ) );
    mOmnivoreFunctionalGroups.resize( mFunctionalGroupIndicesToEat.size( ) );
    mPlanktonFunctionalGroups.resize( mFunctionalGroupIndicesToEat.size( ) );

    // Loop over rows in the jagged arrays, initialise each vector within the jagged arrays, and calculate the current number of cohorts in
    // each functional group
    for( int i = 0; i < gcl.mCohorts.size( ); i++ ) {
        // Calculate the current number of cohorts in this functional group
        int NumCohortsThisFG = gcl.mCohorts[i].size( );
        mNumberCohortsPerFunctionalGroupNoNewCohorts[i] = NumCohortsThisFG;
        // Initialise the jagged arrays
        mAbundancesEaten[i].resize( mNumberCohortsPerFunctionalGroupNoNewCohorts[i] );
        for( auto& a: mAbundancesEaten[i] )a = 0;
        mPotentialAbundanceEaten[i].resize( mNumberCohortsPerFunctionalGroupNoNewCohorts[i] );
        for( auto& p: mPotentialAbundanceEaten[i] )p = 0;
    }

    // Loop over functional groups that are potential prey and determine which are carnivores
    for( int FunctionalGroup: mFunctionalGroupIndicesToEat )
        mCarnivoreFunctionalGroups[FunctionalGroup] = params.mCohortFunctionalGroupDefinitions.GetTraitNames( "Nutrition source", FunctionalGroup ) == "carnivore";

    for( int FunctionalGroup: mFunctionalGroupIndicesToEat )
        mOmnivoreFunctionalGroups[FunctionalGroup] = params.mCohortFunctionalGroupDefinitions.GetTraitNames( "Nutrition source", FunctionalGroup ) == "omnivore";

    for( int FunctionalGroup: mFunctionalGroupIndicesToEat )
        mPlanktonFunctionalGroups[FunctionalGroup] = params.mCohortFunctionalGroupDefinitions.GetTraitNames( "Mobility", FunctionalGroup ) == "planktonic";
}

void EatingCarnivory::GetEatingPotentialMarine( GridCell& gcl, Cohort* actingCohort, MadingleyInitialisation& params ) {
    mBinnedPreyDensities.resize( gcl.mCohorts.size( ) );
    for( auto& b: mBinnedPreyDensities )b.resize( mNumberOfBins );
    for( auto& b: mBinnedPreyDensities )for( auto& bin: b )bin = 0;
    // Set the total eaten by the acting cohort to zero
    mTotalBiomassEatenByCohort = 0.0;

    // Set the total number of units to handle all potential prey individuals eaten to zero
    mTimeUnitsToHandlePotentialFoodItems = 0.0;

    // Get the individual body mass of the acting (predator) cohort
    mBodyMassPredator = actingCohort->mIndividualBodyMass;

    // Get the abundance of the acting (predator) cohort
    mAbundancePredator = actingCohort->mCohortAbundance;

    // Pre-calculate individual values for this predator to speed things up
    mSpecificPredatorKillRateConstant = mKillRateConstant * pow( mBodyMassPredator, ( mKillRateConstantMassExponent ) );
    mSpecificPredatorTimeUnitsEatingPerGlobalTimeStep = mDeltaT * mProportionTimeEating;
    mPredatorAssimilationEfficiency = mAssimilationEfficiency;
    mPredatorNonAssimilation = ( 1 - mAssimilationEfficiency );

    mDietIsAllSpecial = params.mCohortFunctionalGroupDefinitions.GetTraitNames( "Diet", actingCohort->mFunctionalGroupIndex ) == "allspecial";

    mPredatorLogOptimalPreyBodySizeRatio = actingCohort->mLogOptimalPreyBodySizeRatio;

    // If a filter feeder, then optimal body size is a value not a ratio: convert it to a ratio to ensure that all calculations work correctly
    if( mDietIsAllSpecial ) {
        // Optimal body size is actually a value, not a ratio, so convert it to a ratio based on the present body size
        mPredatorLogOptimalPreyBodySizeRatio = log( exp( actingCohort->mLogOptimalPreyBodySizeRatio ) / actingCohort->mIndividualBodyMass ); //actingCohort->LogOptimalPreyBodySizeRatio-log(actingCohort->IndividualBodyMass);
    }

    // Calculate the reference mass scaling ratio
    mReferenceMassRatioScalingMarine = mHandlingTimeScalarMarine * pow( mReferenceMass / mBodyMassPredator, mHandlingTimeExponentMarine );

    mPredatorAbundanceMultipliedByTimeEating = mAbundancePredator * mSpecificPredatorTimeUnitsEatingPerGlobalTimeStep;

    // Calculate the abundance of prey in each of the prey mass bins
    PopulateBinnedPreyAbundance( gcl, actingCohort, mPredatorLogOptimalPreyBodySizeRatio );

    // Loop over potential prey functional groups
    for( int FunctionalGroup: mFunctionalGroupIndicesToEat ) {
        // Eating operates differently for planktivores
        // This can certainly be sped up
        // Loop over cohorts within the functional group

        

        for( int i = 0; i < mNumberCohortsPerFunctionalGroupNoNewCohorts[FunctionalGroup]; i++ ) {
        
            //No Cannibalism
            if( gcl.mCohorts[FunctionalGroup][i]->mID == actingCohort->mID ) {
                mPotentialAbundanceEaten[FunctionalGroup][i] = 0.0;
                mTimeUnitsToHandlePotentialFoodItems -= mPotentialAbundanceEaten[ FunctionalGroup ][ i ] * CalculateHandlingTimeTerrestrial( mBodyMassPredator );
            } else {
                if( mDietIsAllSpecial ) {

                    // Get the body mass of individuals in this cohort
                    mBodyMassPrey = gcl.mCohorts[FunctionalGroup][i]->mIndividualBodyMass;

                    // Get the bin number of this prey cohort
                    if( mBodyMassPrey > 0 )mPreyMassBinNumber = GetBinNumber( mBodyMassPrey, mBodyMassPredator, mPredatorLogOptimalPreyBodySizeRatio );

                    // Check whether
                    // The prey cohort is within the feeding range of the predator
                    // the prey cohort still exists in the model (i.e. body mass > 0)
                    // Currently having whales etc eat everything, but preferentially feed on very small things (i.e. filter feeders)
                    if( ( mBodyMassPrey > 0 ) && ( mPlanktonFunctionalGroups[FunctionalGroup] ) && ( 0 < mPreyMassBinNumber ) && ( mPreyMassBinNumber < mNumberOfBins ) ) {
                        // Calculate the potential abundance from this cohort eaten by the acting cohort
                        mPotentialAbundanceEaten[FunctionalGroup][i] = CalculateExpectedNumberKilledMarine( gcl.mCohorts[FunctionalGroup][i]->mCohortAbundance, mBodyMassPrey, mPreyMassBinNumber,
                          FunctionalGroup, mBodyMassPredator, mCarnivoreFunctionalGroups[FunctionalGroup],
                          mOmnivoreFunctionalGroups[FunctionalGroup], mOmnivoreFunctionalGroups[actingCohort->mFunctionalGroupIndex], mPredatorLogOptimalPreyBodySizeRatio, actingCohort->mFunctionalGroupIndex );

                        // Add the time required to handle the potential abundance eaten from this cohort to the cumulative total for all cohorts
                        mTimeUnitsToHandlePotentialFoodItems += mPotentialAbundanceEaten[ FunctionalGroup ][ i ] * CalculateHandlingTimeMarine( mBodyMassPrey );
                    } else {
                        // Assign a potential abundance eaten of zero
                        mPotentialAbundanceEaten[FunctionalGroup][i] = 0.0;
                    }
                } else {
                    // Get the body mass of individuals in this cohort
                    mBodyMassPrey = gcl.mCohorts[FunctionalGroup][i]->mIndividualBodyMass;

                    // Get the bin number of this prey cohort
                    if( mBodyMassPrey > 0 )mPreyMassBinNumber = GetBinNumber( mBodyMassPrey, mBodyMassPredator, mPredatorLogOptimalPreyBodySizeRatio );

                    // Check whether
                    // The prey cohort is within the feeding range of the predator
                    // the prey cohort still exists in the model (i.e. body mass > 0)
                    if( ( mBodyMassPrey > 0 ) && ( 0 < mPreyMassBinNumber ) && ( mPreyMassBinNumber < mNumberOfBins ) ) {
                        // Calculate the potential abundance from this cohort eaten by the acting cohort
                        mPotentialAbundanceEaten[FunctionalGroup][i] = CalculateExpectedNumberKilledMarine( gcl.mCohorts[FunctionalGroup][i]->mCohortAbundance, mBodyMassPrey, mPreyMassBinNumber,
                        FunctionalGroup, mBodyMassPredator, mCarnivoreFunctionalGroups[FunctionalGroup], mOmnivoreFunctionalGroups[FunctionalGroup],
                        mOmnivoreFunctionalGroups[actingCohort->mFunctionalGroupIndex], mPredatorLogOptimalPreyBodySizeRatio, actingCohort->mFunctionalGroupIndex );

                        // Add the time required to handle the potential abundance eaten from this cohort to the cumulative total for all cohorts
                        mTimeUnitsToHandlePotentialFoodItems += mPotentialAbundanceEaten[FunctionalGroup][i] * CalculateHandlingTimeMarine( mBodyMassPrey );
                    } else {
                        // Assign a potential abundance eaten of zero
                        mPotentialAbundanceEaten[FunctionalGroup][i] = 0.0;
                    }
                }
            }
        }
    }
    // No cannibalism; do this outside the loop to speed up the calculations
    //MB Now moved back into the loop
    //  TimeUnitsToHandlePotentialFoodItems -= PotentialAbundanceEaten[actingCohort->FunctionalGroupIndex][actingCohort->positionInList] *
    //          CalculateHandlingTimeMarine(BodyMassPredator);
    //  PotentialAbundanceEaten[actingCohort->FunctionalGroupIndex][actingCohort->positionInList] = 0.0;
}

void EatingCarnivory::PopulateBinnedPreyAbundance( GridCell& gcl, Cohort* actingCohort, double logOptimalPreyBodySizeRatio ) {
    int BinNumber = 0;
    // Loop through prey functional groups
    for( auto& fg: mFunctionalGroupIndicesToEat ) {
        for( auto cohort: gcl.mCohorts[fg] ) {
            // Calculate the difference between the actual body size ratio and the optimal ratio,
            // and then divide by the standard deviation in log ratio space to determine in
            // which bin to assign the prey item.
            if( cohort->mIndividualBodyMass > 0 ) {
                BinNumber = GetBinNumber( cohort->mIndividualBodyMass, actingCohort->mIndividualBodyMass, logOptimalPreyBodySizeRatio );

                if( ( 0 < BinNumber ) && ( BinNumber < mNumberOfBins ) ) {
                    mBinnedPreyDensities[fg][ BinNumber] += cohort->mCohortAbundance / mCellAreaHectares;
                }
            }            
        }
    }
}

int EatingCarnivory::GetBinNumber( double preyMass, double predatorMass, double predatorOptimalPreyBodySizeRatio ) {
    return ( int )( GetBinNumberFractional( preyMass, predatorMass, predatorOptimalPreyBodySizeRatio ) + ( mNumberOfBins / 2 ) );
}

double EatingCarnivory::GetBinNumberFractional( double preyMass, double predatorMass, double predatorOptimalPreyBodySizeRatio ) {
    return (log( preyMass / predatorMass ) - predatorOptimalPreyBodySizeRatio ) / ( 0.5 * mFeedingPreferenceStandardDeviation );
}

void EatingCarnivory::GetEatingPotentialTerrestrial( GridCell& gcl, Cohort* actingCohort, MadingleyInitialisation& params ) {
    mBinnedPreyDensities.resize( gcl.mCohorts.size( ) );
    for( auto& b: mBinnedPreyDensities )b.resize( mNumberOfBins );
    for( auto& b: mBinnedPreyDensities )for( auto& n: b )n = 0;
    // Set the total eaten by the acting cohort to zero
    mTotalBiomassEatenByCohort = 0.0;

    // Set the total number of units to handle all potential prey individuals eaten to zero
    mTimeUnitsToHandlePotentialFoodItems = 0.0;

    // Get the individual body mass of the acting (predator) cohort
    mBodyMassPredator = actingCohort->mIndividualBodyMass;

    // Get the abundance of the acting (predator) cohort
    mAbundancePredator = actingCohort->mCohortAbundance;

    // Pre-calculate individual values for this predator
    mSpecificPredatorKillRateConstant = mKillRateConstant * pow( mBodyMassPredator, ( mKillRateConstantMassExponent ) );
    mSpecificPredatorTimeUnitsEatingPerGlobalTimeStep = mDeltaT * mProportionTimeEating;
    mPredatorAssimilationEfficiency = mAssimilationEfficiency;
    mPredatorNonAssimilation = ( 1 - mAssimilationEfficiency );
    std::string PredatorNutritionSource = params.mCohortFunctionalGroupDefinitions.GetTraitNames( "Nutrition source" , actingCohort->mFunctionalGroupIndex );

    // When body sizes are less than one gram, we have a flat handling time relationship to stop small things having extraordinarily short handling times
    mReferenceMassRatioScalingTerrestrial = mHandlingTimeScalarTerrestrial * pow( mReferenceMass / mBodyMassPredator, mHandlingTimeExponentTerrestrial );

    mPredatorAbundanceMultipliedByTimeEating = mAbundancePredator * mSpecificPredatorTimeUnitsEatingPerGlobalTimeStep;

    mPredatorLogOptimalPreyBodySizeRatio = actingCohort->mLogOptimalPreyBodySizeRatio;

    // Calculate the abundance of prey in each of the prey mass bins
    PopulateBinnedPreyAbundance( gcl, actingCohort, mPredatorLogOptimalPreyBodySizeRatio );

    // Loop over potential prey functional groups
    for( int FunctionalGroup: mFunctionalGroupIndicesToEat ) {

        // Loop over cohorts within the functional group
        for( int i = 0; i < mNumberCohortsPerFunctionalGroupNoNewCohorts[FunctionalGroup]; i++ ) {

            //# No cannibalism
            if( gcl.mCohorts[FunctionalGroup][i]->mID == actingCohort->mID ) {
                
                mPotentialAbundanceEaten[FunctionalGroup][i] = 0.0;
                mTimeUnitsToHandlePotentialFoodItems -= mPotentialAbundanceEaten[ FunctionalGroup ][ i ] * CalculateHandlingTimeTerrestrial( mBodyMassPredator );
            
            // //# Omnivores only able to feed on small prey (<10 kg)
            // } else if(  PredatorNutritionSource == "omnivore" &&
            //             ( gcl.mCohorts[FunctionalGroup][i]->mAdultMass * mRelativeMaxPreyRatioOmnivores > actingCohort->mIndividualBodyMass ) &&
            //             gcl.mCohorts[FunctionalGroup][i]->mAdultMass > 10000
            //     ) {
                
            //     mPotentialAbundanceEaten[FunctionalGroup][i] = 0.0;
            //     mTimeUnitsToHandlePotentialFoodItems -= mPotentialAbundanceEaten[ FunctionalGroup ][ i ] * CalculateHandlingTimeTerrestrial( mBodyMassPredator );
            
            // //# Large carnivores (>21 kg) can not eat cohorts with an adult body mass smaller than 0.5 kg
            // } else if( actingCohort->mAdultMass > 21000 && 
            //            gcl.mCohorts[FunctionalGroup][i]->mAdultMass < 1000 && 
            //            PredatorNutritionSource == "carnivore" ) {

            //    mPotentialAbundanceEaten[FunctionalGroup][i] = 0.0;
            //    mTimeUnitsToHandlePotentialFoodItems -= mPotentialAbundanceEaten[ FunctionalGroup ][ i ] * CalculateHandlingTimeTerrestrial( mBodyMassPredator );
            
            // // //# Small carnivores (<21 kg) can not eat cohorts with an adult body mass larger than 100 kg
            // // } else if(  actingCohort->mAdultMass < 21000 && 
            // //             gcl.mCohorts[FunctionalGroup][i]->mAdultMass > 100000 && 
            // //             PredatorNutritionSource == "carnivore" ) {

            // //     mPotentialAbundanceEaten[FunctionalGroup][i] = 0.0;
            // //     mTimeUnitsToHandlePotentialFoodItems -= mPotentialAbundanceEaten[ FunctionalGroup ][ i ] * CalculateHandlingTimeTerrestrial( mBodyMassPredator );

            //# Eat!!
            } else {
                // Get the body mass of individuals in this cohort
                mBodyMassPrey = gcl.mCohorts[FunctionalGroup][i]->mIndividualBodyMass;

                // Get the bin number of this prey cohort
                if( mBodyMassPrey > 0 )mPreyMassBinNumber = GetBinNumber( mBodyMassPrey, mBodyMassPredator, mPredatorLogOptimalPreyBodySizeRatio );

                // Check whether the prey cohort still exists in the model (i.e. body mass > 0)            
                if( ( mBodyMassPrey > 0 ) && ( 0 < mPreyMassBinNumber ) && ( mPreyMassBinNumber < mNumberOfBins ) ) {
                    // Calculate the potential abundance from this cohort eaten by the acting cohort
                    mPotentialAbundanceEaten[FunctionalGroup][i] = CalculateExpectedNumberKilledTerrestrial( gcl.mCohorts[FunctionalGroup][i]->mCohortAbundance, mBodyMassPrey, mPreyMassBinNumber, FunctionalGroup, mBodyMassPredator, 
                        mCarnivoreFunctionalGroups[FunctionalGroup], mOmnivoreFunctionalGroups[FunctionalGroup], mOmnivoreFunctionalGroups[actingCohort->mFunctionalGroupIndex], mPredatorLogOptimalPreyBodySizeRatio, actingCohort->mFunctionalGroupIndex );

                    // Add the time required to handle the potential abundance eaten from this cohort to the cumulative total for all cohorts
                    mTimeUnitsToHandlePotentialFoodItems += mPotentialAbundanceEaten[ FunctionalGroup ][ i ] * CalculateHandlingTimeTerrestrial( mBodyMassPrey );
                } else {
                    // Assign a potential abundance eaten of zero
                    mPotentialAbundanceEaten[ FunctionalGroup ][ i ] = 0.0;
                }
            }
            //std::cout << "pred bm = " << mBodyMassPredator << " prey bm = " << mBodyMassPrey << " pot eaten = " << mPotentialAbundanceEaten[FunctionalGroup][i] << std::endl;
        }
    }
}

void EatingCarnivory::Run( GridCell& gcl, Cohort* actingCohort, unsigned currentTimestep, MadingleyInitialisation& params ) {
    mTemporaryValue = 0.0;

    // Temporary variable to hold the total time spent eating + 1. Saves an extra calculation in CalculateAbundanceEaten
    double TotalTimeUnitsToHandlePlusOne = mTimeUnitsToHandlePotentialFoodItems + 1;
	
    TimeStep_Months_Calc_FoodWeb =  InputParameters::Get( )->GetTimeStep_Months_Calc_FoodWeb();

    if( currentTimestep >= TimeStep_Months_Calc_FoodWeb ) {
        //# vectors for tracking binned consumption
        std::vector<float> allocFLOAT(11,0.0f);
        actingCohort->mConsumed_HerbivoreBIN_ecto = allocFLOAT;
        actingCohort->mConsumed_HerbivoreBIN_endo = allocFLOAT;
        actingCohort->mConsumed_OmnivoreBIN_ecto = allocFLOAT;
        actingCohort->mConsumed_OmnivoreBIN_endo = allocFLOAT;
        actingCohort->mConsumed_CarnivoreBIN_ecto = allocFLOAT;
        actingCohort->mConsumed_CarnivoreBIN_endo = allocFLOAT;
    }

    //std::cout << "############: " << actingCohort->mIndividualBodyMass << std::endl;
    // Loop over potential prey functional groups
	unsigned counter = 0;
    for( int FunctionalGroup: mFunctionalGroupIndicesToEat ) {
        
        // Loop over cohorts within the functional group
        for( int i = 0; i < mNumberCohortsPerFunctionalGroupNoNewCohorts[FunctionalGroup]; i++ ) {
    
            // Get the individual body mass of this cohort
            mBodyMassPrey = gcl.mCohorts[FunctionalGroup][i]->mIndividualBodyMass;
			mAdultBodyMassPrey = gcl.mCohorts[FunctionalGroup][i]->mAdultMass;

            // Calculate the actual abundance of prey eaten from this cohort
            if( gcl.mCohorts[FunctionalGroup][i]->mCohortAbundance > 0 ) {

                // Calculate the actual abundance of prey eaten from this cohort
                mAbundancesEaten[FunctionalGroup][i] = CalculateAbundanceEaten( mPotentialAbundanceEaten[FunctionalGroup][i], mPredatorAbundanceMultipliedByTimeEating,
                TotalTimeUnitsToHandlePlusOne, gcl.mCohorts[FunctionalGroup][i]->mCohortAbundance );
            } else {
                mAbundancesEaten[FunctionalGroup][i] = 0;
            }

   	        //std::cout << mAbundancesEaten[FunctionalGroup][i] << " ";
            // Remove number of prey eaten from the prey cohort
            gcl.mCohorts[FunctionalGroup][i]->mCohortAbundance -= mAbundancesEaten[FunctionalGroup][i];


            // Check that the abundance eaten from this cohort is not negative
            if( mAbundancesEaten[FunctionalGroup][i] < 0. ) {
                std::cout << "Predation negative for this prey cohort" << actingCohort->mFunctionalGroupIndex << " " << actingCohort->mID << mAbundancesEaten[FunctionalGroup][i] << std::endl;
            }
            // Create a temporary value to speed up the predation function
            // This is equivalent to the body mass of the prey cohort including reproductive potential mass, times the abundance eaten of the prey cohort,
            // divided by the abundance of the predator
            mTemporaryValue += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i] / mAbundancePredator;
	        if(mTemporaryValue < 0 ) std::cout << mTemporaryValue << std::endl;           

			if( currentTimestep >= TimeStep_Months_Calc_FoodWeb ) {

				if(params.mCohortFunctionalGroupDefinitions.GetTraitNames( "Nutrition source" , gcl.mCohorts[FunctionalGroup][i]->mFunctionalGroupIndex ) == "herbivore" )
				{
					actingCohort->mConsumed_Herbivore += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i]; 
					
					if( params.mCohortFunctionalGroupDefinitions.GetTraitNames( "Endo/Ectotherm" , gcl.mCohorts[FunctionalGroup][i]->mFunctionalGroupIndex ) == "ectotherm" ) {
						if(mAdultBodyMassPrey > 0.0001 & mAdultBodyMassPrey < 0.001){
							actingCohort->mConsumed_HerbivoreBIN_ecto[0] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 0.001 & mAdultBodyMassPrey < 0.01){
							actingCohort->mConsumed_HerbivoreBIN_ecto[1] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 0.01 & mAdultBodyMassPrey < 0.1){
							actingCohort->mConsumed_HerbivoreBIN_ecto[2] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 0.1 & mAdultBodyMassPrey < 1){
							actingCohort->mConsumed_HerbivoreBIN_ecto[3] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 1 & mAdultBodyMassPrey < 10){
							actingCohort->mConsumed_HerbivoreBIN_ecto[4] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 10 & mAdultBodyMassPrey < 100){
							actingCohort->mConsumed_HerbivoreBIN_ecto[5] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 100 & mAdultBodyMassPrey < 1000){
							actingCohort->mConsumed_HerbivoreBIN_ecto[6] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 1000 & mAdultBodyMassPrey < 10000){
							actingCohort->mConsumed_HerbivoreBIN_ecto[7] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];	
						}else if(mAdultBodyMassPrey > 10000 & mAdultBodyMassPrey < 100000){
							actingCohort->mConsumed_HerbivoreBIN_ecto[8] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 100000 & mAdultBodyMassPrey < 1000000){
							actingCohort->mConsumed_HerbivoreBIN_ecto[9] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 1000000){
							actingCohort->mConsumed_HerbivoreBIN_ecto[10] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}
					}
					
					if( params.mCohortFunctionalGroupDefinitions.GetTraitNames( "Endo/Ectotherm" , gcl.mCohorts[FunctionalGroup][i]->mFunctionalGroupIndex ) == "endotherm" ) {
						if(mAdultBodyMassPrey > 0.0001 & mAdultBodyMassPrey < 0.001){
							actingCohort->mConsumed_HerbivoreBIN_endo[0] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 0.001 & mAdultBodyMassPrey < 0.01){
							actingCohort->mConsumed_HerbivoreBIN_endo[1] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 0.01 & mAdultBodyMassPrey < 0.1){
							actingCohort->mConsumed_HerbivoreBIN_endo[2] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 0.1 & mAdultBodyMassPrey < 1){
							actingCohort->mConsumed_HerbivoreBIN_endo[3] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 1 & mAdultBodyMassPrey < 10){
							actingCohort->mConsumed_HerbivoreBIN_endo[4] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 10 & mAdultBodyMassPrey < 100){
							actingCohort->mConsumed_HerbivoreBIN_endo[5] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 100 & mAdultBodyMassPrey < 1000){
							actingCohort->mConsumed_HerbivoreBIN_endo[6] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 1000 & mAdultBodyMassPrey < 10000){
							actingCohort->mConsumed_HerbivoreBIN_endo[7] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];	
						}else if(mAdultBodyMassPrey > 10000 & mAdultBodyMassPrey < 100000){
							actingCohort->mConsumed_HerbivoreBIN_endo[8] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 100000 & mAdultBodyMassPrey < 1000000){
							actingCohort->mConsumed_HerbivoreBIN_endo[9] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 1000000){
							actingCohort->mConsumed_HerbivoreBIN_endo[10] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}
					}
					
				}
				if(params.mCohortFunctionalGroupDefinitions.GetTraitNames( "Nutrition source" , gcl.mCohorts[FunctionalGroup][i]->mFunctionalGroupIndex ) == "omnivore" )
				{
					actingCohort->mConsumed_Omnivore  += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i]; 
					
					if( params.mCohortFunctionalGroupDefinitions.GetTraitNames( "Endo/Ectotherm" , gcl.mCohorts[FunctionalGroup][i]->mFunctionalGroupIndex ) == "endotherm" ) {
						if(mAdultBodyMassPrey > 0.0001 & mAdultBodyMassPrey < 0.001){
							actingCohort->mConsumed_OmnivoreBIN_endo[0] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 0.001 & mAdultBodyMassPrey < 0.01){
							actingCohort->mConsumed_OmnivoreBIN_endo[1] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 0.01 & mAdultBodyMassPrey < 0.1){
							actingCohort->mConsumed_OmnivoreBIN_endo[2] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 0.1 & mAdultBodyMassPrey < 1){
							actingCohort->mConsumed_OmnivoreBIN_endo[3] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 1 & mAdultBodyMassPrey < 10){
							actingCohort->mConsumed_OmnivoreBIN_endo[4] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 10 & mAdultBodyMassPrey < 100){
							actingCohort->mConsumed_OmnivoreBIN_endo[5] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 100 & mAdultBodyMassPrey < 1000){
							actingCohort->mConsumed_OmnivoreBIN_endo[6] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 1000 & mAdultBodyMassPrey < 10000){
							actingCohort->mConsumed_OmnivoreBIN_endo[7] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];	
						}else if(mAdultBodyMassPrey > 10000 & mAdultBodyMassPrey < 100000){
							actingCohort->mConsumed_OmnivoreBIN_endo[8] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 100000 & mAdultBodyMassPrey < 1000000){
							actingCohort->mConsumed_OmnivoreBIN_endo[9] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 1000000){
							actingCohort->mConsumed_OmnivoreBIN_endo[10] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}
					}
					
					if( params.mCohortFunctionalGroupDefinitions.GetTraitNames( "Endo/Ectotherm" , gcl.mCohorts[FunctionalGroup][i]->mFunctionalGroupIndex ) == "ectotherm" ) {
						if(mAdultBodyMassPrey > 0.0001 & mAdultBodyMassPrey < 0.001){
							actingCohort->mConsumed_OmnivoreBIN_ecto[0] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 0.001 & mAdultBodyMassPrey < 0.01){
							actingCohort->mConsumed_OmnivoreBIN_ecto[1] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 0.01 & mAdultBodyMassPrey < 0.1){
							actingCohort->mConsumed_OmnivoreBIN_ecto[2] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 0.1 & mAdultBodyMassPrey < 1){
							actingCohort->mConsumed_OmnivoreBIN_ecto[3] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 1 & mAdultBodyMassPrey < 10){
							actingCohort->mConsumed_OmnivoreBIN_ecto[4] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 10 & mAdultBodyMassPrey < 100){
							actingCohort->mConsumed_OmnivoreBIN_ecto[5] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 100 & mAdultBodyMassPrey < 1000){
							actingCohort->mConsumed_OmnivoreBIN_ecto[6] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 1000 & mAdultBodyMassPrey < 10000){
							actingCohort->mConsumed_OmnivoreBIN_ecto[7] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];	
						}else if(mAdultBodyMassPrey > 10000 & mAdultBodyMassPrey < 100000){
							actingCohort->mConsumed_OmnivoreBIN_ecto[8] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 100000 & mAdultBodyMassPrey < 1000000){
							actingCohort->mConsumed_OmnivoreBIN_ecto[9] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 1000000){
							actingCohort->mConsumed_OmnivoreBIN_ecto[10] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}
					}
				}
				if(params.mCohortFunctionalGroupDefinitions.GetTraitNames( "Nutrition source" , gcl.mCohorts[FunctionalGroup][i]->mFunctionalGroupIndex ) == "carnivore" )
				{
					actingCohort->mConsumed_Carnivore += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i]; 
					
					if( params.mCohortFunctionalGroupDefinitions.GetTraitNames( "Endo/Ectotherm" , gcl.mCohorts[FunctionalGroup][i]->mFunctionalGroupIndex ) == "endotherm" ) {
						if(mAdultBodyMassPrey > 0.0001 & mAdultBodyMassPrey < 0.001){
							actingCohort->mConsumed_CarnivoreBIN_endo[0] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 0.001 & mAdultBodyMassPrey < 0.01){
							actingCohort->mConsumed_CarnivoreBIN_endo[1] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 0.01 & mAdultBodyMassPrey < 0.1){
							actingCohort->mConsumed_CarnivoreBIN_endo[2] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 0.1 & mAdultBodyMassPrey < 1){
							actingCohort->mConsumed_CarnivoreBIN_endo[3] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 1 & mAdultBodyMassPrey < 10){
							actingCohort->mConsumed_CarnivoreBIN_endo[4] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 10 & mAdultBodyMassPrey < 100){
							actingCohort->mConsumed_CarnivoreBIN_endo[5] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 100 & mAdultBodyMassPrey < 1000){
							actingCohort->mConsumed_CarnivoreBIN_endo[6] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 1000 & mAdultBodyMassPrey < 10000){
							actingCohort->mConsumed_CarnivoreBIN_endo[7] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];	
						}else if(mAdultBodyMassPrey > 10000 & mAdultBodyMassPrey < 100000){
							actingCohort->mConsumed_CarnivoreBIN_endo[8] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 100000 & mAdultBodyMassPrey < 1000000){
							actingCohort->mConsumed_CarnivoreBIN_endo[9] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 1000000){
							actingCohort->mConsumed_CarnivoreBIN_endo[10] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}
					}
					
					if( params.mCohortFunctionalGroupDefinitions.GetTraitNames( "Endo/Ectotherm" , gcl.mCohorts[FunctionalGroup][i]->mFunctionalGroupIndex ) == "ectotherm" ) {
						if(mAdultBodyMassPrey > 0.0001 & mAdultBodyMassPrey < 0.001){
							actingCohort->mConsumed_CarnivoreBIN_ecto[0] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 0.001 & mAdultBodyMassPrey < 0.01){
							actingCohort->mConsumed_CarnivoreBIN_ecto[1] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 0.01 & mAdultBodyMassPrey < 0.1){
							actingCohort->mConsumed_CarnivoreBIN_ecto[2] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 0.1 & mAdultBodyMassPrey < 1){
							actingCohort->mConsumed_CarnivoreBIN_ecto[3] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 1 & mAdultBodyMassPrey < 10){
							actingCohort->mConsumed_CarnivoreBIN_ecto[4] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 10 & mAdultBodyMassPrey < 100){
							actingCohort->mConsumed_CarnivoreBIN_ecto[5] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 100 & mAdultBodyMassPrey < 1000){
							actingCohort->mConsumed_CarnivoreBIN_ecto[6] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 1000 & mAdultBodyMassPrey < 10000){
							actingCohort->mConsumed_CarnivoreBIN_ecto[7] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];	
						}else if(mAdultBodyMassPrey > 10000 & mAdultBodyMassPrey < 100000){
							actingCohort->mConsumed_CarnivoreBIN_ecto[8] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 100000 & mAdultBodyMassPrey < 1000000){
							actingCohort->mConsumed_CarnivoreBIN_ecto[9] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}else if(mAdultBodyMassPrey > 1000000){
							actingCohort->mConsumed_CarnivoreBIN_ecto[10] += ( mBodyMassPrey + gcl.mCohorts[FunctionalGroup][i]->mIndividualReproductivePotentialMass ) * mAbundancesEaten[FunctionalGroup][i];
						}
					}
					
				}

			}
			
        }
       
    }

    // Add the biomass eaten and assimilated by an individual to the delta biomass for the acting (predator) cohort
    Cohort::mMassAccounting["biomass"]["predation"] = mTemporaryValue * mPredatorAssimilationEfficiency;

    // Move the biomass eaten but not assimilated by an individual into the organic matter pool
    Cohort::mMassAccounting["organicpool"]["predation"] = mTemporaryValue * mPredatorNonAssimilation * mAbundancePredator;

    // Check that the delta biomass from eating for the acting cohort is not negative
    assert( Cohort::mMassAccounting["biomass"]["predation"] >= 0 && "Predation yields negative biomass" );
    if( Cohort::mMassAccounting["biomass"]["predation"] < 0 ) {
	std::cout << "Predation biomass eaten is negative: " << Cohort::mMassAccounting["biomass"]["predation"] << std::endl;
	Cohort::mMassAccounting["biomass"]["predation"]=0;	
    }
    
    if( std::isnan(Cohort::mMassAccounting["biomass"]["predation"] ) ) {
	Cohort::mMassAccounting["biomass"]["predation"]=0;
	std::cout << "Changed Predation" << std::endl;
    }

    // Calculate the total biomass eaten by the acting (predator) cohort
    mTotalBiomassEatenByCohort = Cohort::mMassAccounting["biomass"]["predation"] * mAbundancePredator;

}

double EatingCarnivory::CalculateExpectedNumberKilledTerrestrial( double preyAbundance, double preyIndividualMass, int preyMassBinNumber, int preyFunctionalGroup, double predatorIndividualMass, bool preyIsCarnivore, bool preyIsOmnivore, bool predatorIsOmnivore,
        double logOptimalPreyPredatorMassRatio, int predatorFunctionalGroup ) {
    // Calculate the killing rate of an individual predator per unit prey density per hectare per time unit
    mAlphaIJ = CalculateIndividualKillingRatePerHectare( preyIndividualMass, preyMassBinNumber, preyFunctionalGroup, predatorIndividualMass, logOptimalPreyPredatorMassRatio, predatorFunctionalGroup );

    // Calculate the potential number of prey killed given the number of prey detections
    return mAlphaIJ * preyAbundance / mCellAreaHectares;
}

double EatingCarnivory::CalculateExpectedNumberKilledMarine( double preyAbundance, double preyIndividualMass, int preyMassBinNumber, int preyFunctionalGroup, double predatorIndividualMass, bool preyIsCarnivore, bool preyIsOmnivore, bool predatorIsOmnivore,
        double logOptimalPreyPredatorMassRatio, int predatorFunctionalGroup ) {
    // Calculate the killing rate of an individual predator per unit prey density per hectare per time unit
    mAlphaIJ = CalculateIndividualKillingRatePerHectare( preyIndividualMass, preyMassBinNumber, preyFunctionalGroup, predatorIndividualMass, logOptimalPreyPredatorMassRatio, predatorFunctionalGroup );

    // Calculate the potential number of prey killed given the number of prey detections
    return mAlphaIJ * preyAbundance / mCellAreaHectares;
}

double EatingCarnivory::CalculateIndividualKillingRatePerHectare( double preyIndividualMass, int preyMassBinNumber, int preyFunctionalGroup, double predatorIndividualMass, double logOptimalPreyPredatorMassRatio, int predatorFunctionalGroup ) {
    //int PreyBinNumber;

    // Calculate the relative feeding preference from a log-normal distribution with mean equal to the optimal 
    // prey to predator ratio and standard deviation as specified
    mRelativeFeedingPreference = exp( -( pow( ( ( log( preyIndividualMass / predatorIndividualMass ) - logOptimalPreyPredatorMassRatio ) / mFeedingPreferenceStandardDeviation ), 2 ) ) );

    // Calculate the individual killing rate
    return mSpecificPredatorKillRateConstant * mRelativeFeedingPreference * mBinnedPreyDensities[preyFunctionalGroup][ preyMassBinNumber];
}


double EatingCarnivory::CalculateHandlingTimeTerrestrial( double preyIndividualMass ) {
    return mReferenceMassRatioScalingTerrestrial * preyIndividualMass;
}

double EatingCarnivory::CalculateHandlingTimeMarine( double preyIndividualMass ) {
    return mReferenceMassRatioScalingMarine * preyIndividualMass;
}

double EatingCarnivory::CalculateAbundanceEaten( double potentialKills, double predatorAbundanceMultipliedByTimeEating, double totalHandlingTimePlusOne, double preyAbundance ) {
    // Optimized for speed; check for zero abundance prey moved to the calling function
    return preyAbundance * ( 1.0 - exp( -( predatorAbundanceMultipliedByTimeEating * ( ( potentialKills / totalHandlingTimePlusOne ) / preyAbundance ) ) ) );
}

double EatingCarnivory::CalculateVisibility( double preyAbundance ) {
    return pow( preyAbundance, 0 );
}
