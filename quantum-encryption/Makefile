# USDTgVerse Quantum-Safe Encryption Makefile
# Author: Irfan Gedik
# Version: 1.0

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -D_GNU_SOURCE
LDFLAGS = -lm
TARGET = quantum_encryption
SOURCES = quantum_encryption_library.c

# Default target
all: $(TARGET)

# Build quantum encryption
$(TARGET): $(SOURCES)
	@echo "🔐 Building quantum-safe encryption system..."
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)
	@echo "✅ Quantum encryption system built successfully"

# Generate quantum keypair
generate-key: $(TARGET)
	@echo "🔑 Generating quantum keypair..."
	./$(TARGET) generate-key quantum.key
	@echo "✅ Quantum keypair generated"

# Encrypt GitHub repository
encrypt-repo: $(TARGET)
	@echo "🔐 Encrypting GitHub repository..."
	./$(TARGET) encrypt-repo ../USDTgVerse
	@echo "✅ Repository encrypted with quantum-safe cryptography"

# Decrypt GitHub repository
decrypt-repo: $(TARGET)
	@echo "🔓 Decrypting GitHub repository..."
	./$(TARGET) decrypt-repo ../USDTgVerse.quantum
	@echo "✅ Repository decrypted successfully"

# Encrypt single file
encrypt-file: $(TARGET)
	@echo "🔐 Encrypting file..."
	@read -p "Enter input file: " input; \
	read -p "Enter output file: " output; \
	./$(TARGET) encrypt-file "$$input" "$$output"

# Decrypt single file
decrypt-file: $(TARGET)
	@echo "🔓 Decrypting file..."
	@read -p "Enter input file: " input; \
	read -p "Enter output file: " output; \
	./$(TARGET) decrypt-file "$$input" "$$output"

# Test quantum encryption
test: $(TARGET)
	@echo "🧪 Testing quantum encryption system..."
	@echo "Creating test file..."
	echo "This is a test file for quantum encryption" > test.txt
	@echo "Encrypting test file..."
	./$(TARGET) encrypt-file test.txt test.txt.quantum
	@echo "Decrypting test file..."
	./$(TARGET) decrypt-file test.txt.quantum test_decrypted.txt
	@echo "Comparing files..."
	@if diff test.txt test_decrypted.txt > /dev/null; then \
		echo "✅ Test passed - files match"; \
	else \
		echo "❌ Test failed - files don't match"; \
	fi
	@rm -f test.txt test.txt.quantum test_decrypted.txt

# Clean build files
clean:
	@echo "🧹 Cleaning build files..."
	rm -f $(TARGET)
	rm -f quantum.key
	rm -f *.quantum
	@echo "✅ Clean completed"

# Install dependencies
install-deps:
	@echo "📦 Installing dependencies..."
	@echo "✅ No external dependencies required (Pure C)"

# Show help
help:
	@echo "USDTgVerse Quantum-Safe Encryption System Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all              - Build quantum encryption system"
	@echo "  generate-key     - Generate quantum keypair"
	@echo "  encrypt-repo     - Encrypt GitHub repository"
	@echo "  decrypt-repo     - Decrypt GitHub repository"
	@echo "  encrypt-file     - Encrypt single file (interactive)"
	@echo "  decrypt-file     - Decrypt single file (interactive)"
	@echo "  test             - Test quantum encryption system"
	@echo "  clean            - Clean build files"
	@echo "  install-deps     - Install dependencies"
	@echo "  help             - Show this help"
	@echo ""
	@echo "Examples:"
	@echo "  make all         - Build the system"
	@echo "  make generate-key - Generate quantum keypair"
	@echo "  make encrypt-repo - Encrypt entire repository"
	@echo "  make test        - Test the encryption system"

.PHONY: all generate-key encrypt-repo decrypt-repo encrypt-file decrypt-file test clean install-deps help
