#pragma once

#include "../System.hpp"

namespace Engine
{
    class BehaviorSubsystem;

    class BehaviorSystem final : public System
    {
    public:
        BehaviorSystem();
        ~BehaviorSystem() override;

        void Initialize() override;
        void Update() override;
        void Shutdown() override;

        SPtr<BehaviorSubsystem> CreateSubsystem();
    private:
        std::vector<SPtr<BehaviorSubsystem>> m_subsystems;
    };
}
