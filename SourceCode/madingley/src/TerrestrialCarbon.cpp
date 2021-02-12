#include "TerrestrialCarbon.h"
#include "InputParameters.h"

TerrestrialCarbon::TerrestrialCarbon( ) {
    // Initialise parameters
    InitialisePlantModelParameters( );
}

void TerrestrialCarbon::InitialisePlantModelParameters( ) {

    UseNonDefaultModelParameters = InputParameters::Get( )->GetUseNonDefaultModelParameters();

    // Assign the parameters for the plant model
    mMaxNPP = 0.961644704; //VegetationModel_Parameters[0] 
    mT1NPP = 0.237468183; //VegetationModel_Parameters[1] 
    mT2NPP = 0.100597089; //VegetationModel_Parameters[2] 
    mPNPP = 0.001184101; //VegetationModel_Parameters[3] 
    mFracStructScalar = 7.154615419; //VegetationModel_Parameters[4] 
    mAFracEvergreen = 1.270782192; //VegetationModel_Parameters[5] 
    mBFracEvergreen = -1.828591558; //VegetationModel_Parameters[6] 
    mCFracEvergreen = 0.844864063; //VegetationModel_Parameters[7] 
    mMEGLeafMortality = 0.040273936; //VegetationModel_Parameters[8] 
    mCEGLeafMortality = 1.013070062; //VegetationModel_Parameters[9] 
    mMDLeafMortality = 0.020575964; //VegetationModel_Parameters[10] 
    mCDLeafMortality = -1.195235464; //VegetationModel_Parameters[11] 
    mMFRootMort = 0.04309283; //VegetationModel_Parameters[12] 
    mCFRootMort = -1.478393163; //VegetationModel_Parameters[13] 
    mP2StMort = 0.139462774; //VegetationModel_Parameters[14] 
    mP1StMort = -4.395910091; //VegetationModel_Parameters[15] 
    mMaxFracStruct = 0.362742634; //VegetationModel_Parameters[16] 
    mLFSHalfSaturationFire = 0.388125108; //VegetationModel_Parameters[17] 
    mLFSScalarFire = 19.98393943; //VegetationModel_Parameters[18] 
    mNPPHalfSaturationFire = 1.148698636; //VegetationModel_Parameters[19] 
    mNPPScalarFire = 8.419032427; //VegetationModel_Parameters[20] 
    mErMin = 0.01; //VegetationModel_Parameters[21] 
    mErMax = 24.0; //VegetationModel_Parameters[22] 
    mDrMin = 0.01; //VegetationModel_Parameters[23] 
    mDrMax = 24.0; //VegetationModel_Parameters[24] 
    mFrmMin = 0.01; //VegetationModel_Parameters[25] 
    mFrmMax = 12.0; //VegetationModel_Parameters[26] 
    mStmMax = 1; //VegetationModel_Parameters[27] 
    mStmMin = 0.001; //VegetationModel_Parameters[28] 
    mBaseScalarFire = 2.0; //VegetationModel_Parameters[29] 

    // changed in most recent version was exp( -13.0 );
    mMinReturnInterval = 0.00000226032940698105;  //VegetationModel_Parameters[30] 

    // mass of Leaf C per g leaf dry matter = 0.4761 g g-1 (from Kattge et al. (2011), TRY- A global database of plant traits, Global Change Biology)
    mMassCarbonPerMassLeafDryMatter = 0.476; //VegetationModel_Parameters[31] 

    // mass of leaf dry matter per g leaf wet matter = 0.213 g g-1 (from Kattge et al. (2011), TRY- A global database of plant traits, Global Change Biology)
    mMassLeafDryMatterPerMassLeafWetMatter = 0.213; //VegetationModel_Parameters[32] 


    if(UseNonDefaultModelParameters==1){
        VegetationModel_Parameters = InputParameters::Get( )->Get_VegetationModel_Parameters();
        mMaxNPP = VegetationModel_Parameters[0]; 
        mT1NPP = VegetationModel_Parameters[1];
        mT2NPP = VegetationModel_Parameters[2];
        mPNPP = VegetationModel_Parameters[3]; 
        mFracStructScalar = VegetationModel_Parameters[4]; 
        mAFracEvergreen = VegetationModel_Parameters[5];
        mBFracEvergreen = VegetationModel_Parameters[6];
        mCFracEvergreen = VegetationModel_Parameters[7];
        mMEGLeafMortality = VegetationModel_Parameters[8]; 
        mCEGLeafMortality = VegetationModel_Parameters[9]; 
        mMDLeafMortality = VegetationModel_Parameters[10]; 
        mCDLeafMortality = VegetationModel_Parameters[11]; 
        mMFRootMort = VegetationModel_Parameters[12]; 
        mCFRootMort = VegetationModel_Parameters[13]; 
        mP2StMort = VegetationModel_Parameters[14]; 
        mP1StMort = VegetationModel_Parameters[15]; 
        mMaxFracStruct = VegetationModel_Parameters[16]; 
        mLFSHalfSaturationFire = VegetationModel_Parameters[17]; 
        mLFSScalarFire = VegetationModel_Parameters[18]; 
        mNPPHalfSaturationFire = VegetationModel_Parameters[19]; 
        mNPPScalarFire = VegetationModel_Parameters[20]; 
        mErMin = VegetationModel_Parameters[21]; 
        mErMax = VegetationModel_Parameters[22]; 
        mDrMin = VegetationModel_Parameters[23]; 
        mDrMax = VegetationModel_Parameters[24]; 
        mFrmMin = VegetationModel_Parameters[25]; 
        mFrmMax = VegetationModel_Parameters[26]; 
        mStmMax = VegetationModel_Parameters[27]; 
        mStmMin = VegetationModel_Parameters[28]; 
        mBaseScalarFire = VegetationModel_Parameters[29]; 
        mMinReturnInterval = VegetationModel_Parameters[30]; 
        mMassCarbonPerMassLeafDryMatter = VegetationModel_Parameters[31]; 
        mMassLeafDryMatterPerMassLeafWetMatter = VegetationModel_Parameters[32]; 
    }

    mM2ToKm2Conversion = 1000000.0;
}

