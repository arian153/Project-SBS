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
        GeneralVertex() = default;

        GeneralVertex(Real px, Real py, Real pz, Real u, Real v, Real nx, Real ny, Real nz, Real tx, Real ty, Real tz)
            : pos(px, py, pz), tex(u, v), n(nx, ny, nz), t(tx, ty, tz)
        {
        }
    public:
        Vector3 pos;
        Vector2 tex;
        Vector3 n; //normal
        Vector3 t; //tangent
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
        }

    public:
        Vector3  pos;
        Vector2  tex;
        Vector3  n; //normal
        Vector3  t; //tangent
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
