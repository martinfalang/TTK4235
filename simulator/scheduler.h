#ifndef SCHEDULER_H
#define SCHEDULER_H

#define MAX_INSIDE_ORDERS 4
#define MAX_OUTSIDE_ORDERS 6

typedef struct inside_order {
    int floor;
} inside_order_t;

typedef struct outside_order {
    int floor;
    int direction;
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

static inside_queue_t inside_queue;
static outside_queue_t outside_queue;

void scheduler_insert_inside_order(inside_queue_t *queue, int floor);
void scheduler_delete_inside_order(inside_queue_t *queue, int floor);
void scheduler_display_inside_queue(inside_queue_t *queue);

void scheduler_insert_outside_order(outside_queue_t *queue, int floor, int direction);
void scheduler_delete_outside_order(outside_queue_t *queue, int floor, int direction);
void scheduler_display_outside_queue(outside_queue_t *queue);

#endif