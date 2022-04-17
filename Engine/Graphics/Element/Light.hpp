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
#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Algebra/Vector4.hpp"
#include "../Data/Color.hpp"

namespace Engine
{
    enum class eLightType : Uint32
    {
        DirectionalLight,
        PointLight,
        SpotLight,
    };

    struct LightColor
    {
        Color diffuse;
        Color ambient;
        Color specular;
    };

    struct LightInfo
    {
        LightColor color;
        Vector4    position;
        Vector4    direction;
        Uint32     light_type = 0;
        float      range      = 0.0f;
        float      angle      = 0.0f;
        Sint32     padding    = 0;
    };

    struct LightParams
    {
        Uint32    light_count = 0;
        Vector3   padding;
        LightInfo lights[50];
    };
}
