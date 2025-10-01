#include <thread>
#include <chrono>

#include <core/util/logger.hpp>
#include <core/platform/platform.hpp>

using namespace core;

static void* server_lib_handle = nullptr;

typedef void (*ServerInitFn)(void);
typedef void (*ServerTickFn)(void);
typedef void (*ServerQuitFn)(void);

static ServerInitFn server_init_fn;
static ServerTickFn server_tick_fn;
static ServerQuitFn server_quit_fn;

int main()
{
    auto handle = platform::DlOpen("server");

    if (!handle)
    {
        LOG_INFO("Failed to load server library.");
        return -1;
    }

    server_init_fn = (ServerInitFn)platform::DlSym(handle, "ServerInit");
    server_tick_fn = (ServerTickFn)platform::DlSym(handle, "ServerTick");
    server_quit_fn = (ServerQuitFn)platform::DlSym(handle, "ServerQuit");

    server_init_fn();

    while (1)
    {
        server_tick_fn();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    server_quit_fn();
    return 0;
}