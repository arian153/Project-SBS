#pragma once

#include "Color.hpp"
#include "../../EngineDefine.hpp"
#include "../../Math/Algebra/Vector2.hpp"
#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Algebra/Vector4.hpp"

namespace Engine
{
   
    class ColorVertex
    {
    public:
        Vector3 pos;
        Color   color;
        Vector2 tex;
    };

    class TexVertex
    {
    public:
        Vector3 pos;
        Vector2 tex;
    };

    class NormalVertex
    {
    public:
        Vector3 pos;
        Vector3 normal;
    };

    class LightVertex
    {
    public:
        Vector3 pos;
        Vector2 tex;
        Vector3 normal;
    };

    class GeneralVertex
    {
    public:
        Vector3 pos;
        Vector2 tex;
        Vector3 normal;
        Vector3 tangent;
        Vector3 binormal;
    };

    struct IVector4
    {
        Sint32 x = -1;
        Sint32 y = -1;
        Sint32 z = -1;
        Sint32 w = -1;
    };

    class SkinnedVertex
    {
    public:
        Vector3  pos;
        Vector2  tex;
        Vector3  normal;
        Vector3  tangent;
        Vector3  binormal;
        Vector4  weights;
        IVector4 indices;
    };
}
