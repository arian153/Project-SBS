/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "Ellipse.hpp"
#include "../../Utility/Utility.hpp"
#include "../Others/Ray.hpp"

namespace Engine
{
    Ellipse::Ellipse()
    {
        m_type = ePrimitiveType::Ellipse;
    }

    Ellipse::~Ellipse()
    {
    }

    void Ellipse::Initialize()
    {
    }

    void Ellipse::Shutdown()
    {
    }

    void Ellipse::SetUnit()
    {
        radius.SetNormalize();
    }

    Vector3 Ellipse::Support(const Vector3& direction)
    {
        Vector2 sub_space_direction(direction);
        sub_space_direction.SetNormalize();
        Vector2 result = radius.HadamardProduct(radius);
        result         = result.HadamardProduct(sub_space_direction);
        result /= radius.HadamardProduct(sub_space_direction).Length();
        return Vector3(result.x, result.y, 0.0f);
    }

    bool Ellipse::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        minimum_t          = -1.0f;
        maximum_t          = -1.0f;
        Real denominator_x = 1.0f / (radius.x * radius.x);
        Real denominator_y = 1.0f / (radius.y * radius.y);
        //Quadratic elements
        Real a = local_ray.direction.x * local_ray.direction.x * denominator_x + local_ray.direction.y * local_ray.direction.y * denominator_y;
        Real b = 2.0f * (local_ray.direction.x * local_ray.position.x * denominator_x + local_ray.direction.y * local_ray.position.y * denominator_y);
        Real c = local_ray.position.x * local_ray.position.x * denominator_x + local_ray.position.y * local_ray.position.y * denominator_y - 1.0f;
        Real ellipse_min_t, ellipse_max_t;
        //plane elements
        Vector3 normal(0.0f, 0.0f, 1.0f);
        Vector3 pc          = -local_ray.position;
        Real    denominator = normal.DotProduct(local_ray.direction);
        if (Math::IsZero(denominator) == true)
        {
            //ray is parallel to plane.
            if (Math::IsZero(pc.DotProduct(normal)) == true)
            {
                //ray is on the plane.
                if (Math::SolveQuadratic(a, b, c, ellipse_max_t, ellipse_min_t))
                {
                    //solve intersection
                    if (Math::IsEqual(ellipse_min_t, ellipse_max_t))
                    {
                        minimum_t = maximum_t = ellipse_min_t;
                    }
                    else
                    {
                        minimum_t = ellipse_min_t;
                        maximum_t = ellipse_max_t;
                    }
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            //ray-plane intersect one point.
            Real    plane_t            = pc.DotProduct(normal) / denominator;
            Vector3 plane_intersection = local_ray.position + local_ray.direction * plane_t;
            //define ellipse.
            Real ellipse_result = plane_intersection.x * plane_intersection.x * denominator_x + plane_intersection.y * plane_intersection.y * denominator_y;
            if (ellipse_result <= 1.0f)
            {
                minimum_t = maximum_t = plane_t;
            }
            else
            {
                return false;
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

    Vector3 Ellipse::GetNormal(const Vector3& local_point_on_primitive)
    {
        Real ellipse_result = local_point_on_primitive.x * local_point_on_primitive.x / (radius.x * radius.x)
                + local_point_on_primitive.y * local_point_on_primitive.y / (radius.y * radius.y);
        if (Math::IsEqual(ellipse_result, 1.0f))
        {
            Vector3 normal;
            normal.x = 2.0f * local_point_on_primitive.x / radius.x * radius.x;
            normal.y = 2.0f * local_point_on_primitive.y / radius.y * radius.y;
            normal.SetNormalize();
            return normal;
        }
        return Math::Vector3::Z_AXIS;
    }

    MassData Ellipse::CalculateMassData(Real density) const
    {
        MassData data;

        data.mass  = density * CalculateVolume();
        Real a     = radius.x;
        Real b     = radius.y;
        Real it_xx = 0.25f * data.mass * b * b;
        Real it_yy = 0.25f * data.mass * a * a;
        Real it_zz = 0.25f * data.mass * (a * a + b * b);
        data.local_inertia.SetZero();
        data.local_inertia.SetDiagonal(it_xx, it_yy, it_zz);
        data.local_centroid = Math::Vector3::ORIGIN;
        data.CalculateInverse();

        return data;
    }

    Real Ellipse::CalculateVolume() const
    {
        return Math::PI * radius.x * radius.y;
    }

    Vector3Pair Ellipse::CalculateBoundPair(const VecQuatScale& world) const
    {
        Real w = radius.x;
        Real h = radius.y;
        Real d = BOUNDING_VOLUME_MARGIN;

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
