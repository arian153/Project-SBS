#pragma once

#include "../../EngineDefine.hpp"

namespace Engine
{
    class ComponentFactory;

    class ComponentRegistry
    {
    public:
        static bool Initialize();
        static bool Shutdown();
        static bool AddFactory(ComponentFactory* factory);

    private:
        inline static HashMap<String, ComponentFactory*> m_factories_name;
        inline static HashMap<size_t, ComponentFactory*> m_factories_uuid;
        inline static std::vector<String>                m_keys;

        friend class ComponentManager;

    private:
    };
}
