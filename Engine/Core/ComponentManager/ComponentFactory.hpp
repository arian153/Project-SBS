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
#include <string>

namespace Engine
{
    class Component;
    class Object;
    class AppState;

    class ComponentFactory
    {
    public:
        virtual ~ComponentFactory()
        {
        }

        virtual RPtr<Component> Create() = 0;

    protected:
        friend class ComponentRegistry;
        friend class ComponentManager;

    protected:
        String m_type_name;
        size_t m_type_id = 0;
    };
}
