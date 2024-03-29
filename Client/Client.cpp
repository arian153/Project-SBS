/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include <crtdbg.h>
#include <Engine.h>
#include <windows.h>
#include <Core/ResourceManager/ResourceType/JsonData.hpp>

#include "Level/Splash.h"
#include "Level/TestBox.h"
#include "Level/TestSkeletal.h"
#include "Level/TestSquare.h"
#include "Level/TestSphere.h"

int APIENTRY wWinMain(_In_ [[maybe_unused]] HINSTANCE     hInstance,
                      _In_opt_ [[maybe_unused]] HINSTANCE hPrevInstance,
                      _In_ [[maybe_unused]] LPWSTR        lpCmdLine,
                      _In_ [[maybe_unused]] int           nShowCmd)
{
    //Leak check
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetBreakAlloc(-1);

    Engine::g_core->SetInstance(hInstance);
    Engine::g_core->Initialize();

    Engine::g_core->GetAppStateManager()->AddFactory("TestSphere", new Client::TestSphereFactory());
    Engine::g_core->GetAppStateManager()->AddFactory("TestSquare", new Client::TestSquareFactory());
    Engine::g_core->GetAppStateManager()->AddFactory("TestBox", new Client::TestBoxFactory());
    Engine::g_core->GetAppStateManager()->AddFactory("Splash", new Client::SplashFactory());
    Engine::g_core->GetAppStateManager()->AddFactory("TestSkeletal", new Client::TestSkeletalFactory());
    Engine::g_core->GetAppStateManager()->SetInitialState("TestSkeletal");

    Engine::g_core->Update();
    Engine::g_core->Shutdown();

    return 0;
}
