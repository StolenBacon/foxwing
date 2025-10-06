#include <iostream>
#include <csignal>
#include <raylib.h>
#include <array>
#include <algorithm>
#include <queue>

#include "cvar.hpp"
#include "settings.hpp"
#include "server.hpp"
#include "msg.hpp"
#include "util.hpp"

extern Settings g_settings;

std::queue<Msg> g_to_server;
std::queue<Msg> g_to_client;

static void Init()
{
    InitWindow(g_settings.graphics.width, g_settings.graphics.height, "Foxwing");
    std::cout << "Client protocol version: " << NET_PROTOCOL_VERSION << "\n";
    
    Msg msg(MsgType::C2S_CONNECT);
    msg.Write(GetBuildHash());
    msg.Write(g_settings.player_name);

    g_to_server.push(msg);
}

static void Render(void)
{
    BeginDrawing();
    EndDrawing();
}

static void OnMessage(Msg& msg)
{
    std::cout << "Recieved message from server [" << MsgTypeString(msg.type()) << "]" << std::endl;

    auto type = msg.type();

    if (type == MsgType::S2C_SERVER_INFO)
    {
        uint32_t server_build_hash;
        msg.Read(server_build_hash);

        std::cout << "SERVER INFO" << std::endl;
        std::cout << "-- Client Build Hash --> " << GetBuildHash() << std::endl;
        std::cout << "-- Server Build Hash --> " << server_build_hash << std::endl;
    }
}

static void Tick(void)
{
    while (!g_to_client.empty())
    {
        auto msg = g_to_client.front();  // Access the front element
       
        MsgType type;
        msg.Read(type);

        if (type == MsgType::ERROR)
        {
            MsgError error;
            msg.Read(error);
            std::cout << "Error from server: '" << MsgErrorString(error) << "'\n";
        }
      
        OnMessage(msg);
        g_to_client.pop();  // Remove it
    }
}

static void Quit()
{
    CloseWindow();
}

int main(int argc, char** argv)
{
    Init();
    ServerInit();

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose())
    {
        Tick();
        ServerTick();
        Render();
    }

    Quit();
    ServerQuit();

    return 0;
}