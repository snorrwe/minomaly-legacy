# Minomaly Game Engine

- _master_ status: [![Build status](https://ci.appveyor.com/api/projects/status/1ix6lgn3objhh270/branch/master?svg=true)](https://ci.appveyor.com/project/snorrwe/minomaly/branch/master)

## Requirements

### General 

- [CMake](https://cmake.org/) 2.8+
- C++17

### Platform specific

- Windows:
    - Windows SDK 10.0.14393.0
    - See also: [How to set up SDL with Visual Studio 2017](https://www.wikihow.com/Set-Up-SDL-with-Visual-Studio-2017). _Part 1 helps getting started, the rest of the dependencies are already in contained by the project. There's no need for additional Setup!_
- Linux: TBA
- OS X: TBA

## Installing

- Windows:
    - Clone the repository. `https://github.com/snorrwe/minomaly.git`
    - Move to Setup
- Linux: TBA
- OS X: TBA

## Setup

- Windows:
    - Init CMake. `cmake -H. -Bbuild`
    - You're all set!
- Linux: TBA
- OS X: TBA

## Building

`cmake --build build`

## Usage

> __The project is in an extremely early stage. I would discourage anyone from using it for anything serious at this time.__

## Project Structure

```
examples/
    assets/     # example assets go here
    src/        # example sources go here

lib/            # external dependencies

src/
    common/     # common, not directly engine related stuff
    engine/     # engine code goes here
```

## Adding new project

Example projects can be added by the `generate_example.py` script.

`python generate_example.py --name my-super-awesome-project`

Run _CMake Configure_ and you're all set!

If you do not wish to use the script simply copy the provided `__template` project and rename it appropriately.

## Releases

__TBA__

## Special thanks to

- [SDL](https://www.libsdl.org/)
- [LazyFoo](http://www.lazyfoo.net/tutorials/SDL/index.php)
- [LLVM](https://llvm.org/)
- [CMake](https://cmake.org/)
