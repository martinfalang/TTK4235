
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
    {initialize, go_down, initialize},
    {initialize, hold, floor_1},            //Added this one in testing, better return codes should be implemented
    {initialize, stop_flr, stop_floor},
    {initialize, stop_btw, stop_between},
    {initialize, fail, end},

    {floor_1, go_up, driving_up},
    {floor_1, hold, floor_1},
    {floor_1, idle, idle_state},
    {floor_1, stop_flr, stop_floor},
    {floor_1, fail, end},

    {floor_2, go_up, driving_up},
    {floor_2, go_down, driving_down},
    {floor_2, hold, floor_2},
    {floor_2, stop_flr, stop_floor},
    {floor_2, fail, end},

    {floor_3, go_up, driving_up},
    {floor_3, go_down, driving_down},
    {floor_3, hold, floor_3},
    {floor_3, stop_flr, stop_floor},
    {floor_3, fail, end},

    {floor_4, go_down, driving_down},
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

state_codes_t lookup_transitions(state_codes_t cur_state, return_codes_t ret_code) {   
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

    if (elev_get_stop_signal() && elev_get_floor_sensor_signal() >= 0) {
        return stop_flr;
    }
    else if (elev_get_stop_signal()) {
        return stop_btw;
    }
    else if(elev_get_floor_sensor_signal() != 0)
    {
        return go_down;
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
        exec_update_position(floor_1);
        return stop_flr;
    }

    //Open door, wait and close if any order
    exec_check_order_buttons();
    int *order_array = order_get_orders();
    order_print_orders();
    //This code opens the door for 3 sec if there was an order to 1
    if (order_array[outside_1_up] || order_array[inside_1]) {
        elev_set_door_open_lamp(1);

        order_remove(outside_1_up);
        order_remove(inside_1);

        exec_timer(3000);
        
        elev_set_door_open_lamp(0);
    }
    

    //Decide destination floor

    //Decide return code


    //Hold on floor
    return hold;
}

return_codes_t fsm_floor_2_state(void) 
{
    printf("We are now in floor_2_state\n");
    elev_set_motor_direction(DIRN_STOP);
    return hold;
}

return_codes_t fsm_floor_3_state(void) 
{
    printf("We are now in floor_3_state\n");
    elev_set_motor_direction(DIRN_STOP);
    return hold;
}

return_codes_t fsm_floor_4_state(void) 
{
    printf("We are now in floor_4_state\n");
    elev_set_motor_direction(DIRN_STOP);
    return hold;
}

return_codes_t fsm_driving_up_state(void) 
{
    printf("We are now in driving_up_state\n");
    //elev_set_motor_direction(DIRN_STOP);
    return hold;
}

return_codes_t fsm_driving_down_state(void) 
{
    printf("We are now in driving_down_state\n");
    //elev_set_motor_direction(DIRN_STOP);
    return hold;
}

return_codes_t fsm_stop_floor_state(void) {
    printf("State: stop_floor_state\n");

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
    printf("We are now in stop_between_state\n");
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