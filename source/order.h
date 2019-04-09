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

int order_add_order_to_queue(floor_codes_t floor, elev_motor_direction_t direction);
void order_remove_all_orders_at_floor(floor_codes_t floor);
void order_remove_all_orders();

void order_print_orders(void);

#endif /* ORDER_H */