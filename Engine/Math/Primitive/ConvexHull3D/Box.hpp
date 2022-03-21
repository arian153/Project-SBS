#pragma once
#include "../Primitive.hpp"

namespace Engine
{
    class Box final : public Primitive
    {
    public:
        Box();
        Box(Real width, Real height, Real depth);
        explicit Box(const Vector3& half_extent);
        ~Box() override;

        void Initialize() override;
        void Shutdown() override;

        //Primitive Data
        void SetUnit() override;
        void SetBox(Real width, Real height, Real depth);

        //Minkowski Support - gjk, epa
        Vector3 Support(const Vector3& direction) override;

        //Ray - Primitive Intersection
        bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const override;
        Vector3 GetNormal(const Vector3& local_point_on_primitive) override;

        MassData    CalculateMassData(Real density = 1.0f) const override;
        Real        CalculateVolume() const override;
        Vector3Pair CalculateBoundPair(const VecQuatScale& world) const override;

    public:
        Vector3 vertices[8];
    };
}
