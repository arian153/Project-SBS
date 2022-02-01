#pragma once
#include "AppState.hpp"
#include "AppStateFactory.hpp"

namespace Engine
{
    class DefaultAppState final : public AppState
    {
    public:
        DefaultAppState();
        ~DefaultAppState() override;

        void Initialize() override;
        void Update(float dt) override;
        void FixedUpdate(float dt) override;
        void Render() override;
        void Shutdown() override;

    private:
    };

    class DefaultAppStateFactory final : public AppStateFactory
    {
    public:
        ~DefaultAppStateFactory() override;
        AppState* Create() override;
    private:
    };
}
