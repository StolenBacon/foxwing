#pragma once

#include <array>
#include <queue>
#include <unordered_map>
#include <functional>

#include "logger.hpp"

#include "msg.hpp"
#include "net_client.hpp"

#define MAX_CLIENTS 32

enum class ServerState
{
    INACTIVE,
    LOADING,
    ACTIVE,
    PAUSED
};

class Server
{
public:
    Server(double tick_rate, unsigned int max_players) : m_max_players{max_players} 
    {
        set_tick_rate(tick_rate);
        register_handlers();
    }

    void start();
    void quit();

    void set_tick_rate(double tick_rate);

    inline ServerState& state() { return m_state; }
    inline std::queue<Msg>& messages_to_server() { return m_messages_to_server; }
    inline std::queue<Msg>& messages_to_client() { return m_messages_to_client; }

private:
    void tick();

    // Network message handlers
    void register_handlers();
    void handle_server_info_msg(Msg& msg);
    void handle_connect_msg(Msg& msg);
    void handle_disconnect_msg(Msg& msg);
private:
    bool m_running;
    ServerState m_state = ServerState::INACTIVE;
    
    double m_tick_rate;
    double m_fixed_delta; // Seconds per tick

    unsigned int m_max_players;
    std::array<NetClient, MAX_CLIENTS> m_clients;

    // Queues for local client communication
    std::queue<Msg> m_messages_to_server;
    std::queue<Msg> m_messages_to_client;

    // Network message handlers
    std::unordered_map<MsgType, std::function<void(Msg&)>> m_handlers;

};

#if 0
void ServerInit(void);

void ServerTick(void);

void ServerQuit(void);
#endif