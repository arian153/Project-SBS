/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#pragma once
#include "../../EngineDefine.hpp"

namespace Engine
{
    class Color
    {
    public:
        Color();
        Color(Real red, Real green, Real blue, Real alpha);

        Real*       Data();
        const Real* Data() const;

    public:
        union
        {
            Real data[4] = { 1, 1, 1, 1 };

            struct
            {
                Real r;
                Real g;
                Real b;
                Real a;
            };
        };
    };

    enum class eColorInfo : Uint32
    {
        Aquamarine,
        Black,
        Blue,
        BlueViolet,
        Brown,
        Chocolate,
        Coral,
        Crimson,
        Cyan,
        DarkBlue,
        DarkCyan,
        DarkGray,
        DarkGreen,
        DarkMagenta,
        DarkOrange,
        DarkRed,
        DarkViolet,
        DeepPink,
        DeepSkyBlue,
        GhostWhite,
        Gold,
        Gray,
        Green,
        GreenYellow,
        HotPink,
        IndianRed,
        Indigo,
        Ivory,
        Khaki,
        LightBlue,
        LightCyan,
        LightGreen,
        LightGray,
        LightPink,
        LightSkyBlue,
        LightYellow,
        Lime,
        LimeGreen,
        Magenta,
        MidnightBlue,
        Navy,
        Orange,
        OrangeRed,
        Pink,
        Plum,
        Purple,
        Red,
        RoyalBlue,
        SaddleBrown,
        SandyBrown,
        Silver,
        SkyBlue,
        Snow,
        Transparent,
        Violet,
        White,
        Yellow,
        YellowGreen,
        LastColor
    };

    constexpr Uint32 COLOR_INFO_COUNT = static_cast<Uint32>(eColorInfo::LastColor);

    const Color COLOR[] = {
        Color(0.4980f, 1.0000f, 0.8313f, 1.0f),
        Color(0.0000f, 0.0000f, 0.0000f, 1.0f),
        Color(0.0000f, 0.0000f, 1.0000f, 1.0f),
        Color(0.5411f, 0.1686f, 0.8862f, 1.0f),
        Color(0.6470f, 0.1647f, 0.1647f, 1.0f),
        Color(0.8235f, 0.4117f, 0.1176f, 1.0f),
        Color(1.0000f, 0.4980f, 0.3137f, 1.0f),
        Color(0.8627f, 0.0784f, 0.2352f, 1.0f),
        Color(0.0000f, 1.0000f, 1.0000f, 1.0f),
        Color(0.0000f, 0.0000f, 0.5450f, 1.0f),
        Color(0.0000f, 0.5450f, 0.5450f, 1.0f),
        Color(0.6627f, 0.6627f, 0.6627f, 1.0f),
        Color(0.0000f, 0.3921f, 0.0000f, 1.0f),
        Color(0.5450f, 0.0000f, 0.5450f, 1.0f),
        Color(1.0000f, 0.5490f, 0.0000f, 1.0f),
        Color(0.5450f, 0.0000f, 0.0000f, 1.0f),
        Color(0.5803f, 0.0000f, 0.8274f, 1.0f),
        Color(1.0000f, 0.0784f, 0.5764f, 1.0f),
        Color(0.0000f, 0.7490f, 1.0000f, 1.0f),
        Color(0.9725f, 0.9725f, 1.0000f, 1.0f),
        Color(1.0000f, 0.8431f, 0.0000f, 1.0f),
        Color(0.5019f, 0.5019f, 0.5019f, 1.0f),
        Color(0.0000f, 0.5019f, 0.0000f, 1.0f),
        Color(0.6784f, 1.0000f, 0.1843f, 1.0f),
        Color(1.0000f, 0.4117f, 0.7058f, 1.0f),
        Color(0.8039f, 0.3607f, 0.3607f, 1.0f),
        Color(0.2941f, 0.0000f, 0.5098f, 1.0f),
        Color(1.0000f, 1.0000f, 0.9411f, 1.0f),
        Color(0.9411f, 0.9019f, 0.5490f, 1.0f),
        Color(0.6784f, 0.8470f, 0.9019f, 1.0f),
        Color(0.8784f, 1.0000f, 1.0000f, 1.0f),
        Color(0.5647f, 0.9333f, 0.5647f, 1.0f),
        Color(0.8274f, 0.8274f, 0.8274f, 1.0f),
        Color(1.0000f, 0.7137f, 0.7568f, 1.0f),
        Color(0.5294f, 0.8078f, 0.9803f, 1.0f),
        Color(1.0000f, 1.0000f, 0.8784f, 1.0f),
        Color(0.0000f, 1.0000f, 0.0000f, 1.0f),
        Color(0.1960f, 0.8039f, 0.1960f, 1.0f),
        Color(1.0000f, 0.0000f, 1.0000f, 1.0f),
        Color(0.0980f, 0.0980f, 0.4392f, 1.0f),
        Color(0.0000f, 0.0000f, 0.5019f, 1.0f),
        Color(1.0000f, 0.6470f, 0.0000f, 1.0f),
        Color(1.0000f, 0.2705f, 0.0000f, 1.0f),
        Color(1.0000f, 0.7529f, 0.7960f, 1.0f),
        Color(0.8666f, 0.6274f, 0.8666f, 1.0f),
        Color(0.5019f, 0.0000f, 0.5019f, 1.0f),
        Color(1.0000f, 0.0000f, 0.0000f, 1.0f),
        Color(0.2549f, 0.4117f, 0.8823f, 1.0f),
        Color(0.5450f, 0.2705f, 0.0745f, 1.0f),
        Color(0.9568f, 0.6431f, 0.3764f, 1.0f),
        Color(0.7529f, 0.7529f, 0.7529f, 1.0f),
        Color(0.5294f, 0.8078f, 0.9215f, 1.0f),
        Color(1.0000f, 0.9803f, 0.9803f, 1.0f),
        Color(0.0000f, 0.0000f, 0.0000f, 0.0f),
        Color(0.9333f, 0.5098f, 0.9333f, 1.0f),
        Color(1.0000f, 1.0000f, 1.0000f, 1.0f),
        Color(1.0000f, 1.0000f, 0.0000f, 1.0f),
        Color(0.6039f, 0.8039f, 0.1960f, 1.0f)
    };

