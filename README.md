# York-Engine
![Ubuntu Build Support](https://img.shields.io/github/workflow/status/mxtt-mmxix/York-Engine/CMake?logo=ubuntu)
![BSD-2-Clause License](https://img.shields.io/github/license/mxtt-mmxix/York-Engine)
[![Documentation](https://img.shields.io/badge/Documentation-https%3A%2F%2Fyork.mmccall.dev-blue)](https://img.shields.io/badge/Documentation-https%3A%2F%2Fyork.mmccall.dev-blue)

A platform for developing realtime interactive 3D media.

## Building
### Dependencies
York-Engine uses [vcpkg](https://github.com/microsoft/vcpkg) to manage its dependencies. As a result, it is a matter of running `./utils/Setup.sh` then `./utils/InstallDependencies.sh` (or `.\utils\Setup.bat` and `.\utils\InstallDependencies.bat` on Windows respectively) for the end user. The following describes the projects used and some install notes. Please note that any redistributions of York-Engine must not only respect our [license](https://github.com/mxtt-mmxix/York-Engine/blob/main/LICENSE), but our dependencies' licenses as well.
#### LunarG Vulkan SDK
https://vulkan.lunarg.com/sdk/home

You may *optionally* install the tools included in the SDK instead of the SDK itself. See the LunarG documentation for more information. At bare minimum, York-Engine needs the [C++ Vulkan Headers](https://github.com/khronosgroup/vulkan-hpp), [Vulkan Loader](https://github.com/KhronosGroup/Vulkan-Loader), [Validation Layers](https://github.com/KhronosGroup/Vulkan-ValidationLayers), and any other development packages or drivers necessary if you were to opt out of installing the SDK.
#### SDL2
https://libsdl.org/ 

Version 2.0.18 or newer. On Windows, vcpkg should try to install the dynamic libraries for SDL2. This is necessary as dynamic linking prevents some issues that will occur with static linking. However on macOS and Linux, vcpkg will try to reference an existing installation of SDL2. As such, one needs to install the SDL2 dynamic libraries on macOS and Linux via their package manager (including Homebrew or MacPorts). If version 2.0.18 or newer is not available, SDL2 comes submoduled. In this case you must [build and install SDL2](https://wiki.libsdl.org/Installation#linuxunix) manually prior to running that startup scripts. (You may need Autotools)

#### libcurl
https://curl.se/libcurl/

Used to perform network requests. You may need to [install Perl to build OpenSSL](https://github.com/openssl/openssl/issues/13761#issuecomment-753686265) which is a dependency of curl, otherwise vcpkg installation may fail. 

### CMake
To build the York-Engine Library and Runtime: 
- Install SDL2 and Perl if not already installed.
- Run the setup script: `./utils/Setup.sh`.
- Install dependencies: `./utils/InstallDependencies.sh`.
- Use [CMake](https://cmake.org/) to generate the build files for the project: `cmake -B build .`. 
- Then, build the project: `cmake --build build`.
- To build the sample: `cmake --build build -- SampleLayer`.

#### CMake Options
##### BUILD_SHADERC
It has been observed on some platforms (so far Linux) that there may be issues with linking libshaderc. As such, set this option to `ON` (`-DLIBSHADERC=ON`) when invoking CMake. This will cause libshaderc to be built from source and linked to the Engine.

## Using
### Sample
A sample application is located in the [`/sample/`](https://github.com/mxtt-mmxix/York-Engine/tree/main/sample) folder. See that folder for a reference on how to develop a York-Engine client application.

### How it works
```
York-Engine     york::Layer ◀─ York-Engine
     │               │                     
     │               └─▶ class yourLayer ┐
     ▼                                   │
  Runtime ◀─ createLayer() ◀── Client ◀──┘
```
The York-Engine is a library that provides an API to make developing real-time, interactive programs easier. However, instead of requiring the client to build an executable linked to York-Engine, York-Engine provides its own Runtime. Thus, all the client needs to do is to inherit the `york::Layer` class, and define a function which returns a `york::Layer*`. This must be compiled a shared object and will be loaded by the Runtime. The Runtime will search for your function and create your Layer and manages platform initialization for you. A layer source file might look like the following:
```cpp
#include "york/York.hpp"

class SampleLayer : public york::Layer {
public:
    SampleLayer() {}

    void onAttach() override {}

    void onEvent(york::Event& e) override {}

    void onUpdate(float deltaTime) override {}

    void onRender() override {}

    void onDetach() override {}
};

extern "C" york::Layer* createLayer()
{
    return new SampleLayer();
}
```
It is important to note that your create function must not be name-mangled as to make it easier for the Runtime to search and call the function, hence the `extern "C"`. Otherwise, this is how client applications will be structured: A series of classes that implement `york::Layer`, each defining a create function and compiled into a separate standalone shared object. The name of the shared object, excluding any platform prefixes such as "lib" and file extension will be the name of the layer. For example, `libSampleLayer.so` will be treated as a layer with the name `SampleLayer`. 

#### Client.xml
The Runtime will search for and load layers from the current working directory only. However, the Runtime needs to know which layers its looking for. Thus, the Runtime expects each application to have a `Client.xml` manifest. This manifest contains the name of the client, the name of the layers, and the name of the create function. A `Client.xml` file might look like the following:
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<Client xmlns="https://mxtt-mmxix.github.io/York-Engine/"
        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
        xsi:schemaLocation="https://mxtt-mmxix.github.io/York-Engine/ https://raw.githubusercontent.com/mxtt-mmxix/York-Engine/runtime-impl/Client.xsd">
    <Name>SampleClient</Name>
    <Layers>
        <Layer>
            <Name>SampleLayer</Name>
            <CreateFunction>createLayer</CreateFunction>
        </Layer>
    </Layers>
</Client>
```
Note that the `<Name />` of the layer is platform independent, the Runtime will automatically add any prefixes and file extension to the name to find the layer. Also note that the XML above makes use of a Schema. You are encouraged to produce well-formed and valid XML for describing your application.

A collection of a `Client.xml` file, Layer shared object files, and assets are considered to be a York-Engine client application.
