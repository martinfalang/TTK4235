.DEFAULT_GOAL := sfml-app
CC:=g++
LIBS:=-lsfml-graphics -lsfml-window -lsfml-system

SFML_OBJ := Elevator.o ElevatorGraphics.o Simulator.o elev.o fsm.o main.o 

.PHONY: clean
clean:
	rm -f *.o

all: sfml-app

Elevator.o: Elevator.cpp
	g++ -g3 -c "Elevator.cpp" -o Elevator.o

ElevatorGraphics.o: ElevatorGraphics.cpp
	g++ -g3 -c "ElevatorGraphics.cpp" -o ElevatorGraphics.o

Simulator.o: Simulator.cpp
	g++ -g3 -c "Simulator.cpp" -o Simulator.o

elev.o: elev.cpp
	g++ -g3 -c "elev.cpp" -o elev.o

#Note how we underneath use the gcc compiler to get the .o file as this is a C-file.
fsm.o: fsm.cpp
	g++ -g3 -c "fsm.cpp" -o fsm.o

sfml-app: $(SFML_OBJ)
	$(CC) -g3 -o $@ $(SFML_OBJ) $(LIBS)

