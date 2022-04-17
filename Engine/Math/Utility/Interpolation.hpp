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
#include "../Algebra/Quaternion.hpp"
#include "../Algebra/Vector3.hpp"

namespace Engine
{
    //scalar
    Real Lerp(Real start, Real end, Real t);
    Real Elerp(Real start, Real end, Real t);

    //vector
    Vector3 Lerp(const Vector3& start, const Vector3& end, Real t);

    //quaternion
    Quaternion Lerp(const Quaternion& start, const Quaternion& end, Real t);
    Quaternion Slerp(const Quaternion& start, const Quaternion& end, Real t);
    Quaternion Sterp(const Quaternion& start, const Quaternion& end, Real t, const Vector3& twist = Math::Vector3::UP);
}
