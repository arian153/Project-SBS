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
#include "../Data/MeshData.hpp"

namespace Engine
{
    class MeshDataGenerator
    {
    public:
        static MeshData CreateBox(Real width, Real height, Real depth, Uint32 num_subdivisions);
        static MeshData CreateSphere(Real radius, Uint32 slice_count, Uint32 stack_count);
        static MeshData CreateGeodesicSphere(Real radius, Uint32 subdivisions);
        static MeshData CreateCylinder(Real bottom_radius, Real top_radius, Real height, Uint32 slice_count, Uint32 stack_count);
        static MeshData CreateCapsule(Real radius,  Real height, Uint32 slice_count, Uint32 stack_count);
        static MeshData CreateGrid(Real width, Real depth, Uint32 m, Uint32 n);
        static MeshData CreateQuad(Real x, Real y, Real w, Real h, Real depth);
        static MeshData CreateRectangle(Real w, Real h);

    private:
        static SkinnedVertex MidPoint(const SkinnedVertex& v0, const SkinnedVertex& v1);

        static void Subdivide(MeshData& mesh_data);
        static void BuildCylinderTopCap(Real top_radius, Real height, Uint32 slice_count, MeshData& mesh_data);
        static void BuildCylinderBottomCap(Real bottom_radius, Real height, Uint32 slice_count, MeshData& mesh_data);
        static void BuildCapsuleSphere(Real radius, Real height, Uint32 slice_count, MeshData& mesh_data);
    };
}
