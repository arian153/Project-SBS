/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "RenderSubsystem.hpp"

#include "GraphicsDefine.hpp"
#include "../Core/ComponentManager/Components/LightCompo.hpp"
#include "../Core/ComponentManager/Components/OrbitCameraCompo.hpp"
#include "../External/imgui/imgui.h"
#include "Element/Camera.hpp"
#include "Element/Light.hpp"
#include "Element/Material.hpp"
#include "Element/Model.hpp"
#include "Utility/MeshDataGenerator.hpp"
#include "Utility/PrimitiveRenderer.hpp"
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

        m_deferred_rect = std::make_shared<Model>();
        m_deferred_rect->SetMeshData(MeshDataGenerator::CreateRectangle(1.0f, 1.0f));
        m_deferred_rect->SetMaterial(MATERIAL_MANAGER->GetMaterial("Final"));

        m_primitive_renderer = std::make_shared<PrimitiveRenderer>();
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

        m_forward_compos.clear();
        m_deferred_compos.clear();

        //Do frustum culling
        for (auto& model : m_models)
        {
            model->ClearInstance();
        }

        MatrixParams matrix_params;
        matrix_params.view = m_curr_camera->GetViewMatrix();
        matrix_params.proj = m_perspective;

        m_primitive_renderer->Update(matrix_params.view, matrix_params.proj);

        for (auto& mesh_compo : m_mesh_compos)
        {
            mesh_compo->Update(dt);

            if (mesh_compo->IsDeferred())
                mesh_compo->UpdateInstanceData(matrix_params.view, matrix_params.proj);
            else
                m_forward_compos.push_back(mesh_compo);
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

        Uint32 back_index = RENDER_SYS_DX12->GetBackBufferIndex();

        // Clear Render targets
        {
            // Swap Chain
            RENDER_SYS->GetRTGroup(eRenderTargetGroupType::SwapChain)->ClearRenderTargetView(back_index);

            // Deferred G-Buffer Group
            RENDER_SYS->GetRTGroup(eRenderTargetGroupType::GBuffer)->ClearRenderTargetView();

            // Deferred Lighting Group
            RENDER_SYS->GetRTGroup(eRenderTargetGroupType::Lighting)->ClearRenderTargetView();
        }

        MatrixParams matrix_params;
        matrix_params.view = m_curr_camera->GetViewMatrix();
        matrix_params.proj = m_perspective;

        // DeferredGeo OMSet
        {
            RENDER_SYS->GetRTGroup(eRenderTargetGroupType::GBuffer)->OMSetRenderTargets();

            /* for (auto& mesh_compo : m_deferred_compos)
             {
                 matrix_params.world = mesh_compo->GetWorldMatrix();
                 matrix_params.wv    = matrix_params.world * matrix_params.view;
                 matrix_params.wvp   = matrix_params.world * matrix_params.view * matrix_params.proj;
                 GetConstantBuffer(eConstantBufferType::Transform)->PushData(&matrix_params, sizeof(matrix_params));
                 mesh_compo->Render();
             }*/

            m_primitive_renderer->RenderDeferred(GetConstantBuffer(eConstantBufferType::Material));

            if (m_show_meshes)
            {
                for (auto& model : m_models)
                {
                    if (model->IsInstanced())
                    {
                        model->Render(GetConstantBuffer(eConstantBufferType::Material));
                    }
                }
            }
            RENDER_SYS->GetRTGroup(eRenderTargetGroupType::GBuffer)->WaitTargetToResource();
        }

        // Light OMSet
        {
            RENDER_SYS->GetRTGroup(eRenderTargetGroupType::Lighting)->OMSetRenderTargets();
            Sint32 index = 0;
            for (auto& light_compo : m_light_compos)
            {
                matrix_params.world = light_compo->GetWorldMatrix();
                matrix_params.wv    = matrix_params.world * matrix_params.view;
                matrix_params.wvp   = matrix_params.world * matrix_params.view * matrix_params.proj;
                GetConstantBuffer(eConstantBufferType::Transform)->PushData(&matrix_params, sizeof(matrix_params));
                light_compo->DeferredBind(index);
                index++;
            }
            RENDER_SYS->GetRTGroup(eRenderTargetGroupType::Lighting)->WaitTargetToResource();
        }

        // Swap Chain OMSet
        {
            back_index = RENDER_SYS_DX12->GetBackBufferIndex();
            RENDER_SYS->GetRTGroup(eRenderTargetGroupType::SwapChain)->OMSetRenderTargets(1, back_index);

            // Render Deferred Final Rect
            m_deferred_rect->Render(GetConstantBuffer(eConstantBufferType::Material));

            // Render Forward Meshes
            m_primitive_renderer->RenderForward(GetConstantBuffer(eConstantBufferType::Material));
            m_primitive_renderer->RenderLines(
                                              GetConstantBuffer(eConstantBufferType::Transform),
                                              GetConstantBuffer(eConstantBufferType::Material));

            for (auto& mesh_compo : m_forward_compos)
            {
                matrix_params.world = mesh_compo->GetWorldMatrix();
                matrix_params.wv    = matrix_params.world * matrix_params.view;
                matrix_params.wvp   = matrix_params.world * matrix_params.view * matrix_params.proj;
                GetConstantBuffer(eConstantBufferType::Transform)->PushData(&matrix_params, sizeof(matrix_params));
                mesh_compo->Render();
            }
        }
    }

    void RenderSubsystem::Shutdown()
    {
        m_mesh_compos.clear();
        m_models.clear();
    }

    void RenderSubsystem::Edit()
    {
        if (ImGui::CollapsingHeader("Render Subsystem", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("Show Mesh", &m_show_meshes);
        }
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

    SPtr<Model> RenderSubsystem::AddModel(const String& name)
    {
        auto found = m_model_map.find(name);
        if (found == m_model_map.end())
        {
            auto model = std::make_shared<Model>();
            model->SetName(name);

            m_models.push_back(model);
            m_model_map.emplace(name, model);
            return model;
        }

        return found->second;
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

    SPtr<PrimitiveRenderer> RenderSubsystem::GetPrimitiveRenderer() const
    {
        return m_primitive_renderer;
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
