cmake_minimum_required(VERSION 3.10)

project(imgui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS " -g")
get_filename_component(abs_include_dir "." REALPATH)
set(include_dirs ${abs_include_dir})
get_filename_component(abs_include_dir "backends/" REALPATH)
list(APPEND include_dirs ${abs_include_dir})
    
include_directories(${include_dirs})
    
file(GLOB header_files "*.h" "backends/*.h")
file(GLOB src_files "*.cpp" "backends/*.cpp")

add_library(${PROJECT_NAME} STATIC ${src_files} ${header_files})
