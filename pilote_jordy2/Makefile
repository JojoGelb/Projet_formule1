
PILOT_NAME = VelocityPilote

MORE_FILE = AStarAlgorithm

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Wpedantic -g -O3
LDLIBS = -lm

SANITIZE=off
ifeq ($(SANITIZE),on)
CFLAGS += -fsanitize=address
endif

.PHONY: all clean distclean install try

DRIVERS_PATH = ../drivers

all: $(PILOT_NAME)

install: $(DRIVERS_PATH)/$(PILOT_NAME)

try: install
	cd .. ; ./GrandPrix

$(DRIVERS_PATH)/$(PILOT_NAME): $(PILOT_NAME)
	cp $(PILOT_NAME) $(DRIVERS_PATH)

$(PILOT_NAME): $(PILOT_NAME).o $(MORE_FILE).o follow_line.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

#VelocityPilote
$(PILOT_NAME).o: $(PILOT_NAME).c
	$(CC) $(CFLAGS) -I . $< -c -o $@

#follow_line
follow_line.o: ../follow_line/follow_line.c ../follow_line/follow_line.h
	$(CC) $(CFLAGS) -I ../follow_line $< -c -o $@

#AStarAlgorithm
$(MORE_FILE).o: $(MORE_FILE).c ./$(MORE_FILE).h
	$(CC) $(CFLAGS) -I ./ $< -c -o $@


distclean: clean
	rm -f $(PILOT_NAME)

clean:
	rm -f ./*~ ./*.o
