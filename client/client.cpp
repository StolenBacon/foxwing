#include <thread>
#include <chrono>

#include <core/engine.hpp>
#include <core/net/msg.hpp>
#include <core/platform/platform.hpp>
#include <core/util/logger.hpp>
#include <core/net/net.hpp>

#include <raylib.h>

using namespace core;

/* Server library */
static void* server_lib_handle = nullptr;

typedef void (*ServerInitFn)(void);
typedef void (*ServerTickFn)(void);
typedef void (*ServerQuitFn)(void);

static ServerInitFn server_init_fn;
static ServerTickFn server_tick_fn;
static ServerQuitFn server_quit_fn;

Engine g_engine;

static std::thread local_server_thread;
static std::atomic<bool> local_server_thread_stop_flag{false};
void LocalServerWorker()
{
    server_init_fn();

    while (!local_server_thread_stop_flag)
    {
        server_tick_fn();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    server_quit_fn();
}

void StartLocalServer()
{
    LOG_INFO("StartLocalServer");
    local_server_thread_stop_flag = false;
    local_server_thread = std::thread(LocalServerWorker);
}

void StopLocalServer()
{
    local_server_thread_stop_flag = true;
    if (local_server_thread.joinable())
    {
        local_server_thread.join();
    }
    LOG_INFO("StopLocalServer");
}

void OnMessageRecv(net::Msg& msg)
{
    LOG_INFO("OnMsgRecv");
}

static bool Init(void)
{
    // Initialize local server dynamic library
    auto handle = platform::DlOpen("server");

    if (!handle)
    {
        LOG_INFO("Failed to load server library.");
        return false;
    }

    server_init_fn = (ServerInitFn)platform::DlSym(handle, "ServerInit");
    server_tick_fn = (ServerTickFn)platform::DlSym(handle, "ServerTick");
    server_quit_fn = (ServerQuitFn)platform::DlSym(handle, "ServerQuit");

    if (!net::Init())
    {
        LOG_INFO("Failed to initialize networking");
        return false;
    }

    // Initialize display
    InitWindow(800, 600, "Foxwing");

    return true;
}

static void Quit(void)
{
    CloseWindow();

    StopLocalServer();

    // Close the server dynamic library
    platform::DlClose(server_lib_handle);
}

int main(void)
{
    SetTraceLogLevel(LOG_NONE);

    if (!Init())
    {
        LOG_INFO("Failed to initialize client");
        return -1;
    }

    SetTargetFPS(60);

    StartLocalServer();

    net::SetMessageHandler(OnMessageRecv);

    net::Connect("", 27015);

    while (!WindowShouldClose())
    {
        net::Service();

        if (IsKeyPressed(KEY_A))
        {
            net::Msg msg(net::MsgType::kServerInfoRes);
            net::Send(msg);
        }

        // Render
        BeginDrawing();
        EndDrawing();
    }

    Quit();

    return 0;
}