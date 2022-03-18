#include "PhysicsSubsystem.hpp"

#include "../Core/ComponentManager/Components/RigidBodyCompo.hpp"
#include "../Core/ComponentManager/Components/SoftBodyCompo.hpp"
#include "../External/imgui/imgui.h"
#include "../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../Math/Structure/Transform.hpp"

namespace Engine
{
    PhysicsSubsystem::PhysicsSubsystem()
    {
    }

    PhysicsSubsystem::~PhysicsSubsystem()
    {
    }

    void PhysicsSubsystem::Initialize()
    {
    }

    void PhysicsSubsystem::Update(Real dt)
    {
        for (auto& rigid_body : m_rigid_body_compos)
        {
            rigid_body->Update(dt);
        }

        for (auto& soft_body_compo : m_soft_body_compos)
        {
            soft_body_compo->Update(dt);
        }
    }

    void PhysicsSubsystem::Render()
    {
        if (m_b_show_wire_frame)
        {
            for (auto& soft_body_compo : m_soft_body_compos)
            {
                soft_body_compo->m_body.Draw(m_primitive_renderer);
            }
        }
    }

    void PhysicsSubsystem::Shutdown()
    {
    }

    void PhysicsSubsystem::Edit()
    {
        if (ImGui::CollapsingHeader("Physics Subsystem"))
        {
            ImGui::Checkbox("Show Wire Frame", &m_b_show_wire_frame);
        }
    }

    void PhysicsSubsystem::AddCompo(RPtr<RigidBodyCompo> compo)
    {
        m_rigid_body_compos.push_back(compo);
    }

    void PhysicsSubsystem::RemoveCompo(RPtr<RigidBodyCompo> compo)
    {
        auto found = std::find(m_rigid_body_compos.begin(), m_rigid_body_compos.end(), compo);
        if (found != m_rigid_body_compos.end())
        {
            m_rigid_body_compos.erase(found);
        }
    }

    void PhysicsSubsystem::AddCompo(RPtr<SoftBodyCompo> compo)
    {
        m_soft_body_compos.push_back(compo);
    }

    void PhysicsSubsystem::RemoveCompo(RPtr<SoftBodyCompo> compo)
    {
        auto found = std::find(m_soft_body_compos.begin(), m_soft_body_compos.end(), compo);
        if (found != m_soft_body_compos.end())
        {
            m_soft_body_compos.erase(found);
        }
    }

    void PhysicsSubsystem::SetPrimitiveRenderer(SPtr<PrimitiveRenderer> renderer)
    {
        m_primitive_renderer = renderer;
    }
}
