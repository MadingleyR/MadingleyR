#ifndef ACTIVITY
#define ACTIVITY

#include "GridCell.h"

/** \brief Calculates the relative activity rate of a cohort */
class Activity {
public:


    int UseNonDefaultModelParameters;
    std::vector<double> Activity_Parameters;


    /** \brief Constructor for the Activity class: assigns parameter values */
    Activity( );

    /** \brief Initialise parameters related to the activity of cohorts */
    void InitialiseActivityParameters( );

    /** \brief Calculate the proportion of time for which this cohort could be active and assign it to the cohort's properties
    @param gcl the current cell
    @param actingCohort The Cohort for which proportion of time active is being calculated 
    @param params the model parameters 
    @param currentTimestep Current timestep index 
     */
    void AssignProportionTimeActive( GridCell&, Cohort*, unsigned, unsigned, MadingleyInitialisation& );

    /** \brief Calculate the proportion of each timestep for which this cohort is active
    For ectotherms: is a function of the critical max and min temperatures for this ectotherm cohort and also the ambient temperature and diurnal variation in this cell
    Assumes that the diurnal temperature range is symmetrical around the monthly mean temperature
    Alse assumes that the diurnal temperature profile is approximated by a sinusoidal time-series
    Source: Deutsch et al (2008), Impacts of climate warming on terrestrial ecototherms across latitude, PNAS.
    @param gcl The grid cell 
    @param currentMonth Currnent month in the model 
    @param endotherm Boolean indicating if cohort is endotherm or ectotherm (true if endotherm) 
    @return The proportion of the timestep for which this cohort could be active*/
    double CalculateProportionTimeSuitableTerrestrial( GridCell&, unsigned, bool );
    double CalculateProportionTimeSuitableMarine( GridCell&, unsigned, bool );

    /** \brief Calculate the proportion of the current timestep that this cohort is active for
    Is a function of the critical max and min temperatures for this ectotherm cohort and also the ambient temperature and diurnal variation in this cell
    Assumes that the diurnal temperature range is symmetrical around the monthly mean temperature
    Alse assumes that the diurnal temperature profile is approximated by a sinusoidal time-series
    Sin of form:
    //        ///T(h)=Ambient+ [DTR*(0.5*sin(omega*(h-6)))]

    @return The proportion of the day that temperatures are between CTmin and CTmax*/
    double ProportionDaySuitable( );

    /** \brief Intercept of the linear relationship between warming tolerance of terrestrial ectotherms and annual temperature variability */
    double mTerrestrialWarmingToleranceIntercept;
    /** \brief Slope of the linear relationship between warming tolerance of terrestrial ectotherms and annual temperature variability */
    double mTerrestrialWarmingToleranceSlope;
    /** \brief Intercept of the linear relationship between terrestrial safety margin of terrestrial ectotherms and annual temperature variability */
    double mTerrestrialTSMIntercept;
    /** \brief Slope of the linear relationship between terrestrial safety margin of terrestrial ectotherms and annual temperature variability */
    double mTerrestrialTSMSlope;

private:
    /** \brief The distance of the maximum critical temperature from the ambient temperature */
    double mWarmingTolerance;
    /** \brief Distance of the optimal performance temperature from the ambient temperature */
    double mThermalSafetyMargin;
    /** \brief The optimal performance temperature */
    double mTopt;
    /** \brief The maximum critical temperature */
    double mCTmax;
    /** \brief The minimum critical temperature */
    double mCTmin;
    /** \brief The ambient temperature */
    double mAmbientTemp;
    /** \brief The diurnal temperature range */
    double mDTR;
};
#endif
