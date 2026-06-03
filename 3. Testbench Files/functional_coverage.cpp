#include<iostream>

#include "functional_coverage.h"


void FunctionalCoverage::sample_coverage(unsigned char cs, unsigned char we, int addr, int din) {

        if(cs == 1){
            // Sample cs
            cp_cs_sel += 1;
        
            // Sample we
            if(we == 1)
                cp_we_wr += 1;
            else
                cp_we_rd += 1;

            // Sample addr and din
            cp_addr_bin[addr] += 1;
            cp_data_bin[din] += 1;

            //Sample the cross
            auto key = std::make_tuple(we, addr, din);
            cross_cs1_bin[key] += 1;
        }
        else {
            cp_cs_no_sel += 1;
        }
    }

void FunctionalCoverage::report_coverage() {
        std::cout << "\n===================================================\n";
        std::cout << "               DPI-C COVERAGE REPORT               \n";
        std::cout << "===================================================\n";
    
        std::cout << "[cp_cs]   CS=1 Hits: " << cp_cs_sel << " | CS=0 Hits: " << cp_cs_no_sel << "\n";
        std::cout << "[cp_we]   Write Hits: " << cp_we_wr << " | Read Hits: " << cp_we_rd << "\n";
    
        std::cout << "[cp_addr] Unique Addresses Tested: " << cp_addr_bin.size() << "\n";
        std::cout << "[cp_data] Unique Data Values Tested: " << cp_data_bin.size() << "\n";
    
        std::cout << "[cross]   Valid Cross Combinations (CS=1): " << cross_cs1_bin.size() << "\n";
        std::cout << "===================================================\n\n";
}