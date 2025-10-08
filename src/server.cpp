#include "server.hpp"

#include <chrono>
#include <thread>
#include <queue>
#include <unordered_map>
#include <functional>
#include <raymath.h>

#include "logger.hpp"
#include "msg.hpp"
#include "util.hpp"

void Server::start()
{
    using clock = std::chrono::steady_clock;

    m_running = true;
    m_state = ServerState::INACTIVE;

    // const std::chrono::duration<double> tick_interval(1.0 / m_tick_rate);

    // const double tick_rate = 1.0;
    // const double fixed_delta = 1.0 / tick_rate;  // seconds per tick

    /*auto next_tick = std::chrono::steady_clock::now();*/
    std::chrono::duration<double> accumulator(0.0);
    auto previous = clock::now();

    while (m_running)
    {
        auto now = std::chrono::steady_clock::now();
        auto frame_time = now - previous;
        previous = now;
        accumulator += frame_time;

        while (accumulator.count() >= m_fixed_delta)
        {
            this->tick();
            accumulator -= std::chrono::duration<double>(m_fixed_delta);
        }

        // Optional: sleep a little to avoid 100% CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    SERVER_LOG("SERVER QUIT!!!");
}

void Server::quit()
{
    m_running = false;
}

void Server::set_tick_rate(double tick_rate)
{
    m_tick_rate = tick_rate;
    m_fixed_delta = 1.0 / m_tick_rate;
}

static Vector3 cube_position = {0.0f, 0.0f, 0.0f};

void Server::tick()
{
    // Handle network messages
    while (!m_messages_to_server.empty())
    {
        auto msg = m_messages_to_server.front();

        // Get msg type
        MsgType type;
        msg.Read(type);

        SERVER_LOG("Msg From Client: " << MsgTypeString(msg.type()));

        if (m_handlers.count(type))
        {
            m_handlers[type](msg);
        }

        m_messages_to_server.pop();  // Remove it
    }

    cube_position.y += 0.05f;
    if (cube_position.y > 10.0f)
    {
        cube_position.y = 0.0f;
    }

    Msg reply(MsgType::S2C_ENTITY);
    reply.Write(cube_position);
    m_messages_to_client.push(reply);

    if (m_state == ServerState::INACTIVE)
    {
        return;
    }
}

#if 0
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

static std::unordered_map<MsgType, std::function<void(Msg&)>> handlers = {{MsgType::C2S_SERVER_INFO, handleServerInfo},
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

#endif