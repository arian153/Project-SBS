#include "TableDescriptorHeap.hpp"

#include "../GraphicsDefine.hpp"

namespace Engine
{
    void TableDescriptorHeap::Init(Uint32 count)
    {
        m_group_count = count;

        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NumDescriptors             = count * REGISTER_COUNT;
        desc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        desc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

        DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_desc_heap));

        m_handle_size = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        m_group_size  = m_handle_size * REGISTER_COUNT;
    }

    void TableDescriptorHeap::Clear()
    {
        m_current_group_index = 0;
    }

    void TableDescriptorHeap::SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE src_handle, eCBVRegister reg) const
    {
        D3D12_CPU_DESCRIPTOR_HANDLE dest_handle = GetCPUHandle(reg);

        Uint32 dest_range = 1;
        Uint32 src_range  = 1;
        DEVICE->CopyDescriptors(
                                1,
                                &dest_handle,
                                &dest_range,
                                1,
                                &src_handle,
                                &src_range,
                                D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    void TableDescriptorHeap::SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE src_handle, eSRVRegister reg) const
    {
        D3D12_CPU_DESCRIPTOR_HANDLE dest_handle = GetCPUHandle(reg);

        Uint32 dest_range = 1;
        Uint32 src_range  = 1;
        DEVICE->CopyDescriptors(
                                1,
                                &dest_handle,
                                &dest_range,
                                1,
                                &src_handle,
                                &src_range,
                                D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    void TableDescriptorHeap::CommitTable()
    {
        D3D12_GPU_DESCRIPTOR_HANDLE handle = m_desc_heap->GetGPUDescriptorHandleForHeapStart();
        handle.ptr += m_current_group_index * m_group_size;
        CMD_LIST->SetGraphicsRootDescriptorTable(0, handle);

        m_current_group_index++;
    }

    ComPtr<ID3D12DescriptorHeap> TableDescriptorHeap::GetDescriptorHeap()
    {
        return m_desc_heap;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(eCBVRegister reg) const
    {
        return GetCPUHandle(static_cast<Uint32>(reg));
    }

    D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(eSRVRegister reg) const
    {
        return GetCPUHandle(static_cast<Uint32>(reg));
    }

    D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(Uint32 reg) const
    {
        D3D12_CPU_DESCRIPTOR_HANDLE handle = m_desc_heap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += m_current_group_index * m_group_size;
        handle.ptr += reg * m_handle_size;
        return handle;
    }
}
