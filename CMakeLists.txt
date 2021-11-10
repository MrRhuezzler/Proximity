cmake_minimum_required(VERSION 3.17)
project(MAP)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2_ttf REQUIRED sdl2_ttf)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2_ttf_INCLUDE_DIRS})
include_directories(include)

add_executable(MAP "src/main.cpp"
                   "src/map.cpp"    "include/map.h"
                   "src/point.cpp" "include/point.h"
                   "src/quadTree.cpp" "include/quadTree.h")

add_executable(test "test/vec.cpp"
                    "src/map.cpp"    "include/map.h"
                    "src/point.cpp" "include/point.h"
                    "src/quadTree.cpp" "include/quadTree.h")

TARGET_LINK_LIBRARIES(MAP ${SDL2_LIBRARIES} ${SDL2_ttf_LIBRARIES})
TARGET_LINK_LIBRARIES(test ${SDL2_LIBRARIES} ${SDL2_ttf_LIBRARIES})