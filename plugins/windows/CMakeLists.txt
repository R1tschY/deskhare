if (NOT WIN32)
  return()
endif()

# Sources
file(GLOB_RECURSE deskhare_win_SOURCES "*.cpp")

deskhare_plugin(deskhare-win ${deskhare_win_SOURCES})
target_link_libraries(deskhare-win deskhare Qt5::Core Qt5::Sql winqt5shell)
