
#include "fsm.h"
#include "stdio.h"
#include "elev.h"
#include "time.h"
#include "order.h"
#include "exec.h"

#include <stdlib.h> //for exit()

//static int current_destination;

return_codes_t (*state[])(void) = {
    fsm_initialize_state,
    fsm_floor_1_state,
    fsm_floor_2_state,
    fsm_floor_3_state,
    fsm_floor_4_state,
    fsm_driving_up_state,
    fsm_driving_down_state,
    fsm_stop_floor_state,
    fsm_stop_between_state,
    fsm_idle_state,
    fsm_end_state
};

transition_t state_transitions[] = {
    {initialize, drive_down, initialize},
    {initialize, hold, floor_1},            //Added this one in testing, better return codes should be implemented
    {initialize, stop_flr, stop_floor},
    {initialize, stop_btw, stop_between},
    {initialize, fail, end},

    {floor_1, drive_up, driving_up},
    {floor_1, hold, floor_1},
    {floor_1, idle, idle_state},
    {floor_1, stop_flr, stop_floor},
    {floor_1, fail, end},

    {floor_2, drive_up, driving_up},
    {floor_2, drive_down, driving_down},
    {floor_2, hold, floor_2},
    {floor_2, stop_flr, stop_floor},
    {floor_2, fail, end},

    {floor_3, drive_up, driving_up},
    {floor_3, drive_down, driving_down},
    {floor_3, hold, floor_3},
    {floor_3, stop_flr, stop_floor},
    {floor_3, fail, end},

    {floor_4, drive_down, driving_down},
    {floor_4, hold, floor_4},
    {floor_4, stop_flr, stop_floor},
    {floor_4, fail,end},

    {driving_up, arrived_2, floor_2},
    {driving_up, arrived_3, floor_3},
    {driving_up, arrived_4, floor_4},
    {driving_up, hold, driving_up},
    {driving_up, stop_btw, stop_between},
    {driving_up, stop_flr, stop_floor},
    {driving_up, fail, end},

    {driving_down, arrived_3, floor_3},
    {driving_down, arrived_2, floor_2},
    {driving_down, arrived_1, floor_1},
    {driving_down, hold, driving_down},
    {driving_down, stop_btw, stop_between},
    {driving_down, stop_flr, stop_floor},
    {driving_down, fail, end},

    {stop_between, hold, stop_between},
    {stop_between, idle, idle_state},
    {stop_between, fail, end},

    {stop_floor, hold, stop_floor},
    {stop_floor, idle, idle_state},
    {stop_floor, arrived_1, floor_1},
    {stop_floor, arrived_2, floor_2},
    {stop_floor, arrived_3, floor_3},
    {stop_floor, arrived_4, floor_4},
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
    exec_intialize_destination_floor();
    exec_set_floor_light();

    if (elev_get_stop_signal() && elev_get_floor_sensor_signal() >= 0) {
        return stop_flr;
    }
    else if (elev_get_stop_signal()) {
        return stop_btw;
    }
    else if(elev_get_floor_sensor_signal() != 0)
    {
        return drive_down;
    }
    else
    {
        return hold;
    }
}   

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


    //Fetch queue-ptrs
    inside_order_t *inside_queue = order_get_inside_queue();
    outside_order_t *outside_queue = order_get_outside_queue();

    

    //This code opens the door for 3 sec if there was an order to 1
    if (exec_scan_orders(floor_1)) {
        elev_set_door_open_lamp(1);

        order_remove(outside_1_up);
        order_remove(inside_1);

        exec_timer(3000);
        
        elev_set_door_open_lamp(0);
    }
    

    //Decide destination floor
    if (exec_get_destination_floor()==floor_1)
    {
        //Arrived at destination floor, get new destination based on queues
        exec_update_destination_floor(floor_1,inside_queue,outside_queue);
    }

    //Decide return code
    if(exec_get_destination_floor()==floor_1)
    {
        //No new destination, stay on floor!
        exec_update_state_log(floor_1);
        return hold;
    }
    else
    {
        //New destination order, follow it!
        exec_update_state_log(floor_1);
        return drive_up;
    }
    

}

return_codes_t fsm_floor_2_state(void) 
{
    printf("State: floor_2\n");
    if(exec_scan_orders(floor_2) || exec_get_destination_floor()==floor_2)
    {
        elev_set_motor_direction(DIRN_STOP);
        return hold;
        
    }
    return exec_get_last_direction();
}

return_codes_t fsm_floor_3_state(void) 
{
    printf("State: floor_3\n");
    if(exec_scan_orders(floor_3) || exec_get_destination_floor()==floor_3)
    {
        elev_set_motor_direction(DIRN_STOP);
        return hold;
        
    }
    return exec_get_last_direction();
}

return_codes_t fsm_floor_4_state(void) 
{
    printf("State: floor_4\n");
    if(exec_scan_orders(floor_4) || exec_get_destination_floor()==floor_4)
    {
        elev_set_motor_direction(DIRN_STOP);
        return hold;
        
    }
    return exec_get_last_direction();
}

return_codes_t fsm_driving_up_state(void) 
{
    printf("State: Driving up\n");
    //Check for stop button
    if (elev_get_stop_signal()) {
        exec_update_state_log(floor_1);
        return stop_btw;
    }

    //Check for orders
    exec_check_order_buttons();

    //Debug purposes
    order_print_orders();

    elev_set_motor_direction(DIRN_UP);
    return_codes_t return_code=hold;
    switch(elev_get_floor_sensor_signal()){
        case 0: return_code = hold; break;
        case 1: return_code= arrived_2; break;
        case 2: return_code= arrived_3; break;
        case 3: return_code= arrived_4; break;
        default: return_code = hold; break;
    }
    exec_update_state_log(driving_up);
    return return_code;
}

return_codes_t fsm_driving_down_state(void) 
{
    printf("State: Driving down\n");
    //Check for stop button
    if (elev_get_stop_signal()) {
        exec_update_state_log(floor_1);
        return stop_btw;
    }

    //Check for orders
    exec_check_order_buttons();

    elev_set_motor_direction(DIRN_DOWN);
    return_codes_t return_code=hold;
    switch(elev_get_floor_sensor_signal()){
        case 0: return_code = hold; break;
        case 1: return_code= arrived_2; break;
        case 2: return_code= arrived_3; break;
        case 3: return_code= arrived_4; break;
        default: return_code = hold; break;
    }
    exec_update_state_log(driving_down);
    return return_code;
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