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

    RPtr<Component> ComponentManager::Create(const String& type, SPtr<Object> owner)
    {
        auto found = ComponentRegistry::m_factories_name.find(type);
        if (found != ComponentRegistry::m_factories_name.end())
        {
            auto created         = found->second->Create();
            created->m_type_id   = found->second->m_type_id;
            created->m_type_name = found->second->m_type_name;
            created->SetOwner(owner);
            created->SetSpace(m_space);
            m_components.emplace(owner->m_uuid, created);
            return created;
        }
        return nullptr;
    }

    RPtr<Component> ComponentManager::Clone(RPtr<Component> source, SPtr<Object> dest_object)
    {
        auto found = ComponentRegistry::m_factories_name.find(source->m_type_name);
        if (found != ComponentRegistry::m_factories_name.end())
        {
            auto cloned = found->second->Create();
            cloned->SetOwner(dest_object);
            cloned->SetSpace(m_space);
            source->CloneTo(cloned);
            m_components.emplace(dest_object->m_uuid, cloned);
            return cloned;
        }
        return nullptr;
    }

    void ComponentManager::Remove(RPtr<Component> component)
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

    void ComponentManager::Remove(RPtr<Component> component, SPtr<Object> owner)
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
            }
        }
        m_components.clear();
    }
}
