#include "order.h"
#include "elev.h"

//Array of orders, 1 means order of type on, 0 means no order of given type.
static int order_array[10] = {0,0,0,0,0,0,0,0,0,0};

int order_init(void) {
    for (int i = 0; i < sizeof(order_array)/sizeof(int); i++) {
        order_array[i] = 0;
    }
    return 0;
}

int order_add(int type){
    if (type <= ORDER_ARRAY_LENGTH && type >= 0) {
        order_array[type] = 1;
        order_update_floor_lights(type, 1);
        return 0;
    }
    else {
        return -1;
    }
}

int order_remove(int type) {
    if (type <= ORDER_ARRAY_LENGTH && type >= 0) {
        order_array[type] = 0;
        order_update_floor_lights(type, 0);
        return 0;
    }
    else {
        return -1;
    }
}

int *order_get_orders() {
    return order_array;
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

void testorder(void){
    //Add order command floor 3 and floor 2 up
    order_add(inside_3);
    order_add(outside_2_up); 
    
}