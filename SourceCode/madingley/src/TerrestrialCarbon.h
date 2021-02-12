#ifndef TERRESTRIALCARBON
#define TERRESTRIALCARBON

#include "Stock.h"
#include "GridCell.h"
#include "UtilityFunctions.h"

#include <cmath>

/** \brief Revised version of Matt Smith's terrestrial carbon model */
class TerrestrialCarbon {
public:

    int UseNonDefaultModelParameters;
    std::vector<double> VegetationModel_Parameters;


    /** \brief Constructor for the plant model */
    TerrestrialCarbon( );

    /** \brief Estimate the mass of leaves in a specified stock in the specified grid cell at equilibrium, given current environmental conditions
    @param gcl The current grid cell 
    @param deciduous Whether the leaves in the specified stock are deciduous 
    @return The equilibrium mass of leaves in the specified stock*/
    double CalculateEquilibriumLeafMass( GridCell&, bool );

    /** \brief Update the leaf stock during a time step given the environmental conditions in the grid cell
    @param gcl The current grid cell 
    @param actingStock The acting stock  
    @param currentTimeStep The current model time step 
    @param deciduous Whether the acting stock consists of deciduous leaves 
    @param GlobalModelTimeStepUnit The time step unit used in the model 
    @param currentMonth The current model month */
    double UpdateLeafStock( GridCell&, Stock&, unsigned, bool, std::string, unsigned );
    
private:
    /** \brief Initialise parameters for the plant model */
    void InitialisePlantModelParameters( );
    
    /** \brief Calculate NPP in kg C per m2
    @param temperature Current temperature, in degrees Celsius 
    @param precipitation Current precipitation, in mm 
    @return */
    double CalculateMiamiNPP( double, double );
    
    /** \brief Calculate the fractional allocation of productivity to structural tissue
    @param NPP Net primary productivity 
    @return The fractional allocation of productivity to structural tissue */
    double CalculateFracStruct( double );
    
    /** \brief Calculate the fractional allocation of productivity to evergreen plant matter
    @param NDF The proportion of the current year subject to frost 
    @return The fractional allocation of productivity to evergreen plant matter */
    double CalculateFracEvergreen( double );
    
    /** \brief Calculate the mortality rate of evergreen leaves
    @param temperature Current temperature, in degrees Celsius 
    @return The mortality rate of evergreen leaves */
    double CalculateEvergreenAnnualLeafMortality( double );
    
    /** \brief Calculate the mortality rate of deciduous leaves
    @param temperature Current temperature, in degrees Celsius 
    @return The mortality rate of deciduous leaves */
    double CalculateDeciduousAnnualLeafMortality( double );
    
    /** \brief Calculate the fraction of NPP allocated to non-structural tissue that is allocated to leaves
    @param LeafMortRate The mortality rate of leaves 
    @param FRootMort The mortality rate of fine roots 
    @return The fractional mortality of leaves*/
    double CalculateLeafFracAllocation( double, double, double, double, double );
    
    /** \brief Calculate the mortality rate of fine roots
    @param temperature Current temperature, in degrees Celsius 
    @return The mortality rate of fine roots*/
    double CalculateFineRootMortalityRate( double );
    
    /** \brief Calculate the rate of plant mortality to fire
    @param NPP Net Primary Productivity, in kg C per m2 
    @param FractionYearFireSeason The fraction of the year subject to fires 
    @return The rate of plant mortality to fire*/
    double CalculateFireMortalityRate( double, double );
    
    /** \brief Calculate the mortality rate of plant structural tissue
    @param AET Actual evapotranspiration, in mm 
    @return The mortality rate of plant structural tissue*/
    double CalculateStructuralMortality( double );
    
    /** \brief Calculate leaf carbon, in kg C per m2
    @param NPP Net Primary Productivity, in kg C per m2 
    @param FracStruct Fractional allocation to structural tissue 
    @param LeafMortFrac Fractional mortality of leaves 
    @param LeafMortRate Rate of mortality of leaves 
    @param FireMortRate Rate of mortality to fire 
    @param StMort Rate of mortality of structural tissue 
    @return Leaf carbon, in kg C per m2*/
    double CalculateLeafCarbon( double, double, double, double, double, double );
    
    /** \brief Calculate the carbon fixed by leaves, in kg C per m2
    @param NPP Net Primary Productivity, in kg C per m2 
    @param FracStruct Fractional allocation to structural tissue 
    @param LeafMortFrac Fractional mortality of leaves 
    @return The carbon fixed by leaves, in kg C per m2*/
    double CalculateLeafCFixation( double, double, double );
    
