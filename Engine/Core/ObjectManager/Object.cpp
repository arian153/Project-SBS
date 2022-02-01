#include "Object.hpp"
#include "ObjectManager.hpp"
#include "../ComponentManager/Component.hpp"
#include "../ComponentManager/ComponentManager.hpp"
#include "../ResourceManager/ResourceType/JsonData.hpp"

namespace Engine
{
    Object::Object()
    {
    }

    Object::~Object()
    {
    }

    size_t Object::GetID() const
    {
        return m_index;
    }

    String Object::GetName() const
    {
        return m_name;
    }

    void Object::SetName(const String& name)
    {
        if (m_name != name)
        {
            if (m_object_manager != nullptr)
            {
                m_object_manager->ChangeName(shared_from_this(), name);
            }
            else
            {
                m_name = name;
            }
        }
    }

    SPtr<Object> Object::Clone()
    {
        return m_object_manager->CloneObject(m_name, shared_from_this());
    }

    void Object::ClearComponents()
    {
        m_component_manager->Remove(shared_from_this());
        m_component_map.clear();
    }

    bool Object::Load(const Json::Value& data)
    {
        //Add Components
        if (JsonData::HasMember(data, "Components") && data["Components"].isArray())
        {
            for (auto it = data["Components"].begin(); it != data["Components"].end(); ++it)
            {
                //Load Components
                if (JsonData::HasMember(*it, "Type") && (*it)["Type"].isString())
                {
                    String     type    = (*it)["Type"].asString();
                    Component* created = AddComponent(type);
                    if (created != nullptr)
                    {
                        created->Load((*it)["Value"]);
                    }
                }
            }
        }
        return true;
    }

    bool Object::Save(Json::Value& data) const
    {
        data["Name"] = m_name;
        return true;
    }

    void Object::SetManager(ObjectManager* object_manager)
    {
        m_object_manager = object_manager;
    }

    void Object::SetManager(ComponentManager* component_manager)
    {
        m_component_manager = component_manager;
    }

    //Add component that have already been created
    Component* Object::AddComponent(Component* component)
    {
        auto type  = component->Type();
        auto found = m_component_map.find(type);
        if (found == m_component_map.end())
        {
            m_component_map.emplace(type, component);
            m_components.push_back(component);
            component->Initialize();
            return component;
        }
        return nullptr;
    }

    //Create and add new component
    Component* Object::AddComponent(const String& type)
    {
        auto found = m_component_map.find(type);
        if (found == m_component_map.end())
        {
            auto created = m_component_manager->Create(type, shared_from_this());
            m_component_map.emplace(type, created);
            m_components.push_back(created);
            created->Initialize();
            return created;
        }
        return nullptr;
    }

    Component* Object::GetComponent(const String& type)
    {
        auto found = m_component_map.find(type);
        if (found != m_component_map.end())
        {
            return found->second;
        }
        return nullptr;
    }

    bool Object::HasComponent(const String& type)
    {
        auto found = m_component_map.find(type);
        if (found != m_component_map.end())
        {
            return true;
        }
        return false;
    }

    void Object::RemoveComponent(Component* component)
    {
        auto type  = component->Type();
        auto found = m_component_map.find(type);
        if (found != m_component_map.end())
        {
            m_components.erase(std::find(m_components.begin(), m_components.end(), component));
            m_component_manager->Remove(component, shared_from_this());
            m_component_map.erase(found);
        }
    }

    void Object::RemoveComponent(const String& type)
    {
        auto found = m_component_map.find(type);
        if (found != m_component_map.end())
        {
            m_components.erase(std::find(m_components.begin(), m_components.end(), found->second));
            m_component_manager->Remove(found->second, shared_from_this());
            m_component_map.erase(found);
        }
    }

    bool Object::CloneComponents(SPtr<Object> cloned_object, ComponentManager* manager) const
    {
        for (const auto& [fst, snd] : m_component_map)
        {
            cloned_object->AddComponent(manager->Clone(snd, cloned_object));
        }
        return true;
    }
}
