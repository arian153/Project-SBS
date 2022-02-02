namespace Engine
{
    template <typename T>
    RPtr<T> Object::AddComponent()
    {
        auto type = typeid(T).name();
        auto found = m_component_map.find(type);
        if (found == m_component_map.end())
        {
            auto created = m_component_manager->Create(type, shared_from_this());
            m_component_map.emplace(type, created);
            m_components.push_back(created);
            created->Initialize();
            return static_cast<T*>(created);
        }

        return nullptr;
    }

    template <typename T>
    bool Object::HasComponent() const
    {
        return m_component_map.find(typeid(T).name()) != m_component_map.end();
    }

    template <typename T>
    RPtr<T> Object::GetComponent() const
    {
        auto type = typeid(T).name();
        auto found = m_component_map.find(type);

        if (found != m_component_map.end())
            return static_cast<T*>(found->second);

        return nullptr;
    }

    template <typename T>
    void Object::RemoveComponent()
    {
        auto type = typeid(T).name();
        auto found = m_component_map.find(type);

        if (found != m_component_map.end())
        {
            m_components.erase(std::find(m_components.begin(), m_components.end(), found->second));
            m_component_manager->Remove(found->second, shared_from_this());
            m_component_map.erase(found);
        }
    }
}