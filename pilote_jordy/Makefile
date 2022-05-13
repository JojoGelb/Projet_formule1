
PILOT_NAME = ImaFastBoyyyyyy

MORE_FILE = AStarAlgorithm

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Wpedantic -g
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

#$(PILOT_NAME): $(PILOT_NAME).c
#$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)


$(PILOT_NAME): $(PILOT_NAME).o $(MORE_FILE).o
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

#ImaFastBoyyyyyy
$(PILOT_NAME).o: $(PILOT_NAME).c
	$(CC) $(CFLAGS) -I . $< -c -o $@

#AStarAlgorithm
$(MORE_FILE).o: $(MORE_FILE).c ./AStarAlgorithm_pilote_jordy.h
	$(CC) $(CFLAGS) -I ./ $< -c -o $@


distclean: clean
	rm -f $(PILOT_NAME)

clean:
	rm -f ./*~ ./*.o
