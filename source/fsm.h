/**
 * @file
 * @brief A state machine library
 */
#ifndef FSM_H
#define FSM_H

///////////////////////////////////////////////
// Includes
///////////////////////////////////////////////

#include "exec.h"

///////////////////////////////////////////////
// Enums
///////////////////////////////////////////////

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
// Structures
///////////////////////////////////////////////

typedef struct transition {
    state_codes_t src_state;
    return_codes_t return_code;
    state_codes_t destination_state;
} transition_t;

///////////////////////////////////////////////
// Array declarations
///////////////////////////////////////////////

/**
 * @warning Must be in sync with @c state_codes_t
 */
extern return_codes_t (* state[])(void);

extern transition_t state_transitions[];

///////////////////////////////////////////////
// Function declarations
///////////////////////////////////////////////

/**
 * @brief Algorithm for the stationary state. Listen for orders and return a return code to use in the @c lookup_transitions function.
 * 
 * @return Return code of type @c return_codes_t to find next state.
 */
return_codes_t fsm_floor_stationary_state(void);

/**
 * @brief Algorithm for the initialization state. Ignore all orders and move down until detects a floor,
 * then stops and returns a return code to use in the @c lookup_transitions function
 * 
 * @return Return code of type @c return_codes_t to find next state. 
 */
return_codes_t fsm_initialize_state(void);

/**
 * @brief Algorithm for the driving up state. Listen for orders and stop button and return a return code to use in the @c lookup_transitions function.
 * 
 * @return Return code of type @c return_codes_t to find next state.
 */
return_codes_t fsm_driving_up_state(void);

/**
 * @brief Algorithm for the driving down state. Listen for orders and stop button and return a return code to use in the @c lookup_transitions function.
 * 
 * @return Return code of type @c return_codes_t to find next state.
 */
return_codes_t fsm_driving_down_state(void);

/**
 * @brief Algorithm fot the stop state. Delete all orders, ignore new orders and stay in stop state while order_button is pressed. 
 * If stops on floor, keep door open while in this state. When stop button released, return a return code to use in the @c lookup_transitions function.
 * 
 * @return Return code of type @c return_codes_t to find next state.
 */
return_codes_t fsm_stop_state(void);

/**
 * @brief Algorithm for the idle state. Listen for orders and stop button and update queues if necessary. Stay idle until at least one order is recieved. 
 * Return a return code to use in the @c lookup_transitions function. 
 * 
 * @return Return code of type @c return_codes_t to find next state.
 */
return_codes_t fsm_idle_state(void);

/**
 * @brief A fail state, used for debugging.
 * 
 * @return Program exitis before any return occurs.
 * 
 * @warning Elevator should not reach this state, and can not leave it. If it is reached, the program exits with error code @c 1.
 */
return_codes_t fsm_end_state(void);

/**
 * @brief Takes in the current state and a return code generated from the fsm function of that state. 
 * Searches through a @c state_transitions array which contains all transition combinations and looks for the appropriate one to return a new state code.
 * 
 * @param[in] cur_state The current state the fsm is in.
 * @param[in] ret_code The Return code from the respective fsm state function.
 * 
 * @return Return state code for the next state.
 */
state_codes_t lookup_transitions(state_codes_t cur_state, return_codes_t ret_code);

#endif /* FSM_H */