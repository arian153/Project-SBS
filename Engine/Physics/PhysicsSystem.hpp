#pragma once
#include "../System.hpp"

namespace Engine
{
    class PhysicsSystem final : public System
    {
    public:
        PhysicsSystem();
        ~PhysicsSystem() override;

        void Initialize() override;
        void Update() override;
        void Shutdown() override;
    private:
    };
}
