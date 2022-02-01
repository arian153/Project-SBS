#pragma once
#include "../Primitive.hpp"
#include "../../Algebra/Vector2.hpp"

namespace Engine
{
    class TruncatedCone final : public Primitive
    {
    public:
        TruncatedCone();
        ~TruncatedCone() override;

        void Initialize() override;
        void Shutdown() override;

        //Primitive Data
        void SetUnit() override;
        void SetTruncatedCone(const Vector2& radius, Real height, Real ratio);
        Real HalfHeight() const;

        //Minkowski Support - gjk, epa
        Vector3 Support(const Vector3& direction) override;

        //Ray - Primitive Intersection
        bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const override;
        Vector3 GetNormal(const Vector3& local_point_on_primitive) override;

    public:
        Vector2 radius;
        Real    height = 1.0f;
        Real    ratio  = 0.5f;
    };
}
