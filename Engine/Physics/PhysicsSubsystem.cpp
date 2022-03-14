#include "PhysicsSubsystem.hpp"

#include "../Core/ComponentManager/Components/RigidBodyCompo.hpp"

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
    }

    void PhysicsSubsystem::Render()
    {
    }

    void PhysicsSubsystem::Shutdown()
    {
    }

    void PhysicsSubsystem::Edit()
    {
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
}
