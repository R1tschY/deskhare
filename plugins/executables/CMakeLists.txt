file(GLOB deskhare_executable_SOURCES "*.cpp")
  
deskhare_plugin(deskhare-executable ${deskhare_executable_SOURCES})
target_link_libraries(deskhare-executable deskhare Qt5::Core)
