#ifndef WMS_H 
#define WMS_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Grid.h"
#include "GridCell.h"
#include "TimeStep.h"
#include "Maths.h"
#include "Parameters.h"
#include "DataRecorder.h"
#include "Types.h"
//#include "Environment.h"

class WriteModelState {
public:

    void CreateOutputSubfolders( std::string, std::string );

    void WriteCohortProperties( Grid&, std::string, unsigned );

    void WriteStockProperties( Grid&, std::string, unsigned );

    void WriteStockTimeline( Grid&, std::string, unsigned );
    
    void WriteMonthlyTimeLineBiomass( std::string, unsigned, std::vector< std::vector<double> > );

    void WriteSpatialGridProperties( Grid&, std::string );

    void WriteBinnedPreyFoodWebConnections( Grid&, unsigned, std::string  );

    void WriteCohortConsumptionSummary( Grid&, unsigned, std::string );

    void WriteCohortBiomassBinned(Grid&, unsigned, std::string, int ); 

private:

    unsigned temp;

    UtilityFunctions mUtilities;
    
};
#endif 