double TerrestrialCarbon::CalculateEquilibriumLeafMass( GridCell& gcl, bool deciduous ) {

    // Calculate annual average temperature
    double MeanTemp = gcl.GetGridcellAnnualTemperature( );//Environment::Get( "AnnualTemperature", gcl );
    //Calculate total annual precipitation
    double TotalPrecip = gcl.GetGridcellTotalPrecip( ); //Environment::Get( "TotalPrecip", gcl );
    // Calculate total annual AET
    double TotalAET = gcl.GetGridcellTotalAET( ); //Environment::Get( "TotalAET", gcl );
    // Calculate NPP using the Miami model
    double NPP = CalculateMiamiNPP( MeanTemp, TotalPrecip );

    // Calculate fractional allocation to structural tissue
    double FracStruct = CalculateFracStruct( NPP );

    // Calculate the fractional allocation of NPP to evergreen plant matter
    //double FracEvergreen = CalculateFracEvergreen( Environment::Get( "Fraction Year Frost", gcl ) );
    double FracEvergreen = CalculateFracEvergreen( gcl.GetGridcellFractionYearFrost( ) );


    // Calculate the fire mortality rate
    //double FireMortRate = CalculateFireMortalityRate( NPP, Environment::Get( "Fraction Year Fire", gcl ) );
    double FireMortRate = CalculateFireMortalityRate( NPP, gcl.GetGridcellFractionYearFire( ) );

    // Update NPP depending on whether the acting stock is deciduous or evergreen
    if( deciduous ) {
        NPP *= ( 1 - FracEvergreen );
    } else {
        NPP *= FracEvergreen;
    }

    // Calculate fine root mortality rate
    double FRootMort = CalculateFineRootMortalityRate( MeanTemp );

    // Calculate the structural mortality rate
    double StMort = CalculateStructuralMortality( TotalAET );

    double LeafMortRate;

    if( deciduous ) {
        // Calculate deciduous leaf mortality
        LeafMortRate = CalculateDeciduousAnnualLeafMortality( MeanTemp );

    } else {
        // Calculate evergreen leaf mortality
        LeafMortRate = CalculateEvergreenAnnualLeafMortality( MeanTemp );
    }

    // Calculate the fractional mortality of leaves
    double LeafMortFrac = CalculateLeafFracAllocation( LeafMortRate,
            CalculateDeciduousAnnualLeafMortality( MeanTemp ),
            CalculateEvergreenAnnualLeafMortality( MeanTemp ), FracEvergreen, FRootMort );

    // Calculate leaf C fixation
    double LeafCFixation = NPP * ( 1 - FracStruct ) * LeafMortFrac;

    // Calculate leaf carbon mortality
    double LeafCMortality = LeafMortRate + FireMortRate + StMort;

    // Calculate equilibrium leaf carbon in kg C per m2
    double EquilibriumLeafCarbon = LeafCFixation / LeafCMortality;

    // Convert to equilibrium leaf wet matter content
    double LeafWetMatter = ConvertToLeafWetMass( EquilibriumLeafCarbon, gcl.GetCellArea( ) );
    return LeafWetMatter;
}

