#pragma once
#include "../System.hpp"
#include "Dynamics/RigidBody.hpp"

namespace Engine
{
    class PrimitiveRenderer;
    class SoftBodyCompo;
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

        void AddCompo(RPtr<SoftBodyCompo> compo);
        void RemoveCompo(RPtr<SoftBodyCompo> compo);

        void SetPrimitiveRenderer(SPtr<PrimitiveRenderer> renderer);

    private:
        std::vector<RPtr<RigidBodyCompo>> m_rigid_body_compos;
        std::vector<RPtr<SoftBodyCompo>>  m_soft_body_compos;
        std::vector<RigidBody>            m_rigid_bodies;

        bool m_b_show_wire_frame = true;

        SPtr<PrimitiveRenderer> m_primitive_renderer = nullptr;
    };
}
