
enable_testing()

find_package(Qt5Test REQUIRED)

# make check
if(NOT TARGET check)
   add_custom_target(check)
endif()

macro(add_qttest_test)
    foreach(_testname ${ARGN})
        set(_longtestname "xdg-thumbnails-qt-test-${_testname}")
        
        add_executable(${_longtestname} ${_testname}.cpp)
        target_link_libraries(${_longtestname} Qt5::Test xdg-thumbnails-qt)
        target_include_directories(${_longtestname}
            PRIVATE "${PROJECT_SOURCE_DIR}/src"
        )
        
        add_test(
            NAME ${_longtestname} 
            COMMAND ${_longtestname}
        )
        set_tests_properties(
            ${_longtestname} 
            PROPERTIES
            ENVIRONMENT "XDG_CACHE_HOME=/tmp/.cache"
        )
    endforeach()
endmacro()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)


add_qttest_test(thumbnails)

