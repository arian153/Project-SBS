#pragma once
#include <Engine.h>

namespace Client
{
    class Splash final : public Engine::AppState
    {
    public:
        Splash();
        ~Splash() override;

        void Initialize() override;
        void Update(float dt) override;
        void FixedUpdate(float dt) override;
        void Render() override;
        void Shutdown() override;

    private:
        bool m_show_about   = true;
        bool m_show_license = true;
        bool m_show_credit  = true;
    };

    class SplashFactory final : public Engine::AppStateFactory
    {
    public:
        ~SplashFactory() override;
        Engine::AppState* Create() override;
    private:
    };
}
