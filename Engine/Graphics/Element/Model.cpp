/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "Model.hpp"

#include "../../Core/CoreDefine.hpp"
#include "../../Core/ResourceManager/ResourceManager.hpp"
#include "../../Core/ResourceManager/ResourceType/ModelResource.hpp"
#include "../DirectX12/Buffer/InstancingBuffer.hpp"
#include "../Shader/ShaderProgram.hpp"

namespace Engine
{
    Model::Model()
    {
    }

    Model::~Model()
    {
    }

    void Model::SetSize(size_t count)
    {
        m_count = count;
        m_meshes.resize(count);
        m_materials.resize(count);
    }

    void Model::SetUp(RPtr<ModelResource> model_resource)
    {
        m_count = model_resource->Count();
        m_meshes.resize(m_count);
        m_materials.resize(m_count);

        //TODO Change this loading match to material setting

        for (size_t i = 0; i < m_count; ++i)
        {
            m_meshes[i].Init(model_resource->GetMeshData(i));

            auto& [diff, spec, norm] = model_resource->GetPathData(i);

            Uint32 index = 0;
            size_t size  = diff.size();
            for (size_t j = 0; j < size; ++j)
            {
                m_materials[i].SetTexture(index, RESOURCE_MANAGER->GetTextureResourcePath(diff[j])->GetTexture());
                index++;
            }

            size = spec.size();
            for (size_t j = 0; j < size; ++j)
            {
                m_materials[i].SetTexture(index, RESOURCE_MANAGER->GetTextureResourcePath(spec[j])->GetTexture());
                index++;
            }

            size = norm.size();
            for (size_t j = 0; j < size; ++j)
            {
                m_materials[i].SetTexture(index, RESOURCE_MANAGER->GetTextureResourcePath(norm[j])->GetTexture());
                index++;
            }
        }
    }

    void Model::SetMeshData(const MeshData& mesh_data)
    {
        m_meshes.resize(1);
        m_materials.resize(1);
        m_count = 1;

        m_meshes[0].Init(mesh_data);
    }

    void Model::SetMeshData(const ForwardSubMesh& mesh_data, eTopologyType type)
    {
        m_meshes.resize(1);
        m_materials.resize(1);
        m_count = 1;

        m_meshes[0].Init(mesh_data, type);
    }

    void Model::SetMeshData(const DeferredSubMesh& mesh_data)
    {
        m_meshes.resize(1);
        m_materials.resize(1);
        m_count = 1;

        m_meshes[0].Init(mesh_data);
    }

    void Model::UpdateMeshData(const MeshData& mesh_data) const
    {
        m_meshes[0].Update(mesh_data);
    }

    void Model::SetShader(SPtr<ShaderProgram> shader)
    {
        for (size_t i = 0; i < m_count; ++i)
        {
            m_materials[i].SetShader(shader);
        }

        m_is_deferred = shader->IsDeferred();
    }

    void Model::SetShader(size_t i, SPtr<ShaderProgram> shader)
    {
        m_materials[i].SetShader(shader);

        m_is_deferred = shader->IsDeferred();
    }

    void Model::SetMaterial(SPtr<Material> material)
    {
        for (size_t i = 0; i < m_count; ++i)
        {
            m_materials[i].Set(material);
        }

        m_is_deferred = material->GetShader()->IsDeferred();
    }

    void Model::SetMaterial(size_t i, SPtr<Material> material)
    {
        m_materials[i].Set(material);
        m_is_deferred = material->GetShader()->IsDeferred();
    }

    void Model::Render(SPtr<ConstantBuffer> material_buffer) const
    {
        if (m_instancing_buffer == nullptr)
        {
            for (size_t i = 0; i < m_count; ++i)
            {
                m_materials[i].Bind(material_buffer);
                m_meshes[i].Render();
            }
        }
        else
        {
            for (size_t i = 0; i < m_count; ++i)
            {
                m_materials[i].Bind(material_buffer);
                m_instancing_buffer->Bind();
                m_meshes[i].Render(m_instancing_buffer);
            }
        }
    }

    void Model::Render(size_t i, SPtr<ConstantBuffer> material_buffer) const
    {
        if (m_instancing_buffer == nullptr)
        {
            m_materials[i].Bind(material_buffer);
            m_meshes[i].Render();
        }
        else
        {
            m_materials[i].Bind(material_buffer);
            m_instancing_buffer->Bind();
            m_meshes[i].Render(m_instancing_buffer);
        }
    }

    void Model::Shutdown()
    {
        for (size_t i = 0; i < m_count; ++i)
        {
            m_meshes[i].Shutdown();
        }
    }

    void Model::AddInstance(const InstancingParams& param)
    {
        if (m_instancing_buffer == nullptr)
        {
            m_instancing_buffer = std::make_shared<InstancingBuffer>();
            m_instancing_buffer->Init();
        }

        m_instancing_buffer->AddData(param);
        m_is_instanced = true;
    }

    void Model::ClearInstance() 
    {
        if (m_instancing_buffer != nullptr)
            m_instancing_buffer->Clear();

        m_is_instanced = false;
    }

    Material& Model::GetMaterial(size_t i)
    {
        return m_materials[i];
    }

    const Material& Model::GetMaterial(size_t i) const
    {
        return m_materials[i];
    }

    Mesh& Model::GetMesh(size_t i)
    {
        return m_meshes[i];
    }

    const Mesh& Model::GetMesh(size_t i) const
    {
        return m_meshes[i];
    }

    size_t Model::Count() const
    {
        return m_count;
    }

    String Model::GetName() const
    {
        return m_name;
    }

    void Model::SetName(const String& name)
    {
        m_name = name;
    }

    bool Model::IsInstanced() const
    {
        return m_is_instanced;
    }

    bool Model::IsDeferred() const
    {
        return m_is_deferred;
    }
}
