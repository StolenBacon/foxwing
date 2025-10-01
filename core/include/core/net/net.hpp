#pragma once

#include <functional>
#include <core/net/msg.hpp>

namespace core
{
namespace net
{

bool Init();

bool Host(const std::string& host, int port);

void Connect(const std::string& host, int port);

void Send(Msg& msg);

void Broadcast(const Msg& msg);

void Service();

void SetMessageHandler(std::function<void(core::net::Msg&)> handler);

}  // namespace net
}  // namespace core