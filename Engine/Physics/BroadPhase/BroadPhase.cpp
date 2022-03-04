#include "BroadPhase.hpp"

namespace Engine
{
    BroadPhaseNode::BroadPhaseNode()
    {
        children[0] = nullptr;
        children[1] = nullptr;
    }

    BroadPhaseNode::~BroadPhaseNode()
    {
    }

    bool BroadPhaseNode::IsLeaf() const
    {
        return children[0] == nullptr;
    }

    void BroadPhaseNode::SetBranch(BroadPhaseNode* n0, BroadPhaseNode* n1)
    {
        n0->parent  = this;
        n1->parent  = this;
        children[0] = n0;
        children[1] = n1;
    }

    void BroadPhaseNode::SetLeaf(BoundingBox* bounding_aabb)
    {
        // create two-way link
        this->data                 = bounding_aabb;
        bounding_aabb->m_node_data = this;
        children[0]                = nullptr;
        children[1]                = nullptr;
    }

    void BroadPhaseNode::UpdateBox(Real margin)
    {
        if (this->IsLeaf() == true)
        {
            // make fat AABB
            Vector3 margin_vector(margin, margin, margin);
            aabb.m_lower_bound = data->m_lower_bound - margin_vector;
            aabb.m_upper_bound = data->m_upper_bound + margin_vector;
        }
        else
        {
            // make union of child AABBs of child nodes
            aabb = children[0]->aabb.Union(children[1]->aabb);
        }
    }

    BroadPhaseNode* BroadPhaseNode::GetSibling() const
    {
        return this == parent->children[0] ? parent->children[1] : parent->children[0];
    }

    BroadPhase::BroadPhase()
    {
    }

    BroadPhase::~BroadPhase()
    {
    }

    void BroadPhase::Initialize()
    {
        m_drawing_box.SetUnit();
    }

    void BroadPhase::Update(Real dt)
    {
        if (m_root)
        {
            if (m_root->IsLeaf())
            {
                if (m_root->data != nullptr)
                {
                    m_root->data->UpdateVolume();
                }
                m_root->UpdateBox(BROAD_PHASE_MARGIN);
            }
            else
            {
                // grab all invalid nodes
                m_invalid_nodes.clear();
                UpdateNodeRecursive(m_root, m_invalid_nodes);
                // re-insert all invalid nodes
                for (BroadPhaseNode* node : m_invalid_nodes)
                {
                    // grab parent link
                    // (pointer to the pointer that points to parent)
                    BroadPhaseNode*  parent      = node->parent;
                    BroadPhaseNode*  sibling     = node->GetSibling();
                    BroadPhaseNode** parent_link = parent->parent
                                                       ? (parent == parent->parent->children[0]
                                                              ? &parent->parent->children[0]
                                                              : &parent->parent->children[1])
                                                       : &m_root;
                    // replace parent with sibling
                    // root has null parent
                    sibling->parent = parent->parent ? parent->parent : nullptr;
                    *parent_link    = sibling;
                    delete parent;
                    // re-insert node
                    node->UpdateBox(BROAD_PHASE_MARGIN);
                    InsertNodeRecursive(node, &m_root);
                }
                m_invalid_nodes.clear();
            }
        }
    }

    void BroadPhase::Shutdown()
    {
        ShutdownNodeRecursive(m_root);
    }

    void BroadPhase::Add(BoundingBox* aabb)
    {
        if (m_root != nullptr)
        {
            // not first node, insert node to tree
            BroadPhaseNode* node = new BroadPhaseNode();
            node->SetLeaf(aabb);
            node->UpdateBox(BROAD_PHASE_MARGIN);
            InsertNodeRecursive(node, &m_root);
        }
        else
        {
            // first node, make root
            m_root = new BroadPhaseNode();
            m_root->SetLeaf(aabb);
            m_root->UpdateBox(BROAD_PHASE_MARGIN);
        }
    }

    void BroadPhase::Remove(BoundingBox* aabb)
    {
        BroadPhaseNode* node = aabb->GetNodeData();
        // remove two-way link
        node->data        = nullptr;
        aabb->m_node_data = nullptr;
        RemoveNodeRecursive(node);
    }

    void BroadPhase::Clear()
    {
        ClearNodeRecursive(m_root);
    }

    void BroadPhase::Release()
    {
        ReleaseNodeRecursive(m_root);
    }

