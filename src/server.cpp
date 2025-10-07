#include "server.hpp"

#include <queue>
#include <unordered_map>
#include <functional>

#include "logger.hpp"
#include "msg.hpp"
#include "util.hpp"

Server _server;

extern std::queue<Msg> g_to_server;
extern std::queue<Msg> g_to_client;

void ServerInit(void) {}

void handleServerInfo(Msg& pkt)
{
    SERVER_LOG("handleServerInfo()");
    Msg reply(MsgType::S2C_SERVER_INFO);
    reply.Write(GetBuildHash());
    g_to_client.push(reply);
}

void handleConnect(Msg& msg)
{
    SERVER_LOG("handleConnect()");

    uint32_t client_build_hash;
    std::string client_player_name;
    
    msg.Read(client_build_hash);
    msg.Read(client_player_name);

    if (client_build_hash != GetBuildHash())
    {
        SERVER_LOG("Server / Client Version Mismatch!!!");
        Msg reply(MsgType::ERROR);
        reply.Write(MsgError::VERSION_MISMATCH);
        g_to_client.push(reply);
        return;
    }

    // Find a client slow
    for (size_t i = 0; i < 32; i++)
    {
        if (_server.clients[i].state == NetClientState::FREE)
        {
            _server.clients[i].name = client_player_name;
            _server.clients[i].state = NetClientState::CONNECTED;

            Msg reply(MsgType::S2C_CONNECT);
            reply.Write(i);
            g_to_client.push(reply);
            return;
        }
    }

    // We could not find a free slot
    Msg reply(MsgType::ERROR);
    reply.Write(MsgError::NO_FREE_SLOTS);
    g_to_client.push(reply);
}

void handleDisconnect(const Msg& pkt)
{
    SERVER_LOG("handleDisconnect()");
}

static std::unordered_map<MsgType, std::function<void(Msg&)>> handlers = {
    {MsgType::C2S_SERVER_INFO, handleServerInfo},
    {MsgType::C2S_CONNECT, handleConnect},
    {MsgType::C2S_DISCONNECT, handleDisconnect}};


void ServerTick(void)
{
    while (!g_to_server.empty())
    {
        auto msg = g_to_server.front();

        // Get msg type
        MsgType type;
        msg.Read(type);

        SERVER_LOG("Msg From Client: " << MsgTypeString(msg.type()));

        if (handlers.count(type))
        {
            handlers[type](msg);
        }

        g_to_server.pop();  // Remove it
    }
}

void ServerQuit(void) {}