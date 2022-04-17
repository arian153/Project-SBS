/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "Random.hpp"

namespace Engine
{
    Random::Random()
    {
        GenerateSeed();
    }

    Random::Random(size_t seed)
    {
        m_seed     = seed;
        m_random32 = std::mt19937(static_cast<Uint32>(seed));
        m_random64 = std::mt19937_64(seed);
    }

    Random::~Random()
    {
    }

    void Random::SetSeed(size_t seed)
    {
        m_seed     = seed;
        m_random32 = std::mt19937(static_cast<Uint32>(seed));
        m_random64 = std::mt19937_64(seed);
    }

    size_t Random::GetSeed() const
    {
        return m_seed;
    }

    size_t Random::GenerateSeed()
    {
        m_seed     = m_rng();
        m_random32 = std::mt19937(static_cast<Uint32>(m_seed));
        m_random64 = std::mt19937_64(m_seed);

        return m_seed;
    }

    unsigned Random::GetRandomUnsigned()
    {
        return m_random32();
    }

    int Random::GetRandomInteger()
    {
        return static_cast<int>(m_random32());
    }

    Real Random::GetRandomReal()
    {
        return static_cast<Real>(m_random32()) / static_cast<Real>(m_random32.max());
    }

    unsigned Random::GetRangedRandomUnsigned(unsigned min, unsigned max)
    {
        std::uniform_int_distribution dist(min, max);
        return dist(m_random32);
    }

    int Random::GetRangedRandomInteger(int min, int max)
    {
        std::uniform_int_distribution dist(min, max);
        return dist(m_random32);
    }

    Real Random::GetRangedRandomReal(Real min, Real max)
    {
        std::uniform_real_distribution dist(min, max);
        return dist(m_random32);
    }

    bool Random::IsNPercentSuccess(Real percent)
    {
        std::bernoulli_distribution dist(percent);
        return dist(m_random32);
    }

    size_t Random::NPercentSuccessMTimesResult(size_t m, Real percent)
    {
        std::binomial_distribution dist(m, percent);
        return dist(m_random32);
    }

    unsigned Random::GetNormalDistributionUnsigned(unsigned average, unsigned deviation)
    {
        std::normal_distribution<> dist(average, deviation);
        return static_cast<unsigned>(dist(m_random32));
    }

    int Random::GetNormalDistributionInteger(int average, int deviation)
    {
        std::normal_distribution<> dist(average, deviation);
        return static_cast<int>(dist(m_random32));
    }

    Real Random::GetNormalDistributionReal(Real average, Real deviation)
    {
        std::normal_distribution<> dist(average, deviation);
        return static_cast<Real>(dist(m_random32));
    }

    Uint32 Random::GetRandomU32()
    {
        return m_random32();
    }

    Uint64 Random::GetRandomU64()
    {
        return m_random64();
    }

    Sint32 Random::GetRandomI32()
    {
        return static_cast<Sint32>(m_random32());
    }

    Sint64 Random::GetRandomI64()
    {
        return static_cast<Sint64>(m_random64());
    }

    Real32 Random::GetRandomR32()
    {
        return static_cast<Real32>(m_random32()) / static_cast<Real32>(m_random32.max());
    }

    Real64 Random::GetRandomR64()
    {
        return static_cast<Real64>(m_random64()) / static_cast<Real64>(m_random64.max());
    }

    Uint32 Random::GetRangedRandomU32(Uint32 min, Uint32 max)
    {
        std::uniform_int_distribution dist(min, max);
        return dist(m_random32);
    }

    Uint64 Random::GetRangedRandomU64(Uint64 min, Uint64 max)
    {
        std::uniform_int_distribution dist(min, max);
        return dist(m_random64);
    }

    Sint32 Random::GetRangedRandomI32(Sint32 min, Sint32 max)
    {
        std::uniform_int_distribution dist(min, max);
        return dist(m_random32);
    }

    Sint64 Random::GetRangedRandomI64(Sint64 min, Sint64 max)
    {
        std::uniform_int_distribution dist(min, max);
        return dist(m_random64);
    }

    Real32 Random::GetRangedRandomR32(Real32 min, Real32 max)
    {
        std::uniform_real_distribution dist(min, max);
        return dist(m_random64);
    }

    Real64 Random::GetRangedRandomR64(Real64 min, Real64 max)
    {
        std::uniform_real_distribution dist(min, max);
        return dist(m_random64);
    }

    bool Random::IsNPercentSuccessR32(Real32 percent)
    {
        std::bernoulli_distribution dist(percent);
        return dist(m_random32);
    }

    bool Random::IsNPercentSuccessR64(Real64 percent)
    {
        std::bernoulli_distribution dist(percent);
        return dist(m_random64);
    }

    Uint32 Random::GetNormalDistributionU32(Uint32 average, Uint32 deviation)
    {
        std::normal_distribution<> dist(average, deviation);
        return static_cast<Uint32>(dist(m_random32));
    }


    Sint32 Random::GetNormalDistributionI32(Sint32 average, Sint32 deviation)
    {
        std::normal_distribution<> dist(average, deviation);
        return static_cast<Sint32>(dist(m_random32));
    }


    Real32 Random::GetNormalDistributionR32(Real32 average, Real32 deviation)
    {
        std::normal_distribution<> dist(average, deviation);
        return static_cast<Real32>(dist(m_random32));
    }

    Real64 Random::GetNormalDistributionR64(Real64 average, Real64 deviation)
    {
        std::normal_distribution dist(average, deviation);
        return dist(m_random64);
    }
}
