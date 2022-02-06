#pragma once
#include "../Component.hpp"
#include "../ComponentFactory.hpp"

namespace Engine
{
    class OrbitCameraCompo final : public Component
    {
    public:
        OrbitCameraCompo();
        ~OrbitCameraCompo() override;

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
        friend class OrbitCameraFactory;

    private:
    };

    class OrbitCameraFactory final : public ComponentFactory
    {
    public:
        OrbitCameraFactory();
        ~OrbitCameraFactory() override;

        RPtr<Component> Create() override;
    };
}

