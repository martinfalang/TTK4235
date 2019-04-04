#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Elevator;

class ElevatorObserver
{
public:
	ElevatorObserver();
	virtual ~ElevatorObserver();
	virtual void changingState(Elevator* elevator) = 0;
	ElevatorObserver(const ElevatorObserver& other) = delete;
	ElevatorObserver& operator=(const ElevatorObserver& other) = delete;
};


enum FLOOR {
	FIRST, SECOND, THIRD, FOURTH, BETWEEN_FLOORS
};

enum DIRECTION {
	UP_ELEV, DOWN_ELEV, NOT_MOVING_ELEV
};

class Elevator: public sf::RectangleShape
{
public:
	Elevator();
	Elevator(FLOOR floor);
	const FLOOR& getFloor() const;
	void arriveAtFloor(FLOOR floor);
	void leaveFloor();
	void openDoor();
	void closeDoor();
	void moveElevator(const float& dy, const bool& dir);
	void stopElevator();
	void setDirection(DIRECTION direction);
	const DIRECTION& getDirection() const;

	//Listen
	void addObserver(ElevatorObserver* observer);
	~Elevator();
private:
	std::vector<ElevatorObserver*> observers;
	void notifyObserver();
	DIRECTION currentDirection;
	FLOOR currentFloor;
	FLOOR targetFloor;
	bool doorIsOpen;
};

