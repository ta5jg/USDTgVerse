CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LDFLAGS = -L. -lzk_privacy

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

SOURCES = $(SRC_DIR)/zk_privacy.c
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

TARGET_LIB = $(BUILD_DIR)/libzk_privacy.a
TARGET_TEST = $(BIN_DIR)/zk_test

# Test application source
TEST_SRC = test/zk_test_app.c
TEST_OBJ = $(BUILD_DIR)/zk_test_app.o

.PHONY: all clean test

all: dirs $(TARGET_LIB) $(TARGET_TEST)

dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(INCLUDE_DIR)
	@mkdir -p $(SRC_DIR)
	@mkdir -p test

$(TARGET_LIB): $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	cp $(INCLUDE_DIR)/zk_privacy.h $(BUILD_DIR)/.

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_TEST): $(TEST_OBJ) $(TARGET_LIB)
	$(CC) $(CFLAGS) $< -o $@ -L$(BUILD_DIR) -lzk_privacy

$(BUILD_DIR)/zk_test_app.o: test/zk_test_app.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET_TEST)
	$(TARGET_TEST)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	rm -f $(SRC_DIR)/*.o test/*.o

install: $(TARGET_LIB)
	@echo "Installing Zero-Knowledge Privacy Layer System..."
	@mkdir -p /usr/local/include/usdtgverse
	@mkdir -p /usr/local/lib
	@cp $(INCLUDE_DIR)/zk_privacy.h /usr/local/include/usdtgverse/
	@cp $(TARGET_LIB) /usr/local/lib/
	@echo "Installation complete!"

uninstall:
	@echo "Uninstalling Zero-Knowledge Privacy Layer System..."
	@rm -f /usr/local/include/usdtgverse/zk_privacy.h
	@rm -f /usr/local/lib/libzk_privacy.a
	@echo "Uninstallation complete!"
