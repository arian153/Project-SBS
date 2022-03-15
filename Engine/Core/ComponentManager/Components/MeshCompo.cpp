#include "MeshCompo.hpp"

#include "TransformCompo.hpp"
#include "../../../External/imgui/imgui.h"
#include "../../../Graphics/DirectX12/Texture.hpp"
#include "../../../Graphics/Element/Material.hpp"
#include "../../../Graphics/Element/Model.hpp"
#include "../../../Graphics/Shader/ShaderProgram.hpp"
#include "../../AppStateManager/AppState.hpp"
#include "../../ObjectManager/Object.hpp"
#include "../../ResourceManager/ResourceType/JsonData.hpp"
#include "../../ResourceManager/ResourceType/ModelResource.hpp"

namespace Engine
{
    MeshCompo::MeshCompo()
    {
    }

    MeshCompo::~MeshCompo()
    {
    }

    void MeshCompo::Initialize()
    {
        Subscribe();
    }

    void MeshCompo::Update(Real dt)
    {
    }

    void MeshCompo::Shutdown()
    {
        Unsubscribe();
    }

    bool MeshCompo::Load(const Json::Value& data)
    {
        if (data.isNull())
            return false;

        return true;
    }

    void MeshCompo::Save(Json::Value& data) const
    {
    }

    void MeshCompo::Edit(CommandRegistry* command_registry)
    {
        ImGui::CollapsingHeader(MK_STRING(MeshCompo));
    }

    void MeshCompo::CloneTo(RPtr<Component> destination)
    {
    }

    void MeshCompo::CreateModel(const String& name)
    {
        if (m_model == nullptr)
        {
            m_model = m_space->GetRenderSubsystem()->AddModel(name);
        }
    }

    void MeshCompo::SetModelResource(RPtr<ModelResource> resource)
    {
        m_model_resource = resource;
        m_model->SetUp(resource);
    }

    void MeshCompo::SetMaterial(SPtr<Material> material) const
    {
        size_t size = m_model->Count();

        for (size_t i = 0; i < size; ++i)
        {
            m_model->SetMaterial(material);
        }
    }

    void MeshCompo::SetMaterialInfoReal(Uint32 index, Real value) const
    {
        size_t size = m_model->Count();

        for (size_t i = 0; i < size; ++i)
        {
            m_model->GetMaterial(i).SetReal(index, value);
        }
    }

    void MeshCompo::SetMaterialInfo(Uint32 index, Sint32 value) const
    {
        size_t size = m_model->Count();

        for (size_t i = 0; i < size; ++i)
        {
            m_model->GetMaterial(i).SetSint(index, value);
        }
    }

    void MeshCompo::SetMaterialTexture(Uint32 index, SPtr<Texture> texture) const
    {
        size_t size = m_model->Count();

        for (size_t i = 0; i < size; ++i)
        {
            m_model->GetMaterial(i).SetTexture(index, texture);
        }
    }

    void MeshCompo::SetDiffuseTexture(Uint32 index, SPtr<Texture> texture) const
    {
        size_t size = m_model->Count();

        for (size_t i = 0; i < size; ++i)
        {
            m_model->GetMaterial(i).SetTexture(index, texture);
        }
    }

    void MeshCompo::SetSpecularMapTexture(SPtr<Texture> texture) const
    {
        size_t size = m_model->Count();

        for (size_t i = 0; i < size; ++i)
        {
            m_model->GetMaterial(i).SetTexture(4, texture);
        }
    }

    void MeshCompo::SetNormalMapTexture(SPtr<Texture> texture) const
    {
        size_t size = m_model->Count();

        for (size_t i = 0; i < size; ++i)
        {
            m_model->GetMaterial(i).SetTexture(5, texture);
        }
    }

    void MeshCompo::SetMappingTexture(SPtr<Texture> texture) const
    {
        size_t size = m_model->Count();

        for (size_t i = 0; i < size; ++i)
        {
            m_model->GetMaterial(i).SetTexture(6, texture);
        }
    }

    void MeshCompo::SetCubeMapTexture(SPtr<Texture> texture) const
    {
        size_t size = m_model->Count();

        if (texture->IsCubeMap())
        {
            SetMaterialInfo(0, 1);
            for (size_t i = 0; i < size; ++i)
            {
                m_model->GetMaterial(i).SetTexture(7, texture);
            }
        }
        else
        {
            SetMaterialInfo(0, 0);
            for (size_t i = 0; i < size; ++i)
            {
                m_model->GetMaterial(i).SetTexture(0, texture);
            }
        }
    }

    void MeshCompo::SetMeshData(const MeshData& mesh_data) const
    {
        m_model->SetMeshData(mesh_data);
    }

    void MeshCompo::SetShader(SPtr<ShaderProgram> shader)
    {
        m_b_deferred = shader->IsDeferred();
        m_model->SetShader(shader);
    }

    void MeshCompo::Render() const
    {
        m_model->Render(m_space->GetRenderSubsystem()->GetConstantBuffer(eConstantBufferType::Material));
    }

    Matrix44 MeshCompo::GetWorldMatrix() const
    {
        if (m_owner->HasComponent<TransformCompo>())
        {
            return m_owner->GetComponent<TransformCompo>()->GetLocalToWorldMatrix();
        }
        return Matrix44::Identity();
    }

    bool MeshCompo::IsDeferred() const
    {
        return m_b_deferred;
    }

    void MeshCompo::Subscribe()
    {
        m_space->GetRenderSubsystem()->AddMeshCompo(this);
    }

    void MeshCompo::Unsubscribe()
    {
        m_space->GetRenderSubsystem()->RemoveMeshCompo(this);
    }

    MeshFactory::MeshFactory()
    {
        m_type_name = typeid(MeshCompo).name();
        m_type_id   = typeid(MeshCompo).hash_code();
    }

    MeshFactory::~MeshFactory()
    {
    }

    RPtr<Component> MeshFactory::Create()
    {
        return new MeshCompo();
    }
}
