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
