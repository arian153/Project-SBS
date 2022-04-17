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
#include "../EngineDefine.hpp"

#include "CoreSystem.hpp"

#include "AppStateManager/AppStateManager.hpp"
#include "Win32Manager/InputManager.hpp"
#include "Win32Manager/Win32Manager.hpp"

namespace Engine
{
    #define INPUT_MANAGER g_core->GetInputManager()
    #define WIN32_MANAGER g_core->GetWin32Manager()
    #define APP_STATE_MANAGER g_core->GetAppStateManager()
    #define RESOURCE_MANAGER g_core->GetResourceManager()

}
