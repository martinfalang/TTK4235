#ifndef ORDER_H
#define ORDER_H

///////////////////////////////////////////////
/*
    TOP LEVEL UPDATES:
        - Deleted old order_init function as this
            function is not used anymore
        - Removed old defines for order_1_up etc
        - Updated order_add to use elev_motor_direction
            instead of direction_codes_t
        - Deleted old order_update_floor_lights
            function as this is now done in exex.c
        - Deleted order_check_inside_orders function
            as this function is not used anymore
        - Changed order_remove to void function
        - Added comment after endif because it's
            good practice 
        - Changed name of order_remove function to 
            order_remove_all_orders_at_floor
        - Changed name to order_remove_all function
            to order_remove_all_orders
        - Changed name of order_add function to
            order_add_order_to_queue
*/
///////////////////////////////////////////////

///////////////////////////////////////////////
// Includes
///////////////////////////////////////////////

#include "scheduler.h"
#include "fsm.h"

///////////////////////////////////////////////
// Function declarations
///////////////////////////////////////////////

inside_queue_t* order_get_inside_queue(void);
outside_queue_t* order_get_outside_queue(void);

int order_add_order_to_queue(floor_codes_t floor, elev_motor_direction_t direction);
void order_remove_all_orders_at_floor(floor_codes_t floor);
void order_remove_all_orders();

void order_print_orders(void);

#endif /* ORDER_H */