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
