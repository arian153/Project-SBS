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

        void Increase();

    private:
        Uint32 m_num_descriptor = 1;
        ComPtr<ID3D12DescriptorHeap> m_srv_heap;
    };
}
