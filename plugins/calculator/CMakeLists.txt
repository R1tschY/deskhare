file(GLOB deskhare_calc_SOURCES "*.cpp")
deskhare_plugin(deskhare-calc ${deskhare_calc_SOURCES})
target_link_libraries(deskhare-calc deskhare Qt5::Core)

add_subdirectory(tests)
