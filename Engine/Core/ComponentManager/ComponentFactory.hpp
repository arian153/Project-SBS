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
        virtual Component* Create() = 0;

    protected:
        friend class ComponentRegistry;
        String m_type;
        size_t m_id = 0;
    };
}
