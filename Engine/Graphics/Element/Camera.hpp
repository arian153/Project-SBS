/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

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

        Transform& GetTransform();

        void SetPosition(const Vector3& position);
        void AddPosition(const Vector3& delta_pos);

    private:
        Matrix44  m_view_matrix;
        Transform m_transform;
        Basis     m_basis;
    };
}
