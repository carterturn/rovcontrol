cmake_minimum_required(VERSION 2.8)

project(rovcontrolpanel)

set(rovcontrolpanel_MAJOR_VERSION 0)
set(rovcontrolpanel_MINOR_VERSION 2)

add_executable(controlpanel main.cpp)


link_directories(/usr/lib)
link_directories(/usr/lib64)
target_link_libraries(controlpanel ctclient)

link_directories(/usr/lib)
link_directories(/usr/lib64)
target_link_libraries(controlpanel glfw)

link_directories(/usr/lib)
link_directories(/usr/lib64)
target_link_libraries(controlpanel GL)

link_directories(/usr/lib)
link_directories(/usr/lib64)
target_link_libraries(controlpanel ftgl)

include_directories(/usr/include/freetype2)
include_directories(/usr/include/futuregl)
link_directories(/usr/lib)
link_directories(/usr/lib64)
target_link_libraries(controlpanel futuregl)

link_directories(/usr/lib)
link_directories(/usr/lib64)
target_link_libraries(controlpanel tomcrypt)

add_definitions(--std=c++11)