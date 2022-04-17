#include "SRVDescriptorHeap.hpp"

#include "../GraphicsDefine.hpp"

namespace Engine
{
    SRVDescriptorHeap::SRVDescriptorHeap()
    {
    }

    SRVDescriptorHeap::~SRVDescriptorHeap()
    {
    }

    bool SRVDescriptorHeap::Init()
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors             = m_num_descriptor;
        desc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

        if (DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_srv_heap)) != S_OK)
            return false;

        return true;
    }

    ComPtr<ID3D12DescriptorHeap> SRVDescriptorHeap::GetHeap()
    {
        return m_srv_heap;
    }

    void SRVDescriptorHeap::Increase()
    {
        m_num_descriptor++;
        Init();
    }

    Uint32 SRVDescriptorHeap::GetNumDescriptor() const
    {
        return m_num_descriptor;
    }
}
