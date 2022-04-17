/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "Interpolation.hpp"

#include "Utility.hpp"

namespace Engine
{
    Real Lerp(Real start, Real end, Real t)
    {
        return (1.0f - t) * start + t * end;
    }

    Real Elerp(Real start, Real end, Real t)
    {
        return start * powf(end / start, t);
    }

    Vector3 Lerp(const Vector3& start, const Vector3& end, Real t)
    {
        return LinearInterpolation(start, end, t);
    }

    Quaternion Lerp(const Quaternion& start, const Quaternion& end, Real t)
    {
        return LinearInterpolation(start, end, t);
    }

    Quaternion Slerp(const Quaternion& start, const Quaternion& end, Real t)
    {
        return SphericalLinearInterpolation(start, end, t);
    }

    Quaternion Sterp(const Quaternion& start, const Quaternion& end, Real t, const Vector3& twist)
    {
        return SwingTwistInterpolation(start, end, twist, t);
    }
}
