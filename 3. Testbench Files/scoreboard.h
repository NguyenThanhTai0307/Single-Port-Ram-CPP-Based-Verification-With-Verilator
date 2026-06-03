#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include<iostream>
#include<cstdint>
#include<queue>

#include "golden_model.h"
#include "ram_item.h"

class GoldenModel;

class Scoreboard {
    private :
        GoldenModel* gm;

        std::queue<RamItem> scb_queue;

        int match = 0;
        int mismatch = 0;

    public :
        Scoreboard(GoldenModel* _gm);

        void write (const RamItem& item);

        void compare();

        void print_summary();
};
#endif //SCOREBOARD_H