
# Qt

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Sources
file(GLOB_RECURSE deskhare_SOURCES "*.cpp")
    
# Target
add_library(deskhare SHARED ${deskhare_SOURCES} ${deskhare_PFSOURCES})
target_include_directories(deskhare PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(deskhare cpp-utils
  Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql Qt5::Concurrent
  Qt5::QSQLiteDriverPlugin)
  
if (WIN32)
  install(TARGETS deskhare
    LIBRARY DESTINATION .
    RUNTIME DESTINATION .)
endif()
