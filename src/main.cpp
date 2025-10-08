#include <iostream>
#include <csignal>
#include <raylib.h>
#include <array>
#include <algorithm>
#include <queue>
#include <thread>

#include "cvar.hpp"
#include "settings.hpp"
#include "server.hpp"
#include "msg.hpp"
#include "util.hpp"

static Server local_server(20.0f, 1);
static std::thread local_server_thread;

extern Settings g_settings;

std::queue<Msg> g_to_server;
std::queue<Msg> g_to_client;

Camera3D camera = {0};


static void Init()
{
    SetTraceLogLevel(LOG_NONE);  // Disable all raylib logging
    InitWindow(g_settings.window_width, g_settings.window_height, "Foxwing");

    camera.position = {10.0f, 10.0f, 10.0f};  // Camera position
    camera.target = {0.0f, 0.0f, 0.0f};       // Camera looking at point
    camera.up = {0.0f, 1.0f, 0.0f};           // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                               // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;            // Camera projection type

    DisableCursor();  

    Msg msg(MsgType::C2S_CONNECT);
    msg.Write(build_hash());
    std::string s = "StolenBacon";
    msg.Write(s);
    local_server.messages_to_server().push(msg);
}

static Vector3 cube_pos;

static void Render(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);
    BeginMode3D(camera);
        DrawCube(cube_pos, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires(cube_pos, 2.0f, 2.0f, 2.0f, MAROON);
    DrawGrid(10, 1.0f);

    EndMode3D();

    EndDrawing();
}

static void OnMessage(Msg& msg)
{
    // std::cout << "Recieved message from server [" << MsgTypeString(msg.type()) << "]" << std::endl;

    auto type = msg.type();

    if (type == MsgType::S2C_SERVER_INFO)
    {
        uint32_t server_build_hash;
        msg.Read(server_build_hash);

        std::cout << "SERVER INFO" << std::endl;
        std::cout << "-- Client Build Hash --> " << build_hash() << std::endl;
        std::cout << "-- Server Build Hash --> " << server_build_hash << std::endl;
    }
    else if (type == MsgType::S2C_ENTITY)
    {
        msg.Read(cube_pos);
        //std::cout << "CUBE POS = " << cube_pos.x << "," << cube_pos.y << "," << cube_pos.z << "\n";
    }
}

static void Tick(void)
{
    while (!local_server.messages_to_client().empty())
    {
        auto msg = local_server.messages_to_client().front();  // Access the front element
       
        MsgType type;
        msg.Read(type);

        if (type == MsgType::ERROR)
        {
            MsgError error;
            msg.Read(error);
            std::cout << "Error from server: '" << MsgErrorString(error) << "'\n";
        }
      
        OnMessage(msg);
        local_server.messages_to_client().pop();  // Remove it
    }

    UpdateCamera(&camera, CAMERA_FREE);
}

static void Quit()
{
    CloseWindow();
}

int main(int argc, char** argv)
{
    g_settings.parse_cmdline_args(argc, argv);

    Init();

    local_server_thread = std::thread([]() { local_server.start(); });

    // ServerInit();

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose())
    {
        Tick();
        // ServerTick();
        Render();
    }

    local_server.quit();

    Quit();
    
    local_server_thread.join();

    return 0;
}