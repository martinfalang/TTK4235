#include "exec.h"
#include "elev.h"
#include "order.h"
#include "time.h"

#include <stdio.h>

//[0, 0, 0, 0] = undefined, exec_get_position returns 0
//[1, 0, 0, 0] = floor 1, exec_get_position returns 1
//[1, 1, 0, 0] = between floor 1 and 2, exec_get_position returns 2
//[0, 1, 0, 0] = floor 2, exec_get_position returns 3
//[0, 1, 1, 0] = between floor 2 and 3, exec_get_position returns 4
//[0, 0, 1, 0] = floor 3, exec_get_position returns 5
//[0, 0, 1, 1] = between floor 3 and 4, exec_get_position returns 6
//[0, 0, 0, 1] = floor 4, exec_get_position returns 7
//static int current_position[4] = {0, 0, 0, 0};

static state_codes_t last_floor;
static state_codes_t last_direction;

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

int exec_update_position(state_codes_t prev_state) {
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
        printf("Error: prev_state not a floor or direction\n");
        return -1;
    }
}


//return 1 if elevator should stop on the way
int exec_scan_orders(int destination_floor, state_codes_t current_state) {
    int *order_array = order_get_orders();
    int current_floor = elev_get_floor_sensor_signal();

    if (current_state == driving_up && destination_floor - current_floor > 0) {
        if (order_array[outside_2_up] || order_array[outside_3_up]) {
            return 1;
        }
    }
    else if (current_state == driving_down && destination_floor - current_floor < 0) {
        if (order_array[outside_2_down] || order_array[outside_3_down]) {
            return 1;
        }
    }
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
