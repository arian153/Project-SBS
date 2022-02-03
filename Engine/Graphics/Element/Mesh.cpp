#include "Mesh.hpp"

#include "../GraphicsDefine.hpp"

namespace Engine
{
    void Mesh::Init(const MeshData& mesh_data)
    {
        if (mesh_data.vertex_type == eVertexType::PosVertex)
        {
            std::vector<PosVertex> vertices;
            mesh_data.Convert(vertices);
            m_vertex_buffer.Initialize(vertices, true);
        }
        else if (mesh_data.vertex_type == eVertexType::TexVertex)
        {
            std::vector<TexVertex> vertices;
            mesh_data.Convert(vertices);
            m_vertex_buffer.Initialize(vertices, true);
        }
        else if (mesh_data.vertex_type == eVertexType::NormalVertex)
        {
            std::vector<NormalVertex> vertices;
            mesh_data.Convert(vertices);
            m_vertex_buffer.Initialize(vertices, true);
        }
        else if (mesh_data.vertex_type == eVertexType::LightVertex)
        {
            std::vector<LightVertex> vertices;
            mesh_data.Convert(vertices);
            m_vertex_buffer.Initialize(vertices, true);
        }
        else if (mesh_data.vertex_type == eVertexType::GeneralVertex)
        {
            std::vector<GeneralVertex> vertices;
            mesh_data.Convert(vertices);
            m_vertex_buffer.Initialize(vertices, true);
        }
        else
        {
            m_vertex_buffer.Initialize(mesh_data.vertices, true);
        }

        m_index_buffer.Initialize(mesh_data.indices);
    }

    void Mesh::Update(const MeshData& mesh_data) const
    {
        if (mesh_data.vertex_type == eVertexType::PosVertex)
        {
            std::vector<PosVertex> vertices;
            mesh_data.Convert(vertices);
            m_vertex_buffer.Update(vertices);
        }
        else if (mesh_data.vertex_type == eVertexType::TexVertex)
        {
            std::vector<TexVertex> vertices;
            mesh_data.Convert(vertices);
            m_vertex_buffer.Update(vertices);
        }
        else if (mesh_data.vertex_type == eVertexType::NormalVertex)
        {
            std::vector<NormalVertex> vertices;
            mesh_data.Convert(vertices);
            m_vertex_buffer.Update(vertices);
        }
        else if (mesh_data.vertex_type == eVertexType::LightVertex)
        {
            std::vector<LightVertex> vertices;
            mesh_data.Convert(vertices);
            m_vertex_buffer.Update(vertices);
        }
        else if (mesh_data.vertex_type == eVertexType::GeneralVertex)
        {
            std::vector<GeneralVertex> vertices;
            mesh_data.Convert(vertices);
            m_vertex_buffer.Update(vertices);
        }
        else
        {
            m_vertex_buffer.Update(mesh_data.vertices);
        }
    }

    void Mesh::Shutdown()
    {
        m_vertex_buffer.Shutdown();
        m_index_buffer.Shutdown();
    }

    void Mesh::Bind() const
    {
        CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_vertex_buffer.Bind();
        m_index_buffer.Bind();
    }

    void Mesh::Render() const
    {
        DESCRIPTOR_HEAP->CommitTable();
        m_index_buffer.Draw();
    }
}
