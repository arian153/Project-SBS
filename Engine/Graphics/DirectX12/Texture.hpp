#pragma once
#include "DirectXIncludes.hpp"
#include "../../EngineDefine.hpp"

namespace Engine
{
    class Texture
    {
    public:
        Texture();
        ~Texture();
        bool Initialize(const StringWide& path, const String& ext);

        D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() const;

    public:
        bool CreateTexture(const StringWide& path, const String& ext);
        bool CreateView();

    private:
        ScratchImage           m_image;
        ComPtr<ID3D12Resource> m_texture_2d;

        ComPtr<ID3D12DescriptorHeap> m_srv_heap;
        D3D12_CPU_DESCRIPTOR_HANDLE  m_srv_handle = {};
    };
}
