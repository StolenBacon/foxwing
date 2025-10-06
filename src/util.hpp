#pragma once

#include <string>

std::string GetBuildDateTime();

uint32_t GetBuildHash();

constexpr uint32_t fnv1a(const char* str, uint32_t hash = 2166136261u)
{
    return *str ? fnv1a(str + 1, (hash ^ (uint8_t)(*str)) * 16777619u) : hash;
}