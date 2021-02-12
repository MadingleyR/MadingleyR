#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Grid.h"
#include "GridCell.h"

//#include "Environment.h"
#include "TimeStep.h"
#include "Maths.h"
#include "Parameters.h"
#include "DataRecorder.h"
#include "Types.h"
#include "Constants.h"

class LoadCSVEnvironment {
public:

    // function for loading single spatial input var (csv)
    std::vector <std::vector<double>> Read_Env_CSV( std::string, int );

    // function loading env var into model
    std::vector<double> ExtractVarGrid( std::vector <std::vector<double>>, int, int, int, int, int ); 


private:
    unsigned temp;
};
