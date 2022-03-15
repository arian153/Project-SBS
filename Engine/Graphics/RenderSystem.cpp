#include "RenderSystem.hpp"

#include <memory>
#include <memory>
#include <memory>
#include <memory>

#include "RenderSubsystem.hpp"
#include "../Core/CoreDefine.hpp"
#include "../Core/ResourceManager/ResourceManager.hpp"
#include "Data/Vertex.hpp"
#include "DirectX12/DirectX12Layer.hpp"
#include "DirectX12/InputLayout.hpp"
#include "DirectX12/RootSignature.hpp"
#include "DirectX12/TableDescriptorHeap.hpp"
#include "Element/Material.hpp"
#include "Element/SimpleMesh.hpp"
#include "Shader/ShaderManager.hpp"
#include "Utility/MaterialManager.hpp"

namespace Engine
{
    RenderSystem::RenderSystem()
    {
    }

    RenderSystem::~RenderSystem()
    {
    }

    void RenderSystem::Initialize()
    {
        m_dx12_layer            = std::make_shared<DirectX12Layer>();
        m_root_signature        = std::make_shared<RootSignature>();
        m_table_descriptor_heap = std::make_shared<TableDescriptorHeap>();
        m_shader_manager        = std::make_shared<ShaderManager>();
        m_material_manager      = std::make_shared<MaterialManager>();

        m_dx12_layer->Initialize(m_window_info);
        m_root_signature->Initialize();

        m_table_descriptor_heap->Init(256);

        CreateRenderTargetGroups();
        m_dx12_layer->WaitSync();
    }

    void RenderSystem::Update()
    {
    }

    void RenderSystem::Shutdown()
    {
        for (auto& subsystem : m_subsystems)
        {
            subsystem->Shutdown();
        }
    }

    void RenderSystem::SetWindowInfo(const WindowInfo& info)
    {
        m_window_info = info;
    }

    void RenderSystem::RenderBegin() const
    {
        m_dx12_layer->RenderBegin(COLOR[static_cast<Uint32>(eColorInfo::Black)]);
    }

    void RenderSystem::RenderEnd() const
    {
        m_dx12_layer->RenderEnd();
    }

    SPtr<RenderSubsystem> RenderSystem::CreateSubsystem()
    {
        auto subsystem = std::make_shared<RenderSubsystem>();
        subsystem->OnResize(m_viewport_manager.GetPerspectiveMatrix(), m_viewport_manager.GetOrthoGraphicMatrix());
        m_subsystems.push_back(subsystem);
        return subsystem;
    }

    void RenderSystem::ClearConstantBuffers() const
    {
        for (auto& subsystem : m_subsystems)
        {
            subsystem->ClearConstantBuffers();
        }

        m_table_descriptor_heap->Clear();
        ID3D12DescriptorHeap* desc_heap = m_table_descriptor_heap->GetDescriptorHeap().Get();
        m_dx12_layer->GetCmdList()->SetDescriptorHeaps(1, &desc_heap);
    }

    void RenderSystem::OnResize(Sint32 width, Sint32 height)
    {
        m_window_info.width  = width;
        m_window_info.height = height;
        m_viewport_manager.SetClientRect(width, height);

        if (m_dx12_layer == nullptr)
        {
            return;
        }

        m_dx12_layer->OnResize(width, height, m_window_info.b_windowed);

        for (auto& render_subsystem : m_subsystems)
        {
            render_subsystem->OnResize(m_viewport_manager.GetPerspectiveMatrix(), m_viewport_manager.GetOrthoGraphicMatrix());
        }
    }

    void RenderSystem::OnFullscreen(bool b_fullscreen)
    {
        m_window_info.b_windowed = !b_fullscreen;
        if (m_dx12_layer == nullptr)
        {
            return;
        }
        m_dx12_layer->OnFullscreen(b_fullscreen);
    }

    void RenderSystem::PostInitialize()
    {
        m_shader_manager->Initialize();
        m_material_manager->Initialize();

        m_b_init = true;
        m_dx12_layer->WaitSync();
    }

