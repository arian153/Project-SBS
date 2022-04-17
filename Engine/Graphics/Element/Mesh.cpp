/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "Mesh.hpp"

#include "../GraphicsDefine.hpp"
#include "../DirectX12/Buffer/InstancingBuffer.hpp"

namespace Engine
{
    void Mesh::Init(const MeshData& mesh_data)
    {
        m_vertex_type = mesh_data.vertex_type;
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

    void Mesh::Init(const MeshData& mesh_data, eTopologyType type)
    {
        Init(mesh_data);

        switch (type)
        {
        case eTopologyType::DotList:
            m_topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
            break;
        case eTopologyType::LineList:
            m_topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
            break;
        case eTopologyType::TriangleList:
            m_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            break;
        default: ;
        }
    }

    void Mesh::Update(const MeshData& mesh_data) const
    {
        if (m_vertex_type != mesh_data.vertex_type)
            return;

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

    void Mesh::Init(const ForwardSubMesh& mesh_data, eTopologyType type)
    {
        m_vertex_type = eVertexType::PosVertex;
        m_vertex_buffer.Initialize(mesh_data.vertices, true);
        m_index_buffer.Initialize(mesh_data.indices);

        switch (type)
        {
        case eTopologyType::DotList:
            m_topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
            break;
        case eTopologyType::LineList:
            m_topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
            break;
        case eTopologyType::TriangleList:
            m_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            break;
        default: ;
        }
    }

    void Mesh::Update(const ForwardSubMesh& mesh_data) const
    {
        m_vertex_buffer.Update(mesh_data.vertices);
    }

    void Mesh::Init(const DeferredSubMesh& mesh_data)
    {
        m_topology    = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        m_vertex_type = eVertexType::NormalVertex;
        m_vertex_buffer.Initialize(mesh_data.vertices, true);
        m_index_buffer.Initialize(mesh_data.indices);
    }

    void Mesh::Update(const DeferredSubMesh& mesh_data) const
    {
        m_vertex_buffer.Update(mesh_data.vertices);
    }

    void Mesh::Render() const
    {
        CMD_LIST->IASetPrimitiveTopology(m_topology);
        m_vertex_buffer.Bind();
        m_index_buffer.Bind();
        DESCRIPTOR_HEAP->CommitTable();
        m_index_buffer.Draw();
    }

    void Mesh::Render(const SPtr<InstancingBuffer>& instancing_buffer) const
    {
        CMD_LIST->IASetPrimitiveTopology(m_topology);
        m_vertex_buffer.Bind(instancing_buffer);
        m_index_buffer.Bind();
        DESCRIPTOR_HEAP->CommitTable();
        m_index_buffer.Draw(instancing_buffer->GetInstanceCount());
    }
}
