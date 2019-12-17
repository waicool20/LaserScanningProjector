# Laser Scanning Projector - PC Companion Software

Companion software for streaming image data to the Laser Scanning Projector.

## Getting Started

These instructions will set the project up and run the application on your 
local machine. Note that currently only the Linux platform is officially tested 
and supported.

### Prerequisites

- [CMake 3.10.0+](https://cmake.org/download/)
- Any C++17-supported compiler

### Compiling and Running

Run CMake in a separate directory.
```bash
cmake -G "CodeBlocks - Unix Makefiles" ..
cmake --build . --target all
```

The application will be named as `LaserScanningProject-PC`.

## Usage

```
LaserScanningProject-PC <image_file>
```

- `<image_file>`: Path to the image file to render
