#include "exec.h"
#include "elev.h"
#include "order.h"
#include "time.h"


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

