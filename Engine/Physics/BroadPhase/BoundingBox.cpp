#include "BoundingBox.hpp"

#include "../../Math/Primitive/Primitive.hpp"
#include "../../Math/Structure/Transform.hpp"
#include "../../Math/Utility/Utility.hpp"
#include "../Dynamics/RigidBody.hpp"

namespace Engine
{
    BoundingBox::BoundingBox()
    {
    }

    BoundingBox::BoundingBox(const Vector3& lower_bound, const Vector3& upper_bound)
        : m_lower_bound(lower_bound), m_upper_bound(upper_bound)
    {
        m_type = eBoundingObjectType::None;
    }

    BoundingBox::BoundingBox(void* object_data, eBoundingObjectType type)
        : m_type(type)
    {
        if (object_data == nullptr)
        {
            m_type = eBoundingObjectType::None;
            return;
        }

        m_object_data = object_data;
        UpdateVolume();
    }

    BoundingBox::~BoundingBox()
    {
    }

    void BoundingBox::Set(const Vector3& lower_bound, const Vector3& upper_bound)
    {
        m_lower_bound = lower_bound;
        m_upper_bound = upper_bound;
    }

    void BoundingBox::Set(void* object_data, eBoundingObjectType type)
    {
        m_type = type;

        if (object_data == nullptr)
        {
            m_type = eBoundingObjectType::None;
            return;
        }

        m_object_data = object_data;
        UpdateVolume();
    }

    void BoundingBox::ExpandMargin(Real margin)
    {
        m_lower_bound -= margin;
        m_upper_bound += margin;
    }

    bool BoundingBox::Intersect(BoundingBox* aabb) const
    {
        // if separated in x direction
        if (m_lower_bound.x > aabb->m_upper_bound.x || aabb->m_lower_bound.x > m_upper_bound.x)
            return false;
        // if separated in y direction
        if (m_lower_bound.y > aabb->m_upper_bound.y || aabb->m_lower_bound.y > m_upper_bound.y)
            return false;
        // if separated in z direction
        if (m_lower_bound.z > aabb->m_upper_bound.z || aabb->m_lower_bound.z > m_upper_bound.z)
            return false;
        // if same object, skip intersection

        if (m_type == aabb->m_type && m_type != eBoundingObjectType::None && m_object_data == aabb->m_object_data)
        {
            return false;
        }
        // no separation, must be intersecting
        return true;
    }

    bool BoundingBox::Intersect(const BoundingBox& aabb) const
    {
        // if separated in x direction
        if (m_lower_bound.x > aabb.m_upper_bound.x || aabb.m_lower_bound.x > m_upper_bound.x)
            return false;
        // if separated in y direction
        if (m_lower_bound.y > aabb.m_upper_bound.y || aabb.m_lower_bound.y > m_upper_bound.y)
            return false;
        // if separated in z direction
        if (m_lower_bound.z > aabb.m_upper_bound.z || aabb.m_lower_bound.z > m_upper_bound.z)
            return false;

        // no separation, must be intersecting
        return true;
    }

    bool BoundingBox::Intersect(const Vector3& pos, Real radius) const
    {
        Real sq_d = 0.0f;
        for (size_t i = 0; i < 3; ++i)
        {
            Real v = pos[i];
            if (v < m_lower_bound[i])
                sq_d += (m_lower_bound[i] - v) * (m_lower_bound[i] - v);
            if (v > m_upper_bound[i])
                sq_d += (v - m_upper_bound[i]) * (v - m_upper_bound[i]);
        }
        return sq_d <= radius * radius;
    }

    bool BoundingBox::Contains(const Vector3& point) const
    {
        if (m_lower_bound.x > point.x || point.x > m_upper_bound.x)
        {
            return false;
        }
        if (m_lower_bound.y > point.y || point.y > m_upper_bound.y)
        {
            return false;
        }
        if (m_lower_bound.z > point.z || point.z > m_upper_bound.z)
        {
            return false;
        }
        return true;
    }

