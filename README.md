# VSCode Clone

A simple VSCode-like text editor built with C++ and GTK+ for Linux.

## Features

- Multiple tabs support
- Syntax highlighting
- Line numbers
- Current line highlighting
- Auto-indentation
- File open/save functionality
- Modern GTK+ interface

## Dependencies

- CMake (>= 3.10)
- C++17 compatible compiler
- GTK+ 3.0
- GTKSourceView 3.0
- pkg-config

## Building

1. Install dependencies:
```bash
sudo apt-get install build-essential cmake libgtkmm-3.0-dev libgtksourceviewmm-3.0-dev
```

2. Create build directory and build:
```bash
mkdir build
cd build
cmake ..
make
```

3. Run the editor:
```bash
./vscode_clone
```

## Usage

- File -> New: Create a new file
- File -> Open: Open an existing file
- File -> Save: Save the current file (TODO)
- File -> Save As: Save the current file with a new name (TODO)
- File -> Quit: Exit the application

## Contributing

Feel free to submit issues and enhancement requests! 