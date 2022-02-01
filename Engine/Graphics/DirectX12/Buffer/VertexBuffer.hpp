#pragma once

#include "../DirectXIncludes.hpp"

#include "../../Data/Vertex.hpp"

namespace Engine
{
    class VertexBuffer
    {
    public:
        VertexBuffer();
        ~VertexBuffer();

        void Initialize(const std::vector<ColorVertex>& vertices, bool b_dynamic = false);
        void Initialize(const std::vector<TexVertex>& vertices, bool b_dynamic = false);
        void Initialize(const std::vector<NormalVertex>& vertices, bool b_dynamic = false);
        void Initialize(const std::vector<LightVertex>& vertices, bool b_dynamic = false);
        void Initialize(const std::vector<GeneralVertex>& vertices, bool b_dynamic = false);
        void Initialize(const std::vector<SkinnedVertex>& vertices, bool b_dynamic = false);

        void Update(const std::vector<ColorVertex>& vertices) const;
        void Update(const std::vector<TexVertex>& vertices) const;
        void Update(const std::vector<NormalVertex>& vertices) const;
        void Update(const std::vector<LightVertex>& vertices) const;
        void Update(const std::vector<GeneralVertex>& vertices) const;
        void Update(const std::vector<SkinnedVertex>& vertices) const;

        template <typename T>
        void Initialize(const std::vector<T>& vertices, bool b_dynamic = false);

        template <typename T>
        void Update(const std::vector<T>& vertices) const;

        void Shutdown();
        void Bind() const;

        void CreateBuffer(const void* data);
        void UpdateBuffer(const void* data) const;

    private:
        ComPtr<ID3D12Resource>   m_vertex_buffer;
        D3D12_VERTEX_BUFFER_VIEW m_vertex_buffer_view = {};
        Uint32                   m_vertex_count       = 0;
        Uint32                   m_vertex_stride      = 0;
        bool                     m_b_dynamic          = false;
    };

    template <typename T>
    void VertexBuffer::Initialize(const std::vector<T>& vertices, bool b_dynamic)
    {
        m_b_dynamic     = b_dynamic;
        m_vertex_count  = static_cast<Uint32>(vertices.size());
        m_vertex_stride = sizeof(T);
        CreateBuffer(&vertices[0]);
    }

    template <typename T>
    void VertexBuffer::Update(const std::vector<T>& vertices) const
    {
        if (m_b_dynamic == false)
            return;

        if (m_vertex_stride != sizeof(T))
            return;

        if (m_vertex_count != vertices.size())
            return;

        UpdateBuffer(&vertices[0]);
    }
}
