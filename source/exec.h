#ifndef EXEC_H
#define EXEC_H

#include "fsm.h"

int exec_check_order_buttons(void);

int exec_scan_orders(int destination_floor, state_codes_t current_state);

void exec_timer(int ms);


#endif