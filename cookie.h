#ifndef COOKIE_H
#define COOKIE_H

#define TPS 30

// BUILDINGS
// P represents the price in cookies.
// R represents the rate in cookies per 30 ticks.

#define CURSOR_P    15
#define CURSOR_R    0.1

#define GRANDMA_P   100
#define GRANDMA_R   1.0

#define FARM_P      1100
#define FARM_R      8.0

#define MINE_P      12000
#define MINE_R      47.0

#define FACTORY_P   130000
#define FACTORY_R   260.0

// UPGRADES
// P represents the price of a particular number of upgrade.
// R represents a required number of a certain building.

#define CURSOR_UP_1_P   100
#define CURSOR_UP_1_CURSORS_R   1
#define CURSOR_UP_2_P   500
#define CURSOR_UP_2_CURSORS_R   1
#define CURSOR_UP_3_P   10000
#define CURSOR_UP_3_CURSORS_R   10

#define GRANDMA_UP_1_P  1000
#define GRANDMA_UP_1_GRANDMAS_R 1
#define GRANDMA_UP_2_P  5000
#define GRANDMA_UP_2_GRANDMAS_R 5
#define GRANDMA_UP_3_P  50000
#define GRANDMA_UP_3_GRANDMAS_R 25

#define FARM_UP_1_P     11000
#define FARM_UP_1_FARMS_R       1
#define FARM_UP_2_P     55000
#define FARM_UP_2_FARMS_R       5
#define FARM_UP_3_P     550000
#define FARM_UP_3_FARMS_R       25

#define MINE_UP_1_P     120000
#define MINE_UP_1_MINES_R       1
#define MINE_UP_2_P     600000
#define MINE_UP_2_MINES_R       5

#define SUPER_UP_P      55000
#define SUPER_UP_GRANDMAS_R     1
#define SUPER_UP_FARMS_R        15

#define PLASTIC_UP_P 50000
// Skipping the 1000 handmade cookies requirement.
// Just so unlikely you make this many cookies without clicking 1000 times.

#define FINGERS_UP_P    100000
#define FINGERS_UP_CURSORS_R    25

// Bit masks for the cursor and grandma upgrade ownership byte.
#define OWN_CURSOR_UP       0B01000000
#define OWN_CURSOR_UP_MASK  0B11000000
#define OWN_PLASTIC_UP      0B00100000
#define OWN_FINGERS_UP      0B00010000
#define OWN_MINE_UP         0B00000100
#define OWN_MINE_UP_MASK    0B00001100

// Bit masks for the farm and mine upgrade ownership byte.
#define OWN_FARM_UP         0B01000000
#define OWN_FARM_UP_MASK    0B11000000
#define OWN_GRANDMA_UP      0B00010000
#define OWN_GRANDMA_UP_MASK 0B00110000
#define OWN_SUPER_UP        0B00001000

/**
 * Represents a frame of a game of Cookie Clicker. 
 */
typedef struct {
    // FIRST FOUR BYTES
    unsigned char cursors;
    unsigned char mines;
    unsigned char cursor_mine_up_ownership;
    unsigned char factories;

    // SECOND FOUR BYTES
    unsigned char grandmas;
    unsigned char farms;
    unsigned char grandma_farm_up_ownership;
    unsigned char reserve_byte;
} cookie;

double price(cookie state, char p);

void make_purchase(cookie* state, char p);

int is_purchaseable(cookie state, char p);

double rate(cookie state, double click_rate);

#endif