double TerrestrialCarbon::UpdateLeafStock( GridCell& gcl, Stock& actingStock, unsigned currentTimeStep, bool deciduous, std::string GlobalModelTimeStepUnit, unsigned currentMonth ) {

    // ESTIMATE ANNUAL LEAF CARBON FIXATION ASSUMING ENVIRONMENT THROUGHOUT THE YEAR IS THE SAME AS IN THIS MONTH
    // Get annual average temperature

    double MeanTemp = gcl.GetGridcellAnnualTemperature( );//Environment::Get( "AnnualTemperature", gcl );
    //Calculate total annual precipitation
    double TotalPrecip = gcl.GetGridcellTotalPrecip( ); //Environment::Get( "TotalPrecip", gcl );
    // Calculate annual NPP
    double NPP = CalculateMiamiNPP( MeanTemp, TotalPrecip );

    // Calculate fractional allocation to structural tissue
    double FracStruct = CalculateFracStruct( NPP );

    // Estimate monthly NPP based on seasonality layer
    NPP *= gcl.GetGridcellSeasonality( currentMonth );//Environment::Get( "Seasonality", gcl );

    // Calculate leaf mortality rates
    double AnnualLeafMortRate;
    double MonthlyLeafMortRate;
    double TimeStepLeafMortRate;

    if( deciduous ) {
        // Calculate annual deciduous leaf mortality
        AnnualLeafMortRate = CalculateDeciduousAnnualLeafMortality( MeanTemp );

        // For deciduous plants monthly leaf mortality is weighted by temperature deviance from the average, to capture seasonal patterns
        double Weight = gcl.GetGridcellExpTDevWeight( currentMonth ); //Environment::Get( "ExpTDevWeight", gcl );
        MonthlyLeafMortRate = AnnualLeafMortRate * Weight;
        TimeStepLeafMortRate = MonthlyLeafMortRate * mUtilities.ConvertTimeUnits( GlobalModelTimeStepUnit, "month" );
    } else {
        // Calculate annual evergreen leaf mortality
        AnnualLeafMortRate = CalculateEvergreenAnnualLeafMortality( MeanTemp );

        // For evergreen plants, leaf mortality is assumed to be equal throughout the year
        MonthlyLeafMortRate = AnnualLeafMortRate * ( 1.0 / 12.0 );
        TimeStepLeafMortRate = MonthlyLeafMortRate * mUtilities.ConvertTimeUnits( GlobalModelTimeStepUnit, "month" );
    }

    // Calculate fine root mortality rate
    double AnnualFRootMort = CalculateFineRootMortalityRate( gcl.GetGridcellNearSurfaceTemp( currentMonth ) ); // Environment::Get( "Temperature", gcl )

    // Calculate the NPP allocated to non-structural tissues
    double FracNonStruct = ( 1 - FracStruct );

    // Calculate the fractional allocation of NPP to evergreen plant matter
    double FracEvergreen = CalculateFracEvergreen( gcl.GetGridcellFractionYearFire( ) ); //Environment::Get( "Fraction Year Frost", gcl )

    // Calculate the fractional allocation to leaves
    double FracLeaves = FracNonStruct * CalculateLeafFracAllocation( AnnualLeafMortRate, CalculateDeciduousAnnualLeafMortality( MeanTemp ), CalculateEvergreenAnnualLeafMortality( MeanTemp ), FracEvergreen, AnnualFRootMort );

    // Update NPP depending on whether the acting stock is deciduous or evergreen
    if( deciduous ) {
        NPP *= ( 1 - FracEvergreen );
    } else {
        NPP *= FracEvergreen;
    }

    // Calculate the fire mortality rate
    double FireMortRate = CalculateFireMortalityRate( NPP, gcl.GetGridcellFractionYearFire( ) ); //Environment::Get( "Fraction Year Frost", gcl )

    // Calculate the structural mortality rate
    double StMort = CalculateStructuralMortality( gcl.GetGridcellAET( currentMonth ) * 12 ); //Environment::Get( "AET", gcl )

    // Calculate leaf C fixation
    double LeafCFixation = NPP * FracLeaves;

    // Convert from carbon to leaf wet matter
    double WetMatterIncrement = ConvertToLeafWetMass( LeafCFixation, gcl.GetCellArea( ) );
    // Convert from the monthly time step used for this process to the global model time step unit
    WetMatterIncrement *= mUtilities.ConvertTimeUnits( GlobalModelTimeStepUnit, "month" );

    // Add the leaf wet matter to the acting stock - this line has been change in the latest version!
    // actingStock.TotalBiomass += max( -actingStock.TotalBiomass, WetMatterIncrement );
    double NPPWetMatter = std::max( -actingStock.mTotalBiomass, WetMatterIncrement );


    // Calculate fractional leaf mortality
    double LeafMortFrac = 1 - exp( -TimeStepLeafMortRate );

    // Update the leaf stock biomass owing to the leaf mortality
    actingStock.mTotalBiomass *= ( 1 - LeafMortFrac );
    NPPWetMatter *= ( 1 - LeafMortFrac );

    return NPPWetMatter;

}

