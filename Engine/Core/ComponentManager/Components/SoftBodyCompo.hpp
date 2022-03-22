#pragma once
#include "../Component.hpp"
#include "../ComponentFactory.hpp"
#include "../../../Physics/Dynamics/RigidBody.hpp"
#include "../../../Physics/Dynamics/SoftBody.hpp"

namespace Engine
{
    class SoftBodyCompo final : public Component
    {
    public:
        SoftBodyCompo();
        ~SoftBodyCompo() override;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void CloneTo(RPtr<Component> destination) override;

        void CreateSample(size_t w, size_t h);

        const MeshData& GetSoftBodyMeshData() const;
        MeshData&       GetSoftBodyMeshData();

    protected:
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class SoftBodyFactory;
        friend class PhysicsSubsystem;

    private:
        SoftBody m_body;
        Real     m_scale = 100.0f;
    };

    class SoftBodyFactory final : public ComponentFactory
    {
    public:
        SoftBodyFactory();
        ~SoftBodyFactory() override;

        RPtr<Component> Create() override;
    };
}
