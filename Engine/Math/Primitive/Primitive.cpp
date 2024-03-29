/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "Primitive.hpp"

#include "../../Utility/UUIDUtility.hpp"
#include "../Algebra/Matrix44.hpp"
#include "../Utility/MatrixUtility.hpp"

namespace Engine
{
    Primitive::Primitive()
    {
        m_uuid = UUIDUtility::Generate();
    }

    Primitive::Primitive(const Vector3& position, const Quaternion& orientation)
    {
        transform.position = position;
        transform.rotation = orientation;
        m_uuid             = UUIDUtility::Generate();
    }

    Primitive::~Primitive()
    {
    }

    //void Primitive::CastRay(PrimitiveRayCastResult& result, Real max_distance)
    //{
    //    result.hit_data.primitive = this;
    //    Ray  local_ray(WorldToLocalPoint(result.ray.position), WorldToLocalVector(result.ray.direction));
    //    Real minimum_t = -1.0f;
    //    Real maximum_t = -1.0f;
    //    if (this->TestRayIntersection(local_ray, minimum_t, maximum_t) == true)
    //    {
    //        result.hit_data.t   = minimum_t;
    //        result.hit_data.hit = true;
    //        //ray cast done
    //        Vector3 local_intersection = local_ray.position + local_ray.direction * result.hit_data.t;
    //        result.intersection        = result.ray.position + result.ray.direction * result.hit_data.t;
    //        result.normal              = LocalToWorldVector(GetNormal(local_intersection));
    //        if (result.hit_data.t > max_distance && max_distance >= 0.0f)
    //        {
    //            result.hit_data.hit = false;
    //        }
    //    }
    //}

    //void Primitive::IntersectRay(PrimitiveRayIntersectionResult& result, Real max_distance)
    //{
    //    Ray              world_ray = result.ray;
    //    Ray              local_ray(WorldToLocalPoint(result.ray.position), WorldToLocalVector(result.ray.direction));
    //    PrimitiveHitData hit_data_a;
    //    PrimitiveHitData hit_data_b;
    //    Real             minimum_t = -1.0f;
    //    Real             maximum_t = -1.0f;
    //    if (TestRayIntersection(local_ray, minimum_t, maximum_t) == true)
    //    {
    //        hit_data_a.t         = minimum_t;
    //        hit_data_a.primitive = this;
    //        hit_data_b.t         = maximum_t;
    //        hit_data_b.primitive = this;

    //        //ray cast done
    //        if (Math::IsEqual(hit_data_a.t, hit_data_b.t))
    //        {
    //            if (hit_data_a.t > max_distance || max_distance < 0.0f)
    //            {
    //                hit_data_a.hit = true;
    //                result.hit_list.push_back(hit_data_a);
    //            }
    //        }
    //        else
    //        {
    //            if (hit_data_b.t < hit_data_a.t)
    //            {
    //                auto temp  = hit_data_a;
    //                hit_data_a = hit_data_b;
    //                hit_data_b = temp;
    //            }
    //            if (hit_data_a.t < max_distance || max_distance < 0.0f)
    //            {
    //                hit_data_a.hit = true;
    //                result.hit_list.push_back(hit_data_a);
    //            }
    //            if (hit_data_b.t < max_distance || max_distance < 0.0f)
    //            {
    //                hit_data_b.hit = true;
    //                result.hit_list.push_back(hit_data_b);
    //            }
    //        }
    //    }
    //}

    Vector3 Primitive::LocalToWorldPoint(const Vector3& local_point) const
    {
        return transform.LocalToWorldPoint(local_point);
    }

    Vector3 Primitive::WorldToLocalPoint(const Vector3& world_point) const
    {
        return transform.WorldToLocalPoint(world_point);
    }

    Vector3 Primitive::LocalToWorldVector(const Vector3& local_vector) const
    {
        return transform.LocalToWorldVector(local_vector);
    }

    Vector3 Primitive::WorldToLocalVector(const Vector3& world_vector) const
    {
        return transform.LocalToWorldVector(world_vector);
    }

    Matrix44 Primitive::LocalToWorldMatrix() const
    {
        return transform.ToMatrix();
    }

    ePrimitiveType Primitive::Type() const
    {
        return m_type;
    }

    size_t Primitive::UUID() const
    {
        return m_uuid;
    }

   }
