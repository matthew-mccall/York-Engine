cmake_minimum_required(VERSION 3.20)

project(YorkEngine)

find_package(PkgConfig REQUIRED)
find_package(Vulkan REQUIRED)
find_package(SDL)

set(SDL_BUILDING_LIBRARY)

if (SDL_FOUND)
    set(SDL_DEP SDL::SDL)
else(SDL_FOUND)
    pkg_check_modules(SDL IMPORTED_TARGET sdl2)
    set(SDL_DEP PkgConfig::SDL)
endif(SDL_FOUND)

add_subdirectory(libs/spdlog)

set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC
    "src/Application.cpp"
    "src/Asset.cpp"
    "src/Async.cpp"
    "src/Log.cpp"
)

set(TEST_SRC
    "test/App.cpp"
)

add_library(YorkEngine ${SRC})

target_include_directories(YorkEngine PUBLIC "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/libs/taskflow")
target_link_libraries(YorkEngine PUBLIC
    spdlog::spdlog
)
target_link_libraries(YorkEngine PRIVATE 
    Vulkan::Vulkan
    ${SDL_DEP}
)

unset(SDL_BUILDING_LIBRARY)

add_executable(YorkTest ${TEST_SRC})
target_link_libraries(YorkTest PUBLIC YorkEngine)