/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "TruncatedCone.hpp"

#include "../../Utility/Utility.hpp"
#include "../Others/Ray.hpp"

namespace Engine
{
    TruncatedCone::TruncatedCone()
    {
        m_type = ePrimitiveType::TruncatedCone;
    }

    TruncatedCone::~TruncatedCone()
    {
    }

    void TruncatedCone::Initialize()
    {
    }

    void TruncatedCone::Shutdown()
    {
    }

    void TruncatedCone::SetUnit()
    {
        Real division = Math::Max(radius.x * 2.0f, Math::Max(radius.y * 2.0f, height));
        if (division > 0.0f)
        {
            radius /= division;
            height /= division;
        }
    }

    void TruncatedCone::SetTruncatedCone(const Vector2& radius, Real height, Real ratio)
    {
        this->radius = radius;
        this->height = height;
        this->ratio  = ratio;
    }

    Real TruncatedCone::HalfHeight() const
    {
        return height * 0.5f;
    }

    Vector3 TruncatedCone::Support(const Vector3& direction)
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
        Real top_support       = direction.DotProduct(ratio * ellipse_vector + axis_vector);
        Real bot_support       = direction.DotProduct(ellipse_vector - axis_vector);
        return top_support > bot_support ? ratio * ellipse_vector + axis_vector : ellipse_vector - axis_vector;
    }

    bool TruncatedCone::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        Real half_height        = HalfHeight();
        minimum_t               = -1.0f;
        maximum_t               = -1.0f;
        Real denominator_x      = 1.0f / (radius.x * radius.x);
        Real denominator_y      = 1.0f / (height * height);
        Real denominator_z      = 1.0f / (radius.y * radius.y);
        Real ratio_multiplier_a = (ratio - 1.0f) * (ratio - 1.0f);
        Real ratio_multiplier_b = (ratio + 1.0f) * (ratio + 1.0f);
        Real ratio_multiplier_c = ratio * ratio - 1.0f;
        //make quadratic
        Real a = local_ray.direction.x * local_ray.direction.x * denominator_x
                + local_ray.direction.z * local_ray.direction.z * denominator_z
                - local_ray.direction.y * local_ray.direction.y * ratio_multiplier_a * denominator_y;
        Real b = 2.0f * local_ray.direction.x * local_ray.position.x * denominator_x
                + 2.0f * local_ray.direction.z * local_ray.position.z * denominator_z
                - 2.0f * local_ray.direction.y * local_ray.position.y * ratio_multiplier_a * denominator_y
                - local_ray.direction.y * ratio_multiplier_c / height;
        Real c = local_ray.position.x * local_ray.position.x * denominator_x
                + local_ray.position.z * local_ray.position.z * denominator_z
                - local_ray.position.y * local_ray.position.y * ratio_multiplier_a * denominator_y
                - local_ray.position.y * ratio_multiplier_c / height
                - 0.25f * ratio_multiplier_b;
        Real truncated_min_t, truncated_max_t;
        bool b_disc_dir         = !Math::IsZero(local_ray.direction.y);
        Real top_disc_t         = b_disc_dir ? (half_height - local_ray.position.y) / local_ray.direction.y : -1.0f;
        Real bot_disc_t         = b_disc_dir ? (-half_height - local_ray.position.y) / local_ray.direction.y : -1.0f;
        Real top_disc_a         = local_ray.direction.x * top_disc_t + local_ray.position.x;
        Real top_disc_b         = local_ray.direction.z * top_disc_t + local_ray.position.z;
        Real bot_disc_a         = local_ray.direction.x * bot_disc_t + local_ray.position.x;
        Real bot_disc_b         = local_ray.direction.z * bot_disc_t + local_ray.position.z;
        bool b_contain_top_disc = b_disc_dir ? top_disc_a * top_disc_a * denominator_x + top_disc_b * top_disc_b * denominator_z <= ratio * ratio : false;
        bool b_contain_bot_disc = b_disc_dir ? bot_disc_a * bot_disc_a * denominator_x + bot_disc_b * bot_disc_b * denominator_z <= 1.0f : false;
        if (Math::SolveQuadratic(a, b, c, truncated_max_t, truncated_min_t) == true)
        {
            if (truncated_min_t > truncated_max_t)
            {
                Real temp       = truncated_min_t;
                truncated_min_t = truncated_max_t;
                truncated_max_t = temp;
            }
            if (bot_disc_t > top_disc_t)
            {
                Real temp          = top_disc_t;
                top_disc_t         = bot_disc_t;
                bot_disc_t         = temp;
                bool temp2         = b_contain_top_disc;
                b_contain_top_disc = b_contain_bot_disc;
                b_contain_bot_disc = temp2;
            }
            minimum_t            = truncated_min_t;
            maximum_t            = truncated_max_t;
            Real min_axis_height = local_ray.position.y + local_ray.direction.y * truncated_min_t;
            Real max_axis_height = local_ray.position.y + local_ray.direction.y * truncated_max_t;
            if (min_axis_height > half_height)
            {
                if (max_axis_height > half_height)
                {
                    if (b_contain_top_disc && b_contain_bot_disc)
                    {
                        minimum_t = bot_disc_t;
                        maximum_t = top_disc_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (b_contain_top_disc)
                {
                    minimum_t = top_disc_t;
                }
                if (b_contain_bot_disc)
                {
                    minimum_t = bot_disc_t;
                }
            }
            else if (min_axis_height < -half_height)
            {
                if (max_axis_height < -half_height)
                {
                    if (b_contain_top_disc && b_contain_bot_disc)
                    {
                        minimum_t = bot_disc_t;
                        maximum_t = top_disc_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (b_contain_top_disc)
                {
                    minimum_t = top_disc_t;
                }
                if (b_contain_bot_disc)
                {
                    minimum_t = bot_disc_t;
                }
            }
            if (max_axis_height > half_height)
            {
                if (min_axis_height < -half_height)
                {
                    if (b_contain_top_disc && b_contain_bot_disc)
                    {
                        minimum_t = bot_disc_t;
                        maximum_t = top_disc_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (b_contain_bot_disc)
                {
                    maximum_t = bot_disc_t;
                }
                if (b_contain_top_disc)
                {
                    maximum_t = top_disc_t;
                }
            }
            else if (max_axis_height < -half_height)
            {
                if (min_axis_height > half_height)
                {
                    if (b_contain_top_disc && b_contain_bot_disc)
                    {
                        minimum_t = bot_disc_t;
                        maximum_t = top_disc_t;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (b_contain_bot_disc)
                {
                    maximum_t = bot_disc_t;
                }
                if (b_contain_top_disc)
                {
                    maximum_t = top_disc_t;
                }
            }
        }
        else
        {
            if (ratio < 1.0f)
            {
                if (b_contain_bot_disc)
                {
                    minimum_t = bot_disc_t;
                    if (b_contain_top_disc)
                    {
                        maximum_t = top_disc_t;
                    }
                    else
                    {
                        if (Math::IsEqual(truncated_max_t, truncated_min_t))
                        {
                            maximum_t = truncated_max_t;
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
                if (b_contain_top_disc)
                {
                    minimum_t = top_disc_t;
                    if (b_contain_bot_disc)
                    {
                        maximum_t = bot_disc_t;
                    }
                    else
                    {
                        if (Math::IsEqual(truncated_max_t, truncated_min_t))
                        {
                            maximum_t = truncated_max_t;
                        }
                    }
                }
                else
                {
                    return false;
                }
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

    Vector3 TruncatedCone::GetNormal(const Vector3& local_point_on_primitive)
    {
        Real    half_height   = HalfHeight();
        Real    n_ratio       = (ratio + 1.0f) * 0.5f + local_point_on_primitive.y * (ratio - 1.0f) / height;
        Real    x_max         = local_point_on_primitive.x / n_ratio;
        Real    z_max         = local_point_on_primitive.z / n_ratio;
        Real    denominator_x = 1.0f / (radius.x * radius.x);
        Real    denominator_y = 1.0f / (height * height);
        Real    denominator_z = 1.0f / (radius.y * radius.y);
        Real    t             = 2.0f * (1.0f - ratio) * (local_point_on_primitive.x * x_max * denominator_x + local_point_on_primitive.z * z_max * denominator_z) * denominator_y;
        Real    point_radius  = local_point_on_primitive.x * local_point_on_primitive.x * denominator_x + local_point_on_primitive.z * local_point_on_primitive.z * denominator_z;
        Vector3 normal;
        if (Math::IsEqual(local_point_on_primitive.y, half_height) && point_radius < ratio * ratio)
        {
            normal.y = Math::Signum(local_point_on_primitive.y);
        }
        else if (Math::IsEqual(local_point_on_primitive.y, -half_height) && point_radius < 1.0f)
        {
            normal.y = Math::Signum(local_point_on_primitive.y);
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

    MassData TruncatedCone::CalculateMassData(Real density) const
    {
        MassData data;

        data.mass          = density * CalculateVolume();
        Real ratio_factor  = (ratio * ratio + ratio + 1.0f);
        Real ratio_multi_a = (ratio * ratio * ratio * ratio + ratio * ratio * ratio + ratio * ratio + ratio + 1.0f) / ratio_factor;
        Real ratio_multi_b = (ratio * ratio * ratio * ratio + 4.0f * ratio * ratio * ratio + 10.0f * ratio * ratio + 4.0f * ratio + 1.0f) / (ratio_factor * ratio_factor);
        Real a             = radius.x;
        Real b             = radius.y;
        Real h             = height;
        Real it_xx         = 0.15f * data.mass * b * b * ratio_multi_a + 0.0375f * data.mass * h * h * ratio_multi_b;
        Real it_zz         = 0.15f * data.mass * a * a * ratio_multi_a + 0.0375f * data.mass * h * h * ratio_multi_b;
        Real it_yy         = 0.15f * data.mass * (a * a + b * b) * ratio_multi_a;
        data.local_inertia.SetZero();
        data.local_inertia.SetDiagonal(it_xx, it_yy, it_zz);
        data.local_centroid = Vector3(0.0f, ((3.0f * ratio * ratio + 2.0f * ratio + 1.0f) * h / (4.0f * ratio_factor)) - HalfHeight(), 0.0f);
        data.CalculateInverse();
        return data;
    }

    Real TruncatedCone::CalculateVolume() const
    {
        Real ratio_factor = ratio * ratio + ratio + 1.0f;
        return Math::PI * radius.x * radius.y * height * ratio_factor / 3.0f;
    }

    Vector3Pair TruncatedCone::CalculateBoundPair(const VecQuatScale& world) const
    {
        Real w = ratio > 1.0f ? radius.x * ratio : radius.x;
        Real h = height * 0.5f;
        Real d = ratio > 1.0f ? radius.y * ratio : radius.y;

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
