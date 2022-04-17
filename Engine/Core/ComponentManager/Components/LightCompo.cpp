/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "LightCompo.hpp"

#include "TransformCompo.hpp"
#include "../../../External/imgui/imgui.h"
#include "../../../External/JSONCPP/json/json.h"
#include "../../../Graphics/GraphicsDefine.hpp"
#include "../../../Graphics/Element/Model.hpp"
#include "../../../Graphics/Utility/MeshDataGenerator.hpp"
#include "../../AppStateManager/AppState.hpp"
#include "../../ObjectManager/Object.hpp"

namespace Engine
{
    LightCompo::LightCompo()
    {
    }

    LightCompo::~LightCompo()
    {
    }

    void LightCompo::Initialize()
    {
        Subscribe();
        m_model = std::make_shared<Model>();
    }

    void LightCompo::Update(Real dt)
    {
        if (m_owner->HasComponent<TransformCompo>())
        {
            m_light_info.position = m_owner->GetComponent<TransformCompo>()->GetPosition();
            m_transform           = m_owner->GetComponent<TransformCompo>()->GetTransform();
            float scale           = 2.0f * m_light_info.range;

            switch (static_cast<eLightType>(m_light_info.light_type))
            {
            case eLightType::PointLight:
            case eLightType::SpotLight:
                m_transform.scale = Vector3(scale, scale, scale);
                break;
            default: ;
            }
        }
    }

    void LightCompo::Shutdown()
    {
        Unsubscribe();
    }

    bool LightCompo::Load(const Json::Value& data)
    {
        if (data.isNull())
            return false;

        return true;
    }

    void LightCompo::Save(Json::Value& data) const
    {
    }

    void LightCompo::Edit(CommandRegistry* command_registry)
    {
        ImGui::CollapsingHeader(MK_STRING(LightCompo));
    }

    void LightCompo::CloneTo(RPtr<Component> destination)
    {
    }

    const LightInfo& LightCompo::GetLightInfo() const
    {
        return m_light_info;
    }

    LightInfo& LightCompo::GetLightInfo()
    {
        return m_light_info;
    }

    void LightCompo::SetLightDirection(const Vector3& direction)
    {
        m_light_info.direction = direction;
    }

    void LightCompo::SetDiffuse(const Color& diffuse)
    {
        m_light_info.color.diffuse = diffuse;
    }

    void LightCompo::SetAmbient(const Color& ambient)
    {
        m_light_info.color.ambient = ambient;
    }

    void LightCompo::SetSpecular(const Color& specular)
    {
        m_light_info.color.specular = specular;
    }

    void LightCompo::SetLightType(eLightType type)
    {
        m_light_info.light_type = static_cast<Uint32>(type);

        switch (type)
        {
        case eLightType::DirectionalLight:
            m_model->SetMeshData(MeshDataGenerator::CreateRectangle(1.0f, 1.0f));
            m_model->SetMaterial(MATERIAL_MANAGER->GetMaterial("DirLight"));
            break;
        case eLightType::PointLight:
            m_model->SetMeshData(MeshDataGenerator::CreateSphere(0.5f, 20, 20));
            m_model->SetMaterial(MATERIAL_MANAGER->GetMaterial("PointLight"));
            break;
        case eLightType::SpotLight:
            m_model->SetMeshData(MeshDataGenerator::CreateSphere(0.5f, 20, 20));
            m_model->SetMaterial(MATERIAL_MANAGER->GetMaterial("PointLight"));
            break;
        default: ;
        }
    }

    void LightCompo::SetLightRange(float range)
    {
        m_light_info.range = range;
    }

    void LightCompo::SetLightAngle(float angle)
    {
        m_light_info.angle = angle;
    }

    void LightCompo::DeferredBind(Sint32 light_index) const
    {
        size_t size = m_model->Count();
        for (size_t i = 0; i < size; ++i)
        {
            m_model->GetMaterial(i).SetSint(0, light_index);
        }

        m_model->Render(m_space->GetRenderSubsystem()->GetConstantBuffer(eConstantBufferType::Material));
    }

    Matrix44 LightCompo::GetWorldMatrix() const
    {
        return m_transform.LocalToWorldMatrix();
    }

    void LightCompo::Subscribe()
    {
        m_space->GetRenderSubsystem()->AddLightCompo(this);
    }

    void LightCompo::Unsubscribe()
    {
        m_space->GetRenderSubsystem()->RemoveLightCompo(this);
    }

    LightFactory::LightFactory()
    {
        m_type_name = typeid(LightCompo).name();
        m_type_id   = typeid(LightCompo).hash_code();
    }

    LightFactory::~LightFactory()
    {
    }

    RPtr<Component> LightFactory::Create()
    {
        return new LightCompo();
    }
}
