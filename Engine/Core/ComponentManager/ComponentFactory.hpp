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
