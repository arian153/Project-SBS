#include "ComponentRegistry.hpp"
#include "ComponentFactory.hpp"

//factories
#include "Components/LightCompo.hpp"
#include "Components/MeshCompo.hpp"
#include "Components/OrbitCameraCompo.hpp"
#include "Components/TransformCompo.hpp"
#include "Components/RigidBodyCompo.hpp"

namespace Engine
{
    bool ComponentRegistry::Initialize()
    {
        AddFactory(new TransformFactory());
        AddFactory(new MeshFactory());
        AddFactory(new OrbitCameraFactory());
        AddFactory(new LightFactory());
        AddFactory(new RigidBodyFactory());

        return true;
    }

    bool ComponentRegistry::Shutdown()
    {
        for (auto it = m_factories_name.begin(); it != m_factories_name.end(); ++it)
        {
            delete it->second;
            it->second = nullptr;
        }
        m_keys.clear();
        m_factories_name.clear();
        m_factories_uuid.clear();
        return true;
    }

    bool ComponentRegistry::AddFactory(ComponentFactory* factory)
    {
        if (factory != nullptr)
        {
            if (m_factories_name.find(factory->m_type_name) == m_factories_name.end())
            {
                m_keys.push_back(factory->m_type_name);
                m_factories_uuid.emplace(factory->m_type_id, factory);
                m_factories_name.emplace(factory->m_type_name, factory);
                return true;
            }
        }
        return false;
    }
}
