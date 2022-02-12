# York-Engine
![workflow](https://github.com/mxtt-mmxix/York-Engine/actions/workflows/cmake.yml/badge.svg)

A realtime 3D platform for developing interactive media.

## Building
### Dependencies
#### [LunarG Vulkan SDK](https://vulkan.lunarg.com/sdk/home) 
You may *optionally* install the tools included in the SDK instead of the SDK itself. See the LunarG documentation for more information. At bare minimum, York-Engine needs the [C++ Vulkan Headers](https://github.com/khronosgroup/vulkan-hpp), [Vulkan Loader](https://github.com/KhronosGroup/Vulkan-Loader), and any other development packages provided if you were to opt out of installing the SDK.
#### [SDL2](https://libsdl.org/) 
Version 2.0.18 or newer. If CMake could not automatically find and link to a suitable local copy, it will build SDL2 from source. You can specify `SDL_LIBRARY` and `SDL_INCLUDE_DIR` to assist CMake in finding SDL2.
#### [Apache Xerces-C++](https://xerces.apache.org/xerces-c/)
Version 3.2.3 or newer. If CMake could not automatically find and link to a suitable local copy, it will build Xerces-C++ from source.
#### [libcurl](https://curl.se/libcurl/)
Used to perform network requests.

### CMake
To build the York Engine Library, Runtime, and Sample: 
- Run the setup script: `./utils/Setup.sh`.
- Use [CMake](https://cmake.org/) to generate the build files for the project: `cmake -B build .`. 
- Then, build the project: `cmake --build build`.

## Using
A sample application is located in the [`/sample/`](https://github.com/mxtt-mmxix/York-Engine/tree/main/sample) folder. See that folder for a reference on how to develop a York Engine client applicaition.
