#include "Elevator.h"


//New elevator, floor 1
Elevator::Elevator(): Elevator(THIRD)
{
}

Elevator::Elevator(FLOOR floor) : currentFloor(floor), 
	targetFloor(floor), currentDirection(NOT_MOVING_ELEV), doorIsOpen(false)
{
}

const FLOOR &Elevator::getFloor() const
{
	return currentFloor;
}


void Elevator::arriveAtFloor(FLOOR floor)
{

	if (currentFloor != floor)
	{
		std::cout << "Arrived at " << floor << ". floor." << std::endl;
		setDirection(NOT_MOVING_ELEV);
		currentFloor = floor;
		notifyObserver();
	}

}

void Elevator::leaveFloor()
{
	currentFloor = BETWEEN_FLOORS;
	notifyObserver();

}

void Elevator::openDoor() {
	doorIsOpen = true;
	if (currentFloor == BETWEEN_FLOORS)
	{
		std::cout << "WARNING: Opened door between floors!" << std::endl;
	}
	notifyObserver();

}

void Elevator::closeDoor() {
	doorIsOpen = false;
	notifyObserver();

}


void Elevator::stopElevator()
{
	currentDirection = NOT_MOVING_ELEV;
}

void Elevator::setDirection(DIRECTION direction)
{
	currentDirection = direction;
	notifyObserver();

	std::cout << "Setting new direction: " << direction << std::endl;
}





const DIRECTION &Elevator::getDirection() const
{
	return currentDirection;
}


void Elevator::addObserver(ElevatorObserver * observer)
{
	observers.push_back(observer);
}

Elevator::~Elevator()
{
}

void Elevator::notifyObserver()
{
	for (auto &d : observers) {
		d->changingState(this);
	}
}

ElevatorObserver::ElevatorObserver()
{
}

ElevatorObserver::~ElevatorObserver()
{
}
