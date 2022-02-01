#pragma once
#include "../System.hpp"

namespace Engine
{
    class RenderSubsystem final : public Subsystem
    {
    public:
        RenderSubsystem();
        ~RenderSubsystem() override;

        void Initialize() override;
        void Update(Real dt) override;
        void Render() override;
        void Shutdown() override;
    private:

    };
}
