#ifndef ORDER_H
#define ORDER_H

#define ORDER_ARRAY_LENGTH 10

#define inside_1 0
#define inside_2 1
#define inside_3 2
#define inside_4 3
#define outside_1_up 4
#define outside_2_down 5
#define outside_2_up 6
#define outside_3_down 7
#define outside_3_up 8
#define outside_4_down 9

int order_init(void);

int order_add(int type);

int order_remove(int type);

int order_update_floor_lights(int type, int value);

int *order_get_orders();

void testorder(void);










#endif