    /** \brief Convert from kg C per m2 to g of leaf wet matter in the entire grid cell
    @param kgCarbon Value to convert, in kg C per m2 
    @param cellArea The area of the grid cell 
    @return Value in g of wet matter in the grid cell*/
    double ConvertToLeafWetMass( double, double );
    
    /** \brief Convert from g of plant wet matter in the entire grid cell to kg C per m2
    @param leafWetMatter The value to convert as total g wet matter in the grid cell 
    @param cellArea The area of the grid cell 
    @return Value in kg C per m2 */
    double ConvertToKgCarbonPerM2( double, double );
    
    /** \brief Instance of the class to perform general functions */
    UtilityFunctions mUtilities;
    /** \brief The maximum possible NPP (kg C per m2 per year) */
    double mMaxNPP;
    /** \brief First constant in the logistic function relating NPP to temperature in the Miami NPP model */
    double mT1NPP;
    /** \brief Second constant in the logistic function relating NPP to temperature in the Miami NPP model  */
    double mT2NPP;
    /** \brief Constant in the saturating function relating NPP to precipitation in the Miami NPP model */
    double mPNPP;
    /** \brief Scalar relating the fraction of NPP devoted to structural tissue to the total amount of NPP */
    double mFracStructScalar;
    /** \brief Coefficient for the quadratic term in the function relating fractional allocation in evergreen leaf matter to fraction of the year experiencing frost */
    double mAFracEvergreen;
    /** \brief Coefficient for the linear term in the function relating fractional allocation in evergreen leaf matter to fraction of the year experiencing frost */
    double mBFracEvergreen;
    /** \brief Intercept in the function relating fractional allocation in evergreen leaf matter to fraction of the year experiencing frost */
    double mCFracEvergreen;
    /** \brief The slope of the relationship between temperature and evergreen leaf mortality rate */
    double mMEGLeafMortality;
    /** \brief The intercept of the relationship between temperature and evergreen leaf mortality rate */
    double mCEGLeafMortality;
    /** \brief The minimum rate of evergreen leaf mortality */
    double mErMin;
    /** \brief The maximum rate of evergreen leaf mortality */
    double mErMax;
    /** \brief The slope of the relationship between temperature and deciduous leaf mortality rate */
    double mMDLeafMortality;
    /** \brief The intercept of the relationship between temperature and deciduous leaf mortality rate */
    double mCDLeafMortality;
    /** \brief The minimum rate of deciduous leaf mortality */
    double mDrMin;
    /** \brief The maximum rate of deciduous leaf mortality */
    double mDrMax;
    /** \brief The slope of the relationship between fine root mortality rate and temperature */
    double mMFRootMort;
    /** \brief The intercept of the relationship between fine root mortality rate and temperature */
    double mCFRootMort;
    /** \brief The minimum rate of fine root mortality */
    double mFrmMin;
    /** \brief The maximum rate of fine root mortality */
    double mFrmMax;
    /** \brief Scalar relating fire mortality rate to NPP */
    double mNPPScalarFire;
    /** \brief NPP at which fire mortality reaches half its maximum rate */
    double mNPPHalfSaturationFire;
    /** \brief Scalar relating fire mortality rate to the fractional fire season length */
    double mLFSScalarFire;
    /** \brief The fractional fire season length at which fire mortality reaches half its maximum rate */
    double mLFSHalfSaturationFire;
    /** \brief Base scalar for the fire mortality function */
    double mBaseScalarFire;
    /** \brief Minimum fire return interval */
    double mMinReturnInterval;
    /** \brief Second parameter in the structural mortality function */
    double mP2StMort;
    /** \brief First parameter in the structural mortality function */
    double mP1StMort;
    /** \brief Maximum rate of structural mortality */
    double mStmMax;
    /** \brief Minimum rate of structural mortality */
    double mStmMin;
    /** \brief The maximum fraction of productivity that can be allocated to structural tissue */
    double mMaxFracStruct;
    /** \brief Scalar to convert between mass of carbon and mass of leaf dry matter */
    double mMassCarbonPerMassLeafDryMatter;
    /** \brief Scalar to convert between mass of lead dry and mass of leaf wet matter */
    double mMassLeafDryMatterPerMassLeafWetMatter;
    /** \brief Constant to convert from m2 to km2 */
    double mM2ToKm2Conversion;
};

#endif
