
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

static floor_codes_t last_floor;
static elev_motor_direction_t last_direction;

static floor_codes_t destination_floor;

///////////////////////////////////////////////
// Functions
///////////////////////////////////////////////

return_codes_t exec_open_door_3_sec(floor_codes_t current_floor) {
    
    elev_set_door_open_lamp(1);

    for (int i = 0; i < 300; i++) {
        exec_delay(10);
        exec_check_order_buttons();

        exec_clear_all_order_lights_at_floor(current_floor);
        
        // Resets timer if there is a new order to handle on this floor
        if (exec_should_stop_at_floor(current_floor)) {
            i = 0;
        }
        
        order_remove_all_orders_at_floor(current_floor);

        if (elev_get_stop_signal()) {
            return stop;
        }
    }

    elev_set_door_open_lamp(0);
    return hold;
}

//Checks inside queue for orders first. If any orders, choose the first. If not check outside orders. 
//If outside_orders exists, choose the first. If not any outside_orders, don't change destination floor
void exec_update_destination_floor() {
    inside_queue_t *inside_queue = order_get_inside_queue();
    outside_queue_t *outside_queue = order_get_outside_queue();
    if (inside_queue->length) destination_floor = inside_queue->queue[0].floor;
    else if (outside_queue->length) destination_floor = outside_queue->queue[0].floor;
    else destination_floor = between_floors;
}

void exec_delay(int ms) {
    struct timespec req;

	req.tv_sec = ms / 1000;
	req.tv_nsec = (ms % 1000) * 1000000;

	nanosleep(&req, 0);
}

void exec_set_last_floor(floor_codes_t floor) {
    last_floor = floor;
}

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

void exec_set_last_direction(elev_motor_direction_t direction) {
    last_direction = direction;
}

void exec_check_order_buttons() {
    for (int floor = 0; floor < 4; floor++) {
        for (int type = 0; type < 3; type++) {
            if ((type == 1 && floor == 0) || (type == 0 && floor == 3)) {
                continue;
            }
            else if (elev_get_button_signal(type, floor)) {
                if (floor == 0 && type == 0) {
                    order_add_order_to_queue(floor_1, DIRN_UP);
                    elev_set_button_lamp(BUTTON_CALL_UP, floor, 1);
                }
                else if (floor == 0 && type == 2) {
                    order_add_order_to_queue(floor_1, DIRN_STOP);
                    elev_set_button_lamp(BUTTON_COMMAND, floor, 1);
                }
                else if (floor == 1 && type == 0) {
                    order_add_order_to_queue(floor_2, DIRN_UP);
                    elev_set_button_lamp(BUTTON_CALL_UP, floor, 1);
                }
                else if (floor == 1 && type == 1) {
                    order_add_order_to_queue(floor_2, DIRN_DOWN);
                    elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 1);
                }
                else if (floor == 1 && type == 2) {
                    order_add_order_to_queue(floor_2, DIRN_STOP);
                    elev_set_button_lamp(BUTTON_COMMAND, floor, 1);
                }
                else if (floor == 2 && type == 0) {
                    order_add_order_to_queue(floor_3, DIRN_UP);
                    elev_set_button_lamp(BUTTON_CALL_UP, floor, 1);
                }
                else if (floor == 2 && type == 1) {
                    order_add_order_to_queue(floor_3, DIRN_DOWN);
                    elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 1);
                }
                else if (floor == 2 && type == 2) {
                    order_add_order_to_queue(floor_3, DIRN_STOP);
                    elev_set_button_lamp(BUTTON_COMMAND, floor, 1);
                }
                else if (floor == 3 && type == 1) {
                    order_add_order_to_queue(floor_4, DIRN_DOWN);
                    elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 1);
                }
                else if (floor == 3 && type == 2) {
                    order_add_order_to_queue(floor_4, DIRN_STOP);
                    elev_set_button_lamp(BUTTON_COMMAND, floor, 1);
                }
            }

        }
    }
}

//return 1 if elevator should stop on the way
//is only run when at a floor. Checks if any orders are at this floor and returns 1 if elevator should stop
int exec_should_stop_at_floor(floor_codes_t current_floor) {
    
    inside_queue_t  *inside_queue_ptr = order_get_inside_queue();
    outside_queue_t *outside_queue_ptr = order_get_outside_queue();
    
    // Check inside orders
    if (inside_queue_ptr->length) {
        for(int i = 0; i < inside_queue_ptr->length; i++) {
            if(inside_queue_ptr->queue[i].floor == current_floor)
            {
                return 1;
            }
        }
    }
    // Check outside orders
    if (outside_queue_ptr->length) {
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
    return 0;
}

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
            if (last_floor < floor_2 || (last_floor == floor_2 && last_direction == DIRN_DOWN)) {
                return drive_up;
            }
            else {
                return drive_down;
            }
            break;
        }
        case floor_3: {
            if (last_floor > floor_3 || (last_floor == floor_3 && last_direction == DIRN_UP)) {
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
