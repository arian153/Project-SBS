/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

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

            switch (m_integration_type)
            {
            case 0:
                m_body.IntegrateEuler(dt);
                break;
            case 1:
                m_body.IntegrateVerlet(dt);
                break;

            default:
                m_body.IntegrateEuler(dt);
                break;
            }
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

            ImGui::Text("Custom Force Scale - Use Arrow Key");
            ImGui::SliderFloat("##Scale", &m_scale, 1.f, 50.0f);

            ImGui::Text("Integration Method");

            const char* light_type[] = { "Euler Method", "Verlet Method" };

            if (ImGui::Combo("##Integration Type", &m_integration_type, light_type, 2))
            {
            }
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

    void SoftBodyCompo::ApplyForce(const Vector3& force)
    {
        m_body.ApplyForceCentroid(force);
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
