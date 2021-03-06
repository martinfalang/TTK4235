
///////////////////////////////////////////////
/*
    TOP LEVEL UPDATES:
        - Deleted int current_destination as it 
            was commented out and not used
        - Made stop_floor_state and stop_between state
            one state
*/
///////////////////////////////////////////////

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
        - Updated table to include new stop state
*/
///////////////////////////////////////////////
transition_t state_transitions[] = {
    {initialize, hold, initialize},
    {initialize, stay, floor_stationary},            //Added this one in testing, better return codes should be implemented
    {initialize, stop, stop_state},
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
        - Updated to return just stop if the stop
            button is pressed
*/
///////////////////////////////////////////////
return_codes_t fsm_initialize_state(void)
{
    // Debug options
    printf("State: initilize\n");
    elev_set_motor_direction(DIRN_DOWN);

    floor_codes_t floor = elev_get_floor_sensor_signal();

    if (elev_get_stop_signal()) {
        return stop;
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
        - Now returns just stop instead of stop_flr
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
        return stop;
    }

    //Update queue
    exec_check_order_buttons();
    
    //Debug purposes
    order_print_orders();

    //Update current destination_floor
    exec_update_destination_floor();

    return exec_get_floor_return_code();
}
///////////////////////////////////////////////
/*
    UPDATES:
        - Now returns just stop instead of stop_flr
            and stop_btw
*/
///////////////////////////////////////////////
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
    if (elev_get_stop_signal()) {
        return stop;
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
///////////////////////////////////////////////
/*
    UPDATES:
        - Deleted printf statement printing destination
            floor. exec_get_destination_floor is no 
            longer a function
        - Now returns just stop instead of stop_flr
            and stop_btw
*/
///////////////////////////////////////////////
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
    //Check if stop button is pressed
    if (elev_get_stop_signal()) {
        return stop;
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
    elev_set_motor_direction(DIRN_DOWN);

    //Return hold to continue driving up
    return hold;
}
///////////////////////////////////////////////
/*
    UPDATES:
        - This function is a merge of the other
            two stop_functions we had before. The
            reason for merging was that there was
            a lot of similar code and minimal additional
            logic was needed to merge them
*/
///////////////////////////////////////////////
return_codes_t fsm_stop_state(){
    // Debug purposes
    prinft("State: stop_state\n");

    elev_set_motor_direction(DIRN_STOP);
    elev_set_stop_lamp(1);

    order_remove_all();

    for (floor_codes_t floor = floor_1; floor <= floor_4; floor++) {
        exec_clear_all_order_lights_at_floor(floor);
    }

    floor_codes_t current_floor = elev_get_floor_sensor_signal();

    if (current_floor != between_floors) {
        elev_set_door_open_lamp(1);
    }

    if (!elev_get_stop_signal() && current_floor != between_floors) {
        elev_set_stop_lamp(0);
        elev_set_door_open_lamp(0);
        
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
///////////////////////////////////////////////
/*
    UPDATES:
        - Updated return exec_get_return_code
            to instead call exec_get_idle_return_code
            since this is now it's own function
        - Now returns just stop instead of stop_flr
            and stop_btw
*/
///////////////////////////////////////////////
return_codes_t fsm_idle_state(void) {
    printf("State: idle\n");
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
return_codes_t fsm_end_state(void) {
    printf("We reached the end state\n");
    exit(1);
}