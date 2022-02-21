# York-Engine
https://mxtt-mmxix.github.io/York-Engine

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
- Run the setup script: `./utils/Setup.sh`.
- Use [CMake](https://cmake.org/) to generate the build files for the project: `cmake -B build .`. 
- Then, build the project: `cmake --build build`.
- To build the sample: `cmake --build build -- SampleLayer`.

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
    SampleLayer(york::Registry registry) : york::Layer(registry) {}

    void onAttach() override {}

    void onEvent(york::Event& e) override {}

    void onUpdate(float deltaTime) override {}

    void onRender() override {}

    void onDetach() override {}
};

extern "C" york::Layer* createLayer(york::Registry& registry)
{
    return new SampleLayer(registry);
}
```
It is important to note that your create function must not be name-mangled as to make it easier for the Runtime to search and call the function, hence the `extern "C"`. Otherwise, this how client applications will be structured: A series of classes that implement `york::Layer`, each defining a create function and compiled into a separate standalone shared object. The name of the shared object, excluding any platform prefixes such as "lib" and file extension will be the name of the layer. For example, `libSampleLayer.so` will be treated as a layer with the name `SampleLayer`. 
#### createFunction
Also note that the Runtime will pass a `Registry` to the create function. This is because the Runtime is also linked to the Engine. On most modern operating systems, this results in the Engine library being loaded twice, with the Runtime and your Client have their own copies loaded. As a result these two copies of the Engine have their own memory allocated. Thus, internal data structures created in one copy will not be available in the other copy. As such, the `Registry` will contain the necessary data initialized by the Runtime for use in your client. You need to accept this `Registry` and pass it to the base constructor of the `Layer`.   

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