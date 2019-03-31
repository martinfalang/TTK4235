

// Wrapper for libComedi Elevator control.
// These functions provides an interface to the elevators in the real time lab
//
// 2007, Martin Korsgaard


#include "elev.h"
#include <assert.h>
#include <stdlib.h>

// Number of signals and lamps on a per-floor basis (excl sensor)
#define N_BUTTONS 3
/*
static const int lamp_channel_matrix[N_FLOORS][N_BUTTONS] = {
	{ LIGHT_UP1, LIGHT_DOWN1, LIGHT_COMMAND1 },
	{ LIGHT_UP2, LIGHT_DOWN2, LIGHT_COMMAND2 },
	{ LIGHT_UP3, LIGHT_DOWN3, LIGHT_COMMAND3 },
	{ LIGHT_UP4, LIGHT_DOWN4, LIGHT_COMMAND4 },
};


static const int button_channel_matrix[N_FLOORS][N_BUTTONS] = {
	{ BUTTON_UP1, BUTTON_DOWN1, BUTTON_COMMAND1 },
	{ BUTTON_UP2, BUTTON_DOWN2, BUTTON_COMMAND2 },
	{ BUTTON_UP3, BUTTON_DOWN3, BUTTON_COMMAND3 },
	{ BUTTON_UP4, BUTTON_DOWN4, BUTTON_COMMAND4 },
};*/

Simulator* simulator;

int elev_init(void) {

	// Init simulator
	simulator = new Simulator;

	// Zero all floor button lamps
	for (int i = 0; i < N_FLOORS; ++i) {
		if (i != 0)
			elev_set_button_lamp(BUTTON_CALL_DOWN, i, 0);

		if (i != N_FLOORS - 1)
			elev_set_button_lamp(BUTTON_CALL_UP, i, 0);

		elev_set_button_lamp(BUTTON_COMMAND, i, 0);
	}

	// Clear stop lamp, door open lamp, and set floor indicator to ground floor.
	elev_set_stop_lamp(0);
	//elev_set_door_open_lamp(0);
	elev_set_floor_indicator(0);

	// Return success.
	return 1;
}

void elev_set_motor_direction(elev_motor_direction_t dirn) {
	if (dirn == 0) {
		simulator->motorEnabled = false;
	}
	else if (dirn > 0) {
		simulator->motorEnabled = true;
		simulator->direction = false;
	}
	else if (dirn < 0) {
		simulator->motorEnabled = true;
		simulator->direction = true;
	}
}
/* TODO:
void elev_set_door_open_lamp(int value) {
}*/

int elev_get_stop_signal() {
	return simulator->stopButton;
}

void elev_set_stop_lamp(int value) {
	simulator->toggleButtonLight(STOP_BUTTON, value);
}

int elev_get_floor_sensor_signal() {
	for (int i = 0; i < 4; i++)
	{
		if (simulator->sensors[i]) {
			return i;
		}
	}
	return -1;
}

void elev_set_floor_indicator(int floor) {
	assert(floor >= 0);
	assert(floor < N_FLOORS);

	// Binary encoding. One light must always be on.
	simulator->toggleFloorLight(static_cast<FLOOR>(floor));
}

int elev_get_button_signal(elev_button_type_t button, int floor) {
	assert(floor >= 0);
	assert(floor < N_FLOORS);
	assert(!(button == BUTTON_CALL_UP && floor == N_FLOORS - 1));
	assert(!(button == BUTTON_CALL_DOWN && floor == 0));
	assert(button == BUTTON_CALL_UP || button == BUTTON_CALL_DOWN || button == BUTTON_COMMAND);

	switch (button)
	{
	case BUTTON_CALL_UP:
		return simulator->buttonsUp[floor];
	case BUTTON_CALL_DOWN:
		return simulator->buttonsDown[floor-1];
	case BUTTON_COMMAND:
		return simulator->buttonsInElevator[floor];

	}
}

void elev_set_button_lamp(elev_button_type_t button, int floor, int value) {
	assert(floor >= 0);
	assert(floor < N_FLOORS);
	assert(!(button == BUTTON_CALL_UP && floor == N_FLOORS - 1));
	assert(!(button == BUTTON_CALL_DOWN && floor == 0));
	assert(button == BUTTON_CALL_UP || button == BUTTON_CALL_DOWN || button == BUTTON_COMMAND);

	simulator->toggleButtonLight(buttonConverter(button, floor), value);
	
}



BUTTON buttonConverter(elev_button_type_t button, int floor) {
	if (button == BUTTON_COMMAND)
	{
		return static_cast<BUTTON>(floor);
	}
	if (button == BUTTON_CALL_UP) {
		return static_cast<BUTTON>(floor + 4);
	}
	else {
		return static_cast<BUTTON>(floor + 6);
	}
}

int elev_set_door_open_lamp(int value)
{
	if(value==1)
	{
		printf("Door open\n");
	}
	else
	{
		printf("Door closed\n");
	}
	return 0;
}
