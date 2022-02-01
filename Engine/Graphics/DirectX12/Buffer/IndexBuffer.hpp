#pragma once

#include "../DirectXIncludes.hpp"

namespace Engine
{
    class IndexBuffer
    {
    public:
        IndexBuffer();
        ~IndexBuffer();

        void Initialize(const std::vector<Uint32>& indices);
        void Shutdown();
        void Bind() const;

        void Draw() const;
        void Draw(Uint32 instance_count) const;

    private:
        ComPtr<ID3D12Resource>  m_index_buffer;
        D3D12_INDEX_BUFFER_VIEW m_index_buffer_view = {};
        Uint32                  m_index_count = 0;
    };
}
