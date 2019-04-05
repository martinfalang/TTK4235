#ifndef EXEC_H
#define EXEC_H

///////////////////////////////////////////////
/*
    TOP LEVEL UPDATES:
        - Deleted exec_update_state_log function
        - Deleted exec_set_destination_floor function
        - Added comment next to endif for good practice
*/
///////////////////////////////////////////////

#include "fsm.h"
#include "scheduler.h"

int exec_check_order_buttons(void);

int exec_scan_orders();

void exec_timer(int ms);

void exec_set_floor_light();

return_codes_t get_last_floor();

void exec_update_destination_floor();

return_codes_t exec_get_return_code();

floor_codes_t exec_get_destination_floor();

void exec_set_last_floor(floor_codes_t floor);

direction_codes_t exec_get_last_direction();

void exec_set_last_direction(direction_codes_t direction);

void exec_clear_all_order_lights_at_floor(floor_codes_t floor);

#endif /* EXEC_H */ 