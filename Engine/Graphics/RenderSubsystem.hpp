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
#include "../System.hpp"
#include "../Core/ComponentManager/Components/MeshCompo.hpp"
#include "../Math/Algebra/Matrix44.hpp"
#include "DirectX12/Buffer/ConstantBuffer.hpp"
#include "Element/Mesh.hpp"

namespace Engine
{
    class PrimitiveRenderer;
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

        void Edit();

        void AddMeshCompo(RPtr<MeshCompo> compo);
        void RemoveMeshCompo(RPtr<MeshCompo> compo);

        void AddOrbitCompo(RPtr<OrbitCameraCompo> compo);
        void RemoveOrbitCompo(RPtr<OrbitCameraCompo> compo);

        void AddLightCompo(RPtr<LightCompo> compo);
        void RemoveLightCompo(RPtr<LightCompo> compo);

        void OnResize(const Matrix44& perspective, const Matrix44& orthographic);

        SPtr<Model>  AddModel(const String& name);
        SPtr<Camera> CreateCamera();

        void SetCurrentCamera(SPtr<Camera> camera);

        SPtr<ConstantBuffer> GetConstantBuffer(eConstantBufferType type);
        SPtr<PrimitiveRenderer> GetPrimitiveRenderer() const;

    private:
        friend class RenderSystem;
    private:
        void CreateConstantBuffer(eCBVRegister reg, Uint32 buffer_size, Uint32 count);
        void ClearConstantBuffers() const;

    private:
        Matrix44 m_perspective;
        Matrix44 m_orthographic;

        SPtr<Camera> m_curr_camera   = nullptr;
        SPtr<Model>  m_deferred_rect = nullptr;

        bool m_show_meshes = true;

        std::vector<SPtr<Camera>>           m_cameras;
        std::vector<RPtr<LightCompo>>       m_light_compos;
        std::vector<RPtr<MeshCompo>>        m_mesh_compos;
        std::vector<RPtr<MeshCompo>>        m_deferred_compos;
        std::vector<RPtr<MeshCompo>>        m_forward_compos;
        std::vector<SPtr<Model>>            m_models;
        std::vector<RPtr<OrbitCameraCompo>> m_orbit_compos;
        std::vector<SPtr<ConstantBuffer>>   m_constant_buffers;
        HashMap<String, SPtr<Model>>        m_model_map;
        std::vector<SPtr<Model>>            m_deferred_models;

        SPtr<PrimitiveRenderer> m_primitive_renderer = nullptr;
    };
}
