#ifndef RAM_ITEM_H
#define RAM_ITEM_H

#include <cstdint>

struct RamItem {
    unsigned char cs;
    unsigned char we;
    uint32_t addr;
    uint32_t din;
    uint32_t dout;
};

#endif // RAM_ITEM_H