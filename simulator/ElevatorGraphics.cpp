#include "ElevatorGraphics.h"



ElevatorGraphics::ElevatorGraphics(Elevator& elevator):
	window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Elevator Simulator")
{
	elevatorPtr = &elevator;
	initiateElevator();
	font.loadFromFile(FONT_STRING);
	initiateText();
	initiateFloorShapes();
	initiateButtonsInElevator();
	initiateUpDownButtons();
	initiateFloorLights();
	initiateStopLamp();
}


void ElevatorGraphics::changingState(Elevator * elevator)
{
	updateFloorText(elevator);
}




void ElevatorGraphics::toggleButtonLight(const BUTTON &button, const bool value)
{
	int buttonIndex = static_cast<int>(button);
	switch (button)
	{
	case E1:
	case E2:
	case E3:
	case E4:
		if (value) {
			buttonsInElevator[buttonIndex].setFillColor(BUTTON_LIGHT_UP_COLOR);
		}
		else {
			buttonsInElevator[buttonIndex].setFillColor(sf::Color::Transparent);
		}
		break;
	case D2:
	case D3:
	case D4:
	case U1:
	case U2:
	case U3:
		if (value){
			upDownButtons[buttonIndex-4].setFillColor(BUTTON_LIGHT_UP_COLOR);
		}
		else {
			upDownButtons[buttonIndex-4].setFillColor(sf::Color::Transparent);
		}
		break;
	case STOP_BUTTON:
		if (value)
		{
			stopLamp.setFillColor(sf::Color::Red);
		}
		else {
			stopLamp.setFillColor(sf::Color::Transparent);
		}
	default:
		break;
	}
}

void ElevatorGraphics::toggleFloorLight(const FLOOR floor)
{
	floorLights[lastFloorLamp].setFillColor(sf::Color::Transparent);
	floorLights[floor].setFillColor(BUTTON_LIGHT_UP_COLOR);
	lastFloorLamp = floor;
}


void ElevatorGraphics::updateFloorText(Elevator* elevator)
{
	std::string floor;
	FLOOR f = elevatorPtr->getFloor();
	switch (f)
	{
	case FIRST:
		floor = "First";
		break;
	case SECOND:
		floor = "Second";
		break;
	case THIRD:
		floor = "Third";
		break;
	case FOURTH:
		floor = "Fourth";
		break;
	case BETWEEN_FLOORS:
		floor = "Between floors";
		break;
	default:
		break;
	}
	textVector[0].setString("Current floor: " + floor);
}

void ElevatorGraphics::initiateText()
{
	sf::Text floorText;
	setFontAndColor(&floorText);
	floorText.setString("Current floor: First");
	textVector.push_back(floorText);
}

void ElevatorGraphics::initiateUpDownButtons()
{
	std::vector<sf::ConvexShape> temp; //for getting buttons in right order
	int xposLeft = UPDOWN_BUTTON_X_OFFSET_FROM_ELEVATOR + elevatorPtr->getPosition().x + elevatorPtr->getSize().x;


	for (int i = 0; i < NUMBER_OF_FLOORS*2-2; i++)
	{
		//decide floor
		int floor = 0;
		if (i==1 || i==2)
		{
			floor = 1;
		}
		if (i==4 || i == 3)
		{
			floor = 2;
		}
		if (i == 5)
		{
			floor = 3;
		}
		sf::ConvexShape tri;
		tri.setPointCount(3);
		tri.setOutlineColor(BUTTON_COLOR);
		tri.setFillColor(sf::Color::Transparent);
		tri.setOutlineThickness(-1);

		tri.setPoint(0, sf::Vector2f(0, 0));
		tri.setPoint(1, sf::Vector2f(UPDOWN_BUTTON_HEIGHT*0.8, 0));
		tri.setPoint(2, sf::Vector2f(UPDOWN_BUTTON_HEIGHT*0.4, UPDOWN_BUTTON_HEIGHT));
		tri.setPosition(xposLeft, WINDOW_HEIGHT-(floor*FLOOR_HEIGHT+ DOWN_BUTTON_Y_OFFSET_FROM_FLOOR));

		//up
		if (i%2==0)
		{
			tri.move(0, UPDOWN_BUTTON_SPACING+UPDOWN_BUTTON_HEIGHT);
			temp.push_back(tri);
		}
		else {
			tri.rotate(180);
			tri.move(UPDOWN_BUTTON_HEIGHT*0.8, 0);
			upDownButtons.push_back(tri);
		}
	}
	for (int i = 0; i <3; i++)
	{
		upDownButtons.push_back(temp[i]);
	}
	
}

