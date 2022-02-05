#pragma once
#include "../System.hpp"
#include "../Core/ComponentManager/Components/MeshCompo.hpp"
#include "Element/Mesh.hpp"

namespace Engine
{
    class RenderSubsystem final : public Subsystem
    {
    public:
        RenderSubsystem();
        ~RenderSubsystem() override;

        void Initialize() override;
        void Update(Real dt) override;
        void Render() override;
        void Shutdown() override;

        void AddMeshCompo(RPtr<MeshCompo> compo);
        void RemoveMeshCompo(RPtr<MeshCompo> compo);

    private:


        std::vector<SPtr<Mesh>>      m_meshes;
        std::vector<RPtr<MeshCompo>> m_mesh_compos;
    };
}
