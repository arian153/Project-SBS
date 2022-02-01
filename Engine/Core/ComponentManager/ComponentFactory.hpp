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

        virtual Component* Create(SPtr<Object> owner, AppState* space) = 0;
        virtual Component* Clone(Component* origin, SPtr<Object> dest, AppState* space) = 0;

    protected:
        friend class ComponentRegistry;
        String m_type;
    };
}
