#pragma once
#include "../Component.hpp"
#include "../ComponentFactory.hpp"
#include "../../../Math/Structure/Transform.hpp"

namespace Engine
{
    class TransformCompo final : public Component
    {
    protected:
        TransformCompo();
    public:
        ~TransformCompo() override;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void CloneTo(Component* destination) override;

    protected:
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class TransformFactory;

    private:
        RPtr<Transform> m_transform = nullptr;
    };

    class TransformFactory final : public ComponentFactory
    {
    public:
        TransformFactory();
        ~TransformFactory() override;

        Component* Create() override;
    };
}
