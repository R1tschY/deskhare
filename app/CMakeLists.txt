# Qt

find_package(Qt5Widgets QUIET)
find_package(Qt5Svg QUIET)
include(DeployQt5)

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
 cpp-utils deskhare Qt5::Widgets qxtglobalshortcut5 qtautostart
 Qt5::Svg)
set_property(TARGET deskhare-app PROPERTY OUTPUT_NAME deskhare)
 
# Install
if (WIN32)
    install(TARGETS deskhare-app
        RUNTIME DESTINATION . 
        COMPONENT Runtime)
        
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")
    
    set(DESKHARE_APP
        "${CMAKE_INSTALL_PREFIX}/deskhare.exe")
    set(DESKHARE_LIBS
        "${CMAKE_INSTALL_PREFIX}/libdeskhare.dll;\${INSTALLED_PLUGINS}")
    set(DIRS ${QT_LIBRARY_DIRS}) 
    install(CODE "set(gp_tool objdump)")
    install(CODE "file(GLOB INSTALLED_PLUGINS \"\${CMAKE_INSTALL_PREFIX}/plugins/*.dll\")")
    install_qt5_executable(
        APP "${DESKHARE_APP}"
        QTPLUGINS Qt5::QWindowsIntegrationPlugin Qt5::QSQLiteDriverPlugin
        LIBS ${DESKHARE_LIBS})
elseif(UNIX AND NOT APPLE)
    install(TARGETS deskhare-app
        RUNTIME DESTINATION bin 
        COMPONENT Runtime)
endif()
