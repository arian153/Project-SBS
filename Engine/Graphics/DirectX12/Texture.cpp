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

    ComPtr<ID3D12Resource> Texture::GetTex2D()
    {
        return m_texture_2d;
    }

    ComPtr<ID3D12DescriptorHeap> Texture::GetSRV()
    {
        return m_srv_heap;
    }

    ComPtr<ID3D12DescriptorHeap> Texture::GetRTV()
    {
        return m_rtv_heap;
    }

    ComPtr<ID3D12DescriptorHeap> Texture::GetDSV()
    {
        return m_dsv_heap;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE Texture::GetSRVHandle() const
    {
        return m_srv_heap_begin;
    }

    bool Texture::Load(const StringWide& path, const String& ext)
    {
        TexMetadata meta_data;

        if (ext == ".dds" || ext == ".DDS")
            LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, &meta_data, m_image);
        else if (ext == ".tga" || ext == ".TGA")
            LoadFromTGAFile(path.c_str(), &meta_data, m_image);
        else // png, jpg, jpeg, bmp
            LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &meta_data, m_image);

        m_b_cube_map = meta_data.IsCubemap();
        m_mip_level  = meta_data.mipLevels;

        HRESULT hr = ::CreateTexture(DEVICE.Get(), m_image.GetMetadata(), &m_texture_2d);
        if (FAILED(hr))
        {
            #ifdef _DEBUG
            MessageBoxA(nullptr, "Failed Texture Creation!", nullptr, MB_OK);
            #endif
            return false;
        }

        std::vector<D3D12_SUBRESOURCE_DATA> sub_resources;

        hr = PrepareUpload(
                           DEVICE.Get(),
                           m_image.GetImages(),
                           m_image.GetImageCount(),
                           m_image.GetMetadata(),
                           sub_resources);

        if (FAILED(hr))
        {
            #ifdef _DEBUG
            MessageBoxA(nullptr, "Failed to Prepare Upload!", nullptr, MB_OK);
            #endif
            return false;
        }

        const Uint64 buffer_size = GetRequiredIntermediateSize(
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
            MessageBoxA(nullptr, "Failed to Create Committed Resource!", nullptr, MB_OK);
            #endif
            return false;
        }

        UpdateSubresources(
                           RESOURCE_CMD_LIST.Get(),
                           m_texture_2d.Get(),
                           texture_upload_heap.Get(),
                           0, 0,
                           static_cast<unsigned int>(sub_resources.size()),
                           sub_resources.data());

        RENDER_SYS_DX12->FlushResourceCommandQueue();
        D3D12_DESCRIPTOR_HEAP_DESC srv_heap_desc = {};
        srv_heap_desc.NumDescriptors             = 1;
        srv_heap_desc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        srv_heap_desc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        DEVICE->CreateDescriptorHeap(&srv_heap_desc, IID_PPV_ARGS(&m_srv_heap));

        m_srv_heap_begin = m_srv_heap->GetCPUDescriptorHandleForHeapStart();

        D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
        srv_desc.Format                          = m_image.GetMetadata().format;
        srv_desc.Shader4ComponentMapping         = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        if (m_b_cube_map)
        {
            srv_desc.ViewDimension                   = D3D12_SRV_DIMENSION_TEXTURECUBE;
            srv_desc.TextureCube.MostDetailedMip     = 0;
            srv_desc.TextureCube.MipLevels           = static_cast<UINT>(m_mip_level);
            srv_desc.TextureCube.ResourceMinLODClamp = 0.0f;
        }
        else
        {
            srv_desc.ViewDimension                 = D3D12_SRV_DIMENSION_TEXTURE2D;
            srv_desc.Texture2D.MostDetailedMip     = 0;
            srv_desc.Texture2D.MipLevels           = static_cast<UINT>(m_mip_level);
            srv_desc.Texture2D.ResourceMinLODClamp = 0.0f;
        }

        DEVICE->CreateShaderResourceView(m_texture_2d.Get(), &srv_desc, m_srv_heap_begin);

        return true;
    }

    void Texture::Create(DXGI_FORMAT format, Uint32 width, Uint32 height, const D3D12_HEAP_PROPERTIES& heap_property, D3D12_HEAP_FLAGS heap_flags, D3D12_RESOURCE_FLAGS res_flags, const Color& clear_color)
    {
        D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(format, width, height);
        desc.Flags               = res_flags;

        D3D12_CLEAR_VALUE     optimized_clear_value = {};
        D3D12_RESOURCE_STATES resource_states       = D3D12_RESOURCE_STATE_COMMON;

        if (res_flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)
        {
            resource_states       = D3D12_RESOURCE_STATE_DEPTH_WRITE;
            optimized_clear_value = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
        }
        else if (res_flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
        {
            resource_states       = D3D12_RESOURCE_STATE_RENDER_TARGET;
            float arr_float[4]    = { clear_color.r, clear_color.g, clear_color.b, clear_color.a };
            optimized_clear_value = CD3DX12_CLEAR_VALUE(format, arr_float);
        }

        // Create Texture2D
        HRESULT hr = DEVICE->CreateCommittedResource(
                                                     &heap_property,
                                                     heap_flags,
                                                     &desc,
                                                     resource_states,
                                                     &optimized_clear_value,
                                                     IID_PPV_ARGS(&m_texture_2d));

        assert(SUCCEEDED(hr));

        CreateFromResource(m_texture_2d);
    }

    void Texture::CreateFromResource(ComPtr<ID3D12Resource> tex_2d)
    {
        m_texture_2d = tex_2d;

        D3D12_RESOURCE_DESC desc = tex_2d->GetDesc();

        // - DSV 
        // - SRV
        // - RTV + SRV
        if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)
        {
            // DSV
            D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
            heap_desc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
            heap_desc.NumDescriptors             = 1;
            heap_desc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            heap_desc.NodeMask                   = 0;
            DEVICE->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&m_dsv_heap));

            D3D12_CPU_DESCRIPTOR_HANDLE h_dsv_handle = m_dsv_heap->GetCPUDescriptorHandleForHeapStart();
            DEVICE->CreateDepthStencilView(m_texture_2d.Get(), nullptr, h_dsv_handle);
        }
        else
        {
            if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
            {
                // RTV
                D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
                heap_desc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
                heap_desc.NumDescriptors             = 1;
                heap_desc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
                heap_desc.NodeMask                   = 0;
                DEVICE->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&m_rtv_heap));

                D3D12_CPU_DESCRIPTOR_HANDLE rtv_heap_begin = m_rtv_heap->GetCPUDescriptorHandleForHeapStart();
                DEVICE->CreateRenderTargetView(m_texture_2d.Get(), nullptr, rtv_heap_begin);
            }

            // SRV
            D3D12_DESCRIPTOR_HEAP_DESC srv_heap_desc = {};
            srv_heap_desc.NumDescriptors             = 1;
            srv_heap_desc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            srv_heap_desc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            DEVICE->CreateDescriptorHeap(&srv_heap_desc, IID_PPV_ARGS(&m_srv_heap));

            m_srv_heap_begin = m_srv_heap->GetCPUDescriptorHandleForHeapStart();

            D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
            srv_desc.Shader4ComponentMapping         = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srv_desc.Format                          = m_image.GetMetadata().format;
            srv_desc.ViewDimension                   = D3D12_SRV_DIMENSION_TEXTURE2D;
            srv_desc.Texture2D.MipLevels             = 1;
            DEVICE->CreateShaderResourceView(m_texture_2d.Get(), &srv_desc, m_srv_heap_begin);
        }
    }

    void Texture::Reset()
    {
        m_texture_2d.Reset();
    }
}
