#ifndef FUNCTIONAL_COVERAGE_H
#define FUNCTIONAL_COVERAGE_H

#include<map>
#include<tuple>

class FunctionalCoverage {
    private :
        int cp_cs_sel = 0;
        int cp_cs_no_sel = 0;

        int cp_we_wr = 0;
        int cp_we_rd = 0;

        std::map<int, int> cp_addr_bin;
        std::map<int, int> cp_data_bin;

        std::map<std::tuple<int, int, int>, int> cross_cs1_bin;
    
    public :
        void sample_coverage(unsigned char cs, unsigned char we, int addr, int din);

        void report_coverage();
};
#endif //FUNCTIONAL_COVERAGE_H