#pragma once
#include "MassData.hpp"
#include "../../Math/Structure/VecQuatScale.hpp"

namespace Engine
{
    class RigidBody
    {
    public:
        void IntegrateEuler(Real dt);

        void UpdateCentroid();
        void UpdatePosition();
        void UpdateInertia();

        void SetMassData(const MassData& mass_data);

        void ApplyForce(const Vector3& force, const Vector3& at);
        void ApplyForceCentroid(const Vector3& force);
        void ApplyTorque(const Vector3& torque);

        void SetPosition(const Vector3& position);
        void SetCentroid(const Vector3& centroid);
        void SetOrientation(const Quaternion& orientation);
        void SetTransform(const Vector3& position, const Quaternion& orientation);

        Vector3    GetPosition() const;
        Vector3    GetCentroid() const;
        Vector3    GetLocalCentroid() const;
        Quaternion GetOrientation() const;

        void SetLinearVelocity(const Vector3& linear);
        void SetAngularVelocity(const Vector3& angular);
        void AddLinearVelocity(const Vector3& linear_delta);
        void AddAngularVelocity(const Vector3& angular_delta);

        Vector3 GetLinearVelocity() const;
        Vector3 GetAngularVelocity() const;
        Vector3 GetForce() const;
        Vector3 GetTorque() const;

        void SetPositionalConstraints(const Vector3& linear);
        void SetRotationalConstraints(const Vector3& angular);

        void     SetMassInfinite();
        void     SetMass(Real mass = 1.0f);
        Real     Mass() const;
        Real     InverseMass() const;
        Matrix33 MassMatrix() const;
        Matrix33 InverseMassMatrix() const;

        void     SetInertiaInfinite();
        void     SetInertia(const Matrix33& inertia_tensor);
        Matrix33 Inertia() const;
        Matrix33 InverseInertia() const;

        Matrix33 LocalInertia() const;
        Matrix33 InverseLocalInertia() const;
        void     SetLocalInertia(const Matrix33& inertia);

        VecQuatScale& GetVQS();
        const VecQuatScale& GetVQS() const;

    private:
        VecQuatScale m_transform;

        //linear data
        Vector3 m_linear_velocity;
        Vector3 m_force_accumulator;
        Vector3 m_linear_constraints = Vector3(1.0f, 1.0f, 1.0f);

        //angular data
        Vector3 m_angular_velocity;
        Vector3 m_torque_accumulator;
        Vector3 m_angular_constraints = Vector3(1.0f, 1.0f, 1.0f);

        //mass data
        MassData m_mass_data;
        Vector3  m_world_centroid; //center of mass
        Matrix33 m_world_inverse_inertia;
    };
}