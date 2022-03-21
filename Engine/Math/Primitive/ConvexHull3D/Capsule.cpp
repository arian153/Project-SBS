#include "Capsule.hpp"
#include "../../Utility/Utility.hpp"
#include "../Others/Ray.hpp"

namespace Engine
{
    Capsule::Capsule()
    {
        m_type = ePrimitiveType::Capsule;
    }

    Capsule::Capsule(const Vector3& radius, Real height)
        : radius(radius), height(height)
    {
        m_type = ePrimitiveType::Capsule;
    }

    Capsule::~Capsule()
    {
    }

    void Capsule::Initialize()
    {
        SetCapsule(Vector3(0.25f, 0.25f, 0.25f), 0.5f);
    }

    void Capsule::Shutdown()
    {
    }

    void Capsule::SetUnit()
    {
        Real length = radius.y * 2.0f + height;
        if (length > 0.0f)
        {
            radius /= length;
            height /= length;
        }
    }

    void Capsule::SetCapsule(const Vector3& radius, Real height)
    {
        this->radius = radius;
        this->height = height;
    }

    Real Capsule::HalfHeight() const
    {
        return height * 0.5f;
    }

    Vector3 Capsule::Support(const Vector3& direction)
    {
        Vector3 axis_vector;
        axis_vector.y            = HalfHeight();
        Vector3 ellipsoid_vector = radius.HadamardProduct(radius);
        ellipsoid_vector         = ellipsoid_vector.HadamardProduct(direction);
        Real denom               = radius.HadamardProduct(direction).Length();
        ellipsoid_vector         = Math::IsZero(denom) ? ellipsoid_vector * 0.0f : ellipsoid_vector / denom;

        Real top_projection = direction.DotProduct(ellipsoid_vector + axis_vector);
        Real bot_projection = direction.DotProduct(ellipsoid_vector - axis_vector);

        return top_projection > bot_projection ? ellipsoid_vector + axis_vector : ellipsoid_vector - axis_vector;
    }