    void BroadPhase::Render(PrimitiveRenderer* primitive_renderer, const ColorFlag& broad_phase_color, const ColorFlag& primitive_color) const
    {
        RenderNodeRecursive(m_root, primitive_renderer, broad_phase_color, primitive_color);
    }

    void BroadPhase::ComputePairs(std::vector<PotentialPair>& potential_pairs)
    {
        potential_pairs.clear();
        if (m_root == nullptr || m_root->IsLeaf())
            return;
        // clear Node::childrenCrossed flags
        ClearChildrenCrossFlagRecursive(m_root);
        // base recursive call
        ComputePairsRecursive(m_root->children[0], m_root->children[1], potential_pairs);
    }

    void BroadPhase::UpdateNodeRecursive(BroadPhaseNode* node, std::vector<BroadPhaseNode*>& invalid_nodes)
    {
        if (node->IsLeaf())
        {
            if (node->data != nullptr)
            {
                node->data->UpdateVolume();
            }
            // check if fat AABB doesn't contain the collider AABB anymore
            if (node->aabb.Contains(node->data) == false)
            {
                invalid_nodes.push_back(node);
            }
        }
        else
        {
            UpdateNodeRecursive(node->children[0], invalid_nodes);
            UpdateNodeRecursive(node->children[1], invalid_nodes);
        }
    }

    void BroadPhase::InsertNodeRecursive(BroadPhaseNode* node, BroadPhaseNode** parent) const
    {
        BroadPhaseNode* p = *parent;
        if (p->IsLeaf() == true)
        {
            // parent is leaf, simply split
            BroadPhaseNode* new_parent = new BroadPhaseNode();
            new_parent->parent         = p->parent;
            new_parent->SetBranch(node, p);
            *parent = new_parent;
        }
        else
        {
            // parent is branch, compute volume differences between pre-insert and post-insert
            BoundingBox* aabb0        = &p->children[0]->aabb;
            BoundingBox* aabb1        = &p->children[1]->aabb;
            Real         volume_diff0 = aabb0->Union(node->aabb).Volume() - aabb0->Volume();
            Real         volume_diff1 = aabb1->Union(node->aabb).Volume() - aabb1->Volume();
            // insert to the child that gives less volume increase
            if (volume_diff0 < volume_diff1)
            {
                InsertNodeRecursive(node, &p->children[0]);
            }
            else
            {
                InsertNodeRecursive(node, &p->children[1]);
            }
        }
        // update parent AABB (propagates back up the recursion stack)
        (*parent)->UpdateBox(BROAD_PHASE_MARGIN);
    }

    void BroadPhase::RemoveNodeRecursive(BroadPhaseNode* node)
    {
        // replace parent with sibling, remove parent node
        if (BroadPhaseNode* parent = node->parent) // node is not root
        {
            if (parent->parent) // if there's a grandparent
            {
                // update links
                BroadPhaseNode* sibling = node->GetSibling();
                sibling->parent         = parent->parent;
                (parent == parent->parent->children[0]
                     ? parent->parent->children[0]
                     : parent->parent->children[1]) = sibling;
            }
            else // no grandparent
            {
                // make sibling root
                BroadPhaseNode* sibling = node->GetSibling();
                m_root                  = sibling;
                sibling->parent         = nullptr;
            }
            delete node;
            delete parent;
        }
        else // node is root
        {
            m_root = nullptr;
            delete node;
        }
    }

    void BroadPhase::ClearNodeRecursive(BroadPhaseNode* node)
    {
        if (node != nullptr)
        {
            if (node->children[0] != nullptr)
            {
                ClearNodeRecursive(node->children[0]);
            }
            if (node->children[1] != nullptr)
            {
                ClearNodeRecursive(node->children[1]);
            }
            delete node;
            node = nullptr;
        }
    }

    void BroadPhase::ReleaseNodeRecursive(BroadPhaseNode* node)
    {
        if (node != nullptr)
        {
            if (node->children[0] != nullptr)
            {
                ReleaseNodeRecursive(node->children[0]);
            }
            if (node->children[1] != nullptr)
            {
                ReleaseNodeRecursive(node->children[1]);
            }
            if (node->data != nullptr)
            {
                node->data->m_node_data = nullptr;
                delete node->data;
                node->data = nullptr;
            }
        }
    }

