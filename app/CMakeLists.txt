# Qt

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Sources
file(GLOB_RECURSE deskhare_SOURCES "*.cpp")
file(GLOB_RECURSE deskhare_RESOURCES "*.qrc")

# Target
qt5_add_resources(deskhare_RES ${deskhare_RESOURCES})
add_executable(deskhare-app WIN32 ${deskhare_SOURCES} ${deskhare_RES})
target_link_libraries(deskhare-app 
 cpp-utils deskhare Qt5::Widgets qxtglobalshortcut5 qtautostart)
