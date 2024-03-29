/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#pragma once
#include <vector>

#include "BoundingBox.hpp"
#include "PotentialPair.hpp"
#include "../../EngineDefine.hpp"
#include "../../Graphics/Data/Color.hpp"
#include "../../Math/Primitive/ConvexHull3D/Box.hpp"

namespace Engine
{
    class PrimitiveRenderer;

    static constexpr Real BROAD_PHASE_MARGIN = 0.2f;

  
    class BroadPhaseNode
    {
    public:
        BroadPhaseNode();
        ~BroadPhaseNode();
        bool            IsLeaf() const;
        void            SetBranch(BroadPhaseNode* n0, BroadPhaseNode* n1);
        void            SetLeaf(BoundingBox* bounding_aabb);
        void            UpdateBox(Real margin);
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
        BroadPhase();
        ~BroadPhase();

        void Initialize();
        void Update();
        void Shutdown();
        void Add(BoundingBox* aabb);
        void Remove(BoundingBox* aabb);
        void Clear();
        void Release();
        void Render(PrimitiveRenderer* primitive_renderer, const ColorFlag& broad_phase_color, const ColorFlag& primitive_color) const;
        void ComputePairs(std::vector<PotentialPair>& potential_pairs);

    private:
        void UpdateNodeRecursive(BroadPhaseNode* node, std::vector<BroadPhaseNode*>& invalid_nodes);
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
        BroadPhaseNode*              m_root = nullptr;
        std::vector<BroadPhaseNode*> m_invalid_nodes;
        Box                          m_drawing_box;
    };
}
