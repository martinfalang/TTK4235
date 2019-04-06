
///////////////////////////////////////////////
/*
    TOP LEVEL UPDATES:
        - Deleted int current_destination as it 
            was commented out and not used
*/
///////////////////////////////////////////////

#include "fsm.h"
#include "stdio.h"
#include "elev.h"
#include "time.h"
#include "order.h"
#include "exec.h"

#include <stdlib.h> //for exit()

return_codes_t (*state[])(void) = {
    fsm_floor_stationary_state,
    fsm_initialize_state,
    fsm_driving_up_state,
    fsm_driving_down_state,
    fsm_stop_floor_state,
    fsm_stop_between_state,
    fsm_idle_state,
    fsm_end_state
};

///////////////////////////////////////////////
/*
    UPDATES
        - Added fail state transistion to idle state
        - Updated initialize to initialize transition
            to trigger on hold return code. This because
            all the other states hold their current state
            when return code is hold
        - Updated initialize to floor_stationary return
            code to stay
        - Updated transition between stop_between and idle_state
            to stay instead of idle. Idle is no longer a 
            return code
*/
///////////////////////////////////////////////
transition_t state_transitions[] = {
    {initialize, hold, initialize},
    {initialize, stay, floor_stationary},            //Added this one in testing, better return codes should be implemented
    {initialize, stop_flr, stop_floor},
    {initialize, stop_btw, stop_between},
    {initialize, fail, end},

    {floor_stationary, hold, floor_stationary},
    {floor_stationary, drive_up, driving_up},
    {floor_stationary, drive_down, driving_down},
    {floor_stationary, stop_flr, stop_floor},
    {floor_stationary, fail, end},

    {driving_up, stay, floor_stationary},
    {driving_up, hold, driving_up},
    {driving_up, stop_btw, stop_between},
    {driving_up, stop_flr, stop_floor},
    {driving_up, fail, end},

    {driving_down, stay, floor_stationary},
    {driving_down, hold, driving_down},
    {driving_down, stop_btw, stop_between},
    {driving_down, stop_flr, stop_floor},
    {driving_down, fail, end},

    {stop_between, hold, stop_between},
    {stop_between, stay, idle_state},
    {stop_between, fail, end},

    {stop_floor, hold, stop_floor},
    {stop_floor, stay, floor_stationary},
    {stop_floor, fail, end},

    {idle_state, hold, idle_state},
    {idle_state, stop_btw, stop_between},
    {idle_state, stop_flr, stop_floor},
    {idle_state, drive_up, driving_up},
    {idle_state, drive_down, driving_down},
    {idle_state, fail, end}
};

// No updates made
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

///////////////////////////////////////////////
/*
    UPDATES:
        - Updated return code when floor < 0 to 
            hold
        - Updated return code hold to stay for when
            we leave initialize and go into 
            floor_stationary
        - Deleted exec_set_floor_light function call.
            This function is not neccessary here and
            is also removed from exec files
*/
///////////////////////////////////////////////
return_codes_t fsm_initialize_state(void)
{
    // Debug options
    printf("State: initilize\n");
    elev_set_motor_direction(DIRN_DOWN);

    floor_codes_t floor = elev_get_floor_sensor_signal();

    if (elev_get_stop_signal() && floor >= 0) {
        return stop_flr;
    }
    else if (elev_get_stop_signal()) {
        return stop_btw;
    }
    else if(floor < 0) {
        return hold;
    }
    else {
        exec_update_destination_floor();
        return stay;
    }
}   
///////////////////////////////////////////////
/*
    UPDATES:
        - Made opening door logic now just calling
            execopen_door_3_sec function
*/
///////////////////////////////////////////////
return_codes_t fsm_floor_stationary_state(void) {
    
    // Debug options
    printf("State: floor_stationary_state\n");

    exec_set_last_direction(STOP_DIR);

    floor_codes_t current_floor = elev_get_floor_sensor_signal();
    
    exec_set_last_floor(current_floor);
    
    elev_set_floor_indicator(current_floor);

    //Stop elevator
    elev_set_motor_direction(DIRN_STOP);

    if (exec_scan_orders()) {
        exec_open_door_3_sec();
    }
    

    //Checks if stop button is pressed MAKE THIS A FUNCTION
    if (elev_get_stop_signal()) {
        return stop_flr;
    }

    //Update queue
    exec_check_order_buttons();
    

    //Debug purposes
    order_print_orders();

    //Update current destination_floor
    exec_update_destination_floor();

    

    //printf("Dest. floor: %i\n", exec_get_destination_floor());
    //Get return code and return this
    return_codes_t return_code = exec_get_return_code();
    return return_code;
}

