#pragma once
#include "../Primitive.hpp"
#include <vector>

namespace Engine
{
    class Polyhedron final : public Primitive
    {
    public:
        Polyhedron();
        ~Polyhedron() override;

        void Initialize() override;
        void Shutdown() override;

        //Primitive Data
        void SetUnit() override;

        //Minkowski Support - gjk, epa
        Vector3 Support(const Vector3& direction) override;

        //Ray - Primitive Intersection
        bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const override;
        Vector3 GetNormal(const Vector3& local_point_on_primitive) override;

        void UpdateMinMaxPoint();

    public:
        std::vector<Vector3>* vertices = nullptr;

    private:
        Vector3 m_max_point;
        Vector3 m_min_point;
    };
}
