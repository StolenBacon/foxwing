#include "util.hpp"

constexpr auto BUILD_DATE = __DATE__;
constexpr auto BUILD_TIME = __TIME__;

std::string GetBuildDateTime()
{
    return std::string(BUILD_DATE) + " " + BUILD_TIME;
}

uint32_t GetBuildHash()
{
    return fnv1a(GIT_COMMIT_HASH);
}