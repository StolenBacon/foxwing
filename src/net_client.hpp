#pragma once

#include <string>

enum class NetClientState
{
    FREE,
    CONNECTED,
    DISCONNECTED
};

struct NetClient
{
    unsigned int id;
    int ping = 0;
    std::string name = "Player";
    NetClientState state = NetClientState::FREE;
};