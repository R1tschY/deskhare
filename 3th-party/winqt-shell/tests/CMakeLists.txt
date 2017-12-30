
enable_testing()

find_package(Qt5Test REQUIRED)

macro(add_qttest_test)
    foreach(_testname ${ARGN})
        set(_longtestname "winqt-test-${_testname}")
        
        add_executable(${_longtestname} WIN32 ${_testname}.cpp)
        target_link_libraries(${_longtestname} Qt5::Test winqt5shell)
        
        add_test(
            NAME ${_longtestname} 
            COMMAND ${_longtestname}
        )
    endforeach()
endmacro()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)


add_qttest_test(env)

