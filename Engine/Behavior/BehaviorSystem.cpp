/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

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
