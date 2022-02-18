#include "cookie.h"
#include "route.h"
#include <math.h>
#include <stdio.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

/**
 * A private helper to fetch the number of X2 upgrades owned.
 */
int upgrade_num(unsigned char ownership_byte, unsigned char mask, unsigned char single_up) {
    return (ownership_byte & mask) / single_up;
}

/**
 * A private helper to fetch the X2 multiplication constant based on how many building_UP's are owned.
 */
int x2_multiplier(unsigned char ownership_byte, unsigned char mask, unsigned char single_up) {
    return 1 << upgrade_num(ownership_byte, mask, single_up);
}

double rate(cookie state, double click_rate) {
    int non_cursor_buildings = 0;
    int grandma_multiplier = x2_multiplier(
        state.grandma_farm_up_ownership,
        OWN_GRANDMA_UP_MASK,
        OWN_GRANDMA_UP
    ) << ((state.grandma_farm_up_ownership & OWN_SUPER_UP) > 0);
    double grandma_rate = GRANDMA_R * state.grandmas * grandma_multiplier;
    non_cursor_buildings += state.grandmas;

    double farm_multiplier = (1.00 + 0.01 * state.grandmas * ((state.grandma_farm_up_ownership & OWN_SUPER_UP) > 0)) *
        x2_multiplier(
            state.grandma_farm_up_ownership,
            OWN_FARM_UP_MASK,
            OWN_FARM_UP
        );
    double farm_rate = FARM_R * state.farms * farm_multiplier;
    non_cursor_buildings += state.farms;
    
    int mine_multiplier = x2_multiplier(
        state.cursor_mine_up_ownership,
        OWN_MINE_UP_MASK,
        OWN_MINE_UP
    );
    double mine_rate = MINE_R * state.mines * mine_multiplier;
    non_cursor_buildings += state.mines;

    double factory_rate = FACTORY_R * state.factories;
    non_cursor_buildings += state.factories;

    int cursor_multiplier = x2_multiplier(
        state.cursor_mine_up_ownership,
        OWN_CURSOR_UP_MASK,
        OWN_CURSOR_UP
    );
    double cursor_constant = non_cursor_buildings * ((state.cursor_mine_up_ownership & OWN_FINGERS_UP) > 0) * 0.1;
    double cursor_rate = (CURSOR_R * cursor_multiplier + cursor_constant) * state.cursors;

    double total_building_rate = grandma_rate + farm_rate + mine_rate + factory_rate + cursor_rate;
    double plastic_up_constant = 0.01 * ((state.cursor_mine_up_ownership & OWN_PLASTIC_UP) > 0) * total_building_rate;
    double clicking_rate = (cursor_multiplier + plastic_up_constant + cursor_constant) * click_rate;

    return clicking_rate + total_building_rate;
}

#define P_RATE 1.15

/**
 * A private helper method using the Cookie Clicker pricing formula.
 */
double building_price(int unit_price, unsigned char owned, unsigned char wanted) {
    return ceil(unit_price * (pow(P_RATE, owned + wanted) - pow(P_RATE, owned)) / (P_RATE - 1.0));
}

double price(cookie state, char p) {
    int upgrades;
    switch (p) {
        case CURSOR_BYTE:
            return building_price(CURSOR_P, state.cursors, 1);
        case GRANDMA_BYTE:
            return building_price(GRANDMA_P, state.grandmas, 1);
        case FARM_BYTE:
            return building_price(FARM_P, state.farms, 1);
        case MINE_BYTE:
            return building_price(MINE_P, state.mines, 1);
        case FACTORY_BYTE:
            return building_price(FACTORY_P, state.factories, 1);

        case CURSOR_UP_BYTE:
            upgrades = upgrade_num(state.cursor_mine_up_ownership, OWN_CURSOR_UP_MASK, OWN_CURSOR_UP);
            if (upgrades == 0) {
                return CURSOR_UP_1_P;
            } else if (upgrades == 1) {
                return CURSOR_UP_2_P;
            } else {
                return CURSOR_UP_3_P;
            }
        case GRANDMA_UP_BYTE:
            upgrades = upgrade_num(state.grandma_farm_up_ownership, OWN_GRANDMA_UP_MASK, OWN_GRANDMA_UP);
            if (upgrades == 0) {
                return GRANDMA_UP_1_P;
            } else if (upgrades == 1) {
                return GRANDMA_UP_2_P;
            } else {
                return GRANDMA_UP_3_P;
            }
        case FARM_UP_BYTE:
            upgrades = upgrade_num(state.grandma_farm_up_ownership, OWN_FARM_UP_MASK, OWN_FARM_UP);
            if (upgrades == 0) {
                return FARM_UP_1_P;
            } else if (upgrades == 1) {
                return FARM_UP_2_P;
            } else {
                return FARM_UP_3_P;
            }
        case MINE_UP_BYTE:
            upgrades = upgrade_num(state.cursor_mine_up_ownership, OWN_MINE_UP_MASK, OWN_MINE_UP);
            if (upgrades == 0) {
                return MINE_UP_1_P;
            } else {
                return MINE_UP_2_P;
            }
        case SUPER_UP_BYTE:
            return SUPER_UP_P;
        case PLASTIC_UP_BYTE:
            return PLASTIC_UP_P;
        case FINGERS_UP_BYTE:
            return FINGERS_UP_P;

        default:
            return 0;
    }
}

