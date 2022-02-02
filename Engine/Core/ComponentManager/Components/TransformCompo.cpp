#include "TransformCompo.hpp"

#include "../../AppStateManager/AppState.hpp"
#include "../../ResourceManager/ResourceType/JsonData.hpp"

namespace Engine
{
    TransformCompo::TransformCompo()
    {
    }

    TransformCompo::~TransformCompo()
    {
    }

    void TransformCompo::Initialize()
    {
    }

    void TransformCompo::Update(Real dt)
    {
    }

    void TransformCompo::Shutdown()
    {
    }

    bool TransformCompo::Load(const Json::Value& data)
    {
        if (data.isNull())
            return false;

        return true;
    }

    void TransformCompo::Save(Json::Value& data) const
    {
    }

    void TransformCompo::Edit(CommandRegistry* command_registry)
    {
    }

    void TransformCompo::CloneTo(RPtr<Component> destination)
    {
    }

    void TransformCompo::Subscribe()
    {
        m_space->GetBehaviorSubsystem()->AddTransform(this);
    }

    void TransformCompo::Unsubscribe()
    {
        m_space->GetBehaviorSubsystem()->RemoveTransform(this);
    }

    TransformFactory::TransformFactory()
    {
        m_type_name = typeid(TransformCompo).name();
        m_type_id   = typeid(TransformCompo).hash_code();
    }

    TransformFactory::~TransformFactory()
    {
    }

    RPtr<Component> TransformFactory::Create()
    {
        return new TransformCompo();
    }
}
