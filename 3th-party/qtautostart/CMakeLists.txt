cmake_minimum_required(VERSION 3.0)

project(qtautostart CXX)

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

if (WIN32)
    set(qtautostart_SOURCES "src/qtautostart_win.cpp")
elseif(UNIX AND NOT APPLE)
    set(qtautostart_SOURCES "src/qtautostart_unix.cpp")
endif()
set(qtautostart_HEADER "src/qtautostart.h")

add_library(qtautostart STATIC ${qtautostart_SOURCES})
target_link_libraries(qtautostart Qt5::Gui)
target_include_directories(qtautostart PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
