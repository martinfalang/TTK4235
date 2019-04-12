/**
 * @file
 * @brief A library for doing operations on queues.
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

///////////////////////////////////////////////
// Defines
///////////////////////////////////////////////

#define MAX_INSIDE_ORDERS 4
#define MAX_OUTSIDE_ORDERS 6

///////////////////////////////////////////////
// Includes
///////////////////////////////////////////////

#include "elev.h"

///////////////////////////////////////////////
// Enums
///////////////////////////////////////////////

typedef enum floor_codes {
    floor_1,
    floor_2,
    floor_3,
    floor_4,
    between_floors = -1
} floor_codes_t;

///////////////////////////////////////////////
// Structures
///////////////////////////////////////////////

typedef struct inside_order {
    floor_codes_t floor;
} inside_order_t;

typedef struct outside_order {
    floor_codes_t floor;
    elev_motor_direction_t direction;
} outside_order_t;

typedef struct inside_order_queue {
    int length;
    int rear;
    inside_order_t queue[MAX_INSIDE_ORDERS];
} inside_queue_t;

typedef struct outside_order_queue {
    int length;
    int rear;
    outside_order_t queue[MAX_OUTSIDE_ORDERS];
} outside_queue_t;

///////////////////////////////////////////////
// Function declarations
///////////////////////////////////////////////

/**
 * @brief Add an order for a floor to the back of an inside queue. Ignores  
 * orders already present and orders outside the floor range.
 * 
 * @param[out] queue Buffer queue
 * @param[in] floor Order floor
 */
void scheduler_insert_inside_order(inside_queue_t *queue, floor_codes_t floor);

/**
 * @brief Delete an order for a floor from an inside queue. The queue is unchanged 
 * if the order is not found in the queue. Ignores orders already present 
 * and orders outside the floor range.
 * 
 * @param[out] queue Buffer queue
 * @param[in] floor Order floor
 */
void scheduler_delete_inside_order(inside_queue_t *queue, floor_codes_t floor);

/**
 * @brief Prints an inside queue to the sqreen.
 * 
 * @param[in] queue Queue to print 
 */
void scheduler_display_inside_queue(inside_queue_t *queue);

/**
 * @brief Add an order for a floor and a direction to the back of an outside queue. Ignores 
 * orders already present and orders outside the floor or direction range.
 * 
 * @param[out] queue Buffer queue
 * @param[in] floor Order floor 
 * @param[in] direction Order direction
 */
void scheduler_insert_outside_order(outside_queue_t *queue, floor_codes_t floor, elev_motor_direction_t direction);

/**
 * @brief Delete an order for a floor and a direction from an outside queue. The queue is unchanged 
 * if the order is not found in the queue. Ignores orders already present and orders 
 * outside the floor range.
 * 
 * @param[out] queue Buffer queue
 * @param[in] floor Order Floor
 * @param[in] direction Order Direction
 */
void scheduler_delete_outside_order(outside_queue_t *queue, floor_codes_t floor, elev_motor_direction_t direction);

/**
 * @brief Prints an outside queue for orders made outside the elevator.
 * 
 * @param[in] queue Queue to print 
 */
void scheduler_display_outside_queue(outside_queue_t *queue);

#endif /*SCHEDULER_H*/