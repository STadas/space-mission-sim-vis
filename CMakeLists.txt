cmake_minimum_required(VERSION 3.8)
cmake_policy(SET CMP0079 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

option(BUILD_APP "build the app" ON)
option(BUILD_TESTS "build the tests" OFF)

project("SpaceMissionSimVis" VERSION 1.0.0 LANGUAGES CXX)

add_compile_options(-fno-sized-deallocation)

find_package(Qt5 REQUIRED
    COMPONENTS
    Widgets
    Core
    Concurrent
)

find_package(Threads REQUIRED)

if (BUILD_TESTS)
    find_package(GTest REQUIRED)
endif ()

add_subdirectory("src")

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory("tests")
endif ()
