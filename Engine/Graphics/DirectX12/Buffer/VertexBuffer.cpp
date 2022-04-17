/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "VertexBuffer.hpp"

#include "InstancingBuffer.hpp"
#include "../../GraphicsDefine.hpp"

namespace Engine
{
    VertexBuffer::VertexBuffer()
    {
    }

    VertexBuffer::~VertexBuffer()
    {
    }

    void VertexBuffer::Initialize(const std::vector<PosVertex>& vertices, bool b_dynamic)
    {
        m_b_dynamic = b_dynamic;
        m_vertex_count = static_cast<Uint32>(vertices.size());
        m_vertex_stride = sizeof(PosVertex);
        CreateBuffer(&vertices[0]);
    }

    void VertexBuffer::Initialize(const std::vector<ColorVertex>& vertices, bool b_dynamic)
    {
        m_b_dynamic     = b_dynamic;
        m_vertex_count  = static_cast<Uint32>(vertices.size());
        m_vertex_stride = sizeof(ColorVertex);
        CreateBuffer(&vertices[0]);
    }

    void VertexBuffer::Initialize(const std::vector<TexVertex>& vertices, bool b_dynamic)
    {
        m_b_dynamic     = b_dynamic;
        m_vertex_count  = static_cast<Uint32>(vertices.size());
        m_vertex_stride = sizeof(TexVertex);
        CreateBuffer(&vertices[0]);
    }

    void VertexBuffer::Initialize(const std::vector<NormalVertex>& vertices, bool b_dynamic)
    {
        m_b_dynamic     = b_dynamic;
        m_vertex_count  = static_cast<Uint32>(vertices.size());
        m_vertex_stride = sizeof(NormalVertex);
        CreateBuffer(&vertices[0]);
    }

    void VertexBuffer::Initialize(const std::vector<LightVertex>& vertices, bool b_dynamic)
    {
        m_b_dynamic     = b_dynamic;
        m_vertex_count  = static_cast<Uint32>(vertices.size());
        m_vertex_stride = sizeof(LightVertex);
        CreateBuffer(&vertices[0]);
    }

    void VertexBuffer::Initialize(const std::vector<GeneralVertex>& vertices, bool b_dynamic)
    {
        m_b_dynamic     = b_dynamic;
        m_vertex_count  = static_cast<Uint32>(vertices.size());
        m_vertex_stride = sizeof(GeneralVertex);
        CreateBuffer(&vertices[0]);
    }

    void VertexBuffer::Initialize(const std::vector<SkinnedVertex>& vertices, bool b_dynamic)
    {
        m_b_dynamic     = b_dynamic;
        m_vertex_count  = static_cast<Uint32>(vertices.size());
        m_vertex_stride = sizeof(SkinnedVertex);
        CreateBuffer(&vertices[0]);
    }

    void VertexBuffer::Update(const std::vector<PosVertex>& vertices) const
    {
        if (m_b_dynamic == false)
            return;

        if (m_vertex_stride != sizeof(PosVertex))
            return;

        if (m_vertex_count != vertices.size())
            return;

        UpdateBuffer(&vertices[0]);
    }

    void VertexBuffer::Update(const std::vector<ColorVertex>& vertices) const
    {
        if (m_b_dynamic == false)
            return;

        if (m_vertex_stride != sizeof(ColorVertex))
            return;

        if (m_vertex_count != vertices.size())
            return;

        UpdateBuffer(&vertices[0]);
    }

    void VertexBuffer::Update(const std::vector<TexVertex>& vertices) const
    {
        if (m_b_dynamic == false)
            return;

        if (m_vertex_stride != sizeof(TexVertex))
            return;

        if (m_vertex_count != vertices.size())
            return;

        UpdateBuffer(&vertices[0]);
    }

    void VertexBuffer::Update(const std::vector<NormalVertex>& vertices) const
    {
        if (m_b_dynamic == false)
            return;

        if (m_vertex_stride != sizeof(NormalVertex))
            return;

        if (m_vertex_count != vertices.size())
            return;

        UpdateBuffer(&vertices[0]);
    }

    void VertexBuffer::Update(const std::vector<LightVertex>& vertices) const
    {
        if (m_b_dynamic == false)
            return;

        if (m_vertex_stride != sizeof(LightVertex))
            return;

        if (m_vertex_count != vertices.size())
            return;

        UpdateBuffer(&vertices[0]);
    }

    void VertexBuffer::Update(const std::vector<GeneralVertex>& vertices) const
    {
        if (m_b_dynamic == false)
            return;

        if (m_vertex_stride != sizeof(GeneralVertex))
            return;

        if (m_vertex_count != vertices.size())
            return;

        UpdateBuffer(&vertices[0]);
    }

    void VertexBuffer::Update(const std::vector<SkinnedVertex>& vertices) const
    {
        if (m_b_dynamic == false)
            return;

        if (m_vertex_stride != sizeof(SkinnedVertex))
            return;

        if (m_vertex_count != vertices.size())
            return;

        UpdateBuffer(&vertices[0]);
    }

    void VertexBuffer::Shutdown()
    {
        RELEASE_COM(m_vertex_buffer);
    }

    void VertexBuffer::Bind() const
    {
        CMD_LIST->IASetVertexBuffers(0, 1, &m_vertex_buffer_view);
    }

    void VertexBuffer::Bind(const SPtr<InstancingBuffer>& instancing_buffer) const
    {
        D3D12_VERTEX_BUFFER_VIEW buffer_views[] = { m_vertex_buffer_view, instancing_buffer->GetBufferView() };
        CMD_LIST->IASetVertexBuffers(0, 2, buffer_views);
    }

    void VertexBuffer::CreateBuffer(const void* data)
    {
        Uint32 buffer_size = m_vertex_count * m_vertex_stride;

        D3D12_HEAP_PROPERTIES heap_property = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        D3D12_RESOURCE_DESC   desc          = CD3DX12_RESOURCE_DESC::Buffer(buffer_size);

        DEVICE->CreateCommittedResource(
                                        &heap_property,
                                        D3D12_HEAP_FLAG_NONE,
                                        &desc,
                                        D3D12_RESOURCE_STATE_GENERIC_READ,
                                        nullptr,
                                        IID_PPV_ARGS(&m_vertex_buffer));

        // Copy the vertex data to the vertex buffer.
        void*         vertex_data_buffer = nullptr;
        CD3DX12_RANGE read_range(0, 0);
        m_vertex_buffer->Map(0, &read_range, &vertex_data_buffer);
        ::memcpy(vertex_data_buffer, data, buffer_size);
        m_vertex_buffer->Unmap(0, nullptr);

        // Initialize the vertex buffer view.
        m_vertex_buffer_view.BufferLocation = m_vertex_buffer->GetGPUVirtualAddress();
        m_vertex_buffer_view.StrideInBytes  = m_vertex_stride;
        m_vertex_buffer_view.SizeInBytes    = buffer_size;
    }

    void VertexBuffer::UpdateBuffer(const void* data) const
    {
        Uint32        buffer_size        = m_vertex_count * m_vertex_stride;
        void*         vertex_data_buffer = nullptr;
        CD3DX12_RANGE read_range(0, 0);
        m_vertex_buffer->Map(0, &read_range, &vertex_data_buffer);
        ::memcpy(vertex_data_buffer, data, buffer_size);
        m_vertex_buffer->Unmap(0, nullptr);
    }
}
