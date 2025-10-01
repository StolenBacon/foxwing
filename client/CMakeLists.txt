set(SERVER_SRCS 
    "client.cpp"
)

add_executable(client ${SERVER_SRCS})
target_compile_definitions(client PRIVATE FOXWING_CLIENT)
target_link_libraries(client PUBLIC core)
add_dependencies(client server) # Ensure client builds after server

# Third party dependencies
find_package(raylib CONFIG REQUIRED)
target_link_libraries(client PRIVATE raylib)