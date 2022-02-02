#pragma once
#include "../../EngineDefine.hpp"
#include "../Algebra/Quaternion.hpp"
#include "../Algebra/Vector3.hpp"
#include "../Structure/VqsTransform.hpp"

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

    public:
        VqsTransform transform;

    protected:
        ePrimitiveType m_type = ePrimitiveType::Last;
    };
}
