
#include "fsm.h"
#include "stdio.h"

/* array and enum state_codes below must be in sync! */
int (* state[])(void) = {
    initialize_state, 
    floor_1_state
    /*, 
    floor_2_state, 
    floor_3_state, 
    floor_4_state, 
    driving_up_state, 
    driving_down_state,
    stop_floor_state, 
    stop_between_state, 
    end_state
    */
};

transition_t state_transitions[] = {
    {initialize, go_down, floor_1},
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
    
}

void test_state_stuff() 
{
    state_codes_t current_state = initialize;
    return_codes_t rc;
    int (* state_func)(void);
    state_func=state[current_state];
    rc = state_func();
    printf("This is the current state: %d\n", current_state);
    current_state = lookup_transitions(current_state,rc);

    state_func=state[current_state];
    rc = state_func();

}

int initialize_state(void)
{
    //Talk with Execution Handler to go to floor 1, dont return until finished
    printf("We are now in initilize_state\n");
    return_codes_t return_value = floor_1;
    return return_value; 
}   

int floor_1_state(void) 
{
    printf("We are now in floor_1_state\n");
    return 0;
}