/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "Camera.hpp"

#include "../../Math/Utility/MatrixUtility.hpp"

namespace Engine
{
    Camera::Camera()
    {
    }

    Camera::~Camera()
    {
    }

    void Camera::LookAt(const Vector3& position, const Vector3& target, const Vector3& up)
    {
        m_view_matrix        = Math::Matrix44::LookAt(position, target, up);
        m_transform.position = position;
        m_basis.k            = (target - position).Normalize();
        m_basis.i            = CrossProduct(up, m_basis.k).Normalize();
        m_basis.j            = CrossProduct(m_basis.k, m_basis.i);
        m_transform.orientation.Set(Vector3(1.0f), m_basis.i);
    }

    void Camera::UpdateViewMatrix()
    {
        Vector3 up    = m_transform.orientation.Rotate(Math::Vector3::Y_AXIS);
        Vector3 look  = m_transform.orientation.Rotate(Math::Vector3::Z_AXIS);
        m_view_matrix = Math::Matrix44::LookAt(m_transform.position, look + m_transform.position, up);
    }

    Vector3 Camera::GetPosition() const
    {
        return m_transform.position;
    }

    Quaternion Camera::GetOrientation() const
    {
        return m_transform.orientation;
    }

    Matrix44 Camera::GetViewMatrix() const
    {
        return m_view_matrix;
    }

    Basis Camera::GetBasis() const
    {
        return m_basis;
    }

    Transform& Camera::GetTransform()
    {
        return m_transform;
    }

    void Camera::SetPosition(const Vector3& position)
    {
        m_transform.position = position;
        UpdateViewMatrix();
    }

    void Camera::AddPosition(const Vector3& delta_pos)
    {
        m_transform.position += delta_pos;
        UpdateViewMatrix();
    }
}
