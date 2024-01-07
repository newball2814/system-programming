CC := gcc 
CFLAGS := -Wall -Wextra -std=c11

all: server client

server: ./server/main.c
	$(CC) $(CFLAGS) ./server/main.c -o ./serverc

client: ./client/main.c 
	$(CC) $(CFLAGS) ./client/main.c -o ./clientc

clean: 
	@rm serverc clientc

.PHONY: all
