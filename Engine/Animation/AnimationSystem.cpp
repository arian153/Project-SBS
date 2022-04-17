/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

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
