#pragma once
#include "../System.hpp"
#include "Structure/Transform.hpp"

namespace Engine
{
    class SpaceSubsystem final : public Subsystem
    {
    public:
        SpaceSubsystem();
        ~SpaceSubsystem() override;

        void Initialize() override;
        void Update(Real dt) override;
        void Render() override;
        void Shutdown() override;

    private:
        std::vector<Transform> m_transforms;

    };
}
