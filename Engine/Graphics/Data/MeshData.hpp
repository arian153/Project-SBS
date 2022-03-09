#pragma once
#include "Vertex.hpp"

namespace Engine
{
    class Face
    {
    public:
        Face();
        Face(Uint32 a, Uint32 b, Uint32 c);

    public:
        Uint32 a;
        Uint32 b;
        Uint32 c;
    };

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

        void Set(const std::vector<PosVertex>& new_vertices);
        void Set(const std::vector<NormalVertex>& new_vertices);

    public:
        eVertexType                vertex_type = eVertexType::SkinnedVertex;
        std::vector<SkinnedVertex> vertices;
        std::vector<Uint32>        indices;
        std::vector<Face>          faces;
    };
}
