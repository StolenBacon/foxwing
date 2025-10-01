#pragma once

#if defined(_WIN32)
	#define FW_PLATFORM_WINDOWS
#elif defined(__linux__)
	#define FW_PLATFORM_LINUX
#endif

namespace core { namespace platform {

void* DlOpen(const char* name);

void* DlSym(void* handle, const char* name);

void DlClose(void* handle);

}} // namespace core