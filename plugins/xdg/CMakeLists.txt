if (NOT UNIX OR APPLE)
  return()
endif()

find_package(qt5xdg)

# Sources
file(GLOB deskhare_xdg_SOURCES "*.cpp")

deskhare_plugin(deskhare-xdg ${deskhare_xdg_SOURCES})
target_link_libraries(deskhare-xdg deskhare Qt5::Core Qt5Xdg)
