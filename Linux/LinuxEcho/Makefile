CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SERVER_SRC = server/main.c
CLIENT_SRC = client/main.c

SERVER_BIN = serv
CLIENT_BIN = clnt

all: $(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_BIN) $(SERVER_SRC)

$(CLIENT_BIN): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_BIN) $(CLIENT_SRC)

clean:
	@rm -f $(SERVER_BIN) $(CLIENT_BIN)
