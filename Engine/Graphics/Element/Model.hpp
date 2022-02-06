#pragma once
#include "Mesh.hpp"
#include "../../EngineDefine.hpp"

namespace Engine
{
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

        void Bind(SPtr<ConstantBuffer> material_buffer) const;
        void Render() const;

        void Bind(size_t i, SPtr<ConstantBuffer> material_buffer) const;
        void Render(size_t i) const;

        void Shutdown();

        Material&       GetMaterial(size_t i);
        const Material& GetMaterial(size_t i) const;

        Mesh& GetMesh(size_t i);
        const Mesh& GetMesh(size_t i) const;

        size_t Count() const;

    private:
        size_t m_count = 0;
        

        std::vector<Mesh>     m_meshes;
        std::vector<Material> m_materials;
    };
}
