file(GLOB deskhare_calc_SOURCES "*.cpp")
deskhare_plugin(deskhare-calc ${deskhare_calc_SOURCES})
target_link_libraries(deskhare-calc deskhare Qt5::Core Boost::boost cpp-utils)
target_compile_definitions(deskhare-calc PRIVATE BUILD_CALC_PARSER)

add_subdirectory(tests)
