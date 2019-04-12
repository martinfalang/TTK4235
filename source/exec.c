
///////////////////////////////////////////////
// Includes
///////////////////////////////////////////////

#include "exec.h"
#include "time.h"

#include <stdio.h>

///////////////////////////////////////////////
// Private variables
///////////////////////////////////////////////

static floor_codes_t last_floor;
static elev_motor_direction_t last_direction;

static floor_codes_t destination_floor;

///////////////////////////////////////////////
// Functions
///////////////////////////////////////////////

return_codes_t exec_open_door_3_sec(floor_codes_t current_floor)
{
    elev_set_door_open_lamp(1);

    // Delay 10 ms 300 times in order to delay a total of 3 seconds
    for (int i = 0; i < 300; i++)
    {
        exec_delay(10);
        exec_check_order_buttons();

        exec_clear_all_order_lights_at_floor(current_floor);

        // Reset the timer if a new order for this floor was added
        // while the door was open
        if (exec_should_stop_at_floor(current_floor))
        {
            i = 0;
        }

        order_remove_all_orders_at_floor(current_floor);

        // Interrupt timer if the stop button was pressed while
        // the door was open
        if (elev_get_stop_signal())
        {
            return stop;
        }
    }

    elev_set_door_open_lamp(0);
    return hold;
}

void exec_update_destination_floor()
{
    inside_queue_t *inside_queue = order_get_inside_queue();
    outside_queue_t *outside_queue = order_get_outside_queue();

    // Check if there are elements in queues and find first element
    if (inside_queue->length)
        destination_floor = inside_queue->queue[0].floor;
    else if (outside_queue->length)
        destination_floor = outside_queue->queue[0].floor;
    else
        destination_floor = between_floors;
}

void exec_delay(int ms)
{
    struct timespec req;

    req.tv_sec = ms / 1000;
    req.tv_nsec = (ms % 1000) * 1000000;

    nanosleep(&req, 0);
}

void exec_set_last_floor(floor_codes_t floor)
{
    last_floor = floor;
}

void exec_clear_all_order_lights_at_floor(floor_codes_t floor)
{
    switch (floor)
    {
    case floor_1:
    {
        elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
        elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
        break;
    }
    case floor_4:
    {
        elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
        elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0);
        break;
    }
    default:
    {
        elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
        elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
        elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0);
        break;
    }
    }
}

void exec_set_last_direction(elev_motor_direction_t direction)
{
    last_direction = direction;
}

void exec_check_order_buttons()
{
    // Loops through all floors and all order types (inside order, outside up or outside down order)
    for (int floor = 0; floor < 4; floor++)
    {
        for (int type = 0; type < 3; type++)
        {
            // Skip if order is outside down at floor one or outside up at floor 4
            if ((type == 1 && floor == 0) || (type == 0 && floor == 3))
            {
                continue;
            }
            // Here the correct order is added and lamp turned on if a certain button is pressed
            else if (elev_get_button_signal(type, floor))
            {
                if (floor == 0 && type == 0)
                {
                    order_add_order_to_queue(floor_1, DIRN_UP);
                    elev_set_button_lamp(BUTTON_CALL_UP, floor, 1);
                }
                else if (floor == 0 && type == 2)
                {
                    order_add_order_to_queue(floor_1, DIRN_STOP);
                    elev_set_button_lamp(BUTTON_COMMAND, floor, 1);
                }
                else if (floor == 1 && type == 0)
                {
                    order_add_order_to_queue(floor_2, DIRN_UP);
                    elev_set_button_lamp(BUTTON_CALL_UP, floor, 1);
                }
                else if (floor == 1 && type == 1)
                {
                    order_add_order_to_queue(floor_2, DIRN_DOWN);
                    elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 1);
                }
                else if (floor == 1 && type == 2)
                {
                    order_add_order_to_queue(floor_2, DIRN_STOP);
                    elev_set_button_lamp(BUTTON_COMMAND, floor, 1);
                }
                else if (floor == 2 && type == 0)
                {
                    order_add_order_to_queue(floor_3, DIRN_UP);
                    elev_set_button_lamp(BUTTON_CALL_UP, floor, 1);
                }
                else if (floor == 2 && type == 1)
                {
                    order_add_order_to_queue(floor_3, DIRN_DOWN);
                    elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 1);
                }
                else if (floor == 2 && type == 2)
                {
                    order_add_order_to_queue(floor_3, DIRN_STOP);
                    elev_set_button_lamp(BUTTON_COMMAND, floor, 1);
                }
                else if (floor == 3 && type == 1)
                {
                    order_add_order_to_queue(floor_4, DIRN_DOWN);
                    elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 1);
                }
                else if (floor == 3 && type == 2)
                {
                    order_add_order_to_queue(floor_4, DIRN_STOP);
                    elev_set_button_lamp(BUTTON_COMMAND, floor, 1);
                }
            }
        }
    }
}

