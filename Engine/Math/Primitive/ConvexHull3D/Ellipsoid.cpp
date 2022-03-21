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

    MassData Ellipsoid::CalculateMassData(Real density) const
    {
        MassData data;

        data.mass  = density * CalculateVolume();
        Real a     = radius.x;
        Real b     = radius.z;
        Real c     = radius.y;
        Real it_xx = 0.2f * data.mass * (b * b + c * c);
        Real it_yy = 0.2f * data.mass * (a * a + b * b);
        Real it_zz = 0.2f * data.mass * (a * a + c * c);
        data.local_inertia.SetZero();
        data.local_inertia.SetDiagonal(it_xx, it_yy, it_zz);
        data.local_centroid.SetZero();
        data.CalculateInverse();
        return data;
    }

    Real Ellipsoid::CalculateVolume() const
    {
        return 4.0f / 3.0f * Math::PI * radius.x * radius.y * radius.z;
    }

    Vector3Pair Ellipsoid::CalculateBoundPair(const VecQuatScale& world) const
    {
        Real w = radius.x;
        Real h = radius.y;
        Real d = radius.z;

        Vector3 obb_vertices[8];
        obb_vertices[0].Set(+w, +h, +d);
        obb_vertices[1].Set(+w, +h, -d);
        obb_vertices[2].Set(+w, -h, +d);
        obb_vertices[3].Set(+w, -h, -d);
        obb_vertices[4].Set(-w, +h, +d);
        obb_vertices[5].Set(-w, +h, -d);
        obb_vertices[6].Set(-w, -h, +d);
        obb_vertices[7].Set(-w, -h, -d);

        Vector3 min = world.LocalToWorldPoint(transform.LocalToWorldPoint(obb_vertices[0]));
        Vector3 max = min;

        for (int i = 1; i < 8; ++i)
        {
            Vector3 vertex = world.LocalToWorldPoint(transform.LocalToWorldPoint(obb_vertices[i]));
            min.x          = Math::Min(min.x, vertex.x);
            min.y          = Math::Min(min.y, vertex.y);
            min.z          = Math::Min(min.z, vertex.z);
            max.x          = Math::Max(max.x, vertex.x);
            max.y          = Math::Max(max.y, vertex.y);
            max.z          = Math::Max(max.z, vertex.z);
        }

        return Vector3Pair(min, max);
    }
}
