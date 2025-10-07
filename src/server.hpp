#pragma once

#include <array>
#include "net_client.hpp"

struct Server
{
    std::array<NetClient, 32> clients;
};

void ServerInit(void);

void ServerTick(void);

void ServerQuit(void);