int exec_should_stop_at_floor(floor_codes_t current_floor)
{

    inside_queue_t *inside_queue_ptr = order_get_inside_queue();
    outside_queue_t *outside_queue_ptr = order_get_outside_queue();

    // Check inside orders
    if (inside_queue_ptr->length)
    {
        // Check all orders in the inside queue and return true if
        // an order matches the current floor
        for (int i = 0; i < inside_queue_ptr->length; i++)
        {
            if (inside_queue_ptr->queue[i].floor == current_floor)
            {
                return 1;
            }
        }
    }
    // Check outside orders
    if (outside_queue_ptr->length)
    {
        for (int i = 0; i < outside_queue_ptr->length; i++)
        {
            // Special case for floor 1 and 4
            if (current_floor == floor_1 || current_floor == floor_4)
            {
                if (outside_queue_ptr->queue[i].floor == current_floor)
                {
                    return 1;
                }
            }
            // Check if elevator is not moving and there is an outside order for the current floor
            else if (outside_queue_ptr->queue[i].floor == current_floor && last_direction == DIRN_STOP)
            {
                return 1;
            }
            // Check if the elevator is moving, but there is an order to stop for on the way
            else if (outside_queue_ptr->queue[i].floor == current_floor && outside_queue_ptr->queue[i].direction == last_direction)
            {
                return 1;
            }
            // Check if the current floor is also the destination floor of the elevator
            else if (outside_queue_ptr->queue[i].floor == current_floor && current_floor == destination_floor)
            {
                return 1;
            }
        }
    }
    // If nothing found, return 0
    return 0;
}

return_codes_t exec_get_idle_return_code()
{
    switch (destination_floor)
    {
    // If there is no destination floor the elevator should stay in idle state
    case between_floors:
    {
        return hold;
        break;
    }
    // If destination floor is floor 1 then the elevator should always move down
    case floor_1:
    {
        return drive_down;
        break;
    }
    // Check if the elevator is below or above floor 2
    case floor_2:
    {
        if (last_floor < floor_2 || (last_floor == floor_2 && last_direction == DIRN_DOWN))
        {
            return drive_up;
        }
        else
        {
            return drive_down;
        }
        break;
    }
    // Check if the elevator is below or above floor 3
    case floor_3:
    {
        if (last_floor > floor_3 || (last_floor == floor_3 && last_direction == DIRN_UP))
        {
            return drive_down;
        }
        else
        {
            return drive_up;
        }
        break;
    }
    // If destination floor is floor 4 then the elevator should always move up
    case floor_4:
    {
        return drive_up;
        break;
    }
    // If something fails, return fail to take elevator to the end state
    default:
    {
        return fail;
        break;
    }
    }
}

return_codes_t exec_get_floor_return_code(floor_codes_t current_floor)
{
    // If there is no destination floor the elevator should stay in floor stationary state
    if (destination_floor == between_floors)
    {
        return hold;
    }
    // If elevator is not at the destination floor, find out if it should move up or down
    else if (current_floor != destination_floor)
    {
        if (current_floor < destination_floor)
        {
            return drive_up;
        }
        else
        {
            return drive_down;
        }
    }
    // If something fails, return fail to take elevator to the end state
    else
    {
        return fail;
    }
}
