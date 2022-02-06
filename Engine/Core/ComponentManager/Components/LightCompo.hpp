#pragma once
#include "../Component.hpp"
#include "../ComponentFactory.hpp"
#include "../../../Graphics/Element/Light.hpp"

namespace Engine
{
    class LightCompo final : public Component
    {
    public:
        LightCompo();
        ~LightCompo() override;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void CloneTo(RPtr<Component> destination) override;

        const LightInfo& GetLightInfo() const;
        LightInfo& GetLightInfo();

        void SetLightDirection(const Vector3& direction);
        void SetDiffuse(const Color& diffuse);
        void SetAmbient(const Color& ambient);
        void SetSpecular(const Color& specular);
        void SetLightType(eLightType type);
        void SetLightRange(float range);
        void SetLightAngle(float angle);

    protected:
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class LightFactory;

    private:
        LightInfo m_light_info;
    };

    class LightFactory final : public ComponentFactory
    {
    public:
        LightFactory();
        ~LightFactory() override;

        RPtr<Component> Create() override;
    };
}
