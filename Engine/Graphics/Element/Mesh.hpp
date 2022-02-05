#pragma once
#include "../Data/MeshData.hpp"
#include "../DirectX12/Buffer/IndexBuffer.hpp"
#include "../DirectX12/Buffer/VertexBuffer.hpp"

namespace Engine
{
    class Mesh
    {
    public:
        void Init(const MeshData& mesh_data);
        void Update(const MeshData& mesh_data) const;
        void Shutdown();

        void Bind() const;
        void Render() const;

    private:
        eVertexType m_vertex_type = eVertexType::SkinnedVertex;

        VertexBuffer m_vertex_buffer;
        IndexBuffer  m_index_buffer;
    };
}
