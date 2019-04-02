#ifndef ORDER_H
#define ORDER_H

#include "scheduler.h"

#define ORDER_ARRAY_LENGTH 10


#define outside_1_up 0
#define inside_1 1
#define outside_2_up 2
#define outside_2_down 3
#define inside_2 4
#define outside_3_up 5
#define outside_3_down 6
#define inside_3 7
#define outside_4_down 8
#define inside_4 9

int order_init(void);

int order_add(int type);

int order_remove(int type);

int order_update_floor_lights(int type, int value);

inside_queue_t* order_get_inside_queue(void);
outside_queue_t* order_get_outside_queue(void);

int order_clear_all(void);

void order_print_orders(void);

int order_check_inside_orders();

//Purpose: Returns 1 if any order in inside_queue for floor
int order_find_inside(int floor);









#endif