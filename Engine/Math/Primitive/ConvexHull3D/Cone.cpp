#include"Cone.hpp"
#include "../../Utility/Utility.hpp"
#include "../Others/Ray.hpp"

namespace Engine
{
    Cone::Cone()
    {
        m_type = ePrimitiveType::Cone;
    }

    Cone::Cone(const Vector3& position, const Quaternion& orientation, const Vector3& radius, Real height)
        : Primitive(position, orientation), radius(radius), height(height)
    {
        m_type = ePrimitiveType::Cone;
    }

    Cone::~Cone()
    {
    }

    void Cone::Initialize()
    {
    }

    void Cone::Shutdown()
    {
    }

    void Cone::SetUnit()
    {
        Real division = Math::Max(radius.x * 2.0f, Math::Max(radius.y * 2.0f, height));
        if (division > 0.0f)
        {
            radius /= division;
            height /= division;
        }
    }

    void Cone::SetCone(const Vector2& radius, Real height)
    {
        this->height = height;
        this->radius = radius;
    }

    Real Cone::HalfHeight() const
    {
        return height * 0.5f;
    }

    Vector3 Cone::Support(const Vector3& direction)
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
        Real point_support     = direction.DotProduct(axis_vector);
        Real ellipse_support   = direction.DotProduct(ellipse_vector - axis_vector);
        return point_support >= ellipse_support ? axis_vector : ellipse_vector - axis_vector;
    }

    bool Cone::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real half_height   = HalfHeight();
        minimum_t          = -1.0f;
        maximum_t          = -1.0f;
        Real denominator_x = 1.0f / (radius.x * radius.x);
        Real denominator_y = 1.0f / (height * height);
        Real denominator_z = 1.0f / (radius.y * radius.y);
        //make quadratic
        Real a = local_ray.direction.x * local_ray.direction.x * denominator_x
                + local_ray.direction.z * local_ray.direction.z * denominator_z
                - local_ray.direction.y * local_ray.direction.y * denominator_y;
        Real b = 2.0f * local_ray.direction.x * local_ray.position.x * denominator_x
                + 2.0f * local_ray.direction.z * local_ray.position.z * denominator_z
                - 2.0f * local_ray.direction.y * local_ray.position.y * denominator_y
                + local_ray.direction.y / height;
        Real c = local_ray.position.x * local_ray.position.x * denominator_x
                + local_ray.position.z * local_ray.position.z * denominator_z
                - local_ray.position.y * local_ray.position.y * denominator_y
                + local_ray.position.y / height - 0.25f;
        Real cone_min_t, cone_max_t;
        if (Math::SolveQuadratic(a, b, c, cone_max_t, cone_min_t) == true)
        {
            if (cone_min_t > cone_max_t)
            {
                Real temp  = cone_min_t;
                cone_min_t = cone_max_t;
                cone_max_t = temp;
            }
            Real min_axis_height = local_ray.position.y + local_ray.direction.y * cone_min_t;
            Real max_axis_height = local_ray.position.y + local_ray.direction.y * cone_max_t;
            if (min_axis_height > half_height)
            {
                if (max_axis_height > half_height)
                {
                    return false;
                }
                if (max_axis_height < -half_height)
                {
                    return false;
                }
                //cone_min_t intersect shadow cone.
                minimum_t = cone_max_t;
                maximum_t = (-half_height - local_ray.position.y) / local_ray.direction.y;
            }
            else
            {
                if (min_axis_height < -half_height)
                {
                    //test_disc
                    Real disc_t = (-half_height - local_ray.position.y) / local_ray.direction.y;
                    Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                    Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                    if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                    {
                        minimum_t = disc_t;
                        maximum_t = cone_max_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    minimum_t = cone_min_t;
                    maximum_t = cone_max_t;
                }
                if (max_axis_height < -half_height)
                {
                    //test disc
                    Real disc_t = (-half_height - local_ray.position.y) / local_ray.direction.y;
                    Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                    Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                    if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                    {
                        maximum_t = disc_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (max_axis_height > half_height)
                {
                    //case when intersect disc first after then intersect cone.
                    if (min_axis_height <= half_height)
                    {
                        maximum_t = minimum_t;
                        minimum_t = (-half_height - local_ray.position.y) / local_ray.direction.y;
                    }
                    else
                    {
                        //intersect shadow cone.
                        return false;
                    }
                }
            }
            if (Math::IsEqual(min_axis_height, half_height) == true)
            {
                Real disc_t = (-half_height - local_ray.position.y) / local_ray.direction.y;
                Real disc_a = local_ray.direction.x * disc_t + local_ray.position.x;
                Real disc_b = local_ray.direction.z * disc_t + local_ray.position.z;
                if (disc_a * disc_a * denominator_x + disc_b * disc_b * denominator_z <= 1.0f)
                {
                    minimum_t = disc_t;
                    if (minimum_t > maximum_t)
                    {
                        Real temp = minimum_t;
                        minimum_t = maximum_t;
                        maximum_t = temp;
                    }
                }
            }
        }
        else
        {
            return false;
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

    Vector3 Cone::GetNormal(const Vector3& local_point_on_primitive)
    {
        Real    half_height   = HalfHeight();
        Real    n_ratio       = 0.5f + -local_point_on_primitive.y / height;
        Real    x_max         = local_point_on_primitive.x / n_ratio;
        Real    z_max         = local_point_on_primitive.z / n_ratio;
        Real    denominator_x = 1.0f / (radius.x * radius.x);
        Real    denominator_y = 1.0f / (height * height);
        Real    denominator_z = 1.0f / (radius.y * radius.y);
        Real    t             = 2.0f * (local_point_on_primitive.x * x_max * denominator_x + local_point_on_primitive.z * z_max * denominator_z) * denominator_y;
        Real    point_radius  = local_point_on_primitive.x * local_point_on_primitive.x * denominator_x + local_point_on_primitive.z * local_point_on_primitive.z * denominator_z;
        Vector3 normal;
        Real    point_height = local_point_on_primitive.y;
        if (Math::IsEqual(point_height, half_height)
            || (Math::IsEqual(point_height, -half_height)
                && 1.0f > point_radius))
        {
            // point on top point or bottom disc 
            normal.y = Math::Signum(point_height);
        }
        else if (Math::IsEqual(point_height, -half_height)
            && Math::IsEqual(1.0f, point_radius))
        {
            // point on cylinder disc.
            normal.x = 2.0f * local_point_on_primitive.x * denominator_x;
            normal.z = 2.0f * local_point_on_primitive.z * denominator_z;
            normal.SetNormalize();
        }
        else
        {
            normal.x = 2.0f * local_point_on_primitive.x * denominator_x;
            normal.y = height * t;
            normal.z = 2.0f * local_point_on_primitive.z * denominator_z;
            normal.SetNormalize();
        }
        return normal;
    }

    MassData Cone::CalculateMassData(Real density) const
    {
        MassData data;

        data.mass  = density * CalculateVolume();
        Real a     = radius.x;
        Real b     = radius.y;
        Real h     = height;
        Real it_xx = 0.15f * data.mass * b * b + 0.0375f * data.mass * h * h;
        Real it_zz = 0.15f * data.mass * a * a + 0.0375f * data.mass * h * h;
        Real it_yy = 0.15f * data.mass * (a * a + b * b);
        data.local_inertia.SetZero();
        data.local_inertia.SetDiagonal(it_xx, it_yy, it_zz);
        data.local_centroid = Vector3(0.0f, h * 0.25f - HalfHeight(), 0.0f);
        data.CalculateInverse();

        return data;
    }

    Real Cone::CalculateVolume() const
    {
        return Math::PI * radius.x * radius.y * height / 3.0f;
    }

    Vector3Pair Cone::CalculateBoundPair(const VecQuatScale& world) const
    {
        Real w = radius.x;
        Real h = height * 0.5f;
        Real d = radius.y;

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
