#include "settings.hpp"

#include "logger.hpp"

Settings g_settings;

void Settings::parse_cmdline_args(int argc, char** argv)
{
    for (int i = 0; i < argc; i++)
    {
        LOG("ARG " << i << " " << argv[i]);
    }
}