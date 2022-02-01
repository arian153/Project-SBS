#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "../../EngineDefine.hpp"

namespace Json
{
    class Value;
}

namespace Engine
{
    class ComponentManager;
    class ObjectFactory;
    class ObjectManager;
    class Component;

    class Object : public std::enable_shared_from_this<Object>
    {
    public:
        Object();
        ~Object();

        size_t GetID() const;

        String GetName() const;
        void   SetName(const String& name);

        SPtr<Object> Clone();

        void ClearComponents();

        bool Load(const Json::Value& data);
        bool Save(Json::Value& data) const;

    public: //template member functions
        template <typename T>
        T* AddComponent();

        template <typename T>
        bool HasComponent() const;

        template <typename T>
        T* GetComponent() const;

        template <typename T>
        void RemoveComponent();

    private:
        void SetManager(ObjectManager* object_manager);
        void SetManager(ComponentManager* component_manager);

        Component* AddComponent(Component* component);
        Component* AddComponent(const String& type);
        Component* GetComponent(const String& type);

        bool HasComponent(const String& type);
        void RemoveComponent(Component* component);
        void RemoveComponent(const String& type);

    private:
        friend class ComponentManager;
        friend class ObjectManager;

    private:
        //identifier
        size_t m_index = std::numeric_limits<size_t>::max();
        size_t m_uuid  = 0;
        String m_name;

        //components
        HashMap<String, Component*> m_component_map;
        std::vector<Component*>     m_components;

        //others
        ObjectManager*    m_object_manager    = nullptr;
        ComponentManager* m_component_manager = nullptr;
    };
}

// ReSharper disable once CppUnusedIncludeDirective
#include "ObjectTemplate.cpp"
