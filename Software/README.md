# Laser Scanning Projector

## Prerequisites

You will be needing the following to compile:

- [arm-none-eabi-gcc](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm)
- [CMake](https://cmake.org/)
- [GNU Make](https://www.gnu.org/software/make/)
- [Python](https://www.python.org/)

## Usage

1. Clone this repository to your local system. 

```bash
git clone https://github.com/waicool20/LaserScanningProjector.git
```

2. Initialize all the submodule dependencies.

```bash
cd LaserScanningProjector
git submodule update --init --recursive
```

3. Build LibOpenCM3.

This step is required by RTLib to recognize which targets are supported by libopencm3. Subsequent builds can skip this 
step, as building RTLib will implicitly build libopencm3.

```bash
cd Software/libopencm3
make
cd ..
```
4. Try to build the application.

```bash
# Create a new folder to house all CMake-related files.
mkdir cmake-build && cd cmake-build

# Change the -G option according to your build system, might be different for example if you were using MinGW.
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-toolchain.cmake -G "CodeBlocks - Unix Makefiles" ..

# Finally run the make command to initate the build process. This will build everything into the "cmake-build" folder.
cmake --build . --target all
```

The resulting binaries (`LaserScanningProjector.bin` and `LaserScanningProjector.elf`) will be located in the same 
directory.
