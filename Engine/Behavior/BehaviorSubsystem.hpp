#pragma once
#include "../System.hpp"
#include "../Math/Structure/Transform.hpp"

namespace Engine
{
    class TransformCompo;

    class BehaviorSubsystem final : public Subsystem
    {
    public:
        BehaviorSubsystem();
        ~BehaviorSubsystem() override;

        void Initialize() override;
        void Update(Real dt) override;
        void Render() override;
        void Shutdown() override;

        void AddTransform(RPtr<TransformCompo> compo);
        void RemoveTransform(RPtr<TransformCompo> compo);

    private:
        std::vector<RPtr<TransformCompo>> m_transforms;
    };
}
