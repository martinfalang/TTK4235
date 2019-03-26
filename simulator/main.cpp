/*#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "elev.h"
*/
#include "fsm.h"


int main() {
	/*
	elev_init();
	while (true)
	{
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
		simulator->update();
	}
	*/

	test_state_stuff();

	return 0;

}