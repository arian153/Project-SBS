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
#include <unordered_map>

#include "../../EngineDefine.hpp"

namespace Engine
{
    class AppState;
    class Object;
    class Component;

    class ComponentManager
    {
    public:
        ComponentManager();
        ~ComponentManager();

        void Initialize(AppState* space);
        void Shutdown();

        RPtr<Component> Create(const String& type, SPtr<Object> owner);
        RPtr<Component> Clone(RPtr<Component> source, SPtr<Object> dest_object);

        void Remove(RPtr<Component> component);
        void Remove(RPtr<Component> component, SPtr<Object> owner);
        void Remove(SPtr<Object> owner);
        void Clear();

    private:
        HashMulMap<size_t, RPtr<Component>> m_components; //object id - component map

        AppState* m_space = nullptr;
    };
}
