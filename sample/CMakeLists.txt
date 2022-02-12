cmake_minimum_required(VERSION 3.20)

project(SampleClient VERSION 0.1.0)

if ({${CMAKE_SYSTEM_NAME} EQUAL "Linux")
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

add_library(SampleLayer SHARED SampleLayer.cpp)
target_link_libraries(SampleLayer PUBLIC YorkEngine)

file(COPY Client.xml DESTINATION .)