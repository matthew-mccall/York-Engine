cmake_minimum_required(VERSION 3.20)

project(YorkEngine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC
    "src/Application.cpp"
    "src/Asset.cpp"
    "src/Async.cpp"
    "src/Event.cpp"
    "src/Log.cpp"
    "src/Timer.cpp"
    "src/Graphics/Window.cpp"
)

set(TEST_SRC
    "test/App.cpp"
)

add_library(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/libs/taskflow")
target_link_libraries(${PROJECT_NAME} PUBLIC
    spdlog::spdlog
    ${SDL_DEP}
)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Vulkan::Vulkan
)

unset(SDL_BUILDING_LIBRARY)

add_executable(YorkTest ${TEST_SRC})
target_link_libraries(YorkTest PUBLIC ${PROJECT_NAME})
