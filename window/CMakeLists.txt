cmake_minimum_required(VERSION 3.10)

project(window)

set(CMAKE_CXX_STANDARD 17)

get_filename_component(abs_include_dir "include" REALPATH)
set(include_dirs ${abs_include_dir})
    
file(GLOB_RECURSE header_files "*.h")
file(GLOB_RECURSE src_files "*.cpp")

string(TOLOWER ${CMAKE_GENERATOR} GENERATOR_NAME)
string(REPLACE " " "_" GENERATOR_NAME ${GENERATOR_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY bin/${GENERATOR_NAME}/)

get_filename_component(abs_include_dir "../lib/imgui/" REALPATH)
list(APPEND include_dirs ${abs_include_dir})
list(APPEND link_libs imgui)

include_directories(${include_dirs})
link_libraries(${link_libs})

add_library(${PROJECT_NAME} ${src_files} ${header_files})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
add_dependencies(${PROJECT_NAME} ${link_libs})

target_link_libraries(${PROJECT_NAME} d3d11.lib)
