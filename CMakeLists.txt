cmake_minimum_required(VERSION 3.17)
project(MAP)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
include_directories(include)

add_executable(MAP "src/main.cpp"
                   "src/map.cpp"    "include/map.h"
                   "src/point.cpp" "include/point.h")

add_executable(test "test/vec.cpp"
                    "src/map.cpp"    "include/map.h"
                    "src/point.cpp" "include/point.h")

TARGET_LINK_LIBRARIES(MAP ${SDL2_LIBRARIES})