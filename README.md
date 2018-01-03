# Engine

## Installing

Windows: just clone the repository.
Linux: TBA
OS-X: TBA

## Setting up

`cmake --H. -Bbuild`

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

## Credits

- [SDL](https://www.libsdl.org/)
- [LazyFoo](http://www.lazyfoo.net/tutorials/SDL/index.php)
