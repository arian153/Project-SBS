#pragma once
#include "../Component.hpp"
#include "../ComponentFactory.hpp"
#include "../../../Physics/Dynamics/RigidBody.hpp"

namespace Engine
{
    class RigidBodyCompo final : public Component
    {
    public:
        RigidBodyCompo();
        ~RigidBodyCompo() override;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void CloneTo(RPtr<Component> destination) override;

    protected:
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class RigidBodyFactory;

    private:
        RigidBody m_body;
    };

    class RigidBodyFactory final : public ComponentFactory
    {
    public:
        RigidBodyFactory();
        ~RigidBodyFactory() override;

        RPtr<Component> Create() override;
    };
}
