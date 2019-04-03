
#include "fsm.h"
#include "stdio.h"
#include "elev.h"
#include "time.h"
#include "order.h"
#include "exec.h"

#include <stdlib.h> //for exit()

//static int current_destination;

return_codes_t (*state[])(void) = {
    fsm_floor_stationary_state,
    fsm_initialize_state,
    fsm_driving_up_state,
    fsm_driving_down_state,
    fsm_stop_floor_state,
    fsm_stop_between_state,
    fsm_idle_state,
    fsm_end_state
};

transition_t state_transitions[] = {
    {initialize, drive_down, initialize},
    {initialize, hold, floor_stationary},            //Added this one in testing, better return codes should be implemented
    {initialize, stop_flr, stop_floor},
    {initialize, stop_btw, stop_between},
    {initialize, fail, end},

    {floor_stationary, hold, floor_stationary},
    {floor_stationary, drive_up, driving_up},
    {floor_stationary, drive_down, driving_down},
    {floor_stationary, stop_flr, stop_floor},
    {floor_stationary, fail, end},

    {driving_up, stay, floor_stationary},
    {driving_up, hold, driving_up},
    {driving_up, stop_btw, stop_between},
    {driving_up, stop_flr, stop_floor},
    {driving_up, fail, end},

    {driving_down, stay, floor_stationary},
    {driving_down, hold, driving_down},
    {driving_down, stop_btw, stop_between},
    {driving_down, stop_flr, stop_floor},
    {driving_down, fail, end},

    {stop_between, hold, stop_between},
    {stop_between, idle, idle_state},
    {stop_between, fail, end},

    {stop_floor, hold, stop_floor},
    {stop_floor, idle, idle_state},
    {stop_floor, fail, end},

    {idle_state, hold, idle_state},
    {idle_state, stop_btw, stop_between},
    {idle_state, stop_flr, stop_floor}
};

state_codes_t lookup_transitions(state_codes_t cur_state, return_codes_t ret_code)
{   
    int i;
    for (i = 0; i < sizeof(state_transitions)/sizeof(transition_t); i++)
    {
        if(state_transitions[i].src_state == cur_state && state_transitions[i].return_code == ret_code) 
        {
            return state_transitions[i].destination_state;
        }
    }
    return end;
}


return_codes_t fsm_initialize_state(void)
{
    //Go to floor 1, reject any orders until finished

    printf("State: initilize\n");
    elev_set_motor_direction(DIRN_DOWN);

    exec_set_floor_light();

    floor_codes_t floor = elev_get_floor_sensor_signal();

    if (elev_get_stop_signal() && floor >= 0) {
        return stop_flr;
    }
    else if (elev_get_stop_signal()) {
        return stop_btw;
    }
    else if(floor < 0)
    {
        return drive_down;
    }
    else
    {
        exec_set_destination_floor(floor);
        return hold;
    }
}   

