#pragma once
#include "../System.hpp"

namespace Engine
{
    class RigidBodyCompo;

    class PhysicsSubsystem final : public Subsystem
    {
    public:
        PhysicsSubsystem();
        ~PhysicsSubsystem() override;

        void Initialize() override;
        void Update(Real dt) override;
        void Render() override;
        void Shutdown() override;

        void AddCompo(RPtr<RigidBodyCompo> compo);
        void RemoveCompo(RPtr<RigidBodyCompo> compo);

    private:
        std::vector<RPtr<RigidBodyCompo>> m_rigid_body_compos;
    };
}
