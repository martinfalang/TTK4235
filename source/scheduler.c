
#include "scheduler.h"
#include "elev.h"

#include <stdio.h> //for printf()

//Purpose: This function inserts arg floor at the end of queue of arg queue
void scheduler_insert_inside_order(inside_queue_t *queue, int floor) {
    inside_order_t order;
    for (int i = 0; i < queue->length; i++) {
        if (queue->queue[i].floor == floor) {
            printf("Error: scheduler_insert_inside(): floor already exists in queue\n");
            return;
        }
    }
    if (queue->length == MAX_INSIDE_ORDERS) {
        printf("Error: scheduler_insert_inside(): queue full\n");
        return;
    }
    else if (floor < 0 || floor > N_FLOORS -1) {
        printf("Error: scheduler_insert_inside(): floor out of range\n");
        return;
    }
    else {
        order.floor = floor;
        queue->queue[queue->rear] = order;
        queue->rear++;
        queue->length++;
    }
}

//Purpose: This function deletes arg floor from arg queue
void scheduler_delete_inside_order(inside_queue_t *queue, int floor) {
    if (floor > 3 || floor < 0) {
        printf("Error: delete_inside_at_index(): floor out of range\n");
        return;
    }
    else if (queue->length == 0) {
        printf("Error: delete_inside_at_index(): Queue is empty\n");
        return;
    }
    else {
        int i;
        int index = -1;
        for (i = 0; i < queue->length; i++) {
            if (queue->queue[i].floor == floor) {
                index = i;
            }
        }
        if (index == -1) {
            printf("Error: scheduler_delete_inside_order(): Element does not exist\n");
            return;
        }
        for (i = index; i < MAX_INSIDE_ORDERS; i++) {
            queue->queue[i] = queue->queue[i+1];
        }
        queue->length--;
        queue->rear--;
    }
}

//Purpose: This function prints arg queue to screen
void scheduler_display_inside_queue(inside_queue_t *queue) {
    int i;
    if (!queue->length) {
        printf("Error: display_inside(): Inside queue is empty\n");
    }
    else {
        printf("------------------------------\n");
        printf("Inside queue:\n");
        printf("Length: %d\n", queue->length);
        printf("Rear: %d\n", queue->rear);
        for (i = 0; i < queue->length; i++) {
            printf("[%d] Floor: %d\n", i, queue->queue[i].floor);
        }
        printf("------------------------------\n");
    }
}

//Purpose: This function inserts an order with arg floor and arg direction to the en of arg queue
void scheduler_insert_outside_order(outside_queue_t *queue, int floor, int direction) {
    outside_order_t order;

    for (int i = 0; i < queue->length; i++) {
        if (queue->queue[i].floor == floor && queue->queue[i].direction == direction) {
            printf("Error: scheduler_insert_outside_order(): order already exists in queue\n");
            return;
        } 
    }

    if (queue->rear == MAX_OUTSIDE_ORDERS) {
        printf("Error: scheduler_insert_outside_order(): Queue full\n");
        return;
    }
    else if (floor < 0 || floor > N_FLOORS -1) {
        printf("Error: scheduler_insert_outside_order(): floor out of range\n");
        return;
    }
    else if (direction != -1 && direction != 1) {
        printf("Error: scheduler_insert_outside_order(): direction not -1 or 1\n");
        return;
    }
    else if ((floor == 3 && direction == 1) || (floor == 0 && direction == -1)) {
        printf("Error: insert_outside_order(): Can't go up from floor 4 or down from floor 1\n");
        return;
    } 
    else {
        order.floor = floor;
        order.direction = direction;
        queue->queue[queue->rear] = order;
        queue->rear++;
        queue->length++;
    }
}

//Purpose: This function deletes order with arg floor and arg direction from arg queue
void scheduler_delete_outside_order(outside_queue_t *queue, int floor, int direction) {
    if (floor > 3 || floor < 0) {
        printf("Error: delete_outside_order(): floor out of range\n");
        return;
    }
    else if (direction != -1 && direction != 1) {
        printf("Error: delete_outside_order(): direction not -1 or 1\n");
        return;
    }
    else if (queue->length == 0) {
        printf("Error: delete_outside_order(): Queue is empty");
        return;
    }
    else {
        int i;
        int index = -1;
        for (i = 0; i < queue->length; i++) {
            if (queue->queue[i].floor == floor && queue->queue[i].direction == direction) {
                index = i;
            }
        }
        if (index == -1) {
            printf("Error: scheduler_delete_outside_floor(): Element does not exist\n");
            return;
        }
        for (i = index; i < MAX_OUTSIDE_ORDERS; i++) {
            queue->queue[i] = queue->queue[i+1];
        }
        queue->length--;
        queue->rear--;
    }
}

//Purpose: This function prints arg queue to screen
void scheduler_display_outside_queue(outside_queue_t *queue) {
    int i;
    if (!queue->length) {
        printf("Error: display_outside(): Outside queue is empty\n");
    }
    else {
        printf("------------------------------\n");
        printf("Outside queue:\n");
        printf("Length: %d\n", queue->length);
        printf("Rear: %d\n", queue->rear);
        char *dir;

        for (i = 0; i < queue->length; i++) {
            if (queue->queue[i].direction == -1) {
                dir = "DOWN";
            }
            else if (queue->queue[i].direction == 1) {
                dir = "UP";
            }
            printf("[%d] Floor: %d Dir: %s\n", i, queue->queue[i].floor, dir);
        }
        printf("------------------------------\n");
    }
}
