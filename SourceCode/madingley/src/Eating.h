#ifndef EATING
#define EATING

#include "FunctionalGroupDefinitions.h"
#include "MadingleyInitialisation.h"
#include <map>

/** \brief //    /// Interface for implementations of the ecological process of eating */
class Eating {
public:
    Eating( ) {
        ;
    }

    Eating( std::string globalModelTimeStepUnit ) {
        std::cout << "Virtual IEatingImplementation constructor called:This is probably a mistake" << std::endl;
    }

    virtual ~Eating( ) {
        ;
    }

    /** \brief Initialises eating implementation each time step
    @param gcl The current grid cell
    @param madingleyStockDefinitions The definitions for stocks in the model  */
    virtual void InitializeEatingPerTimeStep( GridCell&, MadingleyInitialisation& ) {
        ;
    }

    /** \brief Calculate the potential biomass that could be gained through eating for marine cells
    @param gcl The current grid cell
    @param params The current model settings
     */
    virtual void GetEatingPotentialMarine( GridCell&, Cohort*, MadingleyInitialisation& ) {
        ;
    }

    /** \brief Calculate the potential biomass that could be gained through eating for terrestrial cells
    @param gcl The current grid cell
    @param actingCohort The position of the acting cohort in the jagged array of cohorts
    @param params The current model thingies  */
    virtual void GetEatingPotentialTerrestrial( GridCell&, Cohort*, MadingleyInitialisation& ) {
        ;
    }

    /** \brief Calculate the actual biomass eaten from each cohort or sotck, apply changes from eating to the cohorts or stocks eaten, and update deltas for the acting cohort
    @param gcl The current grid cell
    @param currentTimestep The current model time step
    @param params the actual model settings  */
    virtual void Run( GridCell&, Cohort*, unsigned, MadingleyInitialisation& ) {
        std::cout << "Top level IEatingImplementation RunEating process called: should be virtual so this is probably not what you want!" << std::endl;
    }

    /** \brief List of functional group indices to act on*/
    std::vector< int > mFunctionalGroupIndicesToEat;
    /** \brief Assimilation efficiency of food mass into acting cohort mass*/
    double mAssimilationEfficiency;
    /** \brief Proportion of time spent eating*/
    double mProportionTimeEating;
    /** \brief Time to handle all prey cohorts or plant mass encountered*/
    double mTimeUnitsToHandlePotentialFoodItems;
    /** \brief The total biomass eaten by the acting cohort */
    double mTotalBiomassEatenByCohort;
};
#endif
