#include "SoftBodyCompo.hpp"

#include "TransformCompo.hpp"
#include "../../CoreDefine.hpp"
#include "../../../External/imgui/imgui.h"
#include "../../../External/JSONCPP/json/json.h"
#include "../../AppStateManager/AppState.hpp"
#include "../../ObjectManager/Object.hpp"

namespace Engine
{
    SoftBodyCompo::SoftBodyCompo()
    {
    }

    SoftBodyCompo::~SoftBodyCompo()
    {
    }

    void SoftBodyCompo::Initialize()
    {
        Subscribe();
    }

    void SoftBodyCompo::Update(Real dt)
    {
        if (m_owner->HasComponent<TransformCompo>())
        {
            auto transform = m_owner->GetComponent<TransformCompo>();
            m_body.SolveSpringDamper();
            m_body.Integrate(dt);
            //transform->SetPosition(m_body.m_transform.position);
        }

        Vector3 force;
        Vector3 at;

        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::ArrowRight))
        {
            force.x += dt * 100.0f;
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::ArrowLeft))
        {
            force.x -= dt * 100.0f;
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::ArrowUp))
        {
            force.z += dt * 100.0f;
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::ArrowDown))
        {
            force.z -= dt * 100.0f;
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::B))
        {
            force.y += dt * 100.0f;
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::V))
        {
            force.y -= dt * 100.0f;
        }

        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::Space))
        {
            m_body.Clear();
        }

        m_body.ApplyForce(force, at);
    }

    void SoftBodyCompo::Shutdown()
    {
        Unsubscribe();
    }

    bool SoftBodyCompo::Load(const Json::Value& data)
    {
        if (data.isNull())
            return false;

        return true;
    }

    void SoftBodyCompo::Save(Json::Value& data) const
    {
    }

    void SoftBodyCompo::Edit(CommandRegistry* command_registry)
    {
        if (ImGui::CollapsingHeader(MK_STRING(SoftBodyCompo)))
        {
            ImGui::Text("Spring Constant");
            ImGui::SliderFloat("##Spring", &m_body.m_spring_constant, 0.01f, 10.0f);

            ImGui::Text("Damper Constant");
            ImGui::SliderFloat("##Damper", &m_body.m_damper_constant, 0.01f, 5.0f);
        }
    }

    void SoftBodyCompo::CloneTo(RPtr<Component> destination)
    {
    }

    void SoftBodyCompo::CreateSample(size_t w, size_t h)
    {
        m_body.CreateSampleCloth(w, h, true);
    }

    const MeshData& SoftBodyCompo::GetSoftBodyMeshData() const
    {
        return m_body.m_mesh_data;
    }

    MeshData& SoftBodyCompo::GetSoftBodyMeshData()
    {
        return m_body.m_mesh_data;
    }

    void SoftBodyCompo::Subscribe()
    {
        m_space->GetPhysicsSubsystem()->AddCompo(this);
    }

    void SoftBodyCompo::Unsubscribe()
    {
        m_space->GetPhysicsSubsystem()->RemoveCompo(this);
    }

    SoftBodyFactory::SoftBodyFactory()
    {
        m_type_name = typeid(SoftBodyCompo).name();
        m_type_id   = typeid(SoftBodyCompo).hash_code();
    }

    SoftBodyFactory::~SoftBodyFactory()
    {
    }

    RPtr<Component> SoftBodyFactory::Create()
    {
        return new SoftBodyCompo();
    }
}
