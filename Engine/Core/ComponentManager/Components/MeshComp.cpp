#include "MeshComp.hpp"

#include "../../ResourceManager/ResourceType/JsonData.hpp"

namespace Engine
{
    MeshComp::MeshComp()
    {
    }

    MeshComp::~MeshComp()
    {
    }

    void MeshComp::Initialize()
    {
    }

    void MeshComp::Update(Real dt)
    {
    }

    void MeshComp::Shutdown()
    {
    }

    bool MeshComp::Load(const Json::Value& data)
    {
        if (data.isNull())
            return false;

        return true;
    }

    void MeshComp::Save(Json::Value& data) const
    {
    }

    void MeshComp::Edit(CommandRegistry* command_registry)
    {
    }

    void MeshComp::CloneTo(Component* destination)
    {
    }

    void MeshComp::Subscribe()
    {
    }

    void MeshComp::Unsubscribe()
    {
    }

    MeshFactory::MeshFactory()
    {
        m_type = typeid(MeshComp).name();
        m_id   = typeid(MeshComp).hash_code();
    }

    MeshFactory::~MeshFactory()
    {
    }

    Component* MeshFactory::Create()
    {
        return new MeshComp();
    }
}
