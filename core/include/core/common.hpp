#pragma once

#pragma once

#if defined(_WIN32) || defined(_WIN64)
	#define FOXWING_EXPORT __declspec(dllexport)
#else
	#define FOXWING_EXPORT __attribute__((visibility("default")))
#endif
