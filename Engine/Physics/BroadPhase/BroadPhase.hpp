#pragma once
#include <vector>

#include "BoundingBox.hpp"
#include "../../EngineDefine.hpp"
#include "../../Graphics/Data/Color.hpp"
#include "../../Math/Primitive/ConvexHull3D/Box.hpp"

namespace Engine
{
    class PrimitiveRenderer;

    class PotentialPair
    {
    public:
        PotentialPair(BoundingBox* a, BoundingBox* b)
            : bv_a(a), bv_b(b)
        {
        }

    public:
        BoundingBox* bv_a = nullptr;
        BoundingBox* bv_b = nullptr;
    };

    class BroadPhaseNode
    {
    public:
        BroadPhaseNode();
        ~BroadPhaseNode();

        bool IsLeaf() const;
        void SetBranch(BroadPhaseNode* n0, BroadPhaseNode* n1);
        void SetLeaf(BoundingBox* bounding_aabb);
        void UpdateBox(Real margin);

        BroadPhaseNode* GetSibling() const;

    public:
        BroadPhaseNode* parent = nullptr;
        BroadPhaseNode* children[2];
        bool            children_crossed = false;
        BoundingBox     aabb;
        BoundingBox*    data = nullptr;
    };

    class BroadPhase
    {
    public:
        void Initialize();
        void Update(Real dt);
        void Shutdown();

        void Add(BoundingBox* aabb);
        void Remove(BoundingBox* aabb);
        void Clear();
        void Release();
        void Render(PrimitiveRenderer* primitive_renderer, const ColorFlag& broad_phase_color, const ColorFlag& primitive_color);
        void ComputePairs(std::vector<PotentialPair>& potential_pairs);

    private:
        void UpdateNodeRecursive(BroadPhaseNode* node, std::vector<BroadPhaseNode*>& invalid_nodes);
        void InsertNodeRecursive(BroadPhaseNode* node, BroadPhaseNode** parent) const;
        void RemoveNodeRecursive(BroadPhaseNode* node);
        void ClearNodeRecursive(BroadPhaseNode* node);
        void ReleaseNodeRecursive(BroadPhaseNode* node);
        void ShutdownNodeRecursive(BroadPhaseNode* node);
        void CopyNodeRecursive(BroadPhaseNode* node, BroadPhase* other);
        void RenderNodeRecursive(BroadPhaseNode* node, PrimitiveRenderer* primitive_renderer, const ColorFlag& broad_phase_color, const ColorFlag& primitive_color) const;
        void ComputePairsRecursive(BroadPhaseNode* n0, BroadPhaseNode* n1, std::vector<PotentialPair>& potential_pairs);
        void ClearChildrenCrossFlagRecursive(BroadPhaseNode* node);
        void CrossChildren(BroadPhaseNode* node, std::vector<PotentialPair>& potential_pairs);

    private:
        BroadPhaseNode*              m_root = nullptr;
        std::vector<BroadPhaseNode*> m_invalid_nodes;

        Box m_drawing_box;

    protected:
        std::vector<BoundingBox> m_bounding_boxes;
    };
}
