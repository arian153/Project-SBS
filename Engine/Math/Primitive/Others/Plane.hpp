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
#include "../../Algebra/Vector3.hpp"
#include "../../Algebra/Quaternion.hpp"

namespace Engine
{
    class Ray;

    class Plane
    {
    public:
        Plane();
        ~Plane();
        Plane(Real a, Real b, Real c, Real d);
        Plane(const Vector3& p0, const Vector3& p1, const Vector3& p2);
        Plane(const Plane& rhs);
        Plane& operator=(const Plane& rhs);
        bool   operator==(const Plane& rhs) const;

        void Set(Real a, Real b, Real c, Real d);
        void Set(const Vector3& normal, Real offset);
        void Set(const Vector3& normal, const Vector3& point);
        void Set(const Vector3& p0, const Vector3& p1, const Vector3& p2);

        Real    Distance(const Vector3& point) const;
        Real    PlaneTest(const Vector3& point) const;
        Vector3 ClosestPoint(const Vector3& point) const;
        Vector3 Normal() const;
        Vector3 Origin() const;

        bool    IntersectRay(const Ray& ray, Real& t) const;
        Vector3 IntersectEdge(const Vector3& p0, const Vector3& p1) const;
        Vector3 IntersectPoint(const Vector3& p0, const Vector3& p1) const;

        void SetOrigin(const Vector3& origin);
        void Rotate(const Quaternion& rotation);
        void SetNormalize();

    public:
        Real a, b, c, d;
    };
}
