#pragma once
#include "../System.hpp"
#include "Dynamics/RigidBody.hpp"

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

        void Edit();

        void AddCompo(RPtr<RigidBodyCompo> compo);
        void RemoveCompo(RPtr<RigidBodyCompo> compo);



    private:
        std::vector<RPtr<RigidBodyCompo>> m_rigid_body_compos;
        std::vector<RigidBody>            m_rigid_bodies;



    };
}