return_codes_t fsm_floor_stationary_state(void) {
    printf("State: floor_stationary_state\n");

    floor_codes_t current_floor = elev_get_floor_sensor_signal();
    exec_set_last_floor(current_floor);
    exec_set_last_direction(STOP_DIR);
    elev_set_floor_indicator(current_floor);

    //Since this state is only reached when the elevator should stop we can remove all orders on this floor
    order_remove(current_floor);

    //Checks if stop button is pressed
    if (elev_get_stop_signal()) {
        return stop_flr;
    }

    //Update queue
    exec_check_order_buttons();
    
    //Stop elevator
    elev_set_motor_direction(DIRN_STOP);

    //Debug purposes
    order_print_orders();

    //Fetch current queues
    inside_queue_t *inside_queue = order_get_inside_queue();
    outside_queue_t *outside_queue = order_get_outside_queue();

    //Update current destination_floor
    exec_update_destination_floor(inside_queue, outside_queue);

    

    //printf("Dest. floor: %i\n", exec_get_destination_floor());
    //Get return code and return this
    return_codes_t return_code = exec_get_return_code(current_floor);
    return return_code;
}
/*
return_codes_t fsm_floor_1_state(void) 
{

    //Declare, stop and indicate
    printf("State: floor_1\n");

    elev_set_motor_direction(DIRN_STOP);
    elev_set_floor_indicator(0);
    
    //Check if stop button is pressed, if so open door
    if (elev_get_stop_signal()) {
        exec_update_state_log(floor_1);
        return stop_flr;
    }

    //Check for orders and update queue
    exec_check_order_buttons();
    //Debug purposes
    order_print_orders();
    //exec_timer(1000);

    //Fetch queue-ptrs
    inside_queue_t *inside_queue = order_get_inside_queue();
    outside_queue_t *outside_queue = order_get_outside_queue();
    

    //This code opens the door for 3 sec if there was an order to 1
    if (exec_scan_orders(floor_1)) {
        elev_set_door_open_lamp(1);

        order_remove(outside_1_up);
        order_remove(inside_1);

        exec_timer(3000);
        
        elev_set_door_open_lamp(0);
    }
    

    //Decide destination floor if floor was destination
    if (exec_get_destination_floor()==floor_1)
    {
        //Arrived at destination floor, get new destination based on queues
        exec_update_destination_floor(floor_1,inside_queue,outside_queue);
    }

    //Update state log and possibly leave the state.
    exec_update_state_log(floor_1);
    return exec_get_return_code(floor_1);
    

}

return_codes_t fsm_floor_2_state(void) 
{
    printf("State: floor_2\n");

    //Check STOP-button
    if (elev_get_stop_signal()) {
        exec_update_state_log(floor_2);
        return stop_flr;
    }
    if(exec_scan_orders(floor_2) || exec_get_destination_floor()==floor_2)
    {
        elev_set_motor_direction(DIRN_STOP);
        if(exec_scan_orders(floor_2))
        {
            elev_set_door_open_lamp(1);

            order_remove(outside_2_up);
            order_remove(outside_2_down);
            order_remove(inside_2);

            exec_timer(3000);
        
            elev_set_door_open_lamp(0);
        }

        //Check if was destination floor:
        if (exec_get_destination_floor()==floor_2)
        {
            //Fetch queues
            inside_queue_t *inside_queue = order_get_inside_queue();
            outside_queue_t *outside_queue = order_get_outside_queue();

            //Arrived at destination floor, get new destination based on queues
            exec_update_destination_floor(floor_1,inside_queue,outside_queue);
        }

    }

    //Check for orders and update queues.
    exec_check_order_buttons();

    exec_update_state_log(floor_2);
    return exec_get_return_code(floor_2);
}

return_codes_t fsm_floor_3_state(void) 
{
    printf("State: floor_3\n");

    //Check STOP-button
    if (elev_get_stop_signal()) {
        exec_update_state_log(floor_3);
        return stop_flr;
    }

    //Check for orders and update queues.
    exec_check_order_buttons();

    if(exec_scan_orders(floor_3) || exec_get_destination_floor()==floor_3)
    {
        if(exec_scan_orders(floor_3))
        {
            elev_set_motor_direction(DIRN_STOP);
            elev_set_door_open_lamp(1);

            order_remove(outside_3_up);
            order_remove(outside_3_down);
            order_remove(inside_3);

            exec_timer(3000);
            
            elev_set_door_open_lamp(0);
        }
        if (exec_get_destination_floor()==floor_3)
        {
            //Fetch queues
            inside_queue_t *inside_queue = order_get_inside_queue();
            outside_queue_t *outside_queue = order_get_outside_queue();

            //Arrived at destination floor, get new destination based on queues
            exec_update_destination_floor(floor_3,inside_queue,outside_queue);
        }

    }
    //Check if was destination floor:
    exec_update_state_log(floor_3);
    return exec_get_return_code(floor_3);
}

return_codes_t fsm_floor_4_state(void) 
{
    printf("State: floor_4\n");

    //Check STOP-button
    if (elev_get_stop_signal()) {
        exec_update_state_log(floor_4);
        return stop_flr;
    }

    //Check for orders and update queues.
    exec_check_order_buttons();

    if(exec_scan_orders(floor_4) || exec_get_destination_floor()==floor_4)
    {
        if(exec_scan_orders(floor_4))
        {
            elev_set_motor_direction(DIRN_STOP);
            elev_set_door_open_lamp(1);

            order_remove(outside_4_down);
            order_remove(inside_4);

            exec_timer(3000);
            
            elev_set_door_open_lamp(0);
        }
        if (exec_get_destination_floor()==floor_4)
        {
            //Fetch queues
            inside_queue_t *inside_queue = order_get_inside_queue();
            outside_queue_t *outside_queue = order_get_outside_queue();

            //Arrived at destination floor, get new destination based on queues
            exec_update_destination_floor(floor_4, inside_queue,outside_queue);
        }

    }
    //Check if was destination floor:
    exec_update_state_log(floor_4);
    return exec_get_return_code(floor_4);
}
*/
return_codes_t fsm_driving_up_state(void) 
{
    printf("State: Driving up\n");

    //Set last_direction to UP
    //exec_set_last_direction(UP);

    //Check if elevator is at a floor
    floor_codes_t current_floor = elev_get_floor_sensor_signal();

    //Update last floor if elevator is at a floor
    if (current_floor != between_floors) {
        exec_set_last_floor(current_floor);
    }

    //Check if stop button is pressed
    int stop = elev_get_stop_signal();
    if (stop && current_floor == -1) { //Check if this works!
        return stop_btw;
    }
    else if (stop) {
        return stop_flr;
    }

    //Check for orders
    exec_check_order_buttons();

    //Debug purposes
    order_print_orders();

    //Check if elevator is near a floor. If yes, check if it should stop
    if (current_floor != between_floors) {
        if (exec_scan_orders()) {
            return stay;
        }
    }

    //Set motor direction
    elev_set_motor_direction(DIRN_UP);

    //Return hold to continue driving up
    return hold;
}

