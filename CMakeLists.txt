cmake_minimum_required(VERSION 3.18)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/libs/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(YorkEngine VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Vulkan REQUIRED)
find_package(PkgConfig)
find_package(SDL2 CONFIG REQUIRED)
find_package(XercesC CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Boost REQUIRED)
find_package(unofficial-curlpp CONFIG REQUIRED)
find_package(Threads)

option(BUILD_SHADERC "Build libshaderc instead of linking with system install" OFF)

set(SDL_BUILDING_LIBRARY)

if (BUILD_SHADERC)
    set(SHADERC_SKIP_TESTS ON)
    add_subdirectory(libs/shaderc)
    set(SHADERC_DEP shaderc)
else()
    set(SHADERC_DEP shaderc_combined)
endif()

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
        src/Asset.cpp
        src/Async.cpp
        src/Event.cpp
        src/Timer.cpp
        src/Window.cpp
        src/Graphics/Handle.cpp
        src/Graphics/Instance.cpp
        src/Graphics/Device.cpp
        src/Graphics/PhysicalDevice.cpp
        src/Graphics/Shader.cpp
        src/LayerStack.cpp
        src/Identifiable.cpp
        src/Graphics/Surface.cpp
        src/Graphics/SwapChain.cpp
        src/Graphics/Pipeline.cpp
        src/Graphics/PipelineLayout.cpp
        src/Graphics/RenderPass.cpp
        src/Graphics/Framebuffer.cpp
        src/Graphics/ImageView.cpp
        src/Renderer/Renderer.cpp
        src/Graphics/CommandPool.cpp
        src/Graphics/Semaphore.cpp
        src/Log.cpp
        src/Layer.cpp
        src/XML/Tag.cpp
        src/XML/Document.cpp
        src/Graphics/FrameData.cpp
        src/Graphics/Fence.cpp
        src/Registry.cpp
        include/york/Event.hpp
        include/york/York.hpp
        include/york/Async.hpp
        include/york/Timer.hpp
        include/york/KeyCodes.hpp
        include/york/Graphics/Instance.hpp
        include/york/Graphics/RequestableItem.hpp
        include/york/Graphics/Handle.hpp
        include/york/Window.hpp
        include/york/Graphics/Device.hpp
        include/york/Graphics/PhysicalDevice.hpp
        include/york/Log.hpp
        include/york/Asset.hpp
        include/york/Graphics/Shader.hpp
        include/york/Layer.hpp
        include/york/LayerStack.hpp
        include/york/Identifiable.hpp
        include/york/Graphics/Surface.hpp
        include/york/Graphics/SwapChain.hpp
        include/york/Graphics/PipelineLayout.hpp
        include/york/Graphics/Pipeline.hpp
        include/york/Graphics/RenderPass.hpp
        include/york/Graphics/Framebuffer.hpp
        include/york/Graphics/ImageView.hpp
        include/york/Renderer/Renderer.hpp
        include/york/Graphics/CommandPool.hpp
        include/york/Graphics/Semaphore.hpp
        include/york/XML/Tag.hpp
        include/york/Graphics/FrameData.hpp
        include/york/XML/Document.hpp
        include/york/Graphics/Fence.hpp
        include/york/Registry.hpp)

set(RUNTIME_SRC
        runtime/Runtime.cpp
        runtime/LayerLoader.cpp
        runtime/LayerLoader.hpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SRC})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${RUNTIME_SRC})

configure_file(include/york/Config.hpp.in include/york/Config.hpp)

add_library(${PROJECT_NAME} ${SRC})

message(STATUS "Using VulkanSDK at: $ENV{VULKAN_SDK}")

link_directories($ENV{LD_LIBRARY_PATH})
link_directories($ENV{VULKAN_SDK}/lib)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}/include" "${PROJECT_SOURCE_DIR}/libs/taskflow" "${PROJECT_SOURCE_DIR}/libs/curlpp/include")
target_link_libraries(${PROJECT_NAME} PUBLIC
        Vulkan::Vulkan
        Threads::Threads
        ${SHADERC_DEP}
        fmt::fmt
        SDL2::SDL2
        SDL2::SDL2main
        xerces_xerces-c
        XercesC::XercesC
        Boost::boost
	    unofficial::curlpp::curlpp)

if ({${CMAKE_SYSTEM_NAME} EQUAL "Linux")
    set_property(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

unset(SDL_BUILDING_LIBRARY)

add_executable(YorkRuntime ${RUNTIME_SRC})
target_link_libraries(YorkRuntime PUBLIC ${PROJECT_NAME})

add_subdirectory(sample EXCLUDE_FROM_ALL)

file(COPY shaders DESTINATION sample/)