void ElevatorGraphics::initiateFloorShapes()
{
	float xPos = elevatorPtr->getSize().x+elevatorPtr->getPosition().x+3;
	for (int i = 1; i < 5; i++)
	{
		sf::RectangleShape s;
		s.setFillColor(BROWN_COLOR);
		s.setPosition(xPos, elevatorPtr->getSize().y*i - LINE_WIDTH);
		s.setSize(sf::Vector2f(WINDOW_WIDTH - xPos, LINE_WIDTH));
		floorShapeVector.push_back(s);
	}
}

void ElevatorGraphics::initiateElevator()
{
	elevatorPtr->setSize(ELEVATOR_SIZE);
	elevatorPtr->setFillColor(sf::Color::Transparent);
	elevatorPtr->setOutlineColor(ELEVATOR_COLOR);
	elevatorPtr->setOutlineThickness(-LINE_WIDTH);
	elevatorPtr->move(ELEVATOR_SIZE.x * 2, WINDOW_HEIGHT - FLOOR_HEIGHT);
	elevatorPtr->addObserver(this);
}


void ElevatorGraphics::setFontAndColor(sf::Text* text)
{
	text->setFont(font);
	text->setFillColor(sf::Color::White);
	text->setCharacterSize(TEXT_SIZE);
}


ElevatorGraphics::~ElevatorGraphics()
{
}

void ElevatorGraphics::initiateButtonsInElevator() {;
	sf::Vector2f elevatorSize = elevatorPtr->getSize();
	sf::Vector2f elevatorPos = elevatorPtr->getPosition();
	float radius = (elevatorSize.x - 2 * LINE_WIDTH) / 12;
	float xPos = elevatorPos.x + LINE_WIDTH + 1;
	float dy = (elevatorSize.y - 2 * LINE_WIDTH + 10) / 4;
	for (int i = 0; i < 4; i++)
	{
		float yPos = 1 + elevatorPos.y + LINE_WIDTH + (3-i)*dy;
		sf::CircleShape s;
		s.setRadius(radius);
		s.setOutlineThickness(-1);
		s.setPosition(xPos, yPos);
		s.setOutlineColor(BUTTON_COLOR);
		s.setFillColor(sf::Color::Transparent);
		buttonsInElevator.push_back(s);
	}

}

void ElevatorGraphics::initiateFloorLights()
{
	const double xPos = WINDOW_WIDTH*(3 / 4.0);
	const double yStartPos = FLOOR_HEIGHT / 2.0;
	for (int i = 0; i < NUMBER_OF_FLOORS; i++)
	{
		double yPos = WINDOW_HEIGHT - (yStartPos + i*FLOOR_HEIGHT);
		sf::CircleShape c;
		c.setRadius(FLOOR_LIGHT_RADIUS);
		c.setOutlineThickness(-1);
		c.setOutlineColor(BUTTON_COLOR);
		c.setFillColor(sf::Color::Transparent);
		c.setPosition(xPos, yPos);
		floorLights.push_back(c);
	}
}

void ElevatorGraphics::initiateStopLamp()
{
	double xPos = WINDOW_WIDTH / 5.0;
	double yPos = WINDOW_HEIGHT / 2.0;
	sf::CircleShape c;
	c.setRadius(20);
	c.setFillColor(sf::Color::Transparent);
	c.setOutlineThickness(-1);
	c.setOutlineColor(sf::Color::Red);
	c.setPosition(xPos, yPos);
	stopLamp = c;
}

//Move elevator in current direction, dy> 0
void ElevatorGraphics::moveElevator(const float& dy, const bool& dir)
{

	int dirSign = 1;
	if (!dir)
	{
		dirSign = -1;
	}

	elevatorPtr->move(0, dirSign*dy);
	for (auto &d : buttonsInElevator) {
		d.move(0, dirSign*dy);
	}
}

void ElevatorGraphics::turnButtonLightOn(BUTTON & button)
{
	toggleButtonLight(button, false);

}

void ElevatorGraphics::turnButtonLightOff(BUTTON & button)
{
	toggleButtonLight(button, true);
}

void ElevatorGraphics::updateGraphics()
{
	
	window.clear();
	window.draw(textVector[0]);
	window.draw(*elevatorPtr);
	for (auto &d : floorShapeVector) {
		window.draw(d);
	}
	for (auto &d : buttonsInElevator) {
		window.draw(d);
	}
	for (auto &d : upDownButtons)
	{
		window.draw(d);
	}

	for (auto &d : floorLights) {
		window.draw(d);
	}

	window.draw(stopLamp);


	window.display();
}
