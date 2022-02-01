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

        Component* Create(const String& type, SPtr<Object> owner);
        Component* Clone(Component* origin, SPtr<Object> dest);
        void       Remove(Component* component);
        void       Remove(Component* component, SPtr<Object> owner);
        void       Remove(SPtr<Object> owner);
        void       Clear();

    private:
        std::unordered_multimap<size_t, Component*> m_components;

        AppState* m_space = nullptr;
    };
}
