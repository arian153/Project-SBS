/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "EngineDefine.hpp"

#include "Core/CoreSystem.hpp"

namespace Engine
{
    std::unique_ptr<CoreSystem> g_core = std::make_unique<CoreSystem>();

    CoreSystem* GetCore()
    {
        return g_core.get();
    }
}
