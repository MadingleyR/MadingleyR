#ifndef EATINGCARNIVORY
#define EATINGCARNIVORY

#include "Eating.h"
#include "GridCell.h"

/** \brief A revised version of the predation process, written November 2011 */
class EatingCarnivory : public Eating {
public:


    int TimeStep_Months_Calc_FoodWeb;
    int UseNonDefaultModelParameters;
    std::vector<double> EatingCarnivory_Parameters;


    /** \brief Constructor for predation: assigns all parameter values
    @param cellArea The area (in square km) of the grid cell
    @param globalModelTimeStepUnit The time step unit used in the model */
    EatingCarnivory( std::string );

    ~EatingCarnivory( );

    /** \brief Initialises predation implementation each time step
    @param gcl The current grid cell
    @param params The definitions of things in the model
    @remark This only works if: a) predation is initialised in every grid cell; and b) if parallelisation is done by latitudinal strips
    It is critical to run this every time step */
    void InitializeEatingPerTimeStep( GridCell&, MadingleyInitialisation& );

    /** \brief Calculate the potential number of prey that could be gained through predation on each cohort in the grid cell
    @param gcl The current grid cell
    @param actingCohort The acting cohort
    @param The definitions for all the model parameters */
    void GetEatingPotentialMarine( GridCell&, Cohort*, MadingleyInitialisation& );

    /** \brief Create the matrix of prey abundances in each weight bin
    @param gcl The current grid cell
    @param actingCohort The predator cohort
    @param logOptimalPreyBodySizeRatio How big is the prey  */
    void PopulateBinnedPreyAbundance( GridCell&, Cohort*, double );

    /** \brief Get the bin number for a prey of a particular body mass */
    int GetBinNumber( double, double, double );

    /** \brief Get the fractional bin number for a prey of a particular body mass */
    double GetBinNumberFractional( double, double, double );

    /** \brief Calculate the potential number of prey that could be gained through predation on each cohort in the grid cell
    @param gcl The current grid cell
    @param actingCohort The acting cohort
    @param params The definitions for items in the model  */
    void GetEatingPotentialTerrestrial( GridCell&, Cohort*, MadingleyInitialisation& );

    /** \brief Apply the changes from predation to prey cohorts, and update deltas for the predator cohort
    @param gridCell The current grid cell
    @param actingCohort The acting cohort
    @param currentTimestep The current model time step */
    void Run( GridCell&, Cohort*, unsigned, MadingleyInitialisation& );

    /** \brief Calculate the potential number of individuals in a prey cohort eaten by an acting predator cohort given the number of prey detections
    @param preyAbundance The number of individuals in the prey cohort
    @param preyIndividualMass The body mass of prey individuals
    @param predatorIndividualMass The body mass of predator individuals
    @param preyIsCarnivore Whether the prey cohort is a carnivore cohort
    @param preyIsOmnivore Whether the prey cohort is an omnivore cohort
    @param predatorIsOmnivore Whether the predator cohort is an omnivore cohort
    @param logOptimalPreyPredatorMassRatio The log ratio of optimal prey body mass to predator body mass
    @return The potential number of individuals in a prey cohort eaten by an acting predator cohort*/
    double CalculateExpectedNumberKilledTerrestrial( double, double, int, int, double, bool, bool, bool, double, int );

    /** \brief Calculate the potential number of individuals in a prey cohort eaten by an acting predator cohort given the number of prey detections
    @param preyAbundance The number of individuals in the prey cohort
    @param preyIndividualMass The body mass of prey individuals
    @param predatorIndividualMass The body mass of predator individuals
    @param preyIsCarnivore Whether the prey cohort is a carnivore cohort
    @param preyIsOmnivore Whether the prey cohort is an omnivore cohort
    @param predatorIsOmnivore Whether the predator cohort is am omnivore cohort
    @param logOptimalPreyPredatorMassRatio The log ratio of optimal prey body mass to predator body mass
    @return The potential number of individuals in a prey cohort eaten by an acting predator cohort*/
    double CalculateExpectedNumberKilledMarine( double, double, int, int, double, bool, bool, bool, double, int );

    /** \brief Calculates the killing rate of an individual predator per unit prey density per hectare per time unit
    @param preyIndividualMass The body mass of individuals in the prey cohort
    @param predatorIndividualMass The body mass of individuals in the predator cohort
    @param logOptimalPreyPredatorMassRatio The log ratio of optimal prey body mass to predator body mass
    @return The killing rate of an individual predator per unit prey density per hectare per time unit*/
    double CalculateIndividualKillingRatePerHectare( double, int, int, double, double, int );

    /** \brief Calculates the time for an individual predator to handle an individual prey in the terrestrial realm
    @param preyIndividualMass The body mass of prey individuals
    @return The time for an individual predator to handle an individual prey*/
    double CalculateHandlingTimeTerrestrial( double );

    /** \brief Calculates the time for an individual predator to handle an individual prey in the marine realm
    @param preyIndividualMass The body mass of prey individuals
    @return The time for an individual predator to handle an individual prey*/
    double CalculateHandlingTimeMarine( double );

    /** \brief Calculate the actual abundance of a prey cohort eaten by a predator cohort
    @param potentialKills The potential abundance of the prey cohort eaten by the predator cohort given the number of detections
    @param totalHandlingTimePlusOne The total time that would be taken to eat all detected prey individuals in all prey cohorts plus one
    @param predatorAbundanceMultipliedByTimeEating The abundance in the predator cohort
    @param preyAbundance The abundance in the prey cohort
    @return The actual abundance of a prey cohort eaten by a predator cohort*/
    double CalculateAbundanceEaten( double, double, double, double );

