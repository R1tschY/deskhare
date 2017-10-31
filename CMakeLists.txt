cmake_minimum_required(VERSION 3.0)

project(deskhare CXX)

## CMake

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
include(AddConfig)

add_config(
  Debug 
  CXX_FLAGS "-Og -g -DDEBUG -D_DEBUG" 
  LINKER_FLAGS "-Og -g"
)

add_config(
  Release 
  CXX_FLAGS "-O2 -DNDEBUG" 
  LINKER_FLAGS "-O2 -g"
)

## C++

set(CMAKE_CXX_STANDARD 14)

## Qt

add_definitions(
 -DQT_DEPRECATED_WARNINGS
 -DQT_DISABLE_DEPRECATED_BEFORE=0x050500
 -DQT_USE_QSTRINGBUILDER)
 
## Windows

if (WIN32)
    # compile for 7
    add_definitions(
        -DUNICODE -D_UNICODE
        -DNTDDI_VERSION=0x06010000
        -D_WIN32_WINNT=0x0601
        -DWINVER=0x0601
        -D_WIN32_IE=0x0A00
    )
endif()

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Concurrent REQUIRED)

## Boost

find_package(Boost REQUIRED)

## Build

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)


add_subdirectory(3th-party/cpp-utils)
add_subdirectory(3th-party/qxtglobalshortcut5)
add_subdirectory(3th-party/xdg-thumbnails-qt)
<<<<<<< HEAD
add_subdirectory(3th-party/qtautostart)
=======
add_subdirectory(3th-party/winqt-shell)
>>>>>>> [win] applications source
add_subdirectory(libdeskhare)

add_subdirectory(plugins)

add_subdirectory(app)

