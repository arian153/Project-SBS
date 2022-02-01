#include "Sphere.hpp"
#include "../../Utility/Utility.hpp"
#include "../Others/Ray.hpp"

namespace Engine
{
    Sphere::Sphere()
    {
        m_type = ePrimitiveType::Sphere;
    }

    Sphere::Sphere(const Vector3& position, const Quaternion& orientation, Real radius)
        : Primitive(position, orientation), radius(radius)
    {
        m_type = ePrimitiveType::Sphere;
    }

    Sphere::~Sphere()
    {
    }

    void Sphere::Initialize()
    {
        SetUnit();
    }

    void Sphere::Shutdown()
    {
    }

    void Sphere::SetUnit()
    {
        radius = 0.5f;
    }

    Vector3 Sphere::Support(const Vector3& direction)
    {
        return radius * direction;
    }

    bool Sphere::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real a = (local_ray.direction.x * local_ray.direction.x / (radius * radius))
                + (local_ray.direction.y * local_ray.direction.y / (radius * radius))
                + (local_ray.direction.z * local_ray.direction.z / (radius * radius));
        Real b = (2.0f * local_ray.direction.x * local_ray.position.x / (radius * radius))
                + (2.0f * local_ray.direction.y * local_ray.position.y / (radius * radius))
                + (2.0f * local_ray.direction.z * local_ray.position.z / (radius * radius));
        Real c = (local_ray.position.x * local_ray.position.x / (radius * radius))
                + (local_ray.position.y * local_ray.position.y / (radius * radius))
                + (local_ray.position.z * local_ray.position.z / (radius * radius)) - 1.0f;
        if (Math::SolveQuadratic(a, b, c, maximum_t, minimum_t) == true)
        {
            if (minimum_t < 0.0f && maximum_t < 0.0f)
            {
                return false;
            }
            if (minimum_t <= 0.0f)
            {
                minimum_t = 0.0f;
            }
            return true;
        }
        return false;
    }

    Vector3 Sphere::GetNormal(const Vector3& local_point_on_primitive)
    {
        return local_point_on_primitive.Normalize();
    }

    
    bool Sphere::HasIntersection(const Sphere& sphere) const
    {
        Real sq_d = position.DistanceSquaredTo(sphere.position);
        return sq_d <= (radius + sphere.radius) * (radius + sphere.radius);
    }
}
