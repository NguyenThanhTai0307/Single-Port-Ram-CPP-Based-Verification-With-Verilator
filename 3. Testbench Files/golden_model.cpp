#include"golden_model.h"

void GoldenModel::get_golden_input(const RamItem& item) {
    if (item.cs == 1) {
        if (item.we == 1) {
            ram_model[item.addr] = item.din;
            
            std::cout << "Write At Address 0x" << std::hex << item.addr 
            << " -> Data : 0x" << std::hex << item.din << std::endl;
        }
    }
}

uint32_t GoldenModel::get_golden_data(uint32_t addr) {
    auto key = ram_model.find(addr);

    if (key != ram_model.end()) {
        uint32_t golden_data = ram_model[addr];
        return golden_data;
    } 
    else {
        return 16;
    }
}