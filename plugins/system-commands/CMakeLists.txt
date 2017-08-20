# Sources
set(deskhare_system_commands_SOURCES 
    "plugin.cpp" "systemcommandssource.cpp")

if (WIN32)
  return()
  list(APPEND deskhare_system_commands_SOURCES "systemcommands_win.cpp")
  
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  list(APPEND deskhare_system_commands_SOURCES "systemcommands_linux.cpp")
  
  find_package(Qt5DBus REQUIRED)
  set(EXTRA_LIBS Qt5::DBus)
  
else()
  message(ERROR "System commands plugin does not support ${CMAKE_SYSTEM_NAME}")
endif()

deskhare_plugin(deskhare-system-commands ${deskhare_system_commands_SOURCES})
target_link_libraries(deskhare-system-commands deskhare Qt5::Core ${EXTRA_LIBS})
