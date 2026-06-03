#ifndef DRIVER_CPP
#define DRIVER_CPP

#include "driver.h"

Driver::Driver(Vtop* _dut, GoldenModel* _gm) :
            dut (_dut),
            gm (_gm)
        {}

void Driver::trans_to_driver(const RamItem& item) {
    input_queue.push(item);
}

void Driver::drive() {
    if (!input_queue.empty()) {
        RamItem item = input_queue.front();
        input_queue.pop();

        // Drive the DUT with the input item
        dut->addr = item.addr;
        dut->din = item.din;
        dut->cs = item.cs;
        dut->we = item.we;

        gm->get_golden_input(item);
    }
    else {
        dut->cs = 0;
        dut->we = 0;
    }
}

bool Driver::is_finished() {
    return input_queue.empty();
}

#endif // DRIVER_CPP