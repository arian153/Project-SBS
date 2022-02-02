#pragma once

#include "../System.hpp"

namespace Engine
{
    class AnimationSystem final : public System
    {
    public:
        AnimationSystem();
        ~AnimationSystem() override;

        void Initialize() override;
        void Update() override;
        void Shutdown() override;
    private:
    };
}
