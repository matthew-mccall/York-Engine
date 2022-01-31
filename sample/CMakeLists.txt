cmake_minimum_required(VERSION 3.20)

project(SampleClient VERSION 0.1.0)

add_library(SampleLayer SHARED SampleLayer.cpp)
target_link_libraries(SampleLayer PUBLIC YorkEngine)