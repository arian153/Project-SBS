#pragma once
#include "DirectXIncludes.hpp"

namespace Engine
{
    class SRVDescriptorHeap
    {
    public:
        SRVDescriptorHeap();
        ~SRVDescriptorHeap();

        bool Init();

        ComPtr<ID3D12DescriptorHeap> GetHeap();

    private:
        ComPtr<ID3D12DescriptorHeap> m_srv_heap;
    };
}
