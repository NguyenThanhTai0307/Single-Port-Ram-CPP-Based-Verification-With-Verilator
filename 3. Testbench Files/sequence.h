#ifndef SEQUENCE_H
#define SEQUENCE_H
#include <random>

#include "driver.h"
#include "ram_item.h"

class Driver;

class Sequence {
    private : 
        Driver* drv;

        std::mt19937 rng;
        std::uniform_int_distribution<uint32_t> addr_range;
        std::uniform_int_distribution<uint32_t> data_range;
        std::uniform_int_distribution<unsigned char> cs_range;
        std::uniform_int_distribution<unsigned char> we_range;

        void push_item(unsigned char cs, unsigned char we, uint32_t addr, uint32_t din);

    public : 
        Sequence (Driver* _drv, int master_seed);

        void basic_rw_test();
        void memory_fill_dump_test();
        void b2b_write_test();
        void raw_test();
        void unselected_test();
        void random_stress_test(int trans_num);
};
#endif // SEQUENCE_H