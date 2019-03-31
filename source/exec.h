#ifndef EXEC_H
#define EXEC_H

#include "fsm.h"

int exec_check_order_buttons(void);

int exec_scan_orders(int destination_floor, state_codes_t current_state);

int exec_execute_order(int type);

int exec_interact_door(int value);

int exec_timer(int ms);






#endif