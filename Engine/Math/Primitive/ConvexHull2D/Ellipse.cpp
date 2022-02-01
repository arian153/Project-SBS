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
}
