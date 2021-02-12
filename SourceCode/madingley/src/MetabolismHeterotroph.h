#ifndef METABOLISMHETEROTROPH
#define METABOLISMHETEROTROPH

#include "Metabolism.h"
#include "UtilityFunctions.h"

/** \brief A formulation of the metabolism process 
\remarks Functional form and parameters taken from fitted relationship in Brown's (2004) Metabolic Theory of Ecology.
 Currently mass assigned to reproductive potential is not metabolised */
class MetabolismHeterotroph : public Metabolism {
    
    /** \brief Constructor for metabolism: assigns all parameter values
     \remarks Most parameters currently drawn from Brown's (2004) Metabolic Theory of Ecology
    The scalar to convert kJ to grams mass currently a very rough estimate based on the calorific values
    of fat, protein and carbohydrate */
    MetabolismHeterotroph( std::string );

    /** \brief Run metabolism for the acting cohort
    @param actingCohort The position of the acting cohort in the jagged array of grid cell cohorts 
    @param currentTimestep The current model time step
    @param currentMonth the current month as an integer */
    void Run( Cohort*, unsigned, unsigned );


    int UseNonDefaultModelParameters;
    std::vector<double> MetabolismHeterotroph_Parameters;


private:
    /** \brief Calculate metabolic loss in grams for an individual
    @param individualBodyMass The body mass of individuals in the acting cohort 
    @param temperature The ambient temperature, in degrees Kelvin 
    @return The metabolic loss for an individual*/
    double CalculateIndividualMetabolicRate( double, double );

    /** \brief Include Utility class */
    UtilityFunctions mUtilities;
    /** \brief Time units associated with the formulation of metabolism */
    std::string mTimeUnitImplementation;
    /** \brief Exponent describing the mass-dependency of metabolic rate */
    double mMetabolismMassExponent;
    /** \brief Normalization constatnt for metabolic rate  (independent of mass and temperature) */
    double mNormalizationConstant;
    /** \brief The activation energy of metabolism */
    double mActivationEnergy;
    /** \brief Boltzmann's constant */
    double mBoltzmannConstant;
    /** \brief Scalar to convert energy in kJ to energy in grams mass */
    double mEnergyScalar;
    /** \brief Constant to convert temperature in degrees Celsius to temperature in Kelvin */
    double mTemperatureUnitsConvert;
    /** \brief Scalar to convert from the time step units used by this formulation of dispersal to global model time step units */
    double mDeltaT;
};
#endif
