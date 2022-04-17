/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "ShaderManager.hpp"

#include <algorithm>

#include "ShaderProgram.hpp"
#include "../../Core/CoreDefine.hpp"
#include "../../Core/ResourceManager/ResourceManager.hpp"
#include "../../Core/ResourceManager/ResourceType/ShaderResource.hpp"
#include "../../Utility/Utility.hpp"
#include "../Data/Vertex.hpp"
#include "../DirectX12/InputLayout.hpp"

namespace Engine
{
    bool CompareShaderProgram(SPtr<ShaderProgram> a, SPtr<ShaderProgram> b)
    {
        return std::strcmp(a->GetName().c_str(), b->GetName().c_str()) < 0;
    }

    ShaderManager::ShaderManager()
    {
    }

    ShaderManager::~ShaderManager()
    {
    }

    void ShaderManager::AddInputLayout(const String& name, const InputLayout& input_layout)
    {
        m_input_layouts.emplace(name, input_layout);
    }

    bool ShaderManager::AddShaderProgram(ShaderResource* shader_resource)
    {
        if (shader_resource->HasSubResource())
        {
            Uint32 count  = shader_resource->GetSubShaderCount();
            bool   result = true;

            for (Uint32 i = 0; i < count; ++i)
            {
                auto shader_program = std::make_shared<ShaderProgram>();

                if (shader_resource->IsIntegrated())
                {
                    shader_program->Initialize(
                                               shader_resource->GetShaderPath(),
                                               shader_resource->GetSubShaderEntry(i),
                                               shader_resource->GetSubShaderInfo(i));
                }
                else
                {
                    shader_program->Initialize(
                                               shader_resource->GetShaderPaths(),
                                               shader_resource->GetSubShaderEntry(i),
                                               shader_resource->GetSubShaderInfo(i));
                }

                shader_program->m_shader_name = shader_resource->GetSubShaderName(i);
                m_shader_map_name.emplace(shader_program->m_shader_name, shader_program);
                m_shader_programs.push_back(shader_program);

                if (shader_program->Compile() == false)
                    result = false;
            }

            return result;
        }

        auto shader_program = std::make_shared<ShaderProgram>();

        ShaderArg default_entry;
        default_entry.vertex_shader = "VS_Main";
        default_entry.pixel_shader  = "PS_Main";
        ShaderInfo default_info;
        default_info.shader_model = "5_0";
        default_info.input_layout = "DefaultLayout";

        shader_program->Initialize(shader_resource->GetShaderPath(), default_entry, default_info);

        shader_program->m_shader_name = shader_resource->FileName();
        m_shader_map_name.emplace(shader_resource->FileName(), shader_program);
        m_shader_programs.push_back(shader_program);

        return shader_program->Compile();
    }

    InputLayout& ShaderManager::GetInputLayout(const String& name)
    {
        auto found = m_input_layouts.find(name);
        if (found != m_input_layouts.end())
            return found->second;

        return m_input_layouts.find("DefaultLayout")->second;
    }

    const InputLayout& ShaderManager::GetInputLayout(const String& name) const
    {
        auto found = m_input_layouts.find(name);
        if (found != m_input_layouts.end())
            return found->second;

        return m_input_layouts.find("DefaultLayout")->second;
    }

    SPtr<ShaderProgram> ShaderManager::GetShaderProgram(const String& name)
    {
        auto found = m_shader_map_name.find(name);
        if (found != m_shader_map_name.end())
            return found->second;
        return nullptr;
    }

    bool ShaderManager::RecompileShader(const String& name)
    {
        auto found = m_shader_map_name.find(name);
        if (found != m_shader_map_name.end())
            return found->second->Compile();
        return false;
    }

