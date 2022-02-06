#pragma once
#include "../../Math/Algebra/Matrix44.hpp"
#include "../../Math/Structure/Basis.hpp"
#include "../../Math/Structure/Transform.hpp"

namespace Engine
{
    class Camera
    {
    public:
        Camera();
        ~Camera();

        void LookAt(const Vector3& position, const Vector3& target, const Vector3& up = Math::Vector3::Y_AXIS);
        void UpdateViewMatrix();

        Vector3    GetPosition() const;
        Quaternion GetOrientation() const;
        Matrix44   GetViewMatrix() const;
        Basis      GetBasis() const;

    private:
        Matrix44  m_view_matrix;
        Transform m_transform;
        Basis     m_basis;
    };
}
