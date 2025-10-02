set(CORE_SRCS 
    src/net/net.cpp
    src/platform/platform_win32.cpp
    src/platform/platform_linux.cpp
)

add_library(core ${CORE_SRCS})
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/core/include)

# ENet
find_path(ENET_INCLUDE_DIR enet/enet.h)
find_library(ENET_LIBRARY enet)

if (NOT ENET_INCLUDE_DIR OR NOT ENET_LIBRARY)
    message(WARNING "ENet not found, trying to use unofficial-enet")
    find_package(unofficial-enet CONFIG REQUIRED)
    target_link_libraries(core PUBLIC unofficial::enet::enet)
else()
    target_include_directories(core PUBLIC ${ENET_INCLUDE_DIR})
    target_link_libraries(core PUBLIC ${ENET_LIBRARY})
endif()

if(WIN32)
    target_link_libraries(core PRIVATE Ws2_32 Winmm)
endif()