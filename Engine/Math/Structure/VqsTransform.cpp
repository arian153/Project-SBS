#include "VqsTransform.hpp"
#include <ostream>

#include "../Utility/Interpolation.hpp"
#include "../Utility/MatrixUtility.hpp"
#include "../Utility/Utility.hpp"

namespace Engine
{
    VqsTransform::VqsTransform()
    {
    }

    VqsTransform::VqsTransform(const VqsTransform& vqs)
        : position(vqs.position), scale(vqs.scale), rotation(vqs.rotation)
    {
    }

    VqsTransform::VqsTransform(const Vector3& pos, const Quaternion& rot, Real s)
        : position(pos), scale(s), rotation(rot)
    {
    }

    VqsTransform::~VqsTransform()
    {
    }

    void VqsTransform::Set(const Vector3& pos, const Quaternion& rot, Real s)
    {
        position = pos;
        rotation = rot;
        scale    = s;
    }

    Matrix44 VqsTransform::ToMatrix() const
    {
        Matrix44 result = Math::Matrix44::Scale(scale, scale, scale, 1.0f);
        result          = Math::Matrix44::Rotation(rotation) * result;
        result.AddVectorColumn(3, position);
        return result;
    }

    VqsTransform VqsTransform::Identity()
    {
        return VqsTransform(Vector3(0.0f, 0.0f, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), 1.0f);
    }

    VqsTransform VqsTransform::Inverse() const
    {
        Real       inv_scale = Math::IsZero(scale) ? 0.0f : 1.0f / scale;
        Quaternion inv_rot   = rotation.Inverse();
        return VqsTransform(inv_rot.Rotate(inv_scale * (-position)), inv_rot, inv_scale);
    }

    Vector3 VqsTransform::ApplyTransform(const Vector3& point) const
    {
        return rotation.Rotate(scale * point) + position;
    }

    Vector3 VqsTransform::ApplyInverse(const Vector3& point) const
    {
        Real       inv_scale = Math::IsZero(scale) ? 0.0f : 1.0f / scale;
        Quaternion inv_rot   = rotation.Inverse();
        Vector3    inv_pos   = inv_rot.Rotate(inv_scale * (-position));

        return inv_rot.Rotate(inv_scale * point) + inv_pos;
    }

    Vector3 VqsTransform::LocalToWorldPoint(const Vector3& local_point) const
    {
        return rotation.Rotate(scale * local_point) + position;
    }

    Vector3 VqsTransform::WorldToLocalPoint(const Vector3& world_point) const
    {
        Real inv_scale = Math::IsZero(scale) ? 0.0f : 1.0f / scale;
        return inv_scale * rotation.Inverse().Rotate(world_point - position);
    }

    Vector3 VqsTransform::LocalToWorldVector(const Vector3& local_vector) const
    {
        return rotation.Rotate(scale * local_vector);
    }

    Vector3 VqsTransform::WorldToLocalVector(const Vector3& world_vector) const
    {
        Real inv_scale = Math::IsZero(scale) ? 0.0f : 1.0f / scale;
        return inv_scale * rotation.Inverse().Rotate(world_vector);
    }

    VqsTransform VqsTransform::Concatenate(const VqsTransform& a, const VqsTransform& b)
    {
        return VqsTransform(a.rotation.Rotate(a.scale * b.position) + a.position, a.rotation * b.rotation, a.scale * b.scale);
    }

    VqsTransform VqsTransform::Interpolation(const VqsTransform& start, const VqsTransform& end, Real t)
    {
        return VqsTransform(
                            Lerp(start.position, end.position, t),
                            Slerp(start.rotation, end.rotation, t),
                            Elerp(start.scale, end.scale, t));
    }

    Matrix44 VqsTransform::Concatenate(const Matrix44& a, const Matrix44& b)
    {
        return a * b;
    }

    VqsTransform& VqsTransform::operator=(const VqsTransform& vqs)
    {
        if (this != &vqs)
        {
            position = vqs.position;
            scale    = vqs.scale;
            rotation = vqs.rotation;
        }
        return *this;
    }

    VqsTransform VqsTransform::operator+(const VqsTransform& vqs) const
    {
        return VqsTransform(position + vqs.position, rotation + vqs.rotation, scale + vqs.scale);
    }

    VqsTransform VqsTransform::operator*(Real real) const
    {
        return VqsTransform(position * real, rotation * real, scale * real);
    }

    VqsTransform VqsTransform::operator*(const VqsTransform& vqs) const
    {
        return VqsTransform(rotation.Rotate(scale * vqs.position) + position, rotation * vqs.rotation, scale * vqs.scale);
    }

    Vector3 VqsTransform::operator*(const Vector3& rhs) const
    {
        return rotation.Rotate(scale * rhs) + position;
    }

    std::ostream& operator<<(std::ostream& os, const VqsTransform& vqs)
    {
        os << "[" << vqs.position << ", " << vqs.rotation << ", " << vqs.scale << "]";
        return os;
    }

    VqsTransform operator*(Real real, const VqsTransform& vqs)
    {
        return VqsTransform(vqs.position * real, vqs.rotation * real, vqs.scale * real);
    }
}
