# USDTgVerse Native IDE

**Quantum-Safe Development Environment for USDTgScript**

## Overview

USDTgVerse Native IDE is a professional-grade Integrated Development Environment built in C with a modern web interface. It provides quantum-safe development capabilities for the USDTgVerse ecosystem.

## Features

### 🚀 Core Features
- **Native C Backend**: High-performance, low-resource usage
- **Web Interface**: Modern Monaco Editor with real-time collaboration
- **USDTgScript Support**: Full language support with syntax highlighting
- **Quantum-Safe**: Built-in quantum-resistant encryption and security
- **Real-time Communication**: WebSocket-based communication
- **File Management**: Complete project and file management
- **Compiler Integration**: Direct USDTgScript compiler integration
- **Terminal Integration**: Built-in terminal for command execution

### 🔧 Development Tools
- **Code Editor**: Monaco Editor with USDTgScript language support
- **Syntax Highlighting**: Custom USDTgScript syntax highlighting
- **Code Completion**: IntelliSense for USDTgScript
- **Error Detection**: Real-time error highlighting
- **Debugging**: Integrated debugging capabilities
- **Version Control**: Git integration
- **Project Management**: Complete project lifecycle management

### ⚛️ Quantum-Safe Features
- **Quantum Encryption**: AES-256-GCM encryption
- **Quantum Signatures**: Quantum-resistant digital signatures
- **Secure Communication**: Encrypted WebSocket communication
- **Key Management**: Secure key generation and storage
- **Audit Logging**: Comprehensive security audit trails

## Architecture

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Web Frontend  │    │   C Backend     │    │   USDTgScript   │
│   (HTML/CSS/JS) │◄──►│   (Native C)    │◄──►│   Compiler      │
│                 │    │                 │    │                 │
│ • Monaco Editor │    │ • IDE Core      │    │ • usdtgc        │
│ • File Manager  │    │ • File System   │    │ • Quantum Safe  │
│ • Terminal      │    │ • Process Mgmt  │    │ • HotStuff BFT  │
│ • Debug Panel   │    │ • WebSocket     │    │                 │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

## Installation

### Prerequisites

- **Operating System**: Linux (Ubuntu 20.04+ recommended)
- **Compiler**: GCC 9.0+
- **Dependencies**: libwebsockets, libjson-c, OpenSSL
- **USDTgScript Compiler**: usdtgc

### Quick Start

1. **Clone the repository**:
   ```bash
   git clone https://github.com/ta5jg/USDTgVerse.git
   cd USDTgVerse/native-ide
   ```

2. **Install dependencies**:
   ```bash
   make install-deps
   ```

3. **Build the IDE**:
   ```bash
   make all
   ```

4. **Run the IDE**:
   ```bash
   make run
   ```

5. **Access the web interface**:
   Open your browser and go to: `http://localhost:8000`

### Manual Installation

1. **Install system dependencies**:
   ```bash
   sudo apt-get update
   sudo apt-get install -y libwebsockets-dev libjson-c-dev build-essential
   ```

2. **Build the IDE**:
   ```bash
   make clean
   make all
   ```

3. **Install system-wide** (optional):
   ```bash
   sudo make install
   ```

## Usage

### Starting the IDE

```bash
# Start the IDE
make run

# Or start manually
./usdtgverse-ide &
cd web && python3 -m http.server 8000 &
```

### Web Interface

1. Open your browser
2. Navigate to `http://localhost:8000`
3. Start coding in USDTgScript!

### Command Line Options

```bash
./usdtgverse-ide [options]

Options:
  -p, --port PORT        WebSocket port (default: 8080)
  -w, --web-port PORT    Web server port (default: 8000)
  -d, --debug           Enable debug mode
  -h, --help            Show help message
```

### Project Management

```bash
# Create new project
./usdtgverse-ide --create-project /path/to/project

# Open existing project
./usdtgverse-ide --open-project /path/to/project

# List projects
./usdtgverse-ide --list-projects
```

