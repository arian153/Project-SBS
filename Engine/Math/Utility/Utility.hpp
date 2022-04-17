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
#include "MathDef.hpp"

namespace Engine
{
    class Vector3;
    class Vector3Pair;
}

namespace Engine::Math
{
    Real        SmoothStart(Real t);
    Real        SmoothStop(Real t);
    Vector3Pair GetTangentUsingQuaternion(const Engine::Vector3& normal);
    bool        SolveQuadratic(Real a, Real b, Real c, Real& result1, Real& result2);

    Vector3Pair GetTBPairFast(const Engine::Vector3& normal);

    Engine::Vector3 GetTangentFast(const Engine::Vector3& normal);

    template <typename T>
    T Min(const T& a, const T& b)
    {
        return a < b ? a : b;
    }

    template <typename T>
    T Max(const T& a, const T& b)
    {
        return a > b ? a : b;
    }

    template <typename T>
    T Clamp(const T& x, const T& low, const T& high)
    {
        return x < low ? low : (x > high ? high : x);
    }

    inline bool IsValid(Real rhs)
    {
        return !isnan(rhs) && isfinite(rhs);
    }

    inline bool IsInfinite(Real rhs)
    {
        return !isfinite(rhs);
    }

    inline bool IsZero(Real rhs)
    {
        return (fabsf(rhs) < Math::EPSILON);
    }

    inline bool IsEqual(Real a, Real b)
    {
        return (fabsf(a - b) < Math::EPSILON);
    }

    inline bool IsNotEqual(Real a, Real b)
    {
        return !(fabsf(a - b) < Math::EPSILON);
    }

    inline bool IsLess(Real a, Real b)
    {
        return ((a + Math::EPSILON) < (b - Math::EPSILON));
    }

    inline bool IsGreator(Real a, Real b)
    {
        return ((a - Math::EPSILON) > (b + Math::EPSILON));
    }

    inline bool IsGreatorThanZero(Real a)
    {
        return (a >= Math::EPSILON);
    }

    inline Real InvSqrt(Real rhs)
    {
        return 1.f / sqrtf(rhs);
    }

    inline Real Signum(Real param)
    {
        if (IsZero(param))
        {
            return 0.0f;
        }
        if (param >= Math::EPSILON)
        {
            return +1.0f;
        }
        return -1.0f;
    }

    inline int Round(Real param)
    {
        return static_cast<int>(floorf(param + 0.5f));
    }

    inline Real DegreesToRadians(Real degrees)
    {
        return (degrees * Math::RADIAN);
    }

    inline Real RadiansToDegrees(Real radians)
    {
        return (radians / Math::RADIAN);
    }

    inline Real XyToRadian(Real x, Real y)
    {
        Real theta = atan2f(y, x);;
        if (theta > Math::PI)
            theta = theta + Math::TWO_PI;
        if (theta > Math::TWO_PI)
            theta -= Math::TWO_PI;
        if (theta < 0.0f)
            theta += Math::TWO_PI;
        return theta;
    }

    inline Real XyToDegree(Real x, Real y)
    {
        Real theta = atan2f(y, x);;
        if (theta > Math::PI)
            theta = theta + Math::TWO_PI;
        if (theta > Math::TWO_PI)
            theta -= Math::TWO_PI;
        if (theta < 0.0f)
            theta += Math::TWO_PI;
        return (theta / Math::RADIAN);
    }

    inline Real ClearError(Real value, Real digit = Math::EPSILON)
    {
        return std::round(value / digit) * digit;
    }

    template <typename T>
    void Swap(T& a, T& b)
    {
        T temp = a;
        a      = b;
        b      = temp;
    }
}
