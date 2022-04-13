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
            m_body.SetTransform(transform->GetTransform());
            m_body.UpdateWorld();
            m_body.SolveSpringDamper();
            m_body.Integrate(dt);
        }

        Vector3 force;
        Vector3 at;

        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::ArrowRight))
        {
            force.x += m_scale;
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::ArrowLeft))
        {
            force.x -= m_scale;
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::ArrowUp))
        {
            force.z += m_scale;
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::ArrowDown))
        {
            force.z -= m_scale;
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::B))
        {
            force.y += m_scale;
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::V))
        {
            force.y -= m_scale;
        }

        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::Space))
        {
            m_body.Clear();
        }

        m_body.ApplyForce(force, at);
    }

    void SoftBodyCompo::Shutdown()
    {
        m_body.ShutdownBVH();
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
            ImGui::SliderFloat("##Spring", &m_body.m_spring_constant, 0.01f, 20.0f);

            ImGui::Text("Damper Constant");
            ImGui::SliderFloat("##Damper", &m_body.m_damper_constant, 0.01f, 5.0f);

            ImGui::Text("Force Scale - Temp");
            ImGui::SliderFloat("##Scale", &m_scale, 1.f, 50.0f);
        }
    }

    void SoftBodyCompo::CloneTo(RPtr<Component> destination)
    {
    }

    void SoftBodyCompo::CreateSampleCloth(size_t w, size_t h, bool is_fixed)
    {
        m_body.CreateSampleCloth(w, h, is_fixed);
    }

    void SoftBodyCompo::CreateSampleSphere(bool is_fixed)
    {
        m_body.CreateSampleSphere(is_fixed);
    }

    void SoftBodyCompo::CreateSampleBox()
    {
        m_body.CreateSampleBox();
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
