#pragma once

#include <string>

struct GraphicsSettings
{
    unsigned int width = 800;
    unsigned int height = 600;
    bool fullscreen = false;
};

struct SoundSettings
{
    float volume = 1.0f;
    float music_volume = 1.0f;
};

struct ServerSettings
{
    std::string host = "0.0.0.0";
    unsigned int port = 27015;
};

struct Settings
{
    GraphicsSettings graphics;
    SoundSettings sound;
    ServerSettings server;
    std::string player_name = "Player";
};