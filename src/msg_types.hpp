#pragma once

constexpr uint16_t NET_PROTOCOL_VERSION = 1;

enum class MsgType : uint8_t
{
    UNKNOWN,

    ERROR,

    C2S_SERVER_INFO,
    S2C_SERVER_INFO,

    C2S_CONNECT,
    S2C_CONNECT,

    C2S_DISCONNECT,
    S2C_DISCONNECT
};

constexpr const char* MsgTypeString(MsgType type)
{
    switch (type)
    {
        case MsgType::ERROR:
            return "Error";
        case MsgType::C2S_SERVER_INFO:
            return "Server Info";
        case MsgType::S2C_SERVER_INFO:
            return "Server Info";
        case MsgType::C2S_CONNECT:
            return "Connect";
        case MsgType::S2C_CONNECT:
            return "Connect";
        case MsgType::C2S_DISCONNECT:
            return "Disconnect";
        case MsgType::S2C_DISCONNECT:
            return "Disconnect";
        default:
            return "Unknown";
    }
}

enum class MsgError : uint8_t
{
    UNKNOWN,
    VERSION_MISMATCH,
    NO_FREE_SLOTS,
};

constexpr const char* MsgErrorString(MsgError type)
{
    switch (type)
    {
        case MsgError::VERSION_MISMATCH:
            return "Version Mismatch";
        default:
            return "Unknown";
    }
}