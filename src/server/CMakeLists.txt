set(SERVER_SRCS 
    server_main.cpp
)

add_executable(server ${SERVER_SRCS})
target_link_libraries(server PUBLIC common)