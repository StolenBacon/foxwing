set(COMMON_SRCS 
    util.cpp
)

add_library(common ${COMMON_SRCS})
target_include_directories(common PUBLIC .)