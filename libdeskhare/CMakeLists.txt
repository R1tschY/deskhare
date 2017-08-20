# Sources
file(GLOB deskhare_SOURCES 
    "*.cpp"
    "filesystem/*.cpp"
    "indexes/*.cpp"
    "shell/*.cpp"
    "utils/*.cpp"
    "actions/*.cpp"
    "history/*.cpp"
    "matches/*.cpp")
    
if(WIN32)
  #file(GLOB_RECURSE deskhare_PFSOURCES "platforms/windows/*.cpp")
else()
  file(GLOB_RECURSE deskhare_PFSOURCES "platforms/linux/*.cpp")
endif()

# Target
add_library(deskhare SHARED ${deskhare_SOURCES} ${deskhare_PFSOURCES})
target_include_directories(deskhare PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(deskhare cpp-utils
  Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql Qt5::Concurrent)
