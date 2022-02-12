# York-Engine
![workflow](https://github.com/mxtt-mmxix/York-Engine/actions/workflows/cmake.yml/badge.svg)

A realtime 3D platform for developing interactive media.

## Building
### Dependencies
York Engine currently depends on:
- [LunarG Vulkan SDK](https://vulkan.lunarg.com/sdk/home) (or the tools included in the SDK)
- [SDL2](https://libsdl.org/) 2.0.18 or newer
- [Apache Xerces-C](https://xerces.apache.org/xerces-c/)
- [curl](https://curl.se/)

*If Apache and SDL2 are not found on your system, they will be built from source automatically.*

### CMake
To build the York Engine Library, Runtime, and Sample: 
- Use [CMake](https://cmake.org/) to generate the build files for the project: `cmake -B build .`. 
- Then, build the project: `cmake --build build`.

## Using
A sample application is located in the [`/sample/`](sample/) folder. See that folder for a reference on how to develop a York Engine client applicaition.
