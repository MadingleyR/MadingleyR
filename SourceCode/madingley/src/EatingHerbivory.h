#ifndef EATINGHERBIVORY
#define EATINGHERBIVORY

#include "Eating.h"
#include "GridCell.h"

/** \brief A revised version of the herbivory process, written November 2011 */
class EatingHerbivory : public Eating {
public:

    int UseNonDefaultModelParameters;
    std::vector<double> EatingHerbivory_Parameters;

    /** \brief Constructor for herbivory: assigns all parameter values
    @param cellArea The area (in square km) of the grid cell
    @param globalModelTimeStepUnit The time step unit used in the model */
    EatingHerbivory( std::string );

    ~EatingHerbivory( );

    /** \brief Calculate the actual amount eaten in herbivory, apply the changes to the eaten autotroph stocks, and update deltas for the herbivore cohort
    @param gcl this grid cell
    @param actingCohort The acting cohort
    @param currentTimestep The current model time step
    @param params  The model parameters */
    void Run( GridCell&, Cohort*, unsigned, MadingleyInitialisation& );

    /** \brief Initialises herbivory implementation each time step
    @param gcl The current grid cell
    @param params The definitions of model parameters
    \remark This only works if: a) herbivory is initialised in every grid cell; and b) if parallelisation is done by latitudinal strips
    It is critical to run this every time step */
    void InitializeEatingPerTimeStep( GridCell&, MadingleyInitialisation& );

    /** \brief Calculate the potential biomass that could be gained through herbivory on each grid cell autotroph stock
    @param gcl The current grid cell
    @param actingCohort The acting cohort
    @param params The definitions for stuff in the model */
    void GetEatingPotentialTerrestrial( GridCell&, Cohort*, MadingleyInitialisation& );

    /** \brief Calculate the potential biomass that could be gained through herbivory on each grid cell autotroph stock
    @param gcl The current grid cell
    @param actingCohort The acting cohort
    @params All your base are belong to us */
    void GetEatingPotentialMarine( GridCell&, Cohort*, MadingleyInitialisation& );

private:

    /** \brief Calculates the potential biomass of an autotroph stock eaten by a herbivore cohort (terrestrial)
    @param autotrophBiomass The total biomass of the autotroph stock
    @param herbivoreIndividualMass The individual body mass of the acting (herbivore) cohort
    @return The potential biomass eaten by the herbivore cohort*/
    double CalculatePotentialBiomassEatenTerrestrial( double, double, Cohort*, MadingleyInitialisation& );

    /** \brief Calculates the potential biomass of an autotroph stock eaten by a herbivore cohort (marine)
    @param autotrophBiomass The total biomass of the autotroph stock
    @param herbivoreIndividualMass The individual body mass of the acting (herbivore) cohort
    @return The potential biomass eaten by the herbivore cohort*/
    double CalculatePotentialBiomassEatenMarine( double, double );

    /** \brief Calculate the herbivory rate of an individual herbivore per unit autotroph mass-density per hectare
    @param herbivoreIndividualMass Herbivore individual body mass
    @return The herbivory rate of an individual herbivore per unit autotroph mass-density per hectare*/
    double CalculateIndividualHerbivoryRatePerHectare( double );

    /** \brief Calculate the time taken for a herbivore in the marine realm to handle unit mass (1 g) of autotroph mass
    @param herbivoreIndividualMass The body mass of an individual herbivore
    @return The time taken for a herbivore to handle unit mass (1 g) of autotroph mass*/
    double CalculateHandlingTimeMarine( double );

    /** \brief Calculate the time taken for a herbivore in the terrestrial realm to handle unit mass (1 g) of autotroph mass
    @param herbivoreIndividualMass The body mass of an individual herbivore
    @return The time taken for a herbivore to handle unit mass (1 g) of autotroph mass*/
    double CalculateHandlingTimeTerrestrial( double );

    /** \brief Calculate the actual biomass eaten by a herbivore cohort from an autotroph stock
    @param potentialBiomassEaten The potential biomass eaten by the herbivore cohort from the autotroph stock given the encounter rate
    @param totalHandlingTime The total time that would be taken to handle all encountered autotroph biomass in all autotroph stocks
    @param herbivoreAbundance The number of individuals in the acting herbivore cohort
    @param autotrophBiomass The total biomass in the autotroph stock
    @return The biomass eaten by the herbivore cohort from the autotroph stock*/
    double CalculateBiomassesEaten( double, double, double, double );

    /** \brief Instance of the class to perform general functions*/
    UtilityFunctions mUtilities;
    /** \brief The time unit associated with this herbivory implementation and its parameters */
    std::string mTimeUnitImplementation;
    /** \brief The scalar of the relationship between handling time and the function of herbivore mass for the terrestrial realm */
    double mHandlingTimeScalarTerrestrial;
    /** \brief The scalar of the relationship between handling time and the function of herbivore mass for the marine realm */
    double mHandlingTimeScalarMarine;
    /** \brief The exponent applied to herbivore mass in the handling time relationship for the terrestrial realm */
    double mHandlingTimeExponentTerrestrial;
    /** \brief The exponent applied to herbivore mass in the handling time relationship for the marine realm */
    double mHandlingTimeExponentMarine;
    /** \brief Reference mass of plant matter for calculating handling times */
    double mReferenceMass;
    /** \brief The maximum herbivory rate for a herbivore of 1 g */
    double mHerbivoryRateConstant;
    /** \brief The exponent to apply to body mass in the relationship between body mass and herbivory rate */
    double mHerbivoryRateMassExponent;
    /** \brief The exponent applied to prey density when calculating attack rates for organisms in the terrestrial realm */
    double mAttackRateExponentTerrestrial;
    /** \brief The exponent applied to prey density when calculating attack rates for organisms in the marine realm */
    double mAttackRateExponentMarine;
    /** \brief Jagged array mirroring the grid cell stocks to store the biomasses eaten in herbivory*/
    std::vector< std::vector< double > > mBiomassesEaten;
    /** \brief Jagged array mirroring the grid cell stocks to store the potential biomasses eaten (given the rate of encounter) in herbivory*/
    std::vector< std::vector< double > > mPotentialBiomassesEaten;
    /** \brief Scalar to convert from the time step units used by this herbivory implementation to global model time step units*/
    double mDeltaT;
    /** \brief The area (in square km) of the grid cell*/
    double mCellArea;
    /** \brief The area of the current grid cell in hectares*/
    double mCellAreaHectares;
    /** \brief Individual body mass of herbivores*/
    double mBodyMassHerbivore;
    /** \brief  Holds the edible plant mass available */
    double mEdibleMass;
    /** \brief  Holds the scaling to get from exstant autotroph biomass to the edible mass*/
    double mEdibleScaling;
    /** \brief Variable to hold the instantaneous fraction of the autotroph stock biomass that is eaten*/
    double mInstantFractionEaten;

    double mProportionEdibleStockMass;


    // Parameters used for binned herbivore feeding
    std::vector<int> bin_borders;
    int n_bins;

};

#endif
