#include "RenderSystem.hpp"

#include <memory>
#include <memory>
#include <memory>

#include "RenderSubsystem.hpp"
#include "Data/Vertex.hpp"
#include "DirectX12/DirectX12Layer.hpp"
#include "DirectX12/InputLayout.hpp"
#include "DirectX12/RootSignature.hpp"
#include "DirectX12/TableDescriptorHeap.hpp"
#include "DirectX12/Buffer/ConstantBuffer.hpp"
#include "Element/Material.hpp"
#include "Element/SimpleMesh.hpp"
#include "Shader/ShaderManager.hpp"

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

        m_dx12_layer->Initialize(m_window_info);
        m_root_signature->Initialize();

   

        m_table_descriptor_heap->Init(256);

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
        m_dx12_layer->RenderBegin(COLOR[static_cast<Uint32>(eColorInfo::LightSkyBlue)]);

        for (auto& subsystem : m_subsystems)
        {
            subsystem->ClearConstantBuffers();
        }

        m_table_descriptor_heap->Clear();
        ID3D12DescriptorHeap* desc_heap = m_table_descriptor_heap->GetDescriptorHeap().Get();
        m_dx12_layer->GetCmdList()->SetDescriptorHeaps(1, &desc_heap);
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

        m_b_init = true;
        m_dx12_layer->WaitSync();
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

   
    ViewportManager& RenderSystem::GetViewportManager()
    {
        return m_viewport_manager;
    }

    const ViewportManager& RenderSystem::GetViewportManager() const
    {
        return m_viewport_manager;
    }
}
