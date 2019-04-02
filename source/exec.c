#include "exec.h"
#include "elev.h"
#include "order.h"
#include "time.h"
#include "scheduler.h"

#include <stdio.h>

static state_codes_t last_floor;
static state_codes_t last_direction;

static state_codes_t destination_floor;

int exec_last_direction_to_int();

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

int exec_check_order_buttons(void) {
    for (int floor = 0; floor < 4; floor++) {
        for (int type = 0; type < 3; type++) {
            if ((type == 1 && floor == 0) || (type == 0 && floor == 3)) {
                continue;
            }
            else if (elev_get_button_signal(type, floor)) {
                if (floor == 0 && type == 0) {
                    order_add(outside_1_up);
                }
                else if (floor == 0 && type == 2) {
                    order_add(inside_1);
                }
                else if (floor == 1 && type == 0) {
                    order_add(outside_2_up);
                }
                else if (floor == 1 && type == 1) {
                    order_add(outside_2_down);
                }
                else if (floor == 1 && type == 2) {
                    order_add(inside_2);
                }
                else if (floor == 2 && type == 0) {
                    order_add(outside_3_up);
                }
                else if (floor == 2 && type == 1) {
                    order_add(outside_3_down);
                }
                else if (floor == 2 && type == 2) {
                    order_add(inside_3);
                }
                else if (floor == 3 && type == 1) {
                    order_add(outside_4_down);
                }
                else if (floor == 3 && type == 2) {
                    order_add(inside_4);
                }
            }

        }
    }
    return 0;
}

//Function purpose: Update last_direction or last_floor
int exec_update_state_log(state_codes_t prev_state) {
    if (prev_state == driving_up || prev_state == driving_down) {
        last_direction = prev_state;
        return 0;
    }
    else if (prev_state == floor_1 || prev_state == floor_2
        || prev_state == floor_3 || prev_state == floor_4) {
            last_floor = prev_state;
            return 0;
    }
    else {
        printf("Error: exec_update_position: prev_state not a floor or direction\n");
        return -1;
    }
}


//return 1 if elevator should stop on the way
//is only run when at a floor. Checks if any orders are at this floor and returns 1 if elevator should stop
int exec_scan_orders(state_codes_t current_state) {
    inside_queue_t  *inside_queue_ptr = order_get_inside_queue();
    outside_queue_t *outside_queue_ptr = order_get_outside_queue();

    int current_floor = elev_get_floor_sensor_signal();

    //Check inside orders
    if (inside_queue_ptr->length) {
        for(int i = 0; i < inside_queue_ptr->length; i++) {
            if(inside_queue_ptr->queue[i].floor == current_floor)
            {
                return 1;
            }
        }
    }
    
    if (outside_queue_ptr->length) {
    //Check outside orders
        for(int i = 0; i < outside_queue_ptr->length; i++)
        {
            int dir = exec_last_direction_to_int();
            if (current_floor == 0 || current_floor == 3) {
                if(outside_queue_ptr->queue[i].floor == current_floor)
                {
                    return 1;
                }
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

void exec_set_floor_light() {
    int floor = elev_get_floor_sensor_signal();
    if (floor >= 0) {
        elev_set_floor_indicator(floor);
    }
}

int exec_last_direction_to_int()
{
    if (last_direction==driving_up) return 1;
    else if (last_direction==driving_down) return -1;
    else return 0;
}


//Checks inside queue for orders first. If any orders, choose the first. If not check outside orders. 
//If outside_orders exists, choose the first. If not any outside_orders, don't change destination floor
void exec_update_destination_floor(state_codes_t current_state, inside_queue_t* inside_queue, outside_queue_t* outside_queue)
{
    if (inside_queue->length) destination_floor = inside_queue->queue[0].floor;
    else if (outside_queue->length) destination_floor = outside_queue->queue[0].floor;
    else return;
}

return_codes_t exec_get_return_code(state_codes_t current_state, inside_queue_t* inside_queue, outside_queue_t* outside_queue)
{
    return hold;
}

state_codes_t exec_get_destination_floor()
{
    return destination_floor;
}

void exec_intialize_destination_floor()
{
    destination_floor=floor_1;
}

return_codes_t exec_get_last_direction()
{
    if(last_direction == driving_up)
    {
        return drive_up;
    }
    else if (last_direction == driving_down)
    {
        return drive_down;
    }
    else {
        return fail;
    }
}
