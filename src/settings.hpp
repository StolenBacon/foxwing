#pragma once

#include <string>

struct Settings
{
    bool dedicated_server_only = false;  // Should the executable run as a dedicated server only
    std::string player_name = "Player";

    // Graphics settings
    unsigned int window_width = 800;
    unsigned int window_height = 600;
    bool window_fullscreen = false;

    // Server specific settings
    std::string server_host = "0.0.0.0";
    unsigned int server_port = 27015;

    void parse_cmdline_args(int argc, char** argv);
};