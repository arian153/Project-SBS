#include "Model.hpp"

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
        m_mesh_count = model_resource->Count();
        m_meshes.resize(m_mesh_count);

        for (size_t i = 0; i < m_mesh_count; ++i)
        {
            m_meshes[0].Init(model_resource->GetMeshData(i));
        }
    }

    void Model::Bind() const
    {
        for (size_t i = 0; i < m_mesh_count; ++i)
        {
            m_meshes[i].Bind();
        }
    }

    void Model::Render() const
    {
        for (size_t i = 0; i < m_mesh_count; ++i)
        {
            m_meshes[i].Render();
        }
    }

    void Model::Bind(size_t i) const
    {
        m_meshes[i].Bind();
    }

    void Model::Render(size_t i) const
    {
        m_meshes[i].Render();
    }

    void Model::Shutdown()
    {
        for (size_t i = 0; i < m_mesh_count; ++i)
        {
            m_meshes[i].Shutdown();
        }
    }
}
