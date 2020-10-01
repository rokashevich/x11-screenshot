# General options
cmake_minimum_required(VERSION 3.8)
project(x11-screenshot)
set(CMAKE_BUILD_TYPE Release)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_library(x11screenshot STATIC src/screenshot)
target_link_libraries(x11screenshot X11 png.a libz.a)
set_property(TARGET x11screenshot PROPERTY CXX_STANDARD 11)

add_executable(screenshot src/main.cpp)
target_link_libraries(screenshot X11 x11screenshot)
set_property(TARGET screenshot PROPERTY CXX_STANDARD 11)
