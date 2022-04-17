/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "RenderTarget.hpp"

#include "../GraphicsDefine.hpp"

namespace Engine
{
    RenderTargetGroup::RenderTargetGroup()
        : m_rtv_heap_begin(), m_dsv_heap_begin(), m_target_to_resource {}, m_resource_to_target {}
    {
    }

    void RenderTargetGroup::Create(eRenderTargetGroupType group_type, std::vector<RenderTarget>& rt_vec, SPtr<Texture> ds_texture)
    {
        m_group_type = group_type;
        m_rt_vec     = rt_vec;
        m_rt_count   = static_cast<Uint32>(rt_vec.size());
        m_ds_texture = ds_texture;

        D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
        heap_desc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        heap_desc.NumDescriptors             = m_rt_count;
        heap_desc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        heap_desc.NodeMask                   = 0;

        DEVICE->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&m_rtv_heap));

        m_rtv_heap_size  = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        m_rtv_heap_begin = m_rtv_heap->GetCPUDescriptorHandleForHeapStart();
        m_dsv_heap_begin = m_ds_texture->GetDSV()->GetCPUDescriptorHandleForHeapStart();

        for (Uint32 i = 0; i < m_rt_count; i++)
        {
            Uint32                      dest_size   = 1;
            D3D12_CPU_DESCRIPTOR_HANDLE dest_handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtv_heap_begin, i * m_rtv_heap_size);

            Uint32                       src_size           = 1;
            ComPtr<ID3D12DescriptorHeap> src_rtv_heap_begin = m_rt_vec[i].target->GetRTV();
            D3D12_CPU_DESCRIPTOR_HANDLE  src_handle         = src_rtv_heap_begin->GetCPUDescriptorHandleForHeapStart();

            DEVICE->CopyDescriptors(1, &dest_handle, &dest_size, 1, &src_handle, &src_size, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        }

        for (Uint32 i = 0; i < m_rt_count; ++i)
        {
            m_target_to_resource[i] = CD3DX12_RESOURCE_BARRIER::Transition(m_rt_vec[i].target->GetTex2D().Get(),
                D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COMMON);

            m_resource_to_target[i] = CD3DX12_RESOURCE_BARRIER::Transition(m_rt_vec[i].target->GetTex2D().Get(),
                D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET);
        }
    }

    void RenderTargetGroup::OMSetRenderTargets(Uint32 count, Uint32 offset) const
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtv_heap_begin, offset * m_rtv_heap_size);
        CMD_LIST->OMSetRenderTargets(count, &rtv_handle, FALSE, &m_dsv_heap_begin);
    }

    void RenderTargetGroup::OMSetRenderTargets() const
    {
        CMD_LIST->OMSetRenderTargets(m_rt_count, &m_rtv_heap_begin, TRUE, &m_dsv_heap_begin);
    }

    void RenderTargetGroup::ClearRenderTargetView(Uint32 index) const
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtv_heap_begin, index * m_rtv_heap_size);
        CMD_LIST->ClearRenderTargetView(rtv_handle, m_rt_vec[index].clear_color, 0, nullptr);

        CMD_LIST->ClearDepthStencilView(m_dsv_heap_begin, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
    }

    void RenderTargetGroup::ClearRenderTargetView() const
    {
        WaitResourceToTarget();

        for (Uint32 i = 0; i < m_rt_count; i++)
        {
            D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtv_heap_begin, i * m_rtv_heap_size);
            CMD_LIST->ClearRenderTargetView(rtv_handle, m_rt_vec[i].clear_color, 0, nullptr);
        }

        CMD_LIST->ClearDepthStencilView(m_dsv_heap_begin, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
    }

    SPtr<Texture> RenderTargetGroup::GetRTTexture(Uint32 index)
    {
        return m_rt_vec[index].target;
    }

    SPtr<Texture> RenderTargetGroup::GetDSTexture()
    {
        return m_ds_texture;
    }

    void RenderTargetGroup::Reset() const
    {
        if (m_ds_texture != nullptr)
        {
            m_ds_texture->Reset();
        }

        for (Uint32 i = 0; i < m_rt_count; ++i)
        {
            m_rt_vec[i].target->Reset();
        }
    }

    Uint32 RenderTargetGroup::RTCount() const
    {
        return m_rt_count;
    }

    void RenderTargetGroup::WaitTargetToResource() const
    {
        CMD_LIST->ResourceBarrier(m_rt_count, m_target_to_resource);
    }

    void RenderTargetGroup::WaitResourceToTarget() const
    {
        CMD_LIST->ResourceBarrier(m_rt_count, m_resource_to_target);
    }
}
