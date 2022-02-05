#include "BehaviorSystem.hpp"

#include "BehaviorSubsystem.hpp"

namespace Engine
{
    BehaviorSystem::BehaviorSystem()
    {
    }

    BehaviorSystem::~BehaviorSystem()
    {
    }

    void BehaviorSystem::Initialize()
    {
    }

    void BehaviorSystem::Update()
    {
    }

    void BehaviorSystem::Shutdown()
    {
    }

    SPtr<BehaviorSubsystem> BehaviorSystem::CreateSubsystem()
    {
        auto subsystem = std::make_shared<BehaviorSubsystem>();
        m_subsystems.push_back(subsystem);
        return subsystem;
    }
}
