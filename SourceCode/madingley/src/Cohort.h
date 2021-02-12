#ifndef COHORT_H
#define COHORT_H

#include "Types.h"
#include "Location.h"

class Cohort {
public:
    /** \brief Constructor for the Cohort class: assigns cohort starting properties at beginning of model run
    @param gcl The grid cell that holds this cohort
    @param juvenileBodyMass The mean juvenile body mass of individuals in the cohort
    @param adultBodyMass The mean mature adult body mass of individuals in the cohort
    @param initialBodyMass The intial mean body mass of individuals in this cohort
    @param initialAbundance The intial number of individuals in this cohort
    @param optimalPreyBodySizeRatio The optimal prey body mass (as a percentage of this cohorts mass) for individuals in this cohort
    @param birthTimeStep The birth time step for this cohort
    @param nextCohortID The unique ID to assign to the next cohort created    */
    Cohort( GridCell&, unsigned, double, double, double, double, double, unsigned short, 
            double, long long&, double, double, unsigned, int, int, int, int );

    /** \brief Constructor for the Cohort class: assigns cohort starting properties on reproduction
    @param actingCohort The parent of this cohort
    @param p track position of this cohort in the list held in the cell
    @param juvenileBodyMass The mean juvenile body mass of individuals in the cohort
    @param adultBodyMass The mean mature adult body mass of individuals in the cohort
    @param initialBodyMass The intial mean body mass of individuals in this cohort
    @param initialAbundance The intial number of individuals in this cohort
    @param birthTimeStep The birth time step for this cohort
    @param nextCohortID The unique ID to assign to the next cohort created    */
    Cohort( Cohort*, double, double, double, double, double, unsigned, 
            long long&, double, int, int, int, int );

    GridCell& GetCurrentCell( );
    GridCell& GetDestination( );
    void SetCurrentCell( Types::GridCellPointer );

    bool IsMoving( );
    bool IsMature( );
    bool IsMarine( );
    bool IsPlanktonic( MadingleyInitialisation& );
    std::string GetDispersalType( MadingleyInitialisation& );
    double GetRealm( );
    double GetGridcellTemperature( int t );

    void TryLivingAt( Types::GridCellPointer, Location& );
    void Move( );
    static void ResetMassFluxes( );

    int GetBin( std::vector<double> );

    static Types::Double2DMap mMassAccounting;
    static unsigned mNextID;
    #pragma omp threadprivate(mMassAccounting,mNextID)

    long long mID;
    /** \brief A list of all cohort IDs ever associated with individuals in this current cohort */
    Types::LongVector mCohortID;

    GridCell* mCurrentCell;
    GridCell* mDestinationCell;
    Location mCurrentLocation;
    Location mDestinationLocation;
    /** \brief Time step when the cohort was generated */
    unsigned mBirthTimeStep;
    /** \brief The time step at which this cohort reached maturity */
    unsigned mMaturityTimeStep;
    /** \brief The mean juvenile mass of individuals in this cohort */
    double mJuvenileMass;
    /** \brief The mean mature adult mass of individuals in this cohort */
    double mAdultMass;
    /** \brief The mean body mass of an individual in this cohort */
    double mIndividualBodyMass;
    /** \brief Individual biomass assigned to reproductive potential */
    double mIndividualReproductivePotentialMass;
    /** \brief The maximum mean body mass ever achieved by individuals in this cohort */
    double mMaximumAchievedBodyMass;
    /** \brief The number of individuals in the cohort */
    double mCohortAbundance;
    /** \brief The proportion of the timestep for which this cohort is active */
    double mProportionTimeActive;
    /** \brief The optimal prey body size for individuals in this cohort */
    double mLogOptimalPreyBodySizeRatio;
    /** \brief The index of the functional group that the cohort belongs to */
    unsigned mFunctionalGroupIndex;
    /** \brief Whether this cohort has ever been merged with another cohort */
    bool mMerged;

    int mIsAdult;
    int mAgeMonths;
    int mTimeStepsJuviline;
    int mTimeStepsAdult;

    // track cohort consumption
    //##############
    double mConsumed_Autotroph;
    double mConsumed_Herbivore;
    double mConsumed_Omnivore;
    double mConsumed_Carnivore;
    double mConsumed_Total;
    double mTrophicIndex;
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
    double mOriginal_AdultMass;
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
};

#endif
