cmake_minimum_required(VERSION 3.17)
project(Proximity)

set(CMAKE_CXX_STANDARD 17)

set(SDL_DIR "SDL")
add_subdirectory(${SDL_DIR})

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
add_library(IMGUI STATIC)

target_sources( IMGUI
                PRIVATE
                    ${IMGUI_DIR}/imgui_demo.cpp
                    ${IMGUI_DIR}/imgui_draw.cpp
                    ${IMGUI_DIR}/imgui_tables.cpp
                    ${IMGUI_DIR}/imgui_widgets.cpp
                    ${IMGUI_DIR}/imgui.cpp

                PRIVATE
                    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer.cpp
                    ${IMGUI_DIR}/backends/imgui_impl_sdl.cpp
                )

target_include_directories( IMGUI
                            PUBLIC ${IMGUI_DIR}
                            PUBLIC ${IMGUI_DIR}/backends
                            PUBLIC ${SDL2_INCLUDE_DIRS})

target_link_libraries(IMGUI PUBLIC ${OPENGL_LIBRARIES} SDL2main SDL2-static ${CMAKE_DL_LIBS})


add_compile_definitions(SDL_MAIN_HANDLED)
include_directories(include ${SDL_DIR}/include ${IMGUI_DIR} ${IMGUI_DIR} ${IMGUI_DIR}/backends)

add_executable(${PROJECT_NAME} "src/main.cpp"
                "src/map.cpp" "include/map.h"
                "src/point.cpp" "include/point.h"
                "src/quadTree.cpp" "include/quadTree.h"
                "src/zorder.cpp" "include/zorder.h")

target_link_libraries(${PROJECT_NAME} SDL2main SDL2-static IMGUI)


add_executable(test "test/vec.cpp"
                "src/point.cpp"  "include/point.h"
                "src/zorder.cpp" "include/zorder.h")

# target_link_libraries(test SDL2main SDL2-static IMGUI)
