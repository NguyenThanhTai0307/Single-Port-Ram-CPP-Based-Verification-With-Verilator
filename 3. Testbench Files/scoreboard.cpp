#include"scoreboard.h"

Scoreboard::Scoreboard(GoldenModel* _gm) :
    gm (_gm) {}

void Scoreboard::write (const RamItem& tr) {
    RamItem item;
    item.cs = tr.cs;
    item.we = tr.we;
    item.addr = tr.addr;
    item.dout = tr.dout;
    item.din = 0;
    scb_queue.push(item);
}

void Scoreboard::compare () {
    uint32_t expected_data;

    if (!scb_queue.empty()) {
        RamItem item = scb_queue.front();
        scb_queue.pop();

        if (item.cs == 1) {
            if (item.we == 0) {
                expected_data = gm->get_golden_data(item.addr);

                if (expected_data != 16) {
                    if (expected_data == item.dout) {
                        std::cout << "DATA MATCH AT 0x" << std::hex << item.addr <<
                        " -> EXP : 0x" << std::hex << expected_data << " || ACT : 0x" <<
                        std::hex << item.dout << std::endl;

                        match++;
                    }
                    else{
                        std::cerr << "----> DATA MISMATCH AT 0x" << std::hex << item.addr <<
                        " -> EXP : 0x" << std::hex << expected_data << " || ACT : 0x" <<
                        std::hex << item.dout << std::endl;

                        mismatch++;
                    }
                }
                else {
                    std::cout << "ADDRESS 0x" << std::hex << item.addr << " DOES NOT EXIST" << std::endl;
                }
            }
        }
    }
}
void Scoreboard::print_summary() {
    std::cout << "\n======================================================\n";
    std::cout << "                 SCOREBOARD SUMMARY                   \n";
    std::cout << "======================================================\n";
    std::cout << " Total Matches: " << std::dec << match << "\n";
    std::cout << " Total Mismatches:  " << std::dec << mismatch << "\n";
    if (mismatch == 0 && match >= 0) {
        std::cout << " RESULT: PASS \n";
    } else {
        std::cout << " RESULT: FAIL \n";
    }
    std::cout << "======================================================\n";
}