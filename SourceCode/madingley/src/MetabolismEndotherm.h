#ifndef METABOLISMENDOTHERM
#define METABOLISMENDOTHERM

#include "Metabolism.h"
#include "UtilityFunctions.h"

/** \brief A formulation of the metabolism process for Endothermic organisms 
\remarks Functional form and parameters taken from fitted relationship in Brown's (2004) Metabolic Theory of Ecology.
Currently mass assigned to reproductive potential is not metabolised
Assumes that endothermic organisms metabolise at 37degC, and that they can adapt physiologicaly to do this without extra costs */
class MetabolismEndotherm : public Metabolism {
public:

    int UseNonDefaultModelParameters;
    std::vector<double> MetabolismEndotherm_Parameters;

    /** \brief Constructor for metabolism: assigns all parameter values
    \remarks Metabolism exponent and normalization constant calculated based on Nagy et al (1999) field metabolic rates.
    Use the Brown (2004) functional form and take the activation energy for metabolism from there
    The scalar to convert kJ to grams mass currently a very rough estimate based on the calorific values
    of fat, protein and carbohydrate */
    MetabolismEndotherm( std::string );

    /** \brief Run metabolism for the acting cohort
    @param actingCohort The position of the acting cohort in the jagged array of grid cell cohorts 
    @param currentTimestep The current model time step 
    @param currentMonth The current model month */
    void Run( Cohort*, unsigned, unsigned );

private:
    /** \brief Calculate metabolic loss in grams for an individual 
   @param individualBodyMass The body mass of individuals in the acting cohort 
   @param temperature The ambient temperature, in degrees Kelvin 
   @return The metabolic loss for an individual*/
    double CalculateIndividualMetabolicRate( double, double );

    /** \brief Instance of the class to perform general functions */
    UtilityFunctions mUtilities;
    /** \brief Time units associated with the formulation of metabolism */
    std::string mTimeUnitImplementation;
    /** \brief Exponent describing the mass-dependency of metabolic rate*/
    double mMetabolismMassExponent;
    /** \brief Normalization constant for field metabolic rate  (independent of mass and temperature)*/
    double mNormalizationConstant;
    /** \brief The activation energy of metabolism*/
    double mActivationEnergy;
    /** \brief Boltzmann's constant*/
    double mBoltzmannConstant;
    /** \brief Scalar to convert energy in kJ to energy in grams mass*/
    double mEnergyScalar;
    /** \brief Scalar value for endotherm body temperature*/
    double mEndothermBodyTemperature;
    /** \brief Constant to convert temperature in degrees Celsius to temperature in Kelvin */
    double mTemperatureUnitsConvert;
    /** \brief Scalar to convert from the time units used by this metabolism implementation to the global model time step units */
    double mDeltaT;
};
#endif
