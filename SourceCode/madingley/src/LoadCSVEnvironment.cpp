#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <dirent.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "GridDatum.h"
#include "BasicDatum.h"
#include "Stopwatch.h"
#include "ThreadVariables.h"
#include "TimeStep.h"
#include "Maths.h"
#include "Parameters.h"
#include "DataRecorder.h"
#include "Types.h"
#include "LoadCSVEnvironment.h"
#include <fstream>
#include "Grid.h"
#include "GridCell.h"
#include "InputData.h"
#include "DataLayerSet.h"
#include "DataRecorder.h"


std::vector <std::vector<double>> LoadCSVEnvironment::Read_Env_CSV( std::string file_path, int print ) {
    
    // if(print==1){
    //     std::cout << " " << std::endl;
    //     std::cout << "====================================================" << std::endl;
    //     std::cout << "=================== read env csv ===================" << std::endl;
    //     std::cout << "====================================================" << std::endl;
    // }
    
    if(print==1) std::cout << "Reading spatial csv input: " << file_path << "..." << std::endl;
    
    // open a file in read mode.
    std::ifstream csv(file_path);
    std::string line;
    std::vector <std::vector<std::string>> items;

    if (csv.is_open()) {
            for (std::string row_line; std::getline(csv, row_line);)
            {
                items.emplace_back();
                std::istringstream row_stream(row_line);
                for(std::string column; std::getline(row_stream, column, ',');)
                    items.back().push_back(column);
            }
    }
    else {
        cout << "Unable to open file";
    }

    // if(print==1){
    //     std::cout << " " << std::endl;
    //     std::cout << "cols: " << items[0].size() << std::endl; // expects 3 cols: var, long, lat
    //     std::cout << "rows: " << items.size() << std::endl; // expects 64801 rows: n cells + header
    //     std::cout << " " << std::endl;
    // }

    std::vector <std::vector<double>> items_doubles( items.size()-1, std::vector<double>(3, 0.0) );
    for(int i = 1; i < items.size(); i++){  // start from 1 not 0 ==> removes header
        
        
        
        if(std::stod(items[i][0])==-999){
            items_doubles[i-1][0] = Constants::cMissingValue;
        }else{
            items_doubles[i-1][0] = std::stod(items[i][0]);
        }

        items_doubles[i-1][1] = std::stod(items[i][1]);
        items_doubles[i-1][2] = std::stod(items[i][2]);
    }
    
    // if(print==1){
    //     std::cout << "head csv file: " << std::endl;
    //     for(int i = 1; i < 10; i++){  // start from 1 not 0 ==> removes header
    //         std::cout << items_doubles[i][0] << " " << items_doubles[i][1] << " " << items_doubles[i][2] << std::endl;
    //     }

    //     std::cout << "====================================================";
    //     std::cout << "====================================================";
    //     std::cout << " " << std::endl;
    // }

    return items_doubles;
}


std::vector<double> LoadCSVEnvironment::ExtractVarGrid(std::vector <std::vector<double>> test_env_vec1, int Xmin, int Xmax, int Ymin, int Ymax, int print){

    //# extract values from imported csv using:
    //# Xmin; Xmax; Ymin; Ymax;
    //# col 0: var, 
    //# col 1: long 
    //# col 2: lat
    
    int NewYmax;
    int NewXmax;

    NewYmax = Ymax+1;
    NewXmax = Xmax+1;

    

    int cell_counter = 0;
    int long_index_counter = 0;
    int lat_index_counter = 0;
    double store_long = 0.0;
    double store_long_init = 0.0;
    double store_lat = 0.0;
    std::vector<double> test_holder_vec; 
    for(int i = 0; i < test_env_vec1.size(); i++){
        if(test_env_vec1[i][1]>Xmin && test_env_vec1[i][1]<NewXmax){
            if(test_env_vec1[i][2]>Ymin && test_env_vec1[i][2]<NewYmax){
                
                if(cell_counter==0) {
                    store_long = test_env_vec1[i][1];
                    store_long_init = test_env_vec1[i][1];
                    store_lat = test_env_vec1[i][2];
                }
                if(store_long_init==test_env_vec1[i][1]) {
                    long_index_counter = 0;
                    store_long = store_long_init;
                }
                if(store_long<test_env_vec1[i][1]) {
                    store_long = test_env_vec1[i][1];
                    long_index_counter++;
                }
                if(store_lat<test_env_vec1[i][2]) {
                    store_lat = test_env_vec1[i][2];
                    lat_index_counter++;
                }
                if(print==1){
                    std::cout << "   gc index: " << setw(8) <<  cell_counter << " var: " << setw(20) << test_env_vec1[i][0] << 
                                "   x: " << setw(8) << test_env_vec1[i][1] << "   y: " << setw(8) << test_env_vec1[i][2] << 
                                "   long_index: " << setw(8) << long_index_counter << "   lat_index: " << setw(8) << lat_index_counter << std::endl;
                }
                cell_counter++;
                test_holder_vec.push_back(test_env_vec1[i][0]);
            }
        }
    }
    if(print==1) std::cout << std::endl;
    return test_holder_vec;
}