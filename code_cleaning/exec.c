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
        
*/
///////////////////////////////////////////////

#include "exec.h"
#include "elev.h"
#include "order.h"
#include "time.h"
#include "scheduler.h"

#include <stdio.h>

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

return_codes_t exec_get_idle_return_code();

return_codes_t get_last_floor() {
    if (last_floor == floor_1) {
        return arrived_1;
    }
    else if (last_floor == floor_2) {
        return arrived_2;
    }
    else if (last_floor == floor_3) {
        return arrived_3;
    }
    else if (last_floor == floor_4) {
        return arrived_4;
    }
    else {
        printf("Error: get_last_floor(): argument not a floor");
        return fail;
    }
}

///////////////////////////////////////////////
/*
    Updates:
        - Updated all order_add function calls
            to use elev_motor_direction_t instead
            of direction_codes_t
*/
///////////////////////////////////////////////
int exec_check_order_buttons(void) {
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

//return 1 if elevator should stop on the way
//is only run when at a floor. Checks if any orders are at this floor and returns 1 if elevator should stop
int exec_scan_orders() {
    inside_queue_t  *inside_queue_ptr = order_get_inside_queue();
    outside_queue_t *outside_queue_ptr = order_get_outside_queue();

    int current_floor = elev_get_floor_sensor_signal();

    if (current_floor == destination_floor) {
        return 1;
    }

    //Check inside orders
    else if (inside_queue_ptr->length) {
        for(int i = 0; i < inside_queue_ptr->length; i++) {
            if(inside_queue_ptr->queue[i].floor == current_floor)
            {
                return 1;
            }
        }
    }
    
    else if (outside_queue_ptr->length) {
    //Check outside orders
        int dir = last_direction;
        for(int i = 0; i < outside_queue_ptr->length; i++)
        {
            if (current_floor == 0 || current_floor == 3) {
                if(outside_queue_ptr->queue[i].floor == current_floor)
                {
                    return 1;
                }
            }
            else if (outside_queue_ptr->queue[i].floor == current_floor && last_direction == STOP_DIR) {
                //printf("/////////////////////////////////////\nWHY U RUNNING\n//////////////////////////////\n");
                return 1;
            }
            else if(outside_queue_ptr->queue[i].floor == current_floor && outside_queue_ptr->queue[i].direction == dir)
            {
                return 1;
            }
        }
    }
    //If nothing found, return 0
    return 0;


}

void exec_timer(int ms) {
    struct timespec req;

	req.tv_sec = ms / 1000;
	req.tv_nsec = (ms % 1000) * 1000000;

	nanosleep(&req, 0);
}

// No updates made
void exec_set_floor_light() {
    int floor = elev_get_floor_sensor_signal();
    if (floor >= 0) {
        elev_set_floor_indicator(floor);
    }
}

//No updates made
//Checks inside queue for orders first. If any orders, choose the first. If not check outside orders. 
//If outside_orders exists, choose the first. If not any outside_orders, don't change destination floor
void exec_update_destination_floor()
{
    inside_queue_t *inside_queue = order_get_inside_queue();
    outside_queue_t *outside_queue = order_get_outside_queue();
    if (inside_queue->length) destination_floor = inside_queue->queue[0].floor;
    else if (outside_queue->length) destination_floor = outside_queue->queue[0].floor;
    else destination_floor = between_floors;
}

//Maybe improve this
return_codes_t exec_get_return_code()
{   
    floor_codes_t current_floor = elev_get_floor_sensor_signal();
    
    if (destination_floor == between_floors) {
        return hold;
    }
    else if (current_floor == between_floors) {
        return exec_get_idle_return_code();
    }
    else if(current_floor != destination_floor)
    {
        if(current_floor < destination_floor) {
            return drive_up;
        }
        else {
            return drive_down;
        }
    }
    else{
        return hold;
    }
}

return_codes_t exec_get_idle_return_code() {
    switch (destination_floor)
    {
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
    
        default:{
            return fail;
            break;
        }
    }
}

floor_codes_t exec_get_destination_floor()
{
    return destination_floor;
}

void exec_set_last_floor(floor_codes_t floor) {
    last_floor = floor;
}

void exec_set_last_direction(direction_codes_t direction) {
    last_direction = direction;
}

direction_codes_t exec_get_last_direction()
{
    if(last_direction == UP)
    {
        return UP;
    }
    else
    {
        return DOWN;
    }
}

void exec_clear_all_order_lights_at_floor(floor_codes_t floor) {
    
    switch (floor)
        {
            case floor_1: {
                elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
                elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
                break;
            }
            case floor_4:
            {
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