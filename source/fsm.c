
///////////////////////////////////////////////
// Includes
///////////////////////////////////////////////

#include "fsm.h"
#include "time.h"

#include <stdio.h>
#include <stdlib.h> //for exit()

///////////////////////////////////////////////
// Array definitions
///////////////////////////////////////////////

return_codes_t (*state[])(void) = {
    fsm_floor_stationary_state,
    fsm_initialize_state,
    fsm_driving_up_state,
    fsm_driving_down_state,
    fsm_stop_state,
    fsm_idle_state,
    fsm_end_state};

transition_t state_transitions[] = {
    {initialize, hold, initialize},
    {initialize, stay, floor_stationary},
    {initialize, fail, end},

    {floor_stationary, hold, floor_stationary},
    {floor_stationary, drive_up, driving_up},
    {floor_stationary, drive_down, driving_down},
    {floor_stationary, stop, stop_state},
    {floor_stationary, fail, end},

    {driving_up, stay, floor_stationary},
    {driving_up, hold, driving_up},
    {driving_up, stop, stop_state},
    {driving_up, fail, end},

    {driving_down, stay, floor_stationary},
    {driving_down, hold, driving_down},
    {driving_down, stop, stop_state},
    {driving_down, fail, end},

    {stop_state, hold, stop_state},
    {stop_state, stay, floor_stationary},
    {stop_state, idle, idle_state},
    {stop_state, fail, end},

    {idle_state, hold, idle_state},
    {idle_state, stop, stop_state},
    {idle_state, drive_up, driving_up},
    {idle_state, drive_down, driving_down},
    {idle_state, fail, end}};

///////////////////////////////////////////////
// Functions
///////////////////////////////////////////////

state_codes_t lookup_transitions(state_codes_t cur_state, return_codes_t ret_code)
{
    // Loop through state transition array to find the next state based on current state and return
    for (int i = 0; i < sizeof(state_transitions) / sizeof(transition_t); i++)
    {
        if (state_transitions[i].src_state == cur_state && state_transitions[i].return_code == ret_code)
        {
            return state_transitions[i].destination_state;
        }
    }
    // If nothing found, take elevator to end state
    return end;
}

return_codes_t fsm_initialize_state(void)
{
    // Debug options
    //printf("State: initilize\n");
    elev_set_motor_direction(DIRN_DOWN);

    floor_codes_t floor = elev_get_floor_sensor_signal();

    // Hold same state if elevator not at a floor
    if (floor == between_floors)
    {
        return hold;
    }
    else
    {
        exec_update_destination_floor();
        return stay;
    }
}

return_codes_t fsm_floor_stationary_state(void)
{

    // Debug options
    //printf("State: floor_stationary_state\n");

    exec_set_last_direction(DIRN_STOP);

    floor_codes_t current_floor = elev_get_floor_sensor_signal();

    // Update last floor and set floor indicators
    exec_set_last_floor(current_floor);
    elev_set_floor_indicator(current_floor);

    // Check if stop button is pressed
    if (elev_get_stop_signal())
    {
        return stop;
    }

    // Check for orders and update queue
    exec_check_order_buttons();

    // Stop elevator
    elev_set_motor_direction(DIRN_STOP);

    // Check if elevator should stop
    if (exec_should_stop_at_floor(current_floor))
    {
        // Check if the stop button was pressed while the door was open
        if (exec_open_door_3_sec(current_floor) == stop)
        {
            return stop;
        }
    }

    // Debug purposes
    //order_print_orders();

    //Update current destination_floor
    exec_update_destination_floor();
    return_codes_t ret_code = exec_get_floor_return_code(current_floor);

    // If the elevator starts moving up or down, set the last_direction
    if (ret_code == drive_down)
    {
        exec_set_last_direction(DIRN_DOWN);
    }
    else if (ret_code == drive_up)
    {
        exec_set_last_direction(DIRN_UP);
    }
    return ret_code;
}

return_codes_t fsm_driving_up_state(void)
{
    // Debug options
    //printf("State: Driving up\n");

    // Check if stop button is pressed
    if (elev_get_stop_signal())
    {
        return stop;
    }

    // Check if elevator is at a floor
    floor_codes_t current_floor = elev_get_floor_sensor_signal();

    // Check if elevator is near a floor. If yes, check if it should stop
    if (current_floor != between_floors)
    {
        if (exec_should_stop_at_floor(current_floor))
        {
            return stay;
        }
        // Update last floor if elevator is at a floor
        exec_set_last_floor(current_floor);
        elev_set_floor_indicator((int)(current_floor));
    }

    // Check for orders and update queue
    exec_check_order_buttons();

    //Debug purposes
    //order_print_orders();

    //Set motor direction
    elev_set_motor_direction(DIRN_UP);

    //Return hold to continue driving up
    return hold;
}

return_codes_t fsm_driving_down_state(void)
{
    // Debug opetions
    //printf("State: Driving down\n");

    // Check if stop button is pressed
    if (elev_get_stop_signal())
    {
        return stop;
    }

    // Check if elevator is at a floor
    floor_codes_t current_floor = elev_get_floor_sensor_signal();

    // Check if elevator is near a floor. If yes, check if it should stop
    if (current_floor != between_floors)
    {
        if (exec_should_stop_at_floor(current_floor))
        {
            return stay;
        }
        // Update last floor if elevator is at a floor
        exec_set_last_floor(current_floor);
        elev_set_floor_indicator((int)(current_floor));
    }

    //Check for orders
    exec_check_order_buttons();

    //Debug purposes
    //order_print_orders();

    //Set motor direction
    elev_set_motor_direction(DIRN_DOWN);

    //Return hold to continue driving up
    return hold;
}

return_codes_t fsm_stop_state(void)
{
    // Debug purposes
    //printf("State: stop_state\n");

    // Stop elevator and turn on stop lamp
    elev_set_motor_direction(DIRN_STOP);
    elev_set_stop_lamp(1);

    order_remove_all_orders();

    // Turn off all order lights
    for (floor_codes_t floor = floor_1; floor <= floor_4; floor++)
    {
        exec_clear_all_order_lights_at_floor(floor);
    }

    floor_codes_t current_floor = elev_get_floor_sensor_signal();

    // Check if the door should be open
    if (current_floor != between_floors)
    {
        elev_set_door_open_lamp(1);
    }

    // Check if the button is no longer pressed and elevator is at a floor
    if (!elev_get_stop_signal() && current_floor != between_floors)
    {
        elev_set_stop_lamp(0);

        if (exec_open_door_3_sec(current_floor) == stop)
        {
            return hold;
        }

        exec_update_destination_floor();

        return stay;
    }

    // Go to idle state if stop button not pressed and elevator is between floors
    else if (!elev_get_stop_signal())
    {
        elev_set_stop_lamp(0);

        exec_update_destination_floor();
        return idle;
    }
    return hold;
}

return_codes_t fsm_idle_state(void)
{
    // Debug options
    //printf("State: idle\n");

    // Stop elevator just for safety
    elev_set_motor_direction(DIRN_STOP);

    //Check if stop button is pressed
    if (elev_get_stop_signal())
    {
        return stop;
    }

    // Check order buttons and update queue
    exec_check_order_buttons();

    //Set new destination floor if any orders exists
    exec_update_destination_floor();

    return exec_get_idle_return_code();
}

// No updates made
return_codes_t fsm_end_state(void)
{
    printf("We reached the end state\n");
    exit(1);
}