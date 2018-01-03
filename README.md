# Engine

## Installing

- Windows: just clone the repository.
- Linux: TBA
- OS-X: TBA

## Setting up

`cmake -H. -Bbuild`

## Building

`cmake --build build`

## Project Structure

```
assets/         # assets go here
src/
    common/     # common, not directly engine related stuff
    engine/     # engine code goes here
    game_logic/ # example applications go here
```

## CMake

Example projects should be added to the root CMakeLists.txt. 
You can use the provided CMakeLists.txt.template in the subprojects.

## Continous Integration

__TBA__

## Releases

__TBA__

## Credits

- [SDL](https://www.libsdl.org/)
- [LazyFoo](http://www.lazyfoo.net/tutorials/SDL/index.php)
