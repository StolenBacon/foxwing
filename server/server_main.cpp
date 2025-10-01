#include <core/platform/platform.hpp>
#include <core/util/logger.hpp>
#include <core/common.hpp>
#include <core/engine.hpp>

#include <core/net/net.hpp>

Engine g_engine;

using namespace core;

void OnMessageRecv(net::Msg& msg)
{
    LOG_INFO("OnMsgRecv");
}

extern "C" FOXWING_EXPORT void ServerInit(void)
{
    LOG_INFO("Server Init");

    net::Init();
    net::SetMessageHandler(OnMessageRecv);

    if (!net::Host("", 27015))
    {
        LOG_INFO("Failed to create ENet host");
    }
}

extern "C" FOXWING_EXPORT void ServerTick(void)
{
    net::Service();
}

extern "C" FOXWING_EXPORT void ServerQuit(void)
{

}