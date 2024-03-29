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
#include "../Algebra/Vector3.hpp"

namespace Engine
{
    class Vector3Pair
    {
    public:
        explicit Vector3Pair(const Vector3& a = Vector3(), const Vector3& b = Vector3());
        Vector3Pair(const Vector3Pair& rhs);
        Vector3Pair& operator=(const Vector3Pair& rhs);
    public:
        Vector3 a;
        Vector3 b;
    };
}
