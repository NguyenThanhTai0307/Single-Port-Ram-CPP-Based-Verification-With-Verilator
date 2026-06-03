#ifndef DRIVER_H
#define DRIVER_H

#include <iostream>
#include <queue>

#include "Vtop.h"
#include "ram_item.h"
#include "golden_model.h"

class GoldenModel;

class Driver {
    private : 
        Vtop* dut;
        GoldenModel* gm;
        std::queue<RamItem> input_queue;

    public:
        Driver(Vtop* _dut, GoldenModel* _gm);

        void trans_to_driver(const RamItem& item);

        void drive();

        bool is_finished();
};
#endif // DRIVER_H