double TerrestrialCarbon::CalculateMiamiNPP( double temperature, double precipitation ) {
    // Calculate the maximum annual NPP that could be sustained if average temperature were equal to this month's temperature
    double NPPTemp = mMaxNPP / ( 1 + exp( mT1NPP - mT2NPP * temperature ) );

    // Calculate the maximum annual NPP that could be sustained if precipitation in every other month was equal to this month's precipitation
    double NPPPrecip = mMaxNPP * ( 1 - exp( -mPNPP * precipitation ) );

    // Calculate the maximum annual NPP that could be sustained based on temperature and precipitation
    return std::min( NPPTemp, NPPPrecip );
}

double TerrestrialCarbon::CalculateFracStruct( double NPP ) {
    double MinFracStruct = 0.01; // This prevents the prediction becoming zero (makes likelihood calculation difficult)
    double FracStruc = MinFracStruct * ( exp( mFracStructScalar * NPP ) / ( 1 + MinFracStruct * ( exp( mFracStructScalar * NPP ) - 1.0 ) ) );
    if( FracStruc > 0.99 ) FracStruc = 1 - MinFracStruct;
    FracStruc *= mMaxFracStruct;
    return FracStruc;
}

double TerrestrialCarbon::CalculateFracEvergreen( double NDF ) {
    double imed1 = mAFracEvergreen * NDF * NDF + mBFracEvergreen * NDF + mCFracEvergreen;
    if( imed1 < 0 ) imed1 = 0;
    if( imed1 > 1 ) imed1 = 1;
    return imed1;
}

double TerrestrialCarbon::CalculateEvergreenAnnualLeafMortality( double temperature ) {
    double EstimatedRate = exp( mMEGLeafMortality * temperature - mCEGLeafMortality );
    if( EstimatedRate > mErMax ) EstimatedRate = mErMax;
    if( EstimatedRate < mErMin ) EstimatedRate = mErMin;
    return EstimatedRate;
}

double TerrestrialCarbon::CalculateDeciduousAnnualLeafMortality( double temperature ) {
    double EstimatedRate = exp( -( mMDLeafMortality * temperature + mCDLeafMortality ) );
    if( EstimatedRate > mDrMax ) EstimatedRate = mDrMax;
    if( EstimatedRate < mDrMin ) EstimatedRate = mDrMin;
    return EstimatedRate;
}

