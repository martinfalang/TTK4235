#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "elev.h"
#include <iostream>
#include <cstdlib>


#include "fsm.h"

int main() {
	
	//Initialize "hardware"
	elev_init();


	//Randomize position
	bool finished=false;
	printf("Randomising starting position");
	int randnum1=10000; //Not very random yet
	while(!finished)
	{
		if((randnum1)==0)
		{
			finished=true;
		}
		randnum1=randnum1-1;
		printf("Randomizing postion: Steps left until stop is %d\n",randnum1);
		elev_set_motor_direction(DIRN_UP);
		simulator->update();
	}
	elev_set_motor_direction(DIRN_STOP);
	printf("Randomising starting position finished");



	//Run initialization of state machine to get elevator to planned starting position
	state_codes_t current_state = initialize;
    return_codes_t rc;
    return_codes_t (* state_func)(void);
    state_func=state[current_state];
    rc = state_func();


	
	while (true)
	{
		/*
		for (int i = 0; i < 4; i++)
		{
			for (int but = 0; but < 3; but++)
			{
				elev_button_type_t button = static_cast<elev_button_type_t>(but);
				if (i==0 && button == BUTTON_CALL_DOWN)
				{
					continue;
				}
				if (i==3 && button == BUTTON_CALL_UP)
				{
					continue;
				}
				elev_set_button_lamp(button,i, elev_get_button_signal(button, i));
				
			}
		
		}
		elev_set_stop_lamp(elev_get_stop_signal());
		*/
		

		//Finite state machine running
		current_state = lookup_transitions(current_state,rc);
    	state_func=state[current_state];
    	rc = state_func();
		//End of finite state machine

		simulator->update();
	}
	
	

	return 0;

}