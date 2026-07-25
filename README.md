# microrvx

ultra lightweight Windows volume overlay.

a minimal alternative to 3RVX built with native Win32 APIs.

## features

- very low memory usage (~1 MB RAM)
- volume overlay when volume changes
- no installation required
- no external dependencies

## requirements

- Windows 7 SP1 or newer (tested on win10)

## usage

run:

MicroRVX.exe

default hotkeys:

- increase volume:
  Ctrl + Shift + +

- decrease volume:
  Ctrl + Shift + -

application runs in the background.

## build

requirements:

- Visual Studio Build Tools
- CMake

build:

```cmd
cmake -B build
cmake --build build --config Release
