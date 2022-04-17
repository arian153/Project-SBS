/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "ObjectManager.hpp"
#include "Object.hpp"
#include "../../Utility/UUIDUtility.hpp"
#include "../AppStateManager/AppState.hpp"
#include "../ComponentManager/ComponentManager.hpp"

namespace Engine
{
    ObjectManager::ObjectManager()
    {
    }

    ObjectManager::~ObjectManager()
    {
    }

    void ObjectManager::Initialize(AppState* space)
    {
        m_space = space;
    }

    void ObjectManager::Shutdown()
    {
        for (auto& object : m_objects)
        {
            object->m_component_map.clear();
        }
        m_objects.clear();
        m_object_name_map.clear();
        m_object_id_map.clear();
    }

    SPtr<Object> ObjectManager::AddObject(const String& name)
    {
        SPtr<Object> object = CreateRawObject(name);
        object->SetManager(this);
        object->SetManager(m_space->GetComponentManager().get());
        m_object_name_map.emplace(name, object);
        m_object_id_map.emplace(object->m_uuid, object);
        return object;
    }

    SPtr<Object> ObjectManager::CloneObject(SPtr<Object> source)
    {
        SPtr<Object> cloned_object = AddObject(source->GetName());
        for (auto& component : source->m_components)
        {
            cloned_object->AddComponent(m_space->GetComponentManager()->Clone(component, cloned_object));
        }

        return cloned_object;
    }

    void ObjectManager::FindObjects(const String& name, std::vector<SPtr<Object>>& objects)
    {
        auto [fst, snd] = m_object_name_map.equal_range(name);
        for (auto& founds = fst; founds != snd; ++founds)
        {
            objects.push_back(founds->second);
        }
    }

    SPtr<Object> ObjectManager::FindObjectFirst(const String& name)
    {
        auto [fst, snd] = m_object_name_map.equal_range(name);
        if (fst != m_object_name_map.end())
        {
            return fst->second;
        }
        return nullptr;
    }

    SPtr<Object> ObjectManager::FindObjectLast(const String& name)
    {
        auto [fst, snd] = m_object_name_map.equal_range(name);
        if (fst != snd)
        {
            auto  it     = std::distance(fst, snd);
            auto& result = fst;
            std::advance(result, it - 1);
            return result->second;
        }
        return nullptr;
    }

    SPtr<Object> ObjectManager::FindObjectAt(size_t index)
    {
        return m_objects.at(index);
    }

    SPtr<Object> ObjectManager::FindObjectUUID(size_t uuid)
    {
        auto found = m_object_id_map.find(uuid);
        if (found != m_object_id_map.end())
            return found->second;

        return nullptr;
    }

    void ObjectManager::RemoveObjects(const String& name)
    {
        std::vector<size_t> remove_ids;

        auto [fst, snd] = m_object_name_map.equal_range(name);
        for (auto& it = fst; it != snd;)
        {
            remove_ids.push_back(it->second->m_uuid);
        }
        for (size_t uuid : remove_ids)
        {
            RemoveObject(uuid);
        }
        remove_ids.clear();
    }

    void ObjectManager::RemoveObject(const String& name)
    {
        auto found = m_object_name_map.find(name);
        if (found != m_object_name_map.end())
        {
            RemoveObject(found->second->m_uuid);
        }
    }

    void ObjectManager::RemoveObjectAt(size_t index)
    {
        RemoveObject(m_objects.at(index)->m_uuid);
    }

    void ObjectManager::RemoveObject(SPtr<Object> object)
    {
        RemoveObject(object->m_uuid);
    }

    void ObjectManager::RemoveObject(size_t uuid)
    {
        auto found = m_object_id_map.find(uuid);
        if (found != m_object_id_map.end())
        {
            SPtr<Object> object = found->second;
            m_object_id_map.erase(found);

            auto [fst, snd] = m_object_name_map.equal_range(object->m_name);
            for (auto& it = fst; it != snd; ++it)
            {
                if (it->second->m_uuid == uuid)
                {
                    m_object_name_map.erase(it);
                    break;
                }
            }

            size_t index = object->m_index;
            m_objects.erase(m_objects.begin() + index);
            ResetIndex(index);
            object->ClearComponents();
        }
    }

    void ObjectManager::ClearObjects()
    {
        for (auto& object : m_objects)
        {
            object->ClearComponents();
        }
        m_objects.clear();
        m_object_name_map.clear();
        m_object_id_map.clear();
    }

    bool ObjectManager::HasObject(const String& name)
    {
        auto result = m_object_name_map.find(name);
        return result != m_object_name_map.end();
    }

    bool ObjectManager::HasObject(SPtr<Object> object)
    {
        auto found = m_object_id_map.find(object->m_uuid);
        return found != m_object_id_map.end();
    }

    void ObjectManager::ChangeName(SPtr<Object> object, const String& new_name)
    {
        auto [fst, snd] = m_object_name_map.equal_range(object->m_name);

        for (auto& it = fst; it != snd; ++it)
        {
            if (it->second->m_uuid == object->m_uuid)
            {
                m_object_name_map.erase(it);
                m_object_name_map.emplace(new_name, object);
                object->m_name = new_name;
                break;
            }
        }
    }

    SPtr<Object> ObjectManager::CreateRawObject(const String& name)
    {
        size_t index  = m_objects.size();
        auto   object = std::make_shared<Object>();
        m_objects.push_back(object);

        object->m_name  = name;
        object->m_index = index;
        object->m_uuid  = UUIDUtility::Generate();

        return object;
    }

    void ObjectManager::ResetIndex(size_t index) const
    {
        size_t size = m_objects.size();
        for (size_t i = index; i < size; ++i)
        {
            m_objects[i]->m_index = i;
        }
    }
}
