// ReSharper disable CppUnusedIncludeDirective
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
