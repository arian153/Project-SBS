#pragma once
#include "../System.hpp"

namespace Engine
{
    class PhysicsSubsystem;

    class PhysicsSystem final : public System
    {
    public:
        PhysicsSystem();
        ~PhysicsSystem() override;

        void Initialize() override;
        void Update() override;
        void Shutdown() override;

        SPtr<PhysicsSubsystem> CreateSubsystem();

    private:
        std::vector<SPtr<PhysicsSubsystem>> m_subsystems;
    };
}
