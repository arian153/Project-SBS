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
        Component* Clone(Component* source, SPtr<Object> dest_object);
        void       Remove(Component* component);
        void       Remove(Component* component, SPtr<Object> owner);
        void       Remove(SPtr<Object> owner);
        void       Clear();

    private:
        HashMulMap<size_t, Component*> m_components; //object id - component map

        AppState* m_space = nullptr;
    };
}
