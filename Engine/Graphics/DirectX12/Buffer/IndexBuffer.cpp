#include "IndexBuffer.hpp"

#include "../../GraphicsDefine.hpp"

namespace Engine
{
    IndexBuffer::IndexBuffer()
    {
    }

    IndexBuffer::~IndexBuffer()
    {
    }

    void IndexBuffer::Initialize(const std::vector<Uint32>& indices)
    {
        m_index_count      = static_cast<Uint32>(indices.size());
        Uint32 buffer_size = m_index_count * sizeof(Uint32);

        D3D12_HEAP_PROPERTIES heap_property = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        D3D12_RESOURCE_DESC   desc          = CD3DX12_RESOURCE_DESC::Buffer(buffer_size);

        DEVICE->CreateCommittedResource(
                                        &heap_property,
                                        D3D12_HEAP_FLAG_NONE,
                                        &desc,
                                        D3D12_RESOURCE_STATE_GENERIC_READ,
                                        nullptr,
                                        IID_PPV_ARGS(&m_index_buffer));

        void*         index_data_buffer = nullptr;
        CD3DX12_RANGE read_range(0, 0); // We do not intend to read from this resource on the CPU.
        m_index_buffer->Map(0, &read_range, &index_data_buffer);
        ::memcpy(index_data_buffer, &indices[0], buffer_size);
        m_index_buffer->Unmap(0, nullptr);

        m_index_buffer_view.BufferLocation = m_index_buffer->GetGPUVirtualAddress();
        m_index_buffer_view.Format         = DXGI_FORMAT_R32_UINT;
        m_index_buffer_view.SizeInBytes    = buffer_size;
    }

    void IndexBuffer::Shutdown()
    {
        RELEASE_COM(m_index_buffer);
    }

    void IndexBuffer::Bind() const
    {
        CMD_LIST->IASetIndexBuffer(&m_index_buffer_view);
    }

    void IndexBuffer::Draw() const
    {
        CMD_LIST->DrawIndexedInstanced(m_index_count, 1, 0, 0, 0);
    }

    void IndexBuffer::Draw(Uint32 instance_count) const
    {
        CMD_LIST->DrawIndexedInstanced(m_index_count, instance_count, 0, 0, 0);
    }
}
