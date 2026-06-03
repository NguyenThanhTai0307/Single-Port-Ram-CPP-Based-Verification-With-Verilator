#include<iostream>
#include<memory>

#include"Vtop.h"
#include"verilated.h"
#include"verilated_vcd_c.h"
#include "verilated_cov.h"

#include "driver.h"
#include "golden_model.h"
#include "monitor.h"
#include "scoreboard.h"
#include "functional_coverage.h"
#include "sequence.h"

int main(int argc, char** argv){
    std::unique_ptr<VerilatedContext> contextp {new VerilatedContext};
    Vtop* dut = new Vtop{contextp.get()};

    contextp->commandArgs(argc, argv);

    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    dut->trace(tfp, 99);
    tfp->open("waveform.vcd");

    if(contextp->randSeed() == 0)
        contextp->randSeed(time(NULL));
    
    int master_seed = contextp->randSeed();

    FunctionalCoverage* fcov = new FunctionalCoverage ();
    GoldenModel* gm = new GoldenModel ();
    Driver* drv = new Driver (dut, gm);
    Sequence* seq = new Sequence (drv, master_seed);
    Scoreboard* scb = new Scoreboard (gm);
    Monitor* mon = new Monitor (dut, fcov, scb);

    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "  SIMULATION STARTING WITH SEED: " << contextp->randSeed() << std::endl;
    std::cout << "--------------------------------------------" << std::endl;

    //====================================================
    //                 TEST FACTORY & DISPATCH
    //====================================================
    std::string test_name = "All"; // Default to running everything

    // Parse command line arguments for +TESTNAME=
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.rfind("+TESTNAME=", 0) == 0) { 
            // Extract the string directly after "+TESTNAME="
            test_name = arg.substr(10); 
            break;
        }
    }

    std::cout << "\n============================================" << std::endl;
    std::cout << "  DISPATCHING TEST: " << test_name << std::endl;
    std::cout << "============================================\n" << std::endl;

    // Route to the correct sequence based on the extracted string
    if (test_name == "Basic_rw_test") {
        seq->basic_rw_test();
    } 
    else if (test_name == "Fill_dump_test") {
        seq->memory_fill_dump_test();
    } 
    else if (test_name == "B2b_write_test") {
        seq->b2b_write_test();
    } 
    else if (test_name == "Raw_test") {
        seq->raw_test();
    } 
    else if (test_name == "Unselected_test") {
        seq->unselected_test();
    } 
    else if (test_name == "Random_stress_test") {
        seq->random_stress_test(5000);
    } 
    else if (test_name == "All") {
        seq->basic_rw_test();
        seq->memory_fill_dump_test();
        seq->b2b_write_test();
        seq->raw_test();
        seq->unselected_test();
        seq->random_stress_test(5000);
    } 
    else {
        std::cerr << "FATAL: Unknown +TESTNAME=" << test_name << std::endl;
        return 1; // Exit simulation early if test name is invalid
    }
    
    //====================================================
    //                 SIMULATION RUN PHASE
    //====================================================
    int drain_time = 20;

    dut->clk = 0;


    while (!contextp->gotFinish() && contextp->time() < 50000)
    {

        if (drv->is_finished()) {
            if (drain_time == 0) {
                break; 
            }
            drain_time--;
        }

        //============================================
        // Pre-active phase (Before the rising clock)
        //============================================
        mon->sample();

        scb->compare();

        drv->drive();

        dut->eval();
        
        contextp->timeInc(5);
        tfp->dump(contextp->time());

        //============================================
        // Active phase (the rising clock)
        //============================================
        dut->clk = 1;
        dut->eval();

        contextp->timeInc(5);
        tfp->dump(contextp->time());

        //============================================
        // Post-active phase (the falling clock)
        //============================================
        dut->clk = 0;
        dut->eval();
        
    }
    
    scb->print_summary();
    fcov->report_coverage();
    tfp->close();

    Verilated::mkdir("logs");
    VerilatedCov::write("logs/coverage.dat");
    
    delete dut;
    delete drv;
    delete seq;
    delete gm;
    delete scb;
    delete mon;
    delete fcov;
    return 0;
}