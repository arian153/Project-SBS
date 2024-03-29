/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "Utility.hpp"
#include "../Algebra/Vector3.hpp"
#include "../Algebra/Quaternion.hpp"
#include "../Structure/Vector3Pair.hpp"
#include "VectorDef.hpp"

namespace Engine::Math
{
    Real SmoothStart(Real t)
    {
        return t * t;
    }

    Real SmoothStop(Real t)
    {
        return 1.0f - ((1.0f - t) * (1.0f - t));
    }

    Vector3Pair GetTangentUsingQuaternion(const Engine::Vector3& normal)
    {
        Quaternion axis_to_normal(Math::Vector3::Y_AXIS, normal);
        return Vector3Pair(axis_to_normal.Rotate(Math::Vector3::X_AXIS), axis_to_normal.Rotate(Math::Vector3::Z_AXIS));
    }

    bool SolveQuadratic(Real a, Real b, Real c, Real& result1, Real& result2)
    {
        if (IsZero(a))
        {
            if (IsZero(b))
            {
                result1 = -c;
                result2 = -c;
            }
            else
            {
                result1 = -c / b;
                result2 = -c / b;
            }

            return false;
        }

        Real discriminant = (b * b) - (4.0f * a * c);

        if (discriminant > 0.0f)
        {
            result1 = (-b + sqrtf(discriminant)) / (2.0f * a);
            result2 = (-b - sqrtf(discriminant)) / (2.0f * a);
        }
        else if (IsZero(discriminant) == true)
        {
            Real result = -b / (2.0f * a);
            result1     = result;
            result2     = result;
        }
        else
        {
            Real real      = -b / (2.0f * a);
            Real imaginary = sqrtf(-discriminant) / (2.0f * a);
            result1        = real;
            result2        = imaginary;
            return false;
        }

        return true;
    }

    Vector3Pair GetTBPairFast(const Engine::Vector3& normal)
    {
        Vector3Pair result;

        Engine::Vector3 n = normal.Normalize();
        Engine::Vector3 t;

        if (n.x >= 0.57735f)
            t.Set(n.y, -n.x, 0.0f);
        else
            t.Set(0.0f, n.z, -n.y);

        result.a = t.Normalize();
        result.b = CrossProduct(n, result.a);

        return result;
    }

    Engine::Vector3 GetTangentFast(const Engine::Vector3& normal)
    {
        Engine::Vector3 n = normal.Normalize();
        Engine::Vector3 t;

        if (n.x >= 0.57735f)
            t.Set(n.y, -n.x, 0.0f);
        else
            t.Set(0.0f, n.z, -n.y);

        return t.Normalize();
    }
}
