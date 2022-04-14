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
