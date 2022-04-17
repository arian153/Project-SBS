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
