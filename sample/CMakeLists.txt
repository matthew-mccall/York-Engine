cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE ../libs/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(SampleClient VERSION 0.1.0)

add_library(SampleLayer SHARED SampleLayer.cpp)
target_link_libraries(SampleLayer PUBLIC YorkEngine)

file(COPY Client.xml DESTINATION .)
