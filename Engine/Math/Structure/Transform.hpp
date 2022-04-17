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
#include "../Algebra/Matrix44.hpp"
#include "../Algebra/Quaternion.hpp"
#include "../Algebra/Vector3.hpp"

namespace Engine
{
    class Transform
    {
    public:
        Transform();
        ~Transform();

        Transform& operator=(const Transform& rhs);

        Matrix44 LocalToWorldMatrix() const;

        void SetAffineMatrix(const Matrix44& affine);

        Vector3 LocalToWorldPoint(const Vector3& local_point) const;
        Vector3 WorldToLocalPoint(const Vector3& world_point) const;
        Vector3 LocalToWorldVector(const Vector3& local_vector) const;
        Vector3 WorldToLocalVector(const Vector3& world_vector) const;

    public:
        Vector3    position;
        Vector3    scale = Vector3(1.0f, 1.0f, 1.0f);
        Quaternion orientation;
    };
}
