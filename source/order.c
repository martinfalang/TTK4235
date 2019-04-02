#include "order.h"
#include "elev.h"
#include "scheduler.h"

#include <stdio.h>

//Array of orders, 1 means order of type on, 0 means no order of given type.


/*
int order_init(void) {
    for (int i = 0; i < sizeof(order_array)/sizeof(int); i++) {
        order_array[i] = 0;
    }
    return 0;
}
*/

int order_add(int type){
    int floor=0;
    int direction=0;
    switch(type)
    {
        case inside_1:
        {
            floor=0;
            direction=0;
            scheduler_insert_inside(&inside_queue,floor);
            return 0;
        }
        case inside_2:
        {
            floor=1;
            direction=0;
            scheduler_insert_inside(&inside_queue,floor);
            return 0;
        }
        case inside_3:
        {
            floor=2;
            direction=0;
            scheduler_insert_inside(&inside_queue,floor);
            return 0;
        }
        case inside_4:
        {
            floor=3;
            direction=0;
            scheduler_insert_inside(&inside_queue,floor);
            return 0;
        }
        case outside_1_up:
        {
            floor=0;
            direction=1;
            scheduler_insert_outside(&outside_queue,floor,direction);
            return 0;
        }
        case outside_2_up:
        {
            floor=1;
            direction=1;
            scheduler_insert_outside(&outside_queue,floor,direction);
            return 0;
        }
        case outside_3_up:
        {
            floor=2;
            direction=1;
            scheduler_insert_outside(&outside_queue,floor,direction);
            return 0;
        }
        case outside_2_down:
        {
            floor=1;
            direction=-1;
            scheduler_insert_outside(&outside_queue,floor,direction);
            return 0;
        }
        case outside_3_down:
        {
            floor=2;
            direction=-1;
            scheduler_insert_outside(&outside_queue,floor,direction);
            return 0;
        }
        case outside_4_down:
        {
            floor=3;
            direction=-1;
            scheduler_insert_outside(&outside_queue, floor, direction);
            return 0;
        }
        default:
        {
            return -1;
        }

    }
}

int order_remove(int type) {
    int floor=0;
    int direction=0;
    switch(type)
    {
        case inside_1:
        {
            floor=0;
            direction=0;
            scheduler_delete_inside_order(&inside_queue,floor);
            return 0;
        }
        case inside_2:
        {
            floor=1;
            direction=0;
            scheduler_delete_inside_order(&inside_queue,floor);
            return 0;
        }
        case inside_3:
        {
            floor=2;
            direction=0;
            scheduler_delete_inside_order(&inside_queue,floor);
            return 0;
        }
        case inside_4:
        {
            floor=3;
            direction=0;
            scheduler_delete_inside_order(&inside_queue,floor);
            return 0;
        }
        case outside_1_up:
        {
            floor=0;
            direction=1;
            scheduler_delete_outside_order(&outside_queue,floor,direction);
            return 0;
        }
        case outside_2_up:
        {
            floor=1;
            direction=1;
            scheduler_delete_outside_order(&outside_queue,floor,direction);
            return 0;
        }
        case outside_3_up:
        {
            floor=2;
            direction=1;
            scheduler_delete_outside_order(&outside_queue,floor,direction);
            return 0;
        }
        case outside_2_down:
        {
            floor=1;
            direction=-1;
            scheduler_delete_outside_order(&outside_queue,floor,direction);
            return 0;
        }
        case outside_3_down:
        {
            floor=2;
            direction=-1;
            scheduler_delete_outside_order(&outside_queue,floor,direction);
            return 0;
        }
        case outside_4_down:
        {
            floor=3;
            direction=-1;
            scheduler_delete_outside_order(&outside_queue, floor, direction);
            return 0;
        }
        default:
        {
            return -1;
        }

    }
}

inside_queue_t* order_get_inside_queue(void)
{
    return &inside_queue;
}

outside_queue_t* order_get_outside_queue(void)
{
    return &outside_queue;
}



int order_clear_all(void) {
    /*Insert clear_all function from scheduler when ready*/
}

void order_print_orders(void) {
    scheduler_display_inside_queue(&inside_queue);
    scheduler_display_outside_queue(&outside_queue);
}

int order_update_floor_lights(int type, int value) {
    switch (type)
    {
        case inside_1: 
        {
            elev_set_button_lamp(BUTTON_COMMAND,0,value);
            break;
        }
        case inside_2:
        {
            elev_set_button_lamp(BUTTON_COMMAND,1,value);
            break;
        }
        case inside_3:
        {
            elev_set_button_lamp(BUTTON_COMMAND,2,value);
            break;
        }
        case inside_4:
        {
            elev_set_button_lamp(BUTTON_COMMAND,3,value);
            break;
        }
        case outside_1_up:
        {
            elev_set_button_lamp(BUTTON_CALL_UP,0,value);
            break;
        }
        case outside_2_down:
        {
            elev_set_button_lamp(BUTTON_CALL_DOWN,1,value);
            break;
        }
        case outside_2_up:
        {
            elev_set_button_lamp(BUTTON_CALL_UP,1,value);
            break;
        }
        case outside_3_down:
        {
            elev_set_button_lamp(BUTTON_CALL_DOWN,2,value);
            break;
        }
        case outside_3_up:
        {
            elev_set_button_lamp(BUTTON_CALL_UP,2,value);
            break;
        }
        case outside_4_down:
        {
            elev_set_button_lamp(BUTTON_CALL_DOWN,3,value);
            break;
        }
    
        default:
            return -1;
    }
    return 0;
}

int order_find_inside(int floor) {
    
}