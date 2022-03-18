#include "InstancingBuffer.hpp"

#include "../../GraphicsDefine.hpp"

namespace Engine
{
    InstancingBuffer::InstancingBuffer()
        : m_buffer_view()
    {
    }

    InstancingBuffer::~InstancingBuffer()
    {
    }

    void InstancingBuffer::Init(Uint32 max_count)
    {
        m_max_count = max_count;

        Uint32                buffer_size   = sizeof(InstancingParams) * max_count;
        D3D12_HEAP_PROPERTIES heap_property = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        D3D12_RESOURCE_DESC   desc          = CD3DX12_RESOURCE_DESC::Buffer(buffer_size);

        DEVICE->CreateCommittedResource(
                                        &heap_property,
                                        D3D12_HEAP_FLAG_NONE,
                                        &desc,
                                        D3D12_RESOURCE_STATE_GENERIC_READ,
                                        nullptr,
                                        IID_PPV_ARGS(&m_buffer));
    }

    void InstancingBuffer::Clear()
    {
        m_data.clear();
    }

    void InstancingBuffer::AddData(const InstancingParams& params)
    {
        m_data.push_back(params);
    }

    void InstancingBuffer::Bind()
    {
        Uint32 data_count = GetInstanceCount();
        if (data_count > m_max_count)
        {
            Init((data_count + 1) * 2);
        }

        Uint32      buffer_size = data_count * sizeof(InstancingParams);
        void*       data_buffer = nullptr;
        D3D12_RANGE read_range { 0, 0 };

        m_buffer->Map(0, &read_range, &data_buffer);
        memcpy(data_buffer, &m_data[0], buffer_size);
        m_buffer->Unmap(0, nullptr);

        m_buffer_view.BufferLocation = m_buffer->GetGPUVirtualAddress();
        m_buffer_view.StrideInBytes  = sizeof(InstancingParams);
        m_buffer_view.SizeInBytes    = buffer_size;
    }

    Uint32 InstancingBuffer::GetInstanceCount() const
    {
        return static_cast<Uint32>(m_data.size());
    }

    ComPtr<ID3D12Resource> InstancingBuffer::GetBuffer()
    {
        return m_buffer;
    }

    D3D12_VERTEX_BUFFER_VIEW InstancingBuffer::GetBufferView() const
    {
        return m_buffer_view;
    }
}
