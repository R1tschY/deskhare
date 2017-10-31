
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

function(deskhare_plugin target)

    add_library(${target} SHARED ${ARGN})
    set_target_properties(
        ${target} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin/plugins"
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin/plugins"
    )
    message(STATUS "Found plugin: ${target}")

endfunction()


# include all subdirectories

file(GLOB _subdirs RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*")
foreach(_subdir ${_subdirs})
  if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${_subdir}")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_subdir}/CMakeLists.txt")
      add_subdirectory("${_subdir}")
    endif()
  endif()
endforeach()