cycleshooter
============

This project uses the following C++ libraries:

- Ogre3d (for rendering)
- SFML (misc)

Tutorial
========

To build the project:

- `cd <project_root>`
- `mkdir build; cd build`
- `cmake ..`
- `make`

This will check if you have the dependencies already set up.
If not, then they will be automatically downloaded, compiled and installed (by default, to `$HOME/.lib`).

After setting up the dependencies, set the `DEPENDENCIES_READY` variable to `TRUE`:

- `cmake -DDEPENDENCIES_READY=TRUE ..`

Or, alternatively:

- `[ccmake|cmake-gui] .`

Finally, just build your project:

- `make -j$(nproc) -l$(nproc)`

By default, this will place binaries under `<project_build_root>/dest/bin` and update assets on `<project_build_root>/dest`.

Additional custom targets:

- `make run` - a shortcut to run the main executable of the project

Dependencies
============

If you want to build Ogre or SFML, you must have some dependencies already installed on your system:

On Debian/Ubuntu:
- `sudo apt-get build-dep libogre-1.9-dev libsfml-dev; sudo apt-get install cmake libglew-dev libudev-dev`

On Arch Linux:
- `sudo pacman -S $(expac -S "%E" ogre sfml)`
