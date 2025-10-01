set(CORE_SRCS 
    src/net/net.cpp
    src/platform/platform_win32.cpp
)

add_library(core ${CORE_SRCS})
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/core/include)

find_package(unofficial-enet CONFIG REQUIRED)
target_link_libraries(core PUBLIC unofficial::enet::enet)

if(WIN32)
    target_link_libraries(core PRIVATE Ws2_32 Winmm)
endif()