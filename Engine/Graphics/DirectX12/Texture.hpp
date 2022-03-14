#pragma once
#include "DirectXIncludes.hpp"
#include "../../EngineDefine.hpp"
#include "../Data/Color.hpp"

namespace Engine
{
    class Texture
    {
    public:
        Texture();
        ~Texture();

    public:
        ComPtr<ID3D12Resource>       GetTex2D();
        ComPtr<ID3D12DescriptorHeap> GetSRV();
        ComPtr<ID3D12DescriptorHeap> GetRTV();
        ComPtr<ID3D12DescriptorHeap> GetDSV();
        D3D12_CPU_DESCRIPTOR_HANDLE  GetSRVHandle() const;

        bool IsCubeMap() const;

    public:
        bool Load(const StringWide& path, const String& ext);

        void Create(DXGI_FORMAT                  format, Uint32 width, Uint32 height,
                    const D3D12_HEAP_PROPERTIES& heap_property,
                    D3D12_HEAP_FLAGS             heap_flags,
                    D3D12_RESOURCE_FLAGS         res_flags,
                    const Color&                 clear_color = Color(0, 0, 0, 0));

        void CreateFromResource(ComPtr<ID3D12Resource> tex_2d);

        void Reset();

    private:
        bool                   m_b_cube_map = false;
        size_t                 m_mip_level  = 0;
        ScratchImage           m_image;
        ComPtr<ID3D12Resource> m_texture_2d;

        ComPtr<ID3D12DescriptorHeap> m_srv_heap;
        D3D12_CPU_DESCRIPTOR_HANDLE  m_srv_heap_begin = {};

        ComPtr<ID3D12DescriptorHeap> m_rtv_heap;
        ComPtr<ID3D12DescriptorHeap> m_dsv_heap;
    };
}
