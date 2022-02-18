#ifndef ROUTE_H
#define ROUTE_H

#include "cookie.h"

#define WAIT_BYTE       ' '
#define CURSOR_BYTE     'c'
#define GRANDMA_BYTE    'g'
#define FARM_BYTE       'f'
#define MINE_BYTE       'm'
#define FACTORY_BYTE    'r'
#define CURSOR_UP_BYTE  'C'
#define GRANDMA_UP_BYTE  'G'
#define FARM_UP_BYTE     'F'
#define MINE_UP_BYTE     'M'
#define SUPER_UP_BYTE    'S'
#define PLASTIC_UP_BYTE  'P'
#define FINGERS_UP_BYTE  '1'
#define BEGIN_BUNDLE_BYTE   '^'
#define END_BUNDLE_BYTE     'v'

/**
 * Returns the total number of ticks a route takes to complete.
 * Returns -1 if invalid (upgrades not purchaseable).
 */
int total_ticks(char route[], int route_size, double click_rate);

#endif