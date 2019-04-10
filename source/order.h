/**
 * @file
 * @brief A library for adding and deleting orders in the queues and to fetch the queues themselves.
 */

#ifndef ORDER_H
#define ORDER_H

///////////////////////////////////////////////
// Includes
///////////////////////////////////////////////

#include "scheduler.h"
#include "fsm.h"
#include "elev.h"

///////////////////////////////////////////////
// Function declarations
///////////////////////////////////////////////

inside_queue_t* order_get_inside_queue(void);
outside_queue_t* order_get_outside_queue(void);

/**
 * @brief Add order to a queue
 * 
 * @param[in] floor Floor Code
 * @param[in] direction Direction Code
 * 
 * @return 0 if order was successfully added to one of the queues, -1 if not.
 */ 
int order_add_order_to_queue(floor_codes_t floor, elev_motor_direction_t direction);

/**
 * @brief Remove all orders on a given floor from the queues.
 * 
 * @param[in] floor The floor to delete all orders from
 * 
 */
void order_remove_all_orders_at_floor(floor_codes_t floor);

/**
 * @brief Remove all orders on all floors from the queues.
 * 
 */
void order_remove_all_orders();

/**
 * @brief Print both queues in a nice and readable format
 * 
 */
void order_print_orders();

#endif /* ORDER_H */