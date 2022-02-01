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

        CreateConstantBuffer(eCBVRegister::b0, sizeof(Vector4), 256);
        CreateConstantBuffer(eCBVRegister::b1, sizeof(MaterialParams), 256);

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
        ClearConstantBuffers();
        m_table_descriptor_heap->Clear();
        ID3D12DescriptorHeap* desc_heap = m_table_descriptor_heap->GetDescriptorHeap().Get();
        m_dx12_layer->GetCmdList()->SetDescriptorHeaps(1, &desc_heap);
    }

    void RenderSystem::RenderEnd() const
    {
        m_dx12_layer->RenderEnd();
    }

    void RenderSystem::OnResize(Sint32 width, Sint32 height)
    {
        m_window_info.width  = width;
        m_window_info.height = height;

        if (m_dx12_layer == nullptr)
        {
            return;
        }

        m_dx12_layer->OnResize(width, height, m_window_info.b_windowed);
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

    void RenderSystem::CreateConstantBuffer(eCBVRegister reg, Uint32 buffer_size, Uint32 count)
    {
        Uint32 idx = static_cast<Uint32>(reg);
        assert(m_constant_buffers.size() == idx);

        auto buffer = std::make_shared<ConstantBuffer>();
        buffer->Init(reg, buffer_size, count);
        m_constant_buffers.push_back(buffer);
    }

    void RenderSystem::ClearConstantBuffers() const
    {
        for (auto& buffer : m_constant_buffers)
        {
            buffer->Clear();
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

    SPtr<ConstantBuffer> RenderSystem::GetConstantBuffer(eConstantBufferType type)
    {
        return m_constant_buffers[static_cast<Uint32>(type)];
    }
}
