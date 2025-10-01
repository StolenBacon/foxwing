#include <core/platform/platform.hpp>
#include <core/util/logger.hpp>

#ifdef FW_PLATFORM_WINDOWS

#include <Windows.h>

namespace core { namespace platform {

void* DlOpen(const char* name) 
{
    void* handle = LoadLibraryA(name);
    return handle;
}

void* DlSym(void* handle, const char* name) 
{
    if (handle != nullptr) {
        return GetProcAddress((HMODULE) handle, name);
    }
    return nullptr;
}

void DlClose(void* handle) 
{
    LOG_INFO("Closing dynamic library");
    if (handle != nullptr) {
        FreeLibrary((HMODULE) handle);
    }
}

}}
#endif