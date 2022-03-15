#pragma once
#include "../Data/MeshData.hpp"
#include "../DirectX12/Buffer/IndexBuffer.hpp"
#include "../DirectX12/Buffer/VertexBuffer.hpp"
#include "../Shader/ShaderInfo.hpp"

namespace Engine
{
    class InstancingBuffer;

    class Mesh
    {
    public:
        void Init(const MeshData& mesh_data);
        void Init(const MeshData& mesh_data, eTopologyType type);
        void Update(const MeshData& mesh_data) const;
        void Shutdown();

        void Render() const;
        void Render(const SPtr<InstancingBuffer>& instancing_buffer) const;

    private:
        eVertexType            m_vertex_type = eVertexType::SkinnedVertex;
        D3D_PRIMITIVE_TOPOLOGY m_topology    = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

        VertexBuffer m_vertex_buffer;
        IndexBuffer  m_index_buffer;
    };
}
