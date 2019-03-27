.DEFAULT_GOAL := sfml-app
CC:=g++
LIBS:=-lsfml-graphics -lsfml-window -lsfml-system

SFML_OBJ := Elevator.o ElevatorGraphics.o Simulator.o elev.o fsm.o main.o 

.PHONY: clean
clean:
	rm -f *.o

all: sfml-app

Elevator.o: Elevator.cpp
	g++ -c "Elevator.cpp" -o Elevator.o

ElevatorGraphics.o: ElevatorGraphics.cpp
	g++ -c "ElevatorGraphics.cpp" -o ElevatorGraphics.o

Simulator.o: Simulator.cpp
	g++ -c "Simulator.cpp" -o Simulator.o

elev.o: elev.cpp
	g++ -c "elev.cpp" -o elev.o

#Note how we underneath use the gcc compiler to get the .o file as this is a C-file.
fsm.o: fsm.c
	gcc -c "fsm.c" -o fsm.o

sfml-app: $(SFML_OBJ)
	$(CC) -o $@ $(SFML_OBJ) $(LIBS)

