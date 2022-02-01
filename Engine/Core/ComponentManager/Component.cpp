#include "Component.hpp"

#include "../../EngineDefine.hpp"

namespace Engine
{
    Component::Component(Object* owner)
        : m_owner(owner)
    {
    }

    Component::~Component()
    {
    }

    String Component::Type() const
    {
        return m_type;
    }

    const char* Component::TypeCStr() const
    {
        return m_type.c_str();
    }

    bool Component::IsLoaded() const
    {
        return m_b_loaded;
    }

    bool Component::IsUnloaded() const
    {
        return m_b_unloaded;
    }

    Object* Component::GetOwner() const
    {
        return m_owner;
    }

    AppState* Component::GetSpace() const
    {
        return m_space;
    }

    void Component::SetSpace(AppState* space)
    {
        m_space = space;
    }
}
