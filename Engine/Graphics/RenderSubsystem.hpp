#pragma once
#include "../System.hpp"
#include "../Core/ComponentManager/Components/MeshCompo.hpp"
#include "../Math/Algebra/Matrix44.hpp"
#include "DirectX12/Buffer/ConstantBuffer.hpp"
#include "Element/Mesh.hpp"

namespace Engine
{
    class LightCompo;
    class OrbitCameraCompo;
    class Camera;
    class ModelResource;
    class Model;

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

        void AddOrbitCompo(RPtr<OrbitCameraCompo> compo);
        void RemoveOrbitCompo(RPtr<OrbitCameraCompo> compo);

        void AddLightCompo(RPtr<LightCompo> compo);
        void RemoveLightCompo(RPtr<LightCompo> compo);

        void OnResize(const Matrix44& perspective, const Matrix44& orthographic);

        SPtr<Model>  CreateModel();
        SPtr<Camera> CreateCamera();

        void SetCurrentCamera(SPtr<Camera> camera);

        SPtr<ConstantBuffer> GetConstantBuffer(eConstantBufferType type);

    private:
        friend class RenderSystem;
    private:
        void CreateConstantBuffer(eCBVRegister reg, Uint32 buffer_size, Uint32 count);
        void ClearConstantBuffers() const;

    private:
        Matrix44 m_perspective;
        Matrix44 m_orthographic;

        SPtr<Camera> m_curr_camera = nullptr;

        std::vector<SPtr<Camera>>           m_cameras;
        std::vector<RPtr<LightCompo>>       m_light_compos;
        std::vector<RPtr<MeshCompo>>        m_mesh_compos;
        std::vector<RPtr<MeshCompo>>        m_deferred_compos;
        std::vector<RPtr<MeshCompo>>        m_forward_compos;
        std::vector<SPtr<Model>>            m_models;
        std::vector<RPtr<OrbitCameraCompo>> m_orbit_compos;
        std::vector<SPtr<ConstantBuffer>>   m_constant_buffers;
    };
}
