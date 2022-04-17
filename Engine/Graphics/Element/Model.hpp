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
#include "Mesh.hpp"
#include "../../EngineDefine.hpp"

namespace Engine
{
    struct DeferredSubMesh;
    struct ForwardSubMesh;
    struct InstancingParams;
    class InstancingBuffer;
    class ConstantBuffer;
    class ShaderProgram;
    class Material;
    class ModelResource;

    class Model
    {
    public:
        Model();
        ~Model();

        void SetSize(size_t count);

        void SetUp(RPtr<ModelResource> model_resource);
        void SetMeshData(const MeshData& mesh_data);
        void SetMeshData(const ForwardSubMesh& mesh_data, eTopologyType type);
        void SetMeshData(const DeferredSubMesh& mesh_data);

        void UpdateMeshData(const MeshData& mesh_data) const;

        void SetShader(SPtr<ShaderProgram> shader);
        void SetShader(size_t i, SPtr<ShaderProgram> shader);

        void SetMaterial(SPtr<Material> material);
        void SetMaterial(size_t i, SPtr<Material> material);

        void Render(SPtr<ConstantBuffer> material_buffer) const;
        void Render(size_t i, SPtr<ConstantBuffer> material_buffer) const;

        void Shutdown();

        void AddInstance(const InstancingParams& param);
        void ClearInstance();

        Material&       GetMaterial(size_t i);
        const Material& GetMaterial(size_t i) const;

        Mesh&       GetMesh(size_t i);
        const Mesh& GetMesh(size_t i) const;

        size_t Count() const;
        String GetName() const;
        void   SetName(const String& name);

        bool IsInstanced() const;
        bool IsDeferred() const;

    private:
        size_t m_count        = 0;
        String m_name         = "";
        bool   m_is_instanced = false;
        bool   m_is_deferred  = false;

        std::vector<Mesh>      m_meshes;
        std::vector<Material>  m_materials;
        SPtr<InstancingBuffer> m_instancing_buffer;
    };
}
