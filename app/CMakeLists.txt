# Sources
file(GLOB_RECURSE deskhare_SOURCES "*.cpp")

# Target
add_executable(deskhare-app WIN32 ${deskhare_SOURCES})
target_link_libraries(deskhare-app 
 cpp-utils deskhare Qt5::Widgets qxtglobalshortcut5)
