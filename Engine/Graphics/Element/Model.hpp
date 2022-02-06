#pragma once
#include "Mesh.hpp"
#include "../../EngineDefine.hpp"

namespace Engine
{
    class Material;
    class ModelResource;

    class Model
    {
    public:
        Model();
        ~Model();

        void SetUp(RPtr<ModelResource> model_resource);

        void Bind() const;
        void Render() const;

        void Bind(size_t i) const;
        void Render(size_t i) const;

        void Shutdown();

    private:
        size_t m_mesh_count = 0;

        std::vector<Mesh>           m_meshes;
        std::vector<SPtr<Material>> m_materials;
    };
}
