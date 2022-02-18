#include <stdlib.h>
#include <stdio.h>
#include "../solver.h"
#include "../route.h"
#include <math.h>

#define TEMP_INITIAL 512

#define REPLACEMENTS 13
const char replacements[REPLACEMENTS] = " cCgGfFmMrSP1";

// Following suggestions from NUMERICAL RECIPES IN C: THE ART OF THE SCIENTIFIC COMPUTING (ISBN 0-521-43108-5).
#define ALPHA 2

/**
 * Annealing schedule.
 */
double temperature(int i, int iterations) {
    return TEMP_INITIAL * pow((1.0 - ((double) i) / iterations), ALPHA);
}

/**
 * Take the neighbor or not.
 */
int choose(int old_ticks, int new_ticks, double temp) {
    if (new_ticks < old_ticks) {
        return 1; // Always take a better solution.
    } else {
        return exp((old_ticks - new_ticks) / temp) > (rand() / RAND_MAX);
    }
}

void improve(int iterations, char route[], int route_size, double click_rate) {
    // Route is edited in place.
    // Following the pseudo-code at https://en.wikipedia.org/wiki/Simulated_annealing#Pseudocode.
    int old_ticks = total_ticks(route, route_size, click_rate);
    for (int i = 0; i < iterations; i++) { // k is i, k_max is iterations.
        int temp = temperature(i, iterations);
        // Pick a random neighbor.
        int repl_i = rand() % route_size;
        char old_p = route[repl_i];
        int repl_p = replacements[rand() % REPLACEMENTS];
        route[repl_i] = repl_p;
        int new_ticks = total_ticks(route, route_size, click_rate);
        if (new_ticks == -1) {
            // printf("i:%d SKIP!\n", i);
            route[repl_i] = old_p;
            continue;
        }
        int choice = choose(old_ticks, new_ticks, temp);
        // printf("i:%d temp:%d old_ticks:%d new_ticks:%d choice:%d\n", i, temp, old_ticks, new_ticks, choice);
        if (choice) {
            // Keep.
            old_ticks = new_ticks;
        } else {
            // Restore.
            route[repl_i] = old_p;
        }
    }
}