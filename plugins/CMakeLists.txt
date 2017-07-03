function(deskhare_plugin target)

    add_library(${target} SHARED ${ARGN})
    message(STATUS "Found plugin: ${target}")

endfunction()


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/plugins)

# include all subdirectories

file(GLOB _subdirs RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*")
foreach(_subdir ${_subdirs})
  if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${_subdir}")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_subdir}/CMakeLists.txt")
      add_subdirectory("${_subdir}")
    endif()
  endif()
endforeach()