#ifndef SOLVER_H
#define SOLVER_H

/**
 * Mutate the given route in place to make it better.
 * Use iterations if necessary for the method.
 */
void improve(int iterations, char route[], int route_size, double click_rate);

#endif