#pragma once
#include "../System.hpp"

namespace Engine
{
    class PhysicsSubsystem final : public Subsystem
    {
    public:
        PhysicsSubsystem();
        ~PhysicsSubsystem() override;

        void Initialize() override;
        void Update(Real dt) override;
        void Render() override;
        void Shutdown() override;
    private:
        
    };
}
