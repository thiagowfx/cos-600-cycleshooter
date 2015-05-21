cycleshooter
============

- Ogre3d (for rendering)
- SFML (misc)

Tutorial
========

To use this repo, do these steps:

- `git clone https://github.com/thiagowfx/cycleshooter`
- `cd cycleshooter`
- `mkdir build; cd build`
- `cmake ..`
- `make`

This will check if you have the dependencies already set up.
If not, then they will be automatically downloaded, compiled and installed (by default, to $HOME/.lib).

Now, set the `DEPENDENCIES_READY` variable to `TRUE`. There are at least three ways to do that, so choose the most appropriate one for you:

- `cmake-gui .`
- `ccmake .`
- `cmake -DDEPENDENCIES_READY=TRUE ..`

Finally, just build your project:

- `make`

By default, this will place binaries under `<project_build_root>/dest/bin` and update assets on `<project_build_root>/dest`.

Additional custom targets:

- `make run` - to run the main executable of the project

Dependencies
============

If you want to build Ogre or SFML, you must have some dependencies already installed on your system:

On Ubuntu:
- `sudo apt-get build-dep libogre-1.9-dev libsfml-dev; sudo apt-get install cmake libglew-dev libudev-dev`

On Arch Linux:
- `sudo pacman -S $(expac -S "%E" ogre sfml)`

How to use each library
=======================

Ogre
----
Use `find_package(OGRE REQUIRED)`.

SFML
----
Use `find_package(SFML REQUIRED)`.
