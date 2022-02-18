#include <stdio.h>
#include <stdlib.h>
#include "solver.h"
#include "route.h"

#define RANDOM_SEED 4761
#define CLICK_RATE 12.5

void print_route(char route[], int route_size) {
    for (int i = 0; i < route_size; i++) {
        char p = route[i];
        switch (p) {
            case CURSOR_BYTE:
                printf("Cursor\n"); break;
            case GRANDMA_BYTE:
                printf("Grandma\n"); break;
            case FARM_BYTE:
                printf("Farm\n"); break;
            case MINE_BYTE:
                printf("Mine\n"); break;
            case FACTORY_BYTE:
                printf("Factory\n"); break;
            case CURSOR_UP_BYTE:
                printf("Cursor_up\n"); break;
            case GRANDMA_UP_BYTE:
                printf("Grandma_up\n"); break;
            case FARM_UP_BYTE:
                printf("Farm_up\n"); break;
            case MINE_UP_BYTE:
                printf("Mine_up\n"); break;
            case SUPER_UP_BYTE:
                printf("Super_up\n"); break;
            case PLASTIC_UP_BYTE:
                printf("Plastic_up\n"); break;
            case FINGERS_UP_BYTE:
                printf("Fingers_up\n"); break;
            case BEGIN_BUNDLE_BYTE:
                printf("^"); break;
            case END_BUNDLE_BYTE:
                printf("v"); break;
            default:
                break;
        }
    }
}

int main(int argc, char** argv) {
    srand(RANDOM_SEED);
    char route[128] = "cCCcccccccccggggggGggfgcfgcfgfcgGgggggCcccccfcgfFffffmcgfmcgffFffffSggggggGggggPfgmfgfgmcfgmcmMmmmmmffgfggccccccccccccccccccc   ";
    // improve(1 << 14, route, 128, CLICK_RATE);
    printf("total_ticks: %d\n", total_ticks(route, 128, CLICK_RATE));
    return 0;
}