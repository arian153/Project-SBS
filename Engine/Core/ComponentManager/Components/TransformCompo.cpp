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
        Subscribe();
    }

    void TransformCompo::Update(Real dt)
    {
    }

    void TransformCompo::Shutdown()
    {
        Unsubscribe();
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

    Matrix44 TransformCompo::GetLocalToWorldMatrix() const
    {
        return m_transform.LocalToWorldMatrix();
    }

    Matrix44 TransformCompo::GetWorldToLocalMatrix() const
    {
        return m_transform.LocalToWorldMatrix().Inverse();
    }

    void TransformCompo::SetTransform(const Transform& transform)
    {
        m_transform = transform;
    }

    void TransformCompo::SetScale(const Vector3& scale)
    {
        m_transform.scale = scale;
    }

    void TransformCompo::SetPosition(const Vector3& pos)
    {
        m_transform.position = pos;
    }

    void TransformCompo::SetOrientation(const Quaternion& orientation)
    {
        m_transform.orientation = orientation;
    }

    Vector3 TransformCompo::GetPosition() const
    {
        return m_transform.position;
    }

    Quaternion TransformCompo::GetOrientation() const
    {
        return m_transform.orientation;
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
