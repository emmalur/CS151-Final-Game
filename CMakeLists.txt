cmake_minimum_required(VERSION 3.26)
project(DungeonOfDoom)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -pedantic)

set(CMAKE_EXE_LINNKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static")

include (FetchContent)
FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
    GIT_TAG v5.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp src/game.cpp src/mainmenu.cpp src/level.cpp src/player.cpp src/map.cpp src/cell.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)
