#include "SimpleMesh.hpp"

#include "Material.hpp"
#include "../RenderSystem.hpp"
#include "../GraphicsDefine.hpp"
#include "../DirectX12/Buffer/ConstantBuffer.hpp"
#include "../DirectX12/TableDescriptorHeap.hpp"
#include "../DirectX12/Texture.hpp"

namespace Engine
{
    void SimpleMesh::Init(const std::vector<ColorVertex>& vertices, const std::vector<Uint32>& indices)
    {
        m_vertex_buffer.Initialize(vertices, true);
        m_index_buffer.Initialize(indices);
    }

    void SimpleMesh::Update(const std::vector<ColorVertex>& vertices) const
    {
        m_vertex_buffer.Update(vertices);
    }

    void SimpleMesh::Render() const
    {
        CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        m_vertex_buffer.Bind();
        m_index_buffer.Bind();

        CONSTANT_BUFFER(eConstantBufferType::Transform)->PushData(&m_offset, sizeof(m_offset));
        m_material->Bind();

        DESCRIPTOR_HEAP->CommitTable();
        m_index_buffer.Draw();
    }

    void SimpleMesh::SetOffset(const Vector4& offset)
    {
        m_offset = offset;
    }

    void SimpleMesh::SetMaterial(SPtr<Material> material)
    {
        m_material = material;
    }
}
