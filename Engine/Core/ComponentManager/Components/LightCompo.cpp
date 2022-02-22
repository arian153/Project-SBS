#include "LightCompo.hpp"

#include "TransformCompo.hpp"
#include "../../../External/imgui/imgui.h"
#include "../../../External/JSONCPP/json/json.h"
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
    }

    void LightCompo::Update(Real dt)
    {
        if (m_owner->HasComponent<TransformCompo>())
            m_light_info.position = m_owner->GetComponent<TransformCompo>()->GetPosition();
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
    }

    void LightCompo::SetLightRange(float range)
    {
        m_light_info.range = range;
    }

    void LightCompo::SetLightAngle(float angle)
    {
        m_light_info.angle = angle;
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
        m_type_id = typeid(LightCompo).hash_code();
    }

    LightFactory::~LightFactory()
    {
    }

    RPtr<Component> LightFactory::Create()
    {
        return new LightCompo();
    }
}
