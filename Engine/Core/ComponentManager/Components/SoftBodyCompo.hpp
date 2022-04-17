/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

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

        void CreateSampleCloth(size_t w, size_t h, bool is_fixed);
        void CreateSampleSphere(bool is_fixed);
        void CreateSampleBox();
        void ApplyForce(const Vector3& force);

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
        Real     m_scale            = 10.0f;
        int      m_integration_type = 0;
    };

    class SoftBodyFactory final : public ComponentFactory
    {
    public:
        SoftBodyFactory();
        ~SoftBodyFactory() override;

        RPtr<Component> Create() override;
    };
}
