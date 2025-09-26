cmake_minimum_required(VERSION 3.10)

project(foxwing VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src/common)
add_subdirectory(src/server)
add_subdirectory(src/client)
#find_package(raylib CONFIG REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE raylib)