#include "Simulator.h"



Simulator::Simulator() : elevator(), graphics(elevator), clock(), time()
{
	debugTime = clock.getElapsedTime();
	graphics.window.setActive(true);
}


Simulator::~Simulator()
{
}



void Simulator::update()
{
	if (graphics.window.isOpen()) {

		time = clock.restart();
		if (logDebug)
		{
			debugTime += time;
			if (debugTime.asSeconds()>DEBUG_LOG_INTERVALL)
			{
				logStatusToConsole();
				debugTime = sf::Time::Zero;
			}
		}
		//Events
		handleEvents();
		

		//Logic
		if (motorEnabled)
		{
			graphics.moveElevator(ELEVATOR_SPEEED*time.asSeconds(), direction);
		}
		checkFloorSensor();
		graphics.updateGraphics();
	}
}

void Simulator::toggleDebugLogging()
{
	logDebug = !logDebug;
}

void Simulator::toggleFloorLight(FLOOR floor)
{
	graphics.toggleFloorLight(floor);
}

void Simulator::toggleButtonLight(BUTTON button, bool value)
{
	graphics.toggleButtonLight(button, value);
}


void Simulator::logStatusToConsole()
{
	std::cout <<std::endl << "---Buttons in elevator---" << std::endl;
	for (int i = 0; i < NUMBER_OF_FLOORS; i++)
	{
		std::cout << "Floor " << i + 1 << ": " << buttonsInElevator[i] << std::endl;
	}

	std::cout << std::endl << "---Up buttons---" << std::endl;
	for (int i = 0; i < NUMBER_OF_FLOORS - 1; i++) {
		std::cout << "Up floor " << i + 1 << ": " << buttonsUp[i] << std::endl;
	}
	
	std::cout << std::endl << "---Down buttons---" << std::endl << std::endl;;
	for (int i = 0; i < NUMBER_OF_FLOORS-1; i++)
	{
		std::cout << "Down floor " << i + 2 << ": " << buttonsDown[i] << std::endl;
	}
}

void Simulator::setHardwareBitsFromButtonPress(sf::Keyboard::Key key, bool value)
{
	switch (key)
	{
	//Buttons down
	case sf::Keyboard::Num8:
		buttonsUp[2] = value;
		break;
	case sf::Keyboard::I:
		buttonsUp[1] = value;
		break;
	case sf::Keyboard::K:
		buttonsUp[0] = value;
		break;

	//Buttons up
	case sf::Keyboard::N: //1st
		buttonsDown[0] = value;
		break;
	case sf::Keyboard::J: //2nd
		buttonsDown[1] = value;
		break;
	case sf::Keyboard::U: //3rd
		buttonsDown[2] = value;
		break;

	//Inside elevator
	case sf::Keyboard::Num1:
	case sf::Keyboard::Num2:
	case sf::Keyboard::Num3:
	case sf::Keyboard::Num4:
		buttonsInElevator[static_cast<int>(key) - 27] = value;
		break;

	//Stop button
	case sf::Keyboard::Space:
		stopButton = value;
		break;

	default:
		break;
	}
}

void Simulator::checkFloorSensor()
{
	float height = elevator.getPosition().y;
	for (int i = 0; i < NUMBER_OF_FLOORS; i++)
	{
		int floor_y = FLOOR_HEIGHT*i;
		if (SENSOR_PIXEL_MARGIN + floor_y > height && height > floor_y - SENSOR_PIXEL_MARGIN)
		{
			if (logDebug && !sensors[4-i-1])
			{
				std::cout << "Sensor " << 4-i<< " is active." << std::endl;
			}
			sensors[4-i-1] = true;
			return;
		}
		else {
			if (logDebug && sensors[4-i-1])
			{
				std::cout << "Sensor " << 4-i<< " is no longer active." << std::endl;
			}
			sensors[4-i-1] = false;
		}
	}
}

void Simulator::handleEvents()
{
	sf::Event event;
	while (graphics.window.pollEvent(event)) {

		if (event.type == sf::Event::Closed) {
			graphics.window.close();
		}
		if (event.type == sf::Event::MouseButtonPressed) {
			sf::Vector2i pos = sf::Mouse::getPosition(graphics.window);

			std::cout << "Mouse at " << pos.x << "," << pos.y << std::endl;
		}
		if (event.type == sf::Event::KeyPressed)
		{


			//~~~~~~~~DEBUG BUTTONS~~~~~~

			//Direction
			if (event.key.code == sf::Keyboard::Up)
			{
				direction = false;
			}

			if (event.key.code == sf::Keyboard::Down)
			{
				direction = true;
			}
			//Toggle motor on/off
			if (event.key.code == sf::Keyboard::Return)
			{
				motorEnabled = !motorEnabled;
			}
			//~~~~~~~~~~~~~~~~~~~~~~~~~~

			setHardwareBitsFromButtonPress(event.key.code, true);
			
		}
		if (event.type == sf::Event::KeyReleased) 
		{
			setHardwareBitsFromButtonPress(event.key.code, false);
		}

	}
}
