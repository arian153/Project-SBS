#include "Transform.hpp"

#include "../Algebra/Matrix33.hpp"
#include "../Utility/MatrixUtility.hpp"
#include "../Utility/Utility.hpp"

namespace Engine
{
    Transform::Transform()
    {
    }

    Transform::~Transform()
    {
    }

    Transform& Transform::operator=(const Transform& rhs)
    {
        if (this != &rhs)
        {
            position    = rhs.position;
            scale       = rhs.scale;
            orientation = rhs.orientation;
        }
        return *this;
    }

    Matrix44 Transform::LocalToWorldMatrix() const
    {
        Matrix44 result = Math::Matrix44::Scale(scale, 1.0f);
        result          = Math::Matrix44::Rotation(orientation) * result;
        result.AddVectorColumn(3, position);
        result.SetTranspose();
        return result;
    }

    void Transform::SetAffineMatrix(const Matrix44& affine)
    {
        position = affine.GetColumn(3);

        scale = Vector3(
                        Vector3(affine.GetColumn(0)).Length(),
                        Vector3(affine.GetColumn(1)).Length(),
                        Vector3(affine.GetColumn(2)).Length()
                       );

        Matrix33 rotation_mat = affine.GetRotationMatrix();
        rotation_mat.SetColumns(
                                Math::IsZero(scale.x) ? Vector3() : (rotation_mat.GetColumn(0) / scale.x),
                                Math::IsZero(scale.y) ? Vector3() : rotation_mat.GetColumn(1) / scale.y,
                                Math::IsZero(scale.z) ? Vector3() : rotation_mat.GetColumn(2) / scale.z);

        orientation = rotation_mat.ToQuaternion();
    }

    Vector3 Transform::LocalToWorldPoint(const Vector3& local_point) const
    {
        return orientation.Rotate(HadamardProduct(local_point, scale)) + position;
    }

    Vector3 Transform::WorldToLocalPoint(const Vector3& world_point) const
    {
        return HadamardProduct(orientation.Inverse().Rotate(world_point - position), scale.Inverse());
    }

    Vector3 Transform::LocalToWorldVector(const Vector3& local_vector) const
    {
        return orientation.Rotate(HadamardProduct(local_vector, scale));
    }

    Vector3 Transform::WorldToLocalVector(const Vector3& world_vector) const
    {
        return HadamardProduct(orientation.Inverse().Rotate(world_vector), scale.Inverse());
    }
}
