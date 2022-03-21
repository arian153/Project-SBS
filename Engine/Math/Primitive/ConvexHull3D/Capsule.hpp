#pragma once
#include "../Primitive.hpp"

namespace Engine
{
    class Capsule final : public Primitive
    {
    public:
        Capsule();
        Capsule(const Vector3& radius, Real height);
        ~Capsule() override;

        void Initialize() override;
        void Shutdown() override;

        //Primitive Data
        void SetUnit() override;
        void SetCapsule(const Vector3& radius, Real height);
        Real HalfHeight() const;

        //Minkowski Support - gjk, epa
        Vector3 Support(const Vector3& direction) override;

        //Ray - Primitive Intersection
        bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const override;
        Vector3 GetNormal(const Vector3& local_point_on_primitive) override;

        MassData    CalculateMassData(Real density = 1.0f) const override;
        Real        CalculateVolume() const override;
        Vector3Pair CalculateBoundPair(const VecQuatScale& world) const override;

    private:
        bool TestRayEllipsoid(const Ray& ray, const Vector3& centroid, Real& min_t, Real& max_t) const;

    public:
        Vector3 radius;
        Real    height = 1.0f;
    };
}
