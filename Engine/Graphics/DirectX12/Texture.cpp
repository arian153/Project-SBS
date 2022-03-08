#include "Texture.hpp"

#include "../GraphicsDefine.hpp"

namespace Engine
{
    Texture::Texture()
    {
    }

    Texture::~Texture()
    {
    }

    bool Texture::Initialize(const StringWide& path, const String& ext)
    {
        if (CreateTexture(path, ext) == false)
            return false;
        return CreateView();
    }

    D3D12_CPU_DESCRIPTOR_HANDLE Texture::GetCpuHandle() const
    {
        return m_srv_handle;
    }

    bool Texture::CreateTexture(const StringWide& path, const String& ext)
    {
        TexMetadata meta_data;

        if (ext == ".dds" || ext == ".DDS")
            ::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, &meta_data, m_image);
        else if (ext == ".tga" || ext == ".TGA")
            ::LoadFromTGAFile(path.c_str(), &meta_data, m_image);
        else // png, jpg, jpeg, bmp
            ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &meta_data, m_image);

        m_b_cube_map = meta_data.IsCubemap();
        m_mip_level  = meta_data.mipLevels;

        HRESULT hr = ::CreateTexture(DEVICE.Get(), m_image.GetMetadata(), &m_texture_2d);
        if (FAILED(hr))
        {
            #ifdef _DEBUG
            ::MessageBoxA(nullptr, "Failed Texture Creation!", nullptr, MB_OK);
            #endif
            return false;
        }

        std::vector<D3D12_SUBRESOURCE_DATA> sub_resources;

        hr = ::PrepareUpload(
                             DEVICE.Get(),
                             m_image.GetImages(),
                             m_image.GetImageCount(),
                             m_image.GetMetadata(),
                             sub_resources);

        if (FAILED(hr))
        {
            #ifdef _DEBUG
            ::MessageBoxA(nullptr, "Failed to Prepare Upload!", nullptr, MB_OK);
            #endif
            return false;
        }

        const Uint64 buffer_size = ::GetRequiredIntermediateSize(
                                                                 m_texture_2d.Get(),
                                                                 0,
                                                                 static_cast<Uint32>(sub_resources.size()));

        D3D12_HEAP_PROPERTIES heap_property = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        D3D12_RESOURCE_DESC   desc          = CD3DX12_RESOURCE_DESC::Buffer(buffer_size);

        ComPtr<ID3D12Resource> texture_upload_heap;
        hr = DEVICE->CreateCommittedResource(
                                             &heap_property,
                                             D3D12_HEAP_FLAG_NONE,
                                             &desc,
                                             D3D12_RESOURCE_STATE_GENERIC_READ,
                                             nullptr,
                                             IID_PPV_ARGS(texture_upload_heap.GetAddressOf()));

        if (FAILED(hr))
        {
            #ifdef _DEBUG
            ::MessageBoxA(nullptr, "Failed to Create Committed Resource!", nullptr, MB_OK);
            #endif
            return false;
        }

        ::UpdateSubresources(
                             RESOURCE_CMD_LIST.Get(),
                             m_texture_2d.Get(),
                             texture_upload_heap.Get(),
                             0, 0,
                             static_cast<unsigned int>(sub_resources.size()),
                             sub_resources.data());

        RENDER_SYS_DX12->FlushResourceCommandQueue();
        return true;
    }

    bool Texture::CreateView()
    {
        D3D12_DESCRIPTOR_HEAP_DESC srv_heap_desc = {};
        srv_heap_desc.NumDescriptors             = 1;
        srv_heap_desc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        srv_heap_desc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        DEVICE->CreateDescriptorHeap(&srv_heap_desc, IID_PPV_ARGS(&m_srv_heap));

        m_srv_handle = m_srv_heap->GetCPUDescriptorHandleForHeapStart();

        D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
        srv_desc.Format                          = m_image.GetMetadata().format;
        srv_desc.Shader4ComponentMapping         = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        if (m_b_cube_map)
        {
            srv_desc.ViewDimension                   = D3D12_SRV_DIMENSION_TEXTURECUBE;
            srv_desc.TextureCube.MostDetailedMip     = 0;
            srv_desc.TextureCube.MipLevels           = m_mip_level;
            srv_desc.TextureCube.ResourceMinLODClamp = 0.0f;
        }
        else
        {
            srv_desc.ViewDimension                 = D3D12_SRV_DIMENSION_TEXTURE2D;
            srv_desc.Texture2D.MostDetailedMip     = 0;
            srv_desc.Texture2D.MipLevels           = m_mip_level;
            srv_desc.Texture2D.ResourceMinLODClamp = 0.0f;
        }

        DEVICE->CreateShaderResourceView(m_texture_2d.Get(), &srv_desc, m_srv_handle);

        return true;
    }
}
