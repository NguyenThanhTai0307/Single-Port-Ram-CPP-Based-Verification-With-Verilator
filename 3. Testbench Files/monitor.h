#ifndef MONITOR_H
#define MONITOR_H
#include <iostream>

#include "Vtop.h"
#include "ram_item.h"
#include "functional_coverage.h"
#include "scoreboard.h"

class FunctionalCoverage;
class Scoreboard;

class Monitor {
    private :
        Vtop* dut;
        FunctionalCoverage* fcov;
        Scoreboard* scb;

        RamItem m_item;

    public : 
        Monitor (Vtop* _dut, FunctionalCoverage* _fcov, Scoreboard* _scb);

        void sample();
};
#endif // MONITOR_H