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
        void Init(const std::vector<ColorVertex>& vertices, const std::vector<Uint32>& indices);
        void Update(const std::vector<ColorVertex>& vertices) const;
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
