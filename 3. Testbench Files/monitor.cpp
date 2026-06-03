#include "monitor.h"

Monitor::Monitor (Vtop* _dut, FunctionalCoverage* _fcov, Scoreboard* _scb) :
    dut (_dut),
    fcov (_fcov),
    scb (_scb) {}

void Monitor::sample () {
    
    fcov->sample_coverage(m_item.cs, m_item.we, m_item.addr, m_item.din);

    if (dut->cs == 1) {
        m_item.cs = dut->cs;
        m_item.we = dut->we;
        m_item.addr = dut->addr;
        m_item.din = dut->din;
        m_item.dout = dut->dout;

        scb->write(m_item);
    }
}