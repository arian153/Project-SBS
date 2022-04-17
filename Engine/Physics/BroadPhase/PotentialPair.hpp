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
