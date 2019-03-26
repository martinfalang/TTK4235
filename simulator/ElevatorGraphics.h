#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "Elevator.h"

//E = inside elevator, D = down, U = up, number = floor
enum BUTTON
{
	E1, E2, E3, E4, U1, U2, U3, D2, D3, D4, STOP_BUTTON
};

class ElevatorGraphics: public ElevatorObserver
{
public:
	ElevatorGraphics(Elevator& elevator);
	~ElevatorGraphics();
	void updateGraphics();
	void changingState(Elevator* elevator);
	void moveElevator(const float& dy, const bool& dir);
	void turnButtonLightOn(BUTTON &button);
	void turnButtonLightOff(BUTTON &button);


	//Constants
	static const int WINDOW_HEIGHT = 700;
	static const int WINDOW_WIDTH = 900;
	static const int NUMBER_OF_FLOORS = 4;
	static const int LINE_WIDTH = 10;
	const int FLOOR_LIGHT_RADIUS = 10;
	const float FLOOR_HEIGHT = WINDOW_HEIGHT / NUMBER_OF_FLOORS;
	const int UPDOWN_BUTTON_X_OFFSET_FROM_ELEVATOR = 50;
	const int UPDOWN_BUTTON_HEIGHT = FLOOR_HEIGHT / 5;
	const int UPDOWN_BUTTON_SPACING = 2;
	const float DOWN_BUTTON_Y_OFFSET_FROM_FLOOR = (FLOOR_HEIGHT-LINE_WIDTH)/2 - UPDOWN_BUTTON_SPACING / 2.0 ;
	const sf::Vector2f ELEVATOR_SIZE = { WINDOW_WIDTH / 6, FLOOR_HEIGHT };
	sf::RenderWindow window;
	void toggleButtonLight(const BUTTON &button, const bool value);
	void toggleFloorLight(const FLOOR floor);




private:
	void updateFloorText(Elevator* elevator);
	int lastFloorLamp = 0; //Which lamp to turn off when activating a new one

	//Initiate graphical elements
	void initiateText();
	void initiateUpDownButtons();
	void initiateFloorShapes();
	void initiateElevator();
	void initiateButtonsInElevator();
	void initiateFloorLights(); //Sensor lights
	void initiateStopLamp();

	//Drawables
	std::vector<sf::ConvexShape> upDownButtons; //u1,u2,u3,d2,d3,d4
	std::vector<sf::CircleShape> buttonsInElevator;
	std::vector<sf::Text> textVector;
	std::vector<sf::RectangleShape> floorShapeVector;
	std::vector<sf::CircleShape> floorLights;
	sf::CircleShape stopLamp;
	Elevator* elevatorPtr;


	void setFontAndColor(sf::Text* text);
	sf::Font font;

	const std::string FONT_STRING = "assets\\arial.ttf";
	const int TEXT_SIZE = 22;
	const sf::Color BROWN_COLOR = { 161,64,5 };
	const sf::Color BUTTON_LIGHT_UP_COLOR = { 255, 255, 102 };
	const sf::Color BUTTON_COLOR = sf::Color::Yellow;
	const sf::Color ELEVATOR_COLOR = { 100, 100, 100, 255 };
};

