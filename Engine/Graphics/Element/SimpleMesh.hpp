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
#include <vector>
#include "../DirectX12/DirectXIncludes.hpp"

#include "../Data/Vertex.hpp"
#include "../DirectX12/Buffer/IndexBuffer.hpp"
#include "../DirectX12/Buffer/VertexBuffer.hpp"

namespace Engine
{
    class Material;

    class SimpleMesh
    {
    public:
        void Init(const std::vector<TexVertex>& vertices, const std::vector<Uint32>& indices);
        void Update(const std::vector<TexVertex>& vertices) const;
        void Render() const;

        void SetOffset(const Vector4& offset);
        void SetMaterial(SPtr<Material> material);

    private:
        VertexBuffer m_vertex_buffer;
        IndexBuffer  m_index_buffer;

        SPtr<Material> m_material;

    private: //temp
        Vector4 m_offset;
    };
}
