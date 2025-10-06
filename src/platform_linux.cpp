#include <core/platform/platform.hpp>
#include <core/util/logger.hpp>

#ifdef FW_PLATFORM_LINUX

#include <dlfcn.h>

namespace core
{
namespace platform
{

void* DlOpen(const char* name)
{
    return dlopen(name, RTLD_LAZY);
}

void* DlSym(void* handle, const char* name)
{
    if (handle != nullptr)
    {
        return dlsym(handle, name);
    }
    return nullptr;
}

void DlClose(void* handle)
{
    dlclose(handle);
}

}  // namespace platform
}  // namespace core
#endif