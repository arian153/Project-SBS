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

namespace Engine
{
    class Object;
    class AppState;

    class InAppEditor
    {
    public:
        void Update(Real dt);
        void SetAppState(AppState* app_state);
        void Reset();

    private:
        AppState* m_app_state = nullptr;
        Object*   m_object    = nullptr;

        int m_object_index = 0;
    };
}
