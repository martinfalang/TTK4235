#ifndef EXEC_H
#define EXEC_H

///////////////////////////////////////////////
// Includes
///////////////////////////////////////////////

#include "fsm.h"
#include "scheduler.h"
#include "elev.h"

///////////////////////////////////////////////
// Function declarations
///////////////////////////////////////////////

void exec_update_destination_floor();
void exec_delay(int ms);
void exec_set_last_floor(floor_codes_t floor);
void exec_clear_all_order_lights_at_floor(floor_codes_t floor);
void exec_set_last_direction(elev_motor_direction_t direction);

int exec_check_order_buttons();
int exec_should_stop_at_floor(floor_codes_t current_floor);

return_codes_t exec_get_idle_return_code();
return_codes_t exec_get_floor_return_code(floor_codes_t current_floor);
return_codes_t exec_open_door_3_sec(floor_codes_t current_floor);

#endif /* EXEC_H */ 