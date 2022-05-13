cmake_minimum_required(VERSION 3.18)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/libs/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(YorkEngine VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Vulkan REQUIRED)
find_package(Taskflow CONFIG REQUIRED)
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

# execute_process(COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader.vert -o ${PROJECT_BINARY_DIR}/vert.spv)
# execute_process(COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader.frag -o ${PROJECT_BINARY_DIR}/frag.spv)

include(cmake/EmbedShader.cmake)

EmbedShader("${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader.vert" "${PROJECT_BINARY_DIR}/vert.spv" VertexSPV)
EmbedShader("${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader.frag" "${PROJECT_BINARY_DIR}/frag.spv" FragSPV)

set(SRC
        src/Asset.cpp
        src/Async.cpp
        src/Event.cpp
        src/StopWatch.cpp
        src/Window.cpp
        src/Renderer/Vulkan/Handle.cpp
        src/Renderer/Vulkan/Instance.cpp
        src/Renderer/Vulkan/Device.cpp
        src/Renderer/Vulkan/PhysicalDevice.cpp
        src/Renderer/Vulkan/Shader.cpp
        src/Identifiable.cpp
        src/Renderer/Vulkan/Surface.cpp
        src/Renderer/Vulkan/SwapChain.cpp
        src/Renderer/Vulkan/Pipeline.cpp
        src/Renderer/Vulkan/PipelineLayout.cpp
        src/Renderer/Vulkan/RenderPass.cpp
        src/Renderer/Vulkan/Framebuffer.cpp
        src/Renderer/Vulkan/ImageView.cpp
        src/Renderer/Renderer.cpp
        src/Renderer/Vulkan/CommandPool.cpp
        src/Renderer/Vulkan/Semaphore.cpp
        src/Log.cpp
        src/Layer.cpp
        src/XML/Tag.cpp
        src/XML/Document.cpp
        src/Renderer/Vulkan/FrameData.cpp
        src/Renderer/Vulkan/Fence.cpp
        src/Registry.cpp
        src/Renderer/RendererImpl.cpp
        src/Renderer/VulkanRendererImpl.cpp
        src/Renderer/Renderer.cpp
        include/york/Event.hpp
        include/york/York.hpp
        include/york/Async.hpp
        include/york/StopWatch.hpp
        include/york/KeyCodes.hpp
        src/Renderer/Vulkan/Instance.hpp
        src/Renderer/Vulkan/RequestableItem.hpp
        src/Renderer/Vulkan/Handle.hpp
        include/york/Window.hpp
        src/Renderer/Vulkan/Device.hpp
        src/Renderer/Vulkan/PhysicalDevice.hpp
        include/york/Log.hpp
        include/york/Asset.hpp
        src/Renderer/Vulkan/Shader.hpp
        include/york/Layer.hpp
        include/york/Identifiable.hpp
        src/Renderer/Vulkan/Surface.hpp
        src/Renderer/Vulkan/SwapChain.hpp
        src/Renderer/Vulkan/PipelineLayout.hpp
        src/Renderer/Vulkan/Pipeline.hpp
        src/Renderer/Vulkan/RenderPass.hpp
        src/Renderer/Vulkan/Framebuffer.hpp
        src/Renderer/Vulkan/ImageView.hpp
        include/york/Renderer/Renderer.hpp
        src/Renderer/Vulkan/CommandPool.hpp
        src/Renderer/Vulkan/Semaphore.hpp
        include/york/XML/Tag.hpp
        src/Renderer/Vulkan/FrameData.hpp
        include/york/XML/Document.hpp
        src/Renderer/Vulkan/Fence.hpp
        include/york/Registry.hpp
        include/york/Renderer/RendererImpl.hpp
        src/Renderer/VulkanRendererImpl.hpp
        include/york/Renderer/Renderer.hpp
        include/york/Containers.hpp)

set(RUNTIME_SRC
        runtime/Runtime.cpp
        runtime/LayerLoader.cpp
        runtime/LayerLoader.hpp
        runtime/LayerStack.cpp
        runtime/LayerStack.hpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SRC})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/runtime" FILES ${RUNTIME_SRC})

configure_file(include/york/Config.hpp.in "${PROJECT_BINARY_DIR}/include/york/Config.hpp")
add_library(${PROJECT_NAME} ${SRC})

message(STATUS "Using VulkanSDK at: $ENV{VULKAN_SDK}")
message(STATUS "Using glslc at: ${Vulkan_GLSLC_EXECUTABLE}")

link_directories($ENV{LD_LIBRARY_PATH})

if (${CMAKE_SYSTEM_NAME} EQUAL "Windows")
    link_directories($ENV{VULKAN_SDK}/Lib)
else()
    link_directories($ENV{VULKAN_SDK}/lib)
endif()


target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}/include"  "${PROJECT_BINARY_DIR}/EmbedVertexSPV" "${PROJECT_BINARY_DIR}/EmbedFragSPV")
target_link_libraries(${PROJECT_NAME} PUBLIC
        Vulkan::Vulkan
        Taskflow::Taskflow
        Threads::Threads
        ${SHADERC_DEP}
        fmt::fmt
        SDL2::SDL2
        SDL2::SDL2main
        xerces_xerces-c
        XercesC::XercesC
        Boost::boost
	      unofficial::curlpp::curlpp
        EmbedVertexSPV
        EmbedFragSPV
      )

if (${CMAKE_SYSTEM_NAME} EQUAL "Linux")
    set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

unset(SDL_BUILDING_LIBRARY)

add_executable(YorkRuntime ${RUNTIME_SRC})
target_link_libraries(YorkRuntime PUBLIC ${PROJECT_NAME})

add_subdirectory(sample EXCLUDE_FROM_ALL)
