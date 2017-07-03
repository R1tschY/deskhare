# Sources
file(GLOB deskhare_fileindexer_SOURCES "*.cpp")

deskhare_plugin(deskhare-fileindexer ${deskhare_fileindexer_SOURCES})
target_link_libraries(deskhare-fileindexer deskhare Qt5::Core Qt5::Sql)
