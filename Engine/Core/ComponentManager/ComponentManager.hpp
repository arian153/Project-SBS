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
