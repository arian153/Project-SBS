#pragma once
#include <Engine.h>

namespace Client
{
    class TestBox final : public Engine::AppState
    {
    public:
        TestBox();
        ~TestBox() override;

        void Initialize() override;
        void Update(float dt) override;
        void FixedUpdate(float dt) override;
        void Render() override;
        void Shutdown() override;

    private:
        Engine::TruncatedCone m_tr_cone;
        Engine::Sphere m_sphere;

    };

    class TestBoxFactory final : public Engine::AppStateFactory
    {
    public:
        ~TestBoxFactory() override;
        Engine::AppState* Create() override;
    private:
    };
}
