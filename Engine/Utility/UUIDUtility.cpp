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
