#pragma once
#include <random>
#include "MathDef.hpp"
#include "../../EngineDefine.hpp"

namespace Engine
{
    class Random
    {
    public:
        Random();
        Random(size_t seed);
        ~Random();

        void   SetSeed(size_t seed);
        size_t GetSeed() const;
        size_t GenerateSeed();

    public:
        unsigned GetRandomUnsigned();
        int      GetRandomInteger();
        Real     GetRandomReal();

        unsigned GetRangedRandomUnsigned(unsigned min, unsigned max);
        int      GetRangedRandomInteger(int min, int max);
        Real     GetRangedRandomReal(Real min, Real max);

        bool   IsNPercentSuccess(Real percent);
        size_t NPercentSuccessMTimesResult(size_t m, Real percent);

        unsigned GetNormalDistributionUnsigned(unsigned average, unsigned deviation);
        int      GetNormalDistributionInteger(int average, int deviation);
        Real     GetNormalDistributionReal(Real average, Real deviation);

    public:
        Uint32 GetRandomU32();
        Uint64 GetRandomU64();
        Sint32  GetRandomI32();
        Sint64  GetRandomI64();
        Real32 GetRandomR32();
        Real64 GetRandomR64();

        Uint32 GetRangedRandomU32(Uint32 min, Uint32 max);
        Uint64 GetRangedRandomU64(Uint64 min, Uint64 max);
        Sint32  GetRangedRandomI32(Sint32 min, Sint32 max);
        Sint64  GetRangedRandomI64(Sint64 min, Sint64 max);
        Real32 GetRangedRandomR32(Real32 min, Real32 max);
        Real64 GetRangedRandomR64(Real64 min, Real64 max);

        bool IsNPercentSuccessR32(Real32 percent);
        bool IsNPercentSuccessR64(Real64 percent);

        Uint32 GetNormalDistributionU32(Uint32 average, Uint32 deviation);
        Sint32  GetNormalDistributionI32(Sint32 average, Sint32 deviation);
        Real32 GetNormalDistributionR32(Real32 average, Real32 deviation);
        Real64 GetNormalDistributionR64(Real64 average, Real64 deviation);

    private:
        bool               m_b_64_bit = false;
        size_t             m_seed;
        std::mt19937       m_random32;
        std::mt19937_64    m_random64;
        std::random_device m_rng;
    };
}
