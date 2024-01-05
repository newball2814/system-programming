CC = g++
CFLAGS = -Wall -Wextra -std=c++11
TARGET = listps
SRC_DIR = src
SRCS = main.cpp

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	@rm -f $(TARGET)
