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
        static MeshData CreateGrid(Real width, Real depth, Uint32 m, Uint32 n);
        static MeshData CreateQuad(Real x, Real y, Real w, Real h, Real depth);

    private:
        static SkinnedVertex MidPoint(const SkinnedVertex& v0, const SkinnedVertex& v1);

        static void Subdivide(MeshData& mesh_data);
        static void BuildCylinderTopCap(Real top_radius, Real height, Uint32 slice_count, MeshData& mesh_data);
        static void BuildCylinderBottomCap(Real bottom_radius, Real height, Uint32 slice_count, MeshData& mesh_data);
        static void CalculateBinormal(MeshData& mesh_data);
    };
}
