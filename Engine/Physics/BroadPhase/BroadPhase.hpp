#pragma once
#include <vector>

#include "BoundingBox.hpp"
#include "../../EngineDefine.hpp"
#include "../../Graphics/Data/Color.hpp"
#include "../../Math/Primitive/ConvexHull3D/Box.hpp"

namespace Engine
{
    class PrimitiveRenderer;

    static constexpr Real BROAD_PHASE_MARGIN = 0.2f;

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
        void SetBranch(BroadPhaseNode& n0, BroadPhaseNode& n1);
        void SetLeaf(std::vector<BoundingBox>& objects, Sint32 index);
        void UpdateBox(Real margin, std::vector<BoundingBox>& objects, std::vector<BroadPhaseNode>& nodes);

        Sint32 GetSibling(std::vector<BroadPhaseNode>& nodes) const;

    public:
        BoundingBox aabb;

        //to array based tree.
        Sint32 node_index       = -1;
        Sint32 object_index     = -1;
        Sint32 parent_index     = -1;
        Sint32 child1           = -1;
        Sint32 child2           = -1;
        bool   children_crossed = false;
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

        void MakeNode();

    private:
        void UpdateNodeRecursive(BroadPhaseNode* node, std::vector<BroadPhaseNode*>& invalid_nodes);

        void InsertNode(Sint32 node_index, Sint32 parent_index);

        void InsertNodeRecursive(BroadPhaseNode* node, BroadPhaseNode** parent) const;
        void RemoveNodeRecursive(BroadPhaseNode* node);
        void ClearNodeRecursive(BroadPhaseNode* node);
        void ReleaseNodeRecursive(BroadPhaseNode* node);
        void ShutdownNodeRecursive(BroadPhaseNode* node);
        void RenderNodeRecursive(BroadPhaseNode* node, PrimitiveRenderer* primitive_renderer, const ColorFlag& broad_phase_color, const ColorFlag& primitive_color) const;
        void ComputePairsRecursive(BroadPhaseNode* n0, BroadPhaseNode* n1, std::vector<PotentialPair>& potential_pairs);
        void ClearChildrenCrossFlagRecursive(BroadPhaseNode* node);
        void CrossChildren(BroadPhaseNode* node, std::vector<PotentialPair>& potential_pairs);

    private:
        std::vector<BroadPhaseNode&> m_invalid_nodes;

        Sint32 m_root_index = -1;
        Box    m_drawing_box;

    protected:
        std::vector<BroadPhaseNode> m_nodes;
        std::vector<BoundingBox>    m_objects;
    };
}
