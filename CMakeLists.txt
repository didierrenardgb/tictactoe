cmake_minimum_required(VERSION 3.10)

project(TicTacToe)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(app)
add_subdirectory(logic)
add_subdirectory(window)

add_subdirectory(lib/imgui)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT tictactoe_app)
