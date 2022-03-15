#include "Model.hpp"

#include "../../Core/CoreDefine.hpp"
#include "../../Core/ResourceManager/ResourceManager.hpp"
#include "../../Core/ResourceManager/ResourceType/ModelResource.hpp"

namespace Engine
{
    Model::Model()
    {
    }

    Model::~Model()
    {
    }

    void Model::SetUp(RPtr<ModelResource> model_resource)
    {
        m_count = model_resource->Count();
        m_meshes.resize(m_count);
        m_materials.resize(m_count);

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

    void Model::SetShader(SPtr<ShaderProgram> shader)
    {
        for (size_t i = 0; i < m_count; ++i)
        {
            m_materials[i].SetShader(shader);
        }
    }

    void Model::SetShader(size_t i, SPtr<ShaderProgram> shader)
    {
        m_materials[i].SetShader(shader);
    }

    void Model::SetMaterial(SPtr<Material> material)
    {
        for (size_t i = 0; i < m_count; ++i)
        {
            m_materials[i].Set(material);
        }
    }

    void Model::SetMaterial(size_t i, SPtr<Material> material)
    {
        m_materials[i].Set(material);
    }

    void Model::Bind(SPtr<ConstantBuffer> material_buffer) const
    {
        for (size_t i = 0; i < m_count; ++i)
        {
            m_meshes[i].Bind();
            m_materials[i].Bind(material_buffer);
        }
    }

    void Model::Render() const
    {
        for (size_t i = 0; i < m_count; ++i)
        {
            m_meshes[i].Render();
        }
    }

    void Model::Bind(size_t i, SPtr<ConstantBuffer> material_buffer) const
    {
        m_meshes[i].Bind();
        m_materials[i].Bind(material_buffer);
    }

    void Model::Render(size_t i) const
    {
        m_meshes[i].Render();
    }

    void Model::Shutdown()
    {
        for (size_t i = 0; i < m_count; ++i)
        {
            m_meshes[i].Shutdown();
        }
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
}