return_codes_t fsm_driving_up_state(void) 
{
    printf("State: Driving up\n");

    //Set last_direction to UP
    exec_set_last_direction(UP);


    //Check if elevator is at a floor
    floor_codes_t current_floor = elev_get_floor_sensor_signal();

    //Update last floor if elevator is at a floor
    if (current_floor != between_floors) {
        exec_set_last_floor(current_floor);
    }

    //Check if stop button is pressed
    int stop = elev_get_stop_signal();
    if (stop && current_floor == -1) { //Check if this works!
        return stop_btw;
    }
    else if (stop) {
        return stop_flr;
    }

    //Check for orders
    exec_check_order_buttons();

    //Debug purposes
    order_print_orders();

    //Check if elevator is near a floor. If yes, check if it should stop
    if (current_floor != between_floors) {
        if (exec_scan_orders()) {
            return stay;
        }
    }

    //Set motor direction
    elev_set_motor_direction(DIRN_UP);

    //Return hold to continue driving up
    return hold;
}

return_codes_t fsm_driving_down_state(void) 
{
    printf("State: Driving down\n");
    
    //Set last_direction to DOWN
    exec_set_last_direction(DOWN);

    //Check if elevator is at a floor
    floor_codes_t current_floor = elev_get_floor_sensor_signal();

    //Update last floor if elevator is at a floor
    if (current_floor != between_floors) {
        exec_set_last_floor(current_floor);
    }

    //Check if stop button is pressed
    int stop = elev_get_stop_signal();
    if (stop && current_floor == -1) { //Check if this works!
        return stop_btw;
    }
    else if (stop) {
        return stop_flr;
    }

    //Check for orders
    exec_check_order_buttons();

    //Debug purposes
    order_print_orders();

    //Check if elevator is near a floor. If yes, check if it should stop
    if (current_floor != between_floors) {
        if (exec_scan_orders()) {
            printf("Dest. floor: %d\n", exec_get_destination_floor());
            return stay;
        }
    }

    //Set motor direction
    elev_set_motor_direction(DIRN_DOWN);

    //Return hold to continue driving up
    return hold;
}
//NOTE: Maybe make stop_floor and stop_between states one state
return_codes_t fsm_stop_floor_state(void) {
    printf("State: stop_floor_state\n");

    elev_set_motor_direction(DIRN_STOP);
    elev_set_stop_lamp(1);

    elev_set_door_open_lamp(1);
    order_remove_all();
    
    for (floor_codes_t floor = floor_1; floor <= floor_4; floor++) {
        exec_clear_all_order_lights_at_floor(floor);
    }

    if (!elev_get_stop_signal()) {
        elev_set_stop_lamp(0);
        elev_set_door_open_lamp(0);
        
        exec_update_destination_floor();
        return stay;
    }
    return hold;
}

///////////////////////////////////////////////
/*
    UPDATES:
        - Updated return code when stop button
            is NOT pressed to stay instead of
            idle
*/
///////////////////////////////////////////////
return_codes_t fsm_stop_between_state(void) {
    //THIS HAS TO BE FIXED, LOGIC NOT WORKING, REACHES END STATE WHEN NEW ORDER

    printf("State: stop_between_state\n");

    elev_set_motor_direction(DIRN_STOP);
    elev_set_stop_lamp(1);

    order_remove_all();

    for (floor_codes_t floor = floor_1; floor <= floor_4; floor++) {
        exec_clear_all_order_lights_at_floor(floor);
    }

    if (!elev_get_stop_signal()) {
        elev_set_stop_lamp(0);

        exec_update_destination_floor();
        return stay;
    }
    order_print_orders();
    return hold; 
}

return_codes_t fsm_idle_state(void) {
    printf("State: idle\n");
    elev_set_motor_direction(DIRN_STOP);
    if (elev_get_stop_signal() && elev_get_floor_sensor_signal() >= 0) {
        return stop_flr;
    }
    else if (elev_get_stop_signal()) {
        return stop_btw;
    }
    exec_check_order_buttons();

    exec_update_destination_floor();

    return exec_get_return_code();
}

return_codes_t fsm_end_state(void) {
    printf("We reached the end state\n");
    exit(1);
}