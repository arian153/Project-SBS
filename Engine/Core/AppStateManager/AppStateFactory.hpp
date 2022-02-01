#pragma once
#include "../../EngineDefine.hpp"

namespace Engine
{
    class AppState;
    class AppStateFactory
    {
    public:
        AppStateFactory()          = default;
        virtual ~AppStateFactory() = default;

        virtual AppState* Create() = 0;

    protected:
    };
}
