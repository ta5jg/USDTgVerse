# USDTgVerse Airdrop System Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
TARGET = airdrop_api_server

all: $(TARGET)

$(TARGET): airdrop_api_server.c airdrop_manager.c
	$(CC) $(CFLAGS) -o $(TARGET) airdrop_api_server.c airdrop_manager.c

clean:
	rm -f $(TARGET)

.PHONY: all clean

