#pragma once

#include "../EngineDefine.hpp"

namespace Engine
{
    struct ApplicationSetting
    {
    public:
        bool b_confine_cursor = false;
        bool b_show_cursor    = true;
        bool b_vsync          = false;
        bool b_windowed       = true;

        int  screen_width  = 1280;
        int  screen_height = 720;
        Real screen_scale  = 1.0f;

        String caption       = "";
        String initial_level = "";
        String resource_path = "";
    };

    struct WindowInfo
    {
        HWND  hwnd       = nullptr;
        Sint32 width      = 200;
        Sint32 height     = 200;
        bool  b_windowed = true;
        bool  b_vsync    = false;
    };
}
