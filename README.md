# Minomaly Game Engine

## Requirements

- [CMake](https://cmake.org/) 2.8+
- C++14

## Installing

- Windows:
    - Clone the repository. `https://github.com/snorrwe/minomaly.git`
    - Move to Setup
- Linux: TBA
- OS-X: TBA

## Setup

- Windows:
    - Init CMake. `cmake -H. -Bbuild`
    - You're all set, move to Setup
- Linux: TBA
- OS-X: TBA

## Building

`cmake --build build`

## Usage

__TBA__

## Project Structure

```
examples/       # example applications go here
    assets/     # example assets go here
    src/        # example sources go here
src/
    common/     # common, not directly engine related stuff
    engine/     # engine code goes here
```

## Adding new project

Example projects can be added by the `generate_example.py` script.

> python generate_example.py --name my-super-awesome-project

Run _CMake Configure_ and you're all set!

If you do not wish to use the script simply copy the provided `__template` project and rename it appropriately.

## CMake

You can use the provided CMakeLists.txt.template in the subprojects.

## Continous Integration

__TBA__

## Releases

__TBA__

## Special thanks to

- [SDL](https://www.libsdl.org/)
- [LazyFoo](http://www.lazyfoo.net/tutorials/SDL/index.php)
- [LLVM](https://llvm.org/)
- [CMake](https://cmake.org/)
