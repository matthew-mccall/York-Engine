cmake_minimum_required(VERSION 3.20)

project(YorkEngine VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Vulkan REQUIRED)
find_package(PkgConfig)
find_package(SDL 2.0.18)
find_package(XercesC)
find_package(Threads)

set(SDL_BUILDING_LIBRARY)

if (SDL_FOUND)
    set(SDL_DEP SDL::SDL)
else ()
    if (PKG_CONFIG_FOUND)
        pkg_check_modules(SDL IMPORTED_TARGET sdl2>=2.0.18)
        if (sdl2_FOUND)
            set(SDL_DEP PkgConfig::SDL)
        else()
            message(NOTICE "SDL not found, building from source.")
            add_subdirectory(libs/SDL)
            set(SDL_DEP SDL2::SDL2)
        endif()
    else()
        message(NOTICE "SDL not found, building from source.")
        add_subdirectory(libs/SDL)
        set(SDL_DEP SDL2::SDL2)
    endif()
endif ()

if (XercesC_FOUND)
    set(XML_DEP XercesC::XercesC)
else ()
    message(NOTICE "Xerces-C not found, building from source.")
    add_subdirectory(libs/xerces-c)
    set(XML_DEP xerces-c)
endif ()

add_subdirectory(libs/fmt)
add_subdirectory(libs/curlpp)

set(SHADERC_SKIP_TESTS ON)
add_subdirectory(libs/shaderc)

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
        "src/Graphics/SwapChain.cpp"
        src/Graphics/Pipeline.cpp
        src/Graphics/PipelineLayout.cpp
        src/Graphics/RenderPass.cpp
        src/Graphics/Framebuffer.cpp
        src/Graphics/ImageView.cpp
        src/Renderer/Renderer.cpp
        "include/york/Event.hpp"
        "include/york/York.hpp"
        "include/york/Async.hpp"
        "include/york/Timer.hpp"
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
        "include/york/Graphics/SwapChain.hpp"
        include/york/Graphics/PipelineLayout.hpp
        include/york/Graphics/Pipeline.hpp
        include/york/Graphics/RenderPass.hpp
        include/york/Graphics/Framebuffer.hpp
        include/york/Graphics/ImageView.hpp
        include/york/Renderer/Renderer.hpp src/Graphics/CommandPool.cpp include/york/Graphics/CommandPool.hpp src/Graphics/Semaphore.cpp include/york/Graphics/Semaphore.hpp src/Log.cpp src/Layer.cpp src/XML/Tag.cpp include/york/XML/Tag.hpp src/XML/Document.cpp include/york/XML/Document.hpp)

set(RUNTIME_SRC
        runtime/Runtime.cpp runtime/LayerLoader.cpp runtime/LayerLoader.hpp)

source_group(Engine FILES ${SRC})
source_group(Runtime FILES ${RUNTIME_SRC})

configure_file(include/york/Config.hpp.in include/york/Config.hpp)

add_library(${PROJECT_NAME} ${SRC})

message(STATUS "Using VulkanSDK at: $ENV{VULKAN_SDK}")

link_directories($ENV{LD_LIBRARY_PATH})

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}/include" "${PROJECT_SOURCE_DIR}/libs/taskflow" "${PROJECT_SOURCE_DIR}/libs/curlpp/include")
target_link_libraries(${PROJECT_NAME} PUBLIC
        fmt::fmt
        Vulkan::Vulkan
        Threads::Threads
        ${SDL_DEP}
        ${XML_DEP}
        curlpp
        shaderc)

if ({${CMAKE_SYSTEM_NAME} EQUAL "Linux")
    set_property(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

unset(SDL_BUILDING_LIBRARY)

add_executable(YorkRuntime ${RUNTIME_SRC})
target_link_libraries(YorkRuntime PUBLIC ${PROJECT_NAME})

add_subdirectory(sample EXCLUDE_FROM_ALL)

file(COPY shaders DESTINATION sample/)