#include <stdio.h>
#include <math.h>
#include "../route.h"
#include "../cookie.h"

#define BAKE_GOAL 1000000

int total_ticks(char route[], int route_size, double click_rate) {
    cookie state = {0};
    int ticks = 0;
    double cookiesBaked = 0;
    for (int i = 0; i < route_size; i++) {
        char p = route[i];
        if (is_purchaseable(state, p)) {
            double p_price = price(state, p);
            ticks += p_price / rate(state, click_rate) * TPS;
            cookiesBaked += p_price;
            make_purchase(&state, p);
        } else {
            return -1;
        }
    }
    // Finally, we estimate remaining time to the bake goal.
    return ticks + fmax((BAKE_GOAL - cookiesBaked) / rate(state, click_rate) * TPS, 0);
}
