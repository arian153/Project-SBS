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
