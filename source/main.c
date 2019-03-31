#include "elev.h"
#include <stdio.h>
#include "fsm.h"
#include "order.h"

int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    state_codes_t current_state = initialize;
    return_codes_t rc;
    return_codes_t (* state_func)(void);
    state_func=state[current_state];
    rc = state_func();

    printf("Press STOP button to stop elevator and exit program.\n");

    elev_set_motor_direction(DIRN_UP);

    //Testing
    testorder();

    while (1) {
        /*
        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
        }

        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
        */

       /*
        current_state = lookup_transitions(current_state,rc);
    	state_func=state[current_state];
    	rc = state_func();
        */
    }

    return 0;
}
