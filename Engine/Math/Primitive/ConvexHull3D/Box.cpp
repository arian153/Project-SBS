/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "Box.hpp"
#include "../../Utility/Utility.hpp"
#include "../Others/Ray.hpp"

namespace Engine
{
    Box::Box()
    {
        m_type = ePrimitiveType::Box;
    }

    Box::Box(Real width, Real height, Real depth)
    {
        SetBox(width, height, depth);
        m_type = ePrimitiveType::Box;
    }

    Box::Box(const Vector3& half_extent)
    {
        SetBox(half_extent.x, half_extent.y, half_extent.z);
        m_type = ePrimitiveType::Box;
    }

    Box::~Box()
    {
    }

    void Box::Initialize()
    {
        SetUnit();
    }

    void Box::Shutdown()
    {
    }

    void Box::SetUnit()
    {
        auto min_max      = vertices[0] - vertices[7];
        Real scale_factor = min_max.Length();
        min_max /= scale_factor;
        Real w = 0.5f * min_max.x;
        Real h = 0.5f * min_max.y;
        Real d = 0.5f * min_max.z;
        vertices[0].Set(+w, +h, +d);
        vertices[1].Set(+w, +h, -d);
        vertices[2].Set(+w, -h, +d);
        vertices[3].Set(+w, -h, -d);
        vertices[4].Set(-w, +h, +d);
        vertices[5].Set(-w, +h, -d);
        vertices[6].Set(-w, -h, +d);
        vertices[7].Set(-w, -h, -d);
    }

    void Box::SetBox(Real width, Real height, Real depth)
    {
        Real w = 0.5f * width;
        Real h = 0.5f * height;
        Real d = 0.5f * depth;

        vertices[0].Set(+w, +h, +d);
        vertices[1].Set(+w, +h, -d);
        vertices[2].Set(+w, -h, +d);
        vertices[3].Set(+w, -h, -d);
        vertices[4].Set(-w, +h, +d);
        vertices[5].Set(-w, +h, -d);
        vertices[6].Set(-w, -h, +d);
        vertices[7].Set(-w, -h, -d);
    }

    Vector3 Box::Support(const Vector3& direction)
    {
        Real    p = Math::REAL_NEGATIVE_MAX;
        Vector3 result;
        for (size_t i = 0; i < 8; ++i)
        {
            Real projection = DotProduct(vertices[i], direction);
            if (projection > p)
            {
                result = vertices[i];
                p      = projection;
            }
        }
        return result;
    }

    bool Box::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real    t_min, t_max, t_y_min, t_y_max, t_z_min, t_z_max;
        Vector3 box_min = vertices[7];
        Vector3 box_max = vertices[0];
        Vector3 inv_dir;
        inv_dir.x = 1.0f / local_ray.direction.x;
        inv_dir.y = 1.0f / local_ray.direction.y;
        inv_dir.z = 1.0f / local_ray.direction.z;

        if (inv_dir.x >= 0.0f)
        {
            t_min = (box_min.x - local_ray.position.x) * inv_dir.x;
            t_max = (box_max.x - local_ray.position.x) * inv_dir.x;
        }
        else
        {
            t_min = (box_max.x - local_ray.position.x) * inv_dir.x;
            t_max = (box_min.x - local_ray.position.x) * inv_dir.x;
        }
        if (inv_dir.y >= 0.0f)
        {
            t_y_min = (box_min.y - local_ray.position.y) * inv_dir.y;
            t_y_max = (box_max.y - local_ray.position.y) * inv_dir.y;
        }
        else
        {
            t_y_min = (box_max.y - local_ray.position.y) * inv_dir.y;
            t_y_max = (box_min.y - local_ray.position.y) * inv_dir.y;
        }
        if ((t_min > t_y_max) || (t_y_min > t_max))
        {
            return false;
        }
        if (t_y_min > t_min)
        {
            t_min = t_y_min;
        }
        if (t_y_max < t_max)
        {
            t_max = t_y_max;
        }
        if (inv_dir.z >= 0.0f)
        {
            t_z_min = (box_min.z - local_ray.position.z) * inv_dir.z;
            t_z_max = (box_max.z - local_ray.position.z) * inv_dir.z;
        }
        else
        {
            t_z_min = (box_max.z - local_ray.position.z) * inv_dir.z;
            t_z_max = (box_min.z - local_ray.position.z) * inv_dir.z;
        }
        if ((t_min > t_z_max) || (t_z_min > t_max))
        {
            return false;
        }
        if (t_z_min > t_min)
        {
            t_min = t_z_min;
        }
        if (t_z_max < t_max)
        {
            t_max = t_z_max;
        }
        if (t_min < 0.0f && t_max < 0.0f)
        {
            return false;
        }
        minimum_t = t_min;
        maximum_t = t_max;
        if (minimum_t <= 0.0f)
        {
            minimum_t = 0.0f;
        }
        return true;
    }

    Vector3 Box::GetNormal(const Vector3& local_point_on_primitive)
    {
        Vector3 normal;
        Real    w = (vertices[0] - vertices[4]).x;
        Real    h = (vertices[0] - vertices[2]).y;
        Real    d = (vertices[0] - vertices[1]).z;
        Vector3 size(w, h, d);
        Real    min      = Math::REAL_POSITIVE_MAX;
        Real    distance = fabsf(size.x - fabsf(local_point_on_primitive.x));
        if (distance < min)
        {
            min = distance;
            normal.Set(1.0f, 0.0f, 0.0f);
            normal *= Math::Signum(local_point_on_primitive.x);
        }
        distance = fabsf(size.y - fabsf(local_point_on_primitive.y));
        if (distance < min)
        {
            min = distance;
            normal.Set(0.0f, 1.0f, 0.0f);
            normal *= Math::Signum(local_point_on_primitive.y);
        }
        distance = fabsf(size.z - fabsf(local_point_on_primitive.z));
        if (distance < min)
        {
            normal.Set(0.0f, 0.0f, 1.0f);
            normal *= Math::Signum(local_point_on_primitive.z);
        }
        return normal;
    }

    MassData Box::CalculateMassData(Real density) const
    {
        MassData data;

        Real w    = (vertices[0] - vertices[4]).x;
        Real h    = (vertices[0] - vertices[2]).y;
        Real d    = (vertices[0] - vertices[1]).z;
        data.mass = density * w * h * d;
        Real it_a = data.mass / 12.0f * (h * h + d * d);
        Real it_b = data.mass / 12.0f * (w * w + d * d);
        Real it_c = data.mass / 12.0f * (w * w + h * h);
        data.local_inertia.SetZero();
        data.local_inertia.SetDiagonal(it_a, it_b, it_c);
        data.local_centroid = Vector3(0.5f * w, 0.5f * h, 0.5f * d) + vertices[7];
        data.CalculateInverse();

        return data;
    }

    Real Box::CalculateVolume() const
    {
        Real w = (vertices[0] - vertices[4]).x;
        Real h = (vertices[0] - vertices[2]).y;
        Real d = (vertices[0] - vertices[1]).z;
        return w * h * d;
    }

    Vector3Pair Box::CalculateBoundPair(const VecQuatScale& world) const
    {
        Vector3 min = world.LocalToWorldPoint(transform.LocalToWorldPoint(vertices[0]));
        Vector3 max = min;

        for (int i = 1; i < 8; ++i)
        {
            Vector3 vertex = world.LocalToWorldPoint(transform.LocalToWorldPoint(vertices[i]));
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
