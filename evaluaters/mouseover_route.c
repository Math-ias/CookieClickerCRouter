#include <stdio.h>
#include <math.h>
#include "../route.h"
#include "../cookie.h"

#define BAKE_GOAL 1000000
#define MOUSEOVER_TICKS 15

int int_max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int total_ticks(char route[], int route_size, double click_rate) {
    // The current state. We mutate this.
    cookie state = {0};

    // The elapsed time and cookies baked.
    int ticks = 0;
    double cookies_baked = 0;
    int leftover_cookies = 0; // Represents left-overs from previous actions.

    // Bundle variables.
    int in_bundle = 0;
    int bundle_price = 0;
    double pre_bundle_rate_with_clicks = 0;
    double pre_bundle_rate_without_clicks = 0;
    // For each purchase.
    for (int i = 0; i < route_size; i++) {
        char p = route[i];

        // Early exit if the purchase isn't possible.
        if (!is_purchaseable(state, p)) {
            return -1;
        }

        double p_price = price(state, p);
        if (!in_bundle) {
            double rate_with_clicks = rate(state, click_rate);
            double rate_without_clicks = rate(state, 0);
            make_purchase(&state, p);
            if (p == BEGIN_BUNDLE_BYTE) { // Start a new bundle.
                in_bundle = 1;
                bundle_price = 0;
                pre_bundle_rate_with_clicks = rate_with_clicks;
                pre_bundle_rate_without_clicks = rate_without_clicks;
            } else { // Just make the purchase.
                // Raise the required capital.
                int cookies_to_raise = int_max(p_price - leftover_cookies, 0);
                ticks += cookies_to_raise / rate_with_clicks * TPS;
                leftover_cookies = int_max(leftover_cookies - cookies_to_raise, 0);
                cookies_baked += cookies_to_raise;
                // Mouse over to the thing.
                ticks += MOUSEOVER_TICKS;
                int mouse_over_cookies = rate_without_clicks / TPS * MOUSEOVER_TICKS;
                cookies_baked += mouse_over_cookies;
                leftover_cookies += mouse_over_cookies;
                // Purchase is made.
                double new_rate_without_clicks = rate(state, 0);
                // Mouseback to the big cookie.
                ticks += MOUSEOVER_TICKS;
                int mouse_back_cookies = new_rate_without_clicks / TPS * MOUSEOVER_TICKS;
                cookies_baked += mouse_back_cookies;
                leftover_cookies += mouse_back_cookies;
            }
        } else {
            make_purchase(&state, p);
            // Now the harder procedure, if we are in a bundle.
            if (p == END_BUNDLE_BYTE) { // Still TODO ensure bundle DOES end.
                // Purchase the entire bundle.
                in_bundle = 0;
                // Raise the required capital.
                int cookies_to_raise = int_max(bundle_price - leftover_cookies, 0);
                ticks += cookies_to_raise / pre_bundle_rate_with_clicks * TPS;
                leftover_cookies = int_max(leftover_cookies - cookies_to_raise, 0);
                cookies_baked += cookies_to_raise;
                // Mouse over to the bundle.
                ticks += MOUSEOVER_TICKS;
                int mouse_over_cookies = pre_bundle_rate_without_clicks / TPS * MOUSEOVER_TICKS;
                cookies_baked += mouse_over_cookies;
                leftover_cookies += mouse_over_cookies;
                // Purchase is made.
                double new_rate_without_clicks = rate(state, 0);
                // Mouseback to the big cookie.
                ticks += MOUSEOVER_TICKS;
                int mouse_back_cookies = new_rate_without_clicks / TPS * MOUSEOVER_TICKS;
                cookies_baked += mouse_back_cookies;
                leftover_cookies += mouse_back_cookies;
            } else {
                // Add the given purchase to the bundle.
                bundle_price += p_price;
            }
        }
    }
    // Finally, we estimate remaining time to the bake goal.
    return ticks + fmax((BAKE_GOAL - cookies_baked) / rate(state, click_rate) * TPS, 0);
}