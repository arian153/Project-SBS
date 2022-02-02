#include "Component.hpp"

#include "../../EngineDefine.hpp"

namespace Engine
{
    Component::~Component()
    {
    }

    String Component::TypeName() const
    {
        return m_type_name;
    }

    size_t Component::TypeID() const
    {
        return m_type_id;
    }

    SPtr<Object> Component::GetOwner() const
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

    void Component::SetOwner(SPtr<Object> owner)
    {
        m_owner = owner;
    }

    void Component::ClearOwner()
    {
        m_owner.reset();
    }
}
