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
#include "../Data/MeshData.hpp"
#include "../Data/SubMeshData.hpp"
#include "../DirectX12/Buffer/IndexBuffer.hpp"
#include "../DirectX12/Buffer/VertexBuffer.hpp"
#include "../Shader/ShaderInfo.hpp"

namespace Engine
{
    class InstancingBuffer;

    class Mesh
    {
    public:
        void Init(const MeshData& mesh_data);
        void Init(const MeshData& mesh_data, eTopologyType type);
        void Update(const MeshData& mesh_data) const;
        void Shutdown();

        void Init(const ForwardSubMesh& mesh_data, eTopologyType type);
        void Update(const ForwardSubMesh& mesh_data) const;

        void Init(const DeferredSubMesh& mesh_data);
        void Update(const DeferredSubMesh& mesh_data) const;

        void Render() const;
        void Render(const SPtr<InstancingBuffer>& instancing_buffer) const;

    private:
        eVertexType            m_vertex_type = eVertexType::SkinnedVertex;
        D3D_PRIMITIVE_TOPOLOGY m_topology    = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

        VertexBuffer m_vertex_buffer;
        IndexBuffer  m_index_buffer;
    };
}
