#pragma once
#include "BoundingBox.hpp"

namespace Engine
{
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

}
