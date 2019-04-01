#ifndef EXEC_H
#define EXEC_H

#include "fsm.h"

int exec_check_order_buttons(void);

int exec_scan_orders(int destination_floor, state_codes_t current_state);

void exec_timer(int ms);

void exec_set_floor_light();

int exec_update_position(state_codes_t prev_state);

return_codes_t get_last_floor();

state_codes_t exec_update_destination_floor(state_codes_t current_state, int *orderArray);


#endif