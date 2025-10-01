cmake_minimum_required(VERSION 3.10)

project(foxwing VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})   # executables
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})   # shared libraries (.dll/.so/.dylib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})   # static libraries (.lib/.a)

add_subdirectory(core)
add_subdirectory(server)
add_subdirectory(client)

#find_package(raylib CONFIG REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE raylib)