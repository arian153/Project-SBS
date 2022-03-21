#include "Rectangle.hpp"
#include "../../Utility/Utility.hpp"
#include "../Others/Ray.hpp"

namespace Engine
{
    Rectangle::Rectangle()
    {
        m_type = ePrimitiveType::Rectangle;
    }

    Rectangle::~Rectangle()
    {
    }

    void Rectangle::Initialize()
    {
    }

    void Rectangle::Shutdown()
    {
    }

    void Rectangle::SetUnit()
    {
        Real w = 0.5f;
        Real h = 0.5f;

        vertices[0].Set(+w, +h);
        vertices[1].Set(+w, -h);
        vertices[2].Set(-w, +h);
        vertices[3].Set(-w, -h);
    }

    Vector3 Rectangle::Support(const Vector3& direction)
    {
        Vector2 sub_space_direction;
        sub_space_direction.x = direction.x;
        sub_space_direction.y = direction.y;
        sub_space_direction.SetNormalize();
        Vector2 result;
        Real    p = Math::REAL_NEGATIVE_MAX;
        for (size_t i = 0; i < 4; ++i)
        {
            Real projection = vertices[i].DotProduct(sub_space_direction);
            if (projection > p)
            {
                result = vertices[i];
                p      = projection;
            }
        }
        return Vector3(result.x, result.y, 0.0f);
    }

    bool Rectangle::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        //add a ray-collider intersection test code.
        minimum_t = -1.0f;
        maximum_t = -1.0f;
        //plane elements
        Vector3 normal(0.0f, 0.0f, 1.0f);
        Vector3 pc          = -local_ray.position;
        Real    denominator = normal.DotProduct(local_ray.direction);
        Real    t_y_min, t_y_max;
        Vector2 rect_min = vertices[3];
        Vector2 rect_max = vertices[0];
        Vector3 inv_dir  = local_ray.direction;
        inv_dir.SetInverse();
        if (Math::IsZero(denominator) == true)
        {
            //ray is parallel to plane.
            if (Math::IsZero(pc.DotProduct(normal)) == true)
            {
                //ray is on the plane.
                if (inv_dir.x >= 0.0f)
                {
                    minimum_t = (rect_min.x - local_ray.position.x) * inv_dir.x;
                    maximum_t = (rect_max.x - local_ray.position.x) * inv_dir.x;
                }
                else
                {
                    minimum_t = (rect_max.x - local_ray.position.x) * inv_dir.x;
                    maximum_t = (rect_min.x - local_ray.position.x) * inv_dir.x;
                }
                if (inv_dir.y >= 0.0f)
                {
                    t_y_min = (rect_min.y - local_ray.position.y) * inv_dir.y;
                    t_y_max = (rect_max.y - local_ray.position.y) * inv_dir.y;
                }
                else
                {
                    t_y_min = (rect_max.y - local_ray.position.y) * inv_dir.y;
                    t_y_max = (rect_min.y - local_ray.position.y) * inv_dir.y;
                }
                if ((minimum_t > t_y_max) || (t_y_min > maximum_t))
                {
                    return false;
                }
                if (t_y_min > minimum_t)
                {
                    minimum_t = t_y_min;
                }
                if (t_y_max < maximum_t)
                {
                    maximum_t = t_y_max;
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
            //define rectangle.
            if (rect_min.x <= plane_intersection.x && rect_max.x >= plane_intersection.x &&
                rect_min.y <= plane_intersection.y && rect_max.y >= plane_intersection.y)
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

    Vector3 Rectangle::GetNormal(const Vector3& local_point_on_primitive)
    {
        Vector2 rect_min = vertices[3];
        Vector2 rect_max = vertices[0];
        if (Math::IsEqual(local_point_on_primitive.x, rect_min.x))
        {
            return Vector3(-1.0f);
        }
        if (Math::IsEqual(local_point_on_primitive.x, rect_max.x))
        {
            return Vector3(1.0f);
        }
        if (Math::IsEqual(local_point_on_primitive.y, rect_min.y))
        {
            return Vector3(0.0f, -1.0f);
        }
        if (Math::IsEqual(local_point_on_primitive.y, rect_max.y))
        {
            return Vector3(0.0f, 1.0f);
        }
        return Math::Vector3::Z_AXIS;
    }

    MassData Rectangle::CalculateMassData(Real density) const
    {
        MassData data;

        Real w    = (vertices[0] - vertices[2]).x;
        Real h    = (vertices[0] - vertices[3]).y;
        data.mass = density * w * h;
        Real it_a = data.mass / 12.0f * (h * h);
        Real it_b = data.mass / 12.0f * (w * w);
        Real it_c = data.mass / 12.0f * (w * w + h * h);

        data.local_inertia.SetZero();
        data.local_inertia.SetDiagonal(it_a, it_b, it_c);
        data.local_centroid = Vector3(0.5f * w, 0.5f * h, 0.0f) + Vector3(vertices[3].x, vertices[3].y, 0.0f);
        data.CalculateInverse();

        return data;
    }

    Real Rectangle::CalculateVolume() const
    {
        Real w = (vertices[0] - vertices[3]).x;
        Real h = (vertices[0] - vertices[3]).y;
        return w * h;
    }

    Vector3Pair Rectangle::CalculateBoundPair(const VecQuatScale& world) const
    {
        Vector3 obb_vertices[8];
        obb_vertices[0].Set(vertices[0].x, vertices[0].y, BOUNDING_VOLUME_MARGIN);
        obb_vertices[1].Set(vertices[0].x, vertices[0].y, -BOUNDING_VOLUME_MARGIN);
        obb_vertices[2].Set(vertices[1].x, vertices[1].y, BOUNDING_VOLUME_MARGIN);
        obb_vertices[3].Set(vertices[1].x, vertices[1].y, -BOUNDING_VOLUME_MARGIN);
        obb_vertices[4].Set(vertices[2].x, vertices[2].y, BOUNDING_VOLUME_MARGIN);
        obb_vertices[5].Set(vertices[2].x, vertices[2].y, -BOUNDING_VOLUME_MARGIN);
        obb_vertices[6].Set(vertices[3].x, vertices[3].y, BOUNDING_VOLUME_MARGIN);
        obb_vertices[7].Set(vertices[3].x, vertices[3].y, -BOUNDING_VOLUME_MARGIN);

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
