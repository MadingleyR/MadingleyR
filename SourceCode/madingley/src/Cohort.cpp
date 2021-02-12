#include <vector>
#include <limits.h>
#include "Cohort.h"
#include "GridCell.h"
#include "MadingleyInitialisation.h"
#include "DispersalSet.h"
#include "Parameters.h"

unsigned Cohort::mNextID = 0;
Types::Double2DMap Cohort::mMassAccounting;

Cohort::Cohort( GridCell& gcl, unsigned functionalGroupIndex, double juvenileBodyMass, double adultBodyMass, double initialBodyMass, 
                double initialAbundance, double optimalPreyBodySizeRatio, unsigned short birthTimeStep, double proportionTimeActive, 
                long long &nextCohortID, double trophicindex, double individualReproductivePotentialMass, unsigned maturityTimeStep,
                int isAdult, int ageMonths, int timeStepsJuviline, int timeStepsAdult ) {
    mFunctionalGroupIndex = functionalGroupIndex;
    mJuvenileMass = juvenileBodyMass;
    mAdultMass = adultBodyMass;
    mIndividualBodyMass = initialBodyMass;
    mCohortAbundance = initialAbundance;
    mBirthTimeStep = birthTimeStep;
    mMaturityTimeStep = maturityTimeStep; // std::numeric_limits<unsigned>::max( );
    mLogOptimalPreyBodySizeRatio = log( optimalPreyBodySizeRatio );
    mMaximumAchievedBodyMass = juvenileBodyMass;
    mMerged = false;
    mProportionTimeActive = proportionTimeActive;
    mCurrentCell = &gcl;
    mDestinationCell = mCurrentCell;
    mCurrentLocation.SetIndices( gcl.GetLatitudeIndex( ), gcl.GetLongitudeIndex( ) );
    mDestinationLocation = mCurrentLocation;
    mIndividualReproductivePotentialMass = individualReproductivePotentialMass;
    mID = mNextID; //MB added to track this object.
    mNextID++;
    nextCohortID++; // FIX - Is this increment required?

    //#### MortalitySenescence parameters
    mIsAdult = isAdult;
    mAgeMonths = ageMonths;
    mTimeStepsJuviline = timeStepsJuviline;
    mTimeStepsAdult = timeStepsAdult;
    //####

    // track cohort consumption
    //##############
    mConsumed_Autotroph = 0.0;
    mConsumed_Herbivore = 0.0;
    mConsumed_Omnivore  = 0.0;
    mConsumed_Carnivore = 0.0;
    mTrophicIndex = trophicindex;
    //##############
	
	// track cohort consumption log bins
    //##############
	std::vector<float> mConsumed_HerbivoreBIN_ecto;
	std::vector<float> mConsumed_HerbivoreBIN_endo; 
	std::vector<float> mConsumed_OmnivoreBIN_ecto;
	std::vector<float> mConsumed_OmnivoreBIN_endo;
	std::vector<float> mConsumed_CarnivoreBIN_ecto; 
	std::vector<float> mConsumed_CarnivoreBIN_endo;
    //##############

    // track cohort bodymass change
    //##############
    mOriginal_AdultMass = adultBodyMass;
    //##############
	
	/*
	// track food web activity
    std::vector<int> mFGE;
	std::vector<float> mIdivBM;
	std::vector<float> mAbEaten;
	std::vector<float> mIdivReproBM;
	std::vector<float> mAdlBM;
	std::vector<float> mPreyAbundance;
	*/

       
}

Cohort::Cohort( Cohort* actingCohort, double juvenileBodyMass, double adultBodyMass, double initialBodyMass, double initialAbundance, 
                double orignalAdultMass, unsigned birthTimeStep, long long& nextCohortID, double trophicindex,
                int isAdult, int ageMonths, int timeStepsJuviline, int timeStepsAdult ) {
    mFunctionalGroupIndex = actingCohort->mFunctionalGroupIndex;
    mJuvenileMass = juvenileBodyMass;
    mAdultMass = adultBodyMass;
    mIndividualBodyMass = initialBodyMass;
    mCohortAbundance = initialAbundance;
    mBirthTimeStep = birthTimeStep;
    mMaturityTimeStep = std::numeric_limits<unsigned>::max( );
    mLogOptimalPreyBodySizeRatio = actingCohort->mLogOptimalPreyBodySizeRatio;
    mMaximumAchievedBodyMass = juvenileBodyMass;
    mMerged = false;
    mProportionTimeActive = actingCohort->mProportionTimeActive;
    mCurrentCell = actingCohort->mCurrentCell;
    mDestinationCell = mCurrentCell;
    mCurrentLocation = actingCohort->mCurrentLocation;
    mDestinationLocation = mCurrentLocation;
    mIndividualReproductivePotentialMass = 0;
    mID = mNextID; //MB added to track this object.
    mNextID++;
    nextCohortID++; // FIX - Is this increment required?

    //#### MortalitySenescence parameters
    mIsAdult = isAdult;
    mAgeMonths = ageMonths;
    mTimeStepsJuviline = timeStepsJuviline;
    mTimeStepsAdult = timeStepsAdult;
    //####

    // track cohort consumption
    //##############
    mConsumed_Autotroph = 0.0;
    mConsumed_Herbivore = 0.0;
    mConsumed_Omnivore  = 0.0;
    mConsumed_Carnivore = 0.0;
    mConsumed_Total = 0.0;
    mTrophicIndex = trophicindex;
    //##############

    // track cohort bodymass change
    //##############
    mOriginal_AdultMass = orignalAdultMass;
    //##############
	
	
	// track cohort consumption log bins
    //##############
	std::vector<float> mConsumed_HerbivoreBIN_ecto;
	std::vector<float> mConsumed_HerbivoreBIN_endo; 
	std::vector<float> mConsumed_OmnivoreBIN_ecto;
	std::vector<float> mConsumed_OmnivoreBIN_endo;
	std::vector<float> mConsumed_CarnivoreBIN_ecto; 
	std::vector<float> mConsumed_CarnivoreBIN_endo;
    //##############
	
	
	/*
	// track food web activity
    std::vector<int> mFGE;
	std::vector<float> mIdivBM;
	std::vector<float> mAbEaten;
	std::vector<float> mIdivReproBM;
	std::vector<float> mAdlBM;
	std::vector<float> mPreyAbundance;
	*/
}

