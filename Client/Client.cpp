// ReSharper disable CppInconsistentNaming
#include <crtdbg.h>
#include <Engine.h>
#include <windows.h>
#include <Core/ResourceManager/ResourceType/JsonData.hpp>

#include "Level/TestSquare.h"
#include "Level/TestTriangle.h"

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

    Engine::g_core->GetAppStateManager()->AddFactory("TestTriangle", new Client::TestTriangleFactory());
    Engine::g_core->GetAppStateManager()->AddFactory("TestSquare", new Client::TestSquareFactory());
    Engine::g_core->GetAppStateManager()->SetInitialState("TestSquare");

    Engine::g_core->Update();
    Engine::g_core->Shutdown();

    return 0;
}
