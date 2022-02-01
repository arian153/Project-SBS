#pragma once
#include "../Primitive.hpp"
#include <vector>
#include "../../Algebra/Vector2.hpp"

namespace Engine
{
    class Polygon final : public Primitive
    {
    public:
        Polygon();
        ~Polygon() override;

        void Initialize() override;
        void Shutdown() override;

        //Primitive Data
        void SetUnit() override;

        //Minkowski Support - gjk, epa
        Vector3 Support(const Vector3& direction) override;

        //Ray - Primitive Intersection
        bool    TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const override;
        Vector3 GetNormal(const Vector3& local_point_on_primitive) override;
        
    public:
        std::vector<Vector2>* vertices = nullptr;
    };
}
