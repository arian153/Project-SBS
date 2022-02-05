#include "AnimationSystem.hpp"

#include "AnimationSubsystem.hpp"

namespace Engine
{
    AnimationSystem::AnimationSystem()
    {
    }

    AnimationSystem::~AnimationSystem()
    {
    }

    void AnimationSystem::Initialize()
    {
    }

    void AnimationSystem::Update()
    {
    }

    void AnimationSystem::Shutdown()
    {
        m_subsystems.clear();
    }

    SPtr<AnimationSubsystem> AnimationSystem::CreateSubsystem()
    {
        auto subsystem = std::make_shared<AnimationSubsystem>();
        m_subsystems.push_back(subsystem);
        return subsystem;
    }
}
