
///////////////////////////////////////////////
/*
    TOP LEVEL UPDATES:
        - Deleted old order_add function
        - Deleted old order_remove function
        - Deleted old order_init function
        - Deleted old order_update_floor_lights
            function as this is now done in exec.c
        - Deleted order_find_inside as this function
            is not used and was not implemented
        - Not including <stdio.h> anymore as we
            don't print anything in this file
        - Deleted include "fsm.h" statement as
            this file is already included in order.h
*/
///////////////////////////////////////////////

///////////////////////////////////////////////
// Includes
///////////////////////////////////////////////

#include "order.h"
#include "elev.h"

///////////////////////////////////////////////
// Functions
///////////////////////////////////////////////

///////////////////////////////////////////////
/*
    UPDATES:
        - Added initialization to both queues,
            this is tested and works
*/
///////////////////////////////////////////////
static inside_queue_t inside_queue = {.length = 0, .rear = 0};
static outside_queue_t outside_queue = {.length = 0, .rear = 0};

// No updates made
inside_queue_t* order_get_inside_queue() {
    return &inside_queue;
}

// No updates made
outside_queue_t* order_get_outside_queue() {
    return &outside_queue;
}

///////////////////////////////////////////////
/*
    UPDATES:
        - Updated name to better explain what function does
        - Updated direction to be of type elev_motor_direction_t
        - Updated inside of function with DIRN_STOP, DIRN_UP and DIRN_DOWN
            to match argument type of direction
        - Changed failure return code to -1
*/
///////////////////////////////////////////////
int order_add_order_to_queue(floor_codes_t floor, elev_motor_direction_t direction) {
    if (direction == DIRN_STOP) {
        scheduler_insert_inside_order(&inside_queue, floor);
        return 0;
    }
    else if (direction == DIRN_UP || direction == DIRN_DOWN) {
        scheduler_insert_outside_order(&outside_queue, floor, direction);
        return 0;
    }
    else {
        return -1;
    }
}

///////////////////////////////////////////////
/*
    UPDATES:
        - Updated function name to better explain
            what it does
        - Changed this function to a void function
            as is only return 0 anyways
        - Updated direction argument in scheduler_delete_outside_order
            function to be of type elev_motor_direction_t
*/
///////////////////////////////////////////////
// Since all orders at a floor shall be deleted when the elevator arrives, this function only needs floor as argument
void order_remove_all_orders_at_floor(floor_codes_t floor) {
    scheduler_delete_outside_order(&outside_queue, floor, DIRN_UP);
    scheduler_delete_outside_order(&outside_queue, floor, DIRN_DOWN);
    scheduler_delete_inside_order(&inside_queue, floor);
}

///////////////////////////////////////////////
/*
    UPDATES:
        - Updated direction argument in scheduler_delete_outside_order
            function to be of type elev_motor_direction_t
*/
///////////////////////////////////////////////
void order_remove_all_orders() {
    for (floor_codes_t floor = floor_1; floor <= floor_4; floor++) {
        scheduler_delete_inside_order(&inside_queue, floor);
        scheduler_delete_outside_order(&outside_queue, floor, DIRN_UP);
        scheduler_delete_outside_order(&outside_queue, floor, DIRN_DOWN);
    }
}

// No updates made
void order_print_orders() {
    scheduler_display_inside_queue(&inside_queue);
    scheduler_display_outside_queue(&outside_queue);
}