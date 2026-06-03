#ifndef GOLDEN_MODEL_H
#define GOLDEN_MODEL_H

#include <map>
#include <iostream>
#include <cstdint>

#include"ram_item.h"

class GoldenModel {
    private :
        std::map<uint32_t, uint32_t> ram_model;

    public :
        void get_golden_input (const RamItem& item);
        
        uint32_t get_golden_data (uint32_t addr);
};
#endif // GOLDEN_MODEL_H