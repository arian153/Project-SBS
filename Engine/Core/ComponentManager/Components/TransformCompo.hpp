#pragma once
#include "../Component.hpp"
#include "../ComponentFactory.hpp"
#include "../../../Math/Structure/Transform.hpp"

namespace Engine
{
    class TransformCompo final : public Component
    {
    public:
        TransformCompo();
        ~TransformCompo() override;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void CloneTo(RPtr<Component> destination) override;

        Matrix44 GetLocalToWorldMatrix() const;
        Matrix44 GetWorldToLocalMatrix() const;

        Transform GetTransform() const;

        void SetTransform(const Transform& transform);

        void SetScale(const Vector3& scale);
        void SetPosition(const Vector3& pos);
        void SetOrientation(const Quaternion& orientation);

        Vector3    GetPosition() const;
        Quaternion GetOrientation() const;


    protected:
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class TransformFactory;

    private:
        Transform m_transform;
    };

    class TransformFactory final : public ComponentFactory
    {
    public:
        TransformFactory();
        ~TransformFactory() override;

        RPtr<Component> Create() override;
    };
}
