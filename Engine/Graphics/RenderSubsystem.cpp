#include "RenderSubsystem.hpp"

#include "../Core/ComponentManager/Components/LightCompo.hpp"
#include "../Core/ComponentManager/Components/OrbitCameraCompo.hpp"
#include "Element/Camera.hpp"
#include "Element/Light.hpp"
#include "Element/Material.hpp"
#include "Element/Model.hpp"
#include "Utility/ViewportManager.hpp"

namespace Engine
{
    RenderSubsystem::RenderSubsystem()
    {
    }

    RenderSubsystem::~RenderSubsystem()
    {
    }

    void RenderSubsystem::Initialize()
    {
        CreateConstantBuffer(eCBVRegister::b0, sizeof(LightParams), 1);
        CreateConstantBuffer(eCBVRegister::b1, sizeof(MatrixParams), 256);
        CreateConstantBuffer(eCBVRegister::b2, sizeof(MaterialParams), 256);
    }

    void RenderSubsystem::Update(Real dt)
    {
        for (auto& orbit_cam : m_orbit_compos)
        {
            orbit_cam->Update(dt);
        }

        for (auto& light_compo : m_light_compos)
        {
            light_compo->Update(dt);
        }

        for (auto& mesh_compo : m_mesh_compos)
        {
            mesh_compo->Update(dt);
        }
    }

    void RenderSubsystem::Render()
    {
        //global constant buffer per frame
        {
            LightParams light_params;
            light_params.light_count = static_cast<Uint32>(m_light_compos.size());
            for (Uint32 i = 0; i < light_params.light_count; ++i)
            {
                light_params.lights[i] = m_light_compos[i]->GetLightInfo();
            }

            GetConstantBuffer(eConstantBufferType::GlobalPerFrame)->SetGlobalData(&light_params, sizeof(light_params));
        }

        MatrixParams matrix_params;
        matrix_params.view = m_curr_camera->GetViewMatrix();
        matrix_params.proj = m_perspective;

        for (auto& mesh_compo : m_mesh_compos)
        {
            matrix_params.world = mesh_compo->GetWorldMatrix();
            matrix_params.wv = matrix_params.world * matrix_params.view;
            matrix_params.wvp = matrix_params.world * matrix_params.view * matrix_params.proj;
            GetConstantBuffer(eConstantBufferType::Transform)->PushData(&matrix_params, sizeof(matrix_params));
            mesh_compo->Render();
        }

        /* for (auto& model : m_models)
         {
             model->Bind(GetConstantBuffer(eConstantBufferType::Material));
             model->Render();
         }*/
    }

    void RenderSubsystem::Shutdown()
    {
        m_mesh_compos.clear();
        m_models.clear();
    }

    void RenderSubsystem::AddMeshCompo(RPtr<MeshCompo> compo)
    {
        m_mesh_compos.push_back(compo);
    }

    void RenderSubsystem::RemoveMeshCompo(RPtr<MeshCompo> compo)
    {
        auto found = std::find(m_mesh_compos.begin(), m_mesh_compos.end(), compo);
        if (found != m_mesh_compos.end())
        {
            m_mesh_compos.erase(found);
        }
    }

    void RenderSubsystem::AddOrbitCompo(RPtr<OrbitCameraCompo> compo)
    {
        m_orbit_compos.push_back(compo);
    }

    void RenderSubsystem::RemoveOrbitCompo(RPtr<OrbitCameraCompo> compo)
    {
        auto found = std::find(m_orbit_compos.begin(), m_orbit_compos.end(), compo);
        if (found != m_orbit_compos.end())
        {
            m_orbit_compos.erase(found);
        }
    }

    void RenderSubsystem::AddLightCompo(RPtr<LightCompo> compo)
    {
        m_light_compos.push_back(compo);
    }

    void RenderSubsystem::RemoveLightCompo(RPtr<LightCompo> compo)
    {
        auto found = std::find(m_light_compos.begin(), m_light_compos.end(), compo);
        if (found != m_light_compos.end())
        {
            m_light_compos.erase(found);
        }
    }

    void RenderSubsystem::OnResize(const Matrix44& perspective, const Matrix44& orthographic)
    {
        m_perspective  = perspective;
        m_orthographic = orthographic;
    }

    SPtr<Model> RenderSubsystem::CreateModel()
    {
        auto model = std::make_shared<Model>();
        m_models.push_back(model);
        return model;
    }

    SPtr<Camera> RenderSubsystem::CreateCamera()
    {
        auto camera = std::make_shared<Camera>();
        m_cameras.push_back(camera);
        return camera;
    }

    void RenderSubsystem::SetCurrentCamera(SPtr<Camera> camera)
    {
        m_curr_camera = camera;
    }

    SPtr<ConstantBuffer> RenderSubsystem::GetConstantBuffer(eConstantBufferType type)
    {
        return m_constant_buffers[static_cast<Uint32>(type)];
    }

    void RenderSubsystem::CreateConstantBuffer(eCBVRegister reg, Uint32 buffer_size, Uint32 count)
    {
        Uint32 idx = static_cast<Uint32>(reg);
        assert(m_constant_buffers.size() == idx);

        auto buffer = std::make_shared<ConstantBuffer>();
        buffer->Init(reg, buffer_size, count);
        m_constant_buffers.push_back(buffer);
    }

    void RenderSubsystem::ClearConstantBuffers() const
    {
        for (auto& buffer : m_constant_buffers)
        {
            buffer->Clear();
        }
    }
}
