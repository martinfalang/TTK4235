
///////////////////////////////////////////////
// Includes
///////////////////////////////////////////////

#include "elev.h"
#include <stdio.h>
#include "fsm.h"
#include "order.h"
#include "exec.h"

///////////////////////////////////////////////
// Main function
///////////////////////////////////////////////

int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }
    elev_set_motor_direction(DIRN_STOP);
    
    state_codes_t current_state = initialize;
    return_codes_t rc;
    return_codes_t (* state_func)(void);
    state_func = state[current_state];
    rc = state_func();
    printf("Current state: %d", current_state);

    while (1) {
        current_state = lookup_transitions(current_state,rc);
    	state_func=state[current_state];
    	rc = state_func();
    }

    return 0;
}
