#include "sequence.h"

Sequence::Sequence (Driver* _drv, int master_seed) :
            drv (_drv) ,
            rng (master_seed),
            addr_range (0, 0xF),
            data_range (0, 0xF), 
            cs_range (0, 1),
            we_range (0, 1) {}

void Sequence::push_item(unsigned char cs, unsigned char we, uint32_t addr, uint32_t din) {
    RamItem item;

    item.cs = cs;
    item.we = we;
    item.din = din;
    item.dout = 0;
    item.addr = addr;

    drv->trans_to_driver(item);
}

// Write a known value, disable cs, enable cs, and read it back
void Sequence::basic_rw_test () {
    uint32_t target_addr = 0x5;
    uint32_t test_data = 0xA;
    
    push_item(1, 1, target_addr, test_data); // Write
    push_item(0, 0, 0x0, 0x0);               // Disable CS 
    push_item(1, 0, target_addr, 0x0);       // Read back
}

// Sequentially write unique data to all addresses from 0 to DEPTH-1, then sequentially read them all back
void Sequence::memory_fill_dump_test() {
    for (int i = 0; i < 16; i++) {
        push_item(1, 1, i, i); // Fill address with its own index
    }
    for (int i = 0; i < 16; i++) {
        push_item(1, 0, i, 0); // Read back
    }
}

// Perform consecutive writes to different addresses without dropping cs or we
void Sequence::b2b_write_test() {
    for (int i = 0; i < 5; i++) {
        push_item(1, 1, addr_range(rng), data_range(rng));
    }
}

// Write to an address and immediately read from the same address on the next clock cycle
void Sequence::raw_test() {
    uint32_t shared_addr = addr_range(rng);
    push_item(1, 1, shared_addr, data_range(rng)); // Write
    push_item(1, 0, shared_addr, 0x0);             // Immediate Read
}

// Drive toggling we, addr, and din while keeping cs low. Verify memory contents remain completely unchanged
void Sequence::unselected_test() {
    for (int i = 0; i < 5; i++) {
        push_item(0, we_range(rng), addr_range(rng), data_range(rng));
    }
}

// Fully randomized sequence of cs, we, addr, and din to hit crossing scenarios
void Sequence::random_stress_test(int trans_num) {
    for (int i = 0; i < trans_num; i++) {
        push_item(cs_range(rng), we_range(rng), addr_range(rng), data_range(rng));
    }
}