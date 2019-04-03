#ifndef EXEC_H
#define EXEC_H

#include "fsm.h"
#include "scheduler.h"

int exec_check_order_buttons(void);

int exec_scan_orders();

void exec_timer(int ms);

void exec_set_floor_light();

int exec_update_state_log(state_codes_t prev_state);

return_codes_t get_last_floor();

void exec_update_destination_floor(inside_queue_t* inside_queue, outside_queue_t* outside_queue);

return_codes_t exec_get_return_code(floor_codes_t current_floor);

floor_codes_t exec_get_destination_floor();

void exec_set_destination_floor(floor_codes_t floor);

void exec_set_last_floor(floor_codes_t floor);

direction_codes_t exec_get_last_direction();

void exec_set_last_direction(direction_codes_t direction);


#endif