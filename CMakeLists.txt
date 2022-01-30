cmake_minimum_required(VERSION 3.20)

project(YorkEngine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Vulkan REQUIRED)
find_package(PkgConfig)
find_package(SDL 2.0.18)
find_package(XercesC)

set(SDL_BUILDING_LIBRARY)

if (SDL_FOUND)
    set(SDL_DEP SDL::SDL)
else ()
    pkg_check_modules(SDL IMPORTED_TARGET sdl2)
    set(SDL_DEP PkgConfig::SDL)
endif ()

if (XercesC_FOUND)
    set(XML_DEP XercesC::XercesC)
else ()
    add_subdirectory(libs/xerces-c)
    set(XML_DEP xerces-c)
endif ()

add_subdirectory(libs/fmt)
add_subdirectory(libs/curlpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_BUILD_TYPE EQUAL Debug)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fno-omit-frame-pointer")

    if (CMAKE_C_COMPILER EQUAL clang)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mno-omit-leaf-frame-pointer")
    endif ()

    if (CMAKE_CXX_COMPILER EQUAL clang++)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mno-omit-leaf-frame-pointer")
    endif ()
endif ()

set(SRC
        "src/Application.cpp"
        "src/Asset.cpp"
        "src/Async.cpp"
        "src/Event.cpp"
        "src/Timer.cpp"
        "src/Window.cpp"
        "src/Graphics/Handle.cpp"
        "src/Graphics/Instance.cpp"
        "src/Graphics/Device.cpp"
        "src/Graphics/PhysicalDevice.cpp"
        "src/Graphics/Shader.cpp"
        "src/LayerStack.cpp"
        "src/Identifiable.cpp"
        "src/Graphics/Surface.cpp"
        "src/Exit.cpp"
        "src/Graphics/SwapChain.cpp"
        src/Graphics/Pipeline.cpp
        src/Graphics/PipelineLayout.cpp
        src/Graphics/RenderPass.cpp
        src/Graphics/Framebuffer.cpp
        src/Graphics/ImageView.cpp
        src/Renderer/Renderer.cpp
        "include/york/Event.hpp"
        "include/york/Application.hpp"
        "include/york/York.hpp"
        "include/york/Async.hpp"
        "include/york/Timer.hpp"
        "include/york/Entry.hpp"
        "include/york/KeyCodes.hpp"
        "include/york/Graphics/Instance.hpp"
        "include/york/Graphics/RequestableItem.hpp"
        "include/york/Graphics/Handle.hpp"
        "include/york/Window.hpp"
        "include/york/Graphics/Device.hpp"
        "include/york/Graphics/PhysicalDevice.hpp"
        "include/york/Log.hpp"
        "include/york/Asset.hpp"
        "include/york/Graphics/Shader.hpp"
        "include/york/Layer.hpp"
        "include/york/LayerStack.hpp"
        "include/york/Identifiable.hpp"
        "include/york/Graphics/Surface.hpp"
        "include/york/Exit.hpp"
        "include/york/Graphics/SwapChain.hpp"
        include/york/Graphics/PipelineLayout.hpp
        include/york/Graphics/Pipeline.hpp
        include/york/Graphics/RenderPass.hpp
        include/york/Graphics/Framebuffer.hpp
        include/york/Graphics/ImageView.hpp
        include/york/Renderer/Renderer.hpp src/Graphics/CommandPool.cpp include/york/Graphics/CommandPool.hpp src/Graphics/Semaphore.cpp include/york/Graphics/Semaphore.hpp src/Log.cpp)

set(TEST_SRC
        "test/App.cpp"
        "test/SampleLayer.cpp"
        "test/SampleLayer.hpp")

add_library(${PROJECT_NAME} ${SRC} ${INC_SRC})

link_directories($ENV{VULKAN_SDK}/lib)
link_directories($ENV{LD_LIBRARY_PATH})
link_directories(/usr/local/lib)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/libs/taskflow" "${PROJECT_SOURCE_DIR}/libs/curlpp/include")
target_link_libraries(${PROJECT_NAME} PUBLIC
        fmt::fmt
        Vulkan::Vulkan
        ${SDL_DEP}
        ${XML_DEP}
        glslang
        SPIRV
        SPVRemapper
        GenericCodeGen
        MachineIndependent
        OSDependent
        OGLCompiler
        SPIRV-Tools-opt
        SPIRV-Tools
        SPIRV-Tools-link
        curlpp)

unset(SDL_BUILDING_LIBRARY)

add_executable(YorkTest ${TEST_SRC})
target_link_libraries(YorkTest PUBLIC ${PROJECT_NAME})
