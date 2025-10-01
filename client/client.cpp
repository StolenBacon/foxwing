#include <core/platform/platform.hpp>
#include <core/util/logger.hpp>
#include <core/engine.hpp>

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

static bool Init(void) 
{
    // Initialize local server dynamic library
    auto handle = platform::DlOpen("server");

    if (!handle) {
        LOG_INFO("Failed to load server library.");
        return false;
    }

    server_init_fn = (ServerInitFn) platform::DlSym(handle, "ServerInit");
    server_tick_fn = (ServerTickFn) platform::DlSym(handle, "ServerTick");
    server_quit_fn = (ServerQuitFn) platform::DlSym(handle, "ServerQuit");

    server_init_fn();

    // Initialize display
    InitWindow(800, 600, "Foxwing");

    return true;
}

static void Quit(void) 
{
    CloseWindow();

    server_quit_fn();

    // Close the server dynamic library
    platform::DlClose(server_lib_handle);
}

int main(void) {
    
    SetTraceLogLevel(LOG_NONE);

    if (!Init()) {
        LOG_INFO("Failed to initialize client");
        return -1;
    }
    
    while (!WindowShouldClose()) {
        // Update
        server_tick_fn();

        // Render
        BeginDrawing();
        EndDrawing();
    }

    Quit();

    return 0;
}