    void RenderSystem::CreateRenderTargetGroups()
    {
        // DepthStencil
        SPtr<Texture> ds_texture = RESOURCE_MANAGER->GetCreatedTexture("DepthStencil");
        ds_texture->Create(
                           DXGI_FORMAT_D32_FLOAT, m_window_info.width, m_window_info.height,
                           CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
                           D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

        // SwapChain Group
        {
            std::vector<RenderTarget> rt_vec(SWAP_CHAIN_BUFFER_COUNT);

            for (Uint32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
            {
                String name = "SwapChainTarget_" + std::to_string(i);

                ComPtr<ID3D12Resource> resource;
                m_dx12_layer->m_swap_chain->GetBuffer(i, IID_PPV_ARGS(&resource));

                rt_vec[i].target = RESOURCE_MANAGER->GetCreatedTexture(name);
                rt_vec[i].target->CreateFromResource(resource);
            }

            m_rt_groups[static_cast<Uint32>(eRenderTargetGroupType::SwapChain)] = std::make_shared<RenderTargetGroup>();
            m_rt_groups[static_cast<Uint32>(eRenderTargetGroupType::SwapChain)]->Create(eRenderTargetGroupType::SwapChain, rt_vec, ds_texture);
        }

        // DeferredGeo Group
        {
            std::vector<RenderTarget> rt_vec(RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT);
            rt_vec[0].target = RESOURCE_MANAGER->GetCreatedTexture("PositionTarget");
            rt_vec[0].target->Create(
                                     DXGI_FORMAT_R32G32B32A32_FLOAT, m_window_info.width, m_window_info.height,
                                     CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
                                     D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
            rt_vec[1].target = RESOURCE_MANAGER->GetCreatedTexture("NormalTarget");
            rt_vec[1].target->Create(
                                     DXGI_FORMAT_R32G32B32A32_FLOAT, m_window_info.width, m_window_info.height,
                                     CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
                                     D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
            rt_vec[2].target = RESOURCE_MANAGER->GetCreatedTexture("DiffuseTarget");
            rt_vec[2].target->Create(
                                     DXGI_FORMAT_R8G8B8A8_UNORM, m_window_info.width, m_window_info.height,
                                     CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
                                     D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

            m_rt_groups[static_cast<Uint32>(eRenderTargetGroupType::GBuffer)] = std::make_shared<RenderTargetGroup>();
            m_rt_groups[static_cast<Uint32>(eRenderTargetGroupType::GBuffer)]->Create(eRenderTargetGroupType::GBuffer, rt_vec, ds_texture);
        }

        // Lighting Group
        {
            std::vector<RenderTarget> rt_vec(RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT);

            rt_vec[0].target = RESOURCE_MANAGER->GetCreatedTexture("DiffuseLightTarget");

            rt_vec[0].target->Create(
                                     DXGI_FORMAT_R8G8B8A8_UNORM, m_window_info.width, m_window_info.height,
                                     CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
                                     D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

            rt_vec[1].target = RESOURCE_MANAGER->GetCreatedTexture("SpecularLightTarget");

            rt_vec[1].target->Create(
                                     DXGI_FORMAT_R8G8B8A8_UNORM, m_window_info.width, m_window_info.height,
                                     CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
                                     D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

            m_rt_groups[static_cast<Uint32>(eRenderTargetGroupType::Lighting)] = std::make_shared<RenderTargetGroup>();
            m_rt_groups[static_cast<Uint32>(eRenderTargetGroupType::Lighting)]->Create(eRenderTargetGroupType::Lighting, rt_vec, ds_texture);
        }

        m_b_created_rtg = true;
    }

    void RenderSystem::ResetRenderTargetGroups() const
    {
        if (m_b_created_rtg == false)
            return;

        for (Uint32 i = 0; i < RENDER_TARGET_GROUP_COUNT; ++i)
        {
            m_rt_groups[i]->Reset();
        }
    }

    SPtr<DirectX12Layer> RenderSystem::GetDirectX12Layer()
    {
        return m_dx12_layer;
    }

    SPtr<RootSignature> RenderSystem::GetRootSignature()
    {
        return m_root_signature;
    }

    SPtr<TableDescriptorHeap> RenderSystem::GetTableDescriptorHeap()
    {
        return m_table_descriptor_heap;
    }

    SPtr<ShaderManager> RenderSystem::GetShaderManager()
    {
        return m_shader_manager;
    }

    SPtr<MaterialManager> RenderSystem::GetMaterialManager()
    {
        return m_material_manager;
    }

    ViewportManager& RenderSystem::GetViewportManager()
    {
        return m_viewport_manager;
    }

    const ViewportManager& RenderSystem::GetViewportManager() const
    {
        return m_viewport_manager;
    }

    SPtr<RenderTargetGroup> RenderSystem::GetRTGroup(eRenderTargetGroupType type)
    {
        return m_rt_groups[static_cast<Uint32>(type)];
    }
}
