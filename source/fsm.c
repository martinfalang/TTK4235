
///////////////////////////////////////////////
// Includes
///////////////////////////////////////////////

#include "fsm.h"
#include "stdio.h"
#include "elev.h"
#include "time.h"
#include "order.h"
#include "exec.h"

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
    fsm_end_state
};

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
    {idle_state, fail, end}
};

///////////////////////////////////////////////
// Functions
///////////////////////////////////////////////

state_codes_t lookup_transitions(state_codes_t cur_state, return_codes_t ret_code)
{   
    int i;
    for (i = 0; i < sizeof(state_transitions)/sizeof(transition_t); i++)
    {
        if(state_transitions[i].src_state == cur_state && state_transitions[i].return_code == ret_code) 
        {
            return state_transitions[i].destination_state;
        }
    }
    return end;
}

return_codes_t fsm_initialize_state()
{
    // Debug options
    //printf("State: initilize\n");
    elev_set_motor_direction(DIRN_DOWN);

    floor_codes_t floor = elev_get_floor_sensor_signal();

    if(floor == between_floors) {
        return hold;
    }
    else {
        exec_update_destination_floor();
        return stay;
    }
}   

return_codes_t fsm_floor_stationary_state() {
    
    // Debug options
    //printf("State: floor_stationary_state\n");

    exec_set_last_direction(DIRN_STOP);

    floor_codes_t current_floor = elev_get_floor_sensor_signal();
    
    exec_set_last_floor(current_floor);
    
    elev_set_floor_indicator(current_floor);

    //Update queue
    exec_check_order_buttons();

    //Stop elevator
    elev_set_motor_direction(DIRN_STOP);

    if (exec_should_stop_at_floor(current_floor)) {
        if (exec_open_door_3_sec(current_floor) == stop) {
            return stop;
        }
    }
    
    //Checks if stop button is pressed MAKE THIS A FUNCTION
    if (elev_get_stop_signal()) {
        return stop;
    }
    
    //Debug purposes
    //order_print_orders();

    //Update current destination_floor
    exec_update_destination_floor();
    return_codes_t ret_code = exec_get_floor_return_code(current_floor);

    if (ret_code == drive_down) {
        exec_set_last_direction(DIRN_DOWN);
    }
    else if (ret_code == drive_up) {
        exec_set_last_direction(DIRN_UP);
    }
    return ret_code;
}

return_codes_t fsm_driving_up_state() 
{
    // Debug options
    //printf("State: Driving up\n");

    //Check if elevator is at a floor
    floor_codes_t current_floor = elev_get_floor_sensor_signal();

    //Update last floor if elevator is at a floor
    if (current_floor != between_floors) {
        exec_set_last_floor(current_floor);
        elev_set_floor_indicator((int)(current_floor));
    }

    //Check if stop button is pressed
    if (elev_get_stop_signal()) {
        return stop;
    }

    //Check for orders
    exec_check_order_buttons();

    //Debug purposes
    //order_print_orders();

    //Check if elevator is near a floor. If yes, check if it should stop
    if (current_floor != between_floors) {
        if (exec_should_stop_at_floor(current_floor)) {
            return stay;
        }
    }

    //Set motor direction
    elev_set_motor_direction(DIRN_UP);

    //Return hold to continue driving up
    return hold;
}

return_codes_t fsm_driving_down_state() 
{
    // Debug opetions
    //printf("State: Driving down\n");

    //Check if elevator is at a floor
    floor_codes_t current_floor = elev_get_floor_sensor_signal();

    //Update last floor if elevator is at a floor
    if (current_floor != between_floors) {
        exec_set_last_floor(current_floor);
        elev_set_floor_indicator((int)(current_floor));
    }

    //Check if stop button is pressed
    //Check if stop button is pressed
    if (elev_get_stop_signal()) {
        return stop;
    }

    //Check for orders
    exec_check_order_buttons();

    //Debug purposes
    //order_print_orders();

    //Check if elevator is near a floor. If yes, check if it should stop
    if (current_floor != between_floors) {
        if (exec_should_stop_at_floor(current_floor)) {
            return stay;
        }
    }

    //Set motor direction
    elev_set_motor_direction(DIRN_DOWN);

    //Return hold to continue driving up
    return hold;
}

return_codes_t fsm_stop_state(){
    // Debug purposes
    //printf("State: stop_state\n");

    elev_set_motor_direction(DIRN_STOP);
    elev_set_stop_lamp(1);

    order_remove_all_orders();

    for (floor_codes_t floor = floor_1; floor <= floor_4; floor++) {
        exec_clear_all_order_lights_at_floor(floor);
    }

    floor_codes_t current_floor = elev_get_floor_sensor_signal();

    if (current_floor != between_floors) {
        elev_set_door_open_lamp(1);
    }

    if (!elev_get_stop_signal() && current_floor != between_floors) {
        elev_set_stop_lamp(0);

       if (exec_open_door_3_sec(current_floor) == stop) {
           return hold;
       }
        
        exec_update_destination_floor();

        return stay;
    }
    else if (!elev_get_stop_signal()) {
        elev_set_stop_lamp(0);

        exec_update_destination_floor();
        return idle;
    }
    return hold;

}

return_codes_t fsm_idle_state() {
    // Debug options
    //printf("State: idle\n");

    elev_set_motor_direction(DIRN_STOP);

    //Check if stop button is pressed
    if (elev_get_stop_signal()) {
        return stop;
    }

    exec_check_order_buttons();

    exec_update_destination_floor();

    return exec_get_idle_return_code();
}

// No updates made
return_codes_t fsm_end_state() {
    printf("We reached the end state\n");
    exit(1);
}