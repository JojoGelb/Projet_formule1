FOLDER = pilote_jordy2
PILOT_NAME = VelocityPilote
MORE_FILE = AStarAlgorithm
OBJ = ./obj/
SRC = ./src/
HEADER = ./include/
BIN = ./bin/
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Wpedantic -g -O3 -I $(HEADER)
LDLIBS = -lm
DRIVERS_PATH = ../drivers

.PHONY: all install clean

all: $(BIN)$(PILOT_NAME) install clean

install: $(DRIVERS_PATH)/$(PILOT_NAME)

$(DRIVERS_PATH)/$(PILOT_NAME): $(BIN)$(PILOT_NAME)
	mv MemoryPilote $(DRIVERS_PATH)

$(BIN)$(PILOT_NAME): $(OBJ)$(PILOT_NAME).o $(OBJ)$(MORE_FILE).o $(OBJ)follow_line.o $(OBJ)Queue.o
	$(CC) $(CFLAGS) $^ -o MemoryPilote $(LDLIBS)

#VelocityPilote
$(OBJ)$(PILOT_NAME).o: $(SRC)$(PILOT_NAME).c
	$(CC) $(CFLAGS) -I . $< -c -o $@

#follow_line
$(OBJ)follow_line.o: ../follow_line/follow_line.c ../follow_line/follow_line.h
	$(CC) $(CFLAGS) -I ../follow_line $< -c -o $@

#AStarAlgorithm
$(OBJ)$(MORE_FILE).o: $(SRC)$(MORE_FILE).c $(HEADER)$(MORE_FILE).h
	$(CC) $(CFLAGS) -I ./ $< -c -o $@

#Queue
$(OBJ)Queue.o: $(SRC)Queue.c $(HEADER)Queue.h
	$(CC) $(CFLAGS) -I ./ $< -c -o $@

clean:
	rm $(OBJ)*
