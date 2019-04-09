///////////////////////////////////////////////
/*
    PROPOSITIONS:
        - Move all types (such as floor_codes_t etc)
            that are used by multiple .c and .h files
            into one .h file included everywhere
            for example a types.h
*/
///////////////////////////////////////////////


///////////////////////////////////////////////
/*
    TOP LEVEL UPDATES:
        - Deleted exec_update_state_log function
            as it is no longer used
        - Deleted exec_last_direction_to_int function
            as it is no longer used
        - Deleted exec_set_last_direction function
            as it is no longer used
        - Deleted exec_get_destination_floor
            function as it was only used for
            debug purposes in fsm.c
        - Added exec_open_door_3_sec function
        - Modified exec_scan_floor function to take 
            floor_codes_t current_floor as argument.
        - Deleted exec_get_last_direction function
            as it is never used in fsm.c 
        - Deleted get_last_floor function as it is
            no longer used anywhere
        - Deleted exec_set_floor_light function as
            it was only used in initialize state 
            and it wasn't neccessary there
        - Deleted exec_get_idle_return_code declaration as it 
            is now declared in exec.h
        
*/
///////////////////////////////////////////////

///////////////////////////////////////////////
// Includes
///////////////////////////////////////////////

#include "exec.h"
#include "elev.h"
#include "order.h"
#include "time.h"
#include "scheduler.h"

#include <stdio.h>

///////////////////////////////////////////////
// Private variables
///////////////////////////////////////////////

///////////////////////////////////////////////
/*
    Updates:
        - Changed last_direction to type
            elev_motor_direction_t
*/
///////////////////////////////////////////////


static floor_codes_t last_floor;
static elev_motor_direction_t last_direction;

static floor_codes_t destination_floor;

///////////////////////////////////////////////
// Functions
///////////////////////////////////////////////

///////////////////////////////////////////////
/*
    UPDATES:
        Made this its own function
*/
///////////////////////////////////////////////
void exec_open_door_3_sec(floor_codes_t current_floor) {
    exec_clear_all_order_lights_at_floor(current_floor);

    order_remove(current_floor);
    elev_set_door_open_lamp(1);

    for (int i = 0; i < 3000; i++) {
        exec_timer(1);
        exec_check_order_buttons();

        if (elev_get_stop_signal()) {
            return stop;
        }
    }
    elev_set_door_open_lamp(0);
}

//No updates made
//Checks inside queue for orders first. If any orders, choose the first. If not check outside orders. 
//If outside_orders exists, choose the first. If not any outside_orders, don't change destination floor
void exec_update_destination_floor() {
    inside_queue_t *inside_queue = order_get_inside_queue();
    outside_queue_t *outside_queue = order_get_outside_queue();
    if (inside_queue->length) destination_floor = inside_queue->queue[0].floor;
    else if (outside_queue->length) destination_floor = outside_queue->queue[0].floor;
    else destination_floor = between_floors;
}

///////////////////////////////////////////////
/*
    UPDATES:
        - Changed name to exec_delay to better
            explain what the function does
*/
///////////////////////////////////////////////
void exec_delay(int ms) {
    struct timespec req;

	req.tv_sec = ms / 1000;
	req.tv_nsec = (ms % 1000) * 1000000;

	nanosleep(&req, 0);
}

// No updates made
void exec_set_last_floor(floor_codes_t floor) {
    last_floor = floor;
}

// No updates made
void exec_clear_all_order_lights_at_floor(floor_codes_t floor) {
    
    switch (floor)
        {
            case floor_1: {
                elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
                elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
                break;
            }
            case floor_4: {
                elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
                elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0);
                break;
            }
            default: {
                elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
                elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
                elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0);
                break;
            }
        }
}

///////////////////////////////////////////////
/*
    UPDATES:
        - Updated argument direction to be of type
            elev_motor_direction_t
*/
///////////////////////////////////////////////
void exec_set_last_direction(elev_motor_direction_t direction) {
    last_direction = direction;
}

