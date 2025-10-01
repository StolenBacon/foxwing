#pragma once

#include <iostream>

#define LOG_INFO(msg) std::cout << msg << "\n";

#if defined(FOXWING_SERVER)
	#define LOG_INFO(msg) std::cout << "(Server) " <<  msg << "\n";
#elif defined(FOXWING_CLIENT)
	#define LOG_INFO(msg) std::cout << "(Client) " msg << "\n";
#else
	#define LOG_INFO(msg) std::cout << msg << "\n";
#endif

// TODO: add logging class?