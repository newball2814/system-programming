CC = g++
CFLAGS = -Wall -Wextra -std=c11

DROPPER_SRC = dropper_linux.cpp

DROPPER_BIN = dropper

all: $(DROPPER_BIN)

$(DROPPER_BIN): $(DROPPER_SRC)
	$(CC) $(CFLAGS) -o $(DROPPER_BIN) $(DROPPER_SRC)

clean:
	@rm -f $(DROPPER)