return_codes_t fsm_driving_down_state(void) 
{
    printf("State: Driving down\n");

    //Set last_direction to DOWN
    exec_set_last_direction(DOWN);

    //Check if elevator is at a floor
    floor_codes_t current_floor = elev_get_floor_sensor_signal();

    //Update last floor if elevator is at a floor
    if (current_floor != between_floors) {
        exec_set_last_floor(current_floor);
    }

    //Check if stop button is pressed
    int stop = elev_get_stop_signal();
    if (stop && current_floor == -1) { //Check if this works!
        return stop_btw;
    }
    else if (stop) {
        return stop_flr;
    }

    //Check for orders
    exec_check_order_buttons();

    //Debug purposes
    order_print_orders();

    //Check if elevator is near a floor. If yes, check if it should stop
    if (current_floor != between_floors) {
        if (exec_scan_orders()) {
            printf("Dest. floor: %d\n", exec_get_destination_floor());
            return stay;
        }
    }

    //Set motor direction
    elev_set_motor_direction(DIRN_DOWN);

    //Return hold to continue driving up
    return hold;
}

return_codes_t fsm_stop_floor_state(void) {
    printf("State: stop_floor_state\n");

    elev_set_motor_direction(DIRN_STOP);
    elev_set_stop_lamp(1);
    
    elev_set_door_open_lamp(1);
    order_clear_all();
    
    if (!elev_get_stop_signal()) {
        elev_set_stop_lamp(0);
        elev_set_door_open_lamp(0);
        return get_last_floor();
    }
    return hold;
}

return_codes_t fsm_stop_between_state(void) {
    printf("State: stop_between_state\n");
    elev_set_motor_direction(DIRN_STOP);
    elev_set_stop_lamp(1);
    order_clear_all();
    if (!elev_get_stop_signal()) {
        elev_set_stop_lamp(0);
        return idle;
    }
    order_print_orders();
    return hold; 
}

return_codes_t fsm_idle_state(void) {
    printf("State: idle\n");
    elev_set_motor_direction(DIRN_STOP);
    if (elev_get_stop_signal() && elev_get_floor_sensor_signal() >= 0) {
        return stop_flr;
    }
    else if (elev_get_stop_signal()) {
        return stop_btw;
    }
    exec_check_order_buttons();

    return hold;
}

return_codes_t fsm_end_state(void) {
    printf("We reached the end state\n");
    exit(1);
}