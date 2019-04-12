/**
 * @file
 * @brief A library for executing tasks for the elevator.
 */

#ifndef EXEC_H
#define EXEC_H

///////////////////////////////////////////////
// Includes
///////////////////////////////////////////////

#include "order.h"

///////////////////////////////////////////////
// Enums
///////////////////////////////////////////////

typedef enum return_codes {
    drive_up,
    drive_down,
    hold,
    stay,
    idle,
    stop,
    fail
} return_codes_t;

///////////////////////////////////////////////
// Function declarations
///////////////////////////////////////////////

/**
 * @brief Delay operations for @c ms milliseconds.
 * @param[in] ms Number of milliseconds the processor waits
 */
void exec_delay(int ms);

/**
 * @brief Update the current destination floor found in the private 
 * variable @c destination_floor. Choose the 
 * first element of the inside queue. If the inside queue is empty, choose 
 * the first element of the outside queue. If both queues are 
 * empty @c destination_floor is set to @c between_floors to symbolize 
 * that the elevator has no orders to handle.
 */
void exec_update_destination_floor(void);

/**
 * @brief Update the private variable @c last_floor with the last floor 
 * the elevator was at.
 * @param[in] floor Last floor
 */
void exec_set_last_floor(floor_codes_t floor);

/**
 * @brief Turn off all order lamps for a given floor. Both outside and inside 
 * order lamps.
 * @param[in] floor Floor to turn off lamps at
 */
void exec_clear_all_order_lights_at_floor(floor_codes_t floor);

/**
 * @brief Update the private variable @c last_direction to the 
 * last direction the elevator was moving in.
 * @param[in] direction Last direction
 */
void exec_set_last_direction(elev_motor_direction_t direction);

/**
 * @brief Check all order buttons and update inside and outside 
 * queues accordingly. Turn on correct order lamp if an order was made.
 */
void exec_check_order_buttons(void);

/**
 * @brief Check if the elevator should stop at a floor. Search 
 * both queues for orders. If orders found check if the floor (and 
 * direction) of the order matches @c current_floor (and 
 * @c last_direction).
 * @param[in] current_floor The current floor of the elevator
 * @return 1 if elevator should stop, 0 if not
 */
int exec_should_stop_at_floor(floor_codes_t current_floor);

/**
 * @brief Fetch the return code for the idle state. Algorithm for finding  
 * the return code is based on the private variables @c destination_floor, 
 * @c last_floor and @c last_direction.
 * @return Returns the return code for idle state 
 */
return_codes_t exec_get_idle_return_code(void);

/**
 * @brief Fetch the return code for the floor_stationary state. 
 * The algorithm for finding the return code is based on the private 
 * variable @c destination_floor and the parameter @c current_floor.
 * @param[in] current_floor Floor the elevator is currently at
 * @return Returns a return code of type @c return_codes_t
 */
return_codes_t exec_get_floor_return_code(floor_codes_t current_floor);

/**
 * @brief Open the door for 3 seconds. Check for new orders while door is open 
 * and remove orders made for the current floor. Reset timer 
 * if a new order is made for the current floor while the door is open. 
 * Check if stop button is pressed while the door is open.
 *  
 * @param[in] current_floor Floor the elevator is currently at
 * @return Returns @c stop if the stop button is pressed while the door is open 
 * @c hold if not
 */
return_codes_t exec_open_door_3_sec(floor_codes_t current_floor);

#endif /* EXEC_H */