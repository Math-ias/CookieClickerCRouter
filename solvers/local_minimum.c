#include "../route.h"
#include "../solver.h"

#define REPLACEMENTS 13
const char replacements[REPLACEMENTS] = " cCgGfFmMrSP1";

void improve(int iterations, char route[], int route_size, double click_rate) {
    char best_replaced_purchase;
    int best_replaced_index;
    int best_replaced_ticks = total_ticks(route, route_size, click_rate);
    for (int i = 0; i < iterations; i++) {
        best_replaced_purchase = 0;
        best_replaced_index = -1;
        for (int r = 0; r < REPLACEMENTS; r++) {
            char p = replacements[r];
            for (int i = 0; i < route_size; i++) {
                char previous_purchase = route[i];
                route[i] = p;
                int ticks = total_ticks(route, route_size, click_rate);
                if (ticks < best_replaced_ticks && ticks > -1) {
                    best_replaced_index = i;
                    best_replaced_purchase = p;
                    best_replaced_ticks = ticks;
                }
                route[i] = previous_purchase;
            }
        }
        if (best_replaced_index > -1) {
            route[best_replaced_index] = best_replaced_purchase;
        } else {
            break;
        }
    }
}