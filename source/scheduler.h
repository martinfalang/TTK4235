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

#include "exec.h"
#include "elev.h"

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

void scheduler_insert_inside_order(inside_queue_t *queue, floor_codes_t floor);
void scheduler_delete_inside_order(inside_queue_t *queue, floor_codes_t floor);
void scheduler_display_inside_queue(inside_queue_t *queue);

void scheduler_insert_outside_order(outside_queue_t *queue, floor_codes_t floor, elev_motor_direction_t direction);
void scheduler_delete_outside_order(outside_queue_t *queue, floor_codes_t floor, elev_motor_direction_t direction);
void scheduler_display_outside_queue(outside_queue_t *queue);

#endif /*SCHEDULER_H*/