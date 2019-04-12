
///////////////////////////////////////////////
// Includes
///////////////////////////////////////////////

#include "order.h"

///////////////////////////////////////////////
// Private variables
///////////////////////////////////////////////

static inside_queue_t inside_queue = {.length = 0, .rear = 0};
static outside_queue_t outside_queue = {.length = 0, .rear = 0};

///////////////////////////////////////////////
// Functions
///////////////////////////////////////////////

inside_queue_t *order_get_inside_queue(void)
{
    return &inside_queue;
}

outside_queue_t *order_get_outside_queue(void)
{
    return &outside_queue;
}

int order_add_order_to_queue(floor_codes_t floor, elev_motor_direction_t direction)
{
    // If order has direction DIRN_STOP it means the order was made inside the elevator
    if (direction == DIRN_STOP)
    {
        scheduler_insert_inside_order(&inside_queue, floor);
        return 0;
    }
    else if (direction == DIRN_UP || direction == DIRN_DOWN)
    {
        scheduler_insert_outside_order(&outside_queue, floor, direction);
        return 0;
    }
    else
    {
        return -1;
    }
}

void order_remove_all_orders_at_floor(floor_codes_t floor)
{
    scheduler_delete_outside_order(&outside_queue, floor, DIRN_UP);
    scheduler_delete_outside_order(&outside_queue, floor, DIRN_DOWN);
    scheduler_delete_inside_order(&inside_queue, floor);
}

void order_remove_all_orders(void)
{
    for (floor_codes_t floor = floor_1; floor <= floor_4; floor++)
    {
        order_remove_all_orders_at_floor(floor);
    }
}

void order_print_orders(void)
{
    scheduler_display_inside_queue(&inside_queue);
    scheduler_display_outside_queue(&outside_queue);
}