set(FOXWING_SRCS
	main.cpp
	settings.cpp
	server.cpp
	server_handlers.cpp
	util.cpp
)

add_executable(${PROJECT_NAME} ${FOXWING_SRCS})

find_package(raylib CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Get the git commit hash for versioning
execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

target_compile_definitions(${PROJECT_NAME} PRIVATE GIT_COMMIT_HASH="${GIT_COMMIT_HASH}")