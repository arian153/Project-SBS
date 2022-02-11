#pragma once

#include "Color.hpp"
#include "../../EngineDefine.hpp"
#include "../../Math/Algebra/Vector2.hpp"
#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Algebra/Vector4.hpp"

namespace Engine
{
    class PosVertex
    {
    public:
        Vector3 pos;
    };

    class ColorVertex
    {
    public:
        Vector3 pos;
        Color   color;
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
        Vector3 n;
    };

    class LightVertex
    {
    public:
        Vector3 pos;
        Vector2 tex;
        Vector3 n;
    };

    class GeneralVertex
    {
    public:
        Vector3 pos;
        Vector2 tex;
        Vector3 n; //normal
        Vector3 t; //tangent
        Vector3 b; //binormal or bitangent
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
        SkinnedVertex() = default;

        SkinnedVertex(Real px, Real py, Real pz, Real u, Real v, Real nx, Real ny, Real nz, Real tx, Real ty, Real tz)
            : pos(px, py, pz), tex(u, v), n(nx, ny, nz), t(tx, ty, tz)
        {
            b = CrossProduct(t, n).Normalize();
        }

    public:
        Vector3  pos;
        Vector2  tex;
        Vector3  n; //normal
        Vector3  t; //tangent
        Vector3  b; //binormal or bitangent
        Vector4  w; //weight
        IVector4 i; //bone-indices
    };

    enum class eVertexType
    {
        ColorVertex,
        PosVertex,
        TexVertex,
        NormalVertex,
        LightVertex,
        GeneralVertex,
        SkinnedVertex
    };
}
