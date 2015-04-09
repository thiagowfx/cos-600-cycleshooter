cg-games-2015-1
===============

- Bullet
- Ogre3d
- Google Test

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

- `make install`

By default, this will place binaries under `<project_root>/dest/bin`.

Additional custom targets:

- `make run` - to run the main executable of the project
- `make run_tests` - to run the unit tests

Dependencies
============

If you want to build Ogre and/or Bullet, you must have the dependencies of those libraries already installed on your system.

On Ubuntu:
- `sudo apt-get build-dep libogre-1.9-dev libbullet-dev`

On Arch Linux:
- `sudo pacman -S $(expac -S "%E" bullet ogre)`

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