    void BroadPhase::ShutdownNodeRecursive(BroadPhaseNode* node)
    {
        if (node != nullptr)
        {
            if (node->children[0] != nullptr)
            {
                ShutdownNodeRecursive(node->children[0]);
            }
            if (node->children[1] != nullptr)
            {
                ShutdownNodeRecursive(node->children[1]);
            }
            if (node->data != nullptr)
            {
                node->data->m_node_data = nullptr;
                delete node->data;
                node->data = nullptr;
            }
            delete node;
            node = nullptr;
        }
    }

    void BroadPhase::RenderNodeRecursive(BroadPhaseNode* node, PrimitiveRenderer* primitive_renderer, const ColorFlag& broad_phase_color, const ColorFlag& primitive_color) const
    {
        if (node != nullptr)
        {
            if (node->IsLeaf() == true)
            {
                if (node->data != nullptr)
                {
                    /*if (node->data->GetCollider() != nullptr && primitive_color.b_flag)
                    {
                        if (node->data->GetCollider()->GetRigidBody()->IsSleep())
                        {
                            node->data->GetCollider()->Draw(primitive_renderer, eRenderingMode::Line, Color(0.23f, 0.23f, 0.23f, 1.0f));
                        }
                        else
                        {
                            node->data->GetCollider()->Draw(primitive_renderer, eRenderingMode::Line, primitive_color.color);
                        }
                    }*/
                }
            }
            if (broad_phase_color.b_flag)
            {
                //primitive_renderer->DrawPrimitiveInstancing(m_drawing_box, node->aabb.Center(), node->aabb.Size(), eRenderingMode::Line, broad_phase_color.color);
                //primitive_renderer->DrawBox(node->aabb.Center(), Quaternion(), node->aabb.Size(), eRenderingMode::Line, broad_phase_color.color);
            }
            if (node->children[0] != nullptr)
            {
                RenderNodeRecursive(node->children[0], primitive_renderer, broad_phase_color, primitive_color);
            }
            if (node->children[1] != nullptr)
            {
                RenderNodeRecursive(node->children[1], primitive_renderer, broad_phase_color, primitive_color);
            }
        }
    }

    void BroadPhase::ComputePairsRecursive(BroadPhaseNode* n0, BroadPhaseNode* n1, std::vector<PotentialPair>& potential_pairs)
    {
        if (n0->IsLeaf() == true)
        {
            // 2 leaves, check proxies instead of fat AABBs
            if (n1->IsLeaf() == true)
            {
                if (n0->data->Intersect(n1->data) == true)
                {
                    potential_pairs.emplace_back(n0->data, n1->data);
                }
            }
            else // 1 branch / 1 leaf, 2 cross checks
            {
                CrossChildren(n1, potential_pairs);
                ComputePairsRecursive(n0, n1->children[0], potential_pairs);
                ComputePairsRecursive(n0, n1->children[1], potential_pairs);
            }
        }
        else
        {
            // 1 branch / 1 leaf, 2 cross checks
            if (n1->IsLeaf() == true)
            {
                CrossChildren(n0, potential_pairs);
                ComputePairsRecursive(n0->children[0], n1, potential_pairs);
                ComputePairsRecursive(n0->children[1], n1, potential_pairs);
            }
            else // 2 branches, 4 cross checks
            {
                CrossChildren(n0, potential_pairs);
                CrossChildren(n1, potential_pairs);
                ComputePairsRecursive(n0->children[0], n1->children[0], potential_pairs);
                ComputePairsRecursive(n0->children[0], n1->children[1], potential_pairs);
                ComputePairsRecursive(n0->children[1], n1->children[0], potential_pairs);
                ComputePairsRecursive(n0->children[1], n1->children[1], potential_pairs);
            }
        } // end of if (n0->IsLeaf())
    }

    void BroadPhase::ClearChildrenCrossFlagRecursive(BroadPhaseNode* node)
    {
        node->children_crossed = false;
        if (node->IsLeaf() == false)
        {
            ClearChildrenCrossFlagRecursive(node->children[0]);
            ClearChildrenCrossFlagRecursive(node->children[1]);
        }
    }

    void BroadPhase::CrossChildren(BroadPhaseNode* node, std::vector<PotentialPair>& potential_pairs)
    {
        if (node->children_crossed == false)
        {
            ComputePairsRecursive(node->children[0], node->children[1], potential_pairs);
            node->children_crossed = true;
        }
    }
}
