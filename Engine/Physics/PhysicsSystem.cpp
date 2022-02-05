#include "PhysicsSystem.hpp"

#include "PhysicsSubsystem.hpp"

namespace Engine
{
    PhysicsSystem::PhysicsSystem()
    {
    }

    PhysicsSystem::~PhysicsSystem()
    {
    }

    void PhysicsSystem::Initialize()
    {
    }

    void PhysicsSystem::Update()
    {
    }

    void PhysicsSystem::Shutdown()
    {
    }

    SPtr<PhysicsSubsystem> PhysicsSystem::CreateSubsystem()
    {
        auto subsystem = std::make_shared<PhysicsSubsystem>();
        m_subsystems.push_back(subsystem);
        return subsystem;
    }
}
