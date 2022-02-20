#pragma once
#include "../EngineDefine.hpp"

namespace Engine
{
    class AppState;

    class InAppEditor
    {
    public:

        void Update(Real dt);
        void SetAppState(AppState* app_state);
    
    private:
        AppState* m_app_state = nullptr;
    };
}
