if (NOT UNIX OR APPLE)
  return()
endif()

find_package(qt5xdg)

# Sources
file(GLOB deskhare_xdg_SOURCES "*.cpp")

link_libraries(Qt5Xdg)
deskhare_plugin(deskhare-xdg ${deskhare_xdg_SOURCES})