    /** \brief Calculate the visibility of the prey cohort (currently set to 1)
    @param preyAbundance The abundance in the prey cohort
    @return The visibility of the prey cohort*/
    double CalculateVisibility( double );

private:
    /** \brief Instance of the class to perform general functions */
    UtilityFunctions mUtilities;
    GridCell mGridCell;
    /** \brief Jagged array mirroring the grid cell cohorts to store the potential abundance gained (given the number of encounters) from predation on each cohort */
    std::vector< std::vector < double> > mPotentialAbundanceEaten;
    /** \brief Jagged array mirroring the grid cell cohorts to store the abundance gained from predation on each cohort */
    std::vector< std::vector < double> > mAbundancesEaten;
    /** \brief The matrix to hold the abundance of prey items in each functional group and weight bin */
    std::vector< std::vector< double > > mBinnedPreyDensities;
    /** \brief Number of cohorts in each functional group that were present in the grid cell before this time step's new cohorts were created*/
    std::vector<int> mNumberCohortsPerFunctionalGroupNoNewCohorts;
    /** \brief Identifies which functional groups are carnivores */
    std::vector<bool> mCarnivoreFunctionalGroups;
    /** \brief Identifies which functional groups are carnivores */
    std::vector<bool> mOmnivoreFunctionalGroups;
    /** \brief Identifies which functional groups are carnivores */
    std::vector<bool> mPlanktonFunctionalGroups;

    /** \brief The maximum prey ratio omnivores can feed on, e.g. when set to 0.1 omnivores can only eat prey for which: "acting omnivore weight" < 0.1 * "prey weight"  */
    double mRelativeMaxPreyRatioOmnivores;

    /** \brief The time unit associated with this particular implementation of predation and its parameters */
    std::string mTimeUnitImplementation;
    /** \brief The scalar of the relationship between handling time and the function of predator and prey masses for terrestrial animals*/
    double mHandlingTimeScalarTerrestrial;
    /** \brief The exponent applied to predator mass in the handling time relationship for terrestrial animals*/
    double mHandlingTimeExponentTerrestrial;
    /** \brief The scalar of the relationship between handling time and the function of predator and prey masses for terrestrial animals*/
    double mHandlingTimeScalarMarine;
    /** \brief The exponent applied to predator mass in the handling time relationship for terrestrial animals*/
    double mHandlingTimeExponentMarine;
    /** \brief The reference mass property */
    double mReferenceMass;
    /** \brief The maximum kill rate for a predator of 1 g on prey of an optimal size*/
    double mKillRateConstant;
    /** \brief The exponent on body mass in the relationship between body mass and attack rate*/
    double mKillRateConstantMassExponent;
    /** \brief The standard deviation in attack rates around the optimal prey to predator mass ratio*/
    double mFeedingPreferenceStandardDeviation;
    /** \brief The number of bins in which to combine prey cohorts. THIS SHOULD ALWAYS BE AN EVEN VALUE */
    int mNumberOfBins;

    /** \brief Pre-calculate the specific predator handling time scaling to prevent having to do it for every prey cohort*/
    double mSpecificPredatorHandlingTimeScaling;
    /** \brief Pre-calculate the maximum kill rate for a specific predator of 1 g on prey of an optimal size*/
    double mSpecificPredatorKillRateConstant;
    /** \brief The optimal ratio of prey to predator body masses for terrestrial animals*/
    double mOptimalPreyPredatorMassRatioTerrestrial;
    /** \brief The optimal ratio of prey to predator body masses for marine animals*/
    double mOptimalPreyPredatorMassRatioMarine;
    /** \brief Some variable or other */
    double mRelativeFeedingPreference;
    /** \brief Pre-calculate the proportion of time spent eating (in appropriate time units for this class) for a specific predator*/
    double mSpecificPredatorTimeUnitsEatingPerGlobalTimeStep;
    /** \brief Prey density per hectare; */
    double mPreyDensityPerHectare;
    /** \brief Killing rate of an individual predator per unit prey density per hectare per time unit */
    double mAlphaIJ;
    /** \brief Scalar to convert from the time step units used by this predation implementation to global model time step units */
    double mDeltaT;
    double mCellArea;
    /** \brief The area of the current grid cell in hectares */
    double mCellAreaHectares;
    /** \brief The proportion of biomass eaten assimilated by predators*/
    double mPredatorAssimilationEfficiency;
    /** \brief The proportion of biomass eaten not assimilated by predators */
    double mPredatorNonAssimilation;
    /** \brief bool to indicate if the diet of marine species is "allspecial" */
    bool mDietIsAllSpecial;
    /** \brief double to hold the log optimal prey body size ratio for the acting predator cohort */
    double mPredatorLogOptimalPreyBodySizeRatio;
    /** \brief Individual body mass of the prey cohort */
    double mBodyMassPrey;
	double mAdultBodyMassPrey;
    /** \brief Individual body mass of the acting (predator) cohort */
    double mBodyMassPredator;
    /** \brief Abundance of the acting (predator) cohort */
    double mAbundancePredator;
    double mReferenceMassRatioScalingTerrestrial;
    double mReferenceMassRatioScalingMarine;
    double mPredatorAbundanceMultipliedByTimeEating;
    /** Temporary value to hold calculations */
    double mTemporaryValue;
    /** The mass bin number of an individual prey cohort */
    int mPreyMassBinNumber;
    /** \brief A boolean which monitors whether or not to track individual cohorts*/
    bool mTrackIndividualCohorts;
};

#endif