double TerrestrialCarbon::CalculateLeafFracAllocation( double LeafMortRate, double DecidLeafMortRate, double EvergreenLeafMortRate, double FracEvergreen, double FRootMort ) {
    return LeafMortRate / ( LeafMortRate + FRootMort );
}

double TerrestrialCarbon::CalculateFineRootMortalityRate( double temperature ) {
    double EstimatedRate = exp( mMFRootMort * temperature + mCFRootMort );
    if( EstimatedRate > mFrmMax ) EstimatedRate = mFrmMax;
    if( EstimatedRate < mFrmMin ) EstimatedRate = mFrmMin;
    return EstimatedRate;
}

double TerrestrialCarbon::CalculateFireMortalityRate( double NPP, double FractionYearFireSeason ) {
    double NPPFunction = ( 1.0 / ( 1.0 + exp( -mNPPScalarFire * ( NPP - mNPPHalfSaturationFire ) ) ) );
    double LFSFunction = ( 1.0 / ( 1.0 + exp( -mLFSScalarFire * ( FractionYearFireSeason - mLFSHalfSaturationFire ) ) ) );
    double TempRate = mBaseScalarFire * NPPFunction * LFSFunction;
    if( TempRate > 1.0 ) TempRate = 1.0;
    double Rate = ( TempRate <= mMinReturnInterval ) ? mMinReturnInterval : TempRate;
    return Rate;
}

double TerrestrialCarbon::CalculateStructuralMortality( double AET ) {
    double power = mP2StMort * AET / 1000 + mP1StMort;
    double EstimatedRate = exp( power );
    if( EstimatedRate > mStmMax ) EstimatedRate = mStmMax;
    if( EstimatedRate < mStmMin ) EstimatedRate = mStmMin;
    return EstimatedRate;
}

double TerrestrialCarbon::CalculateLeafCarbon( double NPP, double FracStruct, double LeafMortFrac, double LeafMortRate, double FireMortRate, double StMort ) {
    double LeafCFixation = CalculateLeafCFixation( NPP, FracStruct, LeafMortFrac );
    double LeafCMortality = LeafMortRate + FireMortRate + StMort;
    return LeafCFixation / LeafCMortality;
}

double TerrestrialCarbon::CalculateLeafCFixation( double NPP, double FracStruct, double LeafMortFrac ) {
    return NPP * ( 1 - FracStruct * mMaxFracStruct ) * LeafMortFrac;
}

double TerrestrialCarbon::ConvertToLeafWetMass( double kgCarbon, double cellArea ) {
    // Convert from kg to g
    double gCarbonPerM2 = kgCarbon * 1000;

    // Convert from m2 to km2
    double gCarbonPerKm2 = gCarbonPerM2 * mM2ToKm2Conversion;

    // Convert from km2 to cell area
    double gCarbonPerCell = gCarbonPerKm2 * cellArea;

    // Convert from g C to g dry matter
    // mass of Leaf C per g leaf dry matter = 0.4761 g g-1 (from Kattge et al. (2011), TRY- A global database of plant traits, Global Change Biology)
    double LeafDryMatter = gCarbonPerCell / mMassCarbonPerMassLeafDryMatter;

    // Convert from dry matter to wet matter
    // mass of leaf dry matter per g leaf wet matter = 0.213 g g-1 (from Kattge et al. (2011), TRY- A global database of plant traits, Global Change Biology)
    return LeafDryMatter / mMassLeafDryMatterPerMassLeafWetMatter;

}

double TerrestrialCarbon::ConvertToKgCarbonPerM2( double leafWetMatter, double cellArea ) {
    // Convert from wet matter to dry matter
    double LeafDryMatter = leafWetMatter / 2;

    // Convert from dry matter to g C per grid cell
    double gCarbonPerCell = LeafDryMatter * 2;

    // Convert from cell area to km2
    double gCarbonPerKm2 = gCarbonPerCell / cellArea;

    // Convert from km2 to m2
    double gCarbonPerM2 = gCarbonPerKm2 / mM2ToKm2Conversion;

    // Convert from g carbon to kg carbon
    return gCarbonPerM2 / 1000;
}
