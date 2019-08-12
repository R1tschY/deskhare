if (NOT UNIX OR APPLE)
  return()
endif()

find_package(qt5xdg)
find_package(Qt5Xml)

# Sources
file(GLOB_RECURSE deskhare_xdg_SOURCES "*.cpp")

deskhare_plugin(deskhare-xdg ${deskhare_xdg_SOURCES})
target_link_libraries(deskhare-xdg 
    deskhare xdg-thumbnails-qt
    Qt5::Core Qt5::Xml Qt5Xdg)
