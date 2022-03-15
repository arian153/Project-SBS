#pragma once
#include "Mesh.hpp"
#include "../../EngineDefine.hpp"

namespace Engine
{
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

        void SetUp(RPtr<ModelResource> model_resource);
        void SetMeshData(const MeshData& mesh_data);

        void SetShader(SPtr<ShaderProgram> shader);
        void SetShader(size_t i, SPtr<ShaderProgram> shader);

        void SetMaterial(SPtr<Material> material);
        void SetMaterial(size_t i, SPtr<Material> material);

        void Render(SPtr<ConstantBuffer> material_buffer) const;
        void Render(size_t i, SPtr<ConstantBuffer> material_buffer) const;

        void Shutdown();

        void AddInstance(const InstancingParams& param);

        Material&       GetMaterial(size_t i);
        const Material& GetMaterial(size_t i) const;

        Mesh&       GetMesh(size_t i);
        const Mesh& GetMesh(size_t i) const;

        size_t Count() const;
        String GetName() const;
        void   SetName(const String& name);

    private:
        size_t m_count = 0;
        String m_name  = "";

        std::vector<Mesh>      m_meshes;
        std::vector<Material>  m_materials;
        SPtr<InstancingBuffer> m_instancing_buffer;
    };
}
