#pragma once
#include <Engine.h>

namespace Client
{
    class TestSphere final : public Engine::AppState
    {
    public:
        TestSphere();
        ~TestSphere() override;

        void Initialize() override;
        void Update(float dt) override;
        void FixedUpdate(float dt) override;
        void Render() override;
        void Shutdown() override;

    private:
    };

    class TestSphereFactory final : public Engine::AppStateFactory
    {
    public:
        ~TestSphereFactory() override;
        Engine::AppState* Create() override;
    private:
    };
}
