/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "Dome.hpp"

#include "../../Utility/Utility.hpp"
#include "../Others/Ray.hpp"

namespace Engine
{
    Dome::Dome()
    {
        m_type = ePrimitiveType::Dome;
    }

    Dome::~Dome()
    {
    }

    void Dome::Initialize()
    {
    }

    void Dome::Shutdown()
    {
    }

    void Dome::SetUnit()
    {
        radius.SetNormalize();
    }

    Vector3 Dome::Support(const Vector3& direction)
    {
        Vector3 result = radius.HadamardProduct(radius);
        result         = result.HadamardProduct(direction);
        Real denom     = radius.HadamardProduct(direction).Length();
        result         = Math::IsZero(denom) ? result * 0.0f : result / denom;
        //check hemisphere
        if (result.y <= 0.0f)
        {
            Vector3 ellipse_dir = direction;
            ellipse_dir.y       = 0.0f;
            ellipse_dir.SetNormalize();
            Vector3 ellipse_radius = radius;
            ellipse_radius.y       = 0.0f;
            result                 = ellipse_radius.HadamardProduct(ellipse_radius);
            result                 = result.HadamardProduct(ellipse_dir);
            denom                  = ellipse_radius.HadamardProduct(ellipse_dir).Length();
            result                 = Math::IsZero(denom) ? result * 0.0f : result / denom;
        }
        return result;
    }

    bool Dome::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real denominator_x = 1.0f / (radius.x * radius.x);
        Real denominator_z = 1.0f / (radius.y * radius.y);
        //quadratic
        Real a = (local_ray.direction.x * local_ray.direction.x * denominator_x)
                + (local_ray.direction.y * local_ray.direction.y / (radius.y * radius.y))
                + (local_ray.direction.z * local_ray.direction.z * denominator_z);
        Real b = (2.0f * local_ray.direction.x * local_ray.position.x * denominator_x)
                + (2.0f * local_ray.direction.y * local_ray.position.y / (radius.y * radius.y))
                + (2.0f * local_ray.direction.z * local_ray.position.z * denominator_z);
        Real c = (local_ray.position.x * local_ray.position.x * denominator_x)
                + (local_ray.position.y * local_ray.position.y / (radius.y * radius.y))
                + (local_ray.position.z * local_ray.position.z * denominator_z) - 1.0f;
        if (Math::SolveQuadratic(a, b, c, maximum_t, minimum_t) == true)
        {
            Real max_height = local_ray.direction.y * maximum_t + local_ray.position.y;
            Real min_height = local_ray.direction.y * minimum_t + local_ray.position.y;
            if (max_height < 0.0f)
            {
                if (min_height < 0.0f)
                {
                    return false;
                }
                Real disc_t = -local_ray.position.y / local_ray.direction.y;
                Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                {
                    maximum_t = disc_t;
                }
            }
            if (min_height < 0.0f)
            {
                Real disc_t = -local_ray.position.y / local_ray.direction.y;
                Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                {
                    minimum_t = disc_t;
                }
            }
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

    Vector3 Dome::GetNormal(const Vector3& local_point_on_primitive)
    {
        if (Math::IsZero(local_point_on_primitive.y) || local_point_on_primitive.y <= 0.0f)
        {
            Real denominator_x = 1.0f / (radius.x * radius.x);
            Real denominator_z = 1.0f / (radius.y * radius.y);
            Real point_radius  = local_point_on_primitive.x * local_point_on_primitive.x * denominator_x + local_point_on_primitive.z * local_point_on_primitive.z * denominator_z;

            if (point_radius < 1.0f)
            {
                return Vector3(0.0f, -1.0f, 0.0f);
            }

            Vector3 disc_normal;
            disc_normal.x = 2.0f * local_point_on_primitive.x * denominator_x;
            disc_normal.z = 2.0f * local_point_on_primitive.z * denominator_z;
            disc_normal.SetNormalize();
            return disc_normal;
        }

        Vector3 ellipsoid_normal;
        ellipsoid_normal.x = 2.0f * local_point_on_primitive.x / radius.x * radius.x;
        ellipsoid_normal.y = 2.0f * local_point_on_primitive.y / radius.y * radius.y;
        ellipsoid_normal.z = 2.0f * local_point_on_primitive.z / radius.z * radius.z;
        return ellipsoid_normal;
    }

    MassData Dome::CalculateMassData(Real density) const
    {
        MassData data;

        data.mass = density * CalculateVolume();
        Real a    = radius.x;
        Real b    = radius.z;
        Real c    = radius.y;
        //0.059375 = 19/320
        Real it_xx = 0.2f * data.mass * b * b + 0.059375f * data.mass * c * c;
        Real it_zz = 0.2f * data.mass * a * a + 0.059375f * data.mass * c * c;
        Real it_yy = 0.2f * data.mass * (a * a + b * b);
        data.local_inertia.SetZero();
        data.local_inertia.SetDiagonal(it_xx, it_yy, it_zz);
        data.local_centroid.Set(0.0f, 0.375f * c, 0.0f);
        data.CalculateInverse();
        return data;
    }

    Real Dome::CalculateVolume() const
    {
        return 2.0f / 3.0f * Math::PI * radius.x * radius.y * radius.z;
    }

    Vector3Pair Dome::CalculateBoundPair(const VecQuatScale& world) const
    {
        Real w = radius.x;
        Real h = radius.y;
        Real d = radius.z;

        Vector3 obb_vertices[8];
        obb_vertices[0].Set(+w, +h, +d);
        obb_vertices[1].Set(+w, +h, -d);
        obb_vertices[2].Set(+w, 0, +d);
        obb_vertices[3].Set(+w, 0, -d);
        obb_vertices[4].Set(-w, +h, +d);
        obb_vertices[5].Set(-w, +h, -d);
        obb_vertices[6].Set(-w, 0, +d);
        obb_vertices[7].Set(-w, 0, -d);

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
