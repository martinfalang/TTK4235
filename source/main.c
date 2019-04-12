
/**
 * @mainpage Elevator project - TTK4235 - Group 10
 * @section intro_sec Introduction
 * This page contains the documentation for our elevator project.
 * 
 * @section summary Implementation summary
 * 
 * @subsection fsm Finite State Machine (FSM)
 * Our elevator implementation is based on a finite 
 * state machine. All states in the FSM is 
 * in itself its own function and thus the current state 
 * is simply determined by which function is called. 
 * Each function returns a return code which is part 
 * of determining the next state function to call.
 * The FSM includes a transition table which contains 
 * all possible state transitions and a lookup function. 
 * This lookup function uses the transition table to find the 
 * next state based on the current state and the return 
 * code given by the current state function. In the main loop 
 * inside the main function in main.c the state is then updated 
 * and the next state function is called. The FSM is 
 * implemented in the fsm.c file. 
 * 
 * @subsection exec Execution handler
 * This project also has an execution handler module 
 * which is responsible for the actions of the elevator. 
 * The thought here is that the state functions in the 
 * FSM mostly just calls function in the execution 
 * handler to handle orders, get return codes, etc. 
 * The execution handler is implemented in the exec.c file. 
 * 
 * @subsection order_and_scheduler Order handling system
 * The order handling system is based around two queues, 
 * one for orders made inside the elevator and one for 
 * orders made outside the elevator. The reason for the 
 * seperation is because we believe that orders made 
 * inside the elevator should be executed before orders 
 * made outside the elevator, and thus having two seperate 
 * queues eases the logic for deciding which order to handle. 
 * To handle these queues our API includes a scheduler and 
 * an order handler. 
 * 
 * @subsubsection scheduler Scheduler
 * The scheduler handles the queue specific 
 * tasks of adding and deleting orders. It is based on a 
 * first in, first out principle to prioritize the 
 * orders. The scheduler also handles all the logic of 
 * validating the orders. This validation is specific for 
 * this elevator. The scheduler is implemented in the 
 * scheduler.c file.
 * 
 * @subsubsection order Order handler
 * The order handler serves as a link between the execution 
 * handler and the scheduler and does most of the ordering tasks, 
 * specific to this elevator such as removing all orders at 
 * all floor. The reasoning for this is to get another 
 * level of abstraction in the queue system. The implementation 
 * of the order handler can be found in the order.c file.
*/

///////////////////////////////////////////////
// Includes
///////////////////////////////////////////////

#include <stdio.h>
#include "fsm.h"

///////////////////////////////////////////////
// Main function
///////////////////////////////////////////////

int main()
{
    // Initialize hardware
    if (!elev_init())
    {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }
    elev_set_motor_direction(DIRN_STOP);

    // Set the starting state to initialize
    state_codes_t current_state = initialize;
    return_codes_t rc;

    // Make a function pointer
    return_codes_t (*state_func)(void);

    // Set the pointer to the function at place current_state in the state
    // function table.
    state_func = state[current_state];

    // Run the function an get a return code
    rc = state_func();

    printf("Elevator now running\n");

    while (1)
    {
        // Update current_state based on its return code
        current_state = lookup_transitions(current_state, rc);

        // Point to new state function
        state_func = state[current_state];

        //Run state function to generate new return code
        rc = state_func();
    }

    return 0;
}
