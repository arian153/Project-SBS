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
#include "../../EngineDefine.hpp"
#include "../../Physics/Dynamics/MassData.hpp"
#include "../Algebra/Quaternion.hpp"
#include "../Algebra/Vector3.hpp"
#include "../Structure/VecQuatScale.hpp"
#include "../Structure/Vector3Pair.hpp"

namespace Engine
{
    class Ray;
    class PrimitiveRenderer;
    enum class eRenderingMode;
    class Color;

    enum class ePrimitiveType
    {
        Circle,
        Ellipse,
        Polygon,
        Rectangle,
        Triangle,
        Box,
        Capsule,
        Cone,
        Cylinder,
        Dome,
        Ellipsoid,
        Polyhedron,
        Sphere,
        Tetrahedron,
        TruncatedCone,
        Last
    };

    constexpr Real BOUNDING_VOLUME_MARGIN = 0.05f;

    class Primitive
    {
    public:
        Primitive();
        explicit Primitive(const Vector3& position, const Quaternion& orientation);
        virtual  ~Primitive();

        Vector3  LocalToWorldPoint(const Vector3& local_point) const;
        Vector3  WorldToLocalPoint(const Vector3& world_point) const;
        Vector3  LocalToWorldVector(const Vector3& local_vector) const;
        Vector3  WorldToLocalVector(const Vector3& world_vector) const;
        Matrix44 LocalToWorldMatrix() const;

        ePrimitiveType Type() const;
        size_t         UUID() const;

    public:
        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;

        //Primitive Data
        virtual void SetUnit() = 0;

        //Minkowski Support - gjk, epa
        virtual Vector3 Support(const Vector3& direction) = 0;

        //Ray - Primitive Intersection
        virtual bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const = 0;
        virtual Vector3 GetNormal(const Vector3& local_point_on_primitive) = 0;

        virtual MassData    CalculateMassData(Real density) const = 0;
        virtual Real        CalculateVolume() const = 0;
        virtual Vector3Pair CalculateBoundPair(const VecQuatScale& world) const = 0;

    public:
        VecQuatScale transform;

    protected:
        ePrimitiveType m_type = ePrimitiveType::Last;
        size_t         m_uuid = 0;
    };
}
