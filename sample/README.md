# `/sample/`

The `sample` directory contains a sample project template for a client application for use with York Engine. In essence, a client application is simply a collection `york::Layer` child classes compiled as separate, standalone dynamic libraries (a.k.a. shared object files) with an un-mangled (i.e. `extern "C"`) `york::Layer* createLayer()` function defined to create your Layer.

### `Client.xml`
An example `Client.xml` which defines the client application. Every client application needs a `Client.xml` file. The `Client.xml` file is searched for by the engine upon startup so that it knows which layers to load. A `Client.xml` file is a [well-formed XML document](https://www.w3.org/TR/xml/#sec-well-formed) with a `<Client>` root tag encompassing a `<Name>` tag, defining the name of the client application, and a `<Layers>` tag which consists of multiple `<Layer>` tags. Each `<Layer>` tag must have a `<Name>` tag, specifying the name of the compiled layer as known to the filesystem without the "lib" prefix or any file extension, and may optionally specify `<CreateFunction>` tag specifying the un-mangled function name used to create your layer (although if you omit this tag, it is assumed to be `createLayer`).

### `CMakeLists.txt`
An example build system to use to build a client. Simply compile each layer into a separate dynamic library linked with York Engine.

### `SampleLayer.cpp` 
An example source file to use York Engine. Simply create a class that inherits `york::Layer` and implement the necessary functions. Then define a `extern "C" york::Layer* createLayer()` function to create your class. This will be searched for and called by York Engine to create your layer.
