#pragma once
#include "Vertex.hpp"

namespace Engine
{
    class MeshData
    {
    public:
        void    SetNormalize(Vector3& min, Vector3& max);
        Vector3 GetFaceNormal(Uint32 a, Uint32 b, Uint32 c) const;

        void Convert(std::vector<PosVertex>& new_vertices) const;
        void Convert(std::vector<TexVertex>& new_vertices) const;
        void Convert(std::vector<NormalVertex>& new_vertices) const;
        void Convert(std::vector<LightVertex>& new_vertices) const;
        void Convert(std::vector<GeneralVertex>& new_vertices) const;

    public:
        eVertexType                vertex_type = eVertexType::SkinnedVertex;
        std::vector<SkinnedVertex> vertices;
        std::vector<Uint32>        indices;
    };
}
