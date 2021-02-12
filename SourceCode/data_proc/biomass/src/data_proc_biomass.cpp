#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include "data_proc_biomass.h"
#include <algorithm>   

int main(int argc, char* argv[])
{  
    std::vector<double> temp_vec(9000*10,0.0);
    int gc_max = 0;
    int fg_max = 0;
    #pragma omp parallel shared(temp_vec,gc_max,fg_max)
    {
    //#pragma omp parallel for 
    #pragma omp for
    for(int file = 1; file < 13; file++){
    //-----------------------------------------------------------------------------
        
        // read columns (1, 2, 5, 6, 11)
        std::string csv_File_name = argv[file];
		csv_File_name.erase(remove( csv_File_name.begin(), csv_File_name.end(), '\"' ),csv_File_name.end());
        std::ifstream  data(csv_File_name);
        int row_count =0 ;

        std::vector<int> GridcellIndex; // column 1
        std::vector<int> FunctionalGroupIndex; // column 2
        std::vector<double> IndividualBodyMass; // column 5
        std::vector<double> CohortAbundance; // column 6
        std::vector<double> IndividualReproductivePotentialMass; // column 11
        
        std::string line;
        while(std::getline(data,line))
        {   
            row_count +=1;

            std::stringstream  lineStream(line);
            std::string        cell;
            int column_count = 0 ;
            double d;
    
            if( row_count > 1 ){
                while(std::getline(lineStream,cell,','))
                {
                    column_count+=1;
                    if( column_count == 1 ){
                        GridcellIndex.push_back(std::stoi(cell));
                    }else if( column_count == 2 ){
                        FunctionalGroupIndex.push_back(std::stoi(cell));
                    }else if( column_count == 5 ){
                        IndividualBodyMass.push_back(std::stod(cell));
                    }else if( column_count == 6 ){
                        CohortAbundance.push_back(std::stod(cell));   
                    }else if( column_count == 11 ){
                        IndividualReproductivePotentialMass.push_back(std::stod(cell));
                    }
                    // switch(column_count){
                    //     case 1:
                    //         GridcellIndex.push_back(std::stoi(cell));
                    //     case 2:
                    //         FunctionalGroupIndex.push_back(std::stoi(cell));
                    //     case 6:
                    //         CohortAbundance.push_back(std::stod(cell));
                    // }
                }
            }
        }

    //-----------------------------------------------------------------------------
        
        // calculate total abundances per gridcell and per functional groups index
        gc_max = *std::max_element(GridcellIndex.begin(), GridcellIndex.end()); gc_max = gc_max + 1;
        fg_max = *std::max_element(FunctionalGroupIndex.begin(), FunctionalGroupIndex.end()); fg_max = fg_max + 1;
        
        //std::vector<double> temp_vec(gc_max*fg_max,0.0);
        int gc_counter = 0;
        int index = 0;

        for (int i = 0; i < GridcellIndex.size(); i++) {
            index = fg_max*GridcellIndex[i]+FunctionalGroupIndex[i];
            temp_vec[index] += (IndividualReproductivePotentialMass[i] + IndividualBodyMass[i])*CohortAbundance[i];
        };
        
    }// end omp for loop
    }// end omp parallel

    //-----------------------------------------------------------------------------
    // std::cout << "max1: " << fg_max << " max2: " << gc_max << std::endl;
    // int fg = 0;
    // int gc = 0;
    // for (int i = 0; i < fg_max*gc_max; i++) {
    //     std::cout << gc << " " << fg << " " << temp_vec[i]/12.0 << std::endl;
    //     if(fg==(fg_max-1)){
    //         fg = 0;
    //         gc++;
    //     }else{
    //         fg++;
    //     }
    // }

    // write yearly average abundances to csv file
    int fg = 0;
    int gc = 0;
    std::ofstream data_out;
	std::string csv_Out_name = argv[13];
	csv_Out_name.erase(remove( csv_Out_name.begin(), csv_Out_name.end(), '\"' ),csv_Out_name.end());
    data_out.open (csv_Out_name);
    data_out << "GC,FG,Biomass,\n"; // data out header
    for (int i = 0; i < fg_max*gc_max; i++) {
        data_out << gc << "," << fg << "," << temp_vec[i]/12.0 << ",\n";
        if(fg==(fg_max-1)){
            fg = 0;
            gc++;
        }else{
            fg++;
        }
    }
    data_out.close();

    return 0;
    //-----------------------------------------------------------------------------
}