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
#include "RenderSystem.hpp"
#include "../EngineDefine.hpp"
#include "../Core/CoreSystem.hpp"
#include "DirectX12/DirectX12Layer.hpp"
#include "DirectX12/RootSignature.hpp"
#include "DirectX12/TableDescriptorHeap.hpp"
#include "Shader/ShaderManager.hpp"
#include "Shader/ShaderProgram.hpp"
#include "Utility/MaterialManager.hpp"
#include "Utility/ViewportManager.hpp"

namespace Engine
{
    #define DEVICE			g_core->GetRenderSystem()->GetDirectX12Layer()->GetDevice()
    #define CMD_LIST		g_core->GetRenderSystem()->GetDirectX12Layer()->GetCmdList()
    #define CMD_QUEUE		g_core->GetRenderSystem()->GetDirectX12Layer()->GetCmdQueue()
    #define ROOT_SIGNATURE	g_core->GetRenderSystem()->GetRootSignature()->GetSignature()
    #define DESCRIPTOR_HEAP g_core->GetRenderSystem()->GetTableDescriptorHeap()

    #define RESOURCE_CMD_LIST g_core->GetRenderSystem()->GetDirectX12Layer()->GetResourceCmdList()
    #define RENDER_SYS g_core->GetRenderSystem()
    #define RENDER_SYS_DX12 g_core->GetRenderSystem()->GetDirectX12Layer()

    #define SHADER_MANAGER g_core->GetRenderSystem()->GetShaderManager()
    #define VIEWPORT_MANAGER g_core->GetRenderSystem()->GetViewportManager()
    #define MATERIAL_MANAGER g_core->GetRenderSystem()->GetMaterialManager()

    #define GET_SHADER(s) g_core->GetRenderSystem()->GetShaderManager()->GetShaderProgram(s)
}
