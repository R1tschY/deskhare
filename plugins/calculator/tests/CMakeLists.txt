
enable_testing()

find_package(Qt5Test REQUIRED)

# make check
if(NOT TARGET check)
   add_custom_target(check)
endif()

macro(add_qttest_test)
    foreach(_testname ${ARGN})
        set(_longtestname "deskhare-calc-test-${_testname}")
        
        add_executable(${_longtestname} ${_testname}.cpp)
        target_link_libraries(${_longtestname} Qt5::Test deskhare-calc)
        
        add_test(
            NAME ${_longtestname} 
            COMMAND ${_longtestname}
        )
        message(STATUS "added test ${_longtestname}")
    endforeach()
endmacro()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)


add_qttest_test(parser)