    bool Capsule::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real    half_height = HalfHeight();
        Vector3 capsule_a(0.0f, -half_height, 0.0f);
        Vector3 capsule_b(0.0f, half_height, 0.0f);
        Vector3 axis(0.0f, height, 0.0f);
        minimum_t             = -1.0f;
        maximum_t             = -1.0f;
        Real    denominator_x = 1.0f / (radius.x * radius.x);
        Real    denominator_z = 1.0f / (radius.z * radius.z);
        Real    a             = local_ray.direction.x * local_ray.direction.x * denominator_x + local_ray.direction.z * local_ray.direction.z * denominator_z;
        Real    b             = 2.0f * (local_ray.direction.x * local_ray.position.x * denominator_x + local_ray.direction.z * local_ray.position.z * denominator_z);
        Real    c             = local_ray.position.x * local_ray.position.x * denominator_x + local_ray.position.z * local_ray.position.z * denominator_z - 1.0f;
        Vector3 axis_test     = local_ray.direction - local_ray.direction.ProjectionTo(axis);
        Real    cylinder_min_t, cylinder_max_t;
        Real    sphere_a_min_t, sphere_a_max_t, sphere_b_min_t, sphere_b_max_t;
        bool    b_sphere_a = TestRayEllipsoid(local_ray, capsule_a, sphere_a_min_t, sphere_a_max_t);
        bool    b_sphere_b = TestRayEllipsoid(local_ray, capsule_b, sphere_b_min_t, sphere_b_max_t);
        if (Math::IsZero(axis_test.LengthSquared()))
        {
            if (!b_sphere_a || !b_sphere_b)
            {
                return false;
            }
            minimum_t = sphere_a_min_t < sphere_b_min_t ? sphere_a_min_t : sphere_b_min_t;
            maximum_t = sphere_a_max_t > sphere_b_max_t ? sphere_a_max_t : sphere_b_max_t;
        }
        else
        {
            if (Math::SolveQuadratic(a, b, c, cylinder_max_t, cylinder_min_t) == true)
            {
                minimum_t                  = cylinder_min_t;
                maximum_t                  = cylinder_max_t;
                Real ellipsoid_height      = half_height + radius.y;
                Real cylinder_min_t_height = local_ray.position.y + local_ray.direction.y * cylinder_min_t;
                Real cylinder_max_t_height = local_ray.position.y + local_ray.direction.y * cylinder_max_t;
                Real sphere_a_min_t_height = Math::REAL_MAX;
                Real sphere_a_max_t_height = Math::REAL_MAX;
                Real sphere_b_min_t_height = Math::REAL_MAX;
                Real sphere_b_max_t_height = Math::REAL_MAX;
                if (b_sphere_a == true)
                {
                    sphere_a_min_t_height = local_ray.position.y + local_ray.direction.y * sphere_a_min_t;
                    sphere_a_max_t_height = local_ray.position.y + local_ray.direction.y * sphere_a_max_t;
                }
                if (b_sphere_b == true)
                {
                    sphere_b_min_t_height = local_ray.position.y + local_ray.direction.y * sphere_b_min_t;
                    sphere_b_max_t_height = local_ray.position.y + local_ray.direction.y * sphere_b_max_t;
                }
                if (cylinder_min_t_height > ellipsoid_height && cylinder_max_t_height > ellipsoid_height)
                {
                    return false;
                }
                if (cylinder_min_t_height < -ellipsoid_height && cylinder_max_t_height < -ellipsoid_height)
                {
                    return false;
                }
                if (cylinder_min_t_height > half_height)
                {
                    if (b_sphere_b == true)
                    {
                        minimum_t = sphere_b_min_t_height > sphere_b_max_t_height ? sphere_b_min_t : sphere_b_max_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (cylinder_min_t_height < -half_height)
                {
                    if (b_sphere_a == true)
                    {
                        minimum_t = sphere_a_min_t_height < sphere_a_max_t_height ? sphere_a_min_t : sphere_a_max_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (cylinder_max_t_height > half_height)
                {
                    if (b_sphere_b == true)
                    {
                        maximum_t = sphere_b_min_t_height > sphere_b_max_t_height ? sphere_b_min_t : sphere_b_max_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (cylinder_max_t_height < -half_height)
                {
                    if (b_sphere_a == true)
                    {
                        maximum_t = sphere_a_min_t_height < sphere_a_max_t_height ? sphere_a_min_t : sphere_a_max_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (b_sphere_a == true && b_sphere_b == true)
                {
                    minimum_t = Math::Min(Math::Min(Math::Min(sphere_a_min_t, sphere_a_max_t), sphere_b_min_t), sphere_b_max_t);
                    maximum_t = Math::Max(Math::Max(Math::Max(sphere_a_min_t, sphere_a_max_t), sphere_b_min_t), sphere_b_max_t);
                }
            }
            else
            {
                return false;
            }
        }
        if (minimum_t > maximum_t)
        {
            Real temp = minimum_t;
            minimum_t = maximum_t;
            maximum_t = temp;
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

    Vector3 Capsule::GetNormal(const Vector3& local_point_on_primitive)
    {
        Real    half_height = HalfHeight();
        Vector3 normal;
        if (local_point_on_primitive.y > half_height || local_point_on_primitive.y < -half_height)
        {
            //sphere case
            Vector3 sphere_origin;
            sphere_origin.y   = half_height * Math::Signum(local_point_on_primitive.y);
            Vector3 ellipsoid = local_point_on_primitive - sphere_origin;
            normal.x          = 2.0f * ellipsoid.x / radius.x * radius.x;
            normal.y          = 2.0f * ellipsoid.y / radius.y * radius.y;
            normal.z          = 2.0f * ellipsoid.z / radius.z * radius.z;
            normal.SetNormalize();
        }
        else
        {
            //disc case
            normal.x = 2.0f * local_point_on_primitive.x / radius.x * radius.x;
            normal.y = 0.0f;
            normal.z = 2.0f * local_point_on_primitive.z / radius.z * radius.z;
            normal.SetNormalize();
        }
        return normal;
    }

    MassData Capsule::CalculateMassData(Real density) const
    {
        MassData data;

        data.mass    = density * CalculateVolume();
        Real a       = radius.x;
        Real b       = radius.z;
        Real c       = radius.y;
        Real h       = height;
        Real multi_a = 2.0f * c * data.mass / (4.0f * c + 3.0f * h);
        Real multi_b = 3.0f * h * data.mass / (4.0f * c + 3.0f * h);
        Real it_xx   = multi_a * (0.4f * (b * b + c * c) + 0.75f * h * c + 0.5f * h * h) + multi_b * (0.25f * b * b + h * h / 12.0f);
        Real it_zz   = multi_a * (0.4f * (a * a + c * c) + 0.75f * h * c + 0.5f * h * h) + multi_b * (0.25f * a * a + h * h / 12.0f);
        Real it_yy   = multi_a * 0.4f * (a * a + b * b) + multi_b * 0.25f * (a * a + b * b);
        data.local_inertia.SetZero();
        data.local_inertia.SetDiagonal(it_xx, it_yy, it_zz);
        data.local_centroid = Vector3(0.0f, 0.0f, 0.0f);
        data.CalculateInverse();

        return data;
    }

    Real Capsule::CalculateVolume() const
    {
        return Math::PI * radius.x * radius.z * (radius.y * 4.0f / 3.0f + height);
    }

    Vector3Pair Capsule::CalculateBoundPair(const VecQuatScale& world) const
    {
        Real w = radius.x;
        Real h = radius.y + height * 0.5f;
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

    bool Capsule::TestRayEllipsoid(const Ray& ray, const Vector3& centroid, Real& min_t, Real& max_t) const
    {
        Vector3 co = ray.position - centroid;
        Real    a  = (ray.direction.x * ray.direction.x / (radius.x * radius.x))
                + (ray.direction.y * ray.direction.y / (radius.y * radius.y))
                + (ray.direction.z * ray.direction.z / (radius.z * radius.z));
        Real b = (2.0f * ray.direction.x * co.x / (radius.x * radius.x))
                + (2.0f * ray.direction.y * co.y / (radius.y * radius.y))
                + (2.0f * ray.direction.z * co.z / (radius.z * radius.z));
        Real c = (co.x * co.x / (radius.x * radius.x))
                + (co.y * co.y / (radius.y * radius.y))
                + (co.z * co.z / (radius.z * radius.z)) - 1.0f;
        if (Math::SolveQuadratic(a, b, c, max_t, min_t) == true)
        {
            if (min_t < 0.0f && max_t < 0.0f)
            {
                return false;
            }
            if (min_t > max_t)
            {
                Real temp = min_t;
                min_t     = max_t;
                max_t     = temp;
            }
            return true;
        }
        return false;
    }
}
