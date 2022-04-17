/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "DefaultAppState.hpp"

namespace Engine
{
    DefaultAppState::DefaultAppState()
    {
        SetDefaultSubsystems();
    }

    DefaultAppState::~DefaultAppState()
    {
    }

    void DefaultAppState::Initialize()
    {
    }

    void DefaultAppState::Update(float dt)
    {
    }

    void DefaultAppState::FixedUpdate(float dt)
    {
    }

    void DefaultAppState::Render()
    {
    }

    void DefaultAppState::Shutdown()
    {
    }

    DefaultAppStateFactory::~DefaultAppStateFactory()
    {
    }

    AppState* DefaultAppStateFactory::Create()
    {
        return new DefaultAppState();
    }
}
