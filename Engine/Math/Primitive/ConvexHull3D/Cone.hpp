#pragma once
#include "../Primitive.hpp"
#include "../../Algebra/Vector2.hpp"

namespace Engine
{
    class Cone final : public Primitive
    {
    public:
        Cone();
        Cone(const Vector3& position, const Quaternion& orientation, const Vector3& radius, Real height);
        ~Cone() override;

        void Initialize() override;
        void Shutdown() override;

        //Primitive Data
        void SetUnit() override;
        void SetCone(const Vector2& radius, Real height);
        Real HalfHeight() const;

        //Minkowski Support - gjk, epa
        Vector3 Support(const Vector3& direction) override;

        //Ray - Primitive Intersection
        bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const override;
        Vector3 GetNormal(const Vector3& local_point_on_primitive) override;

        MassData    CalculateMassData(Real density = 1.0f) const override;
        Real        CalculateVolume() const override;
        Vector3Pair CalculateBoundPair(const VecQuatScale& world) const override;

    public:
        Vector2 radius;
        Real    height = 1.0f;
    };
}