## Development

### Building from Source

```bash
# Clone repository
git clone https://github.com/ta5jg/USDTgVerse.git
cd USDTgVerse/native-ide

# Install dependencies
make install-deps

# Build debug version
make debug

# Build release version
make all
```

### Project Structure

```
native-ide/
├── src/                    # Source files
│   ├── usdtgverse-ide-core.c
│   ├── file-manager.c
│   ├── compiler-integration.c
│   ├── websocket-handler.c
│   ├── quantum-safe.c
│   └── ide-utils.c
├── include/                # Header files
│   └── ide-core.h
├── web/                    # Web interface
│   └── index.html
├── scripts/                # Build scripts
│   └── build.sh
├── Makefile               # Build system
└── README.md              # This file
```

### Adding New Features

1. **Add source file**: Create new `.c` file in `src/`
2. **Add header**: Add declarations to `include/ide-core.h`
3. **Update Makefile**: Add new source to `SOURCES`
4. **Build and test**: Run `make clean && make all`

## Configuration

### IDE Settings

The IDE can be configured through environment variables:

```bash
export USDTG_IDE_PORT=8080          # WebSocket port
export USDTG_IDE_WEB_PORT=8000      # Web server port
export USDTG_IDE_DEBUG=1            # Enable debug mode
export USDTG_IDE_LOG_LEVEL=INFO     # Log level (DEBUG, INFO, ERROR)
```

### Compiler Settings

```bash
export USDTGC_PATH=/usr/local/bin/usdtgc    # Compiler path
export USDTGC_OPTIMIZATION=2                # Optimization level
export USDTGC_QUANTUM_SAFE=1                # Enable quantum safety
```

## API Reference

### WebSocket API

The IDE communicates with the web interface via WebSocket messages:

#### Message Types

- `save_file`: Save file content
- `load_file`: Load file content
- `compile`: Compile USDTgScript file
- `run`: Run USDTgScript file
- `get_files`: Get project file list
- `create_file`: Create new file
- `delete_file`: Delete file
- `terminal_command`: Execute terminal command

#### Example Messages

```json
// Save file
{
  "type": "save_file",
  "file": "contract.usdtg",
  "content": "contract MyContract { ... }"
}

// Compile file
{
  "type": "compile",
  "file": "contract.usdtg"
}

// Terminal command
{
  "type": "terminal_command",
  "command": "ls -la"
}
```

## Troubleshooting

### Common Issues

1. **Build fails with missing dependencies**:
   ```bash
   make install-deps
   ```

2. **WebSocket connection fails**:
   - Check if port 8080 is available
   - Verify firewall settings
   - Check IDE process is running

3. **Compiler not found**:
   - Install USDTgScript compiler
   - Set USDTGC_PATH environment variable

4. **Permission denied**:
   ```bash
   chmod +x usdtgverse-ide
   ```

### Debug Mode

Enable debug mode for detailed logging:

```bash
./usdtgverse-ide --debug
```

### Log Files

Logs are written to stdout. Redirect to file:

```bash
./usdtgverse-ide > ide.log 2>&1
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

### Code Style

- Use C99 standard
- Follow GNU coding standards
- Add header comments to all files
- Use meaningful variable names
- Add error handling

## License

MIT License - see LICENSE file for details.

## Support

- **Documentation**: [USDTgVerse Docs](https://docs.usdtgverse.com)
- **Issues**: [GitHub Issues](https://github.com/ta5jg/USDTgVerse/issues)
- **Community**: [USDTgVerse Discord](https://discord.gg/usdtgverse)

## Changelog

### v1.0.0 (2025-09-28)
- Initial release
- Native C backend
- Web interface with Monaco Editor
- USDTgScript support
- Quantum-safe encryption
- WebSocket communication
- File management
- Compiler integration

---

**USDTgVerse Native IDE** - Quantum-Safe Development for the Future
