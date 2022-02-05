#pragma once

#include "../System.hpp"

namespace Engine
{
    class AnimationSubsystem;

    class AnimationSystem final : public System
    {
    public:
        AnimationSystem();
        ~AnimationSystem() override;

        void Initialize() override;
        void Update() override;
        void Shutdown() override;

        SPtr<AnimationSubsystem> CreateSubsystem();

    private:
        std::vector<SPtr<AnimationSubsystem>> m_subsystems;
    };
}
