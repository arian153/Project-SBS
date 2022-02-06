#include "RenderSubsystem.hpp"

#include "Element/Camera.hpp"
#include "Element/Material.hpp"
#include "Element/Model.hpp"

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
        CreateConstantBuffer(eCBVRegister::b0, sizeof(Vector4), 256);
        CreateConstantBuffer(eCBVRegister::b1, sizeof(MaterialParams), 256);
    }

    void RenderSubsystem::Update(Real dt)
    {
    }

    void RenderSubsystem::Render()
    {
        for (auto& model : m_models)
        {
            model->Bind(GetConstantBuffer(eConstantBufferType::Material));
            model->Render();
        }
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
