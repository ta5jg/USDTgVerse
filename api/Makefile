CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lcurl

TARGET = static_price_updater

all: $(TARGET)

$(TARGET): static_price_updater.c
	$(CC) $(CFLAGS) -o $(TARGET) static_price_updater.c $(LDFLAGS)

clean:
	rm -f $(TARGET)

test: $(TARGET)
	./$(TARGET)

install:
	cp $(TARGET) /usr/local/bin/

.PHONY: all clean test install
