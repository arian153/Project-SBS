#pragma once
#include "../Component.hpp"
#include "../ComponentFactory.hpp"
#include "../../../Math/Algebra/Vector3.hpp"
#include "../../../Math/Utility/MathDef.hpp"

namespace Engine
{
    class Camera;

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

        void SetAsMainCamera() const;
    protected:
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class OrbitCameraFactory;

    private:
        SPtr<Camera> m_camera = nullptr;

        Real m_theta  = -Math::HALF_PI;
        Real m_phi    = Math::HALF_PI;
        Real m_radius = 10.0f;

        Vector3 m_eye_pos;
        Vector3 m_target_pos;
    };

    class OrbitCameraFactory final : public ComponentFactory
    {
    public:
        OrbitCameraFactory();
        ~OrbitCameraFactory() override;

        RPtr<Component> Create() override;
    };
}
