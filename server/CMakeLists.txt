set(SERVER_SRCS 
    server_main.cpp
)

add_library(server SHARED ${SERVER_SRCS})
target_compile_definitions(server PRIVATE FOXWING_SERVER)
target_link_libraries(server PUBLIC core)

# -- Dedicated server target -- #
add_executable(ds ${SERVER_SRCS} ds.cpp)
target_compile_definitions(ds PRIVATE FOXWING_DS)
target_link_libraries(ds PUBLIC core)
