#pragma once
#include <d3dx12.h>

#include "../System.hpp"
#include "../Core/ApplicationSetting.hpp"
#include "../Math/Utility/MathDef.hpp"
#include "DirectX12/DirectXIncludes.hpp"
#include "Utility/ViewportManager.hpp"

namespace Engine
{
    enum class eConstantBufferType : Uint32;
    class ShaderManager;
    class TableDescriptorHeap;
    class ConstantBuffer;
    class RenderSubsystem;
    class RootSignature;
    class DirectX12Layer;

    class RenderSystem final : public System
    {
    public:
        RenderSystem();
        ~RenderSystem() override;

        void RenderBegin() const;
        void RenderEnd() const;

        SPtr<RenderSubsystem> CreateSubsystem();

    private:
        void Initialize() override;
        void Update() override;
        void Shutdown() override;

        void SetWindowInfo(const WindowInfo& info);

        void OnResize(Sint32 width, Sint32 height);
        void OnFullscreen(bool b_fullscreen);

        void PostInitialize();
        void CreateConstantBuffer(eCBVRegister reg, Uint32 buffer_size, Uint32 count);
        void ClearConstantBuffers() const;

    private:
        friend class CoreSystem;

    public:
        SPtr<DirectX12Layer>      GetDirectX12Layer();
        SPtr<RootSignature>       GetRootSignature();
        SPtr<TableDescriptorHeap> GetTableDescriptorHeap();
        SPtr<ShaderManager>       GetShaderManager();
        SPtr<ConstantBuffer>      GetConstantBuffer(eConstantBufferType type);
        ViewportManager&          GetViewportManager();
        const ViewportManager&    GetViewportManager() const;

    private:
        WindowInfo m_window_info;

        SPtr<DirectX12Layer>      m_dx12_layer;
        SPtr<RootSignature>       m_root_signature;
        SPtr<TableDescriptorHeap> m_table_descriptor_heap;
        SPtr<ShaderManager>       m_shader_manager;
        ViewportManager           m_viewport_manager;

        bool m_b_init = false;

        std::vector<SPtr<ConstantBuffer>>  m_constant_buffers;
        std::vector<SPtr<RenderSubsystem>> m_subsystems;
    };
}
