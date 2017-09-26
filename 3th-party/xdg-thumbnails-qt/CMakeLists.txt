cmake_minimum_required(VERSION 3.0)

project(xdg-thumbnails-qt CXX)

#enable_testing()

## C++

set(CMAKE_CXX_STANDARD 14)

## Qt

add_definitions(
 -DQT_DEPRECATED_WARNINGS
 -DQT_DISABLE_DEPRECATED_BEFORE=0x050500
 -DQT_USE_QSTRINGBUILDER)

find_package(Qt5Gui REQUIRED)

## Build

file(GLOB_RECURSE thumbnails_SOURCES "src/*.cpp")
file(GLOB_RECURSE thumbnails_HEADER "src/*.h")

add_library(xdg-thumbnails-qt ${thumbnails_SOURCES})
target_link_libraries(xdg-thumbnails-qt Qt5::Gui)
target_include_directories(xdg-thumbnails-qt
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

add_subdirectory(tests)