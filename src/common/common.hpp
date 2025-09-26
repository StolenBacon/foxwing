#pragma once

#include <iostream>

#define SERVER_LOG(msg) std::cout << "[Server] " << msg << std::endl;
#define CLIENT_LOG(msg) std::cout << "[Client] " << msg << std::endl;