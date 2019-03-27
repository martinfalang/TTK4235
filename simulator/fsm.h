#ifndef FSM_H
#define FSM_H

extern "C"{
/*
int floor_2_state(void);
int floor_3_state(void);
int floor_4_state(void);
int driving_up_state(void);
int driving_down_state(void);
int stop_floor_state(void);
int stop_between_state(void);
*/


typedef enum state_codes {
    initialize,
    floor_1,
    floor_2,
    floor_3,
    floor_4,
    driving_up,
    driving_down,
    stop_floor,
    stop_between,
    end
} state_codes_t;

typedef enum return_codes {
    go_up,
    go_down,
    hold,
    stop,
    fail,
    arrived_1,
    arrived_2,
    arrived_3,
    arrived_4,
    repeat
} return_codes_t;

typedef struct transition {
    state_codes_t src_state;
    return_codes_t return_code;
    state_codes_t destination_state;
} transition_t;


return_codes_t initialize_state(void);
return_codes_t floor_1_state(void);
return_codes_t floor_2_state(void);
void end_state(void);

/* array and enum state_codes below must be in sync! */
extern return_codes_t (* state[])(void);

extern transition_t state_transitions[];




state_codes_t lookup_transitions(state_codes_t cur_state, return_codes_t ret_code);
}

#endif