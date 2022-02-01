#include "Ellipsoid.hpp"
#include "../../Utility/Utility.hpp"
#include "../Others/Ray.hpp"

namespace Engine
{
    Ellipsoid::Ellipsoid()
    {
        m_type = ePrimitiveType::Ellipsoid;
    }

    Ellipsoid::~Ellipsoid()
    {
    }

    void Ellipsoid::Initialize()
    {
        SetUnit();
    }

    void Ellipsoid::Shutdown()
    {
    }

    void Ellipsoid::SetUnit()
    {
        radius.SetNormalize();
    }

    Vector3 Ellipsoid::Support(const Vector3& direction)
    {
        Vector3 result = radius.HadamardProduct(radius);
        result         = result.HadamardProduct(direction);
        result /= radius.HadamardProduct(direction).Length();
        return result;
    }

    bool Ellipsoid::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real a = (local_ray.direction.x * local_ray.direction.x / (radius.x * radius.x))
                + (local_ray.direction.y * local_ray.direction.y / (radius.y * radius.y))
                + (local_ray.direction.z * local_ray.direction.z / (radius.z * radius.z));
        Real b = (2.0f * local_ray.direction.x * local_ray.position.x / (radius.x * radius.x))
                + (2.0f * local_ray.direction.y * local_ray.position.y / (radius.y * radius.y))
                + (2.0f * local_ray.direction.z * local_ray.position.z / (radius.z * radius.z));
        Real c = (local_ray.position.x * local_ray.position.x / (radius.x * radius.x))
                + (local_ray.position.y * local_ray.position.y / (radius.y * radius.y))
                + (local_ray.position.z * local_ray.position.z / (radius.z * radius.z)) - 1.0f;
        minimum_t = -1.0f;
        maximum_t = -1.0f;
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

    Vector3 Ellipsoid::GetNormal(const Vector3& local_point_on_primitive)
    {
        Vector3 normal;
        normal.x = 2.0f * local_point_on_primitive.x / radius.x * radius.x;
        normal.y = 2.0f * local_point_on_primitive.y / radius.y * radius.y;
        normal.z = 2.0f * local_point_on_primitive.z / radius.z * radius.z;
        return normal;
    }
}
