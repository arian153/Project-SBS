#include "RigidBody.hpp"

namespace Engine
{
    void RigidBody::IntegrateEuler(Real dt)
    {
        // integrate linear velocity
        m_linear_velocity += m_mass_data.inverse_mass * m_force_accumulator * dt;
        // integrate angular velocity
        m_angular_velocity += m_world_inverse_inertia * m_torque_accumulator * dt;
        // zero out accumulated force and torque
        m_force_accumulator.SetZero();
        m_torque_accumulator.SetZero();

        Vector3 delta_linear_velocity = m_linear_velocity * dt;
        delta_linear_velocity         = delta_linear_velocity.HadamardProduct(m_linear_constraints);
        m_world_centroid += delta_linear_velocity;
        // integrate orientation
        Vector3 delta_angular_velocity = m_angular_velocity;
        delta_angular_velocity         = delta_angular_velocity.HadamardProduct(m_angular_constraints);
        Vector3 axis                   = delta_angular_velocity.Unit();
        Real    radian                 = delta_angular_velocity.Length() * dt;
        m_transform.rotation.AddRotation(axis, radian);
        // update remain properties
        m_transform.rotation.SetNormalize();
        UpdateInertia();
        UpdatePosition();
    }

    void RigidBody::UpdateCentroid()
    {
        m_world_centroid = m_transform.rotation.Rotate(m_mass_data.local_centroid) + m_transform.position;
    }

    void RigidBody::UpdatePosition()
    {
        m_transform.position = m_transform.rotation.Rotate(-m_mass_data.local_centroid) + m_world_centroid;
    }

    void RigidBody::UpdateInertia()
    {
        m_world_inverse_inertia = m_transform.rotation * m_mass_data.local_inverse_inertia * m_transform.rotation.Inverse().Unit();
    }

    void RigidBody::SetMassData(const MassData& mass_data)
    {
        m_mass_data = mass_data;
        /* if (m_motion_mode != eMotionMode::Dynamic)
         {
             SetMassInfinite();
             SetInertiaInfinite();
         }*/
    }

    void RigidBody::ApplyForce(const Vector3& force, const Vector3& at)
    {
        //SetAwake();
        m_force_accumulator += force;
        m_torque_accumulator += (at - m_world_centroid).CrossProduct(force);
    }

    void RigidBody::ApplyForceCentroid(const Vector3& force)
    {
        //SetAwake();
        m_force_accumulator += force;
    }

    void RigidBody::ApplyTorque(const Vector3& torque)
    {
        //SetAwake();
        m_torque_accumulator += torque;
    }

    void RigidBody::SetPosition(const Vector3& position)
    {
        m_transform.position = position;
        UpdateCentroid();
    }

    void RigidBody::SetCentroid(const Vector3& centroid)
    {
        m_world_centroid = centroid;
        UpdatePosition();
    }

    void RigidBody::SetOrientation(const Quaternion& orientation)
    {
        m_transform.rotation = orientation.Unit();
        UpdateCentroid();
        UpdateInertia();
    }

    void RigidBody::SetTransform(const Vector3& position, const Quaternion& orientation)
    {
        m_transform.position = position;
        m_transform.rotation = orientation.Unit();
        UpdateCentroid();
        UpdateInertia();
    }

    Vector3 RigidBody::GetPosition() const
    {
        return m_transform.position;
    }

    Vector3 RigidBody::GetCentroid() const
    {
        return m_world_centroid;
    }

    Vector3 RigidBody::GetLocalCentroid() const
    {
        return m_mass_data.local_centroid;
    }

    Quaternion RigidBody::GetOrientation() const
    {
        return m_transform.rotation;
    }

    void RigidBody::SetLinearVelocity(const Vector3& linear)
    {
        m_linear_velocity = linear;
    }

    void RigidBody::SetAngularVelocity(const Vector3& angular)
    {
        m_angular_velocity = angular;
    }

    void RigidBody::AddLinearVelocity(const Vector3& linear_delta)
    {
        m_linear_velocity += linear_delta;
    }

    void RigidBody::AddAngularVelocity(const Vector3& angular_delta)
    {
        m_angular_velocity += angular_delta;
    }

    Vector3 RigidBody::GetLinearVelocity() const
    {
        return m_linear_velocity;
    }

    Vector3 RigidBody::GetAngularVelocity() const
    {
        return m_angular_velocity;
    }

    Vector3 RigidBody::GetForce() const
    {
        return m_force_accumulator;
    }

    Vector3 RigidBody::GetTorque() const
    {
        return m_torque_accumulator;
    }

    void RigidBody::SetPositionalConstraints(const Vector3& linear)
    {
        m_linear_constraints = linear;
    }

    void RigidBody::SetRotationalConstraints(const Vector3& angular)
    {
        m_angular_constraints = angular;
    }

    void RigidBody::SetMassInfinite()
    {
        m_mass_data.mass         = 0.0f;
        m_mass_data.inverse_mass = 0.0f;
    }

    void RigidBody::SetMass(Real mass)
    {
        m_mass_data.mass         = mass;
        m_mass_data.inverse_mass = 1.0f / mass;
    }

    Real RigidBody::Mass() const
    {
        return m_mass_data.mass;
    }

    Real RigidBody::InverseMass() const
    {
        return m_mass_data.inverse_mass;
    }

    Matrix33 RigidBody::MassMatrix() const
    {
        Real     mass = m_mass_data.mass;
        Matrix33 mass_matrix;
        mass_matrix.SetDiagonal(mass, mass, mass);
        return mass_matrix;
    }

    Matrix33 RigidBody::InverseMassMatrix() const
    {
        Real     inv_mass = m_mass_data.inverse_mass;
        Matrix33 inverse_mass_matrix;
        inverse_mass_matrix.SetDiagonal(inv_mass, inv_mass, inv_mass);
        return inverse_mass_matrix;
    }

    void RigidBody::SetInertiaInfinite()
    {
        m_world_inverse_inertia.SetZero();
        m_mass_data.local_inertia.SetZero();
        m_mass_data.local_inverse_inertia.SetZero();
    }

    void RigidBody::SetInertia(const Matrix33& inertia_tensor)
    {
        m_world_inverse_inertia          = inertia_tensor.Inverse();
        m_mass_data.local_inverse_inertia = m_transform.rotation.Inverse().Unit() * m_world_inverse_inertia * m_transform.rotation;
        m_mass_data.local_inertia         = m_mass_data.local_inverse_inertia.Inverse();
    }

    Matrix33 RigidBody::Inertia() const
    {
        return m_transform.rotation * m_mass_data.local_inertia * m_transform.rotation.Inverse().Unit();
    }

    Matrix33 RigidBody::InverseInertia() const
    {
        return m_world_inverse_inertia;
    }

    Matrix33 RigidBody::LocalInertia() const
    {
        return m_mass_data.local_inertia;
    }

    Matrix33 RigidBody::InverseLocalInertia() const
    {
        return m_mass_data.local_inverse_inertia;
    }

    void RigidBody::SetLocalInertia(const Matrix33& inertia)
    {
        m_mass_data.local_inertia         = inertia;
        m_mass_data.local_inverse_inertia = inertia.Inverse();
        m_world_inverse_inertia          = m_transform.rotation * m_mass_data.local_inverse_inertia * m_transform.rotation.Inverse().Unit();
    }

    VecQuatScale& RigidBody::GetVQS()
    {
        return m_transform;
    }

    const VecQuatScale& RigidBody::GetVQS() const
    {
        return m_transform;
    }
}
