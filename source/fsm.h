#ifndef FSM_H
#define FSM_H

typedef enum floor_codes {
    floor_1,
    floor_2,
    floor_3,
    floor_4,
    between_floors = -1
} floor_codes_t;

typedef enum direction_codes {
    UP = 1,
    DOWN = -1
} direction_codes_t;

typedef enum state_codes {
    floor_stationary,
    initialize,
    driving_up,
    driving_down,
    stop_floor,
    stop_between,
    idle_state,
    end
} state_codes_t;

typedef enum return_codes {
    drive_up,
    drive_down,
    hold,
    stay,
    stop_btw,
    stop_flr,
    fail,
    arrived_1,
    arrived_2,
    arrived_3,
    arrived_4,
    idle
} return_codes_t;

typedef struct transition {
    state_codes_t src_state;
    return_codes_t return_code;
    state_codes_t destination_state;
} transition_t;

return_codes_t fsm_floor_stationary_state(void);
return_codes_t fsm_initialize_state(void);
return_codes_t fsm_driving_up_state(void);
return_codes_t fsm_driving_down_state(void);
return_codes_t fsm_stop_floor_state(void);
return_codes_t fsm_stop_between_state(void);
return_codes_t fsm_idle_state(void);
return_codes_t fsm_end_state(void);

/* array and enum state_codes below must be in sync! */
extern return_codes_t (* state[])(void);

extern transition_t state_transitions[];

state_codes_t lookup_transitions(state_codes_t cur_state, return_codes_t ret_code);


#endif