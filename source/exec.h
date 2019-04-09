#ifndef EXEC_H
#define EXEC_H

///////////////////////////////////////////////
/*
    TOP LEVEL UPDATES:
        - Deleted exec_update_state_log function
        - Deleted exec_set_destination_floor function
        - Added comment next to endif for good practice
        - Added exec_open_door_3_sec function
        - Changed name of exec_scan_orders to 
            exec_should_stop_at_floor and added argument
            current_floor to function
        - Changed name of exec_timer function to 
            exec_delay
        - Deleted exec_get_last_direction function
        - Deleted get_last_floor function
        - Deleted exec_set_floor_lights function
        - Updated argument of exec_set_last_direction
            function to be of type elev_motor_direction_t
        - Added exec_get_idle_return_code function 
        - Updated name of exec_get_return_code function to
            exec_get_floor_return_code
        - Updated argument list of exec_get_floor_return_code
            function to include floor_codes_t current_floor 
        - Deleted exec_get_destination_floor function
*/
///////////////////////////////////////////////

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