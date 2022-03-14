#pragma once

#include "../System.hpp"

namespace Engine
{
    class AnimationSubsystem final : public Subsystem
    {
    public:
        AnimationSubsystem();
        ~AnimationSubsystem() override;

        void Initialize() override;
        void Update(Real dt) override;
        void Render() override;
        void Shutdown() override;

        void Edit();
    private:
    };
}
