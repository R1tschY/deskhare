cmake_minimum_required(VERSION 3.0)

project(deskhare LANGUAGES CXX VERSION 0.0.0)
set(PROJECT_NAME "deskhare")
set(PROJECT_AUTHOR "Richard Liebscher")
set(PROJECT_DESCRIPTION "a cross-platform quick launcher")

## CMake

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
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
  LINKER_FLAGS "-O2 -s"
)

# Tests
add_custom_target(tests
    COMMAND ${CMAKE_COMMAND} --build . 
    COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --verbose --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
enable_testing()

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

## Install

if (WIN32)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")
endif()

## Subdirs

add_subdirectory(3th-party/cpp-utils)
add_subdirectory(3th-party/qxtglobalshortcut5)
add_subdirectory(3th-party/xdg-thumbnails-qt)
add_subdirectory(3th-party/qtautostart)
add_subdirectory(3th-party/winqt-shell)
add_subdirectory(libdeskhare)

add_subdirectory(plugins)

add_subdirectory(app)

## Packaging

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_EXECUTABLES "deskhare-app;Deskhare")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "${PROJECT_AUTHOR}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${${PROJECT_NAME}_VERSION_PATCH}")
set(CPACK_MONOLITHIC_INSTALL ON)  

#set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/img/appicon.ico")
set(CPACK_NSIS_INSTALLED_ICON_NAME "deskhare.exe")
set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/R1tschY/deskhare")
set(CPACK_NSIS_MODIFY_PATH ON)
set(CPACK_NSIS_COMPRESSOR "/SOLID lzma")
set(CPACK_NSIS_EXECUTABLES_DIRECTORY ".")
set(CPACK_NSIS_MUI_FINISHPAGE_RUN "deskhare.exe")

include(CPack)

