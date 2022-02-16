#pragma once
#include <vector>

#include "BoundingBox.hpp"
#include "../../EngineDefine.hpp"

namespace Engine
{
    class BroadPhase
    {
    public:
        void Initialize();
        void Update(Real dt);
        void Shutdown();
        void Copy(BroadPhase* other);
        void Add(BoundingAABB* aabb);
        void Remove(BoundingAABB* aabb);
        void Clear();
        void Release() override;
        void Render(PrimitiveRenderer* primitive_renderer, const ColorFlag& broad_phase_color, const ColorFlag& primitive_color);
        void ComputePairs(PotentialPairs& potential_pairs);

        //Query


        void Query(const BoundingAABB& aabb, std::vector<ColliderPrimitive*>& output) const;
        void CastRay(RayCastResult& result, Real max_distance = -1.0f) const;
        void IntersectRay(RayIntersectionResult& result, Real max_distance = -1.0f) const;
        void TraceRay(RayTraceResult& result, Real max_distance = -1.0f, size_t reflect_count = 1) const;

    private:
        void UpdateNodeRecursive(DynamicBVHNode* node, std::vector<DynamicBVHNode*>& invalid_nodes);
        void InsertNodeRecursive(DynamicBVHNode* node, DynamicBVHNode** parent) const;
        void RemoveNodeRecursive(DynamicBVHNode* node);
        void ClearNodeRecursive(DynamicBVHNode* node);
        void ReleaseNodeRecursive(DynamicBVHNode* node);
        void ShutdownNodeRecursive(DynamicBVHNode* node);
        void CopyNodeRecursive(DynamicBVHNode* node, BroadPhase* other);
        void RenderNodeRecursive(DynamicBVHNode* node, PrimitiveRenderer* primitive_renderer, const ColorFlag& broad_phase_color, const ColorFlag& primitive_color) const;
        void ComputePairsRecursive(DynamicBVHNode* n0, DynamicBVHNode* n1, PotentialPairs& potential_pairs);
        void ClearChildrenCrossFlagRecursive(DynamicBVHNode* node);
        void CrossChildren(DynamicBVHNode* node, PotentialPairs& potential_pairs);

    private:
        DynamicBVHNode*              m_root = nullptr;
        std::vector<DynamicBVHNode*> m_invalid_nodes;
        Box                          m_drawing_box;

    protected:
        std::vector<BoundingBox> m_bounding_boxes;
    };
}
