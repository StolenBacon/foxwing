#pragma once

#include <string>

template <typename T>
class CVar
{
public:
    CVar(const std::string& name, const T& default_value) : m_name(name), m_value(default_value) {}

    T& Get() { return m_value; }
    void Set(const T& value) { m_value = value; }

private:
    std::string m_name;
    T m_value;
};