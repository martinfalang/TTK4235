/**
 * @file
 * @brief A library for executing tasks for the elevator.
 */

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

/**
 * @brief Delay operations.
 * @param[in] ms Number of milliseconds the processor waits
 */
void exec_delay(int ms);

/**
 * @brief Update the current destination floor.
 */
void exec_update_destination_floor();

/**
 * @brief Set the last floor the elevator was at.
 * @param[in] floor Last floor
 */
void exec_set_last_floor(floor_codes_t floor);

/**
 * @brief Clears all order lights for a given floor. Both outside and inside.
 * @param[in] floor Floor to clear lights at
 */
void exec_clear_all_order_lights_at_floor(floor_codes_t floor);

/**
 * @brief Set the last direction the elevator was moving.
 * @param[in] direction Last direction
 */
void exec_set_last_direction(elev_motor_direction_t direction);

/**
 * @brief Checks all order buttons and updates inside and outside. 
 * queue accordingly.
 */
void exec_check_order_buttons();

/**
 * @brief Checks if the elevator should stop at a floor.
 * @return 1 if elevator should stop, 0 if not
 */
int exec_should_stop_at_floor(floor_codes_t current_floor);


/**
 * @brief Gets the return code for the idle state.
 * @return Returns a return code of type @c return_codes_t  
 */
return_codes_t exec_get_idle_return_code();

/**
 * @brief Gets the return code for the floor_stationary state .
 * @param[in] current_floor Floor the elevator is currently at
 * @return Returns a return code of type @c return_codes_t
 */
return_codes_t exec_get_floor_return_code(floor_codes_t current_floor);

/**
 * @brief Opens the door for 3 seconds.
 * @param[in] current_floor Floor the elevator is currently at
 * @return Returns @c stop if the stop button is pressed while the door is open 
 * @c hold if not
 */
return_codes_t exec_open_door_3_sec(floor_codes_t current_floor);

#endif /* EXEC_H */ 