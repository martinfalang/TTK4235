/**
 * @file
 * @brief A library for order and queue tasks specific to this elevator.
 */

#ifndef ORDER_H
#define ORDER_H

///////////////////////////////////////////////
// Includes
///////////////////////////////////////////////

#include "scheduler.h"

///////////////////////////////////////////////
// Function declarations
///////////////////////////////////////////////

/**
 * @brief Fetch the inside queue.
 * @return Return pointer to inside queue.
 */
inside_queue_t *order_get_inside_queue(void);

/**
 * @brief Fetch the outside queue.
 * @return Return pointer to outside queue.
 */
outside_queue_t *order_get_outside_queue(void);

/**
 * @brief Add an order to a queue. Parameter @c direction specifies if the 
 * order is for the inside or outside queue. @c DIRN_STOP specifies an inside
 * order. 
 * 
 * @param[in] floor Floor Code
 * @param[in] direction Direction Code
 * 
 * @return 0 if order was successfully added to one of the queues, -1 if not.
 */
int order_add_order_to_queue(floor_codes_t floor, elev_motor_direction_t direction);

/**
 * @brief Remove all orders on a given floor from both inside and outside queue.
 * 
 * @param[in] floor The floor to delete all orders from
 * 
 */
void order_remove_all_orders_at_floor(floor_codes_t floor);

/**
 * @brief Remove all orders on all floors from both inside and outside queue.
 * 
 */
void order_remove_all_orders(void);

/**
 * @brief Print both inside and outside queue in a nice and readable format
 * 
 */
void order_print_orders(void);

#endif /* ORDER_H */