bool Cohort::IsMature( ) {
    //return ( mMaturityTimeStep < std::numeric_limits<unsigned>::max( ) );
    return ( mIsAdult == 1 );
}

void Cohort::ResetMassFluxes( ) {
  
    double ResetValue = 0.0;
    // Initialize delta abundance sorted list with appropriate processes
    mMassAccounting["abundance"]["mortality"] = ResetValue;

    // Initialize delta biomass sorted list with appropriate processes
    mMassAccounting["biomass"]["metabolism"] = ResetValue;
    mMassAccounting["biomass"]["predation"] = ResetValue;
    mMassAccounting["biomass"]["herbivory"] = ResetValue;
    mMassAccounting["biomass"]["reproduction"] = ResetValue;

    // Initialize delta reproductive biomass vector with appropriate processes
    mMassAccounting["reproductivebiomass"]["reproduction"] = ResetValue;

    // Initialize organic pool delta vector with appropriate processes
    mMassAccounting["organicpool"]["herbivory"] = ResetValue;
    mMassAccounting["organicpool"]["predation"] = ResetValue;
    mMassAccounting["organicpool"]["mortality"] = ResetValue;

    // Initialize respiratory CO2 pool delta vector with appropriate processes
    mMassAccounting["respiratoryCO2pool"]["metabolism"] = ResetValue;
}

double Cohort::GetRealm( ) {
    return mCurrentCell->GetRealm( );
}

double Cohort::GetGridcellTemperature( int tstep ) {
    double temp = mCurrentCell->GetGridcellNearSurfaceTemp( tstep );
    return temp;
}

void Cohort::TryLivingAt( Types::GridCellPointer destination, Location& L ) {
    if( destination != 0 && destination->GetRealm( ) == GetRealm( ) ) {
        mDestinationCell = destination;
        mDestinationLocation = L;
    }
}

GridCell& Cohort::GetDestination( ) {
    return *mDestinationCell;
}

GridCell& Cohort::GetCurrentCell( ) {
    return *mCurrentCell;
}

void Cohort::SetCurrentCell( Types::GridCellPointer gclp ) {
    mCurrentCell = gclp;
}

bool Cohort::IsMoving( ) {
    return mCurrentCell != mDestinationCell;
}

void Cohort::Move( ) {
    mCurrentCell->MoveCohort( this );
}

bool Cohort::IsMarine( ) {
    return mCurrentCell->IsMarine( );
}

bool Cohort::IsPlanktonic( MadingleyInitialisation& params ) {
    return ( IsMarine( ) && ( ( mIndividualBodyMass <= Parameters::Get( )->GetPlanktonSizeThreshold( ) ) || ( params.mCohortFunctionalGroupDefinitions.GetTraitNames( "Mobility", mFunctionalGroupIndex ) == "planktonic" ) ) );
}

int Cohort::GetBin( std::vector<double> Bins ) {
    int bin_number = 0;
    for( int i = 0; i < Bins.size(); i++){
        if(mIndividualBodyMass>Bins[i]){
            bin_number++;
        }else{
            break;
        }
    }
    return bin_number;
}

std::string Cohort::GetDispersalType( MadingleyInitialisation& params ) {
    std::string dispersalName;
    if( IsPlanktonic( params ) ) {
        // Advective dispersal
        dispersalName = "advective";
    }// Otherwise, if mature do responsive dispersal
    else if( IsMature( ) ) {
        dispersalName = "responsive";
    }// If the cohort is immature, run diffusive dispersal
    else {
        dispersalName = "diffusive";
    }
    return dispersalName;
}