///////////////////////////////////////////////
/*
    Updates:
        - Updated all order_add function calls
            to use elev_motor_direction_t instead
            of direction_codes_t
*/
///////////////////////////////////////////////
int exec_check_order_buttons() {
    for (int floor = 0; floor < 4; floor++) {
        for (int type = 0; type < 3; type++) {
            if ((type == 1 && floor == 0) || (type == 0 && floor == 3)) {
                continue;
            }
            else if (elev_get_button_signal(type, floor)) {
                if (floor == 0 && type == 0) {
                    order_add(floor_1, DIRN_UP);
                    elev_set_button_lamp(BUTTON_CALL_UP, floor, 1);
                }
                else if (floor == 0 && type == 2) {
                    order_add(floor_1, DIRN_STOP);
                    elev_set_button_lamp(BUTTON_COMMAND, floor, 1);
                }
                else if (floor == 1 && type == 0) {
                    order_add(floor_2, DIRN_UP);
                    elev_set_button_lamp(BUTTON_CALL_UP, floor, 1);
                }
                else if (floor == 1 && type == 1) {
                    order_add(floor_2, DIRN_DOWN);
                    elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 1);
                }
                else if (floor == 1 && type == 2) {
                    order_add(floor_2, DIRN_STOP);
                    elev_set_button_lamp(BUTTON_COMMAND, floor, 1);
                }
                else if (floor == 2 && type == 0) {
                    order_add(floor_3, DIRN_UP);
                    elev_set_button_lamp(BUTTON_CALL_UP, floor, 1);
                }
                else if (floor == 2 && type == 1) {
                    order_add(floor_3, DIRN_DOWN);
                    elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 1);
                }
                else if (floor == 2 && type == 2) {
                    order_add(floor_3, DIRN_STOP);
                    elev_set_button_lamp(BUTTON_COMMAND, floor, 1);
                }
                else if (floor == 3 && type == 1) {
                    order_add(floor_4, DIRN_DOWN);
                    elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 1);
                }
                else if (floor == 3 && type == 2) {
                    order_add(floor_4, DIRN_STOP);
                    elev_set_button_lamp(BUTTON_COMMAND, floor, 1);
                }
            }

        }
    }
    return 0;
}

///////////////////////////////////////////////
/*
    Updates:
        - Changed name to exec_should_stop_at_floor to
            better explain what function does
        - Now takes floor_codes_t current_floor as argument.
            This because all functions calling it
            already call the elev_get_floor_sensor_signal
            function themselves so this makes it so that it
            so that the function is only called once per state 
        - Deleted printf statement in else if check when checking
            if last_dir == STOP_DIR as it was only used for
            debugging
        - Updated direction to be of type elev_motor_direction_t
        - Updated checks for current_floor to be of type floor_codes_t
        - Deleted dir variable and using last_direction directly
            instead.
        - Added else statement at the end because it looks nice :)
*/
///////////////////////////////////////////////
//return 1 if elevator should stop on the way
//is only run when at a floor. Checks if any orders are at this floor and returns 1 if elevator should stop
int exec_should_stop_at_floor(floor_codes_t current_floor) {
    
    inside_queue_t  *inside_queue_ptr = order_get_inside_queue();
    outside_queue_t *outside_queue_ptr = order_get_outside_queue();

    if (current_floor == destination_floor) {
        return 1;
    }
    // Check inside orders
    else if (inside_queue_ptr->length) {
        for(int i = 0; i < inside_queue_ptr->length; i++) {
            if(inside_queue_ptr->queue[i].floor == current_floor)
            {
                return 1;
            }
        }
    }
    // Check outside orders
    else if (outside_queue_ptr->length) {
        for(int i = 0; i < outside_queue_ptr->length; i++)
        {
            if (current_floor == floor_1 || current_floor == floor_4) {
                if(outside_queue_ptr->queue[i].floor == current_floor)
                {
                    return 1;
                }
            }
            else if (outside_queue_ptr->queue[i].floor == current_floor && last_direction == DIRN_STOP) {
                return 1;
            }
            else if(outside_queue_ptr->queue[i].floor == current_floor && outside_queue_ptr->queue[i].direction == last_direction)
            {
                return 1;
            }
        }
    }
    //If nothing found, return 0
    else {
        return 0;
    }
}

///////////////////////////////////////////////
/*
    UPDATES:
        - This function is now a public function
            visible to fsm.c
        - Added a statement in switch block where
            the destination floor is between_floors
*/
///////////////////////////////////////////////
return_codes_t exec_get_idle_return_code() {
    switch (destination_floor)
    {
        case between_floors: {
            return hold;
            break;
        }
        case floor_1: {
            return drive_down;
            break;
        }
        case floor_2: {
            if (last_floor < floor_2 || (last_floor == floor_2 && last_direction == DOWN)) {
                return drive_up;
            }
            else {
                return drive_down;
            }
            break;
        }
        case floor_3: {
            if (last_floor > floor_3 || (last_floor == floor_3 && last_direction == UP)) {
                return drive_down;
            }
            else {
                return drive_up;
            }
            break;
        }
        case floor_4: {
            return drive_up;
            break;
        }
    
        default: {
            return fail;
            break;
        }
    }
}


///////////////////////////////////////////////
/*
    UPDATES:
        - Changed name to exec_get_floor_return_code.
            This since the only functions in fsm.c calling
            this functions are the floor_stationary state
            and the idle state. It therefore makes sense to
            just split the function since they are almost
            doing that already.
        - Also takes in floor_codes_t current_floor argument
            so that elev_get_floor_sensor_signal doesn't have
            to be called twice in the state
        - No longer calls exec_get_idle_return_code as this function
            will now be made public to the fsm.c
*/
///////////////////////////////////////////////
return_codes_t exec_get_floor_return_code(floor_codes_t current_floor) {   
    if (destination_floor == between_floors) {
        return hold;
    }
    else if (current_floor != destination_floor)
    {
        if(current_floor < destination_floor) {
            return drive_up;
        }
        else {
            return drive_down;
        }
    }
    else {
        return hold;
    }
}
