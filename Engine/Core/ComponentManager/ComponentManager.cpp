#include "ComponentManager.hpp"
#include "Component.hpp"
#include "ComponentRegistry.hpp"
#include "ComponentFactory.hpp"
#include "../ObjectManager/Object.hpp"

namespace Engine
{
    ComponentManager::ComponentManager()
    {
    }

    ComponentManager::~ComponentManager()
    {
    }

    void ComponentManager::Initialize(AppState* space)
    {
        m_space = space;
    }

    void ComponentManager::Shutdown()
    {
        Clear();
    }

    Component* ComponentManager::Create(const String& type, SPtr<Object> owner)
    {
        auto found = ComponentRegistry::m_factories.find(type);
        if (found != ComponentRegistry::m_factories.end())
        {
            auto created = found->second->Create(owner, m_space);
            m_components.emplace(owner->m_uuid, created);
            return created;
        }
        return nullptr;
    }

    Component* ComponentManager::Clone(Component* origin, SPtr<Object> dest)
    {
        auto found = ComponentRegistry::m_factories.find(origin->m_type);
        if (found != ComponentRegistry::m_factories.end())
        {
            auto cloned = found->second->Clone(origin, dest, m_space);
            m_components.emplace(dest->m_uuid, cloned);
            return cloned;
        }
        return nullptr;
    }

    void ComponentManager::Remove(Component* component)
    {
        auto [fst, snd] = m_components.equal_range(component->m_owner->m_uuid);
        for (auto& it = fst; it != snd; ++it)
        {
            if (it->second == component)
            {
                it->second->Shutdown();
                delete it->second;
                it->second = nullptr;
                m_components.erase(it);
                break;
            }
        }
    }

    void ComponentManager::Remove(Component* component, SPtr<Object> owner)
    {
        if (component->m_owner->m_uuid == owner->m_uuid)
        {
            auto [fst, snd] = m_components.equal_range(owner->m_uuid);
            for (auto& it = fst; it != snd; ++it)
            {
                if (it->second == component)
                {
                    it->second->Shutdown();
                    delete it->second;
                    it->second = nullptr;
                    m_components.erase(it);
                    break;
                }
            }
        }
    }

    void ComponentManager::Remove(SPtr<Object> owner)
    {
        auto [fst, snd] = m_components.equal_range(owner->m_uuid);
        for (auto& it = fst; it != snd;)
        {
            if (it->second != nullptr)
            {
                it->second->Shutdown();
                delete it->second;
                it->second = nullptr;
            }
            m_components.erase(it++);
        }
    }

    void ComponentManager::Clear()
    {
        for (auto& [uuid, component] : m_components)
        {
            if (component != nullptr)
            {
                component->Shutdown();
                delete component;
                component = nullptr;
            }
        }
        m_components.clear();
    }
}
