#pragma once
#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Primitive/Others/Ray.hpp"

namespace Engine
{
    enum class eBoundingObjectType
    {
        Transform,
        Primitive,
        RigidBody,
        SoftBody,
        None
    };

    class BoundingBox
    {
    public:
        BoundingBox();
        BoundingBox(const Vector3& lower_bound, const Vector3& upper_bound);
        BoundingBox(void* object_data, eBoundingObjectType type);
        ~BoundingBox();

        void        Set(const Vector3& lower_bound, const Vector3& upper_bound);
        void        ExpandMargin(Real margin);
        bool        Intersect(BoundingBox* aabb) const;
        bool        Intersect(const BoundingBox& aabb) const;
        bool        Intersect(const Vector3& pos, Real radius) const;
        bool        Contains(const Vector3& point) const;
        bool        TestRayIntersection(const Ray& ray, Real& t, Real max_distance = -1.0f) const;
        bool        Contains(BoundingBox* aabb) const;
        bool        Contains(const BoundingBox& aabb) const;
        Real        Volume() const;
        Real        SurfaceArea() const;
        Vector3     Center() const;
        Vector3     Size() const;
        Vector3     HalfSize() const;
        Vector3     LowerBound() const;
        Vector3     UpperBound() const;
        BoundingBox Union(const BoundingBox& aabb) const;

        //getter
        void* GetObjectData() const;
        Sint32 GetNodeIndex() const;

        void UpdateVolume();
    private:
        friend class BroadPhaseNode;

    private:
        Vector3 m_lower_bound, m_upper_bound;

        void*  m_object_data = nullptr;
        Sint32 m_node_index  = -1;

        eBoundingObjectType m_type = eBoundingObjectType::None;
    };
}
