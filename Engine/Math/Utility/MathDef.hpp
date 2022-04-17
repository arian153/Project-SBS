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
#include <codecvt>
#include <limits>
#include "../../EngineDefine.hpp"

namespace Engine::Math
{
    constexpr Real PI         = static_cast<Real>(3.14159265358979323846264);
    constexpr Real HALF_PI    = PI * static_cast<Real>(0.5);
    constexpr Real QUARTER_PI = PI * static_cast<Real>(0.25);
    constexpr Real TWO_PI     = PI * static_cast<Real>(2);
    constexpr Real PI_DIV_2   = HALF_PI;
    constexpr Real PI_DIV_3   = PI / static_cast<Real>(3);
    constexpr Real PI_DIV_4   = QUARTER_PI;
    constexpr Real PI_DIV_6   = PI / static_cast<Real>(6);
    constexpr Real RADIAN     = PI / static_cast<Real>(180);

    constexpr Real EPSILON         = static_cast<Real>(0.00001);
    constexpr Real EPSILON_SQUARED = EPSILON * EPSILON;
    constexpr Real EPSILON_BIAS    = static_cast<Real>(1.00001);

    constexpr Real REAL_MAX = FLT_MAX;
    constexpr Real REAL_MIN = FLT_MIN;

    constexpr Real REAL_POSITIVE_MAX = REAL_MAX; //go to +max
    constexpr Real REAL_POSITIVE_MIN = REAL_MIN; //near to 0
    constexpr Real REAL_NEGATIVE_MAX = -REAL_MAX; //go to -max
    constexpr Real REAL_NEGATIVE_MIN = -REAL_MIN; //near to 0

    constexpr Real ROOT_TWO        = static_cast<Real>(1.41421356237309504880168);
    constexpr Real ROOT_THREE      = static_cast<Real>(1.73205080756887729352744);
    constexpr Real ROOT_FIVE       = static_cast<Real>(2.23606797749978969640917);
    constexpr Real ROOT_TEN        = static_cast<Real>(3.16227766016837933199889);
    constexpr Real CUBE_ROOT_TWO   = static_cast<Real>(1.25992104989487316476721);
    constexpr Real CUBE_ROOT_THREE = static_cast<Real>(1.25992104989487316476721);
    constexpr Real FORTH_ROOT_TWO  = static_cast<Real>(1.18920711500272106671749);

    constexpr Real LN_TWO   = static_cast<Real>(0.69314718055994530941723);
    constexpr Real LN_THREE = static_cast<Real>(1.09861228866810969139524);
    constexpr Real LN_TEN   = static_cast<Real>(2.30258509299404568401799);

    const Real FNAN = std::numeric_limits<Real>::quiet_NaN();
    const Real INF  = std::numeric_limits<Real>::infinity();
}
