#include "VecQuatScale.hpp"
#include <ostream>

#include "../Utility/Interpolation.hpp"
#include "../Utility/MatrixUtility.hpp"
#include "../Utility/Utility.hpp"

namespace Engine
{
    VecQuatScale::VecQuatScale()
    {
    }

    VecQuatScale::VecQuatScale(const VecQuatScale& vqs)
        : position(vqs.position), scale(vqs.scale), rotation(vqs.rotation)
    {
    }

    VecQuatScale::VecQuatScale(const Vector3& pos, const Quaternion& rot, Real s)
        : position(pos), scale(s), rotation(rot)
    {
    }

    VecQuatScale::~VecQuatScale()
    {
    }

    void VecQuatScale::Set(const Vector3& pos, const Quaternion& rot, Real s)
    {
        position = pos;
        rotation = rot;
        scale    = s;
    }

    Matrix44 VecQuatScale::ToMatrix() const
    {
        Matrix44 result = Math::Matrix44::Scale(scale, scale, scale, 1.0f);
        result          = Math::Matrix44::Rotation(rotation) * result;
        result.AddVectorColumn(3, position);
        return result;
    }

    VecQuatScale VecQuatScale::Identity()
    {
        return VecQuatScale(Vector3(0.0f, 0.0f, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), 1.0f);
    }

    VecQuatScale VecQuatScale::Inverse() const
    {
        Real       inv_scale = Math::IsZero(scale) ? 0.0f : 1.0f / scale;
        Quaternion inv_rot   = rotation.Inverse();
        return VecQuatScale(inv_rot.Rotate(inv_scale * (-position)), inv_rot, inv_scale);
    }

    Vector3 VecQuatScale::ApplyTransform(const Vector3& point) const
    {
        return rotation.Rotate(scale * point) + position;
    }

    Vector3 VecQuatScale::ApplyInverse(const Vector3& point) const
    {
        Real       inv_scale = Math::IsZero(scale) ? 0.0f : 1.0f / scale;
        Quaternion inv_rot   = rotation.Inverse();
        Vector3    inv_pos   = inv_rot.Rotate(inv_scale * (-position));

        return inv_rot.Rotate(inv_scale * point) + inv_pos;
    }

    Vector3 VecQuatScale::LocalToWorldPoint(const Vector3& local_point) const
    {
        return rotation.Rotate(scale * local_point) + position;
    }

    Vector3 VecQuatScale::WorldToLocalPoint(const Vector3& world_point) const
    {
        Real inv_scale = Math::IsZero(scale) ? 0.0f : 1.0f / scale;
        return inv_scale * rotation.Inverse().Rotate(world_point - position);
    }

    Vector3 VecQuatScale::LocalToWorldVector(const Vector3& local_vector) const
    {
        return rotation.Rotate(scale * local_vector);
    }

    Vector3 VecQuatScale::WorldToLocalVector(const Vector3& world_vector) const
    {
        Real inv_scale = Math::IsZero(scale) ? 0.0f : 1.0f / scale;
        return inv_scale * rotation.Inverse().Rotate(world_vector);
    }

    VecQuatScale VecQuatScale::Concatenate(const VecQuatScale& a, const VecQuatScale& b)
    {
        return VecQuatScale(a.rotation.Rotate(a.scale * b.position) + a.position, a.rotation * b.rotation, a.scale * b.scale);
    }

    VecQuatScale VecQuatScale::Interpolation(const VecQuatScale& start, const VecQuatScale& end, Real t)
    {
        return VecQuatScale(
                            Lerp(start.position, end.position, t),
                            Slerp(start.rotation, end.rotation, t),
                            Elerp(start.scale, end.scale, t));
    }

    Matrix44 VecQuatScale::Concatenate(const Matrix44& a, const Matrix44& b)
    {
        return a * b;
    }

    VecQuatScale& VecQuatScale::operator=(const VecQuatScale& vqs)
    {
        if (this != &vqs)
        {
            position = vqs.position;
            scale    = vqs.scale;
            rotation = vqs.rotation;
        }
        return *this;
    }

    VecQuatScale VecQuatScale::operator+(const VecQuatScale& vqs) const
    {
        return VecQuatScale(position + vqs.position, rotation + vqs.rotation, scale + vqs.scale);
    }

    VecQuatScale VecQuatScale::operator*(Real real) const
    {
        return VecQuatScale(position * real, rotation * real, scale * real);
    }

    VecQuatScale VecQuatScale::operator*(const VecQuatScale& vqs) const
    {
        return VecQuatScale(rotation.Rotate(scale * vqs.position) + position, rotation * vqs.rotation, scale * vqs.scale);
    }

    Vector3 VecQuatScale::operator*(const Vector3& rhs) const
    {
        return rotation.Rotate(scale * rhs) + position;
    }

    std::ostream& operator<<(std::ostream& os, const VecQuatScale& vqs)
    {
        os << "[" << vqs.position << ", " << vqs.rotation << ", " << vqs.scale << "]";
        return os;
    }

    VecQuatScale operator*(Real real, const VecQuatScale& vqs)
    {
        return VecQuatScale(vqs.position * real, vqs.rotation * real, vqs.scale * real);
    }
}
