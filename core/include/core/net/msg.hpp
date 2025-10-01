#pragma once

#include <string>
#include <vector>

#include "../util/logger.hpp"

#include "msg_types.hpp"

namespace core
{
namespace net
{

class Msg
{
public:
    Msg(MsgType type) : m_type(type) { this->Write(m_type); }

    Msg(uint8_t* data, size_t length) 
    { 
        this->m_payload.resize(length);
        std::memcpy(m_payload.data(), data, length);
        
        this->Read(m_type);

        LOG_INFO("MSG TYPE " << (int) m_type);
    }

    template <typename T>
    void Write(T value)
    {
        static_assert(std::is_trivially_copyable_v<T>, "Write only supports trivially copyable types");
        m_payload.insert(m_payload.end(), reinterpret_cast<uint8_t*>(&value),
                         reinterpret_cast<uint8_t*>(&value) + sizeof(value));
    }

    void Write(std::string& value)
    {
        // Get the length of the string and write it into the buffer first
        auto len = static_cast<uint16_t>(value.size());
        this->Write(len);

        // Write string data
        m_payload.insert(m_payload.end(), reinterpret_cast<const uint8_t*>(value.data()),
                         reinterpret_cast<const uint8_t*>(value.data()) + value.size());
    }

    void Write(const char* value)
    {
        std::string str(value);
        this->Write(str);
    }

    template <typename T>
    bool Read(T& out)
    {
        if (m_read_cursor + sizeof(T) > m_payload.size())
        {
            LOG_INFO("Msg:Read Attempted to read past the bounds ")
            return false;
        }
        std::memcpy(&out, m_payload.data() + m_read_cursor, sizeof(T));
        m_read_cursor += sizeof(T);
        return true;
    }

    template <>
    bool Read(std::string& out)
    {
        // Get string length
        uint16_t length;
        if (!Read(length)) return false;

        if (m_read_cursor + length > m_payload.size())
        {
            LOG_INFO("Msg:Read Attempted to read past the bounds ")
            return false;
        }

        out.assign(reinterpret_cast<const char*>(m_payload.data() + m_read_cursor), length);
        m_read_cursor += length;
        return true;
    }

    inline MsgType& type() { return m_type; }
    inline std::vector<uint8_t>& payload() { return m_payload; }

private:
    MsgType m_type;
    std::vector<uint8_t> m_payload;
    size_t m_read_cursor = 0;
};

}  // namespace net
}  // namespace core