    class ColorFlag
    {
    public:
        bool  b_flag = false;
        Color color  = Color(1, 1, 1, 1);
    };
}

namespace Engine::X11ColorData
{
    const Color AQUAMARINE     = Color(0.4980f, 1.0000f, 0.8313f, 1.0f);
    const Color BLACK          = Color(0.0000f, 0.0000f, 0.0000f, 1.0f);
    const Color BLUE           = Color(0.0000f, 0.0000f, 1.0000f, 1.0f);
    const Color BLUE_VIOLET    = Color(0.5411f, 0.1686f, 0.8862f, 1.0f);
    const Color BROWN          = Color(0.6470f, 0.1647f, 0.1647f, 1.0f);
    const Color CHOCOLATE      = Color(0.8235f, 0.4117f, 0.1176f, 1.0f);
    const Color CORAL          = Color(1.0000f, 0.4980f, 0.3137f, 1.0f);
    const Color CRIMSON        = Color(0.8627f, 0.0784f, 0.2352f, 1.0f);
    const Color CYAN           = Color(0.0000f, 1.0000f, 1.0000f, 1.0f);
    const Color DARK_BLUE      = Color(0.0000f, 0.0000f, 0.5450f, 1.0f);
    const Color DARK_CYAN      = Color(0.0000f, 0.5450f, 0.5450f, 1.0f);
    const Color DARK_GRAY      = Color(0.6627f, 0.6627f, 0.6627f, 1.0f);
    const Color DARK_GREEN     = Color(0.0000f, 0.3921f, 0.0000f, 1.0f);
    const Color DARK_MAGENTA   = Color(0.5450f, 0.0000f, 0.5450f, 1.0f);
    const Color DARK_ORANGE    = Color(1.0000f, 0.5490f, 0.0000f, 1.0f);
    const Color DARK_RED       = Color(0.5450f, 0.0000f, 0.0000f, 1.0f);
    const Color DARK_VIOLET    = Color(0.5803f, 0.0000f, 0.8274f, 1.0f);
    const Color DEEP_PINK      = Color(1.0000f, 0.0784f, 0.5764f, 1.0f);
    const Color DEEP_SKY_BLUE  = Color(0.0000f, 0.7490f, 1.0000f, 1.0f);
    const Color GHOST_WHITE    = Color(0.9725f, 0.9725f, 1.0000f, 1.0f);
    const Color GOLD           = Color(1.0000f, 0.8431f, 0.0000f, 1.0f);
    const Color GRAY           = Color(0.5019f, 0.5019f, 0.5019f, 1.0f);
    const Color GREEN          = Color(0.0000f, 0.5019f, 0.0000f, 1.0f);
    const Color GREEN_YELLOW   = Color(0.6784f, 1.0000f, 0.1843f, 1.0f);
    const Color HOT_PINK       = Color(1.0000f, 0.4117f, 0.7058f, 1.0f);
    const Color INDIAN_RED     = Color(0.8039f, 0.3607f, 0.3607f, 1.0f);
    const Color INDIGO         = Color(0.2941f, 0.0000f, 0.5098f, 1.0f);
    const Color IVORY          = Color(1.0000f, 1.0000f, 0.9411f, 1.0f);
    const Color KHAKI          = Color(0.9411f, 0.9019f, 0.5490f, 1.0f);
    const Color LIGHT_BLUE     = Color(0.6784f, 0.8470f, 0.9019f, 1.0f);
    const Color LIGHT_CYAN     = Color(0.8784f, 1.0000f, 1.0000f, 1.0f);
    const Color LIGHT_GREEN    = Color(0.5647f, 0.9333f, 0.5647f, 1.0f);
    const Color LIGHT_GRAY     = Color(0.8274f, 0.8274f, 0.8274f, 1.0f);
    const Color LIGHT_PINK     = Color(1.0000f, 0.7137f, 0.7568f, 1.0f);
    const Color LIGHT_SKY_BLUE = Color(0.5294f, 0.8078f, 0.9803f, 1.0f);
    const Color LIGHT_YELLOW   = Color(1.0000f, 1.0000f, 0.8784f, 1.0f);
    const Color LIME           = Color(0.0000f, 1.0000f, 0.0000f, 1.0f);
    const Color LIME_GREEN     = Color(0.1960f, 0.8039f, 0.1960f, 1.0f);
    const Color MAGENTA        = Color(1.0000f, 0.0000f, 1.0000f, 1.0f);
    const Color MIDNIGHT_BLUE  = Color(0.0980f, 0.0980f, 0.4392f, 1.0f);
    const Color NAVY           = Color(0.0000f, 0.0000f, 0.5019f, 1.0f);
    const Color ORANGE         = Color(1.0000f, 0.6470f, 0.0000f, 1.0f);
    const Color ORANGE_RED     = Color(1.0000f, 0.2705f, 0.0000f, 1.0f);
    const Color PINK           = Color(1.0000f, 0.7529f, 0.7960f, 1.0f);
    const Color PLUM           = Color(0.8666f, 0.6274f, 0.8666f, 1.0f);
    const Color PURPLE         = Color(0.5019f, 0.0000f, 0.5019f, 1.0f);
    const Color RED            = Color(1.0000f, 0.0000f, 0.0000f, 1.0f);
    const Color ROYAL_BLUE     = Color(0.2549f, 0.4117f, 0.8823f, 1.0f);
    const Color SADDLE_BROWN   = Color(0.5450f, 0.2705f, 0.0745f, 1.0f);
    const Color SANDY_BROWN    = Color(0.9568f, 0.6431f, 0.3764f, 1.0f);
    const Color SILVER         = Color(0.7529f, 0.7529f, 0.7529f, 1.0f);
    const Color SKY_BLUE       = Color(0.5294f, 0.8078f, 0.9215f, 1.0f);
    const Color SNOW           = Color(1.0000f, 0.9803f, 0.9803f, 1.0f);
    const Color TRANSPARENT_   = Color(0.0000f, 0.0000f, 0.0000f, 0.0f);
    const Color VIOLET         = Color(0.9333f, 0.5098f, 0.9333f, 1.0f);
    const Color WHITE          = Color(1.0000f, 1.0000f, 1.0000f, 1.0f);
    const Color YELLOW         = Color(1.0000f, 1.0000f, 0.0000f, 1.0f);
    const Color YELLOW_GREEN   = Color(0.6039f, 0.8039f, 0.1960f, 1.0f);
}
