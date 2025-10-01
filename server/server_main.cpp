#include <core/platform/platform.hpp>
#include <core/util/logger.hpp>
#include <core/common.hpp>
#include <core/engine.hpp>

Engine g_engine;

using namespace core;

extern "C" FOXWING_EXPORT void ServerInit(void) 
{
    LOG_INFO("Server Init");
}

extern "C" FOXWING_EXPORT void ServerTick(void) 
{
    LOG_INFO("Server Tick");
}

extern "C" FOXWING_EXPORT void ServerQuit(void) 
{
    LOG_INFO("Server Quit");
}