    bool BoundingBox::TestRayIntersection(const Ray& ray, Real& t, Real max_distance) const
    {
        // do tests against three sets of planes
        Real    t_min, t_max, t_y_min, t_y_max, t_z_min, t_z_max;
        Vector3 box_min = m_lower_bound;
        Vector3 box_max = m_upper_bound;
        Vector3 inv_dir;
        inv_dir.x = 1.0f / ray.direction.x;
        inv_dir.y = 1.0f / ray.direction.y;
        inv_dir.z = 1.0f / ray.direction.z;
        if (inv_dir.x > 0.0f)
        {
            t_min = (box_min.x - ray.position.x) * inv_dir.x;
            t_max = (box_max.x - ray.position.x) * inv_dir.x;
        }
        else
        {
            t_min = (box_max.x - ray.position.x) * inv_dir.x;
            t_max = (box_min.x - ray.position.x) * inv_dir.x;
        }
        if (inv_dir.y > 0.0f)
        {
            t_y_min = (box_min.y - ray.position.y) * inv_dir.y;
            t_y_max = (box_max.y - ray.position.y) * inv_dir.y;
        }
        else
        {
            t_y_min = (box_max.y - ray.position.y) * inv_dir.y;
            t_y_max = (box_min.y - ray.position.y) * inv_dir.y;
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
        if (inv_dir.z > 0.0f)
        {
            t_z_min = (box_min.z - ray.position.z) * inv_dir.z;
            t_z_max = (box_max.z - ray.position.z) * inv_dir.z;
        }
        else
        {
            t_z_min = (box_max.z - ray.position.z) * inv_dir.z;
            t_z_max = (box_min.z - ray.position.z) * inv_dir.z;
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
        t = t_min;
        if (t_min < 0.0f && t_max < 0.0f)
        {
            return false;
        }
        if (t <= 0.0f)
        {
            t = 0.0f;
        }
        // check max distance.
        if (t > max_distance && max_distance >= 0.0f)
        {
            return false;
        }
        // done, have intersection
        return true;
    }

    bool BoundingBox::Contains(BoundingBox* aabb) const
    {
        if (this->Intersect(aabb) == true)
        {
            if (m_lower_bound.x > aabb->m_lower_bound.x || aabb->m_upper_bound.x > m_upper_bound.x)
            {
                return false;
            }
            if (m_lower_bound.y > aabb->m_lower_bound.y || aabb->m_upper_bound.y > m_upper_bound.y)
            {
                return false;
            }
            if (m_lower_bound.z > aabb->m_lower_bound.z || aabb->m_upper_bound.z > m_upper_bound.z)
            {
                return false;
            }
            return true;
        }
        return false;
    }

    bool BoundingBox::Contains(const BoundingBox& aabb) const
    {
        if (Intersect(aabb) == false)
            return false;

        if (m_lower_bound.x > aabb.m_lower_bound.x || aabb.m_upper_bound.x > m_upper_bound.x)
        {
            return false;
        }
        if (m_lower_bound.y > aabb.m_lower_bound.y || aabb.m_upper_bound.y > m_upper_bound.y)
        {
            return false;
        }
        if (m_lower_bound.z > aabb.m_lower_bound.z || aabb.m_upper_bound.z > m_upper_bound.z)
        {
            return false;
        }
        return true;
    }

    Real BoundingBox::Volume() const
    {
        Vector3 d = m_upper_bound - m_lower_bound;
        return (d.x) * (d.y) * (d.z);
    }

    Real BoundingBox::SurfaceArea() const
    {
        Vector3 d = m_upper_bound - m_lower_bound;
        return 2.0f * (d.x * d.y + d.y * d.z + d.z * d.x);
    }

    Vector3 BoundingBox::Center() const
    {
        return 0.5f * (m_upper_bound + m_lower_bound);
    }

    Vector3 BoundingBox::Size() const
    {
        return m_upper_bound - m_lower_bound;
    }

    Vector3 BoundingBox::HalfSize() const
    {
        return 0.5f * (m_upper_bound - m_lower_bound);
    }

    Vector3 BoundingBox::LowerBound() const
    {
        return m_lower_bound;
    }

    Vector3 BoundingBox::UpperBound() const
    {
        return m_upper_bound;
    }

    BoundingBox BoundingBox::Union(const BoundingBox& aabb) const
    {
        BoundingBox result;
        result.m_lower_bound = this->m_lower_bound;
        result.m_upper_bound = this->m_upper_bound;
        if (aabb.m_lower_bound.x < this->m_lower_bound.x)
        {
            result.m_lower_bound.x = aabb.m_lower_bound.x;
        }
        if (aabb.m_lower_bound.y < this->m_lower_bound.y)
        {
            result.m_lower_bound.y = aabb.m_lower_bound.y;
        }
        if (aabb.m_lower_bound.z < this->m_lower_bound.z)
        {
            result.m_lower_bound.z = aabb.m_lower_bound.z;
        }
        if (aabb.m_upper_bound.x > this->m_upper_bound.x)
        {
            result.m_upper_bound.x = aabb.m_upper_bound.x;
        }
        if (aabb.m_upper_bound.y > this->m_upper_bound.y)
        {
            result.m_upper_bound.y = aabb.m_upper_bound.y;
        }
        if (aabb.m_upper_bound.z > this->m_upper_bound.z)
        {
            result.m_upper_bound.z = aabb.m_upper_bound.z;
        }
        return result;
    }

    void* BoundingBox::GetObjectData() const
    {
        return m_object_data;
    }

    BroadPhaseNode* BoundingBox::GetNodeData() const
    {
        return m_node_data;
    }

    RigidBody* BoundingBox::GetRigidBodyData() const
    {
        if (m_type == eBoundingObjectType::RigidBody)
            return static_cast<RigidBody*>(m_object_data);

        return nullptr;
    }

    void BoundingBox::UpdateVolume()
    {
        if (m_type == eBoundingObjectType::RigidBody)
        {
            auto rigid_body = static_cast<RigidBody*>(m_object_data);

            VecQuatScale& transform = rigid_body->GetVqs();


            Real w = 0.5f * transform.scale;
            Real h = 0.5f * transform.scale;
            Real d = 0.5f * transform.scale;

            Vector3 vertices[8];
            vertices[0].Set(+w, +h, +d);
            vertices[1].Set(+w, +h, -d);
            vertices[2].Set(+w, -h, +d);
            vertices[3].Set(+w, -h, -d);
            vertices[4].Set(-w, +h, +d);
            vertices[5].Set(-w, +h, -d);
            vertices[6].Set(-w, -h, +d);
            vertices[7].Set(-w, -h, -d);

            Vector3 min = transform.LocalToWorldPoint(vertices[0]);
            Vector3 max = min;

            for (int i = 1; i < 8; ++i)
            {
                Vector3 vertex = (transform.LocalToWorldPoint(vertices[i]));
                min.x = Math::Min(min.x, vertex.x);
                min.y = Math::Min(min.y, vertex.y);
                min.z = Math::Min(min.z, vertex.z);
                max.x = Math::Max(max.x, vertex.x);
                max.y = Math::Max(max.y, vertex.y);
                max.z = Math::Max(max.z, vertex.z);
            }
            m_lower_bound = min;
            m_upper_bound = max;
        }
        else if (m_type == eBoundingObjectType::Transform)
        {
            auto transform = static_cast<Transform*>(m_object_data);
            transform->position;
            transform->scale;

            Real w = 0.5f * transform->scale.x;
            Real h = 0.5f * transform->scale.y;
            Real d = 0.5f * transform->scale.z;

            Vector3 vertices[8];
            vertices[0].Set(+w, +h, +d);
            vertices[1].Set(+w, +h, -d);
            vertices[2].Set(+w, -h, +d);
            vertices[3].Set(+w, -h, -d);
            vertices[4].Set(-w, +h, +d);
            vertices[5].Set(-w, +h, -d);
            vertices[6].Set(-w, -h, +d);
            vertices[7].Set(-w, -h, -d);

            Vector3 min = transform->LocalToWorldPoint(vertices[0]);
            Vector3 max = min;

            for (int i = 1; i < 8; ++i)
            {
                Vector3 vertex = (transform->LocalToWorldPoint(vertices[i]));
                min.x          = Math::Min(min.x, vertex.x);
                min.y          = Math::Min(min.y, vertex.y);
                min.z          = Math::Min(min.z, vertex.z);
                max.x          = Math::Max(max.x, vertex.x);
                max.y          = Math::Max(max.y, vertex.y);
                max.z          = Math::Max(max.z, vertex.z);
            }
            m_lower_bound = min;
            m_upper_bound = max;
        }
        else if (m_type == eBoundingObjectType::Primitive)
        {
            auto primitive = static_cast<Primitive*>(m_object_data);
            auto pair      = primitive->CalculateBoundPair(VecQuatScale::Identity());
            m_lower_bound  = pair.a;
            m_upper_bound  = pair.b;
        }
        else if (m_type == eBoundingObjectType::SoftBody)
        {
        }
    }
}
