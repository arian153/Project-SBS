#pragma once
#include <Engine.h>

namespace Client
{
    class TestSkeletal final : public Engine::AppState
    {
    public:
        TestSkeletal();
        ~TestSkeletal() override;

        void Initialize() override;
        void Update(float dt) override;
        void FixedUpdate(float dt) override;
        void Render() override;
        void Shutdown() override;

    private:

    };

    class TestSkeletalFactory final : public Engine::AppStateFactory
    {
    public:
        ~TestSkeletalFactory() override;
        Engine::AppState* Create() override;
    private:
    };
}