#pragma once
#include <unordered_map>

#include "../../EngineDefine.hpp"

namespace Engine
{
    class AppState;
    class ComponentManager;
    class Object;

    class ObjectManager
    {
    public:
        ObjectManager();
        ~ObjectManager();

        void Initialize(AppState* space);
        void Shutdown();

        SPtr<Object> AddObject(const String& name);
        SPtr<Object> CloneObject(SPtr<Object> source);

        void         FindObjects(const String& name, std::vector<SPtr<Object>>& objects);
        SPtr<Object> FindObjectFirst(const String& name);
        SPtr<Object> FindObjectLast(const String& name);
        SPtr<Object> FindObjectAt(size_t index);
        SPtr<Object> FindObjectUUID(size_t uuid);

        void RemoveObjects(const String& name);
        void RemoveObject(const String& name);
        void RemoveObjectAt(size_t index);
        void RemoveObject(SPtr<Object> object);
        void RemoveObject(size_t uuid);

        void ClearObjects();

        bool HasObject(const String& name);
        bool HasObject(SPtr<Object> object);

        void ChangeName(SPtr<Object> object, const String& new_name);

    private:
        SPtr<Object> CreateRawObject(const String& name);
        void         ResetIndex(size_t index = 0) const;

    private:
        friend class Object;

    private:
        HashMulMap<String, SPtr<Object>> m_object_name_map;
        HashMap<size_t, SPtr<Object>>    m_object_id_map;
        std::vector<SPtr<Object>>        m_objects;

        AppState* m_space = nullptr;
    };
}
