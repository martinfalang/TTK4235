#pragma once
#include "ElevatorGraphics.h"
#include <iostream>


class Simulator
{
public:
	Simulator();
	~Simulator();

	//Hardware:
	bool motorEnabled = true;
	bool sensors[4] = { false,false,false,false }; //1st, 2nd, 3rd, 4th
	bool buttonsInElevator[4] = { false, false, false, false }; //1st, 2nd, 3rd, 4th
	bool buttonsUp[3] = { false, false, false }; //1st, 2nd, 3rd
	bool buttonsDown[3] = { false, false, false }; //2nd, 3rd, 4th
	bool direction = false; //false = up, true = down;
	bool stopButton = false;
	void update();

	//Disable/Enable debug log to console, default is disabled
	void toggleDebugLogging();
	void toggleFloorLight(FLOOR floor);
	void toggleButtonLight(BUTTON button, bool value);;
	void checkFloorSensor();

private:
	void logStatusToConsole();
	bool logDebug = false;
	void setHardwareBitsFromButtonPress(sf::Keyboard::Key key, bool value);
	Elevator elevator;
	ElevatorGraphics graphics;
	sf::Clock clock;
	sf::Time time;
	const float ELEVATOR_SPEEED = 80; //pixel/sec
	void handleEvents();
	const int NUMBER_OF_FLOORS = ElevatorGraphics::NUMBER_OF_FLOORS;
	const int WINDOW_HEIGHT = ElevatorGraphics::WINDOW_HEIGHT;
	const float FLOOR_HEIGHT = WINDOW_HEIGHT / NUMBER_OF_FLOORS;
	const int SENSOR_PIXEL_MARGIN = 2;
	const int DEBUG_LOG_INTERVALL = 2;
	sf::Time debugTime;






};

