#pragma once
#include <Engine.h>

namespace Client
{
    class TestSquare final : public Engine::AppState
    {
    public:
        TestSquare();
        ~TestSquare() override;

        void Initialize() override;
        void Update(float dt) override;
        void FixedUpdate(float dt) override;
        void Render() override;
        void Shutdown() override;

    private:

    };

    class TestSquareFactory final : public Engine::AppStateFactory
    {
    public:
        ~TestSquareFactory() override;
        Engine::AppState* Create() override;
    private:
    };
}
