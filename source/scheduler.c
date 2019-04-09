
///////////////////////////////////////////////
// Includes
///////////////////////////////////////////////

#include "scheduler.h"
#include "elev.h"

#include <stdio.h> //for printf()

///////////////////////////////////////////////
// Functions
///////////////////////////////////////////////

//Purpose: This function inserts arg floor at the end of queue of arg queue
void scheduler_insert_inside_order(inside_queue_t *queue, floor_codes_t floor) {
    inside_order_t order;

    if (floor < 0 || floor > N_FLOORS -1) {
        //Debug purposes
        //printf("Error: scheduler_insert_inside(): floor out of range\n");
        return;
    }
    for (int i = 0; i < queue->length; i++) {
        if (queue->queue[i].floor == floor) {
            //Debug purposes
            //printf("Error: scheduler_insert_inside(): floor already exists in queue\n");
            return;
        }
    }
    order.floor = floor;
    queue->queue[queue->rear] = order;
    queue->rear++;
    queue->length++;
}

//Purpose: This function deletes arg floor from arg queue
void scheduler_delete_inside_order(inside_queue_t *queue, floor_codes_t floor) {
    if (queue->length == 0) {
        //Debug purposes
        //printf("Error: delete_inside_at_index(): Queue is empty\n");
        return;
    }
    else if (floor > 3 || floor < 0) {
        //Debug purposes
        //printf("Error: delete_inside_at_index(): floor out of range\n");
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
            //Debug purposes
            //printf("Error: scheduler_delete_inside_order(): Element does not exist\n");
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
        //Debug purposes
        //printf("Error: display_inside(): Inside queue is empty\n");
        return;
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
void scheduler_insert_outside_order(outside_queue_t *queue, floor_codes_t floor, elev_motor_direction_t direction) {
    outside_order_t order;

    if (floor < 0 || floor > N_FLOORS - 1) {
        //Debug purposes
        //printf("Error: scheduler_insert_outside_order(): floor out of range\n");
        return;
    }
    else if (direction != DIRN_DOWN && direction != DIRN_UP) {
        //Debug purposes
        //printf("Error: scheduler_insert_outside_order(): direction not -1 or 1\n");
        return;
    }
    else if ((floor == 3 && direction == DIRN_UP) || (floor == 0 && direction == DIRN_DOWN)) {
        //Debug purposes
        //printf("Error: insert_outside_order(): Can't go up from floor 4 or down from floor 1\n");
        return;
    } 

    for (int i = 0; i < queue->length; i++) {
        if (queue->queue[i].floor == floor && queue->queue[i].direction == direction) {
            //Debug purposes
            //printf("Error: scheduler_insert_outside_order(): order already exists in queue\n");
            return;
        } 
    }
    
    order.floor = floor;
    order.direction = direction;
    queue->queue[queue->rear] = order;
    queue->rear++;
    queue->length++;
    
}

//Purpose: This function deletes order with arg floor and arg direction from arg queue
void scheduler_delete_outside_order(outside_queue_t *queue, floor_codes_t floor, elev_motor_direction_t direction) {
    if (queue->length == 0) {
        //Debug purposes 
        //printf("Error: delete_outside_order(): Queue is empty\n");
        return;
    }
    else if (floor > 3 || floor < 0) {
        //Debug purposes
        //printf("Error: delete_outside_order(): floor out of range\n");
        return;
    }
    else if (direction != DIRN_DOWN && direction != DIRN_UP) {
        //Debug purposes
        //printf("Error: delete_outside_order(): direction not -1 or 1\n");
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
            //Debug purposes
            //printf("Error: scheduler_delete_outside_floor(): Element does not exist\n");
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
        //Debug purposes
        //printf("Error: display_outside(): Outside queue is empty\n");
        return;
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
