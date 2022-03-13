#pragma once
#include "DirectXIncludes.hpp"
#include "../../EngineDefine.hpp"
#include "Texture.hpp"

namespace Engine
{
    enum class eRenderTargetGroupType : Uint32
    {
        SwapChain, // BACK_BUFFER, FRONT_BUFFER
        GBuffer, // POSITION, NORMAL, COLOR
        End,
    };

    constexpr Uint32 RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT = 3;
    constexpr Uint32 RENDER_TARGET_GROUP_COUNT                 = static_cast<Uint32>(eRenderTargetGroupType::End);

    struct RenderTarget
    {
        SPtr<Texture> target;
        float         clear_color[4];
    };

    class RenderTargetGroup
    {
    public:
        RenderTargetGroup();

        void Create(eRenderTargetGroupType group_type, std::vector<RenderTarget>& rt_vec, SPtr<Texture> ds_texture);

        void OMSetRenderTargets(Uint32 count, Uint32 offset) const;
        void OMSetRenderTargets() const;

        void ClearRenderTargetView(Uint32 index) const;
        void ClearRenderTargetView() const;

        SPtr<Texture> GetRTTexture(Uint32 index);
        SPtr<Texture> GetDSTexture();

        void Reset() const;

    private:
        eRenderTargetGroupType       m_group_type = eRenderTargetGroupType::End;
        std::vector<RenderTarget>    m_rt_vec;
        Uint32                       m_rt_count   = 0;
        SPtr<Texture>                m_ds_texture = nullptr;
        ComPtr<ID3D12DescriptorHeap> m_rtv_heap   = nullptr;

    private:
        Uint32                      m_rtv_heap_size = 0;
        D3D12_CPU_DESCRIPTOR_HANDLE m_rtv_heap_begin;
        D3D12_CPU_DESCRIPTOR_HANDLE m_dsv_heap_begin;
    };
}
