# USDTgVerse Bonus System Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
TARGET = bonus_api_server

all: $(TARGET)

$(TARGET): bonus_api_server.c bonus_manager.c
	$(CC) $(CFLAGS) -o $(TARGET) bonus_api_server.c bonus_manager.c

clean:
	rm -f $(TARGET)

.PHONY: all clean

