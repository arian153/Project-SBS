#pragma once
#include <vector>

#include "BoundingBox.hpp"

namespace Engine
{
    class BroadPhase
    {
    public:
        virtual      ~BroadPhase() = default;
        virtual void Update() = 0;
        virtual void ComputePair() = 0;
        

    protected:
        std::vector<BoundingBox> m_bounding_boxes;
    };
}
