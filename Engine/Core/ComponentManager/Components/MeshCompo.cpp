#include "MeshCompo.hpp"

#include "../../ResourceManager/ResourceType/JsonData.hpp"

namespace Engine
{
    MeshCompo::MeshCompo()
    {
    }

    MeshCompo::~MeshCompo()
    {
    }

    void MeshCompo::Initialize()
    {
    }

    void MeshCompo::Update(Real dt)
    {
    }

    void MeshCompo::Shutdown()
    {
    }

    bool MeshCompo::Load(const Json::Value& data)
    {
        if (data.isNull())
            return false;

        return true;
    }

    void MeshCompo::Save(Json::Value& data) const
    {
    }

    void MeshCompo::Edit(CommandRegistry* command_registry)
    {
    }

    void MeshCompo::CloneTo(RPtr<Component> destination)
    {
    }

    void MeshCompo::Subscribe()
    {
    }

    void MeshCompo::Unsubscribe()
    {
    }

    MeshFactory::MeshFactory()
    {
        m_type_name = typeid(MeshCompo).name();
        m_type_id   = typeid(MeshCompo).hash_code();
    }

    MeshFactory::~MeshFactory()
    {
    }

    RPtr<Component> MeshFactory::Create()
    {
        return new MeshCompo();
    }
}
