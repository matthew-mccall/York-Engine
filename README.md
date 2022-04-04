# York-Engine

<div>
<a href="https://mxtt-mmxix.github.io/York-Engine">
<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-file-text" viewBox="0 0 16 16">
<path d="M5 4a.5.5 0 0 0 0 1h6a.5.5 0 0 0 0-1H5zm-.5 2.5A.5.5 0 0 1 5 6h6a.5.5 0 0 1 0 1H5a.5.5 0 0 1-.5-.5zM5 8a.5.5 0 0 0 0 1h6a.5.5 0 0 0 0-1H5zm0 2a.5.5 0 0 0 0 1h3a.5.5 0 0 0 0-1H5z"/>
<path d="M2 2a2 2 0 0 1 2-2h8a2 2 0 0 1 2 2v12a2 2 0 0 1-2 2H4a2 2 0 0 1-2-2V2zm10-1H4a1 1 0 0 0-1 1v12a1 1 0 0 0 1 1h8a1 1 0 0 0 1-1V2a1 1 0 0 0-1-1z"/>
</svg> Documentation</a>
</div>
<div>
<a href="https://github.com/mxtt-mmxix/York-Engine"><svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-github" viewBox="0 0 16 16">
  <path d="M8 0C3.58 0 0 3.58 0 8c0 3.54 2.29 6.53 5.47 7.59.4.07.55-.17.55-.38 0-.19-.01-.82-.01-1.49-2.01.37-2.53-.49-2.69-.94-.09-.23-.48-.94-.82-1.13-.28-.15-.68-.52-.01-.53.63-.01 1.08.58 1.23.82.72 1.21 1.87.87 2.33.66.07-.52.28-.87.51-1.07-1.78-.2-3.64-.89-3.64-3.95 0-.87.31-1.59.82-2.15-.08-.2-.36-1.02.08-2.12 0 0 .67-.21 2.2.82.64-.18 1.32-.27 2-.27.68 0 1.36.09 2 .27 1.53-1.04 2.2-.82 2.2-.82.44 1.1.16 1.92.08 2.12.51.56.82 1.27.82 2.15 0 3.07-1.87 3.75-3.65 3.95.29.25.54.73.54 1.48 0 1.07-.01 1.93-.01 2.2 0 .21.15.46.55.38A8.012 8.012 0 0 0 16 8c0-4.42-3.58-8-8-8z"/>
</svg> GitHub</a>
</div>

![workflow](https://github.com/mxtt-mmxix/York-Engine/actions/workflows/cmake.yml/badge.svg) 

A platform for developing realtime interactive 3D media.

## Building
### Dependencies
#### LunarG Vulkan SDK
https://vulkan.lunarg.com/sdk/home

You may *optionally* install the tools included in the SDK instead of the SDK itself. See the LunarG documentation for more information. At bare minimum, York-Engine needs the [C++ Vulkan Headers](https://github.com/khronosgroup/vulkan-hpp), [Vulkan Loader](https://github.com/KhronosGroup/Vulkan-Loader), and any other development packages provided if you were to opt out of installing the SDK.
#### SDL2
https://libsdl.org/ 

Version 2.0.18 or newer. If CMake could not automatically find and link to a suitable local copy, it will build SDL2 from source. You can specify `SDL_LIBRARY` and `SDL_INCLUDE_DIR` to assist CMake in finding SDL2.
#### Apache Xerces-C++
https://xerces.apache.org/xerces-c/

Version 3.2.3 or newer. If CMake could not automatically find and link to a suitable local copy, it will build Xerces-C++ from source.
#### libcurl
https://curl.se/libcurl/

Used to perform network requests.
#### In-Tree Dependencies
York-Engine includes source copies third-party dependencies in the `/libs/` folder. This makes dependency management simple as we can just simply build what we need from source without worrying about how and where to search for local installations. Libraries that are shipped with York-Engine include [{fmt}](https://fmt.dev/latest/index.html), [curlpp](https://www.curlpp.org/), [shaderc](https://github.com/google/shaderc), and [taskflow](https://taskflow.github.io/). Please note that any redistributions of York-Engine must not only respect our [license](https://github.com/mxtt-mmxix/York-Engine/blob/main/LICENSE), but our dependencies' licenses as well.

### CMake
To build the York-Engine Library and Runtime: 
- Run the setup script: `./utils/Setup.sh`. (Make sure submodules are cloned recursively)
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