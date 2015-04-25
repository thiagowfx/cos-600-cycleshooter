cycleshooter
============

- Bullet (for physics)
- Google Test (for unit testing)
- Ogre3d (for rendering)
- SFML

Tutorial
========

To use this repo, do these steps:

- `git clone https://github.com/thiagowfx/cg-games-2015-1`
- `cd ogre-bullet`
- `mkdir build; cd build`
- `cmake ..`
- `make`

This will check if you have bullet and ogre properly set up.
If not, then they will be automatically downloaded and compiled (by default, to $HOME/.lib).

Now, set the `DEPENDENCIES_READY` variable to `TRUE`. There are at least three ways to do that, so choose the most appropriate one for you:

- `cmake-gui .`
- `ccmake .`
- `cmake -DDEPENDENCIES_READY=TRUE ..`

Finally, just build your project:

- `make`

By default, this will place binaries under `<project_build_root>/dest/bin` and update assets on `<project_build_root>/dest`.

Additional custom targets:

- `make run` - to run the main executable of the project
- `make run_tests` - to run the unit tests created with google test

Dependencies
============

If you want to build Bullet, Ogre or SFML, you must have some dependencies already installed on your system:

On Ubuntu:
- `sudo apt-get build-dep libbullet-dev libogre-1.9-dev libsfml-dev; sudo apt-get install libglew-dev libudev-dev`

On Arch Linux:
- `sudo pacman -S $(expac -S "%E" bullet ogre sfml)`

How to use each library
=======================

Google Test
-----------
Set the `BUILD_TESTS` cmake variable to `TRUE`.

Bullet
------
Use `find_package(Bullet REQUIRED)`.

Ogre
----
Use `find_package(OGRE REQUIRED)`.

SFML
----
Use `find_package(SFML REQUIRED)`.
