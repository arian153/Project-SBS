/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#pragma once
#include <array>
#include <d3dx12.h>

#include "../System.hpp"
#include "../Core/ApplicationSetting.hpp"
#include "../Math/Utility/MathDef.hpp"
#include "DirectX12/DirectXIncludes.hpp"
#include "DirectX12/RenderTarget.hpp"
#include "Utility/ViewportManager.hpp"

namespace Engine
{
    class MaterialManager;
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

        void ClearConstantBuffers() const;

    private:
        void Initialize() override;
        void Update() override;
        void Shutdown() override;

        void SetWindowInfo(const WindowInfo& info);

        void OnResize(Sint32 width, Sint32 height);
        void OnFullscreen(bool b_fullscreen);

        void PostInitialize();
        void CreateRenderTargetGroups();
        void ResetRenderTargetGroups() const;

    private:
        friend class CoreSystem;
        friend class DirectX12Layer;

    public:
        SPtr<DirectX12Layer>      GetDirectX12Layer();
        SPtr<RootSignature>       GetRootSignature();
        SPtr<TableDescriptorHeap> GetTableDescriptorHeap();
        SPtr<ShaderManager>       GetShaderManager();
        SPtr<MaterialManager>     GetMaterialManager();

        ViewportManager&       GetViewportManager();
        const ViewportManager& GetViewportManager() const;

        SPtr<RenderTargetGroup> GetRTGroup(eRenderTargetGroupType type);

    private:
        WindowInfo m_window_info;

        SPtr<DirectX12Layer>      m_dx12_layer;
        SPtr<RootSignature>       m_root_signature;
        SPtr<TableDescriptorHeap> m_table_descriptor_heap;
        SPtr<ShaderManager>       m_shader_manager;
        SPtr<MaterialManager>     m_material_manager;
        ViewportManager           m_viewport_manager;

        bool m_b_init        = false;
        bool m_b_created_rtg = false;

        std::array<SPtr<RenderTargetGroup>, RENDER_TARGET_GROUP_COUNT> m_rt_groups;
        std::vector<SPtr<RenderSubsystem>>                             m_subsystems;
    };
}
