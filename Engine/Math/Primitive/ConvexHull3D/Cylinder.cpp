#include "Cylinder.hpp"
#include "../../Utility/Utility.hpp"
#include "../Others/Ray.hpp"

namespace Engine
{
    Cylinder::Cylinder()
    {
        m_type = ePrimitiveType::Cylinder;
    }

    Cylinder::~Cylinder()
    {
    }

    void Cylinder::Initialize()
    {
    }

    void Cylinder::Shutdown()
    {
    }

    void Cylinder::SetUnit()
    {
        Real division = Math::Max(radius.x * 2.0f, Math::Max(radius.y * 2.0f, height));
        if (division > 0.0f)
        {
            radius /= division;
            height /= division;
        }
    }

    void Cylinder::SetCylinder(const Vector2& radius, Real height)
    {
        this->radius = radius;
        this->height = height;
    }

    Real Cylinder::HalfHeight() const
    {
        return height * 0.5f;
    }

    Vector3 Cylinder::Support(const Vector3& direction)
    {
        Vector3 axis_vector;
        axis_vector.y       = HalfHeight();
        Vector3 ellipse_dir = direction;
        ellipse_dir.y       = 0.0f;
        ellipse_dir.SetNormalize();
        Vector3 ellipse_radius(radius.x, 0.0f, radius.y);
        Vector3 ellipse_vector = ellipse_radius.HadamardProduct(ellipse_radius);
        ellipse_vector         = ellipse_vector.HadamardProduct(ellipse_dir);
        Real denom             = ellipse_radius.HadamardProduct(ellipse_dir).Length();
        ellipse_vector         = Math::IsZero(denom) ? ellipse_vector * 0.0f : ellipse_vector / denom;
        Real top_support       = direction.DotProduct(ellipse_vector + axis_vector);
        Real bot_support       = direction.DotProduct(ellipse_vector - axis_vector);
        return top_support > bot_support ? ellipse_vector + axis_vector : ellipse_vector - axis_vector;
    }

    bool Cylinder::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real half_height   = HalfHeight();
        minimum_t          = -1.0f;
        maximum_t          = -1.0f;
        Real denominator_x = 1.0f / (radius.x * radius.x);
        Real denominator_z = 1.0f / (radius.y * radius.y);
        Real a             = local_ray.direction.x * local_ray.direction.x * denominator_x + local_ray.direction.z * local_ray.direction.z * denominator_z;
        Real b             = 2.0f * (local_ray.direction.x * local_ray.position.x * denominator_x + local_ray.direction.z * local_ray.position.z * denominator_z);
        Real c             = local_ray.position.x * local_ray.position.x * denominator_x + local_ray.position.z * local_ray.position.z * denominator_z - 1.0f;
        Real cylinder_min_t, cylinder_max_t;
        if (Math::SolveQuadratic(a, b, c, cylinder_max_t, cylinder_min_t) == true)
        {
            if (cylinder_min_t > cylinder_max_t)
            {
                Real temp      = cylinder_min_t;
                cylinder_min_t = cylinder_max_t;
                cylinder_max_t = temp;
            }
            Real min_axis_height = local_ray.position.y + local_ray.direction.y * cylinder_min_t;
            Real max_axis_height = local_ray.position.y + local_ray.direction.y * cylinder_max_t;
            minimum_t            = cylinder_min_t;
            maximum_t            = cylinder_max_t;
            if (min_axis_height > half_height)
            {
                if (max_axis_height > half_height)
                {
                    return false;
                }
                Real disc_t = (half_height - local_ray.position.y) / local_ray.direction.y;
                Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                {
                    minimum_t = disc_t;
                }
            }
            if (max_axis_height < -half_height)
            {
                if (min_axis_height < -half_height)
                {
                    return false;
                }
                Real disc_t = (-half_height - local_ray.position.y) / local_ray.direction.y;
                Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                {
                    maximum_t = disc_t;
                }
            }
            if (max_axis_height > half_height)
            {
                Real disc_t = (half_height - local_ray.position.y) / local_ray.direction.y;
                Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                {
                    maximum_t = disc_t;
                }
            }
            if (min_axis_height < -half_height)
            {
                Real disc_t = (-half_height - local_ray.position.y) / local_ray.direction.y;
                Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                {
                    minimum_t = disc_t;
                }
            }
        }
        else
        {
            Real disc_t = (-half_height - local_ray.position.y) / local_ray.direction.y;
            Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
            Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
            if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
            {
                minimum_t = disc_t;
                maximum_t = (half_height - local_ray.position.y) / local_ray.direction.y;
                if (minimum_t > maximum_t)
                {
                    Real temp = minimum_t;
                    minimum_t = maximum_t;
                    maximum_t = temp;
                }
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

    Vector3 Cylinder::GetNormal(const Vector3& local_point_on_primitive)
    {
        Real    half_height   = HalfHeight();
        Real    denominator_x = 1.0f / (radius.x * radius.x);
        Real    denominator_z = 1.0f / (radius.y * radius.y);
        Real    point_radius  = local_point_on_primitive.x * local_point_on_primitive.x * denominator_x + local_point_on_primitive.z * local_point_on_primitive.z * denominator_z;
        Vector3 normal;
        Real    point_height = local_point_on_primitive.y;
        if ((Math::IsEqual(point_height, half_height) || Math::IsEqual(point_height, -half_height))
            && 1.0f > point_radius)
        {
            // point on top disc or bottom disc 
            // also point in disc.
            normal.y = Math::Signum(point_height);
        }
        else
        {
            // point on cylinder disc.
            normal.x = 2.0f * local_point_on_primitive.x * denominator_x;
            normal.z = 2.0f * local_point_on_primitive.z * denominator_z;
            normal.SetNormalize();
        }
        return normal;
    }
}