    void ShaderManager::Initialize()
    {
        InputLayout instance_layout;
        instance_layout.PushAttribute(eAttributeType::R32, 4, "W", 0, eInputSlotType::InstanceData, 1, 1);
        instance_layout.PushAttribute(eAttributeType::R32, 4, "W", 1, eInputSlotType::InstanceData, 1, 1);
        instance_layout.PushAttribute(eAttributeType::R32, 4, "W", 2, eInputSlotType::InstanceData, 1, 1);
        instance_layout.PushAttribute(eAttributeType::R32, 4, "W", 3, eInputSlotType::InstanceData, 1, 1);

        instance_layout.PushAttribute(eAttributeType::R32, 4, "WV", 0, eInputSlotType::InstanceData, 1, 1);
        instance_layout.PushAttribute(eAttributeType::R32, 4, "WV", 1, eInputSlotType::InstanceData, 1, 1);
        instance_layout.PushAttribute(eAttributeType::R32, 4, "WV", 2, eInputSlotType::InstanceData, 1, 1);
        instance_layout.PushAttribute(eAttributeType::R32, 4, "WV", 3, eInputSlotType::InstanceData, 1, 1);


        instance_layout.PushAttribute(eAttributeType::R32, 4, "WVP", 0, eInputSlotType::InstanceData, 1, 1);
        instance_layout.PushAttribute(eAttributeType::R32, 4, "WVP", 1, eInputSlotType::InstanceData, 1, 1);
        instance_layout.PushAttribute(eAttributeType::R32, 4, "WVP", 2, eInputSlotType::InstanceData, 1, 1);
        instance_layout.PushAttribute(eAttributeType::R32, 4, "WVP", 3, eInputSlotType::InstanceData, 1, 1);


        //instance_layout.PushAttribute(eAttributeType::R32, 4, "MAT_AM", 0, eInputSlotType::InstanceData, 1, 1);
        instance_layout.PushAttribute(eAttributeType::R32, 4, "MAT_DI", 0, eInputSlotType::InstanceData, 1, 1);
        //instance_layout.PushAttribute(eAttributeType::R32, 4, "MAT_SP", 0, eInputSlotType::InstanceData, 1, 1);
        //instance_layout.PushAttribute(eAttributeType::R32, 4, "MAT_RE", 0, eInputSlotType::InstanceData, 1, 1);

        AddInputLayout("InstanceLayout", instance_layout);

        {
            InputLayout general_layout;
            general_layout.PushAttribute(eAttributeType::R32, 3, "POSITION", 0, eInputSlotType::VertexData, 0, 0);
            general_layout.PushAttribute(eAttributeType::R32, 2, "TEXCOORD", 0, eInputSlotType::VertexData, 0, 0);
            general_layout.PushAttribute(eAttributeType::R32, 3, "NORMAL", 0, eInputSlotType::VertexData, 0, 0);
            general_layout.PushAttribute(eAttributeType::R32, 3, "TANGENT", 0, eInputSlotType::VertexData, 0, 0);

            String name = MK_STRING(GeneralVertex);
            AddInputLayout(name, general_layout);
            AddInputLayout("DefaultLayout", general_layout);
            general_layout.PushLayout(instance_layout);
            AddInputLayout("Instance" + name, general_layout);
        }

        {
            InputLayout skinned_layout;
            skinned_layout.PushAttribute(eAttributeType::R32, 3, "POSITION", 0, eInputSlotType::VertexData, 0, 0);
            skinned_layout.PushAttribute(eAttributeType::R32, 2, "TEXCOORD", 0, eInputSlotType::VertexData, 0, 0);
            skinned_layout.PushAttribute(eAttributeType::R32, 3, "NORMAL", 0, eInputSlotType::VertexData, 0, 0);
            skinned_layout.PushAttribute(eAttributeType::R32, 3, "TANGENT", 0, eInputSlotType::VertexData, 0, 0);
            skinned_layout.PushAttribute(eAttributeType::R32, 4, "WEIGHTS", 0, eInputSlotType::VertexData, 0, 0);
            skinned_layout.PushAttribute(eAttributeType::I32, 4, "BONEINDICES", 0, eInputSlotType::VertexData, 0, 0);
            String name = MK_STRING(SkinnedVertex);
            AddInputLayout(name, skinned_layout);
            skinned_layout.PushLayout(instance_layout);
            AddInputLayout("Instance" + name, skinned_layout);
        }

        {
            InputLayout lighting_layout;
            lighting_layout.PushAttribute(eAttributeType::R32, 3, "POSITION", 0, eInputSlotType::VertexData, 0, 0);
            lighting_layout.PushAttribute(eAttributeType::R32, 2, "TEXCOORD", 0, eInputSlotType::VertexData, 0, 0);
            lighting_layout.PushAttribute(eAttributeType::R32, 3, "NORMAL", 0, eInputSlotType::VertexData, 0, 0);
            String name = MK_STRING(LightVertex);
            AddInputLayout(name, lighting_layout);
            lighting_layout.PushLayout(instance_layout);
            AddInputLayout("Instance" + name, lighting_layout);
        }

        {
            InputLayout normal_layout;
            normal_layout.PushAttribute(eAttributeType::R32, 3, "POSITION", 0, eInputSlotType::VertexData, 0, 0);
            normal_layout.PushAttribute(eAttributeType::R32, 3, "NORMAL", 0, eInputSlotType::VertexData, 0, 0);

            String name = MK_STRING(NormalVertex);
            AddInputLayout(name, normal_layout);
            normal_layout.PushLayout(instance_layout);
            AddInputLayout("Instance" + name, normal_layout);
        }

        {
            InputLayout tex_layout;
            tex_layout.PushAttribute(eAttributeType::R32, 3, "POSITION", 0, eInputSlotType::VertexData, 0, 0);
            tex_layout.PushAttribute(eAttributeType::R32, 2, "TEXCOORD", 0, eInputSlotType::VertexData, 0, 0);

            String name = MK_STRING(TexVertex);
            AddInputLayout(name, tex_layout);
            tex_layout.PushLayout(instance_layout);
            AddInputLayout("Instance" + name, tex_layout);
        }

        {
            InputLayout color_layout;
            color_layout.PushAttribute(eAttributeType::R32, 3, "POSITION", 0, eInputSlotType::VertexData, 0, 0);
            color_layout.PushAttribute(eAttributeType::R32, 4, "COLOR", 0, eInputSlotType::VertexData, 0, 0);

            String name = MK_STRING(ColorVertex);
            AddInputLayout(name, color_layout);
            color_layout.PushLayout(instance_layout);
            AddInputLayout("Instance" + name, color_layout);
        }

        {
            InputLayout pos_layout;
            pos_layout.PushAttribute(eAttributeType::R32, 3, "POSITION", 0, eInputSlotType::VertexData, 0, 0);

            String name = MK_STRING(PosVertex);
            AddInputLayout(name, pos_layout);
            pos_layout.PushLayout(instance_layout);
            AddInputLayout("Instance" + name, pos_layout);
        }

        RESOURCE_MANAGER->GetShaderResources(m_shader_resources);

        for (auto& resource : m_shader_resources)
        {
            AddShaderProgram(resource);
        }

        std::sort(m_shader_programs.begin(), m_shader_programs.end(), CompareShaderProgram);
    }

    void ShaderManager::Update()
    {
        std::vector<ShaderResource*> new_resource;
        std::vector<ShaderResource*> diff_resource;

        RESOURCE_MANAGER->GetShaderResources(new_resource);

        std::set_difference(
                            new_resource.begin(), new_resource.end(),
                            m_shader_resources.begin(), m_shader_resources.end(),
                            std::inserter(diff_resource, diff_resource.begin()));

        if (diff_resource.empty())
            return;

        for (auto& resource : diff_resource)
        {
            AddShaderProgram(resource);
        }

        std::sort(m_shader_programs.begin(), m_shader_programs.end(), CompareShaderProgram);
    }

    void ShaderManager::Shutdown()
    {
    }
}
