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

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

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
add_subdirectory(libdeskhare)

add_subdirectory(plugins)

add_subdirectory(app)