int is_purchaseable(cookie state, char p) {
    int upgrades;
    switch(p) {
        case CURSOR_UP_BYTE:
            upgrades = upgrade_num(state.cursor_mine_up_ownership, OWN_CURSOR_UP_MASK, OWN_CURSOR_UP);
            if (upgrades == 0) {
                return state.cursors >= CURSOR_UP_1_CURSORS_R;
            } else if (upgrades == 1) {
                return state.cursors >= CURSOR_UP_2_CURSORS_R;
            } else if (upgrades == 2) {
                return state.cursors >= CURSOR_UP_3_CURSORS_R;
            } else {
                return 0;
            }
        case GRANDMA_UP_BYTE:
            upgrades = upgrade_num(state.grandma_farm_up_ownership, OWN_GRANDMA_UP_MASK, OWN_GRANDMA_UP);
            if (upgrades == 0) {
                return state.grandmas >= GRANDMA_UP_1_GRANDMAS_R;
            } else if (upgrades == 1) {
                return state.grandmas >= GRANDMA_UP_2_GRANDMAS_R;
            } else if (upgrades == 2) {
                return state.grandmas >= GRANDMA_UP_3_GRANDMAS_R;
            } else {
                return 0;
            }
        case FARM_UP_BYTE:
            upgrades = upgrade_num(state.grandma_farm_up_ownership, OWN_FARM_UP_MASK, OWN_FARM_UP);
            if (upgrades == 0) {
                return state.farms >= FARM_UP_1_FARMS_R;
            } else if (upgrades == 1) {
                return state.farms >= FARM_UP_2_FARMS_R;
            } else if (upgrades == 2) {
                return state.farms >= FARM_UP_3_FARMS_R;
            } else {
                return 0;
            }
        case MINE_UP_BYTE:
            upgrades = upgrade_num(state.cursor_mine_up_ownership, OWN_MINE_UP_MASK, OWN_MINE_UP);
            if (upgrades == 0) {
                return state.mines >= MINE_UP_1_MINES_R;
            } else if (upgrades == 1) {
                return state.mines >= MINE_UP_2_MINES_R;
            } else {
                return 0;
            }
        case SUPER_UP_BYTE:
            return state.grandmas >= SUPER_UP_GRANDMAS_R &&
                state.grandmas > SUPER_UP_FARMS_R && 
                !(state.grandma_farm_up_ownership & OWN_SUPER_UP);
        case FINGERS_UP_BYTE:
            return state.cursors >= FINGERS_UP_CURSORS_R &&
            !(state.cursor_mine_up_ownership & OWN_FINGERS_UP);

        default:
            return 1;
    }
}

void make_purchase(cookie* state, char p) {
    switch (p) {
        case WAIT_BYTE:
            return;
        case CURSOR_BYTE:
            state->cursors += 1; return;
        case GRANDMA_BYTE:
            state->grandmas += 1; return;
        case FARM_BYTE:
            state->farms += 1; return;
        case MINE_BYTE:
            state->mines += 1; return;
        case FACTORY_BYTE:
            state->factories += 1; return;

        case CURSOR_UP_BYTE:
            state->cursor_mine_up_ownership += OWN_CURSOR_UP; return;
        case GRANDMA_UP_BYTE:
            state->grandma_farm_up_ownership += OWN_GRANDMA_UP; return;
        case FARM_UP_BYTE:
            state->grandma_farm_up_ownership += OWN_FARM_UP; return;
        case MINE_UP_BYTE:
            state->cursor_mine_up_ownership += OWN_MINE_UP; return;
        case SUPER_UP_BYTE:
            state->grandma_farm_up_ownership |= OWN_SUPER_UP; return;
        case PLASTIC_UP_BYTE:
            state->cursor_mine_up_ownership |= OWN_PLASTIC_UP; return;
        case FINGERS_UP_BYTE:
            state->cursor_mine_up_ownership |= OWN_FINGERS_UP; return;
    }
}
