/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "UUIDUtility.hpp"

#include <random>

namespace
{
    std::random_device                    s_device;
    std::mt19937_64                       s_engine;
    std::uniform_int_distribution<size_t> s_distribution;
}

namespace Engine
{
    size_t UUIDUtility::Generate()
    {
        return s_distribution(s_engine);
    }
}
