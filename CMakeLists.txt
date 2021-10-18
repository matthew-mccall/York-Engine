cmake_minimum_required(VERSION 3.20)

project(YorkEngine)

add_subdirectory(libs/spdlog)

set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC
    "src/Application.cpp"
    "src/Log.cpp"
)

set(TEST_SRC
    "test/App.cpp"
)

add_library(YorkEngine ${SRC})
add_executable(YorkTest ${TEST_SRC})

target_include_directories(YorkEngine PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(YorkEngine PUBLIC spdlog::spdlog)

target_link_libraries(YorkTest PUBLIC YorkEngine)