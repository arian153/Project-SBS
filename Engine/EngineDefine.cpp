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
