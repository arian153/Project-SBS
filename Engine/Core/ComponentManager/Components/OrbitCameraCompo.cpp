#include "OrbitCameraCompo.hpp"

#include "../../../External/JSONCPP/json/json.h"

namespace Engine
{
    OrbitCameraCompo::OrbitCameraCompo()
    {
    }

    OrbitCameraCompo::~OrbitCameraCompo()
    {
    }

    void OrbitCameraCompo::Initialize()
    {
        Subscribe();
    }

    void OrbitCameraCompo::Update(Real dt)
    {
    }

    void OrbitCameraCompo::Shutdown()
    {
        Unsubscribe();
    }

    bool OrbitCameraCompo::Load(const Json::Value& data)
    {
        if (data.isNull())
            return false;

        return true;
    }

    void OrbitCameraCompo::Save(Json::Value& data) const
    {
    }

    void OrbitCameraCompo::Edit(CommandRegistry* command_registry)
    {
    }

    void OrbitCameraCompo::CloneTo(RPtr<Component> destination)
    {
    }

    void OrbitCameraCompo::Subscribe()
    {
    }

    void OrbitCameraCompo::Unsubscribe()
    {
    }

    OrbitCameraFactory::OrbitCameraFactory()
    {
        m_type_name = typeid(OrbitCameraCompo).name();
        m_type_id   = typeid(OrbitCameraCompo).hash_code();
    }

    OrbitCameraFactory::~OrbitCameraFactory()
    {
    }

    RPtr<Component> OrbitCameraFactory::Create()
    {
        return new OrbitCameraCompo();
    }
}
