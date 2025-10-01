set(CORE_SRCS 
    src/platform/platform_win32.cpp
)

add_library(core ${CORE_SRCS})
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/core/include)