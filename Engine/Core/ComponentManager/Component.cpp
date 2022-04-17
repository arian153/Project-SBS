/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

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
