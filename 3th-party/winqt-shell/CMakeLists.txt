if (NOT WIN32)
  return()
endif()

project(winqt5shell CXX)

find_package(Qt5Gui REQUIRED)
find_package(Qt5WinExtras REQUIRED)
set(winqt5shell_LIBS Qt5::Gui Qt5::WinExtras)

file(GLOB_RECURSE winqt5shell_SOURCES "*.cpp")

add_library(winqt5shell EXCLUDE_FROM_ALL STATIC ${winqt5shell_SOURCES})
target_include_directories(winqt5shell PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(winqt5shell ${winqt5shell_LIBS})


