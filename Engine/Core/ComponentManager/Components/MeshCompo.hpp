#pragma once

#include "../Component.hpp"
#include "../ComponentFactory.hpp"

namespace Engine
{
    class MeshCompo final : public Component
    {
    public:
        MeshCompo();
        ~MeshCompo() override;

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
        friend class MeshFactory;

    private:
    };

    class MeshFactory final : public ComponentFactory
    {
    public:
        MeshFactory();
        ~MeshFactory() override;

        RPtr<Component> Create() override;
    };
}
