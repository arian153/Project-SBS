#include "RigidBodyCompo.hpp"

#include "TransformCompo.hpp"
#include "../../CoreDefine.hpp"
#include "../../../External/imgui/imgui.h"
#include "../../../External/JSONCPP/json/json.h"
#include "../../AppStateManager/AppState.hpp"
#include "../../ObjectManager/Object.hpp"

namespace Engine
{
    RigidBodyCompo::RigidBodyCompo()
    {
    }

    RigidBodyCompo::~RigidBodyCompo()
    {
    }

    void RigidBodyCompo::Initialize()
    {
        Subscribe();
    }

    void RigidBodyCompo::Update(Real dt)
    {
        if (m_owner->HasComponent<TransformCompo>())
        {
            auto transform = m_owner->GetComponent<TransformCompo>();
            m_body.SetTransform(transform->GetPosition(), transform->GetOrientation());
            m_body.IntegrateEuler(dt);
            Matrix33 inertia;
            inertia.SetDiagonal(50.0f, 50.0f, 50.0f);
            m_body.SetInertia(inertia);
            transform->SetPosition(m_body.GetPosition());
            transform->SetOrientation(m_body.GetOrientation());
        }

       /* Vector3 force;
        Vector3 at;

        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::ArrowRight))
        {
            force.x += dt * 1000.0f;
            at.x += 10.0f;
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::ArrowLeft))
        {
            force.x -= dt * 1000.0f;
            at.x -= 10.0f;
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::ArrowUp))
        {
            force.z += dt * 1000.0f;
            at.z += 10.0f;
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::ArrowDown))
        {
            force.z -= dt * 1000.0f;
            at.z -= 10.0f;
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::B))
        {
            force.y += dt * 1000.0f;
            at.y += 10.0f;
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::V))
        {
            force.y -= dt * 1000.0f;
            at.y -= 10.0f;
        }

        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::Space))
        {
            m_body.SetLinearVelocity(Vector3());
            m_body.SetAngularVelocity(Vector3());
        }

        m_body.ApplyForce(force, at);*/
    }

    void RigidBodyCompo::Shutdown()
    {
        Unsubscribe();
    }

    bool RigidBodyCompo::Load(const Json::Value& data)
    {
        if (data.isNull())
            return false;

        return true;
    }

    void RigidBodyCompo::Save(Json::Value& data) const
    {
    }

    void RigidBodyCompo::Edit(CommandRegistry* command_registry)
    {
        ImGui::CollapsingHeader(MK_STRING(RigidBodyCompo));
    }

    void RigidBodyCompo::CloneTo(RPtr<Component> destination)
    {
    }

    void RigidBodyCompo::Subscribe()
    {
        m_space->GetPhysicsSubsystem()->AddCompo(this);
    }

    void RigidBodyCompo::Unsubscribe()
    {
        m_space->GetPhysicsSubsystem()->RemoveCompo(this);
    }

    RigidBodyFactory::RigidBodyFactory()
    {
        m_type_name = typeid(RigidBodyCompo).name();
        m_type_id   = typeid(RigidBodyCompo).hash_code();
    }

    RigidBodyFactory::~RigidBodyFactory()
    {
    }

    RPtr<Component> RigidBodyFactory::Create()
    {
        return new RigidBodyCompo();
    }
}
