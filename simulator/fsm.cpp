
#include "fsm.h"
#include "stdio.h"
#include "elev.h"
#include "time.h"

extern "C" {
/* array and enum state_codes below must be in sync! */
return_codes_t (* state[])(void) = {
    initialize_state, 
    floor_1_state, 
    floor_2_state, 
    /*floor_3_state, 
    floor_4_state, 
    driving_up_state, 
    driving_down_state,
    stop_floor_state, 
    stop_between_state,
    */ 
    //end_state
};

transition_t state_transitions[] = {
    {initialize, go_down, initialize},
    {initialize, hold, floor_1},            //Added this one in testing, better return codes should be implemented
    {initialize, fail, end},
    
    {floor_1, go_up, driving_up},
    {floor_1, hold, floor_1},
    {floor_1, stop, stop_floor},
    {floor_1, fail, end},

    {floor_2, go_up, driving_up},
    {floor_2, go_down, driving_down},
    {floor_2, hold, floor_2},
    {floor_2, stop, stop_floor},
    {floor_2, fail, end},

    {floor_3, go_up, driving_up},
    {floor_3, go_down, driving_down},
    {floor_3, hold, floor_3},
    {floor_3, stop, stop_floor},
    {floor_3, fail, end},

    {floor_4, go_down, driving_down},
    {floor_4, hold, floor_4},
    {floor_4, stop, stop_floor},
    {floor_4, fail,end},

    {driving_up, arrived_2, floor_2},
    {driving_up, arrived_3, floor_3},
    {driving_up, arrived_4, floor_4},
    {driving_up, hold, driving_up},
    {driving_up, stop, stop_between},
    {driving_up, fail, end},

    {driving_down, arrived_3, floor_3},
    {driving_down, arrived_2, floor_2},
    {driving_down, arrived_1, floor_1},
    {driving_down, hold, driving_down},
    {driving_down, stop, stop_between},
    {driving_down, fail, end}
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
}

return_codes_t initialize_state(void)
{
    //Talk with Execution Handler to go to floor 1, dont return until finished

    printf("We are now in initilize_state\n");
    elev_set_motor_direction(DIRN_DOWN);
    if(elev_get_floor_sensor_signal()!=0)
    {
        return go_down;
    }
    else
    {
        return hold;
    }
}   

return_codes_t floor_1_state(void) 
{
    printf("We are now in floor_1_state\n");
    elev_set_motor_direction(DIRN_STOP);
    return hold;
}

return_codes_t floor_2_state(void) 
{
    printf("We are now in floor_2_state\n");
    elev_set_motor_direction(DIRN_STOP);
    return hold;
}
