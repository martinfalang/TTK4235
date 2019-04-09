#ifndef FSM_H
#define FSM_H
///////////////////////////////////////////////
/*
    TOP LEVEL UPDATES:
        - Merged stop_floor state and stop_between state
            and updated state_codes and return_codes 
            accordingly
*/
///////////////////////////////////////////////

///////////////////////////////////////////////
// Enums
///////////////////////////////////////////////

typedef enum floor_codes {
    floor_1,
    floor_2,
    floor_3,
    floor_4,
    between_floors = -1
} floor_codes_t;

typedef enum direction_codes {
    UP = 1,
    DOWN = -1,
    STOP_DIR = 0,
} direction_codes_t;

typedef enum state_codes {
    floor_stationary,
    initialize,
    driving_up,
    driving_down,
    stop_state,
    idle_state,
    end
} state_codes_t;

///////////////////////////////////////////////
/*
    UPDATES:
        - Deleted arrived_(1-4) since these are
            not used anymore
        - Deleted idle return code as it is cleaner
            to just use stay from the stop_between state
        - Made idle a return code again to let be able
            to have 1 stop state
        - Replaced stop_btw and stop_flr with just stop
            as it is now it's own function
*/
///////////////////////////////////////////////
typedef enum return_codes {
    drive_up,
    drive_down,
    hold,
    stay,
    idle,
    stop,
    fail
} return_codes_t;

///////////////////////////////////////////////
// Structures
///////////////////////////////////////////////

typedef struct transition {
    state_codes_t src_state;
    return_codes_t return_code;
    state_codes_t destination_state;
} transition_t;

///////////////////////////////////////////////
// Function declarations
///////////////////////////////////////////////

return_codes_t fsm_floor_stationary_state(void);
return_codes_t fsm_initialize_state(void);
return_codes_t fsm_driving_up_state(void);
return_codes_t fsm_driving_down_state(void);
return_codes_t fsm_stop_state(void);
return_codes_t fsm_idle_state(void);
return_codes_t fsm_end_state(void);

state_codes_t lookup_transitions(state_codes_t cur_state, return_codes_t ret_code);

///////////////////////////////////////////////
// Array declarations
///////////////////////////////////////////////

/* array and enum state_codes below must be in sync! */
extern return_codes_t (* state[])(void);

extern transition_t state_transitions[];

#